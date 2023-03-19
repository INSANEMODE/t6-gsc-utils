#pragma once

namespace game
{
	typedef float vec_t;
	typedef vec_t vec2_t[2];
	typedef vec_t vec3_t[3];
	typedef vec_t vec4_t[4];

	enum scriptInstance_t
	{
		SCRIPTINSTANCE_SERVER,
		SCRIPTINSTANCE_CLIENT,
	};

	enum netsrc_t
	{
		NS_CLIENT1 = 0x0,
		NS_CLIENT2 = 0x1,
		NS_CLIENT3 = 0x2,
		NS_CLIENT4 = 0x3,
		NS_SERVER = 0x4,
		NS_PACKET = 0x5,
		NS_NULL = -1,
	};

	enum netadrtype_t
	{
		NA_BOT = 0x0,
		NA_BAD = 0x1,
		NA_LOOPBACK = 0x2,
		NA_BROADCAST = 0x3,
		NA_IP = 0x4,
	};

	enum ClientState_t
	{
		CS_FREE,
		CS_ZOMBIE,
		CS_RECONNECTING,
		CS_CONNECTED,
		CS_CLIENTLOADING,
		CS_ACTIVE,
	};

	enum team_t
	{
		TEAM_FREE = 0x0,
		TEAM_BAD = 0x0,
		TEAM_ALLIES = 0x1,
		TEAM_AXIS = 0x2,
		TEAM_THREE = 0x3,
		TEAM_FOUR = 0x4,
		TEAM_FIVE = 0x5,
		TEAM_SIX = 0x6,
		TEAM_SEVEN = 0x7,
		TEAM_EIGHT = 0x8,
		TEAM_NUM_PLAYING_TEAMS = 0x9,
		TEAM_SPECTATOR = 0x9,
		TEAM_NUM_TEAMS = 0xA,
		TEAM_LOCALPLAYERS = 0xB,
		TEAM_FIRST_PLAYING_TEAM = 0x1,
		TEAM_LAST_PLAYING_TEAM = 0x8,
	};

	enum ffa_team_t
	{
		TEAM_FFA_NONE = 0x0,
		TEAM_FFA_AXIS = 0x1,
		TEAM_FFA_ALLIES = 0x2,
		TEAM_FFA_THREE = 0x3,
	};

	struct scr_entref_t
	{
		unsigned short entnum;
		unsigned short classnum;
		int client;
	};

	struct BuiltinMethodDef
	{
		const char* actionString;
		unsigned int constId;
		int min_args;
		int max_args;
		void(__cdecl* actionFunc)(scr_entref_t);
		int type;
	};

	struct BuiltinFunctionDef
	{
		const char* actionString;
		unsigned int constId;
		int min_args;
		int max_args;
		void(__cdecl* actionFunc)();
		int type;
	};

	enum scriptType_e
	{
		SCRIPT_NONE = 0,
		SCRIPT_OBJECT = 1,
		SCRIPT_STRING = 2,
		SCRIPT_ISTRING = 3,
		SCRIPT_VECTOR = 4,
		SCRIPT_FLOAT = 6,
		SCRIPT_INTEGER = 7,
		SCRIPT_CODEPOS = 8,
		SCRIPT_END = 9,
		SCRIPT_FUNCTION = 10,
		SCRIPT_THREAD = 0xE,
		SCRIPT_NOTIFY_THREAD = 0xF,
		SCRIPT_TIME_THREAD = 0x10,
		SCRIPT_STRUCT = 18,
		SCRIPT_ENTITY = 20,
		SCRIPT_ARRAY = 21,
		SCRIPT_FREE = 0x17,
	};

	struct VariableStackBuffer
	{
		const char* pos;
		unsigned __int16 size;
		unsigned __int16 bufLen;
		unsigned __int16 localId;
		char time;
		char buf[1];
	};

	union VariableUnion
	{
		int intValue;
		unsigned int uintValue;
		float floatValue;
		unsigned int stringValue;
		const float* vectorValue;
		const char* codePosValue;
		unsigned int pointerValue;
		VariableStackBuffer* stackValue;
		unsigned int entityOffset;
	};

	struct VariableValue
	{
		int type;
		VariableUnion u;
	};

	struct function_stack_t
	{
		char* pos;
		VariableValue* top;
		unsigned int localId;
		unsigned int localVarCount;
		VariableValue* startTop;
	};

	struct function_frame_t
	{
		function_stack_t fs;
	};

	struct scrVmPub_t
	{
		unsigned int* localVars;
		VariableValue* maxstack;
		int function_count;
		function_frame_t* function_frame;
		VariableValue* top;
		bool abort_on_error;
		bool terminal_error;
		bool block_execution;
		unsigned int inparamcount;
		unsigned int outparamcount;
		function_frame_t function_frame_start[32];
		VariableValue stack[2048];
		void(__cdecl* notifyListeners[1])(unsigned int, unsigned int);
	};

	struct scrVarPub_t
	{
		const char* fieldBuffer;
		bool developer;
		bool developer_script;
		bool evaluate;
		const char* error_message;
		int error_index;
		unsigned int time;
		unsigned int timeArrayId;
		unsigned int pauseArrayId;
		unsigned int levelId;
		unsigned int gameId;
		unsigned int animId;
		unsigned int freeEntList;
		unsigned int tempVariable;
		bool bInited;
		unsigned __int16 savecount;
		unsigned int checksum;
		unsigned int entId;
		unsigned int entFieldName;
		void* programHunkUser;
		char* programBuffer;
		char* endScriptBuffer;
		unsigned __int16* saveIdMap;
		unsigned __int16* saveIdMapRev;
		unsigned int numScriptThreads;
		unsigned int numScriptValues;
		unsigned int numScriptObjects;
		char* varUsagePos;
		int ext_threadcount;
		int totalObjectRefCount;
		volatile int totalVectorRefCount;
		int allocationCount;
	};

	struct ObjectVariableChildren
	{
		unsigned int firstChild;
		unsigned int lastChild;
	};

	struct ObjectVariableValue_u_f
	{
		unsigned __int16 prev;
		unsigned __int16 next;
	};

	union ObjectVariableValue_u_o_u
	{
		unsigned __int16 size;
		unsigned __int16 entnum;
		unsigned __int16 nextEntId;
		unsigned __int16 self;
	};

	struct	ObjectVariableValue_u_o
	{
		unsigned __int16 refCount;
		ObjectVariableValue_u_o_u u;
	};

	union ObjectVariableValue_w
	{
		unsigned int type;
		unsigned int classnum;
		unsigned int notifyName;
		unsigned int waitTime;
		unsigned int parentLocalId;
	};

	struct ChildVariableValue_u_f
	{
		unsigned __int16 prev;
		unsigned __int16 next;
	};

	union ChildVariableValue_u
	{
		ChildVariableValue_u_f f;
		VariableUnion u;
	};

	struct ChildBucketMatchKeys_keys
	{
		unsigned __int16 name_hi;
		unsigned __int16 parentId;
	};

	union ChildBucketMatchKeys
	{
		ChildBucketMatchKeys_keys keys;
		unsigned int match;
	};

	struct	ChildVariableValue
	{
		ChildVariableValue_u u;
		unsigned int next;
		char pad[4];
		char type;
		char name_lo;
		char _pad[2];
		ChildBucketMatchKeys k;
		unsigned int nextSibling;
		unsigned int prevSibling;
	};

	union ObjectVariableValue_u
	{
		ObjectVariableValue_u_f f;
		ObjectVariableValue_u_o o;
	};

	struct ObjectVariableValue
	{
		ObjectVariableValue_u u;
		ObjectVariableValue_w w;
	};

	struct scrVarGlob_t
	{
		ObjectVariableValue* objectVariableValue;
		char __pad0[0x7C];
		ObjectVariableChildren* objectVariableChildren;
		char __pad1[0x7C];
		unsigned __int16* childVariableBucket;
		char __pad2[0x7C];
		ChildVariableValue* childVariableValue;
	};

	struct scr_classStruct_t
	{
		unsigned __int16 id;
		unsigned __int16 entArrayId;
		char charId;
		const char* name;
	};

	enum gclientFlag
	{
		NOCLIP = 1 << 0,
		UFO = 1 << 1,
	};

	enum entityFlag
	{
		FL_GODMODE = 0x1,
		FL_DEMI_GODMODE = 0x2,
		FL_NOTARGET = 0x4,
		FL_NO_KNOCKBACK = 0x8,
		FL_DROPPED_ITEM = 0x10,
		FL_NO_BOTS = 0x20,
		FL_NO_HUMANS = 0x40,
		FL_TOGGLE = 0x80,
		FL_SOFTACTIVATE = 0x100,
		FL_LOW_PRIORITY_USEABLE = 0x200,
		FL_NO_TACTICAL_INSERTION = 0x400,
		FL_DYNAMICPATH = 0x800,
		FL_SUPPORTS_LINKTO = 0x1000,
		FL_NO_AUTO_ANIM_UPDATE = 0x2000,
		FL_GRENADE_TOUCH_DAMAGE = 0x4000,
		FL_GRENADE_MARTYRDOM = 0x8000,
		FL_MISSILE_DESTABILIZED = 0x10000,
		FL_STABLE_MISSILES = 0x20000,
		FL_REPEAT_ANIM_UPDATE = 0x40000,
		FL_VEHICLE_TARGET = 0x80000,
		FL_GROUND_ENT = 0x100000,
		FL_CURSOR_HINT = 0x200000,
		FL_USE_TURRET = 0x400000,
		FL_MISSILE_ATTRACTOR = 0x800000,
		FL_TARGET = 0x1000000,
		FL_WEAPON_BEING_GRABBED = 0x2000000,
		FL_OBSTACLE = 0x4000000,
		FL_DODGE_LEFT = 0x8000000,
		FL_DODGE_RIGHT = 0x10000000,
		FL_BADPLACE_VOLUME = 0x20000000,
		FL_AUTO_BLOCKPATHS = 0x40000000,
		FL_MOVER_SLIDE = 0x80000000,
	};

	struct gclient_s
	{
		char __pad0[0x18];
		int eflags;
		char __pad1[0x5668];
		int flags;
	};

	struct EntHandle
	{
		unsigned __int16 number;
		unsigned __int16 infoIndex;
	};

	/* 2885 */
	struct SentientHandle
	{
		unsigned __int16 number;
		unsigned __int16 infoIndex;
	};

	/* 343 */
	enum TurretRotateState : __int32
	{
		TURRET_ROTATE_STOPPED = 0x0,
		TURRET_ROTATE_STOPPING = 0x1,
		TURRET_ROTATE_MOVING = 0x2,
	};

	struct TurretInfo
	{
		bool inuse;
		int state;
		int flags;
		int fireTime;
		EntHandle manualTarget;
		EntHandle target;
		vec3_t targetPos;
		int targetTime;
		vec3_t missOffsetNormalized;
		float arcmin[2];
		float arcmax[2];
		float initialYawmin;
		float initialYawmax;
		float forwardAngleDot;
		float dropPitch;
		float scanningPitch;
		int convergenceTime[2];
		int suppressTime;
		float maxRangeSquared;
		SentientHandle detachSentient;
		int stance;
		int prevStance;
		int fireSndDelay;
		float accuracy;
		vec3_t userOrigin;
		int prevSentTarget;
		float aiSpread;
		float playerSpread;
		team_t eTeam;
		float heatVal;
		bool overheating;
		int fireBarrel;
		float scanSpeed;
		float scanDecelYaw;
		int scanPauseTime;
		vec3_t originError;
		vec3_t anglesError;
		float pitchCap;
		int triggerDown;
		unsigned int fireSnd;
		unsigned int fireSndPlayer;
		unsigned int startFireSnd;
		unsigned int startFireSndPlayer;
		unsigned int loopFireEnd;
		unsigned int loopFireEndPlayer;
		unsigned int rotateLoopSnd;
		unsigned int rotateLoopSndPlayer;
		unsigned int rotateStopSnd;
		unsigned int rotateStopSndPlayer;
		int sndIsFiring;
		vec3_t targetOffset;
		float onTargetAngle;
		TurretRotateState turretRotateState;
		vec3_t previousAngles;
	};

	struct netadr_t
	{
		union
		{
			unsigned char ip[4];
			unsigned int inaddr;
		};
		unsigned __int16 port;
		netadrtype_t type;
		netsrc_t localNetID;
		unsigned __int16 serverID;
	};

	static_assert(sizeof(netadr_t) == 0x14);

	struct netProfileInfo_t
	{
		unsigned char __pad0[0x5E0];
	};

	struct netchan_t
	{
		int outgoingSequence;
		netsrc_t sock;
		int dropped;
		int incomingSequence;
		netadr_t remoteAddress;
		int qport;
		int fragmentSequence;
		int fragmentLength;
		unsigned char* fragmentBuffer;
		int fragmentBufferSize;
		int unsentFragments;
		int unsentOnLoan;
		int unsentFragmentStart;
		int unsentLength;
		unsigned char* unsentBuffer;
		int unsentBufferSize;
		int reliable_fragments;
		unsigned char fragment_send_count[128];
		unsigned int fragment_ack[4];
		int lowest_send_count;
		netProfileInfo_t prof;
	};

	static_assert(sizeof(netchan_t) == 0x6C8);

	struct PredictedVehicleDef
	{
		bool fullPhysics;
		vec3_t origin;
		vec3_t angles;
		vec3_t tVel;
		vec3_t aVel;
		int serverTime;
	};

	static_assert(sizeof(PredictedVehicleDef) == 0x38);

	struct clientHeader_t
	{
		ClientState_t state; //clientstate
		int sendAsActive;
		int deltaMessage;
		int rateDelayed;
		int hasAckedBaselineData;
		int hugeSnapshotSent;
		netchan_t netchan;
		vec3_t predictedOrigin;
		int predictedOriginServerTime;
		int migrationState;
		PredictedVehicleDef predictedVehicle;
	};

	static_assert(sizeof(clientHeader_t) == 0x72C);
	struct svscmd_info_t
	{
		char* cmd;
		int time;
		int type;
	};
	struct bitarray
	{
		int array[2];
	};
	union $60971AFD306BE716C67F4ED6B7576D56
	{
		float rollmove;
		__int16 damageKick[2];
	};
	struct $190F2CF944EC18EE3AF27F473C4F9DBE
	{
		unsigned __int32 weaponIdx : 8;
		unsigned __int32 attachment1 : 6;
		unsigned __int32 attachment2 : 6;
		unsigned __int32 attachment3 : 6;
		unsigned __int32 padding : 6;
	};

	union Weapon
	{
		$190F2CF944EC18EE3AF27F473C4F9DBE __s0;
		unsigned int weaponData;
	};

	struct usercmd_s
	{
		int serverTime;
		bitarray button_bits;
		int angles[3];
		Weapon weapon;
		Weapon offHandWeapon;
		Weapon lastWeaponAltModeSwitch;
		char forwardmove;
		char rightmove;
		char upmove;
		char pitchmove;
		char yawmove;
		__int16 gunPitch;
		__int16 gunYaw;
		__int16 wiiumoteAimX;
		__int16 wiiumoteAimY;
		char wiiuControllerType;
		unsigned __int16 meleeChargeEnt;
		char meleeChargeDist;
		$60971AFD306BE716C67F4ED6B7576D56 ___u18;
		char selectedLocation[2];
		char selectedYaw;
		unsigned __int16 airburstMarkDistance;
		unsigned __int16 lastInput;
	};
	struct clientSnapshot_t
	{
		//playerState_s ps;
		unsigned char __pad0[0x2A08];
		int entityCount;
		int clientCount;
		int actorCount;
		int matchStateIndex;
		int firstEntityIndex;
		int firstClientIndex;
		int firstActorIndex;
		int messageSent;
		int messageAcked;
		int messageSize;
		int serverTime;
		int physicsTime;
		int timeDelta;
		int baselineSnap;
	};

	struct cmd_function_t
	{
		cmd_function_t* next;
		const char* name;
		const char* autoCompleteDir;
		const char* autoCompleteExt;
		void(__cdecl* function)();
		int flags;
	};

	struct CmdArgs
	{
		int nesting;
		int localClientNum[8];
		int controllerIndex[8];
		void* itemDef[8];
		int argshift[8];
		int argc[8];
		const char** argv[8];
		char textPool[8192];
		const char* argvPool[512];
		int usedTextPool[8];
		int totalUsedArgvPool;
		int totalUsedTextPool;
	};

