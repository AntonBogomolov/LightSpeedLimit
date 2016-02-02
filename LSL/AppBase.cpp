#include "AppBase.h"
#include "global.h"
#include "VideoManeger.h"
#include "InputManager.h"
#include "FrameTimer.h"
#include "ResourceManager.h"
#include "FrameBuffer.h"
#include "Texture.h"
#include "GLShaderObject.h"
#include "GLUtils.h"
#include "VertexBuffer.h"
#include "SyncHelper.h"
#include "EngineSettings.h"
#include "Log.h"

#include "OpenGLImageLoader.h"

#include "AppTime.h"
#include "EventsManager.h"
#include "GameEvent.h"
#include "Camera.h"
#include "Scene.h"
#include "Renderer.h"
#include "SimpleScene.h"

CAppBase::CAppBase(void)
{
	quit = false;
	VideoManeger = CVideoManeger::GetInstance();
	InputManeger = CInputManager::getInstance();	
	CResourceManager::getInstance();
	CEventsManager::getInstance();
	CCamera::getInstance();
	CSyncHelper::getInstance();
	CLog::getInstance();

	currScene = new CSimpleScene();
	openGLRenderer = new CRenderer();
		
	drawThread = NULL;
	logicThread = NULL;
}

CAppBase::~CAppBase(void)
{
	if (currScene) delete currScene;
	delete openGLRenderer;

	delete CResourceManager::getInstance();
	delete CEventsManager::getInstance();
	delete CCamera::getInstance();
	delete CSyncHelper::getInstance();
	delete CAppTime::getInstance();
	delete CLog::getInstance();

	delete VideoManeger;
	delete InputManeger;
	
	delete drawThread;	
	delete logicThread;
}

void CAppBase::Init()
{
	InitVideo();

	srand(time(0));	
	quit = false;

	CEventsManager::addEvent(new CGameEvent(Defines::EV_APP_START));
}

void CAppBase::Run()
{
	logicThread = new std::thread(&CAppBase::LogicLoop, this);
	drawThread  = new std::thread(&CAppBase::DrawLoop,  this);	
	
	std::this_thread::sleep_for(std::chrono::milliseconds(1000));
	CLog::getInstance()->addInfo("Message loop started");
	while (isWorking())
	{
		InputManeger->SDLEvents();
		std::this_thread::sleep_for(std::chrono::milliseconds(EngineSettings::eventsThreadUpdateDurationInMs));
	}

	logicThread->join();
	drawThread ->join();
}

void CAppBase::LogicLoop()
{
	std::this_thread::sleep_for(std::chrono::milliseconds(1000));

	CSyncHelper::getInstance()->getDrawMutex()->lock();
	SDL_GL_MakeCurrent(CVideoManeger::GetInstance()->dispWindow, CVideoManeger::GetInstance()->glThreadContext);	
	CSyncHelper::getInstance()->getDrawMutex()->unlock();
	CLog::getInstance()->addInfo("Logic thread started");

	CFrameTimer* logicTimer = new CFrameTimer(1, 0);
	logicTimer->setPrintString("LOGIC FPS: ");
	logicTimer->Reset();

	while (isWorking())
	{		
		logicTimer->addFrame();
		float dt = logicTimer->getDT();		
		if (dt > EngineSettings::logicThreadUpdateDurationInSec) dt = EngineSettings::logicThreadUpdateDurationInSec;
		CAppTime::getInstance()->addDTime(dt);
		
		CSyncHelper::getInstance()->getLogicMutex()->lock();		
		CCamera::getInstance()->updateCamera(dt);
		currScene->updateScene(dt);
		CSyncHelper::getInstance()->getLogicMutex()->unlock();

		std::this_thread::sleep_for(std::chrono::milliseconds(EngineSettings::logicThreadUpdateDurationInMs - (int)(dt * 1000) ));
	}

	CLog::getInstance()->addInfo("Logic thread stoped");
	SDL_GL_MakeCurrent(CVideoManeger::GetInstance()->dispWindow, NULL);
	delete logicTimer;
}

void CAppBase::DrawLoop()
{		
	CSyncHelper::getInstance()->getDrawMutex()->lock();
	SDL_GL_MakeCurrent(CVideoManeger::GetInstance()->dispWindow, CVideoManeger::GetInstance()->glRenderContext);	
	CLog::getInstance()->addInfo("Draw thread started");

	CFrameTimer* drawTimer = new CFrameTimer(1, 0);
	drawTimer->setPrintString("DRAW FPS: ");
	drawTimer->Reset();

	CVideoManeger::GetInstance()->InitDrawThread();
	openGLRenderer->initRender();
	initGUI();
	CSyncHelper::getInstance()->getDrawMutex()->unlock();
		
	while (isWorking())
	{
		drawTimer->addFrame();
		float dt = drawTimer->getDT();
		if (dt > 0.03f) dt = 0.03f;	
		
		appEvents(dt);		
		CSyncHelper::getInstance()->getDrawMutex()->lock();		
		Draw();		
		SDL_GL_SwapWindow(VideoManeger->dispWindow);		
		CSyncHelper::getInstance()->getDrawMutex()->unlock();		
	}

	CLog::getInstance()->addInfo("Draw thread stoped");
	SDL_GL_MakeCurrent(CVideoManeger::GetInstance()->dispWindow, NULL);
	delete drawTimer;
}

void CAppBase::Draw()
{		
	openGLRenderer->render(currScene);	
	DrawUI();
}

void CAppBase::DrawUI()
{
	CTexture::unbind();
	CVertexBuffer::unbind();
	CFrameBuffer::unbind();
	CGLShaderObject::disable();
	
	CSyncHelper::getInstance()->getInputMutex()->lock();
	CVideoManeger::GetInstance()->uiPlatform->getRenderManagerPtr()->drawOneFrame();
	CSyncHelper::getInstance()->getInputMutex()->unlock();
		
	CTexture::unbind();
	CVertexBuffer::unbind();
	CFrameBuffer::unbind();
	CGLShaderObject::disable();
}

void CAppBase::InitVideo()
{
	VideoManeger->InitSDL();	
}

void CAppBase::initGUI()
{
	CVideoManeger* vidMgr = CVideoManeger::GetInstance();
	
	vidMgr->uiPlatform = new MyGUI::OpenGLPlatform();
	vidMgr->uiPlatform->initialise(new COpenGLImageLoader());
	vidMgr->uiPlatform->getDataManagerPtr()->addResourceLocation("media//UI//MyGUI_Media", false);
	vidMgr->uiPlatform->getRenderManagerPtr()->setViewSize(vidMgr->scrWidth, vidMgr->scrHeight);

	vidMgr->myGUI = new MyGUI::Gui();
	vidMgr->myGUI->initialise("MyGUI_Core.xml");
}

void CAppBase::changeScene(CScene* newScene)
{
	if (!newScene) return;
	if (currScene == newScene)
	{
		CSyncHelper::getInstance()->getLogicMutex()->lock();
		newScene->initScene();
		CSyncHelper::getInstance()->getLogicMutex()->unlock();
		return;
	}
	
	CSyncHelper::getInstance()->getLogicMutex()->lock();
	if (currScene) delete currScene;
	newScene->initScene();
	currScene = newScene;
	CSyncHelper::getInstance()->getLogicMutex()->unlock();
}

const bool CAppBase::isWorking() const
{
	return !quit;
}