#ifndef __PAUSEDSTATE_H__
#define __PAUSEDSTATE_H__
#include "gamestate.h"
#include "primitives.h"
#include "playstate.h"

#include <string>


class PausedState : public GameState
{
public:
	PausedState();
	virtual ~PausedState();
	
	virtual void Initialise();
	virtual void Update();
	virtual void Render();
	virtual void PriorState(GameState* prior);
	
private:
	enum MenuOption {RETURN = 0, OPTIONS = 1, HELP = 2, QUITTOMENU = 3, QUITAPP = 4};
	PS2SpriteT pausedTitle;	
	PS2SpriteT returnToGame;
	PS2SpriteT options;
	PS2SpriteT help;
	PS2SpriteT quitMenu;
	PS2SpriteT quitApp;	
	MenuOption cursorPos;
	
	PS2SpriteT& GetOptionOfCursor();
	float analogueDelay;
	
	std::string pauseImage;
	std::string menuImage;	
	GameState* background;
	PS2Sprite darkenOverlay;
};

#endif