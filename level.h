#ifndef __LEVEL_H__
#define __LEVEL_H__
#include <vector>

#define MAPWIDTH 20
#define MAPHEIGHT 20


class Level
{
public:
	Level();
	Level(std::vector<std::vector<unsigned int> > levelMap);
	~Level();
	void LoadMap(std::vector<std::vector<unsigned int> > levelMap);
	void LoadDefault();
	
	unsigned int At(int x, int y);
	
private:
	std::vector<std::vector<unsigned int> > levelMap;
	const static unsigned int DEFAULTMAP[MAPWIDTH][MAPHEIGHT];
};

#endif