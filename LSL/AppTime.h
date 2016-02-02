#pragma once

#include <string>

class CAppTime
{
private:
	float currTime;
public:
	static CAppTime* instance;

private:
	CAppTime();
public:
	void resetTime();
	const float getNow() const;
	const std::string getLocalTime() const;
	void addDTime(const float dt);

	static CAppTime* getInstance();
	virtual ~CAppTime();
};

