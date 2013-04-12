#ifndef __HELPSTATE_H__
#define __HELPSTATE_H__

#include "gamestate.h"
#include "primitives.h"

#include <string>

class HelpState : public GameState
{
public:
	HelpState();
	virtual ~HelpState();
	
	virtual void Initialise();
	virtual void Update();
	virtual void Render();
	virtual void PriorState(GameState* priorState);
	
private:
	GameState* priorState;
	
	PS2SpriteT helpScreen;	
	
	std::string helpImage;
};

#endif