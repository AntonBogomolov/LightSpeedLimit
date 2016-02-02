#pragma once

class CGameEvent;

class IGameEventHandler
{
public:
	virtual void appEventHandler(const CGameEvent* event) = 0;
public:
	IGameEventHandler();
	virtual ~IGameEventHandler();
};

