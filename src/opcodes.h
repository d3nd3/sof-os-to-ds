#define VAR_LENGTH 64

// Command Flags
// Move
#define MOVE_DURATION	0x01
#define MOVE_RATE		0x02
#define MOVE_SIGNALER	0x04
#define MOVE_ABSOLUTE	0x08

// Rotate
#define ROTATE_DURATION	0x01
#define ROTATE_RATE		0x02
#define ROTATE_SIGNALER	0x04
#define ROTATE_ABSOLUTE	0x08


// MoveRotate (disk-file command)
#define MOVEROTATE_SIGNALER 0x01

// Wait
#define WAIT_CLEAR		0x80

// Print
#define PRINT_ENTITY	0x01
#define PRINT_CENTERED	0x02
#define PRINT_LEVEL		0x04
#define PRINT_CAPTIONED	0x08

// Play Sound
#define PLAY_SOUND_ENTITY		0x01
#define PLAY_SOUND_VOLUME		0x02
#define PLAY_SOUND_ATTENUATION	0x04
#define PLAY_SOUND_CHANNEL		0x08
#define PLAY_SOUND_TIMEDELAY	0x10

// Animate
#define ANIMATE_REPEAT		0x001
#define ANIMATE_TURNING		0x002
#define ANIMATE_MOVING		0x004
#define ANIMATE_SIGNALER	0x008
#define ANIMATE_SOURCE		0x010
#define ANIMATE_MOVING_ABS	0x020
#define ANIMATE_HOLD		0x040
#define	ANIMATE_TARGET		0x080
#define ANIMATE_KILL		0x100
#define ANIMATE_NULLTARGET	0x200
#define ANIMATE_EMOTION		0x400

// Debug
#define DEBUG_ENABLE		0x01
#define DEBUG_MOVE			0x02
#define DEBUG_ROTATE		0x04
#define DEBUG_TIME			0x08
#define DEBUG_MOVEROTATE	0x10

//Helicopter
#define HELICOPTER_TARG		0x01
#define HELICOPTER_VECT		0x02
#define HELICOPTER_FLOAT	0x04
#define HELICOPTER_SIGNALER	0x08

//Tank
#define TANK_TARG			0x01
#define TANK_VECT			0x02
#define TANK_FLOAT			0x04
#define TANK_SIGNALER		0x08

//Snowcat
#define SNOWCAT_TARG		0x01
#define SNOWCAT_VECT		0x02
#define SNOWCAT_FLOAT		0x04
#define SNOWCAT_SIGNALER	0x08

enum FunctionTypes
{
	FUNC_FIND_ENTITY_WITH_TARGET = 0,
	FUNC_SIN,//1
	FUNC_COS,//2
	FUNC_FIND_ENTITY_WITH_SCRIPT,//3
	FUNC_FIND_PLAYER,//4
	FUNC_SPAWN,//5
	FUNC_GET_OTHER,//6
	FUNC_GET_ACTIVATOR,//7
	FUNC_RANDOM,//8
	FUNC_GET_PLAYER,//9
};

enum ConditionTypes
{
	COND_EQUAL = 0,
	COND_LESS_THAN,
	COND_LESS_THAN_EQUAL,
	COND_GREATER_THAN,
	COND_GREATER_THAN_EQUAL,
	COND_NOT_EQUAL,
};

enum VariableT 
{ 
	TypeINT = 0, 
	TypeFLOAT, //1
	TypeVECTOR, //2
	TypeENTITY, //3
	TypeSTRING,//4
	TypeUNKNOWN, //5
};
enum PushTypes
{
	PUSH_CONST_INT = 0,
	PUSH_CONST_FLOAT,//1
	PUSH_CONST_VECTOR,//2
	PUSH_CONST_ENTITY,//3
	PUSH_CONST_STRING,//4
	PUSH_VAR,//5
	PUSH_VAR_WITH_FIELD,//6
	PUSH_FUNCTION,//7
	PUSH_CUSTOM//8
};

