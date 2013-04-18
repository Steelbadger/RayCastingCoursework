#ifndef __SHOTGUN_H__
#define __SHOTGUN_H__
//////////////////////--By Ross Davies--//////////////////////
/*		The extended Gun class, this gun has limited 
		ammo in a clip and requires reloading				*/
//////////////////////////////////////////////////////////////
#include "gun.h"

#include "animatedsprite.h"
#include <string>

class Shotgun : public Gun
{
public:
	//  Constructor
	Shotgun();
	
	//  Virtual Destructor for inheritance
	virtual ~Shotgun();
	
	//  Hook into base Gun functions
	//  functions are identical in function
	//  but hundle the additional factor of
	//  ammo and reloading
	virtual void Init();
	virtual void Update();
	virtual void Render();
	virtual void Fire();
	virtual void Reload();
	virtual bool IsFiring(){return (sprite.IsAnimating() || reloadSprite.IsAnimating());}
	
private:
	//  An additional animated sprite used for drawing the
	//  realod animation
	AnimatedSprite reloadSprite;
	
	//  The image file that has the relaoding animation textures
	std::string reloadImage;
	
	//  The ammo in a full clip
	static const int MAXAMMO = 4;
};

#endif