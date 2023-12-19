#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

#include "opcodes.h"
#include "animation_ids.h"
#include "movetypes.h"
#include "sounds.h"
#include "reverseos.h"

extern char REVERSEOS[32768];


extern char anims[356][32];
extern char nullTargets[10][32];
extern const char *movetypes[11];
extern const char* emotions[9];
extern void initAnims(void);
extern void initMovetypes(void);
extern void initSounds(void);
extern void initEmotions(void);
extern const char * findChannel(int input);
extern const char * findAtten(float input);

void errorExit(const char * txt);
void PrintStack(void);


void setupCommonDependency(void);
void parseOpcodes(char * buffer,char * end);
int PushStack(PushTypes type, char * data, int secret_size = 0);
struct stackItem * PopStack(bool reverse = false);

int handleStandardOpcode(const char *startwords,char * buffer,bool assign=false);
int handleGoto(char * buffer);
int handleAssignment(char * buffer);
int handleAddAssignment(char * buffer,const char * symbol);
int handleIf(char * buffer,bool specialOnCase=false);
int handleConsoleCommand(char * buffer);
int handlePrintMessage(char * buffer);
int handleWaitSeconds(char * buffer);
int handleWaitAllAny(char * buffer);
int handleFunction(char * buffer,struct stackItem * item);

int handleAnimate(char * buffer);
int handleRotate(char * buffer);
int handleMove(char * buffer);
int handleMoveRotate(char * buffer);
int handlePlaySound(char * buffer);
int handlePlaySong(char * buffer);
int handleOn(char * buffer);
int handleSuspend(char * buffer);
int handleCacheSound(char * buffer);
int handleCacheRoff(char * buffer);
int handleCacheStringPackage(char * buffer);
int handleEnable(char * buffer, bool enable);
int handleUnloadSound(char * buffer);
int handleUnloadRoff(char * buffer);
int handleSetCvar(char * buffer);
int handleSetViewAngles(char * buffer);
int handleResetAi(char * buffer);
int handleMath(char* buffer, const char * symbol);

int handleTank(char* buffer);
int handleHelicopter(char* buffer);



void DebugPrintf(const char * txt,...);


void saveLineNumber(char * buffer,int sizepushed=0);

void seekToEnthLine(int n,char * lineContent, int length);

void PatchUpJumps(void);

void dumpLabelReals(void);
void dumpFixUps(void);
void dumpLineOffsetTable(void);


typedef union value_u {
	int number;
	int two_numbers[2];
	float three_floats[3];
	float floatNumber;
	char * nullString;
	int variable;
} value_t;
struct stackItem {
	PushTypes type;
	value_t value;
	int secret_size;
	struct stackItem *next;
	struct stackItem *prev;
};


int realLabelsCount = 0;

typedef struct labelFixUp_s {
	struct labelFixUp_s * prev, *next;

	bool active;
	int lab_num;
	int label_offset;
	int num_endifs;
	
} labelFixUp_t;

labelFixUp_t* appendFixUp(int label_offset, int lab_num);
void removeFixUp(labelFixUp_t *which);
labelFixUp_t * getFixUp(int labelOffset);



typedef struct vars_s {
	struct vars_s * prev, *next;
	char * varName;
	int index;
} vars_t;



void appendVariable(char * name,int idx);
void removeVariable(vars_t *which);
char * getVariable(int index);

void appendField(char * name,int idx);
void removeField(vars_t *which);
char * getField(int index);


struct stackItem * top = NULL;
struct stackItem * bottom = NULL;

unsigned int stackSize = 0;


int preventXYZFields = 0;


FILE * outfile = NULL;
int lineNumbersToOffset[1024];
int lineNumbers = 0;
vars_t Variables;
vars_t Fields;
labelFixUp_t FixUps;
unsigned char *fileContents;


bool declareMode = true;
bool fieldMode = true;

char outFileName[128];
char inFileName[128];
char newline[8];

char depPath[128];
int main ( int argc, char ** argv) {
	setvbuf(stdout, NULL, _IONBF, 0);

	// printf("Received %i arguments\n",argc);
	// for ( int g = 0 ; g < argc; g++ ) {
	// 	printf("%s\n",argv[g]);
	// }


	printf("DesignerScript Decompiler V3.0  (Dec 18 2023 16:18)\n\
		sofos.exe infile.os [outfile.ds]\nargs in [] are optional\n");


	//2 means that a valid infile was listed
	if( argc >= 2 ) {
		strcpy(inFileName,argv[1]);
		strcpy(outFileName,"decompiled.ds");
		strcpy(depPath,".\\");
	}else {
		//1 or less arguments
		printf("Please pass the file to be decompiled as an argument\n");
		printf("eg. ./prog.exe myfile.os\n");
		printf("If you want to control the output filename, then supply an extra argument\n");
		printf("eg. ./prog.exe myfile.os outfile.ds\n");
		return 1;
	}
	bool option = false;

	//processing them using 'order'
	int args_found = 0;
	int opts_found = 0;
	int args_array[32];
	int opts_array[32];
	// printf("size of array = %i\n",sizeof(args_array));
	memset(&args_array[0],0x00,sizeof(args_array));
	memset(&opts_array[0],0x00,sizeof(opts_array));
	//start processing at 2
	for ( int q = 2 ; q < argc; q++ ) {
		if ( option == true ) {
			option = false;
			//save option here
			opts_array[opts_found] = q;

			opts_found++;
			continue;
		} else {
			if ( argv[q] == strstr(argv[q],"-dep_path") ) {
				option = true;
			} else {
				//save argument here
				args_array[args_found] = q;
				//its not an option so what is it...


				args_found++;
			}
		}
	}


	//first arg/option
	if ( args_array[0] > 0 ) {
		strcpy(outFileName,argv[args_array[0]]);
		// printf("outfile is %s\n",outFileName);
	}


	initAnims();
	initMovetypes();
	initSounds();
	initEmotions();


	strcpy(newline,"\r\n");
	
	
	char * cc = &inFileName[strlen(inFileName)-2];
	if ( strcmp(cc,"os") ) {
		printf("ERROR: The input file does not have os extension\n");
		return 1;
	}

	FILE * infile = fopen(inFileName,"rb");
	if ( infile == NULL ) {
		printf("ERROR: Failed to open the input file\n");
		return 1;
	}

	fseek(infile, 0L, SEEK_END);
	int size = ftell(infile);
	rewind(infile);

	fileContents = (unsigned char*)malloc(size);	
	fread(fileContents,1,size,infile);
	fclose(infile);
	int i = 0;
	for (i = 0;i<size;i++) {
		//printf(" %02X ",fileContents[i]);
	}
	//printf("\n");

	outfile = fopen(outFileName,"w+b");
	if ( outfile == NULL ) {
		printf("ERROR: Failed to open the output file\n");
		return 1;
	}
	
	printf("INFO: Input file : \"%s\"\n",inFileName);
	printf("INFO: Trying to make file at : \"%s\"\n",outFileName);

	// variableName to variableIndex mappings
	Variables.next = Variables.prev = &Variables;
	Variables.index = -1;

	Fields.next = Fields.prev = &Fields;
	Fields.index = -1;
	// Goto Label Mappings // offset to LineNumber
	FixUps.next = FixUps.prev = &FixUps;
	FixUps.active = false;

	// 4
	// printf("%i <= %i\n",fileContents+4,fileContents+(size-1));


	if ( *(int*)fileContents != 3 ) {
		printf("WARNING: This might not compile, it has an old format!\n");
		// return 1;
	}
	// fileContents +=4;

	// printf("Parsing Opcodes");	
	parseOpcodes((char*)fileContents+4,(char*)fileContents+(size-1));
	setupCommonDependency();


	fclose(outfile);

	return 0;
}

unsigned char checksum (unsigned char *ptr, size_t sz) {
	unsigned char chk = 0;
	while (sz-- != 0)
		chk -= *ptr++;
	return chk;
}


void writeHelperFile(void) {
	FILE  * depend = fopen("helper.ds","w+b");
	if ( depend == NULL ) {
		printf("WARNING: failed to write helper file\n");
	} else {
		printf("INFO: Writing helper file \"helper.ds\" becasue it does not exist%s",newline);
		//write the file
		fprintf(depend,REVERSEOS);

		fclose(depend);	
	}	
}

/*
	Insert #include reverseos.ds 
	Which is based on common/header.ds
*/
void setupCommonDependency(void) {
	// printf("SetupCommonDependency\n");

	char dependency[128];
	// char filepath[128];
	// sprintf(filepath,"%s%s",depPath,"helper.ds");
	sprintf(dependency,"#include \"helper.ds\"%s",newline);
	//this does not write null character
	seekToEnthLine(0,dependency,strlen(dependency));


	// printf("file path for helper = %s\n",filepath);

	//try to see if it already exists
	FILE * helper = fopen("helper.ds","rb");
	if ( helper) {
		//get size
		fseek(helper, 0L, SEEK_END);
		int size = ftell(helper);
		if ( size > 0 ) {
			//checksum it
			rewind(helper);
			
			//read it
			unsigned char * helperfile = (unsigned char*)malloc(size);
			fread(helperfile,1,size,helper);
			fclose(helper);
			unsigned char check = checksum(helperfile,size);
			// 	printf("check is %02X\n",check);
			if ( check == HELPER_CHECK ) {
				//the file is valid
			} else {
				//write a valid file
				writeHelperFile();
			}
		}

	} else {
		//it does not exist so create one.
		writeHelperFile();
	}
}


void PrintStack(void) {
	struct stackItem * init = bottom;
	for ( int i = 0 ; i < stackSize; i++ ) {
		printf("stackitem : type %i\n",bottom);
		init = init->next;
	}
}

/*
item removed from either top or bottom of stack
*/
struct stackItem * PopStack(bool reverse) {
	if ( reverse && bottom == NULL ) {
		return NULL;
	}
	if ( !reverse && top == NULL ) {
		return NULL;
	}
	if ( stackSize == 0 ) {
		errorExit("trying to pop from 0 stack size\n");
	}
	stackSize--;
	struct stackItem * pop = NULL;
	//remove from bottom
	if ( reverse ) {
		pop = bottom;
		//size > 1
		if ( bottom->next ) {
			//this becomes the new bottom
			bottom->next->prev = NULL;
			bottom = bottom->next;
		} else {
			// no next
			// stack empty?
			bottom = NULL;
			top = NULL;
		}
	} else {
		//remove from top
		pop = top;
		if ( top->prev ) {
			top->prev->next = NULL;
			top = top->prev;
		} else {
			// no previous
			// stack empty?
			bottom = NULL;
			top = NULL;
		}
	}
	
	return pop;
}

void errorExit(const char * txt) {
	printf(txt);
	exit(1);
}


// Make sure you PopStack before Pushing Stack, else you Pop Yourself. (for popping inside this func.)
/*
opcode
type
data
*/

