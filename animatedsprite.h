#ifndef __ANIMATEDSPRITE_H__
#define __ANIMATEDSPRITE_H__

#include "primitives.h"

class AnimatedSprite : public PS2SpriteT
{
public:
	AnimatedSprite();	
	AnimatedSprite(const float x, const float y);	
	AnimatedSprite(const float x, const float y, const float z);
	AnimatedSprite(const float x, const float y, const float width, const float height);
	AnimatedSprite(const float x, const float y, const float width, const float height, const int frameBase);
	~AnimatedSprite();
	void SetNumberOfFrames(int frames){totalFrames = frames;}
	void SetAnimationRate(int updates);
	bool IsAnimating(){return animating;}
	void AnimateOnce();
	void AnimateLoop();
	void AnimateCycleReverse();
	void ForceStopAnimation();
	void Update();

private:
	void Initialize();

	void (AnimatedSprite::*CurrentUpdate)();
	
	void UpdateStatic();
	void UpdateAnimateOnce();
	void UpdateAnimateLoop();
	void UpdateAnimateCycle();
	
	int frame;
	int baseFrame;
	int count;
	bool animating;
	bool reverse;
	int totalFrames;
	int frameDelay;
};



#endif