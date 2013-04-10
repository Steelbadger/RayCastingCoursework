#include "startupstate.h"
#include "texturemanager.h"


StartupState::StartupState() :
	splash(-320, -256, 640, 512),
	loadingStart(0, 128, 100, 25),
	titleText(0, 0, 200, 50),
	splashImage("splash.bmp"),
	textImage("loadingStart.bmp")
{
}


StartupState::~StartupState()
{

}

void StartupState::Initialise()
{
	splash.SetUVs(0,0,256,256);
	splash.SetDepth(900);
	TexManager.LoadTexture(splashImage);
	loadingStart.SetUVs(0,0,256,64);
	loadingStart.SetDepth(901);
	titleText.SetUVs(0, 128, 256, 128);
	titleText.SetDepth(902);
	TexManager.LoadTexture(loadingStartImage);

	TexManager.UploadTextureToBuffer(splashImage, TextureManager::BUFFER1);
	TexManager.UploadTextureToBuffer(textImage, TextureManager::BUFFER2);
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
