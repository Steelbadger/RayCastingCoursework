#ifndef __MOB_H__
#define __MOB_H__
//////////////////////--By Ross Davies--//////////////////////
/*		Object to represent enemies  
		is in the main menu, renders the menu to 
		the screen, checks for input and updates the 
		menu sprites to indicate current selection
		contains many functions for handling movement
		in the world and screen as well as animating
		and checking state									*/
//////////////////////////////////////////////////////////////

#include "animatedsprite.h"
#include "texture.h"
#include <string>

#include "vector2.h"

class Level;

class Mob
{	
public:	
	//  Constructors
	Mob();
	Mob(Vector2 position);
	
	//  Destructor
	~Mob();
	
	//  Move the mob in the game world
	void MoveTo(float x, float y);
	void MoveTo(Vector2 newPos);
	void MoveDelta(float x, float y);
	void MoveDelta(Vector2 movement);	
	
	//  Move the sprite on the screen
	void MoveSpriteTo(float x, float y);
	
	//  Check for collision of point with sprite on screen
	bool IsInside(Vector2 point);

	//  Get mob position in game world
	Vector2 GetPosition(){return position;}
	
	//  Get mob sprite position on screen
	Vector2 GetSpritePosition(){return Vector2(sprite.GetX(), sprite.GetY());}
	
	//  Get the apparent size of the sprite on the screen
	Vector2 GetSpriteSize(){return Vector2(sprite.GetWidth(), sprite.GetHeight())*sprite.GetScale();}
	
	//  Scale the sprite on screen by the factor (saved as an additional variable in sprite class
	//  So does not alter the base size of the sprite, only the apparent size
	void ScaleSprite(float scaleFactor){sprite.UniformScale(scaleFactor);}	

	//  Set the z-buffer depth of the sprite
	void SetDistance(float z){sprite.SetDepth(z);}
	
	//  Return the z-buffer depth of the sprite
	float GetDistance(){return sprite.GetZ();}
	
	//  Check for if the mob is dead
	bool IsDead(){return (health > 0? false : true);}
	
	//  Check if the mobs death animation is done
	bool DoneDying(){return !sprite.IsAnimating();}
	
	//  Begin the death animation
	void Kill();
	
	//  Begin the 'cloud of giblets' death animation
	void CatastrophicKill();	
	
	//  Begin the attack animation
	void AnimateAttack();	
	
	//  Begin the walking animation loop
	void AnimateWalk();	
	
	//  Return mobs current health
	int GetHealth(){return health;}
	
	//  Give the mob a target to move towards
	void SetTarget(Vector2 tar){target = tar;}
	
	//  Deal damage to the mob
	void Damage(int dmg);
	
	//  Move the mob towards it's target if possible in here
	//  if target is close queue some damage to be dealt in the
	//  damageDealt variable
	void Update(Level& level, double timeDif);
	
	//  Draw the sprite of the mob to the screen
	void Render();
	
	//  Get the height of the mob (distance from top of sprite to ground)
	float GetHeight(){return height;}
	
	//  fetch the damageDealt variable to check for any
	//  damage the mob has dealt to the player
	int GetDamageDealt(){return damageDealt;}
	
	//  Walk towards the target at speed
	void WalkToTarget(Level& level, double timeDif);
	
	//  Set the base damage dealt by the mob
	void SetDamage(int dmg){baseDamage = dmg;}
	
	//  Set the number of hitpoints the mob has
	void SetHitpoints(int hp);
	
	//  Set the base speed of the sprite
	void SetSpeed(float s){speed = s;}

private:
	//  the location in world coordiantes of the mob
	Vector2 position;
	
	//  the location in world coordinates that the mob is attempting to move
	//  towards
	Vector2 target;
	
	//  How tall the mob is
	float height;
	
	//  The amount of health the mob starts with
	int baseHealth;
	
	//  The amount of health the mob currently has
	int health;
	
	//  The speed with which the mob may move
	float speed;
	
	//  The animated sprite for displaying all textures and
	//  animations associated with the sprite
	AnimatedSprite sprite;
	
	//  the filename of the current texture file being used on the animated sprite
	std::string currentTex;
	
	//  the primary texture file, this has the walking and normal dying animation
	std::string texFile;
	
	//  the secondary texture file, this just has the 'cloud of giblets' animation
	std::string megaKillTex;
	
	
	Vector2 spriteTextureBase;
	Vector2 spriteTextureSize;
	
	//  is mob currently attacking the player
	bool attacking;
	
	//  was the mob attacking the player in the last frame
	bool wasAttacking;
	
	//  How much damage has the mob dealt to the player in the last frame
	int damageDealt;
	
	//  The base damage the mob deals per frame when in range
	int baseDamage;

};



#endif