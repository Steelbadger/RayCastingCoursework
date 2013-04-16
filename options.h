#ifndef __OPTIONS_H__
#define __OPTIONS_H__

struct Options
{
	enum Difficulty {EASY = 0, MEDIUM = 1, HARD = 2, INSANE = 3};
	enum Rumble {RUMBLEOFF = 0, RUMBLEON = 1};
	enum Sensitivity {LOW = 0, MED = 1, HIGH = 2}; 
	
	Difficulty difficulty;
	Rumble rumble;
	Sensitivity sensitivity;
};


#endif