	enum dvarType_t
	{
		DVAR_TYPE_INVALID = 0x0,
		DVAR_TYPE_BOOL = 0x1,
		DVAR_TYPE_FLOAT = 0x2,
		DVAR_TYPE_FLOAT_2 = 0x3,
		DVAR_TYPE_FLOAT_3 = 0x4,
		DVAR_TYPE_FLOAT_4 = 0x5,
		DVAR_TYPE_INT = 0x6,
		DVAR_TYPE_ENUM = 0x7,
		DVAR_TYPE_STRING = 0x8,
		DVAR_TYPE_COLOR = 0x9,
		DVAR_TYPE_INT64 = 0xA,
		DVAR_TYPE_LINEAR_COLOR_RGB = 0xB,
		DVAR_TYPE_COLOR_XYZ = 0xC,
		DVAR_TYPE_COUNT = 0xD,
	};

	union DvarValue
	{
		bool enabled;
		int integer;
		unsigned int unsignedInt;
		__int64 integer64;
		unsigned __int64 unsignedInt64;
		float value; 
		vec4_t vector;
		const char* string;
		char color[4];
	};

	struct $A37BA207B3DDD6345C554D4661813EDD
	{
		int stringCount;
		const char* const* strings;
	};

	struct $9CA192F9DB66A3CB7E01DE78A0DEA53D
	{
		int min;
		int max;
	};

	struct $251C2428A496074035CACA7AAF3D55BD
	{
		float min;
		float max;
	};

	union DvarLimits
	{
		$A37BA207B3DDD6345C554D4661813EDD enumeration;
		$9CA192F9DB66A3CB7E01DE78A0DEA53D integer;
		$251C2428A496074035CACA7AAF3D55BD value;
		$251C2428A496074035CACA7AAF3D55BD vector;
	};

	struct dvar_t
	{
		const char* name;
		const char* description;
		int hash;
		unsigned int flags;
		dvarType_t type;
		bool modified;
		DvarValue current;
		DvarValue latched;
		DvarValue reset;
		DvarLimits domain;
		dvar_t* hashNext;
	};

	struct GSC_OBJ
	{
		char magic[8];
		unsigned int source_crc;
		unsigned int include_offset;
		unsigned int animtree_offset;
		unsigned int cseg_offset;
		unsigned int stringtablefixup_offset;
		unsigned int exports_offset;
		unsigned int imports_offset;
		unsigned int fixup_offset;
		unsigned int profile_offset;
		unsigned int cseg_size;
		unsigned __int16 name;
		unsigned __int16 stringtablefixup_count;
		unsigned __int16 exports_count;
		unsigned __int16 imports_count;
		unsigned __int16 fixup_count;
		unsigned __int16 profile_count;
		char include_count;
		char animtree_count;
		char flags;
	};

	struct GSC_EXPORT_ITEM
	{
		unsigned int checksum;
		unsigned int address;
		unsigned __int16 name;
		char param_count;
		char flags;
	};

	// gsc-tool
	enum class opcode : std::uint8_t
	{
		OP_End = 0x0,
		OP_Return = 0x1,
		OP_GetUndefined = 0x2,
		OP_GetZero = 0x3,
		OP_GetByte = 0x4,
		OP_GetNegByte = 0x5,
		OP_GetUnsignedShort = 0x6,
		OP_GetNegUnsignedShort = 0x7,
		OP_GetInteger = 0x8,
		OP_GetFloat = 0x9,
		OP_GetString = 0xA,
		OP_GetIString = 0xB,
		OP_GetVector = 0xC,
		OP_GetLevelObject = 0xD,
		OP_GetAnimObject = 0xE,
		OP_GetSelf = 0xF,
		OP_GetLevel = 0x10,
		OP_GetGame = 0x11,
		OP_GetAnim = 0x12,
		OP_GetAnimation = 0x13,
		OP_GetGameRef = 0x14,
		OP_GetFunction = 0x15,
		OP_CreateLocalVariable = 0x16,
		OP_SafeCreateLocalVariables = 0x17,
		OP_RemoveLocalVariables = 0x18,
		OP_EvalLocalVariableCached = 0x19,
		OP_EvalArray = 0x1A,
		OP_EvalLocalArrayRefCached = 0x1B,
		OP_EvalArrayRef = 0x1C,
		OP_ClearArray = 0x1D,
		OP_EmptyArray = 0x1E,
		OP_GetSelfObject = 0x1F,
		OP_EvalFieldVariable = 0x20,
		OP_EvalFieldVariableRef = 0x21,
		OP_ClearFieldVariable = 0x22,
		OP_SafeSetVariableFieldCached = 0x23,
		OP_SafeSetWaittillVariableFieldCached = 0x24,
		OP_ClearParams = 0x25,
		OP_CheckClearParams = 0x26,
		OP_EvalLocalVariableRefCached = 0x27,
		OP_SetVariableField = 0x28,
		OP_CallBuiltin = 0x29,
		OP_CallBuiltinMethod = 0x2A,
		OP_Wait = 0x2B,
		OP_WaitTillFrameEnd = 0x2C,
		OP_PreScriptCall = 0x2D,
		OP_ScriptFunctionCall = 0x2E,
		OP_ScriptFunctionCallPointer = 0x2F,
		OP_ScriptMethodCall = 0x30,
		OP_ScriptMethodCallPointer = 0x31,
		OP_ScriptThreadCall = 0x32,
		OP_ScriptThreadCallPointer = 0x33,
		OP_ScriptMethodThreadCall = 0x34,
		OP_ScriptMethodThreadCallPointer = 0x35,
		OP_DecTop = 0x36,
		OP_CastFieldObject = 0x37,
		OP_CastBool = 0x38,
		OP_BoolNot = 0x39,
		OP_BoolComplement = 0x3A,
		OP_JumpOnFalse = 0x3B,
		OP_JumpOnTrue = 0x3C,
		OP_JumpOnFalseExpr = 0x3D,
		OP_JumpOnTrueExpr = 0x3E,
		OP_Jump = 0x3F,
		OP_JumpBack = 0x40,
		OP_Inc = 0x41,
		OP_Dec = 0x42,
		OP_Bit_Or = 0x43,
		OP_Bit_Xor = 0x44,
		OP_Bit_And = 0x45,
		OP_Equal = 0x46,
		OP_NotEqual = 0x47,
		OP_LessThan = 0x48,
		OP_GreaterThan = 0x49,
		OP_LessThanOrEqualTo = 0x4A,
		OP_GreaterThanOrEqualTo = 0x4B,
		OP_ShiftLeft = 0x4C,
		OP_ShiftRight = 0x4D,
		OP_Plus = 0x4E,
		OP_Minus = 0x4F,
		OP_Multiply = 0x50,
		OP_Divide = 0x51,
		OP_Modulus = 0x52,
		OP_SizeOf = 0x53,
		OP_WaitTillMatch = 0x54,
		OP_WaitTill = 0x55,
		OP_Notify = 0x56,
		OP_EndOn = 0x57,
		OP_VoidCodePos = 0x58,
		OP_Switch = 0x59,
		OP_EndSwitch = 0x5A,
		OP_Vector = 0x5B,
		OP_GetHash = 0x5C,
		OP_RealWait = 0x5D,
		OP_VectorConstant = 0x5E,
		OP_IsDefined = 0x5F,
		OP_VectorScale = 0x60,
		OP_AnglesToUp = 0x61,
		OP_AnglesToRight = 0x62,
		OP_AnglesToForward = 0x63,
		OP_AngleClamp180 = 0x64,
		OP_VectorToAngles = 0x65,
		OP_Abs = 0x66,
		OP_GetTime = 0x67,
		OP_GetDvar = 0x68,
		OP_GetDvarInt = 0x69,
		OP_GetDvarFloat = 0x6A,
		OP_GetDvarVector = 0x6B,
		OP_GetDvarColorRed = 0x6C,
		OP_GetDvarColorGreen = 0x6D,
		OP_GetDvarColorBlue = 0x6E,
		OP_GetDvarColorAlpha = 0x6F,
		OP_FirstArrayKey = 0x70,
		OP_NextArrayKey = 0x71,
		OP_ProfileStart = 0x72,
		OP_ProfileStop = 0x73,
		OP_SafeDecTop = 0x74,
		OP_Nop = 0x75,
		OP_Abort = 0x76,
		OP_Object = 0x77,
		OP_ThreadObject = 0x78,
		OP_EvalLocalVariable = 0x79,
		OP_EvalLocalVariableRef = 0x7A,
		OP_DevblockBegin = 0x7B,
		OP_DevblockEnd = 0x7C,
		OP_Breakpoint = 0x7D,
		OP_Count = 0x7E,
	};

	enum XAssetType
	{
		ASSET_TYPE_XMODELPIECES = 0x0,
		ASSET_TYPE_PHYSPRESET = 0x1,
		ASSET_TYPE_PHYSCONSTRAINTS = 0x2,
		ASSET_TYPE_DESTRUCTIBLEDEF = 0x3,
		ASSET_TYPE_XANIMPARTS = 0x4,
		ASSET_TYPE_XMODEL = 0x5,
		ASSET_TYPE_MATERIAL = 0x6,
		ASSET_TYPE_TECHNIQUE_SET = 0x7,
		ASSET_TYPE_IMAGE = 0x8,
		ASSET_TYPE_SOUND = 0x9,
		ASSET_TYPE_SOUND_PATCH = 0xA,
		ASSET_TYPE_CLIPMAP = 0xB,
		ASSET_TYPE_CLIPMAP_PVS = 0xC,
		ASSET_TYPE_COMWORLD = 0xD,
		ASSET_TYPE_GAMEWORLD_SP = 0xE,
		ASSET_TYPE_GAMEWORLD_MP = 0xF,
		ASSET_TYPE_MAP_ENTS = 0x10,
		ASSET_TYPE_GFXWORLD = 0x11,
		ASSET_TYPE_LIGHT_DEF = 0x12,
		ASSET_TYPE_UI_MAP = 0x13,
		ASSET_TYPE_FONT = 0x14,
		ASSET_TYPE_FONTICON = 0x15,
		ASSET_TYPE_MENULIST = 0x16,
		ASSET_TYPE_MENU = 0x17,
		ASSET_TYPE_LOCALIZE_ENTRY = 0x18,
		ASSET_TYPE_WEAPON = 0x19,
		ASSET_TYPE_WEAPONDEF = 0x1A,
		ASSET_TYPE_WEAPON_VARIANT = 0x1B,
		ASSET_TYPE_WEAPON_FULL = 0x1C,
		ASSET_TYPE_ATTACHMENT = 0x1D,
		ASSET_TYPE_ATTACHMENT_UNIQUE = 0x1E,
		ASSET_TYPE_WEAPON_CAMO = 0x1F,
		ASSET_TYPE_SNDDRIVER_GLOBALS = 0x20,
		ASSET_TYPE_FX = 0x21,
		ASSET_TYPE_IMPACT_FX = 0x22,
		ASSET_TYPE_AITYPE = 0x23,
		ASSET_TYPE_MPTYPE = 0x24,
		ASSET_TYPE_MPBODY = 0x25,
		ASSET_TYPE_MPHEAD = 0x26,
		ASSET_TYPE_CHARACTER = 0x27,
		ASSET_TYPE_XMODELALIAS = 0x28,
		ASSET_TYPE_RAWFILE = 0x29,
		ASSET_TYPE_STRINGTABLE = 0x2A,
		ASSET_TYPE_LEADERBOARD = 0x2B,
		ASSET_TYPE_XGLOBALS = 0x2C,
		ASSET_TYPE_DDL = 0x2D,
		ASSET_TYPE_GLASSES = 0x2E,
		ASSET_TYPE_EMBLEMSET = 0x2F,
		ASSET_TYPE_SCRIPTPARSETREE = 0x30,
		ASSET_TYPE_KEYVALUEPAIRS = 0x31,
		ASSET_TYPE_VEHICLEDEF = 0x32,
		ASSET_TYPE_MEMORYBLOCK = 0x33,
		ASSET_TYPE_ADDON_MAP_ENTS = 0x34,
		ASSET_TYPE_TRACER = 0x35,
		ASSET_TYPE_SKINNEDVERTS = 0x36,
		ASSET_TYPE_QDB = 0x37,
		ASSET_TYPE_SLUG = 0x38,
		ASSET_TYPE_FOOTSTEP_TABLE = 0x39,
		ASSET_TYPE_FOOTSTEPFX_TABLE = 0x3A,
		ASSET_TYPE_ZBARRIER = 0x3B,
		ASSET_TYPE_COUNT = 0x3C,
		ASSET_TYPE_STRING = 0x3C,
		ASSET_TYPE_ASSETLIST = 0x3D,
		ASSET_TYPE_REPORT = 0x3E,
		ASSET_TYPE_DEPEND = 0x3F,
		ASSET_TYPE_FULL_COUNT = 0x40,
	};

	struct ScriptParseTree
	{
		const char* name;
		int len;
		GSC_OBJ* obj;
	};

	struct objFileInfo_t
	{
		GSC_OBJ* activeVersion;
		char __pad[0x24];
	};

	union XAssetHeader
	{
		ScriptParseTree* scriptParseTree;
	};

