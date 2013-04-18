#ifndef __MOBMANAGER_H__
#define __MOBMANAGER_H__
//////////////////////--By Ross Davies--//////////////////////
/*		Stores a list of mobs and handles calling 
		all of the necessary functions for each mob
		per frame.  Also collects the responses from
		all mobs and passes damage through to the
		play state to be applied to the player	

		Mobs are stored in the list with some additional
		data to improve sorting and rendering efficiency	*/
//////////////////////////////////////////////////////////////
#include "mob.h"
#include <list>
#include <map>
#include <string>
#include <vector>

//  The struct stored by the mob list at the heart of the MobManager
//  Contains a single Mob, a Boolean that stores whether the mob
//  is onscreen or not (data found by FindMobPositions function)
//  and a Boolean which stores whether the MobManager has ACTED
//  upon the death of the mob (this is not necessarily the same
//  as simply calling mob.IsDead)
struct MobData
{
	Mob mob;
	bool onscreen;
	bool dead;
};

//  Predeclare level as this needs to be passed through the individual mobs
//  The mob manager itself does not use it
class Level;

class MobManager
{
public:
	//  Constructor
	MobManager();
	
	//  Destructor
	~MobManager();
	
	//  Tell all mobs to draw their sprites to the screen
	void DrawMobs();
	
	//  Find where each mob's sprite should be in screen space, then tell
	//  each mob to update with this position and size
	void FindMobPositions(Vector2 playerPosition, Vector2 playerDirection, float FoV);
	
	//  Call the mobs individual update functions so that animations run and mobs can move/attack
	void UpdateMobs(Level& level, double timeDif);
	
	//  DEBUG FUNCTION, duplicate of FindMobPositions with output statements
	void DEBUGFindMobPositions(Vector2 playerPosition, Vector2 playerDirection, float FoV);	
	
	//  Push a new mob to the list of mobs
	void AddMob(Mob mob);
	
	//  Load a while new collection of mobs, clears the previous list of mobs
	void LoadMobList(std::vector<Mob> mobs);

	//  Checks collision of the 0,0 point at the centre of the screen
	//  with each mob's sprite
	void ShootMobs(float distToWallAtCursor, int damage);
	
	//  Set the base hitpoints of all mobs in the list
	void SetMobHitpoints(int hp);
	
	//  Set the base damage of all mobs in the list
	void SetMobDamage(int damage);
	
	//  Set the base speed of all mobs in the list
	void SetMobSpeed(float speed);
	
	//  Collect all damage dealt by all mobs for dealing to player
	int CheckMobDealtDamage();
	
	//  empty the list of mobs
	void ClearMobs();
	
	//  sorting function used in mob list, sorts by depth so that alpha
	//  blending works correctly
	static bool MobComparator(MobData lhs, MobData rhs);
private:

	//  The maximum number of mobs per map
	static const int MAXMOBS = 30;

	//  storage for the mobs
	std::list<MobData> mobs;
};

#endif