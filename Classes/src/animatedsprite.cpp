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
//	std::cout << "Animate Once, StartFrame: " << frame << "\tBaseFrame: " << baseFrame << std::endl;
}

void AnimatedSprite::AnimateOnceAndStop()
{
	CurrentUpdate = &AnimatedSprite::UpdateAnimateOnceAndStop;
	animating = true;
}

void AnimatedSprite::AnimateLoop()
{
	CurrentUpdate = &AnimatedSprite::UpdateAnimateLoop;
	animating = true;
	loopsLeft = -1;
}

void AnimatedSprite::AnimateLoop(int loops)
{
	CurrentUpdate = &AnimatedSprite::UpdateAnimateLoop;
	animating = true;
	loopsLeft = loops;
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
		if (frame < (totalFrames-1)) {
			frame++;	
		} else {
			frame = 0;
			animating = false;
			CurrentUpdate = &AnimatedSprite::UpdateStatic;
		}

		int u = ((frame+baseFrame)*m_tw)%256;
		int v = int(((frame+baseFrame)*m_tw)/256) * m_th;
		
//		std::cout << "Frame: " << frame << "\tUVs: (" << u << "," << v << ")" << std::endl;		
		SetUVs(u, v, m_tw, m_th);
		delayCount = 0;		
	}
}

void AnimatedSprite::UpdateAnimateOnceAndStop()
{
	delayCount += timer.GetTimeDeltaSeconds();
	if (delayCount > frameDelay) {
		if (frame < (totalFrames-1)) {
			frame++;	
		} else {
			animating = false;
			CurrentUpdate = &AnimatedSprite::UpdateStatic;
		}

		int u = ((frame+baseFrame)*m_tw)%256;
		int v = int(((frame+baseFrame)*m_tw)/256) * m_th;

		SetUVs(u, v, m_tw, m_th);
		delayCount = 0;		
	}
}

void AnimatedSprite::UpdateAnimateLoop()
{
	delayCount += timer.GetTimeDeltaSeconds();
	if (delayCount > frameDelay) {
		int u = ((frame+baseFrame)*m_tw)%256;
		int v = int(((frame+baseFrame)*m_tw)/256) * m_th;
		SetUVs(u, v, m_tw, m_th);	
		delayCount = 0;
//		std::cout << "Draw Frame: " << frame << std::endl;
		if (frame < (totalFrames-1)) {
			frame++;	
		} else {
			frame = 0;
			loopsLeft--;			
			if (loopsLeft == 0) {
				CurrentUpdate = &AnimatedSprite::UpdateStatic;
				animating = false;
			}			
		}		
	}
	
}


void AnimatedSprite::UpdateAnimateCycle()
{
	delayCount += timer.GetTimeDeltaSeconds();
	if (delayCount > frameDelay) {	
		int u = ((frame+baseFrame)*m_tw)%256;
		int v = int(((frame+baseFrame)*m_tw)/256) * m_th;
		SetUVs(u, v, m_tw, m_th);	
		delayCount = 0;
		if (!reverse) {
			frame++;	
		} else {
			frame--;
		}
		if (frame == 0 || frame == (totalFrames-1)) {
			reverse = !reverse;
		}			
	}
}

void AnimatedSprite::ForceStopAnimation()
{
	animating = false;
	CurrentUpdate = &AnimatedSprite::UpdateStatic;
}

void AnimatedSprite::ResetAnimationAndStop()
{
	animating = false;
	frame = 0;
	int u = ((frame+baseFrame)*m_tw)%256;
	int v = int(((frame+baseFrame)*m_tw)/256) * m_th;
	SetUVs(u, v, m_tw, m_th);	
	delayCount = 12;
	CurrentUpdate = &AnimatedSprite::UpdateStatic;
}

