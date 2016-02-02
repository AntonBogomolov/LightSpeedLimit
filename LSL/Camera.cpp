#include "Camera.h"

#include "GameEvent.h"
#include "EventsManager.h"
#include "EngineDefines.h"
#include "GameObj.h"
#include "VideoManeger.h"
#include "InputManager.h"

#include "SyncHelper.h"
#include <iostream>

CCamera* CCamera::instance = 0;
CCamera::CCamera()
{
	camPos = glm::vec2(0.0f, 0.0f);
	speedDecrease = 100.0f;
	zoomLevel = 1.0f;

	speedLimit = 600.0f;
	isSpeedDecrease = true;
	target = NULL;

	CInputManager::getInstance()->addEventHandler(this, CInputEvent::IEVENT_KEY_DOWN | CInputEvent::IEVENT_MOUSE_WHEEL);
}

CCamera::~CCamera()
{

}

CCamera* CCamera::getInstance()
{
	if (!instance)
	{
		instance = new CCamera();
	}
	return instance;
}

const glm::vec2* CCamera::getCamCoordsPointer()const
{	
	return &camPos;
}

const glm::vec2 CCamera::getCamCoords()const
{		
	bool syncResult = CSyncHelper::getInstance()->getLogicMutex()->try_lock();
	glm::vec2 pos = camPos;
	if (syncResult) CSyncHelper::getInstance()->getLogicMutex()->unlock();
	return pos;
}

void CCamera::setCamCoords(const glm::vec2* newCoords)
{	
	bool syncResult = CSyncHelper::getInstance()->getLogicMutex()->try_lock();
	camPos.x = newCoords->x;
	camPos.y = newCoords->y;
	if (syncResult) CSyncHelper::getInstance()->getLogicMutex()->unlock();
	CEventsManager::addEvent(new CGameEvent(Defines::EV_CAM_MOVE));		
}

void CCamera::setCamCoords(const glm::vec2 newCoords)
{
	bool syncResult = CSyncHelper::getInstance()->getLogicMutex()->try_lock();
	camPos.x = newCoords.x;
	camPos.y = newCoords.y;
	if (syncResult) CSyncHelper::getInstance()->getLogicMutex()->unlock();
	CEventsManager::addEvent(new CGameEvent(Defines::EV_CAM_MOVE));	
}


void CCamera::setCamCoords(const float x, const float y)
{			
	bool syncResult = CSyncHelper::getInstance()->getLogicMutex()->try_lock();
	camPos.x = x;
	camPos.y = y;
	if (syncResult) CSyncHelper::getInstance()->getLogicMutex()->unlock();
	CEventsManager::addEvent(new CGameEvent(Defines::EV_CAM_MOVE));	
}

void CCamera::moveTo(const glm::vec2 newCoords, const float timeToTravel)
{
	CEventsManager::addEvent(new CGameEvent(Defines::EV_CAM_MOVE));
}

void CCamera::updateCamera(const float dt)
{		
	camVel += camAccel * dt;
	camPos += camVel * dt;

	if (target)
	{		
		const glm::vec2* targetCoords = target->getCenterAbsoluteCoords();
		if (glm::abs(camPos.x - targetCoords->x) > 1 || glm::abs(camPos.y - targetCoords->y) > 1)
		{
			camPos = *targetCoords;
			camPos.x -= CVideoManeger::scrWidth / 2.0f;
			camPos.y -= CVideoManeger::scrHeight / 4.0f;
			CEventsManager::addEvent(new CGameEvent(Defines::EV_CAM_MOVE));
		}
	}
	else
	{
		const float minSpeed = dt * speedDecrease;
		const float vel = glm::length(camVel);
		if (vel > speedLimit)
		{
			camVel *= speedLimit / vel;
		}
		if (vel >= minSpeed) CEventsManager::addEvent(new CGameEvent(Defines::EV_CAM_MOVE));

		if (vel > 0.0f && isSpeedDecrease)
		{
			if (glm::length(camAccel) > 0.0f) camAccel -= speedDecrease * glm::normalize(camAccel) * dt;
			camVel -= speedDecrease * glm::normalize(camVel) * dt;			
		}
	}
}

void CCamera::setCamVelocity(const float velX, const float velY)
{
	bool syncResult = CSyncHelper::getInstance()->getLogicMutex()->try_lock();
	camVel.x = velX;
	camVel.y = velY;
	if (syncResult) CSyncHelper::getInstance()->getLogicMutex()->unlock();
}