	struct WeaponDef
	{
		const char* szOverlayName;
		void** gunXModel;
		void* handXModel;
		const char* szModeName;
		unsigned __int16* notetrackSoundMapKeys;
		unsigned __int16* notetrackSoundMapValues;
		int playerAnimType;
		int weapType;
		int weapClass;
		int penetrateType;
		int impactType;
		int inventoryType;
		int fireType;
		int clipType;
		int barrelType;
		int itemIndex;
		const char* parentWeaponName;
		int iJamFireTime;
		int overheatWeapon;
		float overheatRate;
		float cooldownRate;
		float overheatEndVal;
		bool coolWhileFiring;
		bool fuelTankWeapon;
		int iTankLifeTime;
		int offhandClass;
		int offhandSlot;
		int stance;
		const void* viewFlashEffect;
		const void* worldFlashEffect;
		const void* barrelCooldownEffect;
		int barrelCooldownMinCount;
		vec3_t vViewFlashOffset;
		vec3_t vWorldFlashOffset;
		const char* pickupSound;
		const char* pickupSoundPlayer;
		const char* ammoPickupSound;
		const char* ammoPickupSoundPlayer;
		const char* projectileSound;
		const char* pullbackSound;
		const char* pullbackSoundPlayer;
		const char* fireSound;
		const char* fireSoundPlayer;
		const char* fireLoopSound;
		const char* fireLoopSoundPlayer;
		const char* fireLoopEndSound;
		const char* fireLoopEndSoundPlayer;
		const char* fireStartSound;
		const char* fireStopSound;
		const char* fireKillcamSound;
		const char* fireStartSoundPlayer;
		const char* fireStopSoundPlayer;
		const char* fireKillcamSoundPlayer;
		const char* fireLastSound;
		const char* fireLastSoundPlayer;
		const char* emptyFireSound;
		const char* emptyFireSoundPlayer;
		const char* crackSound;
		const char* whizbySound;
		const char* meleeSwipeSound;
		const char* meleeSwipeSoundPlayer;
		const char* meleeHitSound;
		const char* meleeMissSound;
		const char* rechamberSound;
		const char* rechamberSoundPlayer;
		const char* reloadSound;
		const char* reloadSoundPlayer;
		const char* reloadEmptySound;
		const char* reloadEmptySoundPlayer;
		const char* reloadStartSound;
		const char* reloadStartSoundPlayer;
		const char* reloadEndSound;
		const char* reloadEndSoundPlayer;
		const char* rotateLoopSound;
		const char* rotateLoopSoundPlayer;
		const char* rotateStopSound;
		const char* rotateStopSoundPlayer;
		const char* deploySound;
		const char* deploySoundPlayer;
		const char* finishDeploySound;
		const char* finishDeploySoundPlayer;
		const char* breakdownSound;
		const char* breakdownSoundPlayer;
		const char* finishBreakdownSound;
		const char* finishBreakdownSoundPlayer;
		const char* detonateSound;
		const char* detonateSoundPlayer;
		const char* nightVisionWearSound;
		const char* nightVisionWearSoundPlayer;
		const char* nightVisionRemoveSound;
		const char* nightVisionRemoveSoundPlayer;
		const char* altSwitchSound;
		const char* altSwitchSoundPlayer;
		const char* raiseSound;
		const char* raiseSoundPlayer;
		const char* firstRaiseSound;
		const char* firstRaiseSoundPlayer;
		const char* adsRaiseSoundPlayer;
		const char* adsLowerSoundPlayer;
		const char* putawaySound;
		const char* putawaySoundPlayer;
		const char* overheatSound;
		const char* overheatSoundPlayer;
		const char* adsZoomSound;
		const char* shellCasing;
		const char* shellCasingPlayer;
		const char** bounceSound;
		const char* standMountedWeapdef;
		const char* crouchMountedWeapdef;
		const char* proneMountedWeapdef;
		int standMountedIndex;
		int crouchMountedIndex;
		int proneMountedIndex;
		const void* viewShellEjectEffect;
		const void* worldShellEjectEffect;
		const void* viewLastShotEjectEffect;
		const void* worldLastShotEjectEffect;
		vec3_t vViewShellEjectOffset;
		vec3_t vWorldShellEjectOffset;
		vec3_t vViewShellEjectRotation;
		vec3_t vWorldShellEjectRotation;
		void* reticleCenter;
		void* reticleSide;
		int iReticleCenterSize;
		int iReticleSideSize;
		int iReticleMinOfs;
		int activeReticleType;
		vec3_t vStandMove;
		vec3_t vStandRot;
		vec3_t vDuckedOfs;
		vec3_t vDuckedMove;
		vec3_t vDuckedSprintOfs;
		vec3_t vDuckedSprintRot;
		vec2_t vDuckedSprintBob;
		float fDuckedSprintCycleScale;
		vec3_t vSprintOfs;
		vec3_t vSprintRot;
		vec2_t vSprintBob;
		float fSprintCycleScale;
		vec3_t vLowReadyOfs;
		vec3_t vLowReadyRot;
		vec3_t vRideOfs;
		vec3_t vRideRot;
		vec3_t vDtpOfs;
		vec3_t vDtpRot;
		vec2_t vDtpBob;
		float fDtpCycleScale;
		vec3_t vMantleOfs;
		vec3_t vMantleRot;
		vec3_t vSlideOfs;
		vec3_t vSlideRot;
		vec3_t vDuckedRot;
		vec3_t vProneOfs;
		vec3_t vProneMove;
		vec3_t vProneRot;
		vec3_t vStrafeMove;
		vec3_t vStrafeRot;
		float fPosMoveRate;
		float fPosProneMoveRate;
		float fStandMoveMinSpeed;
		float fDuckedMoveMinSpeed;
		float fProneMoveMinSpeed;
		float fPosRotRate;
		float fPosProneRotRate;
		float fStandRotMinSpeed;
		float fDuckedRotMinSpeed;
		float fProneRotMinSpeed;
		void** worldModel;
		void* worldClipModel;
		void* rocketModel;
		void* mountedModel;
		void* additionalMeleeModel;
		void* fireTypeIcon;
		void* hudIcon;
		int hudIconRatio;
		void* indicatorIcon;
		int indicatorIconRatio;
		void* ammoCounterIcon;
		int ammoCounterIconRatio;
		int ammoCounterClip;
		int iStartAmmo;
		int iMaxAmmo;
		int shotCount;
		const char* szSharedAmmoCapName;
		int iSharedAmmoCapIndex;
		int iSharedAmmoCap;
		bool unlimitedAmmo;
		bool ammoCountClipRelative;
		int damage[6];
		float damageRange[6];
		int minPlayerDamage;
		float damageDuration;
		float damageInterval;
		int playerDamage;
		int iMeleeDamage;
		int iDamageType;
		unsigned __int16 explosionTag;
		int iFireDelay;
		int iMeleeDelay;
		int meleeChargeDelay;
		int iDetonateDelay;
		int iSpinUpTime;
		int iSpinDownTime;
		float spinRate;
		const char* spinLoopSound;
		const char* spinLoopSoundPlayer;
		const char* startSpinSound;
		const char* startSpinSoundPlayer;
		const char* stopSpinSound;
		const char* stopSpinSoundPlayer;
		bool applySpinPitch;
		int iFireTime;
		int iLastFireTime;
		int iRechamberTime;
		int iRechamberBoltTime;
		int iHoldFireTime;
		int iDetonateTime;
		int iMeleeTime;
		int iBurstDelayTime;
		int meleeChargeTime;
		int iReloadTimeRight;
		int iReloadTimeLeft;
		int reloadShowRocketTime;
		int iReloadEmptyTimeLeft;
		int iReloadAddTime;
		int iReloadEmptyAddTime;
		int iReloadQuickAddTime;
		int iReloadQuickEmptyAddTime;
		int iReloadStartTime;
		int iReloadStartAddTime;
		int iReloadEndTime;
		int iDropTime;
		int iRaiseTime;
		int iAltDropTime;
		int quickDropTime;
		int quickRaiseTime;
		int iFirstRaiseTime;
		int iEmptyRaiseTime;
		int iEmptyDropTime;
		int sprintInTime;
		int sprintLoopTime;
		int sprintOutTime;
		int lowReadyInTime;
		int lowReadyLoopTime;
		int lowReadyOutTime;
		int contFireInTime;
		int contFireLoopTime;
		int contFireOutTime;
		int dtpInTime;
		int dtpLoopTime;
		int dtpOutTime;
		int crawlInTime;
		int crawlForwardTime;
		int crawlBackTime;
		int crawlRightTime;
		int crawlLeftTime;
		int crawlOutFireTime;
		int crawlOutTime;
		int slideInTime;
		int deployTime;
		int breakdownTime;
		int iFlourishTime;
		int nightVisionWearTime;
		int nightVisionWearTimeFadeOutEnd;
		int nightVisionWearTimePowerUp;
		int nightVisionRemoveTime;
		int nightVisionRemoveTimePowerDown;
		int nightVisionRemoveTimeFadeInStart;
		int fuseTime;
		int aiFuseTime;
		int lockOnRadius;
		int lockOnSpeed;
		bool requireLockonToFire;
		bool noAdsWhenMagEmpty;
		bool avoidDropCleanup;
		unsigned int stackFire;
		float stackFireSpread;
		float stackFireAccuracyDecay;
		const char* stackSound;
		float autoAimRange;
		float aimAssistRange;
		bool mountableWeapon;
		float aimPadding;
		float enemyCrosshairRange;
		bool crosshairColorChange;
		float moveSpeedScale;
		float adsMoveSpeedScale;
		float sprintDurationScale;
		int overlayReticle;
		int overlayInterface;
		float overlayWidth;
		float overlayHeight;
		float fAdsBobFactor;
		float fAdsViewBobMult;
		bool bHoldBreathToSteady;
		float fHipSpreadStandMin;
		float fHipSpreadDuckedMin;
		float fHipSpreadProneMin;
		float hipSpreadStandMax;
		float hipSpreadDuckedMax;
		float hipSpreadProneMax;
		float fHipSpreadDecayRate;
		float fHipSpreadFireAdd;
		float fHipSpreadTurnAdd;
		float fHipSpreadMoveAdd;
		float fHipSpreadDuckedDecay;
		float fHipSpreadProneDecay;
		float fHipReticleSidePos;
		float fAdsIdleAmount;
		float fHipIdleAmount;
		float adsIdleSpeed;
		float hipIdleSpeed;
		float fIdleCrouchFactor;
		float fIdleProneFactor;
		float fGunMaxPitch;
		float fGunMaxYaw;
		float swayMaxAngle;
		float swayLerpSpeed;
		float swayPitchScale;
		float swayYawScale;
		float swayHorizScale;
		float swayVertScale;
		float swayShellShockScale;
		float adsSwayMaxAngle;
		float adsSwayLerpSpeed;
		float adsSwayPitchScale;
		float adsSwayYawScale;
		bool sharedAmmo;
		bool bRifleBullet;
		bool armorPiercing;
		bool bAirburstWeapon;
		bool bBoltAction;
		bool bUseAltTagFlash;
		bool bUseAntiLagRewind;
		bool bIsCarriedKillstreakWeapon;
		bool aimDownSight;
		bool bRechamberWhileAds;
		bool bReloadWhileAds;
		float adsViewErrorMin;
		float adsViewErrorMax;
		bool bCookOffHold;
		bool bClipOnly;
		bool bCanUseInVehicle;
		bool bNoDropsOrRaises;
		bool adsFireOnly;
		bool cancelAutoHolsterWhenEmpty;
		bool suppressAmmoReserveDisplay;
		bool laserSight;
		bool laserSightDuringNightvision;
		bool bHideThirdPerson;
		bool bHasBayonet;
		bool bDualWield;
		bool bExplodeOnGround;
		bool bThrowBack;
		bool bRetrievable;
		bool bDieOnRespawn;
		bool bNoThirdPersonDropsOrRaises;
		bool bContinuousFire;
		bool bNoPing;
		bool bForceBounce;
		bool bUseDroppedModelAsStowed;
		bool bNoQuickDropWhenEmpty;
		bool bKeepCrosshairWhenADS;
		bool bUseOnlyAltWeaoponHideTagsInAltMode;
		bool bAltWeaponAdsOnly;
		bool bAltWeaponDisableSwitching;
		void* killIcon;
		int killIconRatio;
		bool flipKillIcon;
		bool bNoPartialReload;
		bool bSegmentedReload;
		bool bNoADSAutoReload;
		int iReloadAmmoAdd;
		int iReloadStartAdd;
		const char* szSpawnedGrenadeWeaponName;
		const char* szDualWieldWeaponName;
		unsigned int dualWieldWeaponIndex;
		int iDropAmmoMin;
		int iDropAmmoMax;
		int iDropClipAmmoMin;
		int iDropClipAmmoMax;
		int iShotsBeforeRechamber;
		bool blocksProne;
		bool bShowIndicator;
		int isRollingGrenade;
		int useBallisticPrediction;
		int isValuable;
		int isTacticalInsertion;
		bool isReviveWeapon;
		bool bUseRigidBodyOnVehicle;
		int iExplosionRadius;
		int iExplosionRadiusMin;
		int iIndicatorRadius;
		int iExplosionInnerDamage;
		int iExplosionOuterDamage;
		float damageConeAngle;
		int iProjectileSpeed;
		int iProjectileSpeedUp;
		int iProjectileSpeedRelativeUp;
		int iProjectileSpeedForward;
		float fProjectileTakeParentVelocity;
		int iProjectileActivateDist;
		float projLifetime;
		float timeToAccelerate;
		float projectileCurvature;
		void* projectileModel;
		int projExplosion;
		const void* projExplosionEffect;
		bool projExplosionEffectForceNormalUp;
		const void* projExplosionEffect2;
		bool projExplosionEffect2ForceNormalUp;
		const void* projExplosionEffect3;
		bool projExplosionEffect3ForceNormalUp;
		const void* projExplosionEffect4;
		bool projExplosionEffect4ForceNormalUp;
		const void* projExplosionEffect5;
		bool projExplosionEffect5ForceNormalUp;
		const void* projDudEffect;
		const char* projExplosionSound;
		const char* projDudSound;
		const char* mortarShellSound;
		const char* tankShellSound;
		bool bProjImpactExplode;
		bool bProjSentientImpactExplode;
		bool bProjExplodeWhenStationary;
		bool bBulletImpactExplode;
		int stickiness;
		int rotateType;
		bool plantable;
		bool hasDetonator;
		bool timedDetonation;
		bool bNoCrumpleMissile;
		bool rotate;
		bool bKeepRolling;
		bool holdButtonToThrow;
		bool offhandHoldIsCancelable;
		bool freezeMovementWhenFiring;
		float lowAmmoWarningThreshold;
		bool bDisallowAtMatchStart;
		float meleeChargeRange;
		bool bUseAsMelee;
		bool isCameraSensor;
		bool isAcousticSensor;
		bool isLaserSensor;
		bool isHoldUseGrenade;
		float* parallelBounce;
		float* perpendicularBounce;
		const void* projTrailEffect;
		vec3_t vProjectileColor;
		int guidedMissileType;
		float maxSteeringAccel;
		int projIgnitionDelay;
		const void* projIgnitionEffect;
		const char* projIgnitionSound;
		float fAdsAimPitch;
		float fAdsCrosshairInFrac;
		float fAdsCrosshairOutFrac;
		int adsGunKickReducedKickBullets;
		float adsGunKickReducedKickPercent;
		float fAdsGunKickPitchMin;
		float fAdsGunKickPitchMax;
		float fAdsGunKickYawMin;
		float fAdsGunKickYawMax;
		float fAdsGunKickAccel;
		float fAdsGunKickSpeedMax;
		float fAdsGunKickSpeedDecay;
		float fAdsGunKickStaticDecay;
		float fAdsViewKickPitchMin;
		float fAdsViewKickPitchMax;
		float fAdsViewKickMinMagnitude;
		float fAdsViewKickYawMin;
		float fAdsViewKickYawMax;
		float fAdsRecoilReductionRate;
		float fAdsRecoilReductionLimit;
		float fAdsRecoilReturnRate;
		float fAdsViewScatterMin;
		float fAdsViewScatterMax;
		float fAdsSpread;
		int hipGunKickReducedKickBullets;
		float hipGunKickReducedKickPercent;
		float fHipGunKickPitchMin;
		float fHipGunKickPitchMax;
		float fHipGunKickYawMin;
		float fHipGunKickYawMax;
		float fHipGunKickAccel;
		float fHipGunKickSpeedMax;
		float fHipGunKickSpeedDecay;
		float fHipGunKickStaticDecay;
		float fHipViewKickPitchMin;
		float fHipViewKickPitchMax;
		float fHipViewKickMinMagnitude;
		float fHipViewKickYawMin;
		float fHipViewKickYawMax;
		float fHipViewScatterMin;
		float fHipViewScatterMax;
		float fAdsViewKickCenterDuckedScale;
		float fAdsViewKickCenterProneScale;
		float fAntiQuickScopeTime;
		float fAntiQuickScopeScale;
		float fAntiQuickScopeSpreadMultiplier;
		float fAntiQuickScopeSpreadMax;
		float fAntiQuickScopeSwayFactor;
		float fightDist;
		float maxDist;
		const char* accuracyGraphName[2];
		vec2_t* accuracyGraphKnots[2];
		vec2_t* originalAccuracyGraphKnots[2];
		int accuracyGraphKnotCount[2];
		int originalAccuracyGraphKnotCount[2];
		int iPositionReloadTransTime;
		float leftArc;
		float rightArc;
		float topArc;
		float bottomArc;
		float accuracy;
		float aiSpread;
		float playerSpread;
		float minTurnSpeed[2];
		float maxTurnSpeed[2];
		float pitchConvergenceTime;
		float yawConvergenceTime;
		float suppressTime;
		float maxRange;
		float fAnimHorRotateInc;
		float fPlayerPositionDist;
		const char* szUseHintString;
		const char* dropHintString;
		int iUseHintStringIndex;
		int dropHintStringIndex;
		float horizViewJitter;
		float vertViewJitter;
		float cameraShakeScale;
		int cameraShakeDuration;
		int cameraShakeRadius;
		float explosionCameraShakeScale;
		int explosionCameraShakeDuration;
		int explosionCameraShakeRadius;
		const char* szScript;
		float destabilizationRateTime;
		float destabilizationCurvatureMax;
		int destabilizeDistance;
		float* locationDamageMultipliers;
		const char* fireRumble;
		const char* meleeImpactRumble;
		const char* reloadRumble;
		const char* explosionRumble;
		void* tracerType;
		void* enemyTracerType;
		float adsDofStart;
		float adsDofEnd;
		float hipDofStart;
		float hipDofEnd;
		float scanSpeed;
		float scanAccel;
		int scanPauseTime;
		const char* flameTableFirstPerson;
		const char* flameTableThirdPerson;
		void* flameTableFirstPersonPtr;
		void* flameTableThirdPersonPtr;
		const void* tagFx_preparationEffect;
		const void* tagFlash_preparationEffect;
		bool doGibbing;
		float maxGibDistance;
		float altScopeADSTransInTime;
		float altScopeADSTransOutTime;
		int iIntroFireTime;
		int iIntroFireLength;
		const void* meleeSwipeEffect;
		const void* meleeImpactEffect;
		const void* meleeImpactNoBloodEffect;
		const char* throwBackType;
		void* weaponCamo;
		float customFloat0;
		float customFloat1;
		float customFloat2;
		int customBool0;
		int customBool1;
		int customBool2;
	};

