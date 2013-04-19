//////////////////////------------------//////////////////////
/*			
						By Ross Davies
															*/
//////////////////////------------------//////////////////////

#include "playstate.h"

#include "PS2Defines.h"
#include "pad.h"
#include "audiomanager.h"

#include <time.h>
#include <iostream>

//  Set up the assets
PlayState::PlayState():
	GameState(GameState::GAMEACTIVE),
	redOverlay(0,0,640, 512),
	ambientMusic("Ambience")
{}

PlayState::~PlayState()
{}


void PlayState::Initialise()
//  Load level from file, send data where it's needed, initialise the renderer
//  Load up the mob list, and load the backing music to memory
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
	
	//  Set the flash-red damage overlay
	redAlpha = 0x00;
	redOverlay.SetColour(0xFF0000);
	redOverlay.SetAlpha(redAlpha);
	redOverlay.SetDepth(900);
	Audio.Load(ambientMusic, AudioManager::DSP0);
}

void PlayState::Update()
//  Count time differentials then call updates for player, mobs and renderer
{
	//  Find the time since the last frame
	float timeDif = timer.GetTimeDeltaSeconds();
	if (timeDif > 0.1f) {
		timeDif = 0.0f;
	}
	
	//  Debug output FPS to console out
	if (pad[0].pressed & PAD_CROSS) {
		std::cout << "TimeDiff: " << timeDif << std::endl;
		std::cout << "FoV: " << testRenderer.GetFoV() << std::endl;
		std::cout << "FPS: " << (1.0f/timeDif) << std::endl; 
	}		

	//  Check for game pause
	if (pad[0].pressed & PAD_START) {
		value = GameState::GAMEPAUSED;
		EndMusic();
	}	
	
	//  Update player, renderer and mobs
	player.Update(timeDif);
	testRenderer.SetPosition(player.GetPosition());
	testRenderer.SetDirection(player.GetDirection());
	testRenderer.ConstructDepthMap();	
	testRenderer.BuildScene();
	testMobs.FindMobPositions(player.GetPosition(), player.GetDirection(), testRenderer.GetFoV());
	
	//  Check for damage dealt by player
	if (player.HasFired()) {
		testMobs.ShootMobs(testRenderer.GetCentreWallDistance(), player.GetGunMaxDamage());
	}
	
	testMobs.UpdateMobs(testLevel, timeDif);
	
	//  Check for damage dealt by mobs, flash red on damage
	int damage = testMobs.CheckMobDealtDamage();
	if (redAlpha > 5) {
		redAlpha-=5;
	}
	if (damage > 0) {
		player.Damage(damage);
		redAlpha = 'a';
	}
	redOverlay.SetAlpha(redAlpha);
	
	//  Check for end conditions (win or lose)
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
//  Tell player, renderer and mobs to draw the scene
{
	testRenderer.DrawScene();
	testMobs.DrawMobs();
	player.Render();
	redOverlay.Render();
}

void PlayState::StartPlayMusic()
//  Play the ambient loop
{
	Audio.PlayLoop(ambientMusic);
}

void PlayState::EndMusic()
//  Stop playing the ambient loop
{
	Audio.Stop(ambientMusic);
}

void PlayState::PassOptions(Options options)
//  Receive options from the options state and set the values
//  that need to be set
{
	//  Set sensitivty to selected level
	float sens = float(options.sensitivity)*2.0f + 2.0f;
	player.SetSensitivity(sens);
	
	//  Enable/disable rumble
	if (options.rumble == Options::RUMBLEON) {
		enable_actuator(0,1,1);
	} else {
		enable_actuator(0,0,0);
	}
	
	//  Set mob HP, damage and speed based on difficulty
	int hp = int(options.difficulty)*20 + 30;
	int dmg = int(options.difficulty)*5 + 10;
	float speed = float(options.difficulty)*0.3f + 1.0f;
	testMobs.SetMobHitpoints(hp);
	testMobs.SetMobDamage(dmg);
	testMobs.SetMobSpeed(speed);
}



