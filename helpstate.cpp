#include "helpstate.h"

#include "texturemanager.h"

#include "PS2Defines.h"
#include "pad.h"
#include "dma.h"

#include <iostream>

HelpState::HelpState() :
	helpScreen(0,0,640,512),
	helpImage("help.bmp"),
	GameState(GameState::HELP)	
{}

HelpState::~HelpState()
{}

void HelpState::Initialise()
{
	helpScreen.SetUVs(0,0,256, 256);
	helpScreen.SetDepth(901);
	TexManager.LoadTexture(helpImage);	
}

void HelpState::Update()
{
	if (pad[0].pressed & PAD_CIRCLE) {
		value = priorState->GetState();
	}
}

void HelpState::Render()
{
	TexManager.UploadTextureToBuffer(helpImage, TextureManager::BUFFER1);
	TexManager.SetTexture(helpImage);
	helpScreen.Render();
}

void HelpState::PriorState(GameState* state)
{
	priorState = state;
}