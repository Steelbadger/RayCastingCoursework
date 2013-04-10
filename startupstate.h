#ifndef __STARTUPSTATE_H__
#define __STARTUPSTATE_H__

#include "gamestate.h"
#include "primitives.h"

#include <string>

class StartupState : public GameState
{
public:

	StartupState();
	virtual ~StartupState();
	
	virtual void Initialise();
	virtual void Update();
	virtual void Render();

private:
	void InitRender();
	
	PS2SpriteT splash;
	PS2SpriteT loadingStart;
	PS2SpriteT titleText;
	
	std::string splashImage;
	std::string textImage;	
};



#endif