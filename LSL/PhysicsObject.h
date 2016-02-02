#pragma once

#include "../../tools/includes/glm/glm.hpp"
#include "RungeKutta.h"

class COrbit;

class CPhysicsObject : public CRungeKutta<glm::dvec2>
{
private:
	glm::dvec2 velocity;
	glm::dvec2 acceleration;
	glm::dvec2 pos;
	double mass;
	double radius;
	bool isSmallMassObj;

	COrbit* orbit;
	glm::dvec2 tmpPhyState[2];

	bool isUpdatePosLocked;
private:
	void resetInitialParams();
public:
	const glm::dvec2  getVelocity()const;
	const glm::dvec2  getVelocityUnsafe()const;
	const glm::dvec2* getVelocityPointer()const;
	const glm::dvec2  getAcceleration()const;
	const glm::dvec2* getAccelerationPointer()const;
	const glm::dvec2  getPos()const;
	const glm::dvec2  getPosUnsafe()const;
	const glm::dvec2* getPosPointer()const;
	const double getMass()const;
	const double getRadius()const;
	const bool isSmallMass()const;

	COrbit* getOrbit() const;
	void saveCurrPhyState();
	void restoreCurrPhyState(); 
	
	void setVelocity(const glm::dvec2 velocity);
	void setAcceleration(const glm::dvec2 acceleration);
	void setPos(const glm::dvec2 pos);
	void setMass(const double mass);
	void setRadius(const double radius);
	void setIsSmallMass(const bool isSmallMassObj);

	void setCircularOrbit(const CPhysicsObject* obj);

	virtual void updatePhysics(const float dt);

	void lock();
	void unlock();
	bool isLocked() const;

	CPhysicsObject(const glm::dvec2 velocity = glm::dvec2(0.0f, 0.0f), const glm::dvec2 acceleration = glm::dvec2(0.0f, 0.0f),
		const glm::dvec2 pos = glm::dvec2(0.0f, 0.0f), const double mass = 1, const double radius = 1, const bool isSmallMassObj = true);
	virtual ~CPhysicsObject();
};

