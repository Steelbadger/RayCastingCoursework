#ifndef __MENUSTATE_H__
#define __MENUSTATE_H__
#include "gamestate.h"

#include <string>

class MenuState : public GameState
{
public:
	MenuState();
	virtual ~MenuState();
	
	virtual void Initialise();
	virtual void Update();
	virtual void Render();	

private:
	enum MenuOption {PLAY = 0, OPTIONS = 1, HELP = 2, QUIT = 3};
	PS2SpriteT play;
	PS2SpriteT options;
	PS2SpriteT help;
	PS2SpriteT quit;
	PS2SpriteT background;
	MenuOption cursorPos;
	
	PS2SpriteT& GetOptionOfCursor();
	
	std::string menuImage;
	std::string splashImage;
};

#endif