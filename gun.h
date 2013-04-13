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
	
	void Render();
	virtual void Fire();
	void Update();
	int GetDamage(){return damage;}
	bool IsFiring(){return sprite.IsAnimating();}
	
private:
	AnimatedSprite sprite;
	std::string textureFile;
	int damage;
};

#endif