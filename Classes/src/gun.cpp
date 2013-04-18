#include "gun.h"
#include "texturemanager.h"
#include "audiomanager.h"

#include <iostream>

//  Constructor, setup our texture file, sprite and shot sound
Gun::Gun() :
	textureFile("gunSheet.bmp"),
	sprite(0, 181, 150, 150, 0),
	shotSound("pistol")
{
}

Gun::~Gun()
{}


void Gun::Init()
//  Continue initialising the sprite
{
	sprite.SetNumberOfFrames(5);
	sprite.SetAnimationRate(0.1f);
	sprite.SetDepth(850);
	sprite.SetUVs(0,0,64,64);
	
	// load the texture to memory
	TexManager.LoadTexture(textureFile);
	
	//  Set base damage and ammo
	damage = 20;
	ammo = 12;
	
	//  load the audio slip into audio memory
	Audio.Load(shotSound, AudioManager::DSP1);	
}

void Gun::Update()
//  Just update the animated sprite
{
	sprite.Update();
}

void Gun::Render()
//  Draw the animated sprite to the screen
{
	TexManager.UploadTextureToBuffer(textureFile, TextureManager::BUFFER2);
	TexManager.SetTexture(textureFile);
	sprite.Render();
}

void Gun::Fire()
//  Start firing sound and animation
{
	Audio.Reset(shotSound);
	sprite.AnimateOnce();
	Audio.Play(shotSound);
}	