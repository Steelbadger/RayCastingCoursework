#ifndef _MYPS2APPLICATION_H
#define _MYPS2APPLICATION_H

#include "primitives.h"
#include "texture.h"
#include "raycaster.h"
#include "level.h"
#include "vector2.h"
#include "renderer.h"
#include "mobmanager.h"
#include "mob.h"
#include "animatedsprite.h"
#include "gun.h"


//*** TYPES AND CLASSES
class MyPS2Application
{
public:
	MyPS2Application();
	void Run();
	static void set_quitting(bool quitting) { quitting_ = quitting; }
	static bool quitting() { return quitting_; }
private:
	void Init();
	void CleanUp();
	void Update();
	void Render();
	
	double currentTime;
	double lastTime;
	float timeDiff;
	
	//  game loop boolean
	static bool quitting_;
	
	Level testLevel;
	Renderer testRenderer;
	MobManager testMobs;
	Gun testGun;
	
	
	Vector2 testDirection;
	Vector2 testStrafe;
	Vector2 testPosition;
	
	
};

#endif // _MYPS2APPLICATION_H