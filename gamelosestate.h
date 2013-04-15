#ifndef __GAMELOSESTATE_H__
#define __GAMELOSESTATE_H__

#include "gamestate.h"
#include "primitives.h"

#include <string>


class GameLoseState : public GameState
{
public:
	GameLoseState();
	virtual ~GameLoseState();
	
	virtual void Initialise();
	virtual void Update();
	virtual void Render();
	virtual void PriorState(GameState* priorState);
	
private:
	GameState* priorState;
	
	PS2SpriteT youLose;
	PS2SpriteT returnSprite;		
	
	std::string loseImage;
	PS2Sprite darkenOverlay;	
};


#endif