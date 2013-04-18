#ifndef __OPTIONS_H__
#define __OPTIONS_H__
//////////////////////--By Ross Davies--//////////////////////
/*		Storage struct for options, to be passed between
		options state and play state, all options
		explicitly enumerated so that integer maths can
		be used to simplify the transitions					*/
//////////////////////////////////////////////////////////////
struct Options
{
	enum Difficulty {EASY = 0, MEDIUM = 1, HARD = 2, INSANE = 3};
	enum Rumble {RUMBLEON = 0, RUMBLEOFF = 1};
	enum Sensitivity {LOW = 0, MED = 1, HIGH = 2}; 
	
	Difficulty difficulty;
	Rumble rumble;
	Sensitivity sensitivity;
};


#endif