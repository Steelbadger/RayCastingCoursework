#include "pausedstate.h"

#include "texturemanager.h"

#include "PS2Defines.h"
#include "pad.h"
#include "dma.h"

#include <iostream>



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
	darkenOverlay(0,0,640, 512)
{
}

PausedState::~PausedState()
{}

void PausedState::Initialise()
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
	darkenOverlay.SetAlpha('w');
	darkenOverlay.SetDepth(900);
	
	TexManager.LoadTexture(menuImage);
	TexManager.LoadTexture(pauseImage);
}

void PausedState::Update()
{
	if (pad[0].pressed & PAD_CROSS) {
		switch (cursorPos) {
			case RETURN: 		value = GameState::GAMEACTIVE;
								break;
			case OPTIONS:		value = GameState::OPTIONSPAUSED;
								break;
			case HELP:			value = GameState::HELPPAUSED;
								break;
			case QUITTOMENU:	value = GameState::MENU;
								if (background != NULL) {
									background->Initialise();
								}
								break;
			case QUITAPP:		value = GameState::QUIT;
								break;
		}
	}
	if (pad[0].axes[1] == 0) {
		analogueDelay = 10;
	}

	int cursorTemp = int(cursorPos);
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
	cursorTemp = (cursorTemp > 4 ? 4 : cursorTemp);
	cursorTemp = (cursorTemp < 0 ? 0 : cursorTemp);
	cursorPos = MenuOption(cursorTemp);
	GetOptionOfCursor().UniformScale(1.5);	
}

void PausedState::Render()
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
{
	background = prior;
}

PS2SpriteT& PausedState::GetOptionOfCursor()
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