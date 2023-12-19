#include <string.h>


struct id_name_mapping {
	int id;
	const char * name;
};

struct id_name_mapping chans[9];
struct id_name_mapping attns[4];

#define MAX_ATTNS 4
#define MAX_CHANS 10


void initSounds(void) {

	chans[0].id = 0;
	chans[0].name = "CHAN_AUTO";

	chans[1].id = 1;
	chans[1].name = "CHAN_WEAPON";

	chans[2].id = 2;
	chans[2].name = "CHAN_VOICE";

	chans[3].id = 3;
	chans[3].name = "CHAN_ITEM";

	chans[4].id = 4;
	chans[4].name = "CHAN_BODY";

	chans[5].id = 5;
	chans[5].name = "CHAN_ENT1";

	chans[6].id = 6;
	chans[6].name = "CHAN_ENT2";

	chans[7].id = 8;
	chans[7].name = "CHAN_NO_PHS_ADD";

	chans[8].id = 16;
	chans[8].name = "CHAN_RELIABLE";




	attns[0].id = 0;
	attns[0].name = "ATTN_NONE";

	attns[1].id = 1;
	attns[1].name = "ATTN_NORM";

	attns[2].id = 2;
	attns[2].name = "ATTN_IDLE";

	attns[3].id = 3;
	attns[3].name = "ATTN_STATIC";
}



// search for chan with id == input
// and return its name
const char * findChannel(int input) {
	//iterate structs
	for ( int i = 0 ; i < MAX_CHANS; i ++ ){
		if ( chans[i].id == input ) {
			return chans[i].name;
		}
	}
	return NULL;
}


const char * findAtten(float input) {
	//iterate structs
	for ( int i = 0 ; i < MAX_ATTNS; i ++ ){
		if ( (float)(attns[i].id) == input ) {
			return chans[i].name;
		}
	}
	return NULL;

}