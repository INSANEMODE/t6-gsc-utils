#include <stdinc.hpp>
#include "loader/component_loader.hpp"

#include "game/structs.hpp"
#include "game/game.hpp"

#include "gsc.hpp"
#include "json.hpp"
#include "debug.hpp"

#include "scheduler.hpp"
#include "scripting.hpp"
#include "command.hpp"

#include <utils/hook.hpp>
#include <utils/string.hpp>

namespace gsc
{
    namespace
    {
        std::unordered_map<unsigned int, std::pair<std::string, script_function>> functions;
        std::unordered_map<unsigned int, std::pair<std::string, script_method>> methods;

        utils::hook::detour scr_get_common_function_hook;
        utils::hook::detour player_get_method_hook;

        auto field_offset_start = 0xA000;

        struct entity_field
        {
            std::string name;
            std::function<scripting::script_value(unsigned int entnum)> getter;
            std::function<void(unsigned int entnum, scripting::script_value)> setter;
        };

        std::vector<std::function<void()>> post_load_callbacks;
        std::unordered_map<unsigned int, std::unordered_map<unsigned int, entity_field>> custom_fields;

        std::unordered_map<std::string, scripting::script_value> world;

        std::unordered_map<void*, const char*> hooked_builtins;

        utils::hook::detour scr_error_internal_hook;

        std::atomic_bool disable_longjmp_error = false;

        unsigned int find_function(const std::string& name)
        {
            for (const auto& function : functions)
            {
                if (function.second.first == name)
                {
                    return function.first;
                }
            }

            return 0;
        }

        unsigned int find_method(const std::string& name)
        {
            for (const auto& method : methods)
            {
                if (method.second.first == name)
                {
                    return method.first;
                }
            }

            return 0;
        }

        builtin_function scr_get_common_function(const char** pName, int* type, int* min_args, int* max_args)
        {
            const auto func = reinterpret_cast<builtin_function>(find_function(*pName));

            if (func == nullptr)
            {
                return scr_get_common_function_hook.invoke<builtin_function>(pName, type, min_args, max_args);
            }

            *type = 1;
            *min_args = 0;
            *max_args = 512;

            return func;
        }

        builtin_method player_get_method(const char** pName, int* min_args, int* max_args)
        {
            const auto method = reinterpret_cast<builtin_method>(find_method(*pName));

            if (method == nullptr)
            {
                return player_get_method_hook.invoke<builtin_method>(pName, min_args, max_args);
            }

            *min_args = 0;
            *max_args = 512;

            return method;
        }

        utils::hook::detour scr_get_object_field_hook;
        void scr_get_object_field_stub(unsigned int classnum, int entnum, unsigned int offset)
        {
            if (custom_fields[classnum].find(offset) == custom_fields[classnum].end())
            {
                return scr_get_object_field_hook.invoke<void>(classnum, entnum, offset);
            }

            const auto field = custom_fields[classnum][offset];

            try
            {
                const auto result = field.getter(entnum);
                return_value(result);
            }
            catch (const std::exception& e)
            {
                printf("******* script runtime error *******\n");
                printf("while getting field \"%s\": %s\n", field.name.data(), e.what());
                printf(debug::get_call_stack().data());
                printf("************************************\n");
            }
        }

        utils::hook::detour scr_set_object_field_hook;
        void scr_set_object_field_stub(unsigned int classnum, int entnum, unsigned int offset)
        {
            if (custom_fields[classnum].find(offset) == custom_fields[classnum].end())
            {
                return scr_set_object_field_hook.invoke<void>(classnum, entnum, offset);
            }

            const auto args = get_arguments();
            const auto field = custom_fields[classnum][offset];

            try
            {
                field.setter(entnum, args[0]);
            }
            catch (const std::exception& e)
            {
                printf("******* script runtime error *******\n");
                printf("while setting field \"%s\": %s\n", field.name.data(), e.what());
                printf(debug::get_call_stack().data());
                printf("************************************\n");
            }
        }

        utils::hook::detour scr_post_load_scripts_hook;
        void scr_post_load_scripts_stub()
        {
            for (const auto& callback : post_load_callbacks)
            {
                callback();
            }

            return scr_post_load_scripts_hook.invoke<void>();
        }

