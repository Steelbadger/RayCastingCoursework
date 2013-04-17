#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "gun.h"
#include "shotgun.h"
#include "vector2.h"

#include "primitives.h"

#include <string>

class Level;

class Player
{
public:
	Player();
	~Player();
	
	void Initialise(Vector2 position, Vector2 direction, Level& level);
	void Update(double timeDif);
	void Render();
	bool HasFired();
	int GetGunMaxDamage();
	void Damage(int damage);
	bool IsDead();
	Vector2 GetPosition();
	Vector2 GetDirection();
	void SetSensitivity(float s){sensitivity = s;}
	
private:

	void UpdateHealthBar();
	
	Gun pistol;
	Shotgun shotgun;
	Gun* activeWeapon;
	float sensitivity;
	float speed;
	int hitpoints;
	Vector2 position;
	Vector2 direction;
	
	PS2Sprite healthBar;
	PS2SpriteT ammo;
	
	std::string ammoImg;
	
	char rumbleIntensity;
	
	Level* level;
	bool firedWeapon;
	const int MAXHP;
	
};

#endif