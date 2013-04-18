#ifndef __PLAYER_H__
#define __PLAYER_H__
//////////////////////--By Ross Davies--//////////////////////
/*		object that represents the player and holds all
		state data concerning the player.  Contains and 
		manages the player's weapons, position, movement 
		and health.											*/
//////////////////////////////////////////////////////////////
#include "gun.h"
#include "shotgun.h"
#include "vector2.h"

#include "primitives.h"

#include <string>

//  Predeclare Level, player needs a reference for checking collision
class Level;

class Player
{
public:
	//  Constructor
	Player();
	
	//  Destructor
	~Player();
	
	//  Set up the player's initial state and call gun initialise functions
	void Initialise(Vector2 position, Vector2 direction, Level& level);
	
	//  Check input per frame and act, check for firing and movement
	void Update(double timeDif);
	
	//  Draw the health bar and weapon to the screen
	void Render();
	
	//  Check if the player fired their weapon
	bool HasFired();
	
	//  Get the damage dealt by the current gun
	int GetGunMaxDamage();
	
	//  Deal damage to the player
	void Damage(int damage);
	
	//  Check if the player is dead (health <= 0)
	bool IsDead();
	
	//  Get the position of the player
	Vector2 GetPosition();
	
	//  Get the forward vector for the player
	Vector2 GetDirection();
	
	//  Change the look sensitivity (used when option is changed)
	void SetSensitivity(float s){sensitivity = s;}
	
private:

	//  Keep the health bar the right size for your health
	void UpdateHealthBar();
	
	//  Your two guns
	Gun pistol;
	Shotgun shotgun;
	
	//  The currently equipped weapon
	Gun* activeWeapon;
	
	//  The look sensitivity
	float sensitivity;
	
	//  The base speed of the player
	float speed;
	
	int hitpoints;

	Vector2 position;
	Vector2 direction;
	
	//  Sprite used to indicate how much health the player has
	PS2Sprite healthBar;
	
	//  Sprite used to indicate how much ammo is in clip of equipped weapon
	PS2SpriteT ammo;
	
	//  texture to apply to the ammo sprite
	std::string ammoImg;
	
	char rumbleIntensity;
	
	//  pointer to the level the player is inhabiting for checking collision
	Level* level;
	
	//  Did the player fire the frame
	bool firedWeapon;
	
	//  Player's maximum hitpoints
	const int MAXHP;
	
};

#endif