// received data .. from file .. and we are storing it...
//pop means discard from storage
int PushStack(PushTypes type, char * data, int secret_size) {
	int ret = 0;
	struct stackItem * newItem = (struct stackItem*)malloc(sizeof(struct stackItem));
	newItem->type = type;
	newItem->secret_size = secret_size;
	newItem->next = NULL;

	// handle stuff which Pops stack first.
	if ( type == PUSH_FUNCTION ) {
		//but this file generates data instead of receives it. because functions do stuff with data.
		//i just have to remember how much data this function parsed.
		//the thing we push here is result from the function
		ret += handleFunction(data,newItem);
	}

	if ( top != NULL ) {
		//attach him
		top->next = newItem;
		//link us to him
		newItem->prev = top;

	} else {
		//alone
		bottom = newItem;
		newItem->prev = NULL;
	}
	//make us top now.
	top = newItem;

	stackSize++;

	// char fourbytes[4];
	// memcpy(&fourbytes[0],data,4);

	int len = 0;
	switch ( type ) {
	case PUSH_CONST_INT:
		newItem->value.number = *(int*)data;
		ret = 4;
		break;
	case PUSH_CONST_FLOAT:
		newItem->value.floatNumber = *(float*)data;
		ret = 4;
		break;
	case PUSH_CONST_VECTOR:
			//3 floats
		newItem->value.three_floats[0] = *(float*)data;
		data+=4;
		newItem->value.three_floats[1] = *(float*)data;
		data+=4;
		newItem->value.three_floats[2] = *(float*)data;
		ret = 12;
		break;
	case PUSH_CONST_ENTITY:
		errorExit("PUSH Entity??\n");
		break;
	case PUSH_CONST_STRING:
		len = strlen(data) + 1;
		newItem->value.nullString = (char*)malloc(len);
		strcpy(newItem->value.nullString,(char*)data);
		return len;
		break;
	case PUSH_VAR:
		newItem->value.number = *(int*)data;
		ret = 4;
		break;
	case PUSH_VAR_WITH_FIELD:
			//2 ints?
		newItem->value.two_numbers[0] = *(int*)data;
		data+=4;
		newItem->value.two_numbers[1] = *(int*)data;
		ret = 8;
			// errorExit("PUSH_VAR_WITH_FIELD??\n");
		break;

		//this is very similar to pushFunction, but its created by us instead of the file... (math funcs)
	case PUSH_CUSTOM:
		len = strlen(data) + 1;
		newItem->value.nullString = (char*)malloc(len);
		strcpy(newItem->value.nullString,(char*)data);
		return len;
		break;
	}

	return ret;
}

bool DEBUG_MODE = true;
void DebugPrintf(const char * txt,...) {
	va_list args;
	va_start(args,txt);
	if ( DEBUG_MODE == true ) { 
		// char argz[256];
		vprintf(txt,args);
	}
	va_end(args);
}

void parseOpcodes(char * buffer,char * end) {
	char previousOpcode;

	bool CODE_ON_MODE = false;
	int CODE_ON_COUNT = 0;
	//printf("entering parseopcodes\n");
	struct stackItem * message;
	//if bufffer is not 0x14 CODE_EXIT at end, then dont treat it like opcode
	while( buffer <= end ) {
		if (buffer == end && *buffer!=0x14) {
			DebugPrintf("BREAKING...\n");
			break;
		} 
		//2 pushes have been read whilst ONFuncWasActive
		if ( CODE_ON_COUNT == 2 ) {
			DebugPrintf("CODE_ON_COUNT == 2\n");
			buffer+=handleOn(buffer);
			CODE_ON_COUNT = 0;
			CODE_ON_MODE = false;
		} else
		// printf("HMM : %02X %i %i\n",*buffer, buffer, end);
		if ( *buffer == CODE_PUSH ) {
			
			if ( CODE_ON_MODE ) {
				CODE_ON_COUNT++;
			}
			buffer++;
			
			PushTypes type = (PushTypes)(*buffer);
			buffer++; //increment buffer after read
			
			int uffff = PushStack(type,buffer);
			buffer+=uffff;
			DebugPrintf("CODE_PUSH SIZEREAD == %i , with type %i\n",uffff+2,type);
			// printf("stacksize : %i\n",stackSize);
			
		} else {

			char opCode = *buffer;
			DebugPrintf("PROCESSING OPCODE == %i\n",opCode);

			buffer++;
			if ( fieldMode ) {
				if (opCode != CODE_FIELD) {	
					fieldMode = false;
					fprintf(outfile,"%s",newline);
					//empty line
					saveLineNumber(buffer);
				}
			}
			if ( declareMode ) {
				if (opCode > CODE_FIELD) {
					declareMode = false;
					fprintf(outfile,"%s",newline);
					//empty line
					saveLineNumber(buffer);
				}
			}
			switch((int)opCode) {

			case CODE_NEW_GLOBAL:

				buffer+=handleStandardOpcode("global",buffer);
				DebugPrintf("CODE_NEW_GLOBAL %02X @ %i\n",opCode,(int)(buffer)-(int)fileContents);
				break;
			case CODE_NEW_GLOBAL_PLUS_ASSIGNMENT:

				buffer+=handleStandardOpcode("global",buffer,true);
				DebugPrintf("CODE_NEW_GLOBAL_PLUS_ASSIGNMENT %02X @ %i\n",opCode,(int)(buffer)-(int)fileContents);
				break;
			case CODE_NEW_LOCAL:
				DebugPrintf("CODE_NEW_LOCAL %02X @ %i\n",opCode,(int)(buffer-1));
				buffer+=handleStandardOpcode("local",buffer);
				break;
			case CODE_NEW_LOCAL_PLUS_ASSIGNMENT:

				buffer+=handleStandardOpcode("local",buffer,true);
				DebugPrintf("CODE_NEW_LOCAL_PLUS_ASSIGNMENT %02X @ %i\n",opCode,(int)(buffer)-(int)fileContents);
				break;
			case CODE_NEW_PARAMETER:

				buffer+=handleStandardOpcode("parameter",buffer);
				DebugPrintf("CODE_NEW_PARAMETER %02X @ %i\n",opCode,(int)(buffer)-(int)fileContents);
				break;
			case CODE_NEW_PARAMETER_PLUS_DEFAULT:

				buffer+=handleStandardOpcode("parameter",buffer,true);
				DebugPrintf("CODE_NEW_PARAMETER_PLUS_DEFAULT %02X @ %i\n",opCode,(int)(buffer)-(int)fileContents);
				break;
			case CODE_FIELD:
				buffer+=handleStandardOpcode("field",buffer);
				DebugPrintf("CODE_FIELD %02X @ %i\n",opCode,(int)(buffer)-(int)fileContents);
				break;
			case CODE_ASSIGNMENT:


				buffer+=handleAssignment(buffer);
				DebugPrintf("CODE_ASSIGNMENT %02X @ %i\n",opCode,(int)(buffer)-(int)fileContents);
				break;

			case CODE_ADD_ASSIGNMENT: 
				buffer+=handleAddAssignment(buffer,"+");
				DebugPrintf("CODE_ADD_ASSIGNMENT %02X @ %i\n",opCode,(int)(buffer)-(int)fileContents);
				break;

			case CODE_SUBTRACT_ASSIGNMENT:
				buffer+=handleAddAssignment(buffer,"-");
				DebugPrintf("CODE_SUBTRACT_ASSIGNMENT %02X @ %i\n",opCode,(int)(buffer)-(int)fileContents);
				break;

			case CODE_DIVIDE_ASSIGNMENT:
				buffer+=handleAddAssignment(buffer,"/");
				DebugPrintf("CODE_DIVIDE_ASSIGNMENT %02X @ %i\n",opCode,(int)(buffer)-(int)fileContents);
				break;

			case CODE_MULTIPLY_ASSIGNMENT:
				buffer+=handleAddAssignment(buffer,"*");
				DebugPrintf("CODE_MULTIPLY_ASSIGNMENT %02X @ %i\n",opCode,(int)(buffer)-(int)fileContents);
				break;

			case CODE_DIVIDE:
				buffer+= handleMath(buffer,"/");
				DebugPrintf("CODE_DIVIDE %02X @ %i\n",opCode,(int)(buffer)-(int)fileContents);
				break;

			case CODE_MULTIPLY:
				buffer+= handleMath(buffer,"*");
				DebugPrintf("CODE_MULTIPLY %02X @ %i\n",opCode,(int)(buffer)-(int)fileContents);
				break;

			case CODE_ADD:
				buffer+= handleMath(buffer,"+");
				DebugPrintf("CODE_ADD %02X @ %i\n",opCode,(int)(buffer)-(int)fileContents);
				break;

			case CODE_SUBTRACT:
				buffer+= handleMath(buffer,"-");
				DebugPrintf("CODE_SUBTRACT %02X @ %i\n",opCode,(int)(buffer)-(int)fileContents);
				break;

			case CODE_IF:
				buffer+=handleIf(buffer);
				DebugPrintf("CODE_IF %02X @ %i\n",opCode,(int)(buffer)-(int)fileContents);
				break;

			case CODE_GOTO:
					// convert os offset into ds line number
				buffer+=handleGoto(buffer);
				DebugPrintf("CODE_GOTO %02X @ %i\n",opCode,(int)(buffer)-(int)fileContents);

				break;

			case CODE_ENABLE:
				buffer+= handleEnable(buffer,true);
				DebugPrintf("CODE_ENABLE %02X @ %i\n",opCode,(int)(buffer)-(int)fileContents);
				break;

			case CODE_DISABLE:
				buffer+= handleEnable(buffer,false);
				DebugPrintf("CODE_DISABLE %02X @ %i\n",opCode,(int)(buffer)-(int)fileContents);
				break;

			case CODE_ON:
				saveLineNumber(buffer);
				CODE_ON_MODE = true;
				DebugPrintf("CODE_ON %02X @ %i\n",opCode,(int)(buffer)-(int)fileContents);
					// this function is unusual it wants its arguments pushed after the call, unlike the others.

				break;

			case CODE_USE:
					//use entity
				saveLineNumber(buffer,6);
					// Pop Entity index
				message = PopStack();

				fprintf(outfile,"use entity %s%s",getVariable(message->value.number),newline);
				DebugPrintf("CODE_USE %02X @ %i\n",opCode,(int)(buffer)-(int)fileContents);
				break;

			case CODE_MOVEROTATE:
				buffer+=handleMoveRotate(buffer);
				DebugPrintf("CODE_MOVEROTATE %02X @ %i\n",opCode,(int)(buffer)-(int)fileContents);
				break;

			case CODE_MOVE:
				buffer+=handleMove(buffer);
				DebugPrintf("CODE_MOVE %02X @ %i\n",opCode,(int)(buffer)-(int)fileContents);
				break;
			case CODE_ROTATE:
				buffer+=handleRotate(buffer);
				DebugPrintf("CODE_ROTATE %02X @ %i\n",opCode,(int)(buffer)-(int)fileContents);
				break;
			case CODE_CONSOLE_COMMAND:
				buffer+=handleConsoleCommand(buffer);
				DebugPrintf("CODE_CONSOLE_COMMAND %02X @ %i\n",opCode,(int)(buffer)-(int)fileContents);
				break;

			case CODE_PLAY_SOUND:
				buffer+=handlePlaySound(buffer);
				DebugPrintf("CODE_PLAY_SOUND %02X @ %i\n",opCode,(int)(buffer)-(int)fileContents);
				break;

			case CODE_UNLOAD_SOUND:
				buffer+=handleUnloadSound(buffer);
				DebugPrintf("CODE_UNLOAD_SOUND %02X @ %i\n",opCode,(int)(buffer)-(int)fileContents);
				break;

			case CODE_SETCVAR:
				buffer+=handleSetCvar(buffer);
				DebugPrintf("CODE_SETCVAR %02X @ %i\n",opCode,(int)(buffer)-(int)fileContents);
				break;

			case CODE_SET_VIEW_ANGLES:
				buffer+=handleSetViewAngles(buffer);
				DebugPrintf("CODE_SET_VIEW_ANGLES %02X @ %i\n",opCode,(int)(buffer)-(int)fileContents);
				break;



			case CODE_PRINT:
				buffer+= handlePrintMessage(buffer);
				DebugPrintf("CODE_PRINT %02X @ %i\n",opCode,(int)(buffer)-(int)fileContents);
				break;

			case CODE_SUSPEND:
				buffer+= handleSuspend(buffer);
				DebugPrintf("CODE_SUSPEND %02X @ %i\n",opCode,(int)(buffer)-(int)fileContents);
				break;

			case CODE_WAIT_SECONDS:
				buffer+= handleWaitSeconds(buffer);
				DebugPrintf("CODE_WAIT_SECONDS %02X @ %i\n",opCode,(int)(buffer)-(int)fileContents);
				break;

			case CODE_WAIT_ANY:
			case CODE_WAIT_ALL:
				buffer+= handleWaitAllAny(buffer);
				DebugPrintf("CODE_WAIT_ALL || ANY @ %i\n",opCode,(int)(buffer)-(int)fileContents);
				break;

			case CODE_ANIMATE:
				buffer+= handleAnimate(buffer);
				DebugPrintf("CODE_ANIMATE %02X @ %i\n",opCode,(int)(buffer)-(int)fileContents);
				break;
			case CODE_REMOVE:
					//remove entity
				saveLineNumber(buffer,6);
					// edict to remove
				message = PopStack();
				fprintf(outfile,"remove entity %s%s",getVariable(message->value.number),newline);
				DebugPrintf("CODE_REMOVE %02X @ %i\n",opCode,(int)(buffer)-(int)fileContents);
				break;

			case CODE_RESET_AI:
				buffer+= handleResetAi(buffer);
				DebugPrintf("CODE_RESET_AI %02X @ %i\n",opCode,(int)(buffer)-(int)fileContents);
				break;

			case CODE_PLAYSONG:
				buffer+= handlePlaySong(buffer);
				DebugPrintf("CODE_PLAYSONG %02X @ %i\n",opCode,(int)(buffer)-(int)fileContents);
				break;

			case CODE_CACHE_ROFF:
				buffer+=handleCacheRoff(buffer);
				DebugPrintf("CODE_CACHE_ROFF %02X @ %i\n",opCode,(int)(buffer)-(int)fileContents);
				break;

			case CODE_CACHE_SOUND:
				buffer+=handleCacheSound(buffer);
				DebugPrintf("CODE_CACHE_SOUND %02X @ %i\n",opCode,(int)(buffer)-(int)fileContents);
				break;

			case CODE_CACHE_STRING_PACKAGE:
				buffer+=handleCacheStringPackage(buffer);
				DebugPrintf("CODE_CACHE_SOUND %02X @ %i\n",opCode,(int)(buffer)-(int)fileContents);
				break;

			case CODE_UNLOAD_ROFF:
				buffer+=handleUnloadRoff(buffer);
				DebugPrintf("CODE_UNLOAD_ROFF %02X @ %i\n",opCode,(int)(buffer)-(int)fileContents);
				break;

			case CODE_TANK:
				buffer+=handleTank(buffer);
				DebugPrintf("CODE_TANK %02X @ %i\n",opCode,(int)(buffer)-(int)fileContents);
				break;

			case CODE_HELICOPTER:
				buffer+=handleHelicopter(buffer);
				DebugPrintf("CODE_HELICOPTER %02X @ %i\n",opCode,(int)(buffer)-(int)fileContents);
				break;

			case CODE_SNOWCAT:

				break;
			case CODE_EXIT:
					//printf("previos opcode is %02X\n",previousOpcode);
					//exit
				if ( previousOpcode != CODE_EXIT ) { 
					saveLineNumber(buffer);
					fprintf(outfile,"exit%s",newline);
						//printf("%i Line Numbers :)\n",lineNumbers);
				}

				DebugPrintf("CODE_EXIT %02X @ %i\n",opCode,(int)(buffer)-(int)fileContents);
				break;

			default:
				char lul[64];
				sprintf(lul,"Unrecognised opcode %02X\n",opCode);
				errorExit(lul);
				break;

				
			} // switch
			previousOpcode = opCode;
		} //if else PUSH
	} //while
	PatchUpJumps();
	//printf("condition no longer true\n");
}


