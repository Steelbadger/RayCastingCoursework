#include "level.h"
#include <iostream>
#include <fstream>
#include <iostream>

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
																
Level::Level()
{
	LoadDefault();
}


Level::Level(std::vector<std::vector<unsigned int> > lMap)
{
	LoadMap(lMap);
}

Level::~Level()
{
}


void Level::LoadMap(std::vector<std::vector<unsigned int> > lMap)
{
	levelMap = lMap;
}

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
{
	if (levelFile.empty()) {
		return;
	}
	
	std::ifstream fileStream(levelFile.c_str(), ifstream::in);	
	if (!fileStream.is_open()) {
		std::cout << "Could Not Open Level File: " << levelFile << std::endl;
		return;
	}
	char c;
	int column = 0;
	int row = 0;
	unsigned int wall;
	std::vector<Vector2> mobPositions;
		
	while (fileStream.good()) {
		fileStream.get(c);
		if (c == '#') {
			fileStream.ignore(256, '\n');
		} else if (c == EOF) {
			break;		
		} else if (c != '\n' ){
			levelMap.push_back(temp);
			int peek = fileStream.peek();
			while(peek != '\n' && peek != EOF) {
				wall = 0;
				if (peek >= '0' && peek <= '9') {
					fileStream >> wall;
					column++;
				} else if (peek == 'S') {
					StartPosition = Vector2(column + 0.5f, row + 0.5f);
					column++;
					fileStream.ignore(1);
				} else if (peek == 'E') {
					mobPositions.push_back(Vector2(column + 0.5f, row + 0.5f));
					column++;
					fileStream.ignore(1);
				} else if (peek == 'C') {
					completionConditions.push_back(Vector2(column + 0.5f, row + 0.5f));
					column++;
					fileStream.ignore(1);
				} else {
					fileStream.ignore(1);
					peek = fileStream.peek();
					continue;
				}
				levelMap[row].push_back(wall);
				peek = fileStream.peek();
			}
			row++;
			column = 0;
		}
	}
	
	
}

unsigned int Level::At(int x, int y)
{
	if (x >= levelMap.size() || x < 0 || y >= levelMap[0].size() || y < 0) {
//		cout << "x: " << x << "\tMapSize: " << levelMap.size() << "\ty: " << y << "\tMapSize: " << levelMap[0].size() << std::endl;
		return 1;
	} else {
		return levelMap[x][y];
	}
}