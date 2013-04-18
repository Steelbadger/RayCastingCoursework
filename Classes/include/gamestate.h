#ifndef __GAMESTATE_H__
#define __GAMESTATE_H__
//////////////////////--By Ross Davies--//////////////////////
/*		The basic pure virtual state class from which all
		game states are derived, describes the possible 
			gamestates and provides function hooks for 
		primary state functions (Initialise, Update,
					Render and PriorState)					*/
//////////////////////////////////////////////////////////////

class GameState
{
public:
	//  All possible states
	enum StateReturnValue { NONE, STARTUP, MENU, OPTIONS, HELP, GAMEACTIVE, GAMEPAUSED, GAMEWIN, GAMELOSE, QUIT };
	
	//  Default constructor to handle initialisation of shared variables for inhering classes
	GameState(StateReturnValue s): state(s), value(NONE){};
	
	//  destructor has nothing to do
	virtual ~GameState(){};
	
	//  Pure virtual function, necessary initialisation for inheriting class goes in here
	//  avoid using constructor as much as possible
	virtual void Initialise() = 0;
	
	//  Pure virtual update, all necessary updating for inhering classes should go in here
	virtual void Update() = 0;
	
	//  Pure virtual function for inheriting classes to use for render steps.
	virtual void Render() = 0;
	
	//  function hook for handing an inheriting class another gamestate
	//  has an empty implemtation here so that inheriting classes do not 
	//  need to have a unique use unless they can use the information
	virtual void PriorState(GameState* state){return;}
	
	//  Find the return value of the class, this is the State transition caused by the calling class
	StateReturnValue GetReturn(){StateReturnValue temp = value; value = NONE; return temp;}
	
	//  Get the enumerated state of the calling state (useful as states are mostly accessed with
	//  Gamestate pointers.
	StateReturnValue GetState(){return state;}
	
protected:
	StateReturnValue value;
	const StateReturnValue state;
};

#endif