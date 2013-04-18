#ifndef __PAUSEDSTATE_H__
#define __PAUSEDSTATE_H__
//////////////////////--By Ross Davies--//////////////////////
/*		state object that is called when the player
		pauses the game, allows the player to quit,
		return to the main menu (losing progress),
		change options, look at the help screen or
		return to the game where they left off.  This
		state also draws the current PlayState in 
		the background										*/
//////////////////////////////////////////////////////////////

#include "gamestate.h"
#include "primitives.h"
#include "playstate.h"

#include <string>


class PausedState : public GameState
{
public:
	//  Constructor
	PausedState();
	
	//  Virtual Destructor for inheritance
	virtual ~PausedState();
	
	//  Functions for hooking into virtuals on parent
	//  Initialise loads necessary assets	
	virtual void Initialise();
	
	//  Update waits for input and updates the menu sprites	
	virtual void Update();
	
	//  Draw the background and menu sprites to the screen	
	virtual void Render();
	
	//  Expects to be passed the PlayState so that it may
	//  be draw in the background
	virtual void PriorState(GameState* prior);
	
private:
	//  All possible menu options, explicitly enumerated so we may use
	//  integer maths for cursor movement
	enum MenuOption {RETURN = 0, OPTIONS = 1, HELP = 2, QUITTOMENU = 3, QUITAPP = 4};
	
	//  Sprites to hold the menuoptions
	PS2SpriteT pausedTitle;	
	PS2SpriteT returnToGame;
	PS2SpriteT options;
	PS2SpriteT help;
	PS2SpriteT quitMenu;
	PS2SpriteT quitApp;	
	
	//  Current menu option indicated by the player
	MenuOption cursorPos;
	
	//  Convert from the cursorPos MenuOption enum to the corresponding sprite
	PS2SpriteT& GetOptionOfCursor();
	
	//  delay for slowing down analogue input
	float analogueDelay;
	
	//  image textures for applying to sprites
	std::string pauseImage;
	std::string menuImage;	
	
	//  pointer to the play state for drawing in background
	GameState* background;
	
	//  full screen semi-transparent quad for fading the playstate out
	PS2Sprite darkenOverlay;
};

#endif