	static_assert(sizeof(WeaponDef) == 2448);

	struct msg_t
	{
		int overflowed;
		int readOnly;
		char* data;
		char* splitData;
		int maxsize;
		int cursize;
		int splitSize;
		int readcount;
		int bit;
		int lastEntityRef;
		int flush;
		netsrc_t targetLocalNetID;
	};


	struct trajectory_t
	{
		char trType;
		int trTime;
		int trDuration;
		vec3_t trBase;
		vec3_t trDelta;
	};
	struct LerpEntityStateVehicleRotorArm
	{
		__int16 pitch;
		__int16 roll;
	};
	struct LerpEntityStateVehicleGunnerAngles
	{
		__int16 pitch;
		__int16 yaw;
	};
	struct LerpEntityStateZBarrierPiece
	{
		char flags;
		char animTime;
	};
	struct LerpEntityStateEarthquake
	{
		float scale;
		float radius;
		int duration;
	};
	union $6EDB3B6EA9A5F72E2667D669F0CA2B22
	{
		LerpEntityStateVehicleGunnerAngles gunnerAngles[4];
		LerpEntityStateVehicleRotorArm rotors[4];
	};
	union $ABF14481B6CB3C635AAEF83DF0F35947
	{
		__int16 throttle;
		__int16 bodyPitch;
	};

	struct __declspec(align(2)) LerpEntityStateVehicle
	{
		float steerYaw;
		float bodyRoll;
		$6EDB3B6EA9A5F72E2667D669F0CA2B22 ___u2;
		$ABF14481B6CB3C635AAEF83DF0F35947 ___u3;
		__int16 gunPitch;
		__int16 gunYaw;
		char targetRotorSpeed;

		struct LerpEntityStateJetThrust
		{
			vec3_t thrustDir;
			int dummy;
			float dotLimit;
		};
	}; struct LerpEntityStatePhysicsJitter
	{
		float innerRadius;
		float minDisplacement;
		float maxDisplacement;
	};
	struct LerpEntityStatePlayer
	{
		float leanf;
		int movementDir;
		int moveType;
		Weapon primaryWeapon;
		Weapon stowedWeapon;
		Weapon offhandWeapon;
		Weapon meleeWeapon;
		char meleeWeaponCamo;
		char meleeWeaponModel;
		char vehicleType;
		char vehicleAnimBoneIndex;
		char vehicleSeat;
		char stowedWeaponCamo;
		char weaponHeat;
		char proneLegsAngle;
	};


	struct LerpEntityStateLoopFx
	{
		float cullDist;
		int period;
	};
	struct LerpEntityStateCustomExplode
	{
		int startTime;
		int effectIndex;
	};
	struct LerpEntityStateTurret
	{
		vec3_t gunAngles;
		int ownerNum;
		float heatVal;
		int overheating;
		float pivotOffset;
		int flags;
	};

	union $41090544F01B971CEA6978417397BD1B
	{
		int actorNum;
		int corpseNum;
	};
	struct $0E2ADDF4FFAE096DF2EC3B0B4A5C12BC
	{
		__int16 fBodyPitch;
	};

	struct LerpEntityStateActor
	{
		$41090544F01B971CEA6978417397BD1B index;
		int species;
		int team;
		int enemy;
		int freeCameraLockOnAllowed;
		$0E2ADDF4FFAE096DF2EC3B0B4A5C12BC proneInfo;
		unsigned __int16 aiType;
	};
	struct LerpEntityStateZBarrier
	{
		unsigned int barrierTypeIndex;
		LerpEntityStateZBarrierPiece pieces[6];
	};
	struct LerpEntityStateAnonymous
	{
		int data[9];
	};
	struct LerpEntityStateExplosion
	{
		float innerRadius;
		float magnitude;
		float innerDamage;
		int dummy;
		float outerDamage;
	};
	struct LerpEntityStateBulletHit
	{
		vec3_t start;
	};
	struct LerpEntityStatePrimaryLight
	{
		char colorAndExp[4];
		float intensity;
		float radius;
		float cosHalfFovOuter;
		float cosHalfFovInner;
		unsigned int mixerIndex;
	};
	struct LerpEntityStateCreateDynEnt
	{
		int model;
		int fxId;
		int mature;
	};
	struct LerpEntityStateMissile
	{
		int launchTime;
		int parentClientNum;
		int fuseTime;
		int forcedDud;
		float autoDetonateTime;
	};
	struct LerpEntityStateScriptMover
	{
		char attachTagIndex[4];
		int attachedTagIndex;
		__int16 attachModelIndex[4];
		__int16 animScriptedAnim;
		__int16 attachedEntNum;
		int attachedEntTime;
		__int16 exploderIndex;
		unsigned __int16 scale;
		char flags;
		unsigned __int16 aiType;
	};
	struct LerpEntityStateDestructibleHit
	{
		unsigned int modelState[3];
	};
	struct LerpEntityStateExplosionJolt
	{
		float innerRadius;
		vec3_t impulse;
	};
	struct LerpEntityStateJetThrust
	{
		vec3_t thrustDir;
		int dummy;
		float dotLimit;
	};

	struct LerpEntityStateStreamerHint
	{
		float factor;
	};

	union LerpEntityStateTypeUnion
	{
		LerpEntityStateTurret turret;
		LerpEntityStateLoopFx loopFx;
		LerpEntityStateActor actor;
		LerpEntityStatePrimaryLight primaryLight;
		LerpEntityStatePlayer player;
		LerpEntityStateVehicle vehicle;
		LerpEntityStateMissile missile;
		LerpEntityStateScriptMover scriptMover;
		LerpEntityStateBulletHit bulletHit;
		LerpEntityStateEarthquake earthquake;
		LerpEntityStateCustomExplode customExplode;
		LerpEntityStateExplosion explosion;
		LerpEntityStateExplosionJolt explosionJolt;
		LerpEntityStateJetThrust jetThrust;
		LerpEntityStatePhysicsJitter physicsJitter;
		LerpEntityStateDestructibleHit destructibleHit;
		LerpEntityStateCreateDynEnt createDynEnt;
		LerpEntityStateStreamerHint streamerHint;
		LerpEntityStateZBarrier zbarrier;
		LerpEntityStateAnonymous anonymous;
	};
	union $9B35D18BE7DE1A49784D3DA3953D5C89
	{
		char iHeadIconTeam;
		__int16 teamAndOwnerIndex;
	};

	struct LerpEntityState
	{
		int eFlags;
		int eFlags2;
		trajectory_t pos;
		trajectory_t apos;
		LerpEntityStateTypeUnion u;
		__int16 useCount;
		$9B35D18BE7DE1A49784D3DA3953D5C89 faction;
		unsigned int clientFields;
	};

	struct renderOptions_s
	{
		unsigned int s;
	};
	struct actorAnimState_t
	{
		__int16 state;
		char subState;
		float fLeanAmount;
		float fAimUpDown;
		float fAimLeftRight;
	};
	struct playerAnimState_t
	{
		int legsAnim;
		int torsoAnim;
		float fTorsoPitch;
		float fWaistPitch;
	};
	struct vehicleState_t
	{
		__int16 flags;
		char vehicleDefIndex;
		char treeId;
		__int16 animId;
		__int16 attachModelIndex[2];
		char attachTagIndex[2];
	};
	struct hardlineHint_t
	{
		char team;
		char perk;
		char hint;
	};
	struct scriptMoverState_t
	{
		char fov;
		char treeId;
		__int16 animId;
	};
	struct fxLightingState_t
	{
		float primaryLightFraction;
		int lightingOriginOffset;
	};

	union $F846825EF6B602428693DD44F2E5E7AC
	{
		actorAnimState_t animState;
		playerAnimState_t anim;
		vehicleState_t vehicleState;
		hardlineHint_t hardline;
		scriptMoverState_t moverState;
		fxLightingState_t fx;
	};
	union $8A55D3BB0C758A956A56F8E75035F352
	{
		int hintString;
		int vehicleXModel;
		unsigned int secondBcAlias;
		unsigned int soundTag;
	};
	struct clientLinkInfo_t
	{
		__int16 parentEnt;
		char tagIndex;
		char flags;
	};
	union $32A75A724673AB33BA2D603F67D50EA7
	{
		__int16 brushmodel;
		__int16 xmodel;
		__int16 primaryLight;
		unsigned __int16 bone;
	};
	union $5941A7488DBCD59DA5C855EBC8EBAB48
	{
		char scale;
		char eventParm2;
		char helicopterStage;
		char destructibleid;
		char zombieShrinkOn;
	};

	struct __declspec(align(2)) entityState_s
	{
		int number;
		LerpEntityState lerp;
		int time2;
		unsigned int loopSoundId;
		int solid;
		renderOptions_s renderOptions;
		$F846825EF6B602428693DD44F2E5E7AC un2;
		$8A55D3BB0C758A956A56F8E75035F352 un3;
		unsigned int partBits[5];
		clientLinkInfo_t clientLinkInfo;
		int clientMask[1];
		char events[4];
		unsigned int eventParms[4];
		unsigned int eventParm;
		__int16 eType;
		__int16 groundEntityNum;
		$32A75A724673AB33BA2D603F67D50EA7 index;
		__int16 otherEntityNum;
		__int16 attackerEntityNum;
		__int16 enemyModel;
		Weapon weapon;
		Weapon lastStandPrevWeapon;
		unsigned __int16 targetname;
		__int16 loopSoundFade;
		__int16 eventSequence;
		char surfType;
		char clientNum;
		char iHeadIcon;
		char weaponModel;
		$5941A7488DBCD59DA5C855EBC8EBAB48 un1;
	};
	struct cachedSnapshot_t
	{
		int archivedFrame;
		int time;
		int physicsTime;
		int entityCount;
		int clientCount;
		int actorCount;
		int firstEntityIndex;
		int firstClientIndex;
		int firstActorIndex;
		int matchStateIndex;
		int usesDelta;
	};
	struct archivedEntityShared_t
	{
		int svFlags;
		vec3_t absmin;
		vec3_t absmax;
	};

	struct archivedEntity_s
	{
		entityState_s s;
		archivedEntityShared_t r;
	};
	struct __declspec(align(4)) PlayerVehicleState
	{
		vec3_t origin;
		vec3_t angles;
		vec3_t velocity;
		vec3_t angVelocity;
		vec2_t tilt;
		vec2_t tiltVelocity;
		float targetHeightDelta;
		float lastGroundHeight;
		int entity;
		int flags;
		bool fullPhysics;
	};
	enum ClientNum_t
	{
		INVALID_CLIENT_INDEX = 0xFFFFFFFF,
		CLIENT_INDEX_FIRST = 0x0,
		CLIENT_INDEX_0 = 0x0,
		CLIENT_INDEX_1 = 0x1,
		CLIENT_INDEX_2 = 0x2,
		CLIENT_INDEX_3 = 0x3,
		CLIENT_INDEX_4 = 0x4,
		CLIENT_INDEX_5 = 0x5,
		CLIENT_INDEX_6 = 0x6,
		CLIENT_INDEX_7 = 0x7,
		CLIENT_INDEX_8 = 0x8,
		CLIENT_INDEX_9 = 0x9,
		CLIENT_INDEX_10 = 0xA,
		CLIENT_INDEX_11 = 0xB,
		CLIENT_INDEX_12 = 0xC,
		CLIENT_INDEX_13 = 0xD,
		CLIENT_INDEX_14 = 0xE,
		CLIENT_INDEX_15 = 0xF,
		CLIENT_INDEX_16 = 0x10,
		CLIENT_INDEX_17 = 0x11,
		CLIENT_INDEX_18 = 0x12,
		CLIENT_INDEX_19 = 0x13,
		CLIENT_INDEX_20 = 0x14,
		CLIENT_INDEX_21 = 0x15,
		CLIENT_INDEX_22 = 0x16,
		CLIENT_INDEX_23 = 0x17,
		CLIENT_INDEX_24 = 0x18,
		CLIENT_INDEX_25 = 0x19,
		CLIENT_INDEX_26 = 0x1A,
		CLIENT_INDEX_27 = 0x1B,
		CLIENT_INDEX_28 = 0x1C,
		CLIENT_INDEX_29 = 0x1D,
		CLIENT_INDEX_30 = 0x1E,
		CLIENT_INDEX_31 = 0x1F,
		CLIENT_INDEX_COUNT = 0x12,
	};
	enum OffhandSecondaryClass
	{
		PLAYER_OFFHAND_SECONDARY_SMOKE = 0x0,
		PLAYER_OFFHAND_SECONDARY_FLASH = 0x1,
		PLAYER_OFFHAND_SECONDARIES_TOTAL = 0x2,
	};
	enum OffhandPrimaryClass
	{
		PLAYER_OFFHAND_PRIMARY_FRAG = 0x0,
		PLAYER_OFFHAND_PRIMARY_GEAR = 0x1,
		PLAYER_OFFHAND_PRIMARIES_TOTAL = 0x2,
	};
	struct __declspec(align(4)) PlayerHeldWeapon
	{
		Weapon weapon;
		renderOptions_s options;
		float heatPercent;
		int fuelTankTime;
		int adsZoomSelect;
		bool overHeating;
		bool needsRechamber;
		bool heldBefore;
		bool quickReload;
		bool blockWeaponPickup;
		char model;
	};
	struct AmmoPool
	{
		int count;
	};
	struct AmmoClip
	{
		int count;
	};
	enum ViewLockTypes
	{
		PLAYERVIEWLOCK_NONE = 0x0,
		PLAYERVIEWLOCK_FULL = 0x1,
		PLAYERVIEWLOCK_WEAPONJITTER = 0x2,
		PLAYERVIEWLOCKCOUNT = 0x3,
	};
	enum locSel_t
	{
		LOC_SEL_NONE = 0x0,
		LOC_SEL_ARTILLERY = 0x1,
		LOC_SEL_AIRSTRIKE = 0x2,
		LOC_SEL_MORTAR = 0x3,
		LOC_SEL_NAPALM = 0x4,
		LOC_SEL_COMLINK = 0x5,
	};
	struct SprintState
	{
		int sprintButtonUpRequired;
		int sprintDelay;
		int lastSprintStart;
		int lastSprintEnd;
		int sprintStartMaxLength;
		int sprintDuration;
		int sprintCooldown;
	};
	struct MantleState
	{
		float yaw;
		int timer;
		int transIndex;
		int flags;
	};
	enum ActionSlotType
	{
		ACTIONSLOTTYPE_DONOTHING = 0x0,
		ACTIONSLOTTYPE_SPECIFYWEAPON = 0x1,
		ACTIONSLOTTYPE_ALTWEAPONTOGGLE = 0x2,
		ACTIONSLOTTYPE_NIGHTVISION = 0x3,
		ACTIONSLOTTYPECOUNT = 0x4,
	};
	struct ActionSlotParam_SpecifyWeapon
	{
		Weapon weapon;
	};
	struct ActionSlotParam
	{
		ActionSlotParam_SpecifyWeapon specifyWeapon;
	};
	enum objectiveState_t
	{
		OBJST_EMPTY = 0x0,
		OBJST_ACTIVE = 0x1,
		OBJST_INVISIBLE = 0x2,
		OBJST_NUMSTATES = 0x3,
	};

	struct __declspec(align(2)) objective_t
	{
		objectiveState_t state;
		vec3_t origin;
		__int16 entNum;
		vec2_t size;
		int icon;
		__int16 ownerNum;
		unsigned __int16 name;
		__int16 teamMask;
		char progress;
		int clientUseMask[1];
		char gamemodeFlags;
		char flags;
		char teamNum;
	};

	struct $C96EA5EC2ACBB9C0BF22693F316ACC67
	{
		char r;
		char g;
		char b;
		char a;
	};

	union hudelem_color_t
	{
		$C96EA5EC2ACBB9C0BF22693F316ACC67 __s0;
		int rgba;
	};

