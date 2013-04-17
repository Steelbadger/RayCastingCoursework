#ifndef __ANIMATEDSPRITE_H__
#define __ANIMATEDSPRITE_H__
//////////////////////--By Ross Davies--//////////////////////
/*		The Animated Sprite class inherits from the 
		textured sprite class, this class has functions
		for animating just once then stopping, looping
		a set number, or endlessly and cycling forward 
		and back endlessly.  There are also functions
		for resetting an animation to the first frame
		and for stopping the animation entirely.			*/
//////////////////////////////////////////////////////////////

#include "primitives.h"
#include "timer.h"

class AnimatedSprite : public PS2SpriteT
{
public:

	//  Constructors for various mixtures of data
	AnimatedSprite();	
	AnimatedSprite(const float x, const float y);	
	AnimatedSprite(const float x, const float y, const float z);
	AnimatedSprite(const float x, const float y, const float width, const float height);
	AnimatedSprite(const float x, const float y, const float width, const float height, const int frameBase);
	
	//  Destructor (does nothing special)
	virtual ~AnimatedSprite();
	
	//  Setters for important animation data
	void SetNumberOfFrames(int frames){totalFrames = frames;}
	void SetAnimationRate(float delay);
	
	//  Check the state of the sprite
	bool IsAnimating(){return animating;}
	
	//  Animation functions
	void AnimateOnce();
	void AnimateOnceAndStop();
	void AnimateLoop();
	void AnimateLoop(int loops);
	void AnimateCycleReverse();
	void ForceStopAnimation();
	void ResetAnimationAndStop();
	
	//  Update animation frames
	void Update();
	
	//  Change the base frame from which the frame counter increments
	//  Allows us to stop multiple animations on the same sprite sheet
	//  while using the same update functions
	void SetBase(int base){baseFrame = base;}

private:

	//  Do initial setup
	void Initialize();

	//  Function pointer to correct update function
	void (AnimatedSprite::*CurrentUpdate)();
	
	//  Update functions for different types of animation call
	void UpdateStatic();
	void UpdateAnimateOnce();
	void UpdateAnimateOnceAndStop();
	void UpdateAnimateLoop();
	void UpdateAnimateCycle();
	
	
	//  private data
	int frame;
	int baseFrame;
	bool animating;
	bool reverse;
	int totalFrames;
	float frameDelay;
	float delayCount;
	int loopsLeft;
	CTimer timer;
};



#endif