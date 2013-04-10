#include "menustate.h"
#include "texturemanager.h"

#include "PS2Defines.h"
#include "pad.h"
#include "dma.h"


MenuState::MenuState() : 
	play(0, 0, 128, 32),
	options(0, 32, 128, 32),
	help(0, 64, 128, 32),
	quit(0, 128, 128, 32),
	background(0, 0, 640, 512),
	cursorPos(PLAY),
	menuImage("menu.bmp"),
	splashImage("splash.bmp")
{}

MenuState::~MenuState()
{}

void MenuState::Initialise()
{
	background.SetUVs(0, 0, 256, 256);
	background.SetDepth(900);
	TexManager.LoadTexture(splashImage);
	
	play.SetUVs(0,0,256,64);
	play.SetDepth(901);
	
	options.SetUVs(0,64,256,64);
	options.SetDepth(901);
	
	help.SetUVs(0,128,256,64);
	help.SetDepth(901);
	
	quit.SetUVs(0, 192, 256, 64);
	quit.SetDepth(901);
	
	TexManager.LoadTexture(menuImage);
}

void MenuState::Update()
{
	
	if (pad[0].pressed & PAD_CROSS) {
		switch (cursorPos) {
			case PLAY: 		value = GameState::GAMEACTIVE;
							break;
			case OPTIONS:	value = GameState::OPTIONS;
							break;
			case HELP:		value = GameState::HELP;
							break;
			case QUIT:		value = GameState::QUIT;
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
	cursorTemp = (cursorTemp > 3 ? 3 : cursorTemp);
	cursorTemp = (cursorTemp < 0 ? 0 : cursorTemp);
	cursorPos = MenuOption(cursorTemp);
	GetOptionOfCursor().UniformScale(1.5);		

}

void MenuState::Render()
{
	TexManager.UploadTextureToBuffer(splashImage, TextureManager::BUFFER1);
	TexManager.UploadTextureToBuffer(menuImage, TextureManager::BUFFER2);
	TexManager.SetTexture(splashImage);
	background.Render();
	TexManager.SetTexture(menuImage);
	play.Render();
	options.Render();
	help.Render();
	quit.Render();	
}


PS2SpriteT& MenuState::GetOptionOfCursor()
{
		switch (cursorPos) {
			case PLAY: 		return play;
			case OPTIONS:	return options;
			case HELP:		return help;
			case QUIT:		return quit;
		}
		
		//  Should Never Happen
		return quit;
}