int handleSetViewAngles(char * buffer) {
	char * startbuffer = buffer;
	struct stackItem * message;
	int num_pushed = 0;

	char * entVariable;
	float angles[3];

	char vect_string[64];

	//vect angles
	message = PopStack();
	if ( message->type == PUSH_VAR ) {
		char * c  = getVariable(message->value.number);
		strcpy(vect_string,c);
		num_pushed+= 6;
	} else {
		num_pushed+= 14;
		angles[0] = message->value.three_floats[0];
		angles[1] = message->value.three_floats[1];
		angles[2] = message->value.three_floats[2];
		sprintf(vect_string,"[%.3f ,%.3f ,%.3f]",angles[0],angles[1],angles[2]);
	}
	
	//entity who
	message = PopStack();
	entVariable = getVariable(message->value.number);
	num_pushed+= 6;


	fprintf(outfile,"set view angles of entity %s to %s%s",entVariable,vect_string,newline);

	saveLineNumber(startbuffer,num_pushed);
	return buffer - startbuffer;		
}

int handleSetCvar(char * buffer) {
	char * startbuffer = buffer;
	struct stackItem * message, * val;
	int num_pushed = 0;


	char * cvarname;
	char * value;
	

	//val
	val = PopStack();
	

	//cvarname
	message = PopStack();
	num_pushed+= strlen(message->value.nullString)+1+2;
	cvarname = message->value.nullString;



	if ( val->type == PUSH_CONST_STRING ) {
		value = val->value.nullString;
		num_pushed+= strlen(value)+1+2;
		fprintf(outfile,"setcvar cvar \"%s\" to \"%s\"%s",cvarname,value,newline);
	} else {
		num_pushed+=6;
		fprintf(outfile,"setcvar cvar \"%s\" to %f%s",cvarname,val->value.floatNumber,newline);
	}

	saveLineNumber(startbuffer,num_pushed);
	return buffer - startbuffer;		
}


int handleUnloadSound(char * buffer) {
	char * startbuffer = buffer;
	struct stackItem * message;
	int num_pushed = 0;

	message = PopStack();
	num_pushed+= strlen(message->value.nullString+1+2);


	fprintf(outfile,"unload sound \"%s\"%s",message->value.nullString,newline);

	saveLineNumber(startbuffer,num_pushed);
	return buffer - startbuffer;	
}

int handleHelicopter(char * buffer) {
	char * startbuffer = buffer;
	struct stackItem * message;
	int num_pushed = 0;


	int flags = *(unsigned char*)buffer;
	DebugPrintf("Helicopter Flags : %i\n",flags);
	buffer++;


	float tankFloat;
	float tankVect[3];
	char * tankTarg;
	char * signaler;
	int command;
	char * whichTank;


	//signaling
	if (flags & HELICOPTER_SIGNALER)
	{
		DebugPrintf("HeliFlag: Signaller!\n");
		message = PopStack();
		num_pushed+=6;
		signaler = getVariable(message->value.number);
	}

	if (flags & HELICOPTER_FLOAT)
	{
		DebugPrintf("HeliFlag: Float!\n");
		message = PopStack();

		num_pushed+=6;
		tankFloat = message->value.floatNumber;
	}

	/*
		Seems the flags sometimes bug, where it contains
		HELICOPTER_VECT flag, but its not a float?
		Unless its a vect of shorts... and type still has to be checked?
	*/
	if (flags & HELICOPTER_VECT)
	{
		DebugPrintf("HeliFlag: Vect!\n");
		message = PopStack();
		if ( message->type == PUSH_CONST_VECTOR ) {
			num_pushed+=14;
			tankVect[0] = message->value.three_floats[0];
			tankVect[1] = message->value.three_floats[1];
			tankVect[2] = message->value.three_floats[2];
		}
		else
			num_pushed+=6;
	}

	//targeting
	if (flags & HELICOPTER_TARG)
	{
		DebugPrintf("HeliFlag: Targ!\n");
		message = PopStack();
		num_pushed+=6;
		tankTarg = getVariable(message->value.number);
	}

	message = PopStack();
	num_pushed+=6;
	command = message->value.number;
	message = PopStack();
	num_pushed+=6;
	whichTank = getVariable(message->value.number);


	fprintf(outfile,"helicopter entity %s %i%s",whichTank,command,newline);

	saveLineNumber(startbuffer,num_pushed);
	return buffer - startbuffer;
}

int handleTank(char * buffer) {
	char * startbuffer = buffer;
	struct stackItem * message;
	int num_pushed = 0;


	int flags = *(unsigned char*)buffer;
	buffer++;
	DebugPrintf("Tank Flags : %i\n",flags);

	float tankFloat;
	float tankVect[3];
	char * tankTarg;
	char * signaler;
	int command;
	char * whichTank;


	//signaling
	if (flags & TANK_SIGNALER)
	{
		DebugPrintf("Tank: Signaller!\n");
		message = PopStack();
		num_pushed+=6;
		signaler = getVariable(message->value.number);
	}

	if (flags & TANK_FLOAT)
	{
		DebugPrintf("Tank: Float!\n");
		message = PopStack();
		num_pushed+=6;
		tankFloat = message->value.floatNumber;
	}

	if (flags & TANK_VECT)
	{
		message = PopStack();
		DebugPrintf("Tank: Vect %i!\n",message->type);
		if ( message->type == PUSH_CONST_VECTOR ) {
			num_pushed+=14;
			tankVect[0] = message->value.three_floats[0];
			tankVect[1] = message->value.three_floats[1];
			tankVect[2] = message->value.three_floats[2];
		}
		else
			num_pushed+=6;
	}

	//targeting
	if (flags & TANK_TARG)
	{
		DebugPrintf("Tank: Targ\n");
		message = PopStack();
		num_pushed+=6;
		tankTarg = getVariable(message->value.number);
	}

	message = PopStack();
	num_pushed+=6;
	command = message->value.number;
	message = PopStack();
	num_pushed+=6;
	whichTank = getVariable(message->value.number);


	fprintf(outfile,"tank entity %s %i%s",whichTank,command,newline);

	DebugPrintf("Tank num_pushed = %i\n",num_pushed);
	saveLineNumber(startbuffer,num_pushed);
	return buffer - startbuffer;
}

/*

*/
int handleMath(char* buffer,const char * symbol) {
	char * startbuffer = buffer;
	struct stackItem * left,*right,*bla;
	int num_pushed = 0;

	left = PopStack();
	right = PopStack();

	char leftside[128],rightside[128];
	char * idc;
	bla = left;
	idc = leftside;
	for ( int i = 0; i<2;i++) {
		DebugPrintf("Math Type is %i\n",bla->type);
		switch (bla->type) {
		case PUSH_VAR:
			sprintf(idc,"%s",getVariable(bla->value.number));
			num_pushed+=6;
			break;
		case PUSH_VAR_WITH_FIELD:
			sprintf(idc,"%s.%s",getVariable(bla->value.two_numbers[0]),getField(bla->value.two_numbers[1]));
			num_pushed+=10;
			break;
		case PUSH_CONST_FLOAT:
			sprintf(idc,"%.3f",bla->value.floatNumber);
			num_pushed+=6;
			break;
		case PUSH_CONST_INT:
			sprintf(idc,"%i",bla->value.number);
			num_pushed+=6;
			break;
		case PUSH_CONST_VECTOR:
			sprintf(idc,"[%.3f ,%.3f ,%.3f]",bla->value.three_floats[0],bla->value.three_floats[1],bla->value.three_floats[2]);
			num_pushed+=14;
			break;
		}
		bla=right;
		idc=rightside;
	}
	//dont forget size of self.
	num_pushed+=1;

	char final[256];
	sprintf(final,"%s %s %s",leftside,symbol,rightside);
	
	PushStack(PUSH_CUSTOM,final,num_pushed);

	// saveLineNumber(startbuffer,num_pushed);
	return buffer - startbuffer;
}
int handleEnable(char * buffer, bool enable){
	char * startbuffer = buffer;
	struct stackItem * message;
	int num_pushed = 0;


	int type = *(unsigned char*)buffer;
	buffer++;


	char * entName;

	char enOrDis[8];

	if ( enable ) strcpy(enOrDis,"enable"); else strcpy(enOrDis,"disable");
	char outText[128];
	switch ( type )  {
	case FEATURE_TRIGGER:
			//entity?
		message = PopStack();
		num_pushed +=6;
		entName = getVariable(message->value.number);
		if ( !entName ) errorExit("Couldnt' get variable name error!\n");
		sprintf(outText,"%s trigger entity %s%s",enOrDis,entName,newline);
		break;

	case FEATURE_AMBIENT_SOUNDS:
		sprintf(outText,"%s ambient sounds%s",enOrDis,newline);
		break;

	case FEATURE_CINEMATICS:

			//yes
			//NADA?????
		sprintf(outText,"%s cinematics%s",enOrDis,newline);
		break;

	case FEATURE_PLAGUE_SKINS:
		sprintf(outText,"%s plague skins%s",enOrDis,newline);
		break;
	}


	//printf(outText);
	fprintf(outfile,outText);

	saveLineNumber(startbuffer,num_pushed);
	return buffer - startbuffer;
}

