#include "InputManager.h"
#include "global.h"
#include "VideoManeger.h"
#include "GLUtils.h"
#include "GameEvent.h"
#include "EventsManager.h"
#include "Camera.h"
#include "IInputHandler.h"
#include "InputEvent.h"
#include "SyncHelper.h"
#include "Log.h"

CInputManager* CInputManager::instance = 0;

CInputManager::CInputManager()
{
//	m_keymap = NULL;

	eventHandlers = new std::map<unsigned int, std::vector<IInputHandler*>>();
	std::vector< IInputHandler*> blankVector;
	eventHandlers->insert(std::pair<unsigned int, std::vector<IInputHandler*>>(CInputEvent::IEVENT_KEY_DOWN, blankVector));
	eventHandlers->insert(std::pair<unsigned int, std::vector<IInputHandler*>>(CInputEvent::IEVENT_KEY_UP, blankVector));
	eventHandlers->insert(std::pair<unsigned int, std::vector<IInputHandler*>>(CInputEvent::IEVENT_MOUSE_KEYDOWN, blankVector));
	eventHandlers->insert(std::pair<unsigned int, std::vector<IInputHandler*>>(CInputEvent::IEVENT_MOUSE_KEYUP, blankVector));
	eventHandlers->insert(std::pair<unsigned int, std::vector<IInputHandler*>>(CInputEvent::IEVENT_MOUSE_MOVE, blankVector));
	eventHandlers->insert(std::pair<unsigned int, std::vector<IInputHandler*>>(CInputEvent::IEVENT_MOUSE_WHEEL, blankVector));
	eventHandlers->insert(std::pair<unsigned int, std::vector<IInputHandler*>>(CInputEvent::IEVENT_WINDOW, blankVector));
}

CInputManager::~CInputManager()
{
//	if (m_keymap)delete m_keymap;
	if (eventHandlers)delete eventHandlers;
}

void  CInputManager::SDLEvents(float dt)
{
	MyGUI::InputManager* guiInputManager = MyGUI::InputManager::getInstancePtr();
	if (guiInputManager == NULL)
	{
		CLog::getInstance()->addWarning("CInputManager::SDLEvents() \t| guiInputManager == NULL");
	}

	SDL_Event& e = CVideoManeger::GetInstance()->event;
	if (SDL_WaitEvent(&e))
	{		
		if (&e == NULL) return;
		
		switch (e.type)
		{
		case SDL_QUIT:
			inputEvent.clearEventData();
			inputEvent.eventId = SDL_WINDOWEVENT;
			inputEvent.window.windowEvent = SDL_QUIT;			
			processInputEvent(&inputEvent, CInputEvent::IEVENT_WINDOW);
		break;
		case SDL_MOUSEMOTION:
			CSyncHelper::getInstance()->getInputMutex()->lock();
			guiInputManager->injectMouseMove(e.motion.x, e.motion.y, 0);
			CSyncHelper::getInstance()->getInputMutex()->unlock();

			inputEvent.clearEventData();
			inputEvent.eventId = SDL_MOUSEMOTION;
			inputEvent.mouse.coords[0] = e.motion.x;
			inputEvent.mouse.coords[1] = e.motion.y;
			processInputEvent(&inputEvent, CInputEvent::IEVENT_MOUSE_MOVE);
		break;
		case SDL_MOUSEBUTTONDOWN:
			EventsMouseBDown(e.motion.x, e.motion.y, e.button.button);
		break;
		case SDL_MOUSEBUTTONUP:
			EventsMouseBUp(e.motion.x, e.motion.y, e.button.button);
		break;
		case SDL_MOUSEWHEEL:
			inputEvent.clearEventData();
			inputEvent.eventId = SDL_MOUSEWHEEL;
			inputEvent.mouse.wheelDelta = e.wheel.y;
			processInputEvent(&inputEvent, CInputEvent::IEVENT_MOUSE_WHEEL);
		break;
		case SDL_WINDOWEVENT:
			if (e.window.event == SDL_WINDOWEVENT_RESIZED)
			{
//				if (isCEGUIEnabled)	CEGUI::System::getSingleton().notifyDisplaySizeChanged(CEGUI::Sizef(e.window.data1, e.window.data2));
				
				inputEvent.clearEventData();
				inputEvent.eventId = SDL_WINDOWEVENT;
				inputEvent.window.windowEvent = SDL_WINDOWEVENT_RESIZED;
				inputEvent.window.data1 = e.window.data1;
				inputEvent.window.data2 = e.window.data2;
				processInputEvent(&inputEvent, CInputEvent::IEVENT_WINDOW);
			}
		break;
		case SDL_KEYDOWN:
//			if (isCEGUIEnabled)context->injectKeyDown((*m_keymap)[e.key.keysym.sym]);
//			guiInputManager->injectKeyPress(e.key.keysym.sym);

			keyHandler.setKeyState(e.key.keysym.sym, true);

			inputEvent.clearEventData();
			inputEvent.eventId = SDL_KEYDOWN;
			inputEvent.keyboard.keyboardEvent = e.key.keysym.sym;
			processInputEvent(&inputEvent, CInputEvent::IEVENT_KEY_DOWN);		
		break;

		case SDL_KEYUP:
//			if (isCEGUIEnabled)context->injectKeyUp((*m_keymap)[e.key.keysym.sym]);
			
			keyHandler.setKeyState(e.key.keysym.sym, false);

			inputEvent.clearEventData();
			inputEvent.eventId = SDL_KEYUP;
			inputEvent.keyboard.keyboardEvent = e.key.keysym.sym;			
			processInputEvent(&inputEvent, CInputEvent::IEVENT_KEY_UP);
		break;
		}
	}
}

