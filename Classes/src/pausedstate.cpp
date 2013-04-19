//////////////////////------------------//////////////////////
/*			
						By Ross Davies
															*/
//////////////////////------------------//////////////////////

#include "pausedstate.h"

#include "texturemanager.h"

#include "PS2Defines.h"
#include "pad.h"
#include "dma.h"

#include <iostream>


//  Setup all the sprites in their correct locations
//  and store texture file names
//  Initialise all pointers to NULL
PausedState::PausedState() : 
	pausedTitle(0, -128, 300, 150), 
	returnToGame(0, 0, 128, 32),
	options(0, 32, 128, 32),
	help(0, 64, 128, 32),
	quitMenu(0, 96, 160, 40),
	quitApp(0, 160, 128, 32),
	cursorPos(RETURN),
	pauseImage("pause.bmp"),
	menuImage("menu.bmp"),
	background(NULL),
	darkenOverlay(0,0,640, 512),
	GameState(GameState::GAMEPAUSED)	
{
}

PausedState::~PausedState()
{
	//Nope!
}

void PausedState::Initialise()
//  Continue setting up the sprites (UV and depth)
//  Load in the necessary textures to memory
{
	pausedTitle.SetUVs(0,128,256,128);
	pausedTitle.SetDepth(901);
	
	returnToGame.SetUVs(0,0,256,64);
	returnToGame.SetDepth(901);
	
	options.SetUVs(0,64,256,64);
	options.SetDepth(901);
	
	help.SetUVs(0,128,256,64);
	help.SetDepth(901);
	
	quitMenu.SetUVs(0, 64, 256, 64);
	quitMenu.SetDepth(901);

	quitApp.SetUVs(0, 192, 256, 64);
	quitApp.SetDepth(901);
	
	darkenOverlay.SetColour(0x000000);
	darkenOverlay.SetAlpha('l');
	darkenOverlay.SetDepth(900);
	
	TexManager.LoadTexture(menuImage);
	TexManager.LoadTexture(pauseImage);
}

void PausedState::Update()
//  Wait for input and traverse menu-options as controlled
{
	//  When we hit cross check for the current menu-option we've selected and set
	//  value to transition to that state
	if (pad[0].pressed & PAD_CROSS) {
		switch (cursorPos) {
			case RETURN: 		value = GameState::GAMEACTIVE;
								break;
			case OPTIONS:		value = GameState::OPTIONS;
								break;
			case HELP:			value = GameState::HELP;
								break;
			case QUITTOMENU:	value = GameState::MENU;
								if (background != NULL) {			//  Safeguard, shouldn't be needed
									background->Initialise();		//  If we return to the main menu the level should be reset
								}
								break;
			case QUITAPP:		value = GameState::QUIT;
								break;
		}
	}
	
	//  Circle button ends the paused state
	if (pad[0].pressed & PAD_CIRCLE) {
		value = GameState::GAMEACTIVE;
	}
	
	//  If the analogue stick is centred then the delay is reset, increases responsiveness
	if (pad[0].axes[1] == 0) {
		analogueDelay = 10;
	}
	
	//  No rumble in pause
	set_actuator(0,0,0);

	
	//  Convert cursor enum to int for increment and decrement integer ops
	int cursorTemp = int(cursorPos);
	
	//  Add or subtract to the temporary integer cursor when up/down is hit
	//  If we hit any of these scale the option sprite where we used to be back to normal size
	if (pad[0].pressed & PAD_UP) {
		cursorTemp--;
		GetOptionOfCursor().UniformScale(1.0);
	}
	if (pad[0].axes[1] < 0) {
		analogueDelay -=pad[0].axes[1];
		if (analogueDelay > 10) {
			analogueDelay = 0;		
			cursorTemp--;
			GetOptionOfCursor().UniformScale(1.0);
		}
	}

	if (pad[0].pressed & PAD_DOWN) {
		cursorTemp++;
		GetOptionOfCursor().UniformScale(1.0);		
	}
	
	if (pad[0].axes[1] > 0) {
		analogueDelay += pad[0].axes[1];
		if (analogueDelay > 10) {
			analogueDelay = 0;
			cursorTemp++;
			GetOptionOfCursor().UniformScale(1.0);
		}
	}
	
	//  Clamp the option to the range of available enums
	cursorTemp = (cursorTemp > 4 ? 4 : cursorTemp);
	cursorTemp = (cursorTemp < 0 ? 0 : cursorTemp);
	
	//  Convert back
	cursorPos = MenuOption(cursorTemp);
	
	//  Scale our new option up a bit to make it obvious
	GetOptionOfCursor().UniformScale(1.5);	
}

void PausedState::Render()
//  Draw the background (playstate expected), darken it, then draw the pause
//  menu options on top
{
	if (background != NULL) {
		background->Render();
	}
	darkenOverlay.Render();
	
	TexManager.UploadTextureToBuffer(pauseImage, TextureManager::BUFFER1);
	TexManager.UploadTextureToBuffer(menuImage, TextureManager::BUFFER2);
	TexManager.SetTexture(pauseImage);
	
	pausedTitle.Render();
	returnToGame.Render();
	quitMenu.Render();	

	TexManager.SetTexture(menuImage);	
	options.Render();
	help.Render();
	quitApp.Render();
}

void PausedState::PriorState(GameState* prior)
//  Set the prior state, this should ALWAYS be a GAMEACTIVE state
{
	background = prior;
}

PS2SpriteT& PausedState::GetOptionOfCursor()
//  Find the sprite corresponding to the option of the sprite
{
	switch (cursorPos) {
		case RETURN: 		return returnToGame;
		case OPTIONS:		return options;
		case HELP:			return help;
		case QUITTOMENU:	return quitMenu;
		case QUITAPP:		return quitApp;
	}
	
	//  Should Never Happen
	std::cout << "Unrecognised cursor position, quitting app" << std::endl;
	return quitApp;
}