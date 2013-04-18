#include "helpstate.h"

#include "texturemanager.h"

#include "PS2Defines.h"
#include "pad.h"
#include "dma.h"

#include <iostream>

//  Constructor, create the sprites and get texture file
HelpState::HelpState() :
	helpScreen(0,0,640,512),
	helpImage("help.bmp"),
	GameState(GameState::HELP)	
{}

HelpState::~HelpState()
{}

void HelpState::Initialise()
//  continue seting up the sprite and load the texture
{
	helpScreen.SetUVs(0,0,256, 256);
	helpScreen.SetDepth(901);
	TexManager.LoadTexture(helpImage);	
}

void HelpState::Update()
//  Wait for player to hit circle to return to prior state
{
	if (pad[0].pressed & PAD_CIRCLE) {
	
		value = priorState->GetState();
	}
}

void HelpState::Render()
//  Render the big help splash screen
{
	TexManager.UploadTextureToBuffer(helpImage, TextureManager::BUFFER1);
	TexManager.SetTexture(helpImage);
	helpScreen.Render();
}

void HelpState::PriorState(GameState* state)
//  Pass the prior state so that we may return to it when we leave
{
	priorState = state;
}