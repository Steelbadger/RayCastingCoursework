#ifndef __GAMESTATE_H__
#define __GAMESTATE_H__


class GameState
{
public:
	enum StateReturnValue { NONE, STARTUP, MENU, OPTIONS, HELP, GAMEACTIVE, GAMEPAUSED, GAMEWIN, GAMELOSE, QUIT };
	GameState(){};
	virtual ~GameState(){};
	
	virtual void Initialise() = 0;
	virtual void Update() = 0;
	virtual void Render() = 0;
	StateReturnValue GetReturn(){return value;}
	
protected:
	StateReturnValue value;
};

#endif