int handleCacheRoff(char * buffer){
	char * startbuffer = buffer;
	struct stackItem * message;
	int num_pushed = 0;

	//string.
	message = PopStack();
	num_pushed+=strlen(message->value.nullString)+1+2;

	// !!!!! NICE AND EASY ONE
	fprintf(outfile,"cache roff \"%s\"%s",message->value.nullString,newline);

	saveLineNumber(startbuffer,num_pushed);
	return buffer - startbuffer;
}

int handleCacheStringPackage(char * buffer) {
	char * startbuffer = buffer;
	struct stackItem * message;
	int num_pushed = 0;

		//string.
	message = PopStack();
	num_pushed+=strlen(message->value.nullString)+1+2;

		// !!!!! NICE AND EASY ONE
	fprintf(outfile,"cache strings \"%s\"%s",message->value.nullString,newline);

	saveLineNumber(startbuffer,num_pushed);
	return buffer - startbuffer;
}

int handleCacheSound(char * buffer){
	char * startbuffer = buffer;
	struct stackItem * message;
	int num_pushed = 0;

	//string.
	message = PopStack();
	num_pushed+=strlen(message->value.nullString)+1+2;

	// !!!!! NICE AND EASY ONE
	fprintf(outfile,"cache sound \"%s\"%s",message->value.nullString,newline);

	saveLineNumber(startbuffer,num_pushed);
	return buffer - startbuffer;
}

int handleResetAi(char * buffer) {

	char * startbuffer = buffer;
	struct stackItem * message;
	int num_pushed = 0;


	message=PopStack();
	num_pushed+=6;
	

	fprintf(outfile,"reset ai for entity %s%s",getVariable(message->value.number),newline);

	saveLineNumber(startbuffer,num_pushed);
	return buffer - startbuffer;
}

int handleUnloadRoff(char * buffer) {
	char * startbuffer = buffer;
	struct stackItem * message;
	int num_pushed = 0;


	message = PopStack();
	num_pushed+= strlen(message->value.nullString)+1+2;

	// !!!!! NICE AND EASY ONE
	fprintf(outfile,"unload roff \"%s\"%s",message->value.nullString,newline);

	saveLineNumber(startbuffer);
	return buffer - startbuffer;
}

int handleSuspend(char * buffer) {
	char * startbuffer = buffer;
	struct stackItem * message;
	int num_pushed = 0;


	// !!!!! NICE AND EASY ONE
	fprintf(outfile,"suspend%s",newline);

	saveLineNumber(startbuffer);
	return buffer - startbuffer;
}


// 2 unknown things pushed
// then a call to if statement.
// but its reversed

/*

	dontr subtract amnything frmo thsi guy because its argumetns are pushed afterwards?

*/
int handleOn(char * buffer) {
	char * startbuffer = buffer;
	struct stackItem * message;
	int num_pushed = 0;

	//the unknown guys
	//not sure if i need to pop these guys.
	// message = PopStack();
	// message = PopStack();

	buffer+= handleIf(buffer,true);


	// fprintf(outfile,"on \"%s\"%s%s",soundName,appendString,newline);
	// saveLineNumber(startbuffer,num_pushed);
	return buffer - startbuffer;
}

int handlePlaySong(char * buffer) {
	char * startbuffer = buffer;
	struct stackItem * message;
	int num_pushed = 0;

	message = PopStack();
	num_pushed+=6;

	//-1 means restart
	fprintf(outfile,"playsong %i%s",message->value.number,newline);
	saveLineNumber(startbuffer,num_pushed);
	return buffer - startbuffer;
}


int handlePlaySound(char * buffer) {
	char * startbuffer = buffer;
	struct stackItem * message;
	int num_pushed = 0;

	int flags = *(unsigned char*)buffer;
	buffer++;


	char * soundName;

	char * for_ent;
	const char * sChannel;

	struct stackItem * channel;
	struct stackItem * volume;
	struct stackItem * timedelay;
	struct stackItem * attenuation;
	const char * sAtten;

	//sound name -- string ?
	message = PopStack();
	num_pushed+= strlen(message->value.nullString)+1+2;
	soundName = message->value.nullString;

	//should the sound have a delay?
	if (flags & PLAY_SOUND_TIMEDELAY)
	{
		//float
		timedelay = PopStack();
		num_pushed+=6; 
	}

	//channel to play the sound on
	if (flags & PLAY_SOUND_CHANNEL)
	{
		//int
		channel = PopStack();
		num_pushed+= 6;
	}
	//how far sound travels
	if (flags & PLAY_SOUND_ATTENUATION)
	{
		//float
		num_pushed+= 6;
		attenuation = PopStack();
	}	
	//volume..
	if (flags & PLAY_SOUND_VOLUME)
	{
		//float
		volume = PopStack();
		num_pushed+= 6;
	}

	//sound attached to ent ? or at ent position?
	if (flags & PLAY_SOUND_ENTITY)
	{
		//ent var hm
		message = PopStack();
		num_pushed+= 6;
		for_ent = getVariable(message->value.number);
	}


	char appendString[256] = "";
	char temp[64] = "";

	if (flags & PLAY_SOUND_ENTITY) {
		sprintf(temp," for entity %s",for_ent);
		strcat(appendString,temp);
	}

	if (flags & PLAY_SOUND_CHANNEL) {

		if ( channel->type == PUSH_VAR ) {
			sprintf(temp," on channel %s",getVariable(channel->value.number));
		} else {
			int chnl = channel->value.number;
			sChannel = findChannel(chnl);
			if ( sChannel ) {
				sprintf(temp," on channel %s",sChannel);
			} else {
				//don't know the lookup name of that channel sorry, use number instead
				sprintf(temp," on channel %i",chnl);
			}	
		}
		
		
		strcat(appendString,temp);
	}

	if (flags & PLAY_SOUND_VOLUME) {
		if ( volume->type == PUSH_VAR ) {
			sprintf(temp," at volume %s",getVariable(volume->value.number));
		} else {
			// float
			sprintf(temp," at volume %.3f",volume->value.floatNumber);
		}
		
		strcat(appendString,temp);
	}

	if (flags & PLAY_SOUND_ATTENUATION) {

		if ( attenuation->type == PUSH_VAR ) {
			sprintf(temp," at attenuation %s",getVariable(attenuation->value.number));
		} else {
			float atn = attenuation->value.floatNumber;
			sAtten = findAtten(atn);
			if ( sAtten ) {
				//recognise this value!
				sprintf(temp," at attenuation %s",sAtten);
			} else {
				sprintf(temp," at attenuation %.3f",atn);
			}	
		}	
		strcat(appendString,temp);
	}

	if (flags & PLAY_SOUND_TIMEDELAY) {
		if ( timedelay->type == PUSH_VAR ) { 
			sprintf(temp," after %s seconds",getVariable(timedelay->value.number));
		} else {
			sprintf(temp," after %.3f seconds",timedelay->value.floatNumber);
		}
		
		strcat(appendString,temp);
	}


	fprintf(outfile,"play sound \"%s\"%s%s",soundName,appendString,newline);
	saveLineNumber(startbuffer,num_pushed);
	return buffer - startbuffer;

}

int handleMoveRotate(char * buffer) {
	char * startbuffer = buffer;
	struct stackItem * message;
	int num_pushed = 0;

	int flags = *(unsigned char*)buffer;
	buffer++;

	char *who_ent;
	char * signaling;
	char * filename;

	if (flags & MOVEROTATE_SIGNALER)
	{
		//anything
		message = PopStack();
		num_pushed+=6;
		signaling = getVariable(message->value.number);
	}

	//oh its a var?? but why... hmm string or var mb, ok.
	//filename string
	message = PopStack();
	if ( message->type == PUSH_VAR ) {
		filename = getVariable(message->value.number);
		num_pushed+=6;
	} else {
		// assume its a constr string
		filename = message->value.nullString;
		num_pushed+=strlen(filename)+1+2;
	}
	
	//entity
	message = PopStack();
	who_ent = getVariable(message->value.number);
	num_pushed+=6;


	char appendString[256] = "";
	char temp[64] = "";

	if ( flags & MOVEROTATE_SIGNALER ) {
		sprintf(temp," signaling %s",signaling);
		strcat(appendString,temp);
	}

	//moverotate entity fusedoor from file "generic_door_ccw.rof" signaling sig2

	// printf("ent is %08X\n",who_ent);
	// printf("filename is %s\n",filename);


	fprintf(outfile,"moverotate entity %s from file \"%s\"%s%s",who_ent,filename,appendString,newline);
	saveLineNumber(startbuffer,num_pushed);
	return buffer - startbuffer;
}



int handleMove(char * buffer) {
	char * startbuffer = buffer;
	struct stackItem * message;
	int num_pushed = 0;

	int flags = *(unsigned char*)buffer;
	buffer++;

	char * signaling;
	struct stackItem * speed;
	struct stackItem * duration;

	struct stackItem * byHowMuch;


	if (flags & MOVE_SIGNALER)
	{
		//varname
		message = PopStack();
		num_pushed+=6;
		signaling = getVariable(message->value.number);
	}

	if (flags & MOVE_RATE)
	{
		//float
		message = PopStack();
		speed = message;
		num_pushed+=6;
	}

	if (flags & MOVE_DURATION)
	{
		//float
		message = PopStack();
		duration = message;
		num_pushed+=6;
	}

	char cByHowMuch[64];
	//amount vector
	byHowMuch = PopStack();
	
	if ( byHowMuch->type == PUSH_VAR ) {
		num_pushed+=6;
		strcpy(cByHowMuch,getVariable(byHowMuch->value.number));
	} else {
		float byy[3];
		num_pushed+=14;
		
		byy[0] = byHowMuch->value.three_floats[0];
		byy[1] = byHowMuch->value.three_floats[1];
		byy[2] = byHowMuch->value.three_floats[2];
		sprintf(cByHowMuch,"[%.3f ,%.3f ,%.3f]",byy[0],byy[1],byy[2]);
	}
	//entity
	message = PopStack();
	num_pushed+=6;
	char * ent = getVariable(message->value.number);


	char appendString[256] = "";
	char temp[64] = "";
	if (flags & MOVE_DURATION) {

		if ( duration->type == PUSH_CONST_FLOAT ) {
			sprintf(temp," over %.3f seconds",duration->value.floatNumber);
		} else if ( duration->type == PUSH_VAR ) {
			sprintf(temp," over %s seconds",getVariable(duration->value.number));
		} else if ( duration->type == PUSH_CONST_INT ) {
			sprintf(temp," over %i seconds",duration->value.number);
		}
		strcat(appendString,temp);
	}

	if (flags & MOVE_RATE) {
		// printf("HAHAHHAHAH %i\n",speed->type);
		if ( speed->type == PUSH_CONST_INT ) {
			sprintf(temp," at %i speed",speed->value.number);
		} else
		if ( speed->type == PUSH_CONST_FLOAT ) {
			sprintf(temp," at %.3f speed",speed->value.floatNumber);
		} else if ( speed->type == PUSH_VAR ) {
			sprintf(temp," at %s speed",getVariable(speed->value.number));
		}
		strcat(appendString,temp);
	}

	if ( flags & MOVE_SIGNALER ) {
		sprintf(temp," signaling %s",signaling);
		strcat(appendString,temp);
	}

	char absolute[3];
	absolute[0] = 'b';absolute[1] = 'y';absolute[2] = 0x00;
	if ( flags & MOVE_ABSOLUTE ) { 
		absolute[0] = 't';
		absolute[1] = 'o';
	}


	fprintf(outfile,"move entity %s %s %s%s%s",ent,absolute,cByHowMuch,appendString,newline);

	saveLineNumber(startbuffer,num_pushed);
	return buffer - startbuffer;
}

