#include "gamelosestate.h"

#include "texturemanager.h"

#include "PS2Defines.h"
#include "pad.h"
#include "dma.h"

#include <iostream>

GameLoseState::GameLoseState():
	youLose(0, -128, 300, 150), 
	returnSprite(0, 128, 160, 80),
	loseImage("loseImage.bmp"),
	darkenOverlay(0,0,640, 512),
	GameState(GameState::GAMELOSE)
{}

GameLoseState::~GameLoseState()
{}

void GameLoseState::Initialise()
{
	youLose.SetUVs(0,0,256, 128);
	youLose.SetDepth(901);
	TexManager.LoadTexture(loseImage);
	
	returnSprite.SetUVs(0,128,256, 128);
	returnSprite.SetDepth(901);
	
	darkenOverlay.SetColour(0x000000);
	darkenOverlay.SetAlpha('l');
	darkenOverlay.SetDepth(900);
	rumbleIntensity = 127;
}

void GameLoseState::Update()
{
	set_actuator(0,1,127);
	if (rumbleIntensity > 0) {
		rumbleIntensity--;
	} else {
		set_actuator(0,0,0);
	}
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
{
	if (priorState != NULL) {
		priorState->Render();
	}
	TexManager.UploadTextureToBuffer(loseImage, TextureManager::BUFFER1);	
	darkenOverlay.Render();
	
	TexManager.SetTexture(loseImage);
	
	youLose.Render();
	returnSprite.Render();
}

void GameLoseState::PriorState(GameState* prior)
{
	priorState = prior;
}