        void scr_error_internal_stub()
        {
            if (disable_longjmp_error)
            {
                throw;
            }
            else
            {
                scr_error_internal_hook.invoke<void>();
            }
        }

        __declspec(naked) void scr_error_internal_stub_1_mp()
        {
            __asm
            {
                mov eax, 0
                mov edx, eax
                imul edx, 0x54

                push 0x8F3F65
                retn
            }
        }

        __declspec(naked) void scr_error_internal_stub_1_zm()
        {
            __asm
            {
                mov eax, 0
                mov edx, eax
                imul edx, 0x54

                push 0x8F2CC5
                retn
            }
        }

        bool execute_hook(void* ptr)
        {
            const auto iter = hooked_builtins.find(ptr);
            if (iter == hooked_builtins.end())
            {
                return false;
            }

            scripting::function function(iter->second);
            function.call(*game::levelEntityId, get_arguments());

            return true;
        }
    }

    namespace function
    {
        void add(const std::string& name, const script_function& function)
        {
            const auto id = functions.size() + 1;
            functions[id] = std::make_pair(name, function);
        }
    }

    namespace method
    {
        void add(const std::string& name, const script_method& function)
        {
            const auto id = methods.size() + 1;
            methods[id] = std::make_pair(name, function);
        }
    }

    namespace field
    {
        void add(const classid classnum, const std::string& name,
            const std::function<scripting::script_value(unsigned int entnum)>& getter,
            const std::function<void(unsigned int entnum, const scripting::script_value&)>& setter)
        {
            const auto offset = field_offset_start++;
            custom_fields[classnum][offset] = {name, getter, setter};

            post_load_callbacks.push_back([classnum, name, offset]()
            {
                game::Scr_AddClassField(game::SCRIPTINSTANCE_SERVER, classnum, name.data(), offset);
            });
        }
    }

    void return_value(const scripting::script_value& value)
    {
        if (game::scr_VmPub->outparamcount)
        {
            game::Scr_ClearOutParams(game::SCRIPTINSTANCE_SERVER);
        }

        scripting::push_value(value);
    }

    std::vector<scripting::script_value> get_arguments()
    {
        std::vector<scripting::script_value> args;

        for (auto i = 0; static_cast<unsigned int>(i) < game::scr_VmPub->outparamcount; i++)
        {
            const auto value = game::scr_VmPub->top[-i];
            args.push_back(value);
        }

        return args;
    }

    function_args::function_args(std::vector<scripting::script_value> values)
        : values_(values)
    {
    }

    unsigned int function_args::size() const
    {
        return this->values_.size();
    }

    std::vector<scripting::script_value> function_args::get_raw() const
    {
        return this->values_;
    }

    scripting::value_wrap function_args::get(const int index) const
    {
        if (static_cast<unsigned int>(index) >= this->values_.size())
        {
            throw std::runtime_error(utils::string::va("parameter %d does not exist", index));
        }

        return {this->values_[index], index};
    }

    std::string find_builtin_name(void* function)
    {
        for (auto i = scripting::function_map.begin(); i != scripting::function_map.end(); ++i)
        {
            if (i->second.actionFunc == function)
            {
                return i->second.actionString;
            }
        }

        for (auto i = functions.begin(); i != functions.end(); ++i)
        {
            if (i->first == reinterpret_cast<unsigned int>(function))
            {
                return i->second.first;
            }
        }

        return {};
    }

    std::string find_builtin_method_name(void* function)
    {
        for (auto i = scripting::method_map.begin(); i != scripting::method_map.end(); ++i)
        {
            if (i->second.actionFunc == function)
            {
                return i->second.actionString;
            }
        }

        for (auto i = methods.begin(); i != methods.end(); ++i)
        {
            if (i->first == reinterpret_cast<unsigned int>(function))
            {
                return i->second.first;
            }
        }

        return {};
    }

    bool call_function(unsigned int ptr)
    {
        if (execute_hook(reinterpret_cast<void*>(ptr)))
        {
            return false;
        }

        const auto iter = functions.find(ptr);
        if (iter == functions.end())
        {
            return false;
        }

        try
        {
            const auto result = iter->second.second(get_arguments());
            return_value(result);
        }
        catch (const std::exception& e)
        {
            printf("******* script runtime error *******\n");
            printf("in call to builtin function \"%s\": %s\n", iter->second.first.data(), e.what());
            printf(debug::get_call_stack().data());
            printf("************************************\n");
        }

        return true;
    }

