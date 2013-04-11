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
	helpScreen.SetUVs(
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