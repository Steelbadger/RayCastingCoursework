#ifndef __MENUSTATE_H__
#define __MENUSTATE_H__
//////////////////////--By Ross Davies--//////////////////////
/*		State object that is called while the player 
		is in the main menu, renders the menu to 
		the screen, checks for input and updates the 
		menu sprites to indicate current selection			*/
//////////////////////////////////////////////////////////////

#include "gamestate.h"
#include "primitives.h"

#include <string>

class MenuState : public GameState
{
public:
	//  Constructor
	MenuState();
	
	//  Virtual Destructor for inheritance
	virtual ~MenuState();
	
	//  Functions for hooking into virtuals on parent
	//  Initialise loads necessary assets
	virtual void Initialise();
	
	//  Update waits for input and updates various sprites
	//  based on current selections
	virtual void Update();
	
	//  Draw the background and menu sprites to the screen
	virtual void Render();	

private:
	//  All possible player selections within the menu
	enum MenuOption {PLAY = 0, OPTIONS = 1, HELP = 2, QUIT = 3};
	
	//  Sprites for drawing menu options textures
	PS2SpriteT play;
	PS2SpriteT options;
	PS2SpriteT help;
	PS2SpriteT quit;
	PS2SpriteT background;
	
	//  The current menu option selected
	MenuOption cursorPos;
	
	//  Use the cursorPos to find the sprite indicated
	PS2SpriteT& GetOptionOfCursor();
	
	//  counter variable for delaying analogue input
	//  this makes the analogue sticks usable in the menu
	//  (too fast otherwise)
	float analogueDelay;
	
	//  textures for the menu options and background
	std::string menuImage;
	std::string splashImage;
};

#endif