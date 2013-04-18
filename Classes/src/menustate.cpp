#include "menustate.h"
#include "texturemanager.h"

#include "PS2Defines.h"
#include "pad.h"
#include "dma.h"

//  Constructor, create the sprites and setup texture strings
MenuState::MenuState() : 
	play(0, 0, 128, 32),
	options(0, 32, 128, 32),
	help(0, 64, 128, 32),
	quit(0, 128, 128, 32),
	background(0, 0, 640, 512),
	cursorPos(PLAY),
	menuImage("menu.bmp"),
	splashImage("splash.bmp"),
	GameState(GameState::MENU)	
{}

MenuState::~MenuState()
//  Empty
{}

void MenuState::Initialise()
//  Continue setting up the sprites and load the textures
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
//  Check for input and modify menu sprites based on this input
//  to indicate a current cursor selection
{
	//  Press cross to select an option, set the gamestate return value
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
	
	//  Circle to return to startup screen
	if (pad[0].pressed & PAD_CIRCLE) {
		value = GameState::STARTUP;
	}
	
	//  Reset the delay number when the player releases the stick
	//  stops the response from feeling 'sticky' then the player just flicks the stick
	if (pad[0].axes[1] == 0) {
		analogueDelay = 10;
	}

	//  Convert the enum to an int so we can use maths
	int cursorTemp = int(cursorPos);
	
	//  If we hit up then decrement the cursor (and change the size of the current selection
	//  back to normal)
	if (pad[0].pressed & PAD_UP) {
		cursorTemp--;
		GetOptionOfCursor().UniformScale(1.0);
	}
	//  Do same if we hit up on the left analogue stick
	if (pad[0].axes[1] < 0) {
		analogueDelay -=pad[0].axes[1];
		if (analogueDelay > 10) {
			analogueDelay = 0;		
			cursorTemp--;
			GetOptionOfCursor().UniformScale(1.0);
		}
	}	
	
	//  If we hit down then increment the cursor (and change the size of the current selection
	//  back to normal)	
	if (pad[0].pressed & PAD_DOWN) {
		cursorTemp++;
		GetOptionOfCursor().UniformScale(1.0);		
	}
	//  Do same if we hit down on the left analogue stick	
	if (pad[0].axes[1] > 0) {
		analogueDelay += pad[0].axes[1];
		if (analogueDelay > 10) {
			analogueDelay = 0;
			cursorTemp++;
			GetOptionOfCursor().UniformScale(1.0);
		}
	}
	
	//  Make sure we have not gone beyond the bounds of the menu
	cursorTemp = (cursorTemp > 3 ? 3 : cursorTemp);
	cursorTemp = (cursorTemp < 0 ? 0 : cursorTemp);
	
	//  Set the new modified value back into the enumerated type
	cursorPos = MenuOption(cursorTemp);
	
	//  Scale the current selection (larger) to provide visual feedback
	GetOptionOfCursor().UniformScale(1.5);		

}

void MenuState::Render()
//  Draw all the options to the screen
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
//  Check the enumerated type to find the sprite associated with that option
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