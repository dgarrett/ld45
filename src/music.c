#include <gb/gb.h>
/* Here's a look at how I created a quick music player for use with GBDK.
It basically defines how to play a note, and then stores an array of notes
to be played as a timer interates through the beats */

int currentBeat;
int timerCounter;
//Define note names
typedef enum {
  C3, Cd3, D3, Dd3, E3, F3, Fd3, G3, Gd3, A3, Ad3, B3,
  C4, Cd4, D4, Dd4, E4, F4, Fd4, G4, Gd4, A4, Ad4, B4,
  C5, Cd5, D5, Dd5, E5, F5, Fd5, G5, Gd5, A5, Ad5, B5,
  C6, Cd6, D6, Dd6, E6, F6, Fd6, G6, Gd6, A6, Ad6, B6,
  C7, Cd7, D7, Dd7, E7, F7, Fd7, G7, Gd7, A7, Ad7, B7,
  C8, Cd8, D8, Dd8, E8, F8, Fd8, G8, Gd8, A8, Ad8, B8,
  SILENCE
} pitch;

const UWORD frequencies[] = { //values based on a formula used by the GB processor
  44, 156, 262, 363, 457, 547, 631, 710, 786, 854, 923, 986,
  1046, 1102, 1155, 1205, 1253, 1297, 1339, 1379, 1417, 1452, 1486, 1517,
  1546, 1575, 1602, 1627, 1650, 1673, 1694, 1714, 1732, 1750, 1767, 1783,
  1798, 1812, 1825, 1837, 1849, 1860, 1871, 1881, 1890, 1899, 1907, 1915,
  1923, 1930, 1936, 1943, 1949, 1954, 1959, 1964, 1969, 1974, 1978, 1982,
  1985, 1988, 1992, 1995, 1998, 2001, 2004, 2006, 2009, 2011, 2013, 2015,
  0
};

//Define Instrument names
//Instruments should be confined to one channel
//due to different registers used between ch1, 2, 3, 4
typedef enum {
	NONE,
	MELODY,  //channel 1
	HARMONY, //channel 1
	SNARE,   //channel 4
	CYMBAL   //channel 4
} instrument;

//Define a note as having a pitch, instrument, and volume envelope
typedef struct {
	instrument i;
	pitch p;
	UBYTE env;
} note;

enum DurationNote
{
    LONG = 0x84U,
    SHORT = 0x81U,
    LONG_LONG = 135
};

