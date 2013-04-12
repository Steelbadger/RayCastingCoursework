#ifndef __PLAYSTATE_H__
#define __PLAYSTATE_H__

#include "gamestate.h"
#include "level.h"
#include "renderer.h"
#include "mobmanager.h"
#include "player.h"

#include "vector2.h"

class PlayState : public GameState
{
public:
	PlayState();
	virtual ~PlayState();
	
	virtual void Initialise();
	virtual void Update();
	virtual void Render();
	
private:
	Level testLevel;
	Renderer testRenderer;
	MobManager testMobs;
	Player player;
	
	double timer;
	double timeDif;
};

#endif