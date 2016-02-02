#define _CRT_SECURE_NO_WARNINGS
#include "AppTime.h"

#include <ctime>

CAppTime* CAppTime::instance = 0;

CAppTime::CAppTime()
{
	resetTime();
}


CAppTime::~CAppTime()
{

}

void CAppTime::resetTime()
{
	currTime = 0.0f;
}

const float CAppTime::getNow() const
{
	return currTime;
}

void CAppTime::addDTime(const float dt)
{
	currTime += dt;
}

CAppTime* CAppTime::getInstance()
{
	if (instance == 0)
	{
		instance = new CAppTime();
		return instance;
	}
	return instance;
}

const std::string CAppTime::getLocalTime() const
{
	time_t timeHandle;
	struct tm * timeinfo;
	time(&timeHandle);
	timeinfo = localtime(&timeHandle);

	std::string timeStr = asctime(timeinfo);
	timeStr = timeStr.substr(0, timeStr.length() - 1);
	return timeStr;
}
