#include "level.h"
#include <iostream>
#include <fstream>
#include <iostream>

//  Default hardcoded map
const unsigned int Level::DEFAULTMAP[MAPWIDTH][MAPHEIGHT] = 	{{	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 3, 3, 3, 3, 3, 3, 4, 4, 4, 4},
																{	1, 0, 0, 0, 1, 0, 0, 0, 0, 1, 3, 0, 0, 0, 0, 0, 0, 0, 0, 4},
																{	1, 0, 0, 0, 1, 0, 0, 0, 0, 1, 3, 0, 0, 0, 0, 3, 4, 0, 0, 4},
																{	1, 0, 0, 0, 1, 0, 0, 0, 0, 1, 3, 3, 3, 0, 3, 3, 4, 0, 0, 4},
																{	1, 0, 1, 1, 1, 0, 1, 1, 1, 1, 2, 2, 2, 0, 2, 0, 4, 0, 0, 4},
																{	1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 2, 0, 0, 0, 2, 0, 4, 0, 0, 4},
																{	1, 0, 0, 1, 0, 1, 1, 0, 0, 1, 2, 0, 0, 0, 2, 0, 0, 0, 0, 4},
																{	1, 0, 0, 1, 0, 1, 1, 1, 1, 1, 2, 0, 0, 0, 2, 0, 4, 4, 0, 4},
																{	1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 0, 5, 0, 0, 5},
																{	1, 0, 0, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 0, 5, 0, 5, 5},
																{	1, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 5, 0, 5, 5},
																{	1, 0, 0, 1, 0, 0, 0, 0, 1, 0, 5, 5, 5, 5, 5, 0, 0, 0, 0, 0},
																{	1, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 5, 5, 5, 0, 5, 0},
																{	1, 0, 0, 1, 0, 1, 1, 1, 1, 1, 1, 5, 5, 0, 0, 0, 5, 0, 5, 0},
																{	1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 5, 5, 5, 0, 0, 0, 5, 0},
																{	1, 0, 1, 1, 1, 1, 0, 0, 0, 0, 1, 0, 0, 0, 5, 0, 5, 5, 5, 0},
																{	1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 5, 5, 5, 0, 0, 0, 0, 0},
																{	1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 5, 5, 5, 0},
																{	1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 5, 5, 5, 5, 5, 0, 5, 0},
																{	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0}};
																

															
//  Constructor															
Level::Level()
{
//	LoadDefault();
}

//  Constructor with load map from vector (DEPRECATED)
Level::Level(std::vector<std::vector<unsigned int> > lMap)
{
	LoadMap(lMap);
}

Level::~Level()
{
}

//  Load map from a vector (DEPRECATED)
void Level::LoadMap(std::vector<std::vector<unsigned int> > lMap)
{
	levelMap = lMap;
}

//  Load the default map array into level map vector
void Level::LoadDefault()
{
	std::cout << "Loading Default Map" << std::endl;
	//  Load the default map into the level, but transpose it to make it make more sense visually
	//  (default map is arranged y-x, fill it as if it were x-y)
	levelMap.resize(MAPHEIGHT);
	
	for (int column = 0; column < MAPHEIGHT; column++) {
		for (int row = 0; row < MAPWIDTH; row++) {
			levelMap[column].push_back(DEFAULTMAP[column][row]);
		}	
	}
}

