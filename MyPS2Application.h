#ifndef _MYPS2APPLICATION_H
#define _MYPS2APPLICATION_H
#include "gamestate.h"
#include "startupstate.h"
#include "menustate.h"
#include "playstate.h"
#include "pausedstate.h"
#include "helpstate.h"
#include "gamewinstate.h"
#include "gamelosestate.h"
#include "optionsstate.h"

#include "level.h"


//*** TYPES AND CLASSES
class MyPS2Application
//  DOOOOOOOM
{
public:
	MyPS2Application();
	void Run();
	static void set_quitting(bool quitting) { quitting_ = quitting; }
	static bool quitting() { return quitting_; }
private:
	void Init();
	void CleanUp();
	void Update();
	void Render();
	void CheckState();
	
	GameState* currentState;
	
	StartupState startupState;
	MenuState menuState;
	PlayState playState;
	PausedState pauseState;
	HelpState helpState;
	GameWinState winState;
	GameLoseState loseState;
	OptionsState optionsState;
	

	//  game loop boolean
	static bool quitting_;
	
};

#endif // _MYPS2APPLICATION_H