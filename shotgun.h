#ifndef __SHOTGUN_H__
#define __SHOTGUN_H__

#include "gun.h"

#include "animatedsprite.h"
#include <string>

class Shotgun : public Gun
{
public:
	Shotgun();
	virtual ~Shotgun();
	
	virtual void Init();
	virtual void Update();
	virtual void Render();
	virtual void Fire();
	virtual void Reload();
	
private:
	AnimatedSprite reloadSprite;
	std::string reloadImage;
	static const int MAXAMMO = 4;
};

#endif