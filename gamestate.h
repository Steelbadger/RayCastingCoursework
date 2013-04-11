#ifndef __GAMESTATE_H__
#define __GAMESTATE_H__


class GameState
{
public:
	enum StateReturnValue { NONE, STARTUP, MENU, OPTIONS, OPTIONSPAUSED, HELP, HELPPAUSED, GAMEACTIVE, GAMEPAUSED, GAMEWIN, GAMELOSE, QUIT };
	GameState(){};
	virtual ~GameState(){};
	
	virtual void Initialise() = 0;
	virtual void Update() = 0;
	virtual void Render() = 0;
	virtual void PriorState(GameState* state){return;}
	StateReturnValue GetReturn(){StateReturnValue temp = value; value = NONE; return temp;}
	
protected:
	StateReturnValue value;
};

#endif