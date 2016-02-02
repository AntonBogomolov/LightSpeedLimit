#pragma once

#include <list>
#include"GameEvent.h"

class CEventsManager
{
private:
	static CEventsManager* instance;
	static std::list<CGameEvent*>* eventsList;

	CEventsManager();
public:
	static void cleanup();
	static void addEvent(CGameEvent* event);
	static CGameEvent* getEvent();

	static CEventsManager* getInstance();
	~CEventsManager();
};