enum FeatureTypes
{
	FEATURE_TRIGGER = 0,
	FEATURE_AMBIENT_SOUNDS,
	FEATURE_CINEMATICS,
	FEATURE_PLAGUE_SKINS,
};
enum OpCodes
{
	CODE_NEW_GLOBAL = 0,
	CODE_NEW_GLOBAL_PLUS_ASSIGNMENT,//1
	CODE_NEW_LOCAL,//2
	CODE_NEW_LOCAL_PLUS_ASSIGNMENT,//3
	CODE_NEW_PARAMETER,//4
	CODE_NEW_PARAMETER_PLUS_DEFAULT,//5
	CODE_FIELD,//6
	CODE_ASSIGNMENT,//7
	CODE_ADD,
	CODE_SUBTRACT,//9
	CODE_MULTIPLY,//A
	CODE_DIVIDE,
	CODE_ADD_ASSIGNMENT,
	CODE_SUBTRACT_ASSIGNMENT,
	CODE_MULTIPLY_ASSIGNMENT,
	CODE_DIVIDE_ASSIGNMENT,//F
	CODE_GOTO,//10
	CODE_PUSH,
	CODE_POP,
	CODE_IF,
	CODE_EXIT,
	CODE_SUSPEND,
	CODE_WAIT_SECONDS,
	CODE_WAIT_ALL,
	CODE_WAIT_ANY,
	CODE_MOVE,//19
	CODE_DEBUG,//1A
	CODE_USE,
	CODE_ROTATE,
	CODE_PRINT,
	CODE_PLAY_SOUND,
	CODE_ENABLE,//1F
	CODE_DISABLE,//20
	CODE_ANIMATE,
	CODE_DEBUG_STATEMENT,
	CODE_CACHE_SOUND,
	CODE_COPY_PLAYER_ATTRIBUTES,
	CODE_SET_VIEW_ANGLES,
	CODE_SET_CACHE_SIZE,
	CODE_REMOVE,
	CODE_HELICOPTER,
	CODE_MOVEROTATE,//29
	CODE_PLAYSONG,//2A
	CODE_SETCVAR,
	CODE_UNLOAD_SOUND,
	CODE_TANK,
	CODE_SNOWCAT,
	CODE_CACHE_ROFF,
	CODE_CACHE_STRING_PACKAGE,//30
	CODE_CONSOLE_COMMAND,
	CODE_UNLOAD_ROFF,
	CODE_ON,
	CODE_RESUME,
	CODE_RESET_AI,
};


#define HELI_TAKEOFF		1
#define HELI_LAND			2
#define HELI_REPAIR			3
#define HELI_REARM			4
#define HELI_GOTO_COORDS	5
#define HELI_GOTOREL_ENTITY	6
#define HELI_GOTOREL_ENT_X	7
#define HELI_GOTOREL_ENT_Y	8
#define HELI_GOTOREL_ENT_Z	9
#define HELI_MOVEREL		10
#define HELI_PAUSE			11
#define HELI_FACE_RELENT	12
#define HELI_FACE_ABSCOORDS	13
#define HELI_FACE_ABSDIR	14
#define HELI_FACE_RELCOORDS	15
#define HELI_PILOT_FACERELENT		16
#define HELI_PILOT_FACERELCOORDS	17
#define HELI_PILOT_FACEABSCOORDS	18
#define HELI_GUNNER_FACERELENT		19
#define HELI_GUNNER_FACERELCOORDS	20
#define HELI_GUNNER_FACEABSCOORDS	21
#define HELI_STRAFE_RT				22
#define HELI_STRAFE_LT				23
#define HELI_ROCKETS_ENABLE			24	
#define HELI_ROCKETS_DISABLE		25
#define HELI_CHAINGUN_ENABLE		26
#define HELI_CHAINGUN_DISABLE		27
#define HELI_FIREAT_RELENT			28
#define HELI_FIREAT_ABSCOORDS		29
#define HELI_FIREAT_RELCOORDS		30
#define HELI_AUTOFIRE_ON			31
#define HELI_AUTOFIRE_OFF			32
#define HELI_HOVER_PASSIVE			33
#define HELI_HOVER_AGGRESSIVE		34
#define HELI_SET_WORLDMINS			35
#define HELI_SET_WORLDMAXS			36
#define HELI_SET_MAXHEALTH			37
#define HELI_SET_HEALTH				38
#define HELI_SET_DEATHDEST			39
#define HELI_SET_TRACEDIMS			40
#define HELI_AI						41
#define HELI_WAYPOINT				42
#define HELI_VOLUME					43
#define HELI_VOLUMEMINS				44
#define HELI_VOLUMEMAXS				45
#define HELI_DEBUG					46
#define HELI_CHANGE_SKIN			47
#define HELI_CHANGE_BODY			48
#define HELI_OUT_OF_CONTROL			49


#define TANK_GOTOCOORDS					1
#define TANK_FIRECANNONATCOORDS			2
#define TANK_MACHGUNAUTO				3
#define TANK_DIE						4
#define TANK_AIMTURRET					5