    bool call_method(unsigned int ptr, game::scr_entref_t entref)
    {
        const auto iter = methods.find(ptr);
        if (iter == methods.end())
        {
            return false;
        }

        try
        {
            const scripting::entity entity = game::Scr_GetEntityId(game::SCRIPTINSTANCE_SERVER, entref.entnum, entref.classnum, 0);
            const auto result = iter->second.second(entity, get_arguments());
            return_value(result);

        }
        catch (const std::exception& e)
        {
            printf("******* script runtime error *******\n");
            printf("in call to builtin method \"%s\": %s\n", iter->second.first.data(), e.what());
            printf(debug::get_call_stack().data());
            printf("************************************\n");
        }

        return true;
    }

    class component final : public component_interface
    {
    public:
        void post_unpack() override
        {
            scr_get_common_function_hook.create(SELECT(0x4B57B0, 0x4AD040), scr_get_common_function);
            player_get_method_hook.create(SELECT(0x432480, 0x6F2DB0), player_get_method);

            scr_get_object_field_hook.create(SELECT(0x573160, 0x4224E0), scr_get_object_field_stub);
            scr_set_object_field_hook.create(SELECT(0x5B9820, 0x43F2A0), scr_set_object_field_stub);
            scr_post_load_scripts_hook.create(SELECT(0x6B75B0, 0x492440), scr_post_load_scripts_stub);

            utils::hook::jump(SELECT(0x8F3F60, 0x8F2CC0), SELECT(scr_error_internal_stub_1_mp, scr_error_internal_stub_1_zm));
            scr_error_internal_hook.create(SELECT(0x8F3F60, 0x8F2CC0), scr_error_internal_stub);

            scripting::on_shutdown([&]
            {
                hooked_builtins.clear();
            });

            field::add(classid::entity, "eflags",
                [](unsigned int entnum) -> scripting::script_value
                {
                    const auto entity = &game::g_entities[entnum];
                    return entity->flags;
                },
                [](unsigned int entnum, const scripting::script_value& value)
                {
                    const auto entity = &game::g_entities[entnum];
                    entity->flags = value.as<int>();
                }
            );

            field::add(classid::entity, "eflags2",
                [](unsigned int entnum) -> scripting::script_value
                {
                    const auto entity = &game::g_entities[entnum];
                    return entity->eFlags2;
                },
                    [](unsigned int entnum, const scripting::script_value& value)
                {
                    const auto entity = &game::g_entities[entnum];
                    entity->eFlags2 = value.as<int>();
                }
            );

            field::add(classid::entity, "clientflags",
                [](unsigned int entnum) -> scripting::script_value
                {
                    const auto entity = &game::g_entities[entnum];
                    return entity->client->flags;
                },
                [](unsigned int entnum, const scripting::script_value& value)
                {
                    const auto entity = &game::g_entities[entnum];
                    entity->client->flags = value.as<int>();
                }
            );

            function::add("getfunction", [](const function_args& args) -> scripting::script_value
            {
                const auto filename = args[0].as<std::string>();
                const auto function = args[1].as<std::string>();

                if (scripting::script_function_table[filename].find(function) != scripting::script_function_table[filename].end())
                {
                    return scripting::function{scripting::script_function_table[filename][function]};
                }

                return {};
            });

            function::add("getfunctionname", [](const function_args& args)
            {
                const auto function = args[0].as<scripting::function>();
                return function.get_name();
            });
            
            function::add("getfunctionargcount", [](const function_args& args)
            {
                const auto function = args[0].as<scripting::function>();
                const auto pos = function.get_pos();
                if (*pos != 0x17) // OP_SafeCreateLocalVariables
                {
                    return 0;
                }

                return static_cast<int>(pos[1]);
            });

            function::add("arrayremovekey", [](const function_args& args) -> scripting::script_value
            {
                const auto array = args[0].as<scripting::array>();
                const auto key = args[1].as<std::string>();
                array.erase(key);
                return {};
            });

            function::add("xor", [](const function_args& args)
            {
                const auto a = args[0].as<int>();
                const auto b = args[1].as<int>();
                return a ^ b;
            });

            function::add("not", [](const function_args& args)
            {
                const auto a = args[0].as<int>();
                return ~a;
            });

            function::add("and", [](const function_args& args)
            {
                const auto a = args[0].as<int>();
                const auto b = args[1].as<int>();
                return a & b;
            });

            function::add("or", [](const function_args& args)
            {
                const auto a = args[0].as<int>();
                const auto b = args[1].as<int>();
                return a | b;
            });

            function::add("structget", [](const function_args& args)
            {
                const auto obj = args[0].as<scripting::object>();
                const auto key = args[1].as<std::string>();
                return obj[key];
            });

            function::add("structset", [](const function_args& args) -> scripting::script_value
            {
                const auto obj = args[0].as<scripting::object>();
                const auto key = args[1].as<std::string>();
                obj[key] = args[2];
                return {};
            });

            function::add("structremove", [](const function_args& args) -> scripting::script_value
            {
                const auto obj = args[0].as<scripting::object>();
                const auto key = args[1].as<std::string>();
                obj.erase(key);
                return {};
            });

            function::add("getstructkeys", [](const function_args& args) -> scripting::script_value
            {
                const auto obj = args[0].as<scripting::object>();
                const auto keys = obj.get_keys();
                scripting::array result;

                for (const auto& key : keys)
                {
                    result.push(key);
                }

                return result;
            });

            function::add("isfunctionptr", [](const function_args& args) -> scripting::script_value
            {
                return args[0].is<scripting::function>();
            });

            function::add("isentity", [](const function_args& args) -> scripting::script_value
            {
                const auto value = args[0].get_raw();
                const auto type = game::scr_VarGlob->objectVariableValue[value.u.uintValue].w.type & 0x7F;
                return value.type == game::SCRIPT_OBJECT && type == game::SCRIPT_ENTITY;
            });

            function::add("isstruct", [](const function_args& args)
            {
                return args[0].is<scripting::object>();
            });

            function::add("typeof", [](const function_args& args)
            {
                return args[0].type_name();
            });

            method::add("get", [](const scripting::entity& entity, const function_args& args)
            {
                const auto field = args[0].as<std::string>();
                return entity.get(field);
            });

            method::add("set", [](const scripting::entity& entity, const function_args& args) -> scripting::script_value
            {
                const auto field = args[0].as<std::string>();
                entity.set(field, args[1]);
                return {};
            });

            function::add("worldget", [](const function_args& args) -> scripting::script_value
            {
                const auto key = args[0].as<std::string>();
                return world[key];
            });

            function::add("worldset", [](const function_args& args) -> scripting::script_value
            {
                const auto key = args[0].as<std::string>();
                world[key] = args[1];
                return {};
            });

            function::add("invokefunc", [](const function_args& args) -> scripting::script_value
            {
                const auto name = utils::string::to_lower(args[0].as<std::string>());
                auto arguments = args.get_raw();
                arguments.erase(arguments.begin());
                disable_longjmp_error = true;
                const auto _0 = gsl::finally([&]
                {
                    *reinterpret_cast<const char**>(SELECT(0x2E27C70, 0x2DF7F70)) = nullptr;
                    disable_longjmp_error = false;
                });

                return scripting::call(name, arguments);
            });

            function::add("detourfunc", [](const function_args& args) -> scripting::script_value
            {
                const auto name = utils::string::to_lower(args[0].as<std::string>());
                const auto stub = args[1].as<scripting::function>();
                if (scripting::function_map.find(name) == scripting::function_map.end())
                {
                    throw std::runtime_error("function not found");
                }

                const auto func = scripting::function_map[name].actionFunc;
                hooked_builtins[func] = stub.get_pos();
                return {};
            });

            function::add("disabledetour", [](const function_args& args) -> scripting::script_value
            {
                const auto name = utils::string::to_lower(args[0].as<std::string>());
                if (scripting::function_map.find(name) == scripting::function_map.end())
                {
                    throw std::runtime_error("function not found");
                }

                const auto func = scripting::function_map[name].actionFunc;
                hooked_builtins.erase(func);
                return {};
            });
        }
    };
}

REGISTER_COMPONENT(gsc::component)
