#ifndef __GUN_H__
#define __GUN_H__
//////////////////////--By Ross Davies--//////////////////////
/*		The basic Gun class, can be extended for variation
		but works as a basic pistol with infinite ammo
		out of the box, handles animation of the sprite 
		that represents it									*/
//////////////////////////////////////////////////////////////


#include "animatedsprite.h"

#include <string>


class Gun
{
public:

	//  Constructor
	Gun();
	
	//  Virtual Destructor for inheritance
	virtual ~Gun();
	
	//  Initialisation function, sets up sprites
	//  and textures
	virtual void Init();
	
	//  Draw the gun's sprite to the screen
	virtual void Render();
	
	//  Starts the sprite animating the firing animation
	virtual void Fire();
	
	//  Update the animating sprite each frame
	virtual void Update();
	
	//  Get the base damage of the gun
	int GetDamage(){return damage;}
	
	//  Check if the gun's sprite is currently animating,
	//  if it is then the gun is firing (or, more generally,
	//  the gun is active)
	virtual bool IsFiring(){return sprite.IsAnimating();}
	
	//  Checks if the gun has any ammo in clip
	bool HasAmmo(){return bool(ammo);}
	
	//  Get how much ammo is left in clip
	int GetAmmo(){return ammo;}
	
	//  Reload the clip to full, virtual for
	//  additional extended functionality
	virtual void Reload(){ammo = 12;}
	
	
protected:
	//  Animated gun sprite
	AnimatedSprite sprite;
	
	//  gun texture
	std::string textureFile;
	
	//  gunshot sound
	std::string shotSound;

	//  amount of damage gun deals per shot
	int damage;
	
	//  amount of ammo in clip
	int ammo;
};

#endif