#ifndef __PLAYSTATE_H__
#define __PLAYSTATE_H__

#include "gamestate.h"
#include "level.h"
#include "renderer.h"
#include "mobmanager.h"
#include "player.h"
#include "timer.h"
#include "primitives.h"
#include "options.h"


#include "vector2.h"
#include <string>

class PlayState : public GameState
{
public:
	PlayState();
	virtual ~PlayState();
	
	virtual void Initialise();
	virtual void Update();
	virtual void Render();
	void PassOptions(Options opt);
	void StartPlayMusic();
	void EndMusic();
	
private:
	Level testLevel;
	PS2Sprite redOverlay;
	char redAlpha;
	Renderer testRenderer;
	MobManager testMobs;
	Player player;
	CTimer timer;
	std::string ambientMusic;
};

#endif