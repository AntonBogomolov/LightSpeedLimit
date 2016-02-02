#pragma once

#include <vector>

class CGameEvent
{
private:
	int eventId;
	
	std::vector<float>* eventParams;

public:
	void addParam(const float param);
	void addParam(const float param, const int paramInd);
	const int getEventId()const;
	const float getParam(const int paramInd)const;
	const int   getParamCnt()const;

	CGameEvent();
	CGameEvent(const int eventId);
	~CGameEvent();
};

