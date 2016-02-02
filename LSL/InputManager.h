#pragma once
#include <map>
#include <vector>

#include "InputEvent.h"

class IInputHandler;

class CKeyHandler
{
private:
	mutable std::map<int, bool> keysState;
public:
	void setKeyState(const int keyCode,const bool state);
	bool isPressed(const int keyCode) const;

	CKeyHandler();
	~CKeyHandler();
};

class CInputManager
{
private:
	static CInputManager* instance;
	CInputEvent inputEvent;
	CKeyHandler keyHandler;
	std::map<unsigned int, std::vector<IInputHandler*> >* eventHandlers;

private:
	bool isEventHandlerExist(const IInputHandler* handler, const int eventMask) const;
	bool isEventHandlerExistOnCurrentEvent(const IInputHandler* handler, const int eventCode) const;
	void addEventHandlerToCurrentEvent(IInputHandler* handler, const int eventCode);
	void removeEventHandlerFromCurrentEvent(IInputHandler* handler, const int eventCode);
	void processInputEvent(const CInputEvent* inputEvent, const int eventCode) const;

	CInputManager();
public:
	void SDLEvents(float dt = 0.0f);
	void EventsMouseBUp(int x, int y, unsigned short button);
	void EventsMouseBDown(int x, int y, unsigned short button);

	void addEventHandler(IInputHandler* handler, const int eventMask);
	void removeEventHandler(IInputHandler* handler, const int eventMask);

	const CKeyHandler* getKeysStates() const;

	static CInputManager* getInstance();
	~CInputManager();
};

