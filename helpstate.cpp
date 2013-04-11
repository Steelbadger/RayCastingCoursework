#include "helpstate.h"

#include "texturemanager.h"

#include "PS2Defines.h"
#include "pad.h"
#include "dma.h"

#include <iostream>

HelpState::HelpState() :
	helpScreen(0,0,640,512),
	helpImage("help.bmp")
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

}

void HelpState::Render()
{

}

void HelpState::PriorState(GameState* state)
{
	priorState = state;
}