void Level::LoadLevel(std::string levelFile)
//  This function parses a text file and loads data from it
//  Format:  	Numbers 0-16:	walls
//				Letter S		player start
//				Letter E		enemy start
//				Letter C		completion condition
{
	//  if the file is empty do nothing
	if (levelFile.empty()) {
		return;
	}
	
	//  Open the file, if we cannot then do return and don't try to load
	std::ifstream fileStream(levelFile.c_str(), ifstream::in);	
	if (!fileStream.is_open()) {
		std::cout << "Could Not Open Level File: " << levelFile << std::endl;
		return;
	}
	
	//  Clear our previous data
	levelMap.clear();
	mobList.clear();
	completionConditions.clear();
	
	//  Temp values for tracking what character we got and where we got it
	char c;
	int column = 0;
	int row = 0;
	unsigned int wall;
	
	//  Storage for the basic mob locations
	std::vector<Vector2> mobPositions;
	
	//  Empty dummy vector to push to primary vector (represents a row on the map)
	std::vector<unsigned int> temp;
	
	//  While we can read the file
	while (fileStream.good()) {
		//  Get the next character
		fileStream.get(c);
		
		//  If the next character is a hash then ignore the line (comments)
		if (c == '#') {
			fileStream.ignore(256, '\n');
		//  If the character is end of file then stop reading	
		} else if (c == EOF) {
			break;	
		//  If the character is not a newline then go deeper
		} else if (c != '\n' ){
			//  We've found a new row, so push a new row to our 2D vector
			levelMap.push_back(temp);
			
			//  Take a look at the next character
			int peek = fileStream.peek();
			//  While it's not a newline or End of File
			while(peek != '\n' && peek != EOF) {
				wall = 0;
				//  If the character of peek is a number then get the whole number out
				//  this may be one or two digits long
				if (peek >= '0' && peek <= '9') {
					fileStream >> wall;
					column++;
				//  If the character of peek is S then add our position in the array
				//  by row and column variables to the player start position
				//  then pop the character from the filestream
				} else if (peek == 'S') {
					StartPosition = Vector2(row + 0.5f, column + 0.5f);
					column++;
					fileStream.ignore(1);
				//  If peek is an E then add a new position at which to create an enemy
				//  then pop the character from the filestream
				} else if (peek == 'E') {
					mobPositions.push_back(Vector2(row + 0.5f, column + 0.5f));
					column++;
					fileStream.ignore(1);
				//  If peek is a C then add a new grid base to the completion conditions
				//  then pop the character from the filestream
				} else if (peek == 'C') {
					completionConditions.push_back(Vector2(row, column));
					column++;
					fileStream.ignore(1);
				//  Any other character is probably a whitespace character, ignore it	
				} else {
					fileStream.ignore(1);
					peek = fileStream.peek();
					continue;
				}
				//  Enter the value into the levelmap (S, E, C are all = 0 in the map)
				levelMap[row].push_back(wall);
				//  Look at the next character
				peek = fileStream.peek();
			}
			//  Next row
			row++;
			//  first entry
			column = 0;
		}
	}
	//  Once done close the file
	fileStream.close();
	
	//  Then create mobs from the list of positions found
	std::vector<Vector2>::iterator it;
	for (it = mobPositions.begin(); it != mobPositions.end(); it++) {
		mobList.push_back(Mob(*it));
	}
	
}

unsigned int Level::At(int x, int y)
//  Sample the level map for walls, return walls beyond array extents
//  number 20 is used by mobs to indicate inhabiting a grid square
//  ignore it as this is used for raycasting, mobs are handled elsewhere
{
	if (x >= levelMap.size() || x < 0 || y >= levelMap[0].size() || y < 0) {
		return 1;
	} else if (levelMap[x][y] == 20){
		return 0;
	} else {
		return levelMap[x][y];
	}
}

unsigned int Level::MobCheckAt(int x, int y)
//  This is used for collision by mobs, only mobs may collide with other mobs
{
	if (x >= levelMap.size() || x < 0 || y >= levelMap[0].size() || y < 0) {
		return 1;
	} else if (levelMap[x][y] == 20){
		return 1;
	} else {
		return levelMap[x][y];
	}
}

void Level::MobAt(int x, int y)
//  A mob is current occupying grid x,y
{
	levelMap[x][y] = 20;
}

void Level::ClearMobAt(int x, int y)
//  A mob is no longer occupying grid x, y
{
	levelMap[x][y] = 0;
}

bool Level::IsCompletion(int x, int y)
//  Check if the coordinates are in the grid squares designated as completion conditions
{
	std::vector<Vector2>::iterator it;
	for (it = completionConditions.begin(); it != completionConditions.end(); it++) {
		if (int((*it).x) == x && int((*it).y) == y) {
			return true;
		}
	}
	return false;
}

bool Level::IsCompletion(Vector2 pos)
//  alternative overload using a vector2
{
	int x = int(pos.x);
	int y = int(pos.y);
	return IsCompletion(x, y);
}