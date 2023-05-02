#include <string.h>
const char *movetypes[11];


void initMovetypes(void) {
// Physic movetypes
movetypes[0] = "MOVETYPE_NONE";		
movetypes[1] = "MOVETYPE_NOCLIP";		
movetypes[2] = "MOVETYPE_PUSH";		
movetypes[3] = "MOVETYPE_STOP";		
movetypes[4] = "MOVETYPE_WALK";		
movetypes[5] = "MOVETYPE_STEP";		
movetypes[6] = "MOVETYPE_FLY";		
movetypes[7] = "MOVETYPE_TOSS";		
movetypes[8] = "MOVETYPE_FLYMISSILE";	
movetypes[9] = "MOVETYPE_BOUNCE";		
movetypes[10] = "MOVETYPE_DAN";
}