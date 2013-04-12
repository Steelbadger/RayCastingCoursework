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
	
private:
	std::vector<std::vector<unsigned int> > levelMap;
	Vector2 StartPosition;
	Vector2 StartDirection;
	std::vector<Mob> mobList;
	std::vector<Vector2> completionConditions;
	
	const static unsigned int DEFAULTMAP[MAPWIDTH][MAPHEIGHT];
};

#endif