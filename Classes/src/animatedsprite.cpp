#include "animatedsprite.h"
#include <iostream>

//  Constructors for various initialisation values

//  default
AnimatedSprite::AnimatedSprite(): PS2SpriteT()
{
	Initialize();
}

//  Just a screen position
AnimatedSprite::AnimatedSprite(const float x, const float y): PS2SpriteT(x, y)
{
	Initialize();
}	

//  Screen position and depth
AnimatedSprite::AnimatedSprite(const float x, const float y, const float z): PS2SpriteT(x, y, z)
{
	Initialize();
}

//  screen position and width/height
AnimatedSprite::AnimatedSprite(const float x, const float y, const float width, const float height): PS2SpriteT(x, y, width, height)
{
	Initialize();
}

//  Screen position, width, height and the base frame of the animation
AnimatedSprite::AnimatedSprite(const float x, const float y, const float width, const float height, const int frameBase): PS2SpriteT(x, y, width, height)
{
	Initialize();
	baseFrame = frameBase;
}

//  Default destructor
AnimatedSprite::~AnimatedSprite()
{
}

void AnimatedSprite::Update()
//  Call the requisite update function (based on the kind of animation
//  currently active
{
	(this->*CurrentUpdate)();
}

void AnimatedSprite::SetAnimationRate(float delay)
//  Set the delay between frames (and initialise the timer)
{
	frameDelay = delay;
	timer.Initialise();
}

void AnimatedSprite::AnimateOnce()
//  Make the sprite animate once then 
//	reset to the first frame, make the current 
//	update function pointer point to the
//	animate once update function
{
	CurrentUpdate = &AnimatedSprite::UpdateAnimateOnce;
	animating = true;
}

void AnimatedSprite::AnimateOnceAndStop()
//  Make the sprite animate once then 
//	stay on the last frame, make the current 
//	update function pointer point to the
//	animate once and stop update function
{
	CurrentUpdate = &AnimatedSprite::UpdateAnimateOnceAndStop;
	animating = true;
}

void AnimatedSprite::AnimateLoop()
//  Make the sprite animate in an infinite loop
{
	CurrentUpdate = &AnimatedSprite::UpdateAnimateLoop;
	animating = true;
	loopsLeft = -1;
}

void AnimatedSprite::AnimateLoop(int loops)
//  Make the sprite loop loops number of times
//  negative numbers animate forever
{
	CurrentUpdate = &AnimatedSprite::UpdateAnimateLoop;
	animating = true;
	loopsLeft = loops;
}

void AnimatedSprite::AnimateCycleReverse()
//  Animate in a cycle (forwards-back-forwards etc)
{
	CurrentUpdate = &AnimatedSprite::UpdateAnimateCycle;
	animating = true;
}

void AnimatedSprite::Initialize()
//  Set initial values
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
//  Update function that is called when the sprite is to animate just
// 	once
{
	//  equalize the animation framerate with a timer
	delayCount += timer.GetTimeDeltaSeconds();
	if (delayCount > frameDelay) {
		//  If we're not on the last frame increment the frame
		//  else set to first frame and disable the animation
		if (frame < (totalFrames-1)) {
			frame++;	
		} else {
			frame = 0;
			animating = false;
			CurrentUpdate = &AnimatedSprite::UpdateStatic;
		}
		
		//  Set the UVs based on the frame we're currently on
		int u = ((frame+baseFrame)*m_tw)%256;
		int v = int(((frame+baseFrame)*m_tw)/256) * m_th;
		
		SetUVs(u, v, m_tw, m_th);
		
		//  reset the delay counter
		delayCount = 0;		
	}
}

void AnimatedSprite::UpdateAnimateOnceAndStop()
//  Very similar to UpdateAnimateOnce function
//  difference is doesn't reset to first frame on completion
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
//  Loops the animation until specified number of loops
//  have been performed.
{
	//  Equalise framerate using timer
	delayCount += timer.GetTimeDeltaSeconds();
	if (delayCount > frameDelay) {
		//  Set The UVs for the current frame
		int u = ((frame+baseFrame)*m_tw)%256;
		int v = int(((frame+baseFrame)*m_tw)/256) * m_th;
		SetUVs(u, v, m_tw, m_th);	
		delayCount = 0;
		
		//  Increment to next frame if we're in the middle of the animation
		//  else reset to first frame, if we're at the end of the animation
		//  and have looped the required number of times then disable the
		//  animation
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
//  Similar to loop animation, only cycles forward and back down animation chain
//  rather than looping from beginning to end
{
	//  Equalise framerate using timer
	delayCount += timer.GetTimeDeltaSeconds();
	if (delayCount > frameDelay) {
		// Set UVs based on current frame
		int u = ((frame+baseFrame)*m_tw)%256;
		int v = int(((frame+baseFrame)*m_tw)/256) * m_th;
		SetUVs(u, v, m_tw, m_th);	
		delayCount = 0;
		
		//  If we're going forward increment frame
		//  else we're going backwards decrement frame
		if (!reverse) {
			frame++;	
		} else {
			frame--;
		}
		
		//  flip the direction if we've reached an extremety
		if (frame == 0 || frame == (totalFrames-1)) {
			reverse = !reverse;
		}			
	}
}

void AnimatedSprite::ForceStopAnimation()
//  Stop the animation on the current frame
{
	animating = false;
	CurrentUpdate = &AnimatedSprite::UpdateStatic;
}

void AnimatedSprite::ResetAnimationAndStop()
//  Reset the animation to the first frame and stop it
{
	animating = false;
	frame = 0;
	int u = ((frame+baseFrame)*m_tw)%256;
	int v = int(((frame+baseFrame)*m_tw)/256) * m_th;
	SetUVs(u, v, m_tw, m_th);	
	delayCount = 12;
	CurrentUpdate = &AnimatedSprite::UpdateStatic;
}