	struct __declspec(align(4)) hudelem_s
	{
		float x;
		float y;
		float z;
		float fontScale;
		float fromFontScale;
		int fontScaleStartTime;
		hudelem_color_t color;
		hudelem_color_t fromColor;
		int fadeStartTime;
		int scaleStartTime;
		float fromX;
		float fromY;
		int moveStartTime;
		int time;
		int duration;
		float value;
		float sort;
		hudelem_color_t glowColor;
		int fxBirthTime;
		unsigned int flags;
		__int16 targetEntNum;
		__int16 fontScaleTime;
		__int16 fadeTime;
		__int16 label;
		__int16 width;
		__int16 height;
		__int16 fromWidth;
		__int16 fromHeight;
		__int16 scaleTime;
		__int16 moveTime;
		__int16 text;
		unsigned __int16 fxLetterTime;
		unsigned __int16 fxDecayStartTime;
		unsigned __int16 fxDecayDuration;
		unsigned __int16 fxRedactDecayStartTime;
		unsigned __int16 fxRedactDecayDuration;
		char type;
		char font;
		char alignOrg;
		char alignScreen;
		char materialIndex;
		char offscreenMaterialIdx;
		char fromAlignOrg;
		char fromAlignScreen;
		char soundID;
		char ui3dWindow;
	};

	struct $94851E490C69050A57D20547D7862040
	{
		hudelem_s current[31];
		hudelem_s archival[31];
	};

	struct __declspec(align(8)) playerState_s
	{
		int commandTime;
		int pm_type;
		int bobCycle;
		int pm_flags;
		__int64 weapFlags;
		int otherFlags;
		int pm_time;
		unsigned int loopSoundId;
		int loopSoundFade;
		vec3_t origin;
		vec3_t velocity;
		int remoteEyesEnt;
		int remoteEyesTagname;
		int remoteControlEnt;
		int weaponTime;
		int weaponDelay;
		int weaponTimeLeft;
		int weaponDelayLeft;
		int weaponIdleTime;
		int grenadeTimeLeft;
		int throwBackGrenadeOwner;
		int throwBackGrenadeTimeLeft;
		int weaponRestrictKickTime;
		bool mountAvailable;
		bool bRunLeftGun;
		bool bCarryingTurret;
		vec3_t mountPos;
		float mountDir;
		bool bThirdPerson;
		int foliageSoundTime;
		int gravity;
		float leanf;
		int speed;
		vec3_t delta_angles;
		int groundEntityNum;
		int moverEntityNum;
		int moverTimestamp;
		int groundType;
		vec3_t vLadderVec;
		int jumpTime;
		float jumpOriginZ;
		int slideTime;
		int moveType;
		int legsTimer;
		int torsoTimer;
		__int16 legsAnim;
		__int16 torsoAnim;
		int legsAnimDuration;
		int torsoAnimDuration;
		int damageTimer;
		int damageDuration;
		int dmgDirection;
		int dmgType;
		int corpseIndex;
		int movementDir;
		int eFlags;
		int eFlags2;
		PlayerVehicleState vehicleState;
		__int16 predictableEventSequence;
		__int16 predictableEventSequenceOld;
		int predictableEvents[4];
		unsigned int predictableEventParms[4];
		__int16 unpredictableEventSequence;
		__int16 unpredictableEventSequenceOld;
		int unpredictableEvents[4];
		unsigned int unpredictableEventParms[4];
		ClientNum_t clientNum;
		Weapon offHandWeapon;
		OffhandSecondaryClass offhandSecondary;
		OffhandPrimaryClass offhandPrimary;
		renderOptions_s renderOptions;
		int momentum;
		Weapon weapon;
		Weapon lastStandPrevWeapon;
		Weapon lastWeaponAltModeSwitch;
		Weapon stowedWeapon;
		char unusedCompatibilityPadding;
		Weapon meleeWeapon;
		int weaponstate;
		int weaponstateLeft;
		unsigned int weaponShotCount;
		unsigned int weaponShotCountLeft;
		float fWeaponPosFrac;
		int adsDelayTime;
		int spreadOverride;
		int spreadOverrideState;
		float weaponSpinLerp;
		int viewmodelIndex;
		vec3_t viewangles;
		int viewHeightTarget;
		float viewHeightCurrent;
		int viewHeightLerpTime;
		int viewHeightLerpTarget;
		int viewHeightLerpDown;
		vec2_t viewAngleClampBase;
		vec2_t viewAngleClampRange;
		int damageEvent;
		int damageYaw;
		int damagePitch;
		int damageCount;
		int stats[4];
		PlayerHeldWeapon heldWeapons[15];
		AmmoPool ammoNotInClip[15];
		AmmoClip ammoInClip[15];
		float proneDirection;
		float proneDirectionPitch;
		float proneTorsoPitch;
		ViewLockTypes viewlocked;
		__int16 viewlocked_entNum;
		int vehiclePos;
		int vehicleType;
		int vehicleAnimBoneIndex;
		int linkFlags;
		vec3_t linkAngles;
		int cursorHint;
		int cursorHintString;
		int cursorHintEntIndex;
		int cursorHintWeapon;
		int iCompassPlayerInfo;
		unsigned int spyplaneTypeEnabled;
		unsigned int satelliteTypeEnabled;
		int locationSelectionInfo;
		locSel_t locationSelectionType;
		SprintState sprintState;
		int lastDtpEnd;
		float fTorsoPitch;
		float fWaistPitch;
		float holdBreathScale;
		int holdBreathTimer;
		int chargeShotTimer;
		unsigned int chargeShotLevel;
		unsigned int shotsFiredFromChamber;
		float quickScopeScale;
		int quickScopeTimer;
		unsigned int clientFields;
		unsigned int clientFields2;
		unsigned int clientFields3;
		unsigned int entityStateClientFields;
		float moveSpeedScaleMultiplier;
		MantleState mantleState;
		int vehicleAnimStage;
		int vehicleEntryPoint;
		unsigned int scriptedAnim;
		int scriptedAnimTime;
		int meleeChargeEnt;
		int meleeChargeDist;
		int meleeChargeTime;
		int weapLockFlags;
		int weapLockedEntnum;
		unsigned int airburstMarkDistance;
		unsigned int perks[2];
		ActionSlotType actionSlotType[4];
		ActionSlotParam actionSlotParam[4];
		Weapon inventoryWeapon;
		__int16 wiiumoteAimX;
		__int16 wiiumoteAimY;
		char wiiuControllerType;
		char vehicleDefIndex;
		int entityEventSequence;
		int weapAnim;
		int weapAnimLeft;
		float aimSpreadScale;
		int shellshockIndex;
		int shellshockTime;
		int shellshockDuration;
		float dofNearStart;
		float dofNearEnd;
		float dofFarStart;
		float dofFarEnd;
		float dofNearBlur;
		float dofFarBlur;
		float dofViewmodelStart;
		float dofViewmodelEnd;
		int waterlevel;
		int smokeColorIndex;
		int hudElemLastAssignedSoundID;
		int adsZoomSelect;
		int adsZoomLatchTime;
		bool adsZoomLatchState;
		int adsPrevZoomSelect;
		int adsPrevZoomSelectTime;
		int artilleryInboundIconLocation;
		float visionSetLerpRatio;
		int poisoned;
		int binoculars;
		int scriptCursorHintString;
		objective_t objective[32];
		int deltaTime;
		int killCamEntity;
		int killCamTargetEntity;
		int introShotsFired;
		$94851E490C69050A57D20547D7862040 hud;
	};

	enum pmoveVehAnimState_t
	{
		PMOVE_VEH_ANIM_STATE_PLAYING = 0x0,
		PMOVE_VEH_ANIM_STATE_END_STAGE = 0x1,
		PMOVE_VEH_ANIM_STATE_COMPLETE = 0x2,
	};

	enum LocalClientNum_t
	{
		INVALID_LOCAL_CLIENT = 0xFFFFFFFF,
		LOCAL_CLIENT_FIRST = 0x0,
		LOCAL_CLIENT_0 = 0x0,
		ONLY_LOCAL_CLIENT = 0x0,
		LOCAL_CLIENT_COUNT = 0x1,
	};

	union $FC62ACB8B3FBB71D25811245FDCD5C2F
	{
		//const CollisionAabbTree* tree;
		//const cbrush_t* brush;
		void* tree;
		void* brush;
	};

	struct col_prim_t
	{
		int type;
		$FC62ACB8B3FBB71D25811245FDCD5C2F ___u1;
	};

	struct visitor_base_t
	{
		void* __vftable /*VFT*/;
	};


	struct __declspec(align(8)) colgeom_visitor_t : visitor_base_t
	{
		char pad[0x80];
	};

	struct __declspec(align(16)) colgeom_visitor_inlined_t : colgeom_visitor_t
	{
		int nprims;
		bool overflow;
		col_prim_t prims[300];
	};


	struct __declspec(align(16)) pmove_t
	{
		playerState_s* ps;
		usercmd_s cmd;
		usercmd_s oldcmd;
		int tracemask;
		int numtouch;
		int touchents[32];
		int numGlassTouch;
		int touchGlasses[32];
		vec3_t mins;
		vec3_t maxs;
		float xyspeed;
		float currentPitch;
		float averagePitch;
		int nextPitch;
		float pitchHistory[30];
		int proneChange;
		float maxSprintTimeMultiplier;
		bool mantleStarted;
		vec3_t mantleEndPos;
		int mantleDuration;
		int viewChangeTime;
		float viewChange;
		vec3_t vehicleAngles;
		pmoveVehAnimState_t vehAnimState;
		unsigned __int8 handler;
		LocalClientNum_t localClientNum;
		void* m_gjkcc_input; //const gjkcc_input_t* m_gjkcc_input;
		colgeom_visitor_inlined_t proximity_data;
		bool gravity;
		int gravityOverride;
	};

	struct __declspec(align(16)) trace_t
	{
		char pad[0x40];
		/*
		hybrid_vector normal;
		float fraction;
		int sflags;
		int cflags;
		TraceHitType hitType;
		unsigned __int16 hitId;
		unsigned __int16 modelIndex;
		unsigned __int16 partName;
		unsigned __int16 boneIndex;
		unsigned __int16 partGroup;
		bool allsolid;
		bool startsolid;
		bool walkable;
		cStaticModel_s* staticModel;
		int hitPartition;
		*/
	};


	struct pml_t
	{
		vec3_t forward;
		vec3_t right;
		vec3_t up;
		float frametime;
		int msec;
		int walking;
		int groundPlane;
		int almostGroundPlane;
		trace_t groundTrace;
		float impactSpeed;
		vec3_t previous_origin;
		vec3_t previous_velocity;
		unsigned int holdrand;
	};

	struct serverSnapshot_t
	{
		playerState_s* ps[18];
		int entityCount;
		int clientCount;
		int actorCount;
		int firstEntityIndex;
		int firstClientIndex;
		int firstActorIndex;
		int matchStateIndex;
	};

	struct ArchivedMatchState
	{
		int matchUIVisibilityFlags;
		int bombTimer[2];
		int roundsPlayed;
		int worldFields[8];
	};

	enum scoreboardColumnType_t
	{
		SB_TYPE_INVALID = 0x0,
		SB_TYPE_NONE = 0x1,
		SB_TYPE_SCORE = 0x2,
		SB_TYPE_KILLS = 0x3,
		SB_TYPE_DEATHS = 0x4,
		SB_TYPE_ASSISTS = 0x5,
		SB_TYPE_DEFENDS = 0x6,
		SB_TYPE_PLANTS = 0x7,
		SB_TYPE_DEFUSES = 0x8,
		SB_TYPE_RETURNS = 0x9,
		SB_TYPE_CAPTURES = 0xA,
		SB_TYPE_DESTRUCTIONS = 0xB,
		SB_TYPE_KDRATIO = 0xC,
		SB_TYPE_SURVIVED = 0xD,
		SB_TYPE_STABS = 0xE,
		SB_TYPE_TOMAHAWKS = 0xF,
		SB_TYPE_HUMILIATED = 0x10,
		SB_TYPE_X2SCORE = 0x11,
		SB_TYPE_HEADSHOTS = 0x12,
		SB_TYPE_DOWNS = 0x13,
		SB_TYPE_REVIVES = 0x14,
		SB_TYPE_AGRKILLS = 0x15,
		SB_TYPE_HACKS = 0x16,
		SB_TYPE_POINTS_TO_WIN = 0x17,
		SB_TYPE_KILLS_CONFIRMED = 0x18,
		SB_TYPE_KILLS_DENIED = 0x19,
		NUM_SB_TYPES = 0x1A,
	};

	struct UnarchivedMatchState
	{
		int teamScores[9];
		int matchUIVisibilityFlags;
		scoreboardColumnType_t scoreboardColumnTypes[5];
		char sideHasMeat;
		bool initialPlayersConnected;
		unsigned int talkFlags;
	};

	struct __declspec(align(8)) MatchState
	{
		int index;
		ArchivedMatchState archivedState;
		UnarchivedMatchState unarchivedState;
		unsigned int pad[1];
	};

	union $137F9095F7597C63EB19E8F61F5887B0
	{
		int prestige;
		int lastDaysPlayed;
	};
	struct netUInt64
	{
		unsigned int low;
		unsigned int high;
	};

	union $A4571D12BF10171BF9A5B9DFBEBF42AF
	{
		unsigned __int64 xuid;
		netUInt64 xuid64;
	};
	union $A860F8FD49AE0869BFEBEBC3953ECFC3
	{
		unsigned __int64 leagueTeamID;
		netUInt64 leagueTeamID64;
	};
	union $45B0B4E60D713732115D2818FB67A7F4
	{
		unsigned __int64 leagueSubdivisionID;
		netUInt64 leagueSubdivisionID64;
	};
	enum VehicleAnimState
	{
		VEHICLEANIMSTATE_IDLE = 0x0,
		VEHICLEANIMSTATE_ENTRY = 0x1,
		VEHICLEANIMSTATE_CHANGEPOS = 0x2,
		VEHICLEANIMSTATE_EXIT = 0x3,
		VEHICLEANIMSTATECOUNT = 0x4,
	};
	struct score_s
	{
		int ping;
		int status_icon;
		int place;
		int score;
		int kills;
		int assists;
		int deaths;
		int wagerWinnings;
		int scoreboardColumns[5];
		int downs;
		int revives;
		int headshots;
		int scoreMultiplier;
		int currentStreak;
	};

	struct __declspec(align(8)) clientState_s
	{
		ClientNum_t clientIndex;
		team_t team;
		ffa_team_t ffaTeam;
		int modelindex;
		int riotShieldNext;
		int attachModelIndex[6];
		int attachTagIndex[6];
		char name[32];
		float maxSprintTimeMultiplier;
		int rank;
		$137F9095F7597C63EB19E8F61F5887B0 ___u10;
		int lastDamageTime;
		int lastStandStartTime;
		int turnedHumanTime;
		int beingRevived;
		$A4571D12BF10171BF9A5B9DFBEBF42AF ___u15;
		$A860F8FD49AE0869BFEBEBC3953ECFC3 ___u16;
		int leagueDivisionID;
		$45B0B4E60D713732115D2818FB67A7F4 ___u18;
		int leagueSubdivisionRank;
		unsigned int perks[2];
		int voiceConnectivityBits;
		char clanAbbrev[8];
		int attachedVehEntNum;
		int attachedVehSeat;
		int needsRevive;
		int clanAbbrevEV;
		VehicleAnimState vehAnimState;
		score_s score;
		int clientUIVisibilityFlags;
		int offhandWeaponVisible;
	};
	struct actorState_s
	{
		int actorIndex;
		int entityNum;
		int modelindex;
		int attachModelIndex[6];
		int attachTagIndex[6];
		unsigned int attachIgnoreCollision;
		char name[32];
		int animScriptedAnim;
	};
	struct archivedSnapshot_s
	{
		int start;
		int size;
	};
	struct cachedClient_s
	{
		int playerStateExists;
		clientState_s cs;
		playerState_s ps;
	};
	struct cachedActor_s
	{
		actorState_s as;
	};
	struct challenge_t
	{
		netadr_t adr;
		int challenge;
		int time;
		int pingTime;
		int firstTime;
		int firstPing;
		int connected;
		int guid;
	};
	struct tempBanSlot_t
	{
		int guid;
		int banTime;
	};

	enum connstate_t
	{
		CA_DISCONNECTED = 0x0,
		CA_CINEMATIC = 0x1,
		CA_UICINEMATIC = 0x2,
		CA_LOGO = 0x3,
		CA_CONNECTING = 0x4,
		CA_CHALLENGING = 0x5,
		CA_CONNECTED = 0x6,
		CA_SENDINGSTATS = 0x7,
		CA_LOADING = 0x8,
		CA_PRIMED = 0x9,
		CA_ACTIVE = 0xA,
	};
	enum ControllerIndex_t
	{
		INVALID_CONTROLLER_PORT = 0xFFFFFFFF,
		CONTROLLER_INDEX_FIRST = 0x0,
		CONTROLLER_INDEX_0 = 0x0,
		CONTROLLER_INDEX_COUNT = 0x1,
	};
	struct PartySceNpId
	{
		char handle[20];
		char opt[8];
		char reserved[8];
	};
	struct XNADDR
	{
		char addrBuff[37];
	};

