#include "GameEvent.h"


CGameEvent::CGameEvent()
{
	eventId = -1;
	eventParams = new std::vector<float>();
}

CGameEvent::CGameEvent(const int eventId)
{
	this->eventId = eventId;	
	eventParams = new std::vector<float>();
}

CGameEvent::~CGameEvent()
{
	if (eventParams)
	{
		eventParams->clear();
		delete eventParams;
	}
}

void CGameEvent::addParam(const float param)
{
	eventParams->push_back(param);
}

void CGameEvent::addParam(const float param, const int paramInd)
{
	if ((int)eventParams->size() <= paramInd) return;

	(*eventParams)[paramInd] = param;
}

const int CGameEvent::getEventId()const
{
	return eventId;
}

const float CGameEvent::getParam(const int paramInd)const
{
	int ind = paramInd;
	if (ind >= (int)eventParams->size()) ind = eventParams->size() - 1;
	return (*eventParams)[ind];
}

const int CGameEvent::getParamCnt()const
{
	return (int)eventParams->size();
}
