#include <sps2lib.h>
#include <sps2tags.h>
#include <sps2util.h>
#include "timer.h"


CTimer::CTimer(void)
{
}

CTimer::~CTimer(void)
{
}

void CTimer::Initialise(void)
{
	m_LastTime = *EE_T3_COUNT;
	m_VideoMode = sps2UScreenGetVideoMode();
}

void CTimer::PrimeTimer(void)
{
		m_LastTime = *EE_T3_COUNT;
}


unsigned int CTimer::GetTimeDelta(void)
{
	m_Time = *EE_T3_COUNT;
	
	
	if(m_Time < m_LastTime)// The counter has wrapped
	{
		m_Change = m_Time + (65536 - m_LastTime);
	}
	else
	{
		m_Change = m_Time - m_LastTime;
	}

	m_LastTime = m_Time;
	
	return m_Change;
}


// The numbers in this function are the horizontal blank rates in Hz
// See Page 84 of the GS Users manual

float CTimer::GetFPS(void)
{
	unsigned int TimeDelta = GetTimeDelta();
	
	if(TimeDelta == 0)return -1.0f;
	
	switch(m_VideoMode)
	{
		case SPS2U_SCR_VESA:
			return 31469.0f / (float)TimeDelta;	
	
		case SPS2U_SCR_PAL:
			return 15625.0f / (float)TimeDelta;
	
		case SPS2U_SCR_NTSC:
			return 15734.0f / (float)TimeDelta;
	
		default:
			return 0.0f;
	}
}

float CTimer::GetTimeDeltaSeconds(void)
{
	return (1.0f/GetFPS());
}
