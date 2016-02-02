#include "SimpleScene.h"
#include "Camera.h"
#include "GameEvent.h"
#include "VideoManeger.h"
#include "SyncHelper.h"

CSimpleScene::CSimpleScene()
{

}

CSimpleScene::~CSimpleScene()
{
	clearResources();
	clearScene();
}

void CSimpleScene::initScene()
{
	CSyncHelper::getInstance()->getDrawMutex()->lock();

	__super::initScene();

	CCamera::getInstance()->setCamVelocity(0.0f, 0.0f);
	CCamera::getInstance()->setCamCoords(-CVideoManeger::scrWidth / 2, -CVideoManeger::scrHeight / 2);		
	initGameObjects();

	CSyncHelper::getInstance()->getDrawMutex()->unlock();
}

void CSimpleScene::initGameObjects()
{

}

void CSimpleScene::clearScene()
{
	CSyncHelper::getInstance()->getDrawMutex()->lock();

	__super::clearScene();	

	CSyncHelper::getInstance()->getDrawMutex()->unlock();
}

void CSimpleScene::loadResources()
{

}

void CSimpleScene::clearResources()
{
	CSyncHelper::getInstance()->getDrawMutex()->lock();

	CSyncHelper::getInstance()->getDrawMutex()->unlock();
}

void CSimpleScene::inputEventHandler(const CInputEvent event)
{

}

void CSimpleScene::appEventHandler(const CGameEvent* event)
{
	switch (event->getEventId())
	{
	default:
		break;
	}
}

void CSimpleScene::updateScene(const float dt)
{
	__super::updateScene(dt);
}

void CSimpleScene::resizeScene()
{
	CSyncHelper::getInstance()->getLogicMutex()->lock();
	__super::resizeScene();	
	CSyncHelper::getInstance()->getLogicMutex()->unlock();
}

void CSimpleScene::initUI()
{

}

void CSimpleScene::uiEvents(MyGUI::WidgetPtr _sender)
{

}