/*
var,vector
*/
int handleRotate(char * buffer) {
	char * startbuffer = buffer;
	struct stackItem * message;
	int num_pushed = 0;

	int flags = *(unsigned char*)buffer;
	buffer++;


	char * signaling;
	struct stackItem * speed;
	struct stackItem * duration;

	struct stackItem * byHowMuch;

	//printf("FLAGS OF ROTATE IS : %i\n",flags);

	
	if (flags & ROTATE_SIGNALER)
	{
		//anything
		message = PopStack();
		num_pushed+=6;
		signaling = getVariable(message->value.number);
	}

	
	//speed
	if (flags & ROTATE_RATE)
	{
		message = PopStack();
		speed = message;
		num_pushed+=6;
	}

	if (flags & ROTATE_DURATION)
	{
		//float
		message = PopStack();
		duration = message;
		num_pushed+=6;
	}


	char appendString[256] = "";
	char temp[64] = "";
	if (flags & ROTATE_DURATION) {

		if ( duration->type == PUSH_CONST_FLOAT ) {
			sprintf(temp," over %.3f seconds",duration->value.floatNumber);
		} else if ( duration->type == PUSH_VAR ) {
			sprintf(temp," over %s seconds",getVariable(duration->value.number));
		} else if ( duration->type == PUSH_CONST_INT ) {
			sprintf(temp," over %i seconds",duration->value.number);
		}
		strcat(appendString,temp);
	}

	if (flags & ROTATE_RATE) {
		if ( speed->type == PUSH_CONST_INT ) {
			sprintf(temp," at %i speed",speed->value.number);
		} else
		if ( speed->type == PUSH_CONST_FLOAT ) {
			sprintf(temp," at %.3f speed",speed->value.floatNumber);
		} else if ( speed->type == PUSH_VAR ) {
			sprintf(temp," at %s speed",getVariable(speed->value.number));
		}
		strcat(appendString,temp);
	}

	if ( flags & ROTATE_SIGNALER ) {
		sprintf(temp," signaling %s",signaling);
		strcat(appendString,temp);
	}

	//vector ROTATE BY HOW MUCH
	byHowMuch = PopStack();
	char cByHowMuch[64];
	if ( byHowMuch->type == PUSH_VAR ) { 
		num_pushed+=6;
		strcpy(cByHowMuch,getVariable(byHowMuch->value.number));
	} else {
		num_pushed+=14;
		float byy[3];
		byy[0] = byHowMuch->value.three_floats[0];
		byy[1] = byHowMuch->value.three_floats[1];
		byy[2] = byHowMuch->value.three_floats[2];
		sprintf(cByHowMuch,"[%.3f ,%.3f ,%.3f]",byy[0],byy[1],byy[2]);	
	}
	
	//entity - WHICH ENTITY
	message = PopStack();
	num_pushed+=6;
	char * ent = getVariable(message->value.number);

	// printf("%s\n",ent);
	// printf("%s\n",getLabel(signalling));
	// printf("speed: %f vectorshit:%f %f %f\n",speed,byy[0],byy[1],byy[2]);

	char absolute[3];
	absolute[0] = 'b';absolute[1] = 'y';absolute[2] = 0x00;
	if ( flags & ROTATE_ABSOLUTE ) { 
		absolute[0] = 't';
		absolute[1] = 'o';
	}

	fprintf(outfile,"rotate entity %s %s %s%s%s",ent,absolute,cByHowMuch,appendString,newline);
	// printf("rotate entity %s by [%f, %f,%f] at %f speed signaling %s%s",ent,byy[0],byy[1],byy[2],speed,getLabel(signalling),newline);

	//rotate entity
	//why is this 24???
	//but 20 doesnt divide by 6, so thats impossible.
	//20
	//printf("FUCKK YIOUUUUUU %i\n",num_pushed);
	saveLineNumber(startbuffer,num_pushed);
	return buffer - startbuffer;
}


//func is handled like a string by other functions i believe. But it has a
//secret value so the buffer can subtract all the pushed data parsed to the function.
//the receiving function needs to know that it must parse this data.
//thus PUSH_FUNCTION.
int handleFunction(char * buffer,struct stackItem * item) {
	char * startbuffer = buffer;
	struct stackItem * message;


	int index = *(unsigned char*)buffer;
	buffer++;


	char final[512];
	char temp[256];
	
	switch (index) {
	case FUNC_FIND_ENTITY_WITH_TARGET:
			// printf("FIND ENTITY WITH TARGET PUSH\n");
			// function / string?
		message = PopStack();

		if ( message->type == PUSH_VAR ) {
			sprintf(temp,"find entity with targetname %s",getVariable(message->value.number));
			item->value.nullString = (char*)malloc(strlen(temp)+1);
			strcpy(item->value.nullString,temp);
			item->secret_size = 6;	
		} else {
				//this is how much was read
			item->secret_size = strlen(message->value.nullString)+1+2;

			sprintf(temp,"find entity with targetname \"%s\"",message->value.nullString);
				//temp is pushed our stack
			item->value.nullString = (char*)malloc(strlen(temp)+1);
			strcpy(item->value.nullString,temp);		
		}

		break;

	case FUNC_SIN:
			// float
		message = PopStack();
			// *lineNumberControl +=6;
		sprintf(temp,"sin %f",message->value.floatNumber);
		item->value.nullString = (char*)malloc(strlen(temp)+1);
		strcpy(item->value.nullString,temp);
		item->secret_size = 6;
		break;

	case FUNC_COS:
			//float
		message = PopStack();
			// *lineNumberControl +=6;
		sprintf(temp,"cos %f",message->value.floatNumber);
		item->value.nullString = (char*)malloc(strlen(temp)+1);
		strcpy(item->value.nullString,temp);
		item->secret_size = 6;
		break;

	case FUNC_RANDOM: {
		char cmin[64],cmax[64];
			// 2x int
		message = PopStack();
		if( message->type == PUSH_VAR ) {
			strcpy(cmax,getVariable(message->value.number));
		} else {
			sprintf(cmax,"%i",message->value.number);	
		}

		message = PopStack();
		if( message->type == PUSH_VAR ) {
			strcpy(cmin,getVariable(message->value.number));
		} else {
			sprintf(cmin,"%i",message->value.number);	
		}
		item->secret_size = 12;

		sprintf(temp,"random from %s to %s",cmin,cmax);
		item->value.nullString = (char*)malloc(strlen(temp)+1);
		strcpy(item->value.nullString,temp);
		break;
	}

case FUNC_FIND_ENTITY_WITH_SCRIPT:
			// string
	message = PopStack();

	if ( message->type == PUSH_VAR ) {
		sprintf(temp,"find entity with scripttarget %s",getVariable(message->value.number));
		item->value.nullString = (char*)malloc(strlen(temp)+1);
		strcpy(item->value.nullString,temp);
		item->secret_size = 6;	
	} else {
		item->secret_size = strlen(message->value.nullString)+1+2;
		sprintf(temp,"find entity with scripttarget \"%s\"",message->value.nullString);
		item->value.nullString = (char*)malloc(strlen(temp)+1);
		strcpy(item->value.nullString,temp);	
	}

	break;

case FUNC_FIND_PLAYER:
	sprintf(temp,"find entity player");
	item->value.nullString = (char*)malloc(strlen(temp)+1);
	strcpy(item->value.nullString,temp);
	break;

case FUNC_SPAWN: {
			// PrintStack();
	struct stackItem * val;
	int count = *(unsigned char*)buffer;
	buffer++;
			item->secret_size +=1; //lul??:)

			char small[64];
			strcpy(temp,"");
			bool firstRun = true;
			for ( int i=count;count;count--) {

				//data Value
				val = PopStack(true);
				if ( val == NULL ) errorExit("bad reverse stack pop\n");

				//String Name
				message = PopStack(true);
				if ( message == NULL ) errorExit("bad reverse stack pop\n");

				//+= cos we inside a loop..
				item->secret_size += strlen(message->value.nullString)+1+2;

				if ( !firstRun ) strcat(temp," ,");

				char * spawn_name = message->value.nullString;
				switch ( val->type ) {
					
					// non 4 cases
				case PUSH_CONST_STRING:

					item->secret_size += strlen(val->value.nullString)+1+2;
					sprintf(small," \"%s\" = \"%s\"",spawn_name,val->value.nullString);
					strcat(temp, small);
					break;
				case PUSH_CONST_VECTOR:
					item->secret_size += 14;
					sprintf(small," \"%s\" = [%.3f ,%.3f ,%.3f]",spawn_name,val->value.three_floats[0],val->value.three_floats[1],val->value.three_floats[2]);
					strcat(temp, small);
					break;
					
				case PUSH_CONST_INT:
					item->secret_size += 6;
					sprintf(small," \"%s\" = %i",spawn_name,val->value.number);
					strcat(temp, small);

					break;

				case PUSH_CONST_FLOAT:
					item->secret_size += 6;
					sprintf(small," \"%s\" = %.3f",spawn_name,val->value.floatNumber);
					strcat(temp, small);
					break;
					
				default:
					errorExit("Invalid var type in handleFunction\n");
					break;
				}
				// if the data matches a fieldname
				// extract the data.
				//i care more about the value's type in push than the field.type
				if ( firstRun ) {
					firstRun = false;
				}
			}
			
			sprintf(final,"spawn entity with fields%s",temp);
			item->value.nullString = (char*)malloc(strlen(final)+1);
			strcpy(item->value.nullString,final);
			break;
		}	
	case FUNC_GET_OTHER:
		sprintf(temp,"get entity other");
		item->value.nullString = (char*)malloc(strlen(temp)+1);
		strcpy(item->value.nullString,temp);
		break;

	case FUNC_GET_ACTIVATOR:
		sprintf(temp,"get entity activator");
		item->value.nullString = (char*)malloc(strlen(temp)+1);
		strcpy(item->value.nullString,temp);
		break;

	case FUNC_GET_PLAYER:
			// int
		message = PopStack();
		sprintf(temp,"get entity player %i",message->value.number);
		item->value.nullString = (char*)malloc(strlen(temp)+1);
		strcpy(item->value.nullString,temp);
		item->secret_size = 6;
		break;
	}
	//11 07 type
	item->secret_size+=3;
	return buffer - startbuffer;
}

