#pragma once

#include "../../tools/includes/glm/glm.hpp"
#include "IInputHandler.h"

class CGameObj;

class CCamera : public IInputHandler
{
private:
	static CCamera* instance;

	glm::vec2 camPos;
	glm::vec2 camVel;
	glm::vec2 camAccel;
	float speedDecrease;
	float zoomLevel;
	float speedLimit;

	bool isSpeedDecrease;
	const CGameObj* target;

	CCamera();
public:
	static CCamera*	getInstance();
	const glm::vec2* getCamCoordsPointer()const;
	const glm::vec2 getCamCoords()const;
	void setCamCoords(const glm::vec2* camPos);
	void setCamCoords(const glm::vec2 camPos);
	void setCamCoords(const float x, const float y);
	void setCamVelocity(const float velX, const float velY);
	void addCamVelocity(const float velX, const float velY);
	void setCamAccel(const float accX, const float accY);
	void addCamAccel(const float accX, const float accY);

	void setSpeedDecrease(const float speedDecrease);
	const float getSpeedDecrease()const;
	void setIsSpeedDecrease(const bool isDecrease);
	const bool getIsSpeedDecrease()const;
	void setSpeedLimit(const float speedLimit);
	const float getSpeedLimit()const;
		
	void setZoom(const float zoom);
	void modifyZoom(const float deltaZoom);
	const float getZoom()const;

	void moveTo(const glm::vec2 camPos, const float timeToTravel);
	void setGuiding(const CGameObj* obj);

	void updateCamera(const float dt);
	virtual void inputEventHandler(const CInputEvent event);

	~CCamera();
};

