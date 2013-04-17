#ifndef __LEVEL_H__
#define __LEVEL_H__
#include "mob.h"

#include <vector>
#include <string>

#define MAPWIDTH 20
#define MAPHEIGHT 20


class Level
{
public:
	Level();
	Level(std::vector<std::vector<unsigned int> > levelMap);
	~Level();
	void LoadMap(std::vector<std::vector<unsigned int> > levelMap);
	void LoadLevel(std::string levelFile);
	void LoadDefault();
	
	unsigned int At(int x, int y);
	unsigned int MobCheckAt(int x, int y);
	void MobAt(int x, int y);
	void ClearMobAt(int x, int y);
	bool IsCompletion(int x, int y);
	bool IsCompletion(Vector2 pos);	
	Vector2 GetPlayerStartPosition(){return StartPosition;}
	std::vector<Mob> GetMobs(){return mobList;}
	
private:
	std::vector<std::vector<unsigned int> > levelMap;
	Vector2 StartPosition;
	Vector2 StartDirection;
	std::vector<Mob> mobList;
	std::vector<Vector2> completionConditions;
	
	const static unsigned int DEFAULTMAP[MAPWIDTH][MAPHEIGHT];
};

#endif