int handleAnimate(char * buffer) {
	char * startbuffer = buffer;
	int num_pushed = 0;
	
	struct stackItem * message;

	int anim_emotion;
	int null_target;
	char * entity_target = NULL;
	char * entity_source = NULL;
	char * signal_variable;

	struct stackItem * turning = NULL;
	struct stackItem * moving = NULL;
	struct stackItem * repeat = NULL;
	struct stackItem * hold = NULL;

	//flags
	int flags = *(int*)buffer;
	buffer+=4;

	//printf("flags = %i\n",flags);
	char * target = NULL;

	char emotionOrAction[64];


	if (flags & ANIMATE_EMOTION)
	{
		// int emotion
		message = PopStack();
		anim_emotion = message->value.number;
		num_pushed+=6;

	}

	if (flags & ANIMATE_NULLTARGET)
	{
		// int null target
		message = PopStack();
		null_target = message->value.number;
		num_pushed+=6;
	}
	
	if (flags & ANIMATE_TARGET)
	{
		// int edict index
		message = PopStack();
		entity_target = getVariable(message->value.number);
		// target = getLabel(message->value.number);
		num_pushed+=6;
	}

	if (flags & ANIMATE_SOURCE)
	{
		// int edict target
		message = PopStack();
		entity_source = getVariable(message->value.number);
		num_pushed+=6;
	}

	if (flags & ANIMATE_SIGNALER)
	{
		message = PopStack();
		num_pushed+=6;

		signal_variable = getVariable(message->value.number);

		// can there be many of these??
	}

	if (flags & ANIMATE_MOVING)
	{
		// vector
		moving = PopStack();
		//this can be variable too
	}

	if (flags & ANIMATE_TURNING)
	{
		// vector
		turning = PopStack();
		//this can be variable too
	}

	if (flags & ANIMATE_REPEAT)
	{
		// int
		message = PopStack();
		repeat = message;
		num_pushed+=6;
	}

	// if (flags & ANIMATE_MOVING_ABS)
	// {

	// 	Absolute = true;
	// }

	// if (flags & ANIMATE_KILL)
	// {
	// 	Kill = true;
	// }

	if (flags & ANIMATE_HOLD)
	{
		// float
		hold = PopStack();
		num_pushed+=6;
	}

	char appendString[256] = "";
	char temp[64];

	if ( flags & ANIMATE_EMOTION ) {
		if ( anim_emotion > 0 && anim_emotion < 9) {
			sprintf(emotionOrAction," showing emotion %s",emotions[anim_emotion]);
		} else {
			sprintf(emotionOrAction," showing emotion %i",anim_emotion);
		}	
		// strcat(appendString,temp);
	}
	if (flags & ANIMATE_TURNING) {
		if ( turning->type == PUSH_VAR ) {
			num_pushed+=6;
			sprintf(temp," by turning %s",getVariable(turning->value.number));		
		} else {
			float turning_vect[3];
			turning_vect[0] = turning->value.three_floats[0];
			turning_vect[1] = turning->value.three_floats[1];
			turning_vect[2] = turning->value.three_floats[2];
			num_pushed+=14;	
			sprintf(temp," by turning [%.3f ,%.3f ,%.3f]",turning_vect[0],turning_vect[1],turning_vect[2]);
		}
		strcat(appendString,temp);
	}

	if ( flags & ANIMATE_MOVING ) {

		if ( moving->type == PUSH_VAR ) {
			num_pushed+=6;
			if (flags & ANIMATE_MOVING_ABS)
				sprintf(temp," by moving to %s",getVariable(moving->value.number));
			else
				sprintf(temp," by moving %s",getVariable(moving->value.number));		
		} else {
			float moving_vect[3];
			moving_vect[0] = moving->value.three_floats[0];
			moving_vect[1] = moving->value.three_floats[1];
			moving_vect[2] = moving->value.three_floats[2];
			num_pushed+=14;
			if (flags & ANIMATE_MOVING_ABS)
				sprintf(temp," by moving to [%.3f ,%.3f ,%.3f]",moving_vect[0],moving_vect[1],moving_vect[2]);
			else
				sprintf(temp," by moving [%.3f ,%.3f ,%.3f]",moving_vect[0],moving_vect[1],moving_vect[2]);
		}
		strcat(appendString,temp);
	}

	if (flags & ANIMATE_REPEAT) {
		if ( repeat->type == PUSH_VAR ) {
			char * cc = getVariable(repeat->value.number);
			if (  cc != NULL ) {
				sprintf(temp," repeating for %s times",cc);
			} else {
				errorExit("unknown error in animate function\n");
			}
		}else if( repeat->type == PUSH_CONST_INT ) {
			int r = repeat->value.number;
			if ( r == 0 ) {
				sprintf(temp," repeating");
			} else {
				//repeat indefinately?
				sprintf(temp," repeating for %i times",r);	
			}
		}
		strcat(appendString,temp);
	}

	if (flags & ANIMATE_SOURCE) {
		sprintf(temp," from source entity %s",entity_source);
		strcat(appendString,temp);
	}

	if (flags & ANIMATE_TARGET) {
		sprintf(temp," targeting entity %s",entity_target);
		strcat(appendString,temp);
	}

	if (flags & ANIMATE_KILL)
	{
		sprintf(temp," kill");
		strcat(appendString,temp);
	}

	// animate something to die?? at a location, where??
	if (flags & ANIMATE_NULLTARGET)
	{
		sprintf(temp," nulltarget %s",nullTargets[null_target]);
		strcat(appendString,temp);
	}

	if (flags & ANIMATE_SIGNALER)
	{
		// not sure about this one
		sprintf(temp," signaling %s",signal_variable);
		strcat(appendString,temp);
	}

	if ( flags & ANIMATE_HOLD ) {
		if ( hold->type == PUSH_VAR ) {
			sprintf(temp," holding for %s",getVariable(hold->value.number));
		} else {
			sprintf(temp," holding for %.3f",hold->value.floatNumber);
		}
		strcat(appendString,temp);
	}

	
	// the action to perform
	// int action val
	message = PopStack();
	num_pushed+=6;
	int action = message->value.number;

	if ( !(flags & ANIMATE_EMOTION) ) {
		if ( action > 0 && action < 356) {
			sprintf(emotionOrAction," performing action %s",anims[action]);
		} else {
			sprintf(emotionOrAction," performing action %i",action);
		}	
	}
	
	
	// entity edictvalue
	// the entity to animate
	message = PopStack();
	num_pushed+=6;
	char * who = getVariable(message->value.number);



	fprintf(outfile,"animate entity %s%s%s%s",who,emotionOrAction,appendString,newline);

	//animate entity
	saveLineNumber(startbuffer,num_pushed);
	return buffer - startbuffer;
}


int handleWaitAllAny(char * buffer) {
	char * startbuffer = buffer;
	int push_count = 0;
	
	struct stackItem * message;

	

	int count = *(unsigned char*)buffer;
	buffer++;
	count &= ~WAIT_CLEAR;
	int initCount = count;

	struct stackItem ** miniStack = (struct stackItem **)malloc(sizeof(struct stackItem*) * count);

	//printf("COUNT IS %i\n",count);

	for(int e = 0;e<count;e++){

		message = PopStack();
		miniStack[e] = message;
		push_count+=6;
	}
	char final[128] = "wait for all clearing " ;

	for (int e=count-1;e >=0;e--) {

		if ( e < count-1) {
			strcat(final,",");
		}
		//printf("count is %i\n",count);
		//printf("LULUL: %08X\n",miniStack[e]->value.number);
		strcat(final,getVariable(miniStack[e]->value.number));
	}
	

	// yeye get sig later cba.
	fprintf(outfile,"%s%s",final,newline);

	//waitallany
	saveLineNumber(startbuffer,push_count);
	return buffer - startbuffer;
}


int handleWaitSeconds(char * buffer) {
	
	char * startbuffer = buffer;
	struct stackItem * message;
	
	message = PopStack();


	if ( message->type == PUSH_CONST_INT ) {
		fprintf(outfile,"wait %i seconds%s",message->value.number,newline);
	} else if( message->type == PUSH_CONST_FLOAT ) {
		fprintf(outfile,"wait %.3f seconds%s",message->value.floatNumber,newline);
	} else if ( message->type == PUSH_VAR ) {
		fprintf(outfile,"wait %s seconds%s",getVariable(message->value.number),newline);
	}
	
	
	saveLineNumber(startbuffer,6);
	return buffer - startbuffer;
}

int handlePrintMessage(char * buffer)
{
	
	char * startbuffer = buffer;
	struct stackItem * message;
	int num_pushed = 0;


	int flags = *(unsigned char*)buffer;
	buffer++;

	int level;

	char * startText;
	//string or INT i guess.
	//int == string package index
	message = PopStack();
	
	if ( message->type == PUSH_CONST_STRING ) {
		num_pushed += strlen(message->value.nullString) + 1 + 2;
		//add 2 for the quotes wrap in double quotes
		startText = (char*)malloc(strlen(message->value.nullString) + 1 + 2 );
		sprintf(startText,"\"%s\"",message->value.nullString);
	} else {
		num_pushed += 6;
		//64 lenght number lul
		startText = (char*)malloc(64);
		sprintf(startText,"%i",message->value.number);
	}

	if (flags & PRINT_LEVEL)
	{
		//int
		message = PopStack();
		num_pushed+=6;

		level = message->value.number;

	}

	char extraText[256] = "";
	char extra[64] = "";
	if (flags & PRINT_ENTITY) {
		//entity
		message = PopStack();
		num_pushed+=6;

		sprintf(extra," to entity %s",getVariable(message->value.number));
		strcat(extraText,extra);
	}

	if (flags & PRINT_LEVEL)
	{
		sprintf(extra," at level %i",level);;
		strcat(extraText,extra);
	}


	saveLineNumber(startbuffer,num_pushed);
	fprintf(outfile,"print %s%s%s",startText,extraText,newline);

	return buffer - startbuffer;
}