	struct liveAddr
	{
		XNADDR xnaddr;
	};

	struct __declspec(align(4)) platformNetAdr
	{
		netadr_t netAddr;
		liveAddr liveaddr;
	};
	struct bdSecurityID
	{
		char ab[8];
	};
	union $4CC444478A5CF1CA3E666352329DBEA9
	{
		int rank;
		int rankPosition;
	};
	union $6F592E3A475AB50E77AAA300354707EA
	{
		int prestige;
		int divisionID;
		int daysLastPlayed;
	};
	struct PartyMemberTeam
	{
		int team;
		int switchTeam;
		int switchTeamTime;
		int lastTeam;
	};

	struct PartyMember
	{
		char status;
		int ackedMembers;
		int lastPacketTime;
		int lastHeartbeatTime;
		int lastPartyStateAck;
		int lastDemoHeartBeatTime;
		unsigned int challenge;
		int subpartyIndex;
		connstate_t reportedConnState;
		bool invited;
		bool headsetPresent;
		bool inLivePartyVoice;
		bool inLivePartyTalking;
		bool finishedLoadingDemo;
		bool isReady;
		bool isGuest;
		bool isSplitscreenClient;
		int natType;
		unsigned __int64 player;
		char gamertag[32];
		ControllerIndex_t localControllerIndex;
		int playerEmblem;
		PartySceNpId npid;
		char clanAbbrev[5];
		bool clanAbbrev_IsEliteValidated;
		int codPoints;
		unsigned int affinityBits;
		platformNetAdr platformAddr;
		int availableMapPackFlags;
		bdSecurityID privatePartyId;
		$4CC444478A5CF1CA3E666352329DBEA9 ___u30;
		$6F592E3A475AB50E77AAA300354707EA ___u31;
		int deaths;
		float skillRating;
		float skillVariance;
		PartyMemberTeam teamInfo;
		int score;
		int vetoedMap;
		int downloadPercent;
		bool readyForPlayback;
		int maximumPing;
		int specialFlags;
		int clanTagFeature;
		int voiceConnectivityBits;
		unsigned __int64 leagueTeamID;
		int leagueMemberCount;
		int searchStartUTC;
		int uploadBandwidth;
		char probation;
		char recentPlaylistEntries[3];
		unsigned int serverchallenge;
		int serverChallengeDeadline;
	};

	struct __declspec(align(8)) playerInfo_t
	{
		bool synchronize;
		unsigned __int64 xuid;
		PartyMember pm;
		int natType;
		bool unregisterOldClientFromSession;
		netadr_t remoteAddress;
		char name[32];
	};
	struct rateBoostingStats_t
	{
		int bytesSent;
		int serverMS;
		int clientPings;
		int clientAckFramesBehind;
		int clientUsercmdTimeDiff;
		int numClientFrames;
		int numFrames;
	};
	struct entityShared_t
	{
		char linked;
		char bmodel;
		char svFlags;
		char inuse;
		int broadcastTime;
		vec3_t mins;
		vec3_t maxs;
		int contents;
		vec3_t absmin;
		vec3_t absmax;
		vec3_t currentOrigin;
		vec3_t currentAngles;
		EntHandle ownerNum;
		int eventTime;
	};

	struct $0FC7FBA79DFCD55B13C79CF33C06C3AC
	{
		unsigned __int32 inuse : 1;
		unsigned __int32 useable : 1;
		unsigned __int32 ignoreTriggers : 1;
		unsigned __int32 pushable : 1;
		unsigned __int32 ikPriority : 1;
		unsigned __int32 talkToSpecies : 5;
	};

	/* 2671 */
	union $B0545D26085E3DC191D5FA9537D94CF4
	{
		$0FC7FBA79DFCD55B13C79CF33C06C3AC __s0;
		unsigned int allBits;
	};

	/* 2672 */
	struct ActorFlags
	{
		$B0545D26085E3DC191D5FA9537D94CF4 ___u0;
	};
	enum ai_state_transition_t
	{
		AIS_TRANSITION_CANONICAL = 0xFFFFFFFF,
		AIS_TRANSITION_NONE = 0x0,
		AIS_TRANSITION_SET = 0x1,
		AIS_TRANSITION_PUSH = 0x2,
		AIS_TRANSITION_POP = 0x3,
	};
	enum ai_state_t
	{
		AIS_INVALID = 0x0,
		AIS_KEEPCURRENT = 0x0,
		AIS_EXPOSED = 0x1,
		AIS_TURRET = 0x2,
		AIS_GRENADE_RESPONSE = 0x3,
		AIS_BADPLACE_FLEE = 0x4,
		AIS_COVERARRIVAL = 0x5,
		AIS_DEATH = 0x6,
		AIS_DEFAULT = 0x1,
		AIS_SETABLE_FIRST = 0x1,
		AIS_SETABLE_LAST = 0x6,
		AIS_PAIN = 0x7,
		AIS_REACT = 0x8,
		AIS_SCRIPTEDANIM = 0x9,
		AIS_CUSTOMANIM = 0xA,
		AIS_NEGOTIATION = 0xB,
		AIS_PUSHABLE_FIRST = 0x7,
		AIS_PUSHABLE_LAST = 0xB,
		AIS_COUNT = 0xC,
	};
	/* 2812 */
	struct ai_transition_cmd_t
	{
		ai_state_transition_t eTransition;
		ai_state_t eState;
	};
	enum ai_substate_t
	{
		STATE_EXPOSED_COMBAT = 0x64,
		STATE_EXPOSED_NONCOMBAT = 0x65,
		STATE_EXPOSED_REACQUIRE_MOVE = 0x66,
		STATE_EXPOSED_REACQUIRE_DONE = 0x67,
		STATE_EXPOSED_FLASHBANGED = 0x68,
		STATE_EXPOSED_REACTION = 0x69,
		STATE_DEATH_PRECLEANUP = 0xC8,
		STATE_DEATH_POSTCLEANUP = 0xC9,
		STATE_GRENADE_FLEE = 0x12C,
		STATE_GRENADE_TAKECOVER = 0x12D,
		STATE_GRENADE_COWER = 0x12E,
		STATE_GRENADE_COMBAT = 0x12F,
		STATE_GRENADE_COVERATTACK = 0x130,
		STATE_GRENADE_ACQUIRE = 0x131,
		STATE_GRENADE_THROWBACK = 0x132,
	};
	/* 2813 */
	struct ActorState
	{
		ai_state_t eState[6];
		ai_substate_t eSubState[6];
		unsigned int stateLevel;
		int iStateTime;
		int preThinkTime;
		ai_transition_cmd_t StateTransitions[13];
		unsigned int transitionCount;
		ai_state_t eSimulatedState[6];
		unsigned int simulatedStateLevel;
		actorState_s as;
	};

	/* 2814 */
	struct ActorShoot
	{
		float accuracy;
		float playerSightAccuracy;
		unsigned int missCount;
		unsigned int hitCount;
		float debugLastAccuracy;
		int lastShotTime;
		bool lastCanShootEnemyResult;
		int lastCanShootEnemyTime;
		int bPerfectAim;
		int bIgnoreLocationalDamage;
		float debugWeaponAccuracy;
	};

	/* 2815 */
	struct __declspec(align(4)) ActorOrientation
	{
		float fDesiredBodyYaw;
		float fLookPitch;
		float fLookYaw;
		vec3_t vLookForward;
		vec3_t vLookRight;
		vec3_t vLookUp;
		float yawVeloc;
		int allowPitchAngle;
		float turnRate;
		int bNotifyTurnDone;
		float maxFaceEnemyDistSq;
		char relativeDir;
		char prevRelativeDir;
		bool faceMotion;
		bool gunBlockedByWall;
		int dontTurnTime;
		bool lockScriptOrient;
		bool fixedLinkYawOnly;
	};
	enum ai_orient_mode_t
	{
		AI_ORIENT_INVALID = 0x0,
		AI_ORIENT_DONT_CHANGE = 0x1,
		AI_ORIENT_TO_MOTION = 0x2,
		AI_ORIENT_TO_ENEMY = 0x3,
		AI_ORIENT_TO_ENEMY_OR_MOTION = 0x4,
		AI_ORIENT_TO_ENEMY_OR_MOTION_SIDESTEP = 0x5,
		AI_ORIENT_TO_GOAL = 0x6,
		AI_ORIENT_COUNT = 0x7,
	};

	/* 2816 */
	struct ai_orient_t
	{
		ai_orient_mode_t eMode;
		float fDesiredLookPitch;
		float fDesiredLookYaw;
		float fDesiredBodyYaw;
	};

	/* 2817 */
	struct __declspec(align(4)) ActorPainDeath
	{
		int iPainTime;
		bool allowPain;
		bool blockingPain;
		bool allowDeath;
		bool delayedDeath;
		int iDamageTaken;
		int iDamageYaw;
		vec3_t damageDir;
		unsigned __int16 damageHitLoc;
		unsigned __int16 damageWeapon;
		unsigned __int16 damageMod;
		int deathContents;
		int bDropWeapon;
		bool forceRagdollImmediate;
		int minPainDamage;
		bool dieQuietly;
	};

	/* 2818 */
	struct __declspec(align(4)) ActorProne
	{
		unsigned __int16 animProneLow;
		unsigned __int16 animProneLevel;
		unsigned __int16 animProneHigh;
		int bProneOK;
		float fInvProneAnimLowPitch;
		float fInvProneAnimHighPitch;
		float fProneLastDiff;
		char feetDirection;
	};

	/* 2819 */
	union $A899A4A44C693354E5CF33C9EDFF92AE
	{
		float fTorsoPitch;
		float fBodyPitch;
	};

	/* 2820 */
	union $8F7A1F2A0E788339D3BE9A175DA5EAEF
	{
		float fWaistPitch;
		float fBodyRoll;
	};

	/* 2821 */
	struct actor_prone_info_s
	{
		bool bCorpseOrientation;
		bool orientPitch;
		bool prone;
		int iProneTime;
		int iProneTrans;
		float fBodyHeight;
		$A899A4A44C693354E5CF33C9EDFF92AE ___u6;
		$8F7A1F2A0E788339D3BE9A175DA5EAEF ___u7;
	};

	/* 2822 */
	struct ActorCachedInfo
	{
		int time;
		vec3_t pos;
		vec3_t dir;
	};

	/* 2823 */
	struct ActorLookAtInfo
	{
		vec3_t vLookAtPos;
		float fLookAtTurnAngle;
		float fLookAtTurnSpeed;
		float fLookAtTurnAccel;
		float fLookAtAnimYawLimit;
		float fLookAtYawLimit;
		unsigned __int16 animLookAtStraight;
		unsigned __int16 animLookAtLeft;
		unsigned __int16 animLookAtRight;
		bool bDoLookAt;
		bool bLookAtSetup;
		int iLookAtBlendEndTime;
		float fLookAtAnimBlendRate;
		float fLookAtLimitBlendRate;
	};

	/* 2824 */
	struct ActorCoverArrivalInfo
	{
		int arrivalNotifyRequested;
		int animscriptOverrideRunTo;
		vec3_t animscriptOverrideRunToPos;
		vec2_t offsetIncrement;
		int offsetAdjustCount;
		float arrivalYaw;
		EntHandle scriptedArrivalEnt;
	};

	struct __declspec(align(4)) pathnode_dynamic_t
	{
		SentientHandle pOwner;
		int iFreeTime;
		int iValidTime[3];
		int dangerousNodeTime[3];
		int inPlayerLOSTime;
		__int16 wLinkCount;
		__int16 wOverlapCount;
		__int16 turretEntNumber;
		__int16 userCount;
		bool hasBadPlaceLink;
	};
	enum nodeType
	{
		NODE_BADNODE = 0x0,
		NODE_PATHNODE = 0x1,
		NODE_COVER_STAND = 0x2,
		NODE_COVER_CROUCH = 0x3,
		NODE_COVER_CROUCH_WINDOW = 0x4,
		NODE_COVER_PRONE = 0x5,
		NODE_COVER_RIGHT = 0x6,
		NODE_COVER_LEFT = 0x7,
		NODE_COVER_PILLAR = 0x8,
		NODE_AMBUSH = 0x9,
		NODE_EXPOSED = 0xA,
		NODE_CONCEALMENT_STAND = 0xB,
		NODE_CONCEALMENT_CROUCH = 0xC,
		NODE_CONCEALMENT_PRONE = 0xD,
		NODE_REACQUIRE = 0xE,
		NODE_BALCONY = 0xF,
		NODE_SCRIPTED = 0x10,
		NODE_NEGOTIATION_BEGIN = 0x11,
		NODE_NEGOTIATION_END = 0x12,
		NODE_TURRET = 0x13,
		NODE_GUARD = 0x14,
		NODE_NUMTYPES = 0x15,
		NODE_DONTLINK = 0x15,
	};
	/* 2680 */
	union $73F238679C0419BE2C31C6559E8604FC
	{
		float nodeCost;
		int linkIndex;
	};


	struct __declspec(align(4)) pathlink_s
	{
		float fDist;
		unsigned __int16 nodeNum;
		char disconnectCount;
		char negotiationLink;
		char flags;
		char ubBadPlaceCount[5];
	};

	struct pathnode_constant_t
	{
		nodeType type;
		int spawnflags;
		unsigned __int16 targetname;
		unsigned __int16 script_linkName;
		unsigned __int16 script_noteworthy;
		unsigned __int16 target;
		unsigned __int16 animscript;
		int animscriptfunc;
		vec3_t vOrigin;
		float fAngle;
		vec2_t forward;
		float fRadius;
		float minUseDistSq;
		__int16 wOverlapNode[2];
		unsigned __int16 totalLinkCount;
		pathlink_s* Links;
	};
	struct pathnode_t;
	struct pathnode_transient_t
	{
		int iSearchFrame;
		pathnode_t* pNextOpen;
		pathnode_t* pPrevOpen;
		pathnode_t* pParent;
		float fCost;
		float fHeuristic;
		$73F238679C0419BE2C31C6559E8604FC ___u6;
	};
	struct pathnode_t
	{
		pathnode_constant_t constant;
		pathnode_dynamic_t dynamic;
		pathnode_transient_t transient;
	};
	/* 2681 */



	/* 2679 */

	/* 2825 */
	struct ActorNodeSelect
	{
		int numCoverNodesInGoal;
		int iPotentialCoverNodeCount;
		bool keepNodeDuringScriptedAnim;
		unsigned __int16 potentialCoverNode[10];
		unsigned __int16 potentialAmbushNode[32];
		int iPotentialAmbushNodeCount;
		int nextFindBestCoverTime;
		int coverSearchInterval;
		bool requestDifferentCover;
		bool keepClaimedNode;
		bool keepClaimedNodeIfValid;
		pathnode_t* pPotentialReacquireNode[10];
		int iPotentialReacquireNodeCount;
		pathnode_t* pPotentialCoverNode[32];
		bool doDangerReact;
		int dangerReactGoalTime;
		int dangerReactDuration;
	};

	/* 2766 */
	struct ActorSight
	{
		float fovDot;
		float fMaxSightDistSqrd;
		int ignoreCloseFoliage;
		bool lastEnemySightPosValid;
		vec3_t lastEnemySightPos;
		vec3_t anglesToLikelyEnemyPath;
		int faceLikelyEnemyPathNeedCheckTime;
		int faceLikelyEnemyPathNeedRecalculateTime;
		pathnode_t* faceLikelyEnemyPathNode;
		int iTraceCount;
		unsigned __int16 vis_blockers[72];
		float fovDotBusy;
		int latency;
		float upAimLimit;
		float downAimLimit;
		float rightAimLimit;
		float leftAimLimit;
	};

	/* 2826 */
	struct ActorString
	{
		unsigned __int16 properName;
		unsigned __int16 weaponName;
		unsigned __int16 primaryWeaponName;
		unsigned __int16 secondaryWeaponName;
		unsigned __int16 sideArmName;
		unsigned __int16 anim_pose;
		unsigned __int16 scriptState;
		unsigned __int16 lastScriptState;
		unsigned __int16 stateChangeReason;
		int iUseHintString;
	};

