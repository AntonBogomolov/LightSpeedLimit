#include "PhysicsObject.h"
#include "PhysicsSolver.h"
#include "Orbit.h"

CPhysicsObject::CPhysicsObject(const glm::dvec2 velocity, const glm::dvec2 acceleration, const glm::dvec2 pos, const double mass, const double radius, const bool isSmallMassObj) : CRungeKutta(2)
{
	this->velocity = velocity;
	this->acceleration = acceleration;
	this->pos = pos;
	this->mass = mass;
	this->radius = radius;
	this->isSmallMassObj = isSmallMassObj;
		
	Y[0].x = this->velocity.x;
	Y[0].y = this->velocity.y;
	Y[1].x = this->pos.x;
	Y[1].y = this->pos.y;
	SetModelTime(0.0);
		
	tmpPhyState[0].x = Y[0].x;
	tmpPhyState[0].y = Y[0].y;
	tmpPhyState[1].x = Y[1].x;
	tmpPhyState[1].y = Y[1].y;

	orbit = new COrbit(this);
	isUpdatePosLocked = false;
}

CPhysicsObject::~CPhysicsObject()
{
	if (orbit) delete orbit;
}

const glm::dvec2  CPhysicsObject::getVelocity()const
{
	if(!isLocked()) return velocity;
	else return tmpPhyState[0];
}

const glm::dvec2  CPhysicsObject::getVelocityUnsafe()const
{
	return velocity;	
}

const glm::dvec2* CPhysicsObject::getVelocityPointer()const
{
	if (!isLocked()) return &velocity;
	else return &tmpPhyState[0];
}

const glm::dvec2  CPhysicsObject::getAcceleration()const
{
	return acceleration;
}

const glm::dvec2* CPhysicsObject::getAccelerationPointer()const
{
	return &acceleration;
}

const glm::dvec2  CPhysicsObject::getPos()const
{
	if (!isLocked()) return pos;
	else return tmpPhyState[1];
}

const glm::dvec2  CPhysicsObject::getPosUnsafe()const
{
	return pos;	
}

const glm::dvec2* CPhysicsObject::getPosPointer()const
{
	if (!isLocked()) return &pos;
	else return &tmpPhyState[1];
}

const double CPhysicsObject::getMass()const
{
	return mass;
}

const double CPhysicsObject::getRadius()const
{
	return radius;
}

const bool CPhysicsObject::isSmallMass()const
{
	return isSmallMassObj;
}

void CPhysicsObject::resetInitialParams()
{
	Y[0].x = this->velocity.x;
	Y[0].y = this->velocity.y;
	Y[1].x = this->pos.x;
	Y[1].y = this->pos.y;
	SetModelTime(0.0);
}

void CPhysicsObject::setVelocity(const glm::dvec2 velocity)
{
	this->velocity = velocity;
	resetInitialParams();
}

void CPhysicsObject::setAcceleration(const glm::dvec2 acceleration)
{
	this->acceleration = acceleration;	
}

void CPhysicsObject::setPos(const glm::dvec2 pos)
{
	this->pos = pos;
	resetInitialParams();
}

void CPhysicsObject::setMass(const double mass)
{
	this->mass = mass;
}

void CPhysicsObject::setRadius(const double radius)
{
	this->radius = radius;
}

void CPhysicsObject::setIsSmallMass(const bool isSmallMassObj)
{
	this->isSmallMassObj = isSmallMassObj;
}

void CPhysicsObject::updatePhysics(const float dt)
{
	NextModelStep(dt);

	velocity = Y[0];
	pos = Y[1];
}

void CPhysicsObject::setCircularOrbit(const CPhysicsObject* obj)
{
	CPhysicsSolver::setCircularOrbit(this, obj);
}

COrbit* CPhysicsObject::getOrbit() const
{
	return orbit;
}

void CPhysicsObject::saveCurrPhyState()
{
	resetInitialParams();

	tmpPhyState[0].x = Y[0].x;
	tmpPhyState[0].y = Y[0].y;
	tmpPhyState[1].x = Y[1].x;
	tmpPhyState[1].y = Y[1].y;
}

void CPhysicsObject::restoreCurrPhyState()
{
	Y[0].x = tmpPhyState[0].x;
	Y[0].y = tmpPhyState[0].y;
	Y[1].x = tmpPhyState[1].x;
	Y[1].y = tmpPhyState[1].y;

	velocity = Y[0];
	pos = Y[1];
}

void CPhysicsObject::lock()
{
	isUpdatePosLocked = true;
}

void CPhysicsObject::unlock()
{
	isUpdatePosLocked = false;
}

bool CPhysicsObject::isLocked() const
{
	return isUpdatePosLocked;
}