void CCamera::setCamAccel(const float accX, const float accY)
{
	bool syncResult = CSyncHelper::getInstance()->getLogicMutex()->try_lock();
	camAccel.x = accX;
	camAccel.y = accY;
	if (syncResult) CSyncHelper::getInstance()->getLogicMutex()->unlock();
}

void CCamera::addCamAccel(const float accX, const float accY)
{
	bool syncResult = CSyncHelper::getInstance()->getLogicMutex()->try_lock();
	camAccel.x += accX;
	camAccel.y += accY;
	if (syncResult) CSyncHelper::getInstance()->getLogicMutex()->unlock();
}

void CCamera::addCamVelocity(const float velX, const float velY)
{	
	bool syncResult = CSyncHelper::getInstance()->getLogicMutex()->try_lock();
	camVel.x += velX;
	camVel.y += velY;	
	if(syncResult) CSyncHelper::getInstance()->getLogicMutex()->unlock();
}

void CCamera::setSpeedDecrease(const float speedDecrease)
{
	bool syncResult = CSyncHelper::getInstance()->getLogicMutex()->try_lock();
	this->speedDecrease = speedDecrease;
	if (syncResult) CSyncHelper::getInstance()->getLogicMutex()->unlock();
}

const float CCamera::getSpeedDecrease()const
{
	return speedDecrease;
}

void CCamera::setZoom(const float zoom)
{
	bool syncResult = CSyncHelper::getInstance()->getLogicMutex()->try_lock();
	this->zoomLevel = zoom;
	if (syncResult) CSyncHelper::getInstance()->getLogicMutex()->unlock();
}

const float CCamera::getZoom()const
{
	bool syncResult = CSyncHelper::getInstance()->getLogicMutex()->try_lock();
	return zoomLevel;
	if (syncResult) CSyncHelper::getInstance()->getLogicMutex()->unlock();
}

void CCamera::modifyZoom(const float deltaZoom)
{
	bool syncResult = CSyncHelper::getInstance()->getLogicMutex()->try_lock();
	this->zoomLevel += deltaZoom;
	if (syncResult) CSyncHelper::getInstance()->getLogicMutex()->unlock();
}

void CCamera::setIsSpeedDecrease(const bool isDecrease)
{
	bool syncResult = CSyncHelper::getInstance()->getLogicMutex()->try_lock();
	this->isSpeedDecrease = isDecrease;
	if (syncResult) CSyncHelper::getInstance()->getLogicMutex()->unlock();
}

const bool CCamera::getIsSpeedDecrease()const
{
	return isSpeedDecrease;
}

void CCamera::setSpeedLimit(const float speedLimit)
{
	bool syncResult = CSyncHelper::getInstance()->getLogicMutex()->try_lock();
	this->speedLimit = speedLimit;
	if (syncResult) CSyncHelper::getInstance()->getLogicMutex()->unlock();
}

const float CCamera::getSpeedLimit()const
{
	return speedLimit;
}

void CCamera::setGuiding(const CGameObj* obj)
{
	bool syncResult = CSyncHelper::getInstance()->getLogicMutex()->try_lock();
	this->target = obj;
	if (syncResult) CSyncHelper::getInstance()->getLogicMutex()->unlock();
}

void CCamera::inputEventHandler(const CInputEvent event)
{
	const CKeyHandler* keyHandler = CInputManager::getInstance()->getKeysStates();

	switch (event.eventId)
	{
	case SDL_MOUSEWHEEL:
		//if (event.mouse.wheelDelta > 0)	modifyZoom(-0.05f);
		//if (event.mouse.wheelDelta < 0)	modifyZoom(0.05f);
		break;
	case SDL_KEYDOWN:
		if (keyHandler->isPressed(SDLK_LEFT))  addCamVelocity(-20.0f, 0.0f);
		if (keyHandler->isPressed(SDLK_RIGHT)) addCamVelocity(20.0f, 0.0f);
		if (keyHandler->isPressed(SDLK_UP))    addCamVelocity(0.0f, -20.0f);
		if (keyHandler->isPressed(SDLK_DOWN))  addCamVelocity(0.0f, 20.0f);
			
		break;
	default:
		break;
	}	
}