	/* 2827 */
	struct ActorAnimSets
	{
		unsigned __int16 aimLow;
		unsigned __int16 aimLevel;
		unsigned __int16 aimHigh;
		unsigned __int16 shootLow;
		unsigned __int16 shootLevel;
		unsigned __int16 shootHigh;
	};

	/* 2828 */
	struct __declspec(align(4)) scr_animscript_t
	{
		int func;
		int endFunc;
		unsigned __int16 name;
	};
	enum ai_stance_e
	{
		STANCE_BAD = 0x0,
		STANCE_STAND = 0x1,
		STANCE_CROUCH = 0x2,
		STANCE_PRONE = 0x4,
		STANCE_ANY = 0x7,
	};
	enum ai_traverse_mode_t
	{
		AI_TRAVERSE_INVALID = 0x0,
		AI_TRAVERSE_GRAVITY = 0x1,
		AI_TRAVERSE_NOGRAVITY = 0x2,
		AI_TRAVERSE_NOCLIP = 0x3,
		AI_TRAVERSE_COUNT = 0x4,
	};
	enum ai_animmode_t
	{
		AI_ANIM_UNKNOWN = 0x0,
		AI_ANIM_MOVE_CODE = 0x1,
		AI_ANIM_USE_POS_DELTAS = 0x2,
		AI_ANIM_USE_ANGLE_DELTAS = 0x3,
		AI_ANIM_USE_BOTH_DELTAS = 0x4,
		AI_ANIM_USE_BOTH_DELTAS_NOCLIP = 0x5,
		AI_ANIM_USE_BOTH_DELTAS_NOGRAVITY = 0x6,
		AI_ANIM_USE_BOTH_DELTAS_ZONLY_PHYSICS = 0x7,
		AI_ANIM_NOPHYSICS = 0x8,
		AI_ANIM_POINT_RELATIVE = 0x9,
	};
	/* 2829 */
	struct ActorAnimation
	{
		ai_stance_e eAllowedStances;
		unsigned __int16 AnimScriptHandle;
		scr_animscript_t* pAnimScriptFunc;
		scr_animscript_t* pPrevAnimScriptFunc;
		scr_animscript_t AnimScriptSpecific;
		ai_traverse_mode_t eTraverseMode;
		char moveMode;
		bool useCombatScriptAtCover;
		bool prevAnimScriptTerminated;
		bool safeToChangeScript;
		bool bUseGoalWeight;
		ai_animmode_t eAnimMode;
		ai_animmode_t eScriptSetAnimMode;
		float fAnimTranslationScale;
	};

	/* 2696 */
	struct pathpoint_t
	{
		vec3_t vOrigPoint;
		vec2_t fDir2D;
		float fOrigLength;
		int iNodeNum;
	};

	/* 2697 */
	struct path_t
	{
		pathpoint_t pts[32];
		__int16 wPathLen;
		__int16 wOrigPathLen;
		__int16 wDodgeCount;
		__int16 wNegotiationStartNode;
		__int16 lookaheadNextNode;
		__int16 pathChangeNotifyNode;
		__int16 wDodgeEntity;
		vec3_t vFinalGoal;
		vec3_t vStartPos;
		vec3_t lookaheadDir;
		vec3_t lookaheadPos;
		float fLookaheadDist;
		float fLookaheadAmount;
		float fLookaheadDistToNextNode;
		int minLookAheadNodes;
		int flags;
		int iPathTime;
		int iPathClearedTime;
		team_t eTeam;
		float fCurrLength;
		vec3_t vCurrPoint;
		int iPathEndTime;
		float pathEndAnimDistSq;
		bool pathEndAnimNotified;
		bool lookaheadHitsStairs;
		bool useChokePoints;
		vec2_t pathChangeTracePos;
		int randomPercent;
		int owner;
		float physRadius;
		float physHeight;
	};

	/* 2830 */
	struct path_trim_t
	{
		int iIndex;
		int iDelta;
	};
	enum ai_badplace_t
	{
		AI_BADPLACE_NONE = 0x0,
		AI_BADPLACE_NORMAL = 0x1,
		AI_BADPLACE_REALLYBAD = 0x2,
	};
	/* 2831 */
	struct actor_t;
	struct gentity_s;
	struct __declspec(align(4)) ActorNavigation
	{
		float fWalkDist;
		float fWalkDistFacingMotion;
		float badPlaceAwareness;
		path_t Path;
		path_trim_t TrimInfo;
		int iFollowMin;
		int iFollowMax;
		float fInterval;
		int pathWaitTime;
		int iTeamMoveWaitTime;
		int iTeamMoveDodgeTime;
		int stoppedWaitStartTime;
		EntHandle stoppedWaitEnt;
		actor_t* pPileUpActor;
		gentity_s* pPileUpEnt;
		int bDontAvoidPlayer;
		float sideMove;
		bool noDodgeMove;
		int mayMoveTime;
		float nodeOffsetDist;
		vec3_t nodeOffsetPos;
		vec2_t prevMoveDir;
		float leanAmount;
		EntHandle pCloseEnt;
		ai_badplace_t aiBadPlace;
		bool isInBadPlace;
		char badplaceRecheckPathLen;
	};
	enum AlertLevel
	{
		AI_ALERTNESS_ASLEEP = 0x0,
		AI_ALERTNESS_NONCOMBAT = 0x1,
		AI_ALERTNESS_ALERT = 0x2,
		AI_ALERTNESS_COMBAT = 0x3,
		NUM_ALERT_LEVELS = 0x4,
	};

	/* 309 */
	enum CombatMode
	{
		AI_COMBAT_COVER = 0x0,
		AI_COMBAT_NO_COVER = 0x1,
		AI_COMBAT_COVER_ONLY = 0x2,
		AI_COMBAT_AMBUSH = 0x3,
		AI_COMBAT_AMBUSH_NODES_ONLY = 0x4,
		AI_COMBAT_EXPOSED_NODES_ONLY = 0x5,
		AI_COMBAT_ANY_EXPOSED_NODES_ONLY = 0x6,
		NUM_COMBAT_MODES = 0x7,
	};
	/* 2832 */
	struct ActorCombat
	{
		int exposedStartTime;
		int exposedDuration;
		bool provideCoveringFire;
		scr_animscript_t* pAttackScriptFunc;
		float pathEnemyLookahead;
		float pathEnemyFightDist;
		float meleeAttackDist;
		bool useEnemyGoal;
		bool useMeleeAttackSpot;
		vec3_t goodShootPos;
		int goodShootPosValid;
		float engageMinDist;
		float engageMinFalloffDist;
		float engageMaxDist;
		float engageMaxFalloffDist;
		bool noGrenadeReturnThrow;
		AlertLevel alertLevel;
		CombatMode combatMode;
		int exposedResumeTime;
		int stopPathTime;
		int ambushStartTime;
		bool doingAmbush;
		bool currentAmbushNodeInvalid;
		bool allAmbushNodesFailed;
		bool noAttackerAccuracyMod;
		bool canFlank;
		bool isWounded;
		bool damageShield;
		float frontShieldAngleCos;
	};

	/* 2833 */
	struct __declspec(align(4)) ActorReact
	{
		int iReactTime;
		int nextAllowedReactTime;
		bool allowReact;
		vec3_t reactOrigin;
		int lastHitTime;
		float newEnemyReactionDistSq;
		vec3_t newEnemyReactionPos;
		bool newEnemyReaction;
	};

	/* 2767 */
	struct potential_threat_t
	{
		bool isEnabled;
		vec2_t direction;
	};



	/* 2768 */
	struct ActorSecondaryTarget
	{
		EntHandle entity;
		vec2_t dirToEnt;
		float distToEnt;
		pathnode_t* node;
	};

	/* 2769 */
	struct ActorThreat
	{
		int hasThreateningEnemy;
		int bPacifist;
		int iPacifistWait;
		potential_threat_t potentialThreat;
		int threatUpdateTime;
		SentientHandle pFavoriteEnemy;
		int bDrawOnCompass;
		int bActivateCrosshair;
		float highlyAwareRadius;
		bool ignoreExplosionEvents;
		int numSecondaryTarget;
		ActorSecondaryTarget secondaryTargets[2];
		bool allEnemiesInSimilarDir;
		int goodEnemyOnly;
		float footstepDetectDistSq;
		float footstepDetectDistWalkSq;
		float footstepDetectDistSprintSq;
	};

	/* 2834 */
	struct ActorGrenade
	{
		float grenadeAwareness;
		EntHandle pGrenade;
		unsigned __int16 GrenadeTossMethod;
		int bGrenadeTossValid;
		int bGrenadeTargetValid;
		int iGrenadeAmmo;
		vec3_t vGrenadeTossPos;
		vec3_t vGrenadeTargetPos;
		vec3_t vGrenadeTossVel;
		EntHandle throwBackGrenadeKilledOriginalOwner;
		int bThrowbackGrenades;
		EntHandle pGrenadeFlee;
		int flashBanged;
		float flashBangedStrength;
		int flashBangImmunity;
		int grenadeExistTime;
		Weapon GrenadeWeapon;
		bool grenadeTossWithBounce;
		vec3_t pickupPos;
	};

	/* 2835 */
	struct __declspec(align(2)) ActorTurret
	{
		gentity_s* pTurret;
		unsigned __int16 turretAnim;
		char turretAnimSet;
	};

	/* 2836 */
	struct actor_goal_s
	{
		vec3_t pos;
		vec3_t ang;
		float radius;
		float height;
		pathnode_t* node;
		gentity_s* volume;
	};
	enum aiGoalSources
	{
		AI_GOAL_SRC_SCRIPT_GOAL = 0x0,
		AI_GOAL_SRC_SCRIPT_ENTITY_GOAL = 0x1,
		AI_GOAL_SRC_ENEMY = 0x2,
	};
	/* 2837 */
	struct ActorGoal
	{
		actor_goal_s codeGoal;
		aiGoalSources codeGoalSrc;
		actor_goal_s scriptGoal;
		EntHandle scriptGoalEnt;
		bool goalPosChanged;
		bool commitToFixedNode;
		bool ignoreForFixedNodeSafeCheck;
		bool fixedNode;
		float fixedNodeSafeRadius;
		float fixedNodeSafeVolumeRadiusSq;
		EntHandle fixedNodeSafeVolume;
		int moveHistoryIndex;
		bool moveHistoryConsistent;
		vec2_t moveHistory[10];
	};

	/* 2838 */
	struct ActorSuppression
	{
		int ignoreSuppression;
		int suppressionWait;
		int suppressionDuration;
		int suppressionStartTime;
		float suppressionMeter;
	};
	enum DelayedWeapDropState
	{
		ACTOR_WEAP_DROP_NONE = 0x0,
		ACTOR_WEAP_DROP_GRAB_INITIAL_VALUE = 0x1,
		ACTOR_WEAP_DROP_FINISH = 0x2,
	};
	/* 2839 */
	struct ActorDelayedWeaponDrop
	{
		vec3_t matrix[4];
		int time;
		unsigned __int16 tagName;
		DelayedWeapDropState state;
		int weaponIndex;
	};

	/* 2770 */
	struct vis_cache_t
	{
		bool bVisible;
		int iLastUpdateTime;
		int iLastVisTime;
	};

	/* 2771 */
	struct __declspec(align(4)) sentient_info_t
	{
		vis_cache_t VisCache;
		int iLastAttackMeTime;
		int lastKnownPosTime;
		int attackTime;
		bool surprised;
		vec3_t vLastKnownPos;
		pathnode_t* pLastKnownNode;
		int iPathTestTime;
		bool bPathTestResult;
	};
	struct __declspec(align(4)) sentient_t
	{
		gentity_s* ent;
		team_t eTeam;
		EntHandle scriptOwner;
		int iThreatBias;
		int iThreatBiasGroupIndex;
		bool bIgnoreMe;
		bool bIgnoreAll;
		bool bIgnoreForFriendlyFire;
		bool originChanged;
		vec3_t oldOrigin;
		float maxVisibleDist;
		float surprisedByMeDistSq;
		int iEnemyNotifyTime;
		int attackerCount;
		EntHandle lastAttacker;
		EntHandle syncedMeleeEnt;
		EntHandle targetEnt;
		EntHandle scriptTargetEnt;
		unsigned __int16 scriptTargetTag;
		float entityTargetThreat;
		int meleeAttackerSpot[4];
		float attackerAccuracy;
		bool ignoreRandomBulletDamage;
		bool turretInvulnerability;
		pathnode_t* pClaimedNode;
		pathnode_t* pPrevClaimedNode;
		pathnode_t* pNearestNode;
		char bNearestNodeValid;
		char bNearestNodeBad;
		vec3_t vNearestNodeCheckPos;
		bool inuse;
		int banNodeTime;
		bool bInMeleeCharge;
	};

	/* 2840 */
	struct ai_suppression_t
	{
		int iTime;
		sentient_t* pSuppressor;
		vec3_t clipPlane;
		int movementOnly;
	};
	enum aiphys_t
	{
		AIPHYS_BAD = 0x0,
		AIPHYS_NORMAL_ABSOLUTE = 0x1,
		AIPHYS_NORMAL_RELATIVE = 0x2,
		AIPHYS_NOCLIP = 0x3,
		AIPHYS_NOGRAVITY = 0x4,
		AIPHYS_ZONLY_PHYSICS_RELATIVE = 0x5,
		AIPHYS_ZONLY_PHYSICS_ABSOLUTE = 0x6,
	};
	/* 2843 */
	/* 2568 */
	union CollisionAabbTreeIndex
	{
		int firstChildIndex;
		int partitionIndex;
	};

	/* 2569 */
	struct CollisionAabbTree
	{
		vec3_t origin;
		unsigned __int16 materialIndex;
		unsigned __int16 childCount;
		vec3_t halfSize;
		CollisionAabbTreeIndex u;
	};
	struct cplane_s
	{
		vec3_t normal;
		float dist;
		char type;
		char signbits;
		char pad[2];
	};

	/* 1565 */
	struct cbrushside_t
	{
		cplane_s* plane;
		int cflags;
		int sflags;
	};

	struct __declspec(align(8)) cbrush_t
	{
		vec3_t mins;
		int contents;
		vec3_t maxs;
		unsigned int numsides;
		cbrushside_t* sides;
		int axial_cflags[2][3];
		int axial_sflags[2][3];
		unsigned int numverts;
		vec3_t* verts;
	};
	//struct __declspec(align(16)) colgeom_visitor_inlined_t300 //wrong, cppobj
	//{
	//	int nprims;
	//	bool overflow;
	//	col_prim_t prims[300];
	//};

	//struct gjk_base_t
	//{
	//	__declspec(align(16)) phys_vec3 m_aabb_mn_loc;
	//	phys_vec3 m_aabb_mx_loc;
	//	unsigned int m_gjk_geom_id;
	//	phys_mat44* m_xform_;
	//	gjk_base_t* m_next_geom;
	//	unsigned int m_flags;
	//	int stype;
	//	int m_contents;
	//};


	//const struct gjkcc_input_t
	//{
	//	unsigned int gjkcc_id;
	//	bool is_server_thread;
	//	colgeom_visitor_inlined_t300 proximity_data;
	//	int proximity_mask;
	//	int m_ent_num;
	//	unsigned int m_gjk_query_flags;
	//	char pad[0x50];//gjk_base_t* m_gjk_cg;  //char pad[4];
	//	phys_mat44* m_mat;
	//	gjccc_create_t* create_info;
	//};

	enum AI_STAIRS_STATE
	{
		AI_STAIRS_NONE = 0x0,
		AI_STAIRS_UP = 0x1,
		AI_STAIRS_DOWN = 0x2,
		NUM_STAIRS_STATES = 0x3,
	};

	struct __declspec(align(8)) actor_physics_t
	{
		vec3_t vOrigin;
		vec3_t vVelocity;
		unsigned __int16 groundEntNum;
		int iFootstepTimer;
		int bHasGroundPlane;
		float groundplaneSlope;
		int iSurfaceType;
		vec3_t vWishDelta;
		int bIsAlive;
		int iEntNum;
		aiphys_t ePhysicsType;
		float fGravity;
		int iMsec;
		vec3_t vMins;
		vec3_t vMaxs;
		bool prone;
		int iTraceMask;
		int foliageSoundTime;
		int iNumTouch;
		int iTouchEnts[32];
		int iHitEntnum;
		vec3_t vHitOrigin;
		vec3_t vHitNormal;
		char bStuck;
		char bDeflected;
		char pad[0x24];//gjkcc_input_t* m_gjkcc_input;
		char _pad[0x90F];//colgeom_visitor_inlined_t300 proximity_data;

