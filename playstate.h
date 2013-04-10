#ifndef __PLAYSTATE_H__
#define __PLAYSTATE_H__

#include "level.h"
#include "renderer.h"
#include "mobmanager.h"
#include "gun.h"

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
	Gun testGun;
	
	
	Vector2 testDirection;
	Vector2 testStrafe;
	Vector2 testPosition;	

};

#endif