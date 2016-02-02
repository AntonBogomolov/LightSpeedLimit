#pragma once
#include "Timer.h"

#include <vector>
#include <string>

class CFrameTimer :
	public CTimer
{
private:
	unsigned long _nframes;
	unsigned long _print_nframes;
	unsigned long _frame_interval;
	unsigned long _print_interval;
	unsigned long _last_update;
	unsigned long _update_interval;
	float _fps;
	double dt;

	std::string printStr;
	std::vector<float>* fpsList;
	bool needToCollectFps;
public:
	void addFrame();
	void reset();	
	const long getFrame()const;
	const long getInterval()const;
	const float getFPS()const;
	const double getDT()const;
	std::vector<float>* getFpsList()const;
	const int getFpsListSize()const;
	void clearFpsList();
	void setPrintString(const std::string str);
	const float getAverageFps(const int sec)const;
	const bool isNeedToCollectFps()const;
	void setNeedToCollectFps(const bool needToCollectFps);

	CFrameTimer(const unsigned long print_interval, const unsigned long update_interval);
	~CFrameTimer();
};

