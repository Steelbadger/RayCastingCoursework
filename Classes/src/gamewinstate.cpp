#include "gamewinstate.h"

#include "texturemanager.h"

#include "PS2Defines.h"
#include "pad.h"
#include "dma.h"

//  Constructor setup the sprites to use
GameWinState::GameWinState():
	youWin(0, -128, 300, 150), 
	returnSprite(0, 128, 160, 80),
	winImage("winImage.bmp"),
	darkenOverlay(0,0,640, 512),
	GameState(GameState::GAMEWIN)
{}

GameWinState::~GameWinState()
//  No cleanup to do
{}

void GameWinState::Initialise()
//  Continue setting up the sprites we'll use and load necessary textures
{
	youWin.SetUVs(0,0,256, 128);
	youWin.SetDepth(901);
	TexManager.LoadTexture(winImage);
	
	returnSprite.SetUVs(0,128,256, 128);
	returnSprite.SetDepth(901);
	
	//  Set to black, semi-transparent and at a depth above anything in the play state
	//  but below everything in the lose state	
	darkenOverlay.SetColour(0x000000);
	darkenOverlay.SetAlpha('l');
	darkenOverlay.SetDepth(900);	
}

void GameWinState::Update()
//  Just wait for player to hit start to continue
{
	if (pad[0].pressed & PAD_START) {
		value = GameState::MENU;
		if (priorState != NULL) {
			priorState->Initialise();
		}		
	}
	
	//  Turn off any residual rumble
	set_actuator(0,0,0);
}

void GameWinState::Render()
//  Render the various assets to the screen
{
	//  First render the paused PlayState
	if (priorState != NULL) {
		priorState->Render();
	}
	
	//  Then render the semi-transparent overlay
	TexManager.UploadTextureToBuffer(winImage, TextureManager::BUFFER1);	
	darkenOverlay.Render();
	
	//  Then render the text
	TexManager.SetTexture(winImage);
	
	youWin.Render();
	returnSprite.Render();
}

void GameWinState::PriorState(GameState* prior)
//  Set the prior state, expects prior to be a PlayState
{
	priorState = prior;
}