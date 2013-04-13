#include "gun.h"
#include "texturemanager.h"

#include <iostream>

Gun::Gun() :
	textureFile("gunSheetalt.bmp"),
	sprite(0, 156, 100, 200, 0)
{
}

Gun::~Gun()
{}


void Gun::Init()
{
	std::cout << "Broken Here?" << std::endl;
	sprite.SetNumberOfFrames(4);
	sprite.SetAnimationRate(0.1f);
	sprite.SetDepth(850);
	sprite.SetUVs(0,0,64,96);
	TexManager.LoadTexture(textureFile);
	damage = 20;
	std::cout << "Nope!" << std::endl;
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