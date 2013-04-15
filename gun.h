#ifndef __GUN_H__
#define __GUN_H__
#include "animatedsprite.h"

#include <string>


class Gun
{
public:
	Gun();
	virtual ~Gun();
	virtual void Init();
	
	virtual void Render();
	virtual void Fire();
	virtual void Update();
	int GetDamage(){return damage;}
	bool IsFiring(){return sprite.IsAnimating();}
	bool HasAmmo(){return bool(ammo);}
	int GetAmmo(){return ammo;}
	virtual void Reload(){ammo = 12;}
	
	
protected:
	AnimatedSprite sprite;
	std::string textureFile;

	int damage;
	int ammo;
};

#endif