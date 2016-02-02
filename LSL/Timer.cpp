#include "Timer.h"
#include "global.h"

CTimer::CTimer() : _running(false), _starttime(0), _stoptime(0), _total(0)
{
	_countPerSec = SDL_GetPerformanceFrequency();
}

void CTimer::Reset()
{
	_running = false;
	_starttime = _stoptime = SDL_GetPerformanceCounter();
	_total = 0;
}

void CTimer::Start()
{
	Reset();
	Continue();
}

void CTimer::Stop()
{
	_running = false;
	_stoptime = SDL_GetPerformanceCounter();
	_total += _stoptime - _starttime;
}

void CTimer::Continue()
{
	_running = true;
	_starttime = SDL_GetPerformanceCounter();
}

const double CTimer::GetInterval()const
{
	if (_running)
	{		
		double now = SDL_GetPerformanceCounter();
		double interv = double(now - _starttime) / double(_countPerSec);
		return interv;
	}
	else 
	{
		return double(_stoptime - _starttime) / double(_countPerSec);
	}
}

const double CTimer::GetTotal()const
{
	return ((double)_total / _countPerSec);
}

const Uint64 CTimer::GetNow()const
{
	return SDL_GetPerformanceCounter();
}