void  CInputManager::EventsMouseBUp(int x, int y, unsigned short button)
{	
	MyGUI::InputManager* guiInputManager = MyGUI::InputManager::getInstancePtr();
	CSyncHelper::getInstance()->getInputMutex()->lock();
	switch (button)
	{
	case SDL_BUTTON_LEFT:
		guiInputManager->injectMouseRelease(x, y, MyGUI::MouseButton::Enum::Left);
		break;
	case SDL_BUTTON_MIDDLE:
		guiInputManager->injectMouseRelease(x, y, MyGUI::MouseButton::Enum::Middle);
		break;
	case SDL_BUTTON_RIGHT:
		guiInputManager->injectMouseRelease(x, y, MyGUI::MouseButton::Enum::Right);
		break;
	}
	CSyncHelper::getInstance()->getInputMutex()->unlock();

	inputEvent.clearEventData();
	inputEvent.eventId = SDL_MOUSEBUTTONUP;
	inputEvent.mouse.button = button;
	processInputEvent(&inputEvent, CInputEvent::IEVENT_MOUSE_KEYUP);
}

void  CInputManager::EventsMouseBDown(int x, int y, unsigned short button)
{
	MyGUI::InputManager* guiInputManager = MyGUI::InputManager::getInstancePtr();
	CSyncHelper::getInstance()->getInputMutex()->lock();
	switch (button)
	{
	case SDL_BUTTON_LEFT:
		guiInputManager->injectMousePress(x, y, MyGUI::MouseButton::Enum::Left);
		break;
	case SDL_BUTTON_MIDDLE:
		guiInputManager->injectMousePress(x, y, MyGUI::MouseButton::Enum::Middle);
		break;
	case SDL_BUTTON_RIGHT:
		guiInputManager->injectMousePress(x, y, MyGUI::MouseButton::Enum::Right);
		break;
	}
	CSyncHelper::getInstance()->getInputMutex()->unlock();

	inputEvent.clearEventData();
	inputEvent.eventId = SDL_MOUSEBUTTONDOWN;
	inputEvent.mouse.button = button;
	processInputEvent(&inputEvent, CInputEvent::IEVENT_MOUSE_KEYDOWN);
}

void CInputManager::processInputEvent(const CInputEvent* inputEvent, const int eventCode) const
{
	std::vector<IInputHandler*> *currEventHandlers;
	currEventHandlers = &(*eventHandlers)[eventCode];

	for (auto it = currEventHandlers->begin(); it != currEventHandlers->end(); it++)
	{
		IInputHandler* currHandler = (*it);
		currHandler->inputEventHandler(*inputEvent);
		if (currHandler->getIsCapturedEvent()) break;
	}
}

bool CInputManager::isEventHandlerExistOnCurrentEvent(const IInputHandler* handler, const int eventCode) const
{
	std::vector<IInputHandler*> *currEventHandlers;
	currEventHandlers = &(*eventHandlers)[eventCode];

	for (auto it = currEventHandlers->begin(); it != currEventHandlers->end(); it++)
	{
		if ((*it) == handler)	return true;		
	}

	return false;
}

bool CInputManager::isEventHandlerExist(const IInputHandler* handler, const int eventMask)const
{
	if (eventMask & CInputEvent::IEVENT_KEY_DOWN 		&& isEventHandlerExistOnCurrentEvent(handler, CInputEvent::IEVENT_KEY_DOWN))		return true;
	if (eventMask & CInputEvent::IEVENT_KEY_UP 			&& isEventHandlerExistOnCurrentEvent(handler, CInputEvent::IEVENT_KEY_UP))			return true;
	if (eventMask & CInputEvent::IEVENT_MOUSE_KEYDOWN 	&& isEventHandlerExistOnCurrentEvent(handler, CInputEvent::IEVENT_MOUSE_KEYDOWN))	return true;
	if (eventMask & CInputEvent::IEVENT_MOUSE_KEYUP 	&& isEventHandlerExistOnCurrentEvent(handler, CInputEvent::IEVENT_MOUSE_KEYUP))		return true;
	if (eventMask & CInputEvent::IEVENT_MOUSE_MOVE 		&& isEventHandlerExistOnCurrentEvent(handler, CInputEvent::IEVENT_MOUSE_MOVE))		return true;
	if (eventMask & CInputEvent::IEVENT_MOUSE_WHEEL 	&& isEventHandlerExistOnCurrentEvent(handler, CInputEvent::IEVENT_MOUSE_WHEEL))		return true;
	if (eventMask & CInputEvent::IEVENT_WINDOW 			&& isEventHandlerExistOnCurrentEvent(handler, CInputEvent::IEVENT_WINDOW))			return true;
	
	return false;
}

