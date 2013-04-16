#include "mob.h"
#include "texturemanager.h"
#include "level.h"

#include "PS2Defines.h"

#include <iostream>


Mob::Mob(): position(1.0f, 1.0f), target(0.0f, 0.0f), health(50)
{}

Mob::Mob(Vector2 pos):
	position(pos),
	texFile("mob1.bmp"),
	megaKillTex("mob2.bmp"),
	height(1.6f),
	sprite(0.0f, 0.0f, 1.6f*256.0f, 1.6f*256.0f),
	baseHealth(50),	
	health(baseHealth),
	baseDamage(10)
{
	sprite.SetUVs(0, 0, 64, 64);
	sprite.SetBase(0);
	sprite.SetNumberOfFrames(4);
	sprite.SetAnimationRate(0.2f);
	currentTex = texFile;
	sprite.AnimateLoop();
	target = Vector2(0.0f, 0.0f);
	attacking = false;
	wasAttacking = false;
}

Mob::~Mob()
{}


void Mob::MoveTo(float x, float y)
{
	position = Vector2(x, y);
}
void Mob::MoveTo(Vector2 newPos)
{
	position = newPos;
}

void Mob::MoveSpriteTo(float x, float y)
{
	sprite.MoveTo(x, y);
}

void Mob::MoveDelta(float x, float y)
{
	position += Vector2(x, y);
}
void Mob::MoveDelta(Vector2 movement)
{
	position += movement;
}

void Mob::Kill()
{
	sprite.ResetAnimationAndStop();
	sprite.SetBase(7);
	sprite.SetNumberOfFrames(5);
	sprite.SetAnimationRate(0.15f);
	currentTex = texFile;
	sprite.AnimateOnceAndStop();
}

void Mob::CatastrophicKill()
{
	sprite.ResetAnimationAndStop();
	sprite.SetBase(0);
	sprite.SetNumberOfFrames(8);
	sprite.SetAnimationRate(0.15f);
	currentTex = megaKillTex;
	sprite.AnimateOnceAndStop();
}

void Mob::AnimateAttack()
{
	sprite.ResetAnimationAndStop();
	sprite.SetBase(4);
	sprite.SetNumberOfFrames(3);
	sprite.SetAnimationRate(0.15f);
	currentTex = texFile;
	sprite.AnimateOnceAndStop();
	attacking = true;
}

void Mob::AnimateWalk()
{
	sprite.SetBase(0);
	sprite.SetNumberOfFrames(4);
	sprite.SetAnimationRate(0.2f);
	currentTex = texFile;
	sprite.AnimateLoop();
}

void Mob::Render()
{
	TexManager.UploadTextureToBuffer(currentTex, TextureManager::BUFFER1);
	TexManager.SetTexture(currentTex);
	sprite.Render();
}

void Mob::Update(Level& level, double timeDif)
{
	sprite.Update();
	level.ClearMobAt(position.x, position.y);	
	damageDealt = 0;
	if (!IsDead()) {
		float distSqr = Vector2(position-target).LengthSqr();
		if (distSqr < 0.5f && attacking == false) {
			AnimateAttack();
		} else if (!sprite.IsAnimating()) {
			attacking = false;
			if (wasAttacking && !attacking) {
				damageDealt = baseDamage;
			} 
			AnimateWalk();		
		}
		if (attacking == false && distSqr < 100.0f)	 {
			WalkToTarget(level, timeDif);
		}
	}

	wasAttacking = attacking;
	
}

void Mob::WalkToTarget(Level& level, double timeDif)
{
	Vector2 direction = target-position;	
	Vector2 translationVector = direction.Normalise() * timeDif;
	float x, y;
	x = position.x + (translationVector.x) + (translationVector.x/Abs(translationVector.x))*0.2;
	y = position.y + (translationVector.y) + (translationVector.y/Abs(translationVector.y))*0.2;			
	
	if (level.MobCheckAt(x, position.y) == 0) {
		position.x += translationVector.x;
	}
	
	if (level.MobCheckAt(position.x, y) == 0) {
		position.y += translationVector.y;
	}
	level.MobAt(position.x, position.y);
}


bool Mob::IsInside(Vector2 point)
{
	Vector2 Pos = GetSpritePosition();
	Vector2 siz = GetSpriteSize();
	if (point.x > (sprite.GetX() - sprite.GetWidth()*sprite.GetScale()*0.5f) &&
		point.x < (sprite.GetX() + sprite.GetWidth()*sprite.GetScale()*0.5f))
	{
		if (point.y > (sprite.GetY() - sprite.GetHeight()*sprite.GetScale()*0.5f) &&
			point.y < (sprite.GetY() + sprite.GetHeight()*sprite.GetScale()*0.5f))
		{
			return true;
		}
	}

	
	return false;
}

void Mob::Damage(int dmg)
{
	health -= dmg;
}

void Mob::SetHitpoints(int hp)
{
	health = (health/baseHealth)*hp;
	baseHealth = hp;
}