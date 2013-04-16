#ifndef __OPTIONSSTATE_H__
#define __OPTIONSSTATE_H__

#include "options.h"
#include "gamestate.h"
#include "primitives.h"

#include <string>

class OptionsState : public GameState
{
public:
	OptionsState();
	virtual ~OptionsState();
	
	virtual void Initialise();
	virtual void Update();
	virtual void Render();
	virtual void PriorState(GameState* priorState)
	
	Options GetOptions();
	void SaveOptions();

private:
	PS2SpriteT& GetOptionOfCursor();
	void SetDifficultyOptionSprite();
	void SetRumbleOptionSprite();	
	void SetSensitivityOptionSprite();
	
	void CheckInput();


	enum OptionSelect {DIFFICULTY = 0, RUMBLE = 1, SENSITIVITY = 2, RESET = 3, RETURN = 4};
	PS2SpriteT optionsTitle;
	PS2SpriteT difficulty;
	PS2SpriteT diffOpt;
	PS2SpriteT rumble;
	PS2SpriteT rumbleOpt;
	PS2SpriteT sensitivity;
	PS2SpriteT sensOpt;
	PS2SpriteT back;
	PS2SpriteT reset;
	GameState* background;
	
	std::string difficultyImg;
	std::string rumbleImg;
	std::string sensitivityImg;
	std::string titleImg;
	
	PS2Sprite darkenOverlay;
	
	Options currentOptions;
	Options originalOptions;
	float analogueDelay;

	OptionSelect cursorPos;	

};

#endif