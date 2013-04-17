#include "startupstate.h"
#include "texturemanager.h"
#include "PS2Defines.h"
#include "pad.h"
#include "dma.h"


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

}

void StartupState::Initialise()
{
	splash.SetUVs(0,0,256,256);
	splash.SetDepth(900);
	std::cout << "Load splash.bmp" << std::endl;
	TexManager.LoadTexture(splashImage);
	std::cout << "Done splash.bmp" << std::endl;	
	loadingStart.SetUVs(0,0,256,64);
	loadingStart.SetDepth(901);
	titleText.SetUVs(0, 128, 256, 128);
	titleText.SetDepth(902);
	std::cout << "Load loadingstart.bmp" << std::endl;	
	TexManager.LoadTexture(textImage);
	std::cout << "Done loadingstart.bmp" << std::endl;		

	TexManager.UploadTextureToBuffer(splashImage, TextureManager::BUFFER1);
	TexManager.UploadTextureToBuffer(textImage, TextureManager::BUFFER2);
	InitRender();
}

void StartupState::Update()
{
	loadingStart.SetUVs(0, 64, 256, 64);
	if (pad[0].pressed & PAD_START) {
		value = GameState::MENU;
	}
}


void StartupState::Render()
{
	TexManager.SetTexture(splashImage);
	splash.Render();
	TexManager.SetTexture(textImage);
	titleText.Render();
	loadingStart.Render();
}

void StartupState::InitRender()
{
	SPS2Manager.BeginScene();
	Render();
	SPS2Manager.EndScene();			
}
