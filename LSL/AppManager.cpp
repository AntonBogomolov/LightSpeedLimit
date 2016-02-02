#include "AppManager.h"
#include "global.h"
#include "VideoManeger.h"
#include "ResourceManager.h"
#include "GLUtils.h"
#include "EventsManager.h"
#include "InputManager.h"
#include "GameEvent.h"
#include "InputEvent.h"
#include "AppDefines.h"
#include "Log.h"

#include "Scene.h"
#include "MainMenu.h"
#include "StarSystemScene.h"

CAppManager::CAppManager(void) : CAppBase()
{		
	CInputManager::getInstance()->addEventHandler(this, CInputEvent::IEVENT_KEY_DOWN | CInputEvent::IEVENT_WINDOW);
}

CAppManager::~CAppManager(void)
{
	
}

void CAppManager::Init()
{
	__super::Init();	
}

void CAppManager::appEvents(const float dt)
{
	CGameEvent* currEvent;
	int newScrWidth;
	int newScrHeight;

	while ((currEvent = CEventsManager::getEvent()) != NULL)
	{
		switch (currEvent->getEventId())
		{
		case Defines::EV_WINDOW_RESIZED:
			newScrWidth  = (int)currEvent->getParam(0);
			newScrHeight = (int)currEvent->getParam(1);
			CVideoManeger::scrWidth  = newScrWidth;
			CVideoManeger::scrHeight = newScrHeight;			
			
			CVideoManeger::GetInstance()->uiPlatform->getRenderManagerPtr()->setViewSize(newScrWidth, newScrHeight);
			CLog::getInstance()->addInfo("Resize screen : " + std::to_string(newScrWidth) + "x" + std::to_string(newScrHeight));
			CGLUtils::resizeWindow();
			currScene->resizeScene();				
			break;
		case Defines::EV_UI_NEWGAME:
			changeScene(new CStarSystemScene());	
			break;
		case Defines::EV_UI_MENU:
			changeScene(new CMainMenu());
			break;
		case Defines::EV_APP_START:
			changeScene(new CMainMenu());
			break;
		case Defines::EV_APP_EXIT:
			CLog::getInstance()->addInfo("Normal app exit");
			quit = true;
			break;
		default:
			currScene->appEventHandler(currEvent);
			break;
		}
		if (currEvent) delete currEvent;
	}
}

void CAppManager::inputEventHandler(const CInputEvent event)
{
	CGameEvent* gEvent;

	switch (event.eventId)
	{
	case SDL_WINDOWEVENT:
		switch (event.window.windowEvent)
		{
		case SDL_QUIT:
			CEventsManager::addEvent(new CGameEvent(Defines::EV_APP_EXIT));
			break;
		case SDL_WINDOWEVENT_RESIZED:
			gEvent = new CGameEvent(Defines::EV_WINDOW_RESIZED);
			gEvent->addParam(event.window.data1);
			gEvent->addParam(event.window.data2);
			CEventsManager::addEvent(gEvent);
			break;
		default:
			break;
		}
		break;

	case SDL_KEYDOWN:
		switch (event.keyboard.keyboardEvent)
		{
		case SDLK_ESCAPE:
			CEventsManager::addEvent(new CGameEvent(Defines::EV_APP_EXIT));
			break;
		case SDLK_n:
			CEventsManager::addEvent(new CGameEvent(Defines::EV_UI_NEWGAME));
			break;
		case SDLK_m:
			CEventsManager::addEvent(new CGameEvent(Defines::EV_UI_MENU));
			break;
		default:
			break;
		}
		break;
	default:
		break;
	}
}