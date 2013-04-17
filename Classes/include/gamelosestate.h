#ifndef __GAMELOSESTATE_H__
#define __GAMELOSESTATE_H__
//////////////////////--By Ross Davies--//////////////////////
/*		State object that is called when the player dies
		renders a splash screen and waits for the player
		to hit start										*/
//////////////////////////////////////////////////////////////

#include "gamestate.h"
#include "primitives.h"

#include <string>


class GameLoseState : public GameState
{
public:
	//  Constructor
	GameLoseState();
	
	//  Destructor
	virtual ~GameLoseState();
	
	//  Functions for hooking into virtuals on parent
	//  Initialise loads necessary assets
	virtual void Initialise();
	
	//  Update waits for input
	virtual void Update();
	
	//  Render draws assets to the screen (including
	//  a darkened image of the last thing seen in 
	//  the play state
	virtual void Render();
	
	//  expects a pointer to the play state, this 
	//  allows for drawing the faded image of the world
	virtual void PriorState(GameState* priorState);
	
private:

	//  Play state for rendering
	GameState* priorState;
	
	//  Sprites to hold the text textures
	PS2SpriteT youLose;
	PS2SpriteT returnSprite;		
	
	//  Only one texture file needed
	std::string loseImage;
	
	//  A screen-size semi-transparent quad
	//  to darken to screen
	PS2Sprite darkenOverlay;
	
	//  The intensity of the conroller rumble
	//  When you die a bigger rumble happens
	char rumbleIntensity;
};


#endif