void CInputManager::addEventHandlerToCurrentEvent(IInputHandler* handler, const int eventCode)
{
	if (isEventHandlerExist(handler, eventCode)) return;

	std::vector<IInputHandler*> *currEventHandlers;	
	currEventHandlers = &(*eventHandlers)[eventCode];
	if (currEventHandlers->size() == 0)
	{
		currEventHandlers->push_back(handler);
		return;
	}

	IInputHandler* maxPriorHandler = currEventHandlers->back();
	if (maxPriorHandler->getPriority() < handler->getPriority())
	{
		currEventHandlers->push_back(handler);
	}
	else
	{
		IInputHandler* oldHandler = currEventHandlers->back();
		currEventHandlers->back() = handler;
		currEventHandlers->push_back(oldHandler);
	}
}

void CInputManager::addEventHandler(IInputHandler* handler, const int eventMask)
{
	if (eventMask & CInputEvent::IEVENT_KEY_DOWN)		addEventHandlerToCurrentEvent(handler, CInputEvent::IEVENT_KEY_DOWN);
	if (eventMask & CInputEvent::IEVENT_KEY_UP)			addEventHandlerToCurrentEvent(handler, CInputEvent::IEVENT_KEY_UP);
	if (eventMask & CInputEvent::IEVENT_MOUSE_KEYDOWN)	addEventHandlerToCurrentEvent(handler, CInputEvent::IEVENT_MOUSE_KEYDOWN);
	if (eventMask & CInputEvent::IEVENT_MOUSE_KEYUP)	addEventHandlerToCurrentEvent(handler, CInputEvent::IEVENT_MOUSE_KEYUP);
	if (eventMask & CInputEvent::IEVENT_MOUSE_MOVE)		addEventHandlerToCurrentEvent(handler, CInputEvent::IEVENT_MOUSE_MOVE);
	if (eventMask & CInputEvent::IEVENT_MOUSE_WHEEL)	addEventHandlerToCurrentEvent(handler, CInputEvent::IEVENT_MOUSE_WHEEL);
	if (eventMask & CInputEvent::IEVENT_WINDOW)			addEventHandlerToCurrentEvent(handler, CInputEvent::IEVENT_WINDOW);
}

void CInputManager::removeEventHandlerFromCurrentEvent(IInputHandler* handler, const int eventCode)
{
	std::vector<IInputHandler*> *currEventHandlers;
	currEventHandlers = &(*eventHandlers)[eventCode];

	for (auto it = currEventHandlers->begin(); it != currEventHandlers->end(); it++)
	{
		if ((*it) == handler)
		{
			currEventHandlers->erase(it);
			break;
		}
	}
}

void CInputManager::removeEventHandler(IInputHandler* handler, const int eventMask)
{
	if (eventMask & CInputEvent::IEVENT_KEY_DOWN)		removeEventHandlerFromCurrentEvent(handler, CInputEvent::IEVENT_KEY_DOWN);
	if (eventMask & CInputEvent::IEVENT_KEY_UP)			removeEventHandlerFromCurrentEvent(handler, CInputEvent::IEVENT_KEY_UP);
	if (eventMask & CInputEvent::IEVENT_MOUSE_KEYDOWN)	removeEventHandlerFromCurrentEvent(handler, CInputEvent::IEVENT_MOUSE_KEYDOWN);
	if (eventMask & CInputEvent::IEVENT_MOUSE_KEYUP)	removeEventHandlerFromCurrentEvent(handler, CInputEvent::IEVENT_MOUSE_KEYUP);
	if (eventMask & CInputEvent::IEVENT_MOUSE_MOVE)		removeEventHandlerFromCurrentEvent(handler, CInputEvent::IEVENT_MOUSE_MOVE);
	if (eventMask & CInputEvent::IEVENT_MOUSE_WHEEL)	removeEventHandlerFromCurrentEvent(handler, CInputEvent::IEVENT_MOUSE_WHEEL);
	if (eventMask & CInputEvent::IEVENT_WINDOW)			removeEventHandlerFromCurrentEvent(handler, CInputEvent::IEVENT_WINDOW);
}

const CKeyHandler* CInputManager::getKeysStates() const
{
	return &keyHandler;
}

CInputManager* CInputManager::getInstance()
{
	if (instance == NULL)
	{
		instance = new CInputManager();
	}
	return instance;
}

////////////////////////////////////////////////////////

CKeyHandler::CKeyHandler()
{
	keysState.clear();
}

CKeyHandler::~CKeyHandler()
{

}

void CKeyHandler::setKeyState(const int keyCode, const bool state)
{
	CSyncHelper::getInstance()->getInputMutex()->lock();
	keysState[keyCode] = state;
	CSyncHelper::getInstance()->getInputMutex()->unlock();
}

bool CKeyHandler::isPressed(const int keyCode) const
{			
	CSyncHelper::getInstance()->getInputMutex()->lock();
	bool result = keysState[keyCode];
	CSyncHelper::getInstance()->getInputMutex()->unlock();

	return result;
}