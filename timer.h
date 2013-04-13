#ifndef _TIMER_H_
#define _TIMER_H_


// Timer 3 uses the HBLANK to count 
// (its the only default timer that is slow enough 
// for us to sample in order to calculate the FPS)


class CTimer
{
public:
	CTimer();
	~CTimer();
	
	void 			Initialise(void);
	
	unsigned int 	GetTimeDelta(void);
	void 			PrimeTimer(void);
	float		 	GetFPS(void);
	float			GetTimeDeltaSeconds(void);
	
private:
	
	unsigned int m_LastTime;
	unsigned int m_Time;
	unsigned int m_Change;
	unsigned int m_VideoMode;
};


#endif

