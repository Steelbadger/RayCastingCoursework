#ifndef __PAUSEDSTATE_H__
#define __PAUSEDSTATE_H__

class PausedState : public GameState
{
public:
	PausedState();
	virtual ~PausedState();
	
	virtual void Initialise();
	virtual void Update();
	virtual void Render();
	
	
	
	


};

#endif