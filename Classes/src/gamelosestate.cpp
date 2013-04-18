#include "gamelosestate.h"

#include "texturemanager.h"

#include "PS2Defines.h"
#include "pad.h"
#include "dma.h"

#include <iostream>

//  Constructor, setup the sprites we'll use
GameLoseState::GameLoseState():
	youLose(0, -128, 300, 150), 
	returnSprite(0, 128, 160, 80),
	loseImage("loseImage.bmp"),
	darkenOverlay(0,0,640, 512),
	GameState(GameState::GAMELOSE)
{}

GameLoseState::~GameLoseState()
//  No cleanup to do
{}

void GameLoseState::Initialise()
//  Continue setting up the sprites we'll use and load necessary textures
{
	youLose.SetUVs(0,0,256, 128);
	youLose.SetDepth(901);
	TexManager.LoadTexture(loseImage);
	
	returnSprite.SetUVs(0,128,256, 128);
	returnSprite.SetDepth(901);
	
	//  Set to black, semi-transparent and at a depth above anything in the play state
	//  but below everything in the lose state
	darkenOverlay.SetColour(0x000000);
	darkenOverlay.SetAlpha('l');
	darkenOverlay.SetDepth(900);
	rumbleIntensity = 127;
}

void GameLoseState::Update()
//  Rumble to indicate death for a while then just wait for the player
//  to return to menu
{
	//  Turn on actuator, set to high intensity
	set_actuator(0,1,127);
	
	//  count for a while, once count is over turn off rumble
	if (rumbleIntensity > 0) {
		rumbleIntensity--;
	} else {
		set_actuator(0,0,0);
	}
	
	//  Wait for player to hit start to quit
	//  If player does hit start turn off rumble and reset rumble timer
	//  Call initialise on the prior state (always playstate) so that the level
	//  is reset for next use
	if (pad[0].pressed & PAD_START) {
		value = GameState::MENU;
		if (priorState != NULL) {
			priorState->Initialise();
		}
		rumbleIntensity = 0xFF;
		set_actuator(0,0,0);		
	}

}

void GameLoseState::Render()
//  Render the various assets to the screen
{
	//  First render the paused PlayState
	if (priorState != NULL) {
		priorState->Render();
	}
	
	//  Then render the darken semi-transparent overlay
	TexManager.UploadTextureToBuffer(loseImage, TextureManager::BUFFER1);	
	darkenOverlay.Render();
	
	//  Then render the text
	TexManager.SetTexture(loseImage);
	youLose.Render();
	returnSprite.Render();
}

void GameLoseState::PriorState(GameState* prior)
//  Set the prior state (expects prior to be a playstate)
{
	priorState = prior;
}