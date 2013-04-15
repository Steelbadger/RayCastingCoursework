#ifndef __GAMEWINSTATE_H__
#define __GAMEWINSTATE_H__

#include "gamestate.h"
#include "primitives.h"

#include <string>


class GameWinState : public GameState
{
public:
	GameWinState();
	virtual ~GameWinState();
	
	virtual void Initialise();
	virtual void Update();
	virtual void Render();
	virtual void PriorState(GameState* priorState);
	
private:
	GameState* priorState;
	
	PS2SpriteT youWin;
	PS2SpriteT returnSprite;		
	
	std::string winImage;
	PS2Sprite darkenOverlay;	
};


#endif