		bool pathGoingDown;
		AI_STAIRS_STATE stairsState;
		int groundEntityTimestamp;
	};

	/* 2844 */
	struct ActorCoverInfluenceInfo
	{
		pathnode_t* node;
		float weightAdjust;
		int timeAdded;
	};
	enum AISpecies
	{
		AI_SPECIES_DOG = 0x0,
		AI_SPECIES_BIGDOG = 0x1,
		AI_SPECIES_ZOMBIE = 0x2,
		AI_SPECIES_ZOMBIE_DOG = 0x3,
		MAX_AI_SPECIES = 0x4,
		AI_SPECIES_ALL = 0x4,
	};

	/* 2673 */
	struct actor_t
	{
		ActorFlags flags;
		gentity_s* ent;
		sentient_t* sentient;
		AISpecies species;
		ActorState state;
		ActorShoot shoot;
		ActorOrientation orientation;
		ai_orient_t CodeOrient;
		ai_orient_t ScriptOrient;
		ActorPainDeath painDeath;
		ActorProne prone;
		actor_prone_info_s ProneInfo;
		ActorCachedInfo eyeInfo;
		ActorCachedInfo muzzleInfo;
		ActorLookAtInfo lookAtInfo;
		ActorCoverArrivalInfo arrivalInfo;
		ActorNodeSelect nodeSelect;
		ActorSight sight;
		ActorString string;
		ActorAnimSets animSets;
		ActorAnimation anim;
		ActorNavigation navigation;
		ActorCombat combat;
		ActorReact react;
		ActorThreat threat;
		ActorGrenade grenade;
		ActorTurret turret;
		ActorGoal goal;
		ActorSuppression suppression;
		ActorDelayedWeaponDrop weapDrops[2];
		sentient_info_t sentientInfo[72];
		ai_suppression_t Suppressant[4];
		__declspec(align(16)) actor_physics_t Physics;
		const char* pszDebugInfo;
		ActorCoverInfluenceInfo cover_influencers[8];
	};

	struct DestructibleBurnData
	{
		int burnTime;
		unsigned int fx;
		int sndId;
	};

	struct DESTRUCTIBLE_PIECE_INFO
	{
		__int16 health;
		int xdollHandle;
		unsigned int fx;
		DestructibleBurnData burnData;
	};
	struct PhysPreset
	{
		const char* name;
		int flags;
		float mass;
		float bounce;
		float friction;
		float bulletForceScale;
		float explosiveForceScale;
		const char* sndAliasPrefix;
		float piecesSpreadFraction;
		float piecesUpwardVelocity;
		int canFloat;
		float gravityScale;
		vec3_t centerOfMassOffset;
		vec3_t buoyancyBoxMin;
		vec3_t buoyancyBoxMax;
	};

	union $15188755ECCB7FF50C7B622D9D67D228
	{
		char pad[0x124];
		char _pad[0x124];
		//FxElemDef* elemDefs;
		//FxElemDef* localElemDefs;
	};
	struct FxFloatRange
	{
		float base;
		float amplitude;
	};

	const struct FxEffectDef
	{
		const char* name;
		unsigned __int16 flags;
		char efPriority;
		__int16 elemDefCountLooping;
		__int16 elemDefCountOneShot;
		__int16 elemDefCountEmission;
		int totalSize;
		int msecLoopingLife;
		int msecNonLoopingLife;
		$15188755ECCB7FF50C7B622D9D67D228 ___u9;
		vec3_t boundingBoxDim;
		vec3_t boundingBoxCentre;
		float occlusionQueryDepthBias;
		int occlusionQueryFadeIn;
		int occlusionQueryFadeOut;
		FxFloatRange occlusionQueryScaleRange;
	};

	struct DestructibleStage
	{
		unsigned __int16 showBone;
		float breakHealth;
		float maxTime;
		unsigned int flags;
		FxEffectDef* breakEffect;
		const char* breakSound;
		const char* breakNotify;
		const char* loopSound;
		char _pad[0xF8];//XModel* spawnModel[3];
		PhysPreset* physPreset;
	};

	struct DestructiblePiece
	{
		DestructibleStage stages[5];
		char parentPiece;
		float parentDamagePercent;
		float bulletDamageScale;
		float explosiveDamageScale;
		float meleeDamageScale;
		float impactDamageScale;
		float entityDamageTransfer;
		char pad[0xA88];
		//PhysConstraints* physConstraints;
		int health;
		const char* damageSound;
		FxEffectDef* burnEffect;
		const char* burnSound;
		unsigned __int16 enableLabel;
		int hideBones[5];
	};

	struct DestructibleDef
	{
		const char* name;
		char _pad[0xF8];
		char pad[0xF8];
		//XModel* model;
		//XModel* pristineModel;
		int numPieces;
		DestructiblePiece* pieces;
		int clientOnly;
	};

	struct __declspec(align(4)) Destructible
	{
		int entNum;
		DESTRUCTIBLE_PIECE_INFO* pieceArray;
		int oldestBurnTime;
		char destructiblePoseID;
		int pieceCount;
		DestructibleDef* ddef;
		unsigned int flags;
		char pad[0x10];
		//DestructibleState states[5];
		char bHasBeenHit;
	};

	struct flame_timed_damage_t
	{
		gentity_s* attacker;
		int damage;
		float damageDuration;
		float damageInterval;
		int start_timestamp;
		int end_timestamp;
		int lastupdate_timestamp;
	};
	struct $8B2B5A2D6EB549120AB558355B552286
	{
		unsigned __int16 notifyString;
		unsigned int index;
		char stoppable;
		int basetime;
		int duration;
	};
	struct tagInfo_t
	{
		gentity_s* parent;
		gentity_s* next;
		unsigned __int16 name;
		bool blendToParent;
		bool blendOnlyYaw;
		bool collisionPhysics;
		int index;
		vec3_t axis[4];
		vec3_t parentInvAxis[4];
	};
	struct gclient_t;
	struct gentity_s
	{
		entityState_s s;
		entityShared_t r;
		gclient_t* client;
		actor_t* actor;
		sentient_t* sentient;
		TurretInfo* pTurretInfo;
		Destructible* destructible;
		void* vehicle; //char pad[0x2260];//vehicle_t* vehicle;
		unsigned __int16 model;
		char physicsObject;
		char takedamage;
		char active;
		char nopickup;
		char handler;
		char team;
		char avoidHandle;
		unsigned __int16 classname;
		unsigned __int16 target;
		unsigned __int16 targetname;
		unsigned __int16 script_noteworthy;
		unsigned int attachIgnoreCollision;
		int spawnflags;
		int flags;
		int eventTime;
		int freeAfterEvent;
		int unlinkAfterEvent;
		int clipmask;
		int processedFrame;
		EntHandle parent;
		int nextthink;
		int health;
		int maxHealth;
		int damage;
		flame_timed_damage_t flame_timed_damage[4];
		int last_timed_radius_damage;
		int count;
		char _pad[0x60];//$D054511B64C7859A2972E5005FF506FD ___u36;
		EntHandle missileTargetEnt;
		$8B2B5A2D6EB549120AB558355B552286 snd_wait;
		tagInfo_t* tagInfo;
		gentity_s* tagChildren;
		unsigned __int16 attachModelNames[19];
		unsigned __int16 attachTagNames[19];
		char __pad[0x4];//XAnimTree_s* pAnimTree;
		unsigned __int16 disconnectedLinks;
		int iDisconnectTime;
		int useCount;
		int physObjId;
		gentity_s* nextFree;
		int birthTime;
		int ikPlayerclipTerrainTime;
		int ikDisableTerrainMappingTime;
	};

	static_assert(sizeof(gentity_s) == 0x31C);

	struct client_s
	{
		clientHeader_t header;
		const char* dropReason;
		char userinfo[1024];
		char reliableCommandBuffer[16384];
		int reliableCommandBufferNext;
		svscmd_info_t reliableCommandInfo[128];
		int reliableSequence;
		int reliableAcknowledge;
		int reliableSent;
		int messageAcknowledge;
		int gamestateMessageNum;
		int challenge;
		usercmd_s lastUsercmd;
		int lastClientCommand;
		char lastClientCommandString[1024];
		gentity_s* gentity;
		char name[32];
		char clanAbbrev[5];
		int clanAbbrev_IsEliteValidated;
		unsigned __int64 xuid;
		unsigned __int64 teamid;
		unsigned int doubleXPGroupMask;
		int nextReliableTime;
		int nextReliableCount;
		char reservedSlot;
		int lastPacketTime;
		int lastConnectTime;
		int nextSnapshotTime;
		int lastSnapshotTime;
		int timeoutCount;
		clientSnapshot_t frames[16];
		int ping;
		int pingMin;
		int pingMax;
		int unackCount;
		int rate;
		int pureAuthentic;
		int packetUploadCount;
		__declspec(align(128)) char netchanOutgoingBuffer[65536];
		char netchanIncomingBuffer[2048];
		int guid;
		unsigned __int16 scriptId;
		bool bIsSplitscreenClient;
		bool bIsSecondScreenClient;
		int bIsTestClient;
		int serverId;
		int natType;
		char stats[50176];
		char purchasedItems[32];
		int unlockedAttachments[64];
		char bonusCardsActive[10];
		char modifiedStatBytes[6272];
		int statsSentIndex;
		int statsModified;
		__int64 statPacketsReceived;
		int statsValidated;
		bool liveAuthorized;
		int liveAuthFlags;
		char liveAuthFailCount;
		int liveAuthTimestamp;
		unsigned __int64 liveUserID;
		bool tempPacketDebugging;
		int snapshotHistoryTime;
		int snapshotHistoryFrames;
		int messageHistorySize;
		int demoPacketCount;
		int demoPacketSizes[8];
		int demoPacketIsFragment[8];
		int demoReliableCmdSize;
		unsigned int lastHashedXP;
		int lastSmallDeltaTime;
		int lastSnapSentTime;
	};
	static_assert(sizeof(client_s) == 0x4E100);
	struct __declspec(align(128)) serverStatic_t
	{
		int clientCount;
		int packetBackupCount;
		int packetBackupMask;
		entityState_s* noDeltaEntities;
		entityState_s* snapshotEntities;
		int numNoDeltaEntities;
		int nextNoDeltaEntity;
		cachedSnapshot_t* cachedSnapshotFrames;
		archivedEntity_s* cachedSnapshotEntities;
		serverSnapshot_t snapshots[16];
		int serverSnapshotTimes[16];
		int nextServerSnapshotIndex;
		int baselineCreated;
		int initialized;
		int time;
		int physicsTime;
		int oldestArchiveTime;
		int snapFlagServerBit;
		client_s* clients;
		int numCachedSnapshotMatchStates;
		int numCachedSnapshotEntities;
		int numCachedSnapshotClients;
		int numCachedSnapshotActors;
		int numSnapshotMatchStates;
		int numSnapshotEntities;
		int numSnapshotClients;
		int numSnapshotActors;
		int nextSnapshotMatchStates;
		int nextSnapshotEntities;
		int nextSnapshotClients;
		int nextSnapshotActors;
		MatchState* snapshotMatchStates;
		clientState_s* snapshotClients;
		actorState_s* snapshotActors;
		int nextArchivedSnapshotFrames;
		archivedSnapshot_s* archivedSnapshotFrames;
		char* archivedSnapshotBuffer;
		int nextArchivedSnapshotBuffer;
		int nextCachedSnapshotMatchStates;
		int nextCachedSnapshotEntities;
		int nextCachedSnapshotClients;
		int nextCachedSnapshotActors;
		int nextCachedSnapshotFrames;
		MatchState* cachedSnapshotMatchStates;
		cachedClient_s* cachedSnapshotClients;
		cachedActor_s* cachedSnapshotActors;
		int nextHeartbeatTime;
		int nextStatusResponseTime;
		challenge_t challenges[1024];
		netadr_t redirectAddress;
		netProfileInfo_t OOBProf;
		tempBanSlot_t tempBans[16];
		unsigned __int64 xuids[18];
		playerInfo_t playerInfos[18];
		vec3_t mapCenter;
		vec3_t mapCenterBoundsMin;
		vec3_t mapCenterBoundsMax;
		int dataSentLastSec;
		int activeEntities;
		int archivedEntityCount;
		int snapshotBoostingSuspendTimeout;
		int badServerFPSCount;
		int badServerFPSLast;
		rateBoostingStats_t rateBoostingStats;
		int rateBoostingEnabled;
	};

	enum sessionState_t
	{
		SESS_STATE_PLAYING = 0x0,
		SESS_STATE_DEAD = 0x1,
		SESS_STATE_SPECTATOR = 0x2,
		SESS_STATE_INTERMISSION = 0x3,
	};
	enum clientConnected_t
	{
		CON_DISCONNECTED = 0x0,
		CON_CONNECTING = 0x1,
		CON_CONNECTED = 0x2,
	};
	struct playerTeamState_t
	{
		int location;
	};

	struct __declspec(align(8)) clientSession_t
	{
		sessionState_t sessionState;
		ClientNum_t forceSpectatorClient;
		int killCamEntity;
		int killCamTargetEntity;
		int archiveTime;
		unsigned __int16 scriptPersId;
		clientConnected_t connected;
		usercmd_s cmd;
		usercmd_s oldcmd;
		int localClient;
		int predictItemPickup;
		char newnetname[32];
		int maxHealth;
		int enterTime;
		playerTeamState_t teamState;
		int voteCount;
		int teamVoteCount;
		float moveSpeedScaleMultiplier;
		int viewmodelIndex;
		int noSpectate;
		int teamInfo;
		clientState_s cs;
		int psOffsetTime;
		int scoreboardColumnCache[26];
	};


	struct viewClamp
	{
		vec2_t start;
		vec2_t current;
		vec2_t goal;
	};

	struct viewClampState
	{
		viewClamp min;
		viewClamp max;
		float accelTime;
		float decelTime;
		float totalTime;
		float startTime;
	};

	struct gclient_t
	{
		playerState_s ps;
		playerState_s lastPlayerPS;
		clientSession_t sess;
		ClientNum_t spectatorClient;
		int flags;
		int lastCmdTime;
		bitarray button_bits;
		bitarray oldbutton_bits;
		bitarray latched_button_bits;
		bitarray button_bitsSinceLastFrame;
		float fGunPitch;
		float fGunYaw;
		int damage_blood;
		vec3_t damage_from;
		int damage_fromWorld;
		int inactivityTime;
		int inactivityWarning;
		int lastVoiceTime;
		int outWaterTime;
		int switchSeatTime;
		float currentAimSpreadScale;
		int dropWeaponTime;
		EntHandle pLookatEnt;
		vec4_t prevLinkedInvQuat;
		bool prevLinkAnglesSet;
		bool link_doCollision;
		bool link_useTagAnglesForViewAngles;
		bool link_useBaseAnglesForViewClamp;
		float linkAnglesFrac;
		viewClampState link_viewClamp;
		EntHandle useHoldEntity;
		int useHoldTime;
		int useButtonDone;
		int iLastCompassPlayerInfoEnt;
		int compassPingTime;
		int damageTime;
		float v_dmg_roll;
		float v_dmg_pitch;
		vec3_t swayViewAngles;
		vec3_t swayOffset;
		vec3_t swayAngles;
		vec3_t baseAngles;
		vec3_t baseOrigin;
		vec3_t recoilAngles;
		float fLastIdleFactor;
		int weaponIdleTime;
		vec3_t recoilSpeed;
		int previousRecoilTime;
		float previousRecoilRatio;
		int lastServerTime;
		int lastSpawnTime;
		Weapon lastWeapon;
		bool previouslyFiring;
		bool previouslyUsingNightVision;
		bool previouslyDTP;
		bool previouslyBeganWeaponRaise;
		bool previouslySprinting;
		unsigned int hasSpyplane;
		unsigned int hasSatellite;
		int revive;
		int reviveTime;
		int disallowVehicleUsage;
		unsigned __int16 attachShieldTagName;
		int lastStand;
		int lastStandTime;
	};
}