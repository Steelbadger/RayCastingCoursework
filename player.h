#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "gun.h"
#include "shotgun.h"
#include "vector2.h"

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
	
private:
	Gun pistol;
	Shotgun shotgun;
	Gun* activeWeapon;
	int hitpoints;
	Vector2 position;
	Vector2 direction;
	Level* level;
	bool firedWeapon;
	
};

#endif