#include "EventsManager.h"
#include "SyncHelper.h"

std::list<CGameEvent*>* CEventsManager::eventsList = NULL;
CEventsManager* CEventsManager::instance = NULL;

CEventsManager::CEventsManager()
{
	eventsList = new std::list<CGameEvent*>;
}

CEventsManager::~CEventsManager()
{
	if (eventsList)
	{
		for each (CGameEvent* gameEvent in (*eventsList))
		{
			delete gameEvent;
		}
		eventsList->clear();
		delete eventsList;
	}
}

CEventsManager* CEventsManager::getInstance()
{
	if (instance == NULL)
	{
		instance = new CEventsManager();
	}
	return instance;
}

void CEventsManager::cleanup()
{
	if (eventsList)
	{
		for each (CGameEvent* gameEvent in (*eventsList))
		{
			delete gameEvent;
		}
		eventsList->clear();		
	}
}

void CEventsManager::addEvent(CGameEvent* event)
{
	CSyncHelper::getInstance()->getEventsMutex()->lock();
	eventsList->push_back(event);
	CSyncHelper::getInstance()->getEventsMutex()->unlock();
}

CGameEvent* CEventsManager::getEvent()
{		
	if (eventsList->empty())
	{
		return NULL;
	}

	CSyncHelper::getInstance()->getEventsMutex()->lock();
	CGameEvent* currEvent = eventsList->back();
	eventsList->pop_back();		
	CSyncHelper::getInstance()->getEventsMutex()->unlock();

	return currEvent;
}