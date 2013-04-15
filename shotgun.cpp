#include "shotgun.h"

#include "texturemanager.h"


Shotgun::Shotgun()
{
	textureFile = "shotgunFire.bmp";
	sprite = AnimatedSprite(0, 106, 150, 300, 0);
	reloadImage = "shotgunReload.bmp";
	reloadSprite = AnimatedSprite(0, 106, 300, 300, 0);
}

Shotgun::~Shotgun()
{}

void Shotgun::Init()
{
	sprite.SetNumberOfFrames(6);
	sprite.SetAnimationRate(0.1f);
	sprite.SetDepth(850);
	sprite.SetUVs(0,0,64,128);
	TexManager.LoadTexture(textureFile);
	
	reloadSprite.SetNumberOfFrames(4);
	reloadSprite.SetAnimationRate(0.15f);
	reloadSprite.SetDepth(850);
	reloadSprite.SetUVs(0,0,128,128);
	TexManager.LoadTexture(reloadImage);
	
	damage = 60;
	ammo = MAXAMMO;
}

void Shotgun::Render()
{
	if (reloadSprite.IsAnimating()) {
		std::cout << "Reload Sprite to Draw" << std::endl;
		TexManager.UploadTextureToBuffer(reloadImage, TextureManager::BUFFER2);
		TexManager.SetTexture(reloadImage);	
		reloadSprite.Render();
	} else {
		TexManager.UploadTextureToBuffer(textureFile, TextureManager::BUFFER2);
		TexManager.SetTexture(textureFile);		
		sprite.Render();
	}
}


void Shotgun::Update()
{
	if (reloadSprite.IsAnimating()) {
		reloadSprite.Update();
	} else {
		sprite.Update();
	}
}

void Shotgun::Fire()
{
	sprite.AnimateOnce();
	ammo--;
}

void Shotgun::Reload()
{
	if (MAXAMMO - ammo > 0) {
		reloadSprite.AnimateLoop(MAXAMMO-ammo);
		ammo = MAXAMMO;
	}
}