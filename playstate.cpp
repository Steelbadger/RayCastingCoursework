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
	testLevel.LoadLevel("levelone.level");
	
	player.Initialise(testLevel.GetPlayerStartPosition(), Vector2(1.0f, 1.0f), testLevel);
	testRenderer.SetLevel(&testLevel);
	testRenderer.SetPosition(player.GetPosition());
	testRenderer.SetDirection(player.GetDirection());
	testRenderer.SetFoV(60.0f);
	testRenderer.InitTextures();
	testMobs.ClearMobs();
	testMobs.LoadMobList(testLevel.GetMobs());
	timer.Initialise();
}

void PlayState::Update()
{
	float timeDif = timer.GetTimeDeltaSeconds();
	
	if (pad[0].pressed & PAD_CROSS) {
		//testRenderer.OutputDepthMap();
		std::cout << "TimeDiff: " << timeDif << std::endl;
		std::cout << "FoV: " << testRenderer.GetFoV() << std::endl;
		std::cout << "FPS: " << (1.0f/timeDif) << std::endl; 
	}		

	if (pad[0].pressed & PAD_START) {
		value = GameState::GAMEPAUSED;
	}	
//		std::cout << "PlayerUpdate Begin" << std::endl;
	player.Update(timeDif);
//		std::cout << "PlayerUpdate END" << std::endl;
	testRenderer.SetPosition(player.GetPosition());
	testRenderer.SetDirection(player.GetDirection());
//		std::cout << "Construct DepthMap Begin" << std::endl;
	testRenderer.ConstructDepthMap();	
//		std::cout << "Construct DepthMap END" << std::endl;	
//		std::cout << "BuildScene Begin" << std::endl;			
	testRenderer.BuildScene();
//		std::cout << "BuildScene END" << std::endl;			
	testMobs.FindMobPositions(player.GetPosition(), player.GetDirection(), testRenderer.GetFoV());
	if (player.HasFired()) {
		testMobs.ShootMobs(testRenderer.GetCentreWallDistance(), player.GetGunMaxDamage());
	}
	
	if (testLevel.IsCompletion(player.GetPosition())) {
		value = GameState::GAMEWIN;
	}

}

void PlayState::Render()
{
	testRenderer.DrawScene();
	testMobs.DrawMobs();
	player.Render();
}

