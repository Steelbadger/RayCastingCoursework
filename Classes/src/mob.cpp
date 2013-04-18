#include "mob.h"
#include "texturemanager.h"
#include "level.h"

#include "PS2Defines.h"

#include <iostream>

//  Constructor set a default position, target and health
Mob::Mob(): position(1.0f, 1.0f), target(0.0f, 0.0f), health(50)
{}

//  Primary constructor, set textures, setup sprite sizes and place at pos
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
	
	//  At start we'll be using the normal texture (with walking and normal death)
	currentTex = texFile;
	
	//  It'll always be moving so loop it
	sprite.AnimateLoop();
	
	//  No target for the time being
	target = Vector2(0.0f, 0.0f);
	
	//  Is not attacking and has not attacked
	attacking = false;
	wasAttacking = false;
	
	//  Default base speed
	speed = 1.0f;
}

Mob::~Mob()
{
	//  Nuffin
}


void Mob::MoveTo(float x, float y)
//  Set the mob at a position in the world
{
	position = Vector2(x, y);
}
void Mob::MoveTo(Vector2 newPos)
//  Set the mob at a position in the world
{
	position = newPos;
}

void Mob::MoveSpriteTo(float x, float y)
//  Set mob's sprite at a position on the screen
{
	sprite.MoveTo(x, y);
}

void Mob::MoveDelta(float x, float y)
//  Change position relative to current in world
{
	position += Vector2(x, y);
}
void Mob::MoveDelta(Vector2 movement)
//  Change position relative to current in world
{
	position += movement;
}

void Mob::Kill()
//  Start the normal kill animation, make sure all animation
//  specific variables are correct
{
	//  Stop the current animation
	sprite.ResetAnimationAndStop();
	//  The place in the spritesheet the animation begins
	sprite.SetBase(7);
	
	//  The number of frames the animation lasts
	sprite.SetNumberOfFrames(5);
	
	//  The time between frames
	sprite.SetAnimationRate(0.15f);
	
	//  The texture necessary to draw this animation
	currentTex = texFile;
	
	//  Animate it!
	sprite.AnimateOnceAndStop();
}

void Mob::CatastrophicKill()
//  Start the catastrophic kill animation, make sure all animation
//  specific variables are correct
{
	// stop the current animation
	sprite.ResetAnimationAndStop();
	// the first frame of the animation
	sprite.SetBase(0);
	
	//  the number of frames in the animation
	sprite.SetNumberOfFrames(8);
	
	//  The time between frames
	sprite.SetAnimationRate(0.15f);
	
	//  Use the correct texture
	currentTex = megaKillTex;
	
	//Animate it!
	sprite.AnimateOnceAndStop();
}

void Mob::AnimateAttack()
//  Start the attack animation, make sure all animation
//  specific variables are correct
{
	//  Stop the current animation
	sprite.ResetAnimationAndStop();
	
	//  Set the base frame of the animation
	sprite.SetBase(4);
	
	//  The number of frames in the animation
	sprite.SetNumberOfFrames(3);
	
	//  The time between frames
	sprite.SetAnimationRate(0.15f);
	
	//  The proper texture file
	currentTex = texFile;
	
	//  Animate it!
	sprite.AnimateOnceAndStop();
	
	//  Set state
	attacking = true;
}

void Mob::AnimateWalk()
//  Start the walking animation, make sure all animation
//  specific variables are correct
{
	//  The first frame of the animation
	sprite.SetBase(0);
	//  The number of frames in the animation
	sprite.SetNumberOfFrames(4);
	
	//  The time between frames
	sprite.SetAnimationRate(0.2f);
	
	//  The necessary texture for the animation
	currentTex = texFile;
	
	//  Animate it!
	sprite.AnimateLoop();
}

void Mob::Render()
//  Draw the animated sprite to the screen
{
	TexManager.UploadTextureToBuffer(currentTex, TextureManager::BUFFER1);
	TexManager.SetTexture(currentTex);
	sprite.Render();
}

void Mob::Update(Level& level, double timeDif)
//  Update the sprite, Move sprite towards player, check collision
//  with map and update the animation
{
	//  Update the animation
	sprite.Update();
	
	//  Mob is no longer where it was last frame
	level.ClearMobAt(position.x, position.y);
	
	//  Mob has done no damage this frame
	damageDealt = 0;
	
	//  If the mob is dead then don't do anything else
	if (!IsDead()) {
		//  How far to target
		float distSqr = Vector2(position-target).LengthSqr();
		
		//  if close and not already doing so then start attacking
		if (distSqr < 0.5f && attacking == false) {
			AnimateAttack();
		//  otherwise, if we're not animating then we have finished an attack
		//  (if we're a long way off we'll never stop animating due to walkloop)
		} else if (!sprite.IsAnimating()) {
			//  So we stopped attacking
			attacking = false;
			//  If we just stopped attacking then deal damage (damage dealt at end of attack)
			if (wasAttacking && !attacking) {
				damageDealt = baseDamage;
			}
			//  Start walking again
			AnimateWalk();		
		}
		//  If we're too far to attack (and are not attacking)
		//  Then walk in direction of target
		if (attacking == false && distSqr < 100.0f)	 {
			WalkToTarget(level, timeDif);
		}
	}
	//  save our attack state
	wasAttacking = attacking;
}

void Mob::WalkToTarget(Level& level, double timeDif)
//  Walks in a straight line directly at the target
{
	//  find the direction of travel
	Vector2 direction = target-position;

	//  Get the distance along this vector we should walk
	Vector2 translationVector = direction.Normalise() * timeDif * speed;
	
	//  Find the translated x and y positions
	float x, y;
	//  current pos		translation				a littlebit extra to stop frustum clipping
	x = position.x + (translationVector.x) + (translationVector.x/Abs(translationVector.x))*0.2;
	y = position.y + (translationVector.y) + (translationVector.y/Abs(translationVector.y))*0.2;			
	
	//  Check for collisions
	if (level.MobCheckAt(x, position.y) == 0) {
		position.x += translationVector.x;
	}
	
	if (level.MobCheckAt(position.x, y) == 0) {
		position.y += translationVector.y;
	}
	
	//  tell the level map that this mob is at it's new location
	level.MobAt(position.x, position.y);
}


bool Mob::IsInside(Vector2 point)
//  Check if point is within the sprite in screen space
{
	Vector2 Pos = GetSpritePosition();
	Vector2 siz = GetSpriteSize();
	
	//  Basic AABB collision
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
//  Deal damage to the mob
{
	health -= dmg;
}

void Mob::SetHitpoints(int hp)
//  Set the mob to a new max hp value
//  Keep proportion of health the same across change
{
	health = (health/baseHealth)*hp;
	baseHealth = hp;
}