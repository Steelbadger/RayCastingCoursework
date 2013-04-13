#include "animatedsprite.h"
#include <iostream>


AnimatedSprite::AnimatedSprite(): PS2SpriteT()
{
	Initialize();
}
	
AnimatedSprite::AnimatedSprite(const float x, const float y): PS2SpriteT(x, y)
{
	Initialize();
}	

AnimatedSprite::AnimatedSprite(const float x, const float y, const float z): PS2SpriteT(x, y, z)
{
	Initialize();
}

AnimatedSprite::AnimatedSprite(const float x, const float y, const float width, const float height): PS2SpriteT(x, y, width, height)
{
	Initialize();
}

AnimatedSprite::AnimatedSprite(const float x, const float y, const float width, const float height, const int frameBase): PS2SpriteT(x, y, width, height)
{
	Initialize();
	baseFrame = frameBase;
}

AnimatedSprite::~AnimatedSprite()
{
}

void AnimatedSprite::Update()
{
	(this->*CurrentUpdate)();
}

void AnimatedSprite::SetAnimationRate(float delay)
{
	frameDelay = delay;
	timer.Initialise();
}

void AnimatedSprite::AnimateOnce()
{
	CurrentUpdate = &AnimatedSprite::UpdateAnimateOnce;
	animating = true;
}

void AnimatedSprite::AnimateLoop()
{
	CurrentUpdate = &AnimatedSprite::UpdateAnimateLoop;
	animating = true;
}

void AnimatedSprite::AnimateCycleReverse()
{
	CurrentUpdate = &AnimatedSprite::UpdateAnimateCycle;
	animating = true;
}

void AnimatedSprite::Initialize()
{
	CurrentUpdate = &AnimatedSprite::UpdateStatic;
	animating = false;
	frame = 0;
	baseFrame = 0;
	reverse = false;
}

void AnimatedSprite::UpdateStatic()
{
//  Do nothing!	
}

void AnimatedSprite::UpdateAnimateOnce()
{
	delayCount += timer.GetTimeDeltaSeconds();
	if (delayCount > frameDelay) {
		if (frame < totalFrames) {
			frame++;	
		} else {
			frame = 0;
			animating = false;
			CurrentUpdate = &AnimatedSprite::UpdateStatic;
		}
		int u = ((frame+baseFrame)*m_tw)%256;
		int v = ((frame+baseFrame)*m_th)/256;
		SetUVs(u, v, m_tw, m_th);
		delayCount = 0;		
	}
}

void AnimatedSprite::UpdateAnimateLoop()
{
	delayCount += timer.GetTimeDeltaSeconds();
	if (delayCount > frameDelay) {
		if (frame < totalFrames) {
			frame++;	
		} else {
			frame = 0;
		}
		int u = ((frame+baseFrame)*m_tw)%256;
		int v = ((frame+baseFrame)*m_th)/256;
		SetUVs(u, v, m_tw, m_th);	
		delayCount = 0;
	}
	
}

void AnimatedSprite::UpdateAnimateCycle()
{
	delayCount += timer.GetTimeDeltaSeconds();
	if (delayCount > frameDelay) {	
		if (!reverse) {
			frame++;	
		} else {
			frame--;
		}
		if (frame == 0 || frame == (totalFrames-1)) {
			reverse = !reverse;
		}
		int u = ((frame+baseFrame)*m_tw)%256;
		int v = ((frame+baseFrame)*m_th)/256;
		SetUVs(u, v, m_tw, m_th);	
		delayCount = 0;
	}
}

void AnimatedSprite::ForceStopAnimation()
{
	animating = false;
	CurrentUpdate = &AnimatedSprite::UpdateStatic;
}

