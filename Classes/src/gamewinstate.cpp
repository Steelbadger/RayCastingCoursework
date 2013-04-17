#include "gamewinstate.h"

#include "texturemanager.h"

#include "PS2Defines.h"
#include "pad.h"
#include "dma.h"

GameWinState::GameWinState():
	youWin(0, -128, 300, 150), 
	returnSprite(0, 128, 160, 80),
	winImage("winImage.bmp"),
	darkenOverlay(0,0,640, 512),
	GameState(GameState::GAMEWIN)
{}

GameWinState::~GameWinState()
{}

void GameWinState::Initialise()
{
	youWin.SetUVs(0,0,256, 128);
	youWin.SetDepth(901);
	TexManager.LoadTexture(winImage);
	
	returnSprite.SetUVs(0,128,256, 128);
	returnSprite.SetDepth(901);
	TexManager.LoadTexture(winImage);	
	
	darkenOverlay.SetColour(0x000000);
//	darkenOverlay.SetAlpha('w');
	darkenOverlay.SetAlpha('l');
	darkenOverlay.SetDepth(900);	
}

void GameWinState::Update()
{
	if (pad[0].pressed & PAD_START) {
		value = GameState::MENU;
		if (priorState != NULL) {
			priorState->Initialise();
		}		
	}
	set_actuator(0,0,0);
}

void GameWinState::Render()
{
	if (priorState != NULL) {
		priorState->Render();
	}
	TexManager.UploadTextureToBuffer(winImage, TextureManager::BUFFER1);	
	darkenOverlay.Render();
	
	TexManager.SetTexture(winImage);
	
	youWin.Render();
	returnSprite.Render();
}

void GameWinState::PriorState(GameState* prior)
{
	priorState = prior;
}