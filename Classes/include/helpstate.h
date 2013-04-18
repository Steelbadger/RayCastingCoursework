#ifndef __HELPSTATE_H__
#define __HELPSTATE_H__
//////////////////////--By Ross Davies--//////////////////////
/*		State object that is called when the player hits
		help in any menu, very simple visual ouput, draws
		a single textured quad to the screen with the
		controls											*/
//////////////////////////////////////////////////////////////
#include "gamestate.h"
#include "primitives.h"

#include <string>

class HelpState : public GameState
{
public:
	//  Constructor
	HelpState();
	
	//  Destructor
	virtual ~HelpState();
	
	//  Functions for hooking into virtuals on parent
	//  Initialise loads necessary assets	
	virtual void Initialise();
	
	//  Update waits for input		
	virtual void Update();
	
	//  Render draws assets to the screen
	virtual void Render();
	
	//  expects a pointer to either the menu state
	//  or the paused state, allows game to return
	//  to the state from which help was accessed
	virtual void PriorState(GameState* priorState);
	
private:

	//  pointer to prior state so that we can return to
	//  it upon close of this state
	GameState* priorState;
	
	//  Fullscreen sprite for drawing help image to
	PS2SpriteT helpScreen;	
	
	//  help image splash texture
	std::string helpImage;
};

#endif