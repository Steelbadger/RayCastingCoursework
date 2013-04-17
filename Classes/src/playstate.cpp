#include "playstate.h"

#include "PS2Defines.h"
#include "pad.h"
#include "audiomanager.h"

#include <time.h>
#include <iostream>


PlayState::PlayState():
	GameState(GameState::GAMEACTIVE),
	redOverlay(0,0,640, 512),
	ambientMusic("Ambience")
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
	redAlpha = 0x00;
	redOverlay.SetColour(0xFF0000);
	redOverlay.SetAlpha(redAlpha);
	redOverlay.SetDepth(900);
	Audio.Load(ambientMusic, AudioManager::DSP0);
}

void PlayState::Update()
{
	float timeDif = timer.GetTimeDeltaSeconds();
	if (timeDif > 0.1f) {
		timeDif = 0.0f;
	}
	
	if (pad[0].pressed & PAD_CROSS) {
		//testRenderer.OutputDepthMap();
		std::cout << "TimeDiff: " << timeDif << std::endl;
		std::cout << "FoV: " << testRenderer.GetFoV() << std::endl;
		std::cout << "FPS: " << (1.0f/timeDif) << std::endl; 
	}		

	if (pad[0].pressed & PAD_START) {
		value = GameState::GAMEPAUSED;
		EndMusic();
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
	
	testMobs.UpdateMobs(testLevel, timeDif);
	
	int damage = testMobs.CheckMobDealtDamage();
	if (redAlpha > 5) {
		redAlpha-=5;
	}
	if (damage > 0) {
		player.Damage(damage);
		redAlpha = 'a';
	}
	redOverlay.SetAlpha(redAlpha);
	
	if (testLevel.IsCompletion(player.GetPosition())) {
		value = GameState::GAMEWIN;
		EndMusic();
	}
	if (player.IsDead()) {
		value = GameState::GAMELOSE;
		EndMusic();
	}

}

void PlayState::Render()
{
	testRenderer.DrawScene();
	testMobs.DrawMobs();
	player.Render();
	redOverlay.Render();
}

void PlayState::StartPlayMusic()
{
	Audio.PlayLoop(ambientMusic);
}

void PlayState::EndMusic()
{
	Audio.Stop(ambientMusic);
}

void PlayState::PassOptions(Options options)
{
	float sens = float(options.sensitivity)*2.0f + 2.0f;
	player.SetSensitivity(sens);
	
	if (options.rumble == Options::RUMBLEON) {
		enable_actuator(0,1,1);
	} else {
		enable_actuator(0,0,0);
	}
	
	int hp = int(options.difficulty)*20 + 30;
	int dmg = int(options.difficulty)*5 + 10;
	float speed = float(options.difficulty)*0.3f + 1.0f;
	testMobs.SetMobHitpoints(hp);
	testMobs.SetMobDamage(dmg);
	testMobs.SetMobSpeed(speed);
}



