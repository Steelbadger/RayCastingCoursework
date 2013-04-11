#include "gun.h"
#include "texturemanager.h"

Gun::Gun() :
	textureFile("gunSheetalt.bmp"),
	sprite(0, 156, 100, 200, 0)
{
}

Gun::~Gun()
{}


void Gun::Init()
{
	sprite.SetNumberOfFrames(4);
	sprite.SetAnimationRate(2);
	sprite.SetDepth(850);
	sprite.SetUVs(0,0,64,96);
	TexManager.LoadTexture(textureFile);
}

void Gun::Update()
{
	sprite.Update();
}

void Gun::Render()
{
	TexManager.UploadTextureToBuffer(textureFile, TextureManager::BUFFER2);
	TexManager.SetTexture(textureFile);
	sprite.Render();
}

void Gun::Fire()
{

	sprite.AnimateOnce();
}	