/*
	if ( ! cond ) goto endif
*/
int handleIf(char * buffer,bool specialOnCase) {
	
	char * startbuffer = buffer;
	struct stackItem * compare2;
	struct stackItem * compare1;
	compare2 = PopStack();
	compare1 = PopStack();
			
	int condType = *(unsigned char*)buffer;
	buffer++;

	char condString[3];
	switch ( condType) {
	case COND_GREATER_THAN:
		strcpy(condString,">");
		break;
	case COND_EQUAL:
		strcpy(condString,"==");
		break;
	case COND_NOT_EQUAL:
		strcpy(condString,"!=");
		break;
	case COND_LESS_THAN:
		strcpy(condString,"<");
		break;

	case COND_LESS_THAN_EQUAL:
		strcpy(condString,"<=");
		break;
	case COND_GREATER_THAN_EQUAL:
		strcpy(condString,">=");
		break;
	default:
		errorExit("New Condition Type\n");
		break;
	}

	char compare1s[128], 	compare2s[128];

	// printf("add: %08X %08X\n",compare1s,compare2s);
	struct stackItem * p = compare1;
	char * c = compare1s;
	int bytesForLineNumCalc = 0;
	for (int i =0; i<2;i++) {
		switch(p->type) {
		case PUSH_CONST_INT:
				//64 digit long number
				// c = (char*)malloc(64);
				// printf("LUL %08X %08X\n",*cc,compare2s);
			sprintf(c,"%i",p->value.number);
				// printf("s2 = %s %08X\n",*cc,cc);
				// printf("s2 = %s %08X\n",compare2s,&compare2s);
			bytesForLineNumCalc+=6;
			break;

		case PUSH_CONST_FLOAT:
				//64 digit long number
				// c = (char*)malloc(64);
				// printf("LUL %08X %08X\n",*cc,compare2s);
			sprintf(c,"%.3f",p->value.floatNumber);
				// printf("s2 = %s %08X\n",*cc,cc);
				// printf("s2 = %s %08X\n",compare2s,&compare2s);
			bytesForLineNumCalc+=6;
			break;

		case PUSH_CONST_VECTOR:
			errorExit("Unknown PUSH_CONST_VECTOR in if statement\n");

			bytesForLineNumCalc+=14;
			break;

		case PUSH_CONST_ENTITY:
			errorExit("Unknown PUSH_CONST_ENTITY in if statement\n");
			bytesForLineNumCalc+=6;
			break;

		case PUSH_CONST_STRING:
			bytesForLineNumCalc += strlen(p->value.nullString)+1+2;
			strcpy(c,p->value.nullString);
			break;

		case PUSH_VAR:
			strcpy(c,getVariable(p->value.number));
			bytesForLineNumCalc+=6;
			break;

			// 
		case PUSH_VAR_WITH_FIELD:
			sprintf(c,"%s.%s",getVariable(p->value.two_numbers[0]),getField(p->value.two_numbers[1]));
				// errorExit("Unknown PUSH_VAR_WITH_FIELD in if statement\n");
			bytesForLineNumCalc+=10;
			break;

		case PUSH_FUNCTION:
			errorExit("Unknown PUSH_FUNCTION in if statement\n");
			break;

		default:
			errorExit("unknown PUSH VAR in IF\n");
			break;
		}

		c = (char*)compare2s;
		p = (struct stackItem *)compare2;
		
	}

	// for IF. not ON
	if (!specialOnCase) saveLineNumber(startbuffer,bytesForLineNumCalc);
	

	// read the jump offset if not true.
	int endif_offset = *(int*)buffer;
	buffer+=4;

	// printf("%08X %08X %08X\n",compare1s,condString,compare2s);

	DebugPrintf("IF BEFORE 0\n");
	if ( !specialOnCase ) {
		//IF
		/*
			We want to increment num_endifs.
		*/

		//Does it exist?

		DebugPrintf("IF BEFORE 11\n");
		labelFixUp_t * fixUp = getFixUp(endif_offset);
		if ( !fixUp ) {
			//create new.
			fixUp = appendFixUp(endif_offset,0);

			DebugPrintf("IF BEFORE 55\n");
		}

		DebugPrintf("%08X %08X %08X %08X\n",compare1s,condString,compare2s,outfile);
		fprintf(outfile,"if %s %s %s%s",compare1s,condString,compare2s,newline);
		
		fixUp->num_endifs += 1;
		DebugPrintf("IF BEFORE 22\n");
	}
	else {
		DebugPrintf("IF BEFORE 1\n");
		//ON
		//the on statement uses the ifNOTJUMP as its goto...
		/*
			GENERATES LABEL!!!
		*/
		labelFixUp_t * fixUp = getFixUp(endif_offset);
		DebugPrintf("IF BEFORE 2\n");
		if ( !fixUp ) {
			//create new.
			fixUp = appendFixUp(endif_offset,realLabelsCount+1);
			//label names are 1:1 to fixups. (endifs are not.)
			realLabelsCount+=1;

			DebugPrintf("IF BEFORE 3\n");
		}

		char temp[64];
		sprintf(temp,"%i", fixUp->lab_num);

		DebugPrintf("IF BEFORE 4\n");
		char temp2[64];
		sprintf(temp2,"goto %s%s%s","lab_",temp,newline);
		fprintf(outfile,"on %s %s %s %s",compare1s,condString,compare2s,temp2);
	}
	
	return buffer - startbuffer;
}

/*
	index_var = index_var + value
*/
int handleAddAssignment(char * buffer,const char * symbol) {
	char * startbuffer = buffer;
	
	int num_pushed = 0;
	struct stackItem * index_var;
	struct stackItem * new_value;
	index_var = PopStack();
	new_value = PopStack();

	char leftSide[64];

	switch ( index_var->type ) {
	case PUSH_VAR:
		num_pushed+=6;
		sprintf(leftSide,"%s",getVariable(index_var->value.number));
		break;
	case PUSH_VAR_WITH_FIELD:
			//LEFT SIDE VAR WITH FIELD
		num_pushed+=10;
		sprintf(leftSide,"%s.%s",getVariable(index_var->value.two_numbers[0]),getField(index_var->value.two_numbers[1]));
		break;
	}

	char * varVarName = NULL;
	char * fieldName = NULL;

//RIGHT SIDE OF ASSIGNMENT
	char final[128];
	switch ( new_value->type ) {
		//IF LESS SIDE CONTAINED MOVETYPE IN ITS FIELD
	case PUSH_CONST_INT:
		if ( index_var->type == PUSH_VAR_WITH_FIELD 
			&& !stricmp(getField(index_var->value.two_numbers[1]),"movetype")
			&& new_value->value.number >=0 && new_value->value.number < 11 ) {
			fprintf(outfile,"%s = %s%s",leftSide,movetypes[new_value->value.number],newline);	
		} else
		fprintf(outfile,"%s %s= %i%s",leftSide,symbol,new_value->value.number,newline);

		num_pushed+=6;
		break;

	case PUSH_CONST_FLOAT:
		fprintf(outfile,"%s %s= %.3f%s",leftSide,symbol,new_value->value.floatNumber,newline);
		num_pushed+=6;
		break;

	case PUSH_CONST_VECTOR:
		fprintf(outfile,"%s %s= [%.3f ,%.3f ,%.3f]%s",leftSide,symbol,new_value->value.three_floats[0],new_value->value.three_floats[1],new_value->value.three_floats[2],newline);
		num_pushed+=14;
		break;

	case PUSH_CONST_ENTITY:
		errorExit("Unknown PUSH_CONST_ENTITY\n");
		num_pushed+=6;
		break;

	case PUSH_CONST_STRING:
		sprintf(final,"%s %s= %s%s",leftSide,symbol,new_value->value.nullString,newline);
		num_pushed+=strlen(final)+1+2;
		fprintf(outfile,final);
		break;

	case PUSH_VAR:
		varVarName = getVariable(new_value->value.number);
		if (varVarName == NULL ) {
			errorExit("No label error\n");
		}
		fprintf(outfile,"%s %s= %s%s",leftSide,symbol,varVarName,newline);
		num_pushed+=6;
		break;

	case PUSH_VAR_WITH_FIELD:
		varVarName = getVariable(new_value->value.two_numbers[0]);
		if (varVarName == NULL ) {
			errorExit("No label error\n");
		}
		fieldName = getField(new_value->value.two_numbers[1]);
		if (fieldName == NULL ) {
			errorExit("No label error\n");
		}
		fprintf(outfile,"%s %s= %s.%s%s",leftSide,symbol,varVarName,fieldName,newline);
		num_pushed+=10;
		break;

	case PUSH_CUSTOM:
	case PUSH_FUNCTION:
		sprintf(final,"%s %s= %s%s",leftSide,symbol,new_value->value.nullString,newline);
		fprintf(outfile,final);
				// num_pushed+=strlen(new_value->value.nullString)+1+2;
		num_pushed+= new_value->secret_size;
		break;

	default:
		errorExit("unknown PUSH VAR in IF\n");
		break;
	}


	//Assignment
	// printf("NOOB_ADDASSIGNMENT : %i\n",(int)(((unsigned int)startbuffer-1 - num_pushed) - (unsigned int)fileContents));
	saveLineNumber(startbuffer,num_pushed);
	return buffer - startbuffer;
}

int handleConsoleCommand(char * buffer)
{
	
	char * startbuffer = buffer;
	struct stackItem * message;

	message = PopStack();
	// push 1 .. push type .. push null
	saveLineNumber(startbuffer,strlen(message->value.nullString)+1+2);

	fprintf(outfile,"run console command \"%s\"%s",message->value.nullString,newline);

	return buffer - startbuffer;
}

/*
	index_var = new_value
*/
int handleAssignment(char * buffer) {
	
	char * startbuffer = buffer;
	
	int num_pushed = 0;
	struct stackItem * index_var;
	struct stackItem * new_value;
	index_var = PopStack();
	new_value = PopStack();

	char leftSide[64];



	char *c = NULL;

	switch ( index_var->type ) {
	case PUSH_CONST_ENTITY:
	case PUSH_VAR:
		num_pushed+=6;
		sprintf(leftSide,"%s",getVariable(index_var->value.number));
		break;
	case PUSH_VAR_WITH_FIELD:
			//LEFT SIDE VAR WITH FIELD
		num_pushed+=10;
		c = getVariable(index_var->value.two_numbers[0]);
		if ( c == NULL ) {
			errorExit("Bad variable in assignment\n");
		}
		c = getField(index_var->value.two_numbers[1]);
		if ( c == NULL ) {
			errorExit("Bad variable in assignment\n");
		}
		sprintf(leftSide,"%s.%s",getVariable(index_var->value.two_numbers[0]),getField(index_var->value.two_numbers[1]));
		break;
	}

	char * varVarName = NULL;
	char * fieldName = NULL;




//RIGHT SIDE OF ASSIGNMENT
	char final[256];
	switch ( new_value->type ) {
		//are we assigning to a xyz.movetype = 
	case PUSH_CONST_INT:
		if ( index_var->type == PUSH_VAR_WITH_FIELD 
			&& !stricmp(getField(index_var->value.two_numbers[1]),"movetype")
			&& new_value->value.number >=0 && new_value->value.number < 11 ) {
			fprintf(outfile,"%s = %s%s",leftSide,movetypes[new_value->value.number],newline);	
		} else
			fprintf(outfile,"%s = %i%s",leftSide,new_value->value.number,newline);

		num_pushed+=6;
	break;

	case PUSH_CONST_FLOAT:
		fprintf(outfile,"%s = %.3f%s",leftSide,new_value->value.floatNumber,newline);
		num_pushed+=6;
		break;

	case PUSH_CONST_VECTOR:
		fprintf(outfile,"%s = [%.3f ,%.3f ,%.3f]%s",leftSide,new_value->value.three_floats[0],new_value->value.three_floats[1],new_value->value.three_floats[2],newline);
		num_pushed+=14;
		break;

	case PUSH_CONST_ENTITY:
		errorExit("Unknown PUSH_CONST_ENTITY\n");
		num_pushed+=6;
		break;

	case PUSH_CONST_STRING:
		sprintf(final,"%s = %s%s",leftSide,new_value->value.nullString,newline);
				//secret_size from math function
		num_pushed+=strlen(final)+1+2;
		fprintf(outfile,final);
		break;

	case PUSH_VAR:
		varVarName = getVariable(new_value->value.number);
		if (varVarName == NULL ) {
			errorExit("No label error PUSH_VAR AddASsignment\n");
		}
		fprintf(outfile,"%s = %s%s",leftSide,varVarName,newline);
		num_pushed+=6;
		break;

	case PUSH_VAR_WITH_FIELD:
		varVarName = getVariable(new_value->value.two_numbers[0]);
		if (varVarName == NULL ) {
			errorExit("No label error PUSH_VAR_WITH_FIELD AddASsignment\n");
		}
		fieldName = getField(new_value->value.two_numbers[1]);
		if (fieldName == NULL ) {
			errorExit("No label error PUSH_VAR_WITH_FIELD AddASsignment\n");
		}
		fprintf(outfile,"%s = %s.%s%s",leftSide,varVarName,fieldName,newline);
		num_pushed+=10;
		break;

	case PUSH_CUSTOM:
	case PUSH_FUNCTION:
		sprintf(final,"%s = %s%s",leftSide,new_value->value.nullString,newline);
		fprintf(outfile,final);
				// num_pushed+=strlen(new_value->value.nullString)+1+2;
		num_pushed+= new_value->secret_size;

				// printf("WHAT THE ACTUAL FUCKAFTERRRRRRRRR : %i\n",num_pushed);
		break;

	default:
		errorExit("unknown PUSH VAR in IF\n");
		break;
	}



	// TODO:handle TYPES



	//Assignment

	saveLineNumber(startbuffer,num_pushed);
	return buffer - startbuffer;

}

/*
	The linenumber of the output .ds which marks the start of a .os OPCODE.
*/
void saveLineNumber(char * buffer, int sizepushed) {

	DebugPrintf("%08X\n",buffer);
	lineNumbersToOffset[lineNumbers] = (int)(((unsigned int)buffer-1 - sizepushed) - (unsigned int)fileContents);
	DebugPrintf("%08X\n",lineNumbersToOffset[lineNumbers]);
	lineNumbers++;
}

