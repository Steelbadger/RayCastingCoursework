//////////////////////------------------//////////////////////
/*			
						By Ross Davies
															*/
//////////////////////------------------//////////////////////


#include "shotgun.h"

#include "texturemanager.h"
#include "audiomanager.h"

//  Setup the sprites and store the filenames for assets
Shotgun::Shotgun()
{
	textureFile = "shotgunFire.bmp";
	sprite = AnimatedSprite(0, 106, 150, 300, 0);
	reloadImage = "shotgunReload.bmp";
	reloadSprite = AnimatedSprite(0, 106, 300, 300, 0);
	shotSound = "shotgun";
}

Shotgun::~Shotgun()
{
	//lolnope
}

void Shotgun::Init()
//  Continue setting up the sprites, load in the texture files and audio
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
	
	damage = 80;
	ammo = MAXAMMO;
	
	Audio.Load(shotSound, AudioManager::DSP1);
}

void Shotgun::Render()
//  Depending on which animation set we're using draw the relevant sprite/texture
{
	if (reloadSprite.IsAnimating()) {
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
//  Update the animations
{
	if (reloadSprite.IsAnimating()) {
		reloadSprite.Update();
	} else {
		sprite.Update();
	}
}

void Shotgun::Fire()
//  Begin the animation to fire and play the sound file
{
	Audio.Reset(shotSound);
	sprite.AnimateOnce();
	Audio.Play(shotSound);
	ammo--;
}

void Shotgun::Reload()
//  Play the animation to reload
{
	if (MAXAMMO - ammo > 0) {
		reloadSprite.AnimateLoop(MAXAMMO-ammo);
		ammo = MAXAMMO;
	}
}