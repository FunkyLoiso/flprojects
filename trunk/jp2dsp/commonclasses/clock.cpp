#include "clock.h"

Clock::Clock()
{
	paused = true;
	//LARGE_INTEGER *pf = &(this->freq);
	QueryPerformanceFrequency(&freq);
	//pf->QuadPart = -15;
}

void Clock::Set(int time_ms)
{
	set_ms = time_ms;
	QueryPerformanceCounter(&set);
	lastPause = last = set;
}

void Clock::Go()
{
	if(paused)
	{
		paused = false;
		set_ms -= getDiffFromNow_ms(lastPause);
	}
}

void Clock::Pause()
{
	if(!paused)
	{
		paused = true;
		QueryPerformanceCounter(&lastPause);
	}
}

int Clock::Get_ms()
{
	return set_ms + getDiffFromNow_ms(set);
}

int Clock::getDiffFromNow_ms(LARGE_INTEGER first)
{
	QueryPerformanceCounter(&last);
	return  (last.QuadPart - first.QuadPart)/(freq.QuadPart/1000);
}