//define a song as a series of note structs
//This song is a 16 note loop on channel 1
//each channel should have its own array, so
//that multiple notes can be played simultaneously
note song_ch1[] = { //notes to be played on channel 1
{NONE, SILENCE, LONG_LONG},
    {NONE, SILENCE, LONG_LONG},
    {MELODY, E5, LONG},
    {NONE, SILENCE, LONG_LONG},
    {MELODY, D5, LONG},
    {NONE, SILENCE, LONG_LONG},
    {MELODY, E5, LONG},
    {NONE, SILENCE, LONG_LONG},
    {MELODY, C5, LONG},
    {MELODY, C5, LONG},
    {NONE, SILENCE, LONG_LONG},
    {MELODY, D5, LONG},
    {MELODY, C5, LONG_LONG},
    {NONE, SILENCE, LONG_LONG},
    {NONE, SILENCE, LONG_LONG},
    {NONE, SILENCE, LONG_LONG},
    {NONE, SILENCE, LONG_LONG},
    {NONE, SILENCE, LONG_LONG},
    {MELODY, E5, LONG},
    {NONE, SILENCE, LONG_LONG},
    {MELODY, D5, LONG},
    {NONE, SILENCE, LONG_LONG},
    {MELODY, E5, LONG},
    {NONE, SILENCE, LONG_LONG},
    {MELODY, B4, LONG},
    {MELODY, B4, LONG},
    {NONE, SILENCE, LONG_LONG},
    {MELODY, C5, LONG},
    {MELODY, B4, LONG_LONG},
    {NONE, SILENCE, LONG_LONG},
    {NONE, SILENCE, LONG_LONG},
    {NONE, SILENCE, LONG},
    {MELODY, G4, LONG},
    {MELODY, G4, LONG},
    {MELODY, A4, LONG},
    {MELODY, G4, LONG},
    {MELODY, A4, LONG},
    {MELODY, G4, LONG},
    {MELODY, A5, LONG},
    {MELODY, G5, LONG_LONG},
    {NONE, SILENCE, LONG_LONG},
    {NONE, SILENCE, LONG_LONG},
    {MELODY, F5, LONG},
    {MELODY, F5, LONG},
    {MELODY, E5, LONG_LONG},
    {NONE, SILENCE, LONG_LONG},
    {MELODY, E5, LONG_LONG},
    {MELODY, D5, LONG_LONG},
    {NONE, SILENCE, LONG_LONG},
    {MELODY, D5, LONG_LONG},
    {NONE, SILENCE, LONG_LONG},
    {MELODY, D5, LONG},
    {MELODY, C5, LONG_LONG},
    {NONE, SILENCE, LONG_LONG},
    {NONE, SILENCE, LONG_LONG},
    {NONE, SILENCE, LONG_LONG},
    {MELODY, G4, LONG},
    {MELODY, G4, LONG},
    {MELODY, A4, LONG},
    {MELODY, G4, LONG},
    {MELODY, A4, LONG},
    {MELODY, G4, LONG},
    {MELODY, E5, LONG},
    {MELODY, C5, LONG_LONG},
    {NONE, SILENCE, LONG_LONG},
    {NONE, SILENCE, LONG_LONG},
    {NONE, SILENCE, LONG_LONG},
    {MELODY, G4, LONG},
    {MELODY, G4, LONG},
    {MELODY, A4, LONG},
    {MELODY, G4, LONG},
    {MELODY, A4, LONG},
    {MELODY, G4, LONG},
    {MELODY, C5, LONG},
    {MELODY, B4, LONG_LONG},
    {NONE, SILENCE, LONG},
    {NONE, SILENCE, LONG_LONG},
    {NONE, SILENCE, LONG_LONG},
    {MELODY, G4, LONG},
    {MELODY, G4, LONG},
    {MELODY, A4, LONG},
    {MELODY, G4, LONG},
    {MELODY, A4, LONG},
    {MELODY, G4, LONG},
    {MELODY, A5, LONG},
    {MELODY, G5, LONG_LONG},
    {NONE, SILENCE, LONG_LONG},
    {NONE, SILENCE, LONG_LONG},
    {MELODY, F5, LONG},
    {MELODY, F5, LONG},
    {MELODY, E5, LONG_LONG},
    {NONE, SILENCE, LONG_LONG},
    {MELODY, E5, LONG_LONG},
    {MELODY, D5, LONG_LONG},
    {NONE, SILENCE, LONG_LONG},
    {MELODY, D5, LONG_LONG},
    {NONE, SILENCE, LONG_LONG},
    {MELODY, D5, LONG},
    {MELODY, C5, LONG_LONG},
    {NONE, SILENCE, LONG_LONG},
};

//function to set sound registers based on notes chosen
void setNote(note *n){
	switch((*n).i){
		case MELODY:
			// NR10_REG = 0x00U; //pitch sweep
			NR21_REG = 0x84U; //wave duty
			NR22_REG = (*n).env; //envelope
			NR23_REG = (UBYTE)frequencies[(*n).p]; //low bits of frequency
			NR24_REG = 0x80U | ((UWORD)frequencies[(*n).p]>>8); //high bits of frequency (and sound reset)
		break;
		case HARMONY:
			// NR10_REG = 0x01U;
			NR21_REG = 0x00U; //wave duty for harmony is different
			NR22_REG = (*n).env;
			NR23_REG = (UBYTE)frequencies[(*n).p];
			NR24_REG = 0x80U | ((UWORD)frequencies[(*n).p]>>8);
		break;
		case SNARE:
		break;
		case CYMBAL:
		break;
	}
}

//This function plays whatever note is on
//the current beat in Channel 1
void playChannel1(){
	setNote(&song_ch1[currentBeat]);
	NR51_REG |= 0xFFU; //enable sound on all channels
    NR52_REG = 0x80;
    NR50_REG = 0x77;
}

//Timer function gets called 16 times a second
void timerInterrupt(){
	if (timerCounter == 4){ //every 4 ticks is a beat, or 4 beats per second
		timerCounter=0;
		currentBeat = currentBeat == 15 ? 0 : currentBeat+1;
		playChannel1(); //every beat, play the sound for that beat
		//playChannel2(); make more functions to play sounds on 
		//playChannel3(); the other channels
		//playChannel4();
	}
	timerCounter++;
}

/*To have more notes playing simultaneously, i'd need
to make up to three more note arrays, song_ch2, song_ch3, song_ch4,
add new playChannel1/2/3() functions, and add them to the timer interrupt
If there are multiple songs, I can change the playChannel1() function to
refer to a pointer to the 'active song' array, and then change the active
array based on which song is supposed to be playing.
*/