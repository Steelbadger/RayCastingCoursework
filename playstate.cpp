#include "playstate.h"

#include "PS2Defines.h"
#include "pad.h"

#include <time.h>
#include <iostream>


PlayState::PlayState():
	GameState(GameState::GAMEACTIVE)
{}

PlayState::~PlayState()
{}


void PlayState::Initialise()
{
	player.Initialise(Vector2(2.1f, 2.1f), Vector2(1.0f, 1.0f), testLevel);
	testRenderer.SetLevel(&testLevel);
	testRenderer.SetPosition(player.GetPosition());
	testRenderer.SetDirection(player.GetDirection());
	testRenderer.SetFoV(60.0f);
	testRenderer.InitTextures();
	testMobs.ClearMobs();

	Mob testMob(Vector2(3.0f, 3.0f), Vector2(1.4f, 1.4f), "testEnemy.bmp", Vector2(0,0), Vector2(64, 64));
	testMobs.AddMob(testMob);
	testMob = Mob(Vector2(2.0f, 3.0f), Vector2(1.5f, 1.5f), "testEnemy.bmp", Vector2(0,0), Vector2(64, 64));
	testMobs.AddMob(testMob);
	testMob = Mob(Vector2(18.0f, 2.0f), Vector2(1.6f, 1.6f), "testEnemy.bmp", Vector2(0,0), Vector2(64, 64));
	testMobs.AddMob(testMob);
	timer = clock();
	timeDif = clock()-timer;
}

void PlayState::Update()
{
	timeDif = clock() - timer;
	timer = clock();
	timeDif /= CLOCKS_PER_SEC;

	if (pad[0].pressed & PAD_CROSS) {
		//testRenderer.OutputDepthMap();
		std::cout << "TimeDiff: " << timeDif << std::endl;
		std::cout << "FoV: " << testRenderer.GetFoV() << std::endl;
	}		

	if (pad[0].pressed & PAD_START) {
		value = GameState::GAMEPAUSED;
	}	
	
	player.Update(timeDif);
	
	testRenderer.ConstructDepthMap();	
	testRenderer.SetPosition(player.GetPosition());
	testRenderer.SetDirection(player.GetDirection());	
	testRenderer.BuildScene();
	testMobs.FindMobPositions(player.GetPosition(), player.GetDirection(), testRenderer.GetFoV());
	if (player.HasFired()) {
		testMobs.ShootMobs(testRenderer.GetCentreWallDistance(), player.GetGunMaxDamage());
	}
		
}

void PlayState::Render()
{
	testRenderer.DrawScene();
	testMobs.DrawMobs();
	player.Render();
}

