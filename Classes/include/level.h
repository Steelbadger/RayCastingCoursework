#ifndef __LEVEL_H__
#define __LEVEL_H__
//////////////////////--By Ross Davies--//////////////////////
/*		The level class stores information about wall
		locations for raycasting checks.  Handles 
		loading of the leveldata from a specified file 
		and can pass them to the managers that need
		the information (mob location, win conditions
		and player start position)							*/
//////////////////////////////////////////////////////////////

#include "mob.h"

#include <vector>
#include <string>

//  Size of the default map (loaded maps can be of any size)
#define MAPWIDTH 20
#define MAPHEIGHT 20


class Level
{
public:
	//  Constructors
	Level();
	//  DEPRECATED:  use default and a level file instead
	Level(std::vector<std::vector<unsigned int> > levelMap);
	
	//  Destructor
	~Level();
	
	//  DEPRECATED load a map from a 2D vector, use LoadLevel(filename) instead
	void LoadMap(std::vector<std::vector<unsigned int> > levelMap);
	
	//  Load level data from a file and store it in corresponding vectors
	void LoadLevel(std::string levelFile);
	
	//  Load the default map layout
	void LoadDefault();
	
	//  Check to find if a space is wall of open space (0 indicated open space
	//  other numbers represent different wall textures)
	unsigned int At(int x, int y);
	
	//  Check to see if a mob (entity) is currently occupying a space
	unsigned int MobCheckAt(int x, int y);
	
	//  Edit map to indicate that a mob is in this space
	void MobAt(int x, int y);
	
	//  Edit map to indicate that a mob is NO LONGER in this spot
	void ClearMobAt(int x, int y);
	
	//  Check to see if a given grid square is designated as a
	//  completion condition
	bool IsCompletion(int x, int y);
	bool IsCompletion(Vector2 pos);
	
	//  Return the start position for a loaded level
	//  ONLY use after LoadLevel(filename) has been called
	Vector2 GetPlayerStartPosition(){return StartPosition;}
	
	//  Return the mob start states as defined by the loaded level
	//  ONLY use after LoadLevel(filename) has been called
	std::vector<Mob> GetMobs(){return mobList;}
	
private:

	//  Storage for walls (collision grid)
	std::vector<std::vector<unsigned int> > levelMap;
	
	//  The player's starting direction for the level
	Vector2 StartPosition;
	
	//  The player's starting direction for the level
	Vector2 StartDirection;
	
	//  Initial states for mobs in level
	std::vector<Mob> mobList;
	
	//  List of locations where the player can 'win' the level
	std::vector<Vector2> completionConditions;
	
	
	//  The default map for use when no levelfile exists
	const static unsigned int DEFAULTMAP[MAPWIDTH][MAPHEIGHT];
};

#endif