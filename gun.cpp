#include "gun.h"
#include "texturemanager.h"

#include <iostream>

Gun::Gun() :
	textureFile("gunSheet.bmp"),
	sprite(0, 181, 150, 150, 0)
{
}

Gun::~Gun()
{}


void Gun::Init()
{
	std::cout << "Broken Here?" << std::endl;
	sprite.SetNumberOfFrames(5);
	sprite.SetAnimationRate(0.1f);
	sprite.SetDepth(850);
	sprite.SetUVs(0,0,64,64);
	TexManager.LoadTexture(textureFile);
	damage = 20;
	std::cout << "Nope!" << std::endl;
	ammo = 12;
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
//	ammo--;
}	