#ifndef __STARTUPSTATE_H__
#define __STARTUPSTATE_H__
//////////////////////--By Ross Davies--//////////////////////
/*		State object that is called at startup, draws a 
		splash screen and allows the player to progress
		to next state once all assets have been loaded		*/
//////////////////////////////////////////////////////////////
#include "gamestate.h"
#include "primitives.h"

#include <string>

class StartupState : public GameState
{
public:
	//  Constructor
	StartupState();
	//  Destructor
	virtual ~StartupState();
	
	//  Functions for hooking into virtuals on parent
	//  Initialise loads necessary assets	
	virtual void Initialise();
	
	//  Update waits for player to hit start	
	virtual void Update();
	
	//  Render draws assets to the screen	
	virtual void Render();

private:
	//  A secondary render function for drawing 'loading'
	//  to the screen
	void InitRender();
	
	//  Sprites for holding the textures we wish to draw
	PS2SpriteT splash;
	PS2SpriteT loadingStart;
	PS2SpriteT titleText;
	
	//  The files holding the textures we need
	std::string splashImage;
	std::string textImage;	
};



#endif