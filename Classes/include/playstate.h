#ifndef __PLAYSTATE_H__
#define __PLAYSTATE_H__
//////////////////////--By Ross Davies--//////////////////////
/*		state object that is called while the core
		game is active, contains all mobs, the level,
		the player and handles all interactions between
		these entities										*/
//////////////////////////////////////////////////////////////
#include "gamestate.h"
#include "level.h"
#include "renderer.h"
#include "mobmanager.h"
#include "player.h"
#include "timer.h"
#include "primitives.h"
#include "options.h"


#include "vector2.h"
#include <string>

class PlayState : public GameState
{
public:
	//  Constructor
	PlayState();
	
	//  Virtual Destructor for inheritance
	virtual ~PlayState();
	
	//  Functions for hooking into virtuals on parent
	//  Initialise loads necessary assets and commands
	//  all contained objects to do the same
	virtual void Initialise();
	
	//  Tells the player, level renderer and
	//  mob manager to update
	virtual void Update();
	
	//  Tell the player, level renderer and mob manager
	//  to render their contained sprites to the screen
	virtual void Render();
	
	//  Pass Options as set in the optionsstate to this state
	void PassOptions(Options opt);
	
	//  Play the backing music
	void StartPlayMusic();
	
	//  Stop playing the backing music
	void EndMusic();
	
private:
	//  The level we are playing
	Level testLevel;
	
	//  a sprite for overlaying on the player to indicate when damage
	//  has been taken
	PS2Sprite redOverlay;
	
	//  current transparency of the red overlay
	char redAlpha;
	
	//  Does all raycasting to render walls
	Renderer testRenderer;
	
	//  Handles rendering and updating on all mobs
	MobManager testMobs;
	
	//  The player avatar
	Player player;
	
	//  Timer for equalising speed over variable framerates
	CTimer timer;
	
	//  the filename of the backing track
	std::string ambientMusic;
};

#endif