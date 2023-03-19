#include <stdinc.hpp>
#include "loader/component_loader.hpp"

#include "gsc.hpp"

#include <json.hpp>
#include <utils/io.hpp>
#include <utils/hook.hpp>

namespace players
{
	namespace
	{
		utils::hook::detour pm_air_move_hook;

		float jump_height = 39.0f;

		void jump_height_override(game::playerState_s* ps)
		{
			const auto id = game::Scr_GetEntityId(game::SCRIPTINSTANCE_SERVER, ps->clientNum, 0, 0);
			const scripting::entity player(id);
			const auto script_jump_height = player.get("jumpheightoverride");
			//printf("ps->clientnum = %d\n", ps->clientNum);
			if (script_jump_height.is<float>())
			{
				jump_height = script_jump_height.as<float>();
			}
			else
			{
				jump_height = 39.0f;
			}
		}

		void __declspec(naked) plutonium_jump_height_stub()
		{
			__asm
			{
				push esi;
				call jump_height_override;
				add esp, 4;
				movss xmm1, jump_height;
				push 0x6B1D8B;
				retn;
			}
		}

		void mid_air_jumping_stub(game::pmove_t* pm, game::pml_t* pml)
		{
			const auto id = game::Scr_GetEntityId(game::SCRIPTINSTANCE_SERVER, pm->ps->clientNum, 0, 0);
			const scripting::entity player(id);
			const auto can_multi_jump = player.get("canmultijump");
			if (can_multi_jump.is<bool>() && can_multi_jump.as<bool>())
			{
				game::Jump_Check(pm, pml);
			}
			pm_air_move_hook.invoke<void>(pm, pml);
		}
	}

	class component final : public component_interface
	{
	public:
		void post_unpack() override
		{
			//Hook to allow per player jump height
			utils::hook::jump(0x6B1D83, plutonium_jump_height_stub);

			//Need to change this to check if the player can multi jump instead of just jumping over the if statement in Jump_Check
			//utils::hook::jump(0x6B1D4B, reinterpret_cast<void*>(0x6B1D83));

			//Add ability to jump in midair if player has canmultijump in GSC
			pm_air_move_hook.create(0x943EC0, mid_air_jumping_stub);
		}
	};
}

REGISTER_COMPONENT(players::component)