//unknown size str
/*
opcode
name_of_variable string
type byte
var_index
*/
int handleStandardOpcode(const char *startwords,char * buffer, bool assign) {
	char * startbuffer = buffer;
	char * varName = startbuffer;
	//variables etc
	
	int read = 0;
	preventXYZFields++;
	
	if ( preventXYZFields > 3 )
		saveLineNumber(varName);
	
	//name of variable
	buffer += strlen(buffer)+1;

	//
	VariableT type = (VariableT)(*(unsigned char*)buffer);
	buffer +=1;
	
	// value_t val;
	// val.number = 
	char dataType[64];

	
	int index = *(int*)buffer;
	buffer+=4;

	char varNameQuotes[64];
	sprintf(varNameQuotes,"\"%s\"",varName);


	/*
		30.04.2018
		I want to save the type with the variable/field because its needed
		to resolve the variable's value at push time instead of 'use' time.
		This makes the code way less messsier and reduces code repeatability.
		EDIT: No. Thats bad idea.
		Decompiler wants to preserve variable names, type does not matter.
	*/
	if ( !strcmp(startwords,"field" ) ) {
		//printf("FIELD CREATION:D %s at INDEX : %02X\n",varName,index);
		appendField(varName,index);
		varName = varNameQuotes;
	}
	else{ 
		//printf("VARNAME CREATION:D %s at INDEX : %i\n",varName,index);
		appendVariable(varName,index);
	}
	
	switch (type) { 
	case TypeINT:
		strcpy(dataType,"int");
		break;
	case TypeFLOAT:
		strcpy(dataType,"float");
		break;
	case TypeVECTOR:
		strcpy(dataType,"vector");
		break;
	case TypeENTITY:
		strcpy(dataType,"entity");
		break;
	case TypeSTRING:
		strcpy(dataType,"string");
		break;
	case TypeUNKNOWN:
		errorExit("Unknown Variable\n");
		break;
	}

	char value[64];

	if ( assign ) {
		//handle vector assignment please.
		switch (type) {
		case TypeINT:
			sprintf(value,"%i",*(int*)buffer);
			buffer+=4;
			break;
		case TypeFLOAT:
			strcpy(dataType,"float");
			sprintf(value,"%.3f",*(int*)buffer);
			buffer+=4;
			break;
		case TypeVECTOR:
			strcpy(dataType,"vector");
			sprintf(value,"[%.3f ,%.3f ,%.3f]",*(float*)(buffer+4),*(float*)(buffer+8),*(float*)(buffer+8));
			buffer+=12;
			break;
		case TypeENTITY:
			strcpy(dataType,"entity");
			sprintf(value,"%i",*(int*)buffer);
			buffer+=4;
			break;
		case TypeSTRING:
			strcpy(dataType,"string");
			strcpy(value,buffer);
			buffer+=strlen(buffer);
			break;
		}

		if ( preventXYZFields > 3 )
			fprintf(outfile,"%s %s %s = %s%s",startwords,dataType,varName,value,newline);
	} else {
		if ( preventXYZFields > 3 )
			fprintf(outfile,"%s %s %s%s",startwords,dataType,varName,newline);	
	}

	read = buffer - startbuffer;
	return read;
}

/*
	Creates a Label.
	This is also called for:
		else
		while
*/
int handleGoto(char * buffer) {
	/*
		GENERATES LABEL!!!
	*/
	char * b = buffer;
	//goto
	saveLineNumber(b);
	
	int labelPosition = *(int*)buffer;
	DebugPrintf("Goto Offset = %i\n",labelPosition);
	buffer+=4;

	//int gotoPosition = (int)(b-1)-(int)fileContents;

	labelFixUp_t * fixUp = getFixUp(labelPosition);
	if ( !fixUp ) {
		//create new.
		fixUp = appendFixUp(labelPosition,realLabelsCount+1);

		//label names are 1:1 to fixups. (endifs are not.)
		realLabelsCount+=1;
	}

	char temp[64];
	sprintf(temp,"%i", fixUp->lab_num);

	fprintf(outfile,"goto %s%s%s","lab_",temp,newline);

	// labels are written to the file at the end. endif is a label
	return buffer - b;
}



void seekToEnthLine(int n,char * lineContent, int length) {

	//go to end to calc size.
	if ( !fseek(outfile,0,SEEK_END) ) {
		int size = ftell(outfile);
		//seek to beginning
		rewind(outfile);
		
		// printf("size is %i\n",size);

		char * dsFile = (char*)malloc(size);	
		char * newBuffer = (char*)malloc(size+length);
		fread(dsFile,1,size,outfile);

		// int i = 0;
		// for (i = 0;i<size;i++) {
		// 	printf(" %02X ",dsFile[i]);
		// }
		// printf("\n");

		char * startofline;
		char * newline = startofline = dsFile;

		// printf("%i\n",newline);
		int x = 0;
		
		while ( (int)(newline-dsFile+1) < size && (newline = strstr(newline+1,"\n")) ) {
			// printf("found a line!\n");

			if ( x == n ) { 
				//insert line here
				int loc = (int)(startofline-dsFile);
				// 
				if (loc > 0 ) {
					//copy what came before.
					memcpy(newBuffer,dsFile,loc);	
				}
				//copy special stuff
				memcpy(newBuffer+loc,lineContent,length);
				//copy stuff beyond loc
				memcpy(newBuffer+loc+length,dsFile+loc,size-loc);

				rewind(outfile);
				fwrite(newBuffer,size+length,1,outfile);
				
				break;
				
			}
			startofline=newline+1;
			x++;
		}


	} else {
		errorExit("fseek failed\n");
	}
}


void PatchUpJumps(void) {

	char lol[32];

	
	dumpLineOffsetTable();
	dumpFixUps();

	#if 0
		
		return;
	#endif


	int i = 0;
	labelFixUp_t *fix = NULL;
	/*
		int lab_num
		int label_offset;
	*/

	labelFixUp_t *fixnext;

	for ( fix=FixUps.next; fix != &FixUps; fix=fixnext ) {
		fixnext = fix->next;
		if ( !fix->active ) continue;
		bool found_line_num = false;
		for ( i=0;i < lineNumbers;i++ ) {
			if ( lineNumbersToOffset[i] == fix->label_offset ) {
				found_line_num = true;
				break;
			}
		}
		/*
		1) endifs must be output before labels and do not dissolve.
		2) multiple labels dissolve into one.
		*/
		if ( !found_line_num ) errorExit("Unexpected error parsing, contact developer.");
		

		if ( fix->lab_num ) {
			sprintf(lol,"label lab_%i%s",fix->lab_num,newline);
			seekToEnthLine(i,lol,strlen(lol)); //inserts line
		}

		for ( int j = 0; j < fix->num_endifs; j++ ) {
			sprintf(lol,"endif%s",newline);
			seekToEnthLine(i,lol,strlen(lol)); //inserts line
		}

		
		
		//update line number mappings.
		if ( fix->lab_num ) {

			//Right Shift 
			lineNumbers++;
			for (int j = lineNumbers-1; j > i; j-- ) {
				lineNumbersToOffset[j] = lineNumbersToOffset[j-1];
			}
			lineNumbersToOffset[i] = -1;
		}
		
		for ( int c = 0; c < fix->num_endifs; c++ ) {
			lineNumbers++;
			for (int j = lineNumbers-1; j > i; j-- ) {
				lineNumbersToOffset[j] = lineNumbersToOffset[j-1];
			}
			lineNumbersToOffset[i] = -1;
		}

		removeFixUp(fix);

	}

}


// Index to VarName mapping
void appendField(char * name,int idx) {
	vars_t *l = (vars_t*)malloc(sizeof(vars_t));
	l->next = Fields.next;
	l->prev = &Fields;
	Fields.next->prev = l;
	Fields.next = l;

	l->index = idx;
	l->varName = (char*)malloc(strlen(name)+1);
	strcpy(l->varName,name);
}

// cut out of chain
void removeField(vars_t *which) {
	which->prev->next = which->next;
	which->next->prev = which->prev;

	free(which->varName);
	free(which);
}

char * getField(int index) {
	vars_t	*l, *next;
	for (l=Fields.next ; l != &Fields ; l=next)
	{
		next = l->next;
		if (l->index == index) {
			// printf("found label : %s\n",l->varName);
			return l->varName;
		}
	}

	return NULL;
}


// Index to VarName mapping
void appendVariable(char * name,int idx) {
	vars_t *l = (vars_t*)malloc(sizeof(vars_t));
	l->next = Variables.next;
	l->prev = &Variables;
	Variables.next->prev = l;
	Variables.next = l;

	l->index = idx;
	l->varName = (char*)malloc(strlen(name)+1);
	strcpy(l->varName,name);
}

// cut out of chain
void removeVariable(vars_t *which) {
	which->prev->next = which->next;
	which->next->prev = which->prev;

	free(which->varName);
	free(which);
}

char * getVariable(int index) {
	vars_t	*l, *next;
	for (l=Variables.next ; l != &Variables ; l=next)
	{
		next = l->next;
		if (l->index == index) {
			// printf("found label : %s\n",l->varName);
			return l->varName;
		}
	}

	return NULL;
}

labelFixUp_t * getFixUp(int labelOffset)
{
	labelFixUp_t	*l, *next;
	for (l=FixUps.next ; l != &FixUps ; l=next)
	{
		next = l->next;
		if (l->label_offset == labelOffset) {
			return l;
		}
	}
	return NULL;
}


void dumpLineOffsetTable(void) {
	printf("trying to print line numbers\n");
	for (int i = 0 ; i < lineNumbers;i++) {
		printf("offset stored for line number %i : %i\n",i,lineNumbersToOffset[i]);
	}
}

void dumpFixUps(void) {
	printf("trying to print fixups\n");
	labelFixUp_t	*l, *next;
	for (l=FixUps.next ; l != &FixUps ; l=next)
	{
		next = l->next;
		
		printf("fixup with %i\n",l->label_offset);
	}
}

labelFixUp_t* appendFixUp(int label_offset, int lab_num) {
	// Attempt to allocate memory for labelFixUp_t
	labelFixUp_t *l = (labelFixUp_t*)malloc(sizeof(labelFixUp_t));
	
	// Check if memory allocation was successful
	if (l == NULL) {
		errorExit("Memory allocation failed in appendFixUp\n");
	}

	// Initialize the allocated structure
	l->next = NULL;
	l->prev = NULL;
	l->active = true;
	l->lab_num = lab_num;
	l->label_offset = label_offset;
	l->num_endifs = 0;

	// Check if FixUps is properly initialized
	if (FixUps.next == NULL || FixUps.prev == NULL) {
		errorExit("FixUps is not properly initialized in appendFixUp\n");
	}

	// Insert the new node into the linked list
	l->next = FixUps.next;
	l->prev = &FixUps;
	FixUps.next->prev = l;
	FixUps.next = l;

	l->active = true;
	l->lab_num = lab_num;
	l->label_offset = label_offset;
	l->num_endifs = 0;

	DebugPrintf("FixUp node appended successfully\n");
	return l;
}


void removeFixUp(labelFixUp_t *which) {
	which->prev->next = which->next;
	which->next->prev = which->prev;
	free(which);
}


/*
typedef struct zhead_s
{
	struct zhead_s	*prev, *next;
} zhead_t;
ONINIT
z_chain.next = z_chain.prev = &z_chain;
ONAPPEND
zhead_t	*z = malloc(sizeof(zhead_t));

z->next = z_chain.next;
z->prev = &z_chain;
z_chain.next->prev = z;
z_chain.next = z;
ONREMOVE
z->prev->next = z->next;
z->next->prev = z->prev;
ONSEARCH
zhead_t	*z, *next;
for (z=z_chain.next ; z != &z_chain ; z=next)
{
	next = z->next;
	if (matches) {
		//play with z here
	}
}
*/

