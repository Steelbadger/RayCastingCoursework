#ifndef __OPTIONSSTATE_H__
#define __OPTIONSSTATE_H__
//////////////////////--By Ross Davies--//////////////////////
/*		state object that is called when the player
		enters the options menu, allows the player
		to set difficulty, rumble and sensitivity			*/
//////////////////////////////////////////////////////////////
#include "options.h"
#include "gamestate.h"
#include "primitives.h"

#include <string>

class OptionsState : public GameState
{
public:
	//  Constructor
	OptionsState();
	
	//  Virtual Destructor for inheritance
	virtual ~OptionsState();
	
	
	//  Functions for hooking into virtuals on parent
	//  Initialise loads necessary assets	
	virtual void Initialise();
	
	//  Update waits for input and updates menu option sprites
	virtual void Update();
	
	//  Draw the background and option menu sprites to the screen	
	virtual void Render();
	
	//  Needs a pointer to the last state, expects either PlayState or
	//  MenuState, used to choose what state to return, if this is playstate
	//  it will be used to draw a background+semi-transparent quad like the pause menu
	virtual void PriorState(GameState* priorState);
	
	//  Function for getting the new options that have been set
	Options GetOptions();
	
	//  Function to save the current options so that they are active when
	//  next we open the options state
	void SaveOptions();

private:

	//  Logic for finding the sprite referenced by the cursorPos enum
	PS2SpriteT& GetOptionOfCursor();
	
	//  Function for handling logic when cursor is over the difficulty 
	void SetDifficultyOptionSprite();
	
	//  Function for handling logic when cursor is over the rumble option
	void SetRumbleOptionSprite();	
	
	//  Function for handling logic when cursor is over the sensitivity option
	void SetSensitivityOptionSprite();
	
	//  Check for input and call logic functions to change options
	void CheckInput();

	
	//  All possible changeable options, explicitly enumerated so we can use integer maths
	enum OptionSelect {DIFFICULTY = 0, RUMBLE = 1, SENSITIVITY = 2, RESET = 3, RETURN = 4};
	
	//  Sprites for displaying all menu options
	PS2SpriteT optionsTitle;
	PS2SpriteT difficulty;
	PS2SpriteT diffOpt;
	PS2SpriteT rumble;
	PS2SpriteT rumbleOpt;
	PS2SpriteT sensitivity;
	PS2SpriteT sensOpt;
	PS2SpriteT back;
	PS2SpriteT reset;
	
	//  When this state is MenuState we return to that (and draw a black background)
	//  When this state is PlayState we return to paused (and draw the play state in the background)
	GameState* background;
	
	//  Textures for applying to the menu and options
	std::string difficultyImg;
	std::string rumbleImg;
	std::string sensitivityImg;
	std::string titleImg;
	
	//  Sprite for the black background, or overlay to fade
	//  the playstate background out
	PS2Sprite darkenOverlay;
	
	//  The options currently selected (and displayed on screen)
	Options currentOptions;
	
	//  The actual options being used by the play state
	Options originalOptions;
	
	//  a delay timer to slow the analogue stick movement down
	float analogueDelay;

	//  The options currently indicated by the player
	OptionSelect cursorPos;	

};

#endif