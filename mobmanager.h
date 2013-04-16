#ifndef __MOBMANAGER_H__
#define __MOBMANAGER_H__

#include "mob.h"
#include <list>
#include <map>
#include <string>
#include <vector>

struct MobData
{
	Mob mob;
	bool onscreen;
	bool dead;
};

class Level;

class MobManager
{
public:
	MobManager();
	~MobManager();
	void DrawMobs();
	void FindMobPositions(Vector2 playerPosition, Vector2 playerDirection, float FoV);
	void UpdateMobs(Level& level, double timeDif);
	void DEBUGFindMobPositions(Vector2 playerPosition, Vector2 playerDirection, float FoV);	
	void AddMob(Mob mob);
	void LoadMobList(std::vector<Mob> mobs);
	void RemoveMob(MobData mob);
	void ShootMobs(float distToWallAtCursor, int damage);
	void SetMobHitpoints(int hp);
	void SetMobDamage(int damage);
	int CheckMobDealtDamage();
	void ClearMobs();
	
	static bool MobComparator(MobData lhs, MobData rhs);
private:
	static const int MAXMOBS = 30;

	std::list<MobData> mobs;
};

#endif