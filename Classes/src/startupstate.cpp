#include "startupstate.h"
#include "texturemanager.h"
#include "PS2Defines.h"
#include "pad.h"
#include "dma.h"

//  Constructor, Create our sprites and setup texture file paths
StartupState::StartupState() :
	splash(0, 0, 640, 512),
	loadingStart(0, 150, 160, 40),
	titleText(0, 64, 300, 150),
	splashImage("splash.bmp"),
	textImage("loadingstart.bmp"),
	GameState(GameState::STARTUP)
{
}


StartupState::~StartupState()
{
	//  Nothing to do
}

void StartupState::Initialise()
//  Continue setting up the sprites and load the textures
{
	splash.SetUVs(0,0,256,256);
	splash.SetDepth(900);
	TexManager.LoadTexture(splashImage);
	loadingStart.SetUVs(0,0,256,64);
	loadingStart.SetDepth(901);
	titleText.SetUVs(0, 128, 256, 128);
	titleText.SetDepth(902);
	TexManager.LoadTexture(textImage);

	//  Render loading to screen
	InitRender();
}

void StartupState::Update()
//  Once we get into here we're done loading so set to
//  press start texture instead and wait for input
{
	loadingStart.SetUVs(0, 64, 256, 64);
	if (pad[0].pressed & PAD_START) {
		value = GameState::MENU;
	}
}


void StartupState::Render()
//  Draw the splash screen and text to the screen
{
	TexManager.UploadTextureToBuffer(splashImage, TextureManager::BUFFER1);
	TexManager.UploadTextureToBuffer(textImage, TextureManager::BUFFER2);
	TexManager.SetTexture(splashImage);
	splash.Render();
	TexManager.SetTexture(textImage);
	titleText.Render();
	loadingStart.Render();
}

void StartupState::InitRender()
//  Do a standalone render on startup
{
	SPS2Manager.BeginScene();
	Render();
	SPS2Manager.EndScene();			
}
