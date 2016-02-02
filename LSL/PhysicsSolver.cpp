#include "PhysicsSolver.h"
#include "PhysicsObject.h"
#include "Orbit.h"
#include "EventsManager.h"
#include "GameEvent.h"
#include "EngineDefines.h"
#include "EngineSettings.h"
#include "Log.h"

#include <iostream>
#include <chrono>

double CPhysicsSolver::G = 6.67191e-11;

CPhysicsSolver::CPhysicsSolver()
{	
	physObjects = new std::vector < CPhysicsObject* >();	
	phyTimer = 0.0f;

	solverThread = NULL;
	isSolverThreadRuning = false;
}

CPhysicsSolver::~CPhysicsSolver()
{
	if (physObjects) delete physObjects;
	if (solverThread != NULL) delete solverThread;
}

const std::vector<CPhysicsObject*>* CPhysicsSolver::getPhysicsObjects()const
{
	return physObjects;
}

std::vector<CPhysicsObject*>* CPhysicsSolver::getPhysicsObjectsForModify()const
{
	return physObjects;
}

void CPhysicsSolver::clearPhysicsObjects()
{
	physObjects->clear();
}

void CPhysicsSolver::addPhysicsObject(CPhysicsObject* obj)
{
	physObjects->push_back(obj);
}

void CPhysicsSolver::addPhysicsObjects(const std::vector<CPhysicsObject*>* physObjects)
{
	this->physObjects->insert(this->physObjects->end(), physObjects->begin(), physObjects->end());
}

void CPhysicsSolver::removePhysicsObjects(const CPhysicsObject* obj)
{
	if (!obj) return;
	for (auto it = physObjects->begin(); it != physObjects->end(); ++it)
	{
		if (obj == (*it))
		{
			physObjects->erase(it);
			break;
		}
	}
}

void CPhysicsSolver::solve(const float dt)
{	
	glm::dvec2 tmpAcc;
	glm::dvec2 tmpR;
	double tmpLen;

	phyTimer += dt;
	if (phyTimer > EngineSettings::phyOrbitUpdateTimeInSec)
	{
		phyTimer = 0.0f;
		calcFutureOrbits();
	}

	for each (CPhysicsObject* obj1 in *physObjects)
	{
		tmpAcc = glm::dvec2(0.0, 0.0);
		for each (CPhysicsObject* obj2 in *physObjects)
		{
			if (obj1 == obj2) continue;
			if (obj2->isSmallMass()) continue;

			tmpR = obj2->getPos() - obj1->getPos();
			tmpLen = glm::length(tmpR);
			tmpR = glm::normalize(tmpR);

			tmpAcc += tmpR * ((G * obj2->getMass()) / (tmpLen * tmpLen));
		}
		obj1->setAcceleration(tmpAcc);		
	}

	for each (CPhysicsObject* obj1 in *physObjects)
	{
		obj1->updatePhysics(dt);
	}
}

void CPhysicsSolver::solverThreadWorker()
{
	isSolverThreadRuning = true;

	while (isSolverThreadRuning)
	{
		solve(EngineSettings::physicsThreadUpdateDurationInSec);
		std::this_thread::sleep_for(std::chrono::milliseconds(EngineSettings::physicsThreadUpdateDurationInMs));
	}
}

void CPhysicsSolver::startSolverThread()
{
	isSolverThreadRuning = false;
	if (solverThread != NULL) delete solverThread;
	solverThread = new std::thread(&CPhysicsSolver::solverThreadWorker, this);

	CLog::getInstance()->addInfo("CPhysicsSolver::startSolverThread() \t| physics thread started");
}

void CPhysicsSolver::stopSolverThread()
{
	isSolverThreadRuning = false;
	solverThread->join();

	CLog::getInstance()->addInfo("CPhysicsSolver::stopSolverThread() \t| physics thread stoped");
}

void CPhysicsSolver::setIsSolverThreadRuning(const bool runing)
{
	this->isSolverThreadRuning = runing;
}

bool CPhysicsSolver::getIsSolverThreadRuning() const
{
	return isSolverThreadRuning;
}

void CPhysicsSolver::calcFutureOrbits()
{
	glm::dvec2 tmpAcc;
	glm::dvec2 tmpR;
	double tmpLen;
	const int orbitSteps		= EngineSettings::phyFutureOrbitCalcSteps;
	const int orbitDetailFactor = orbitSteps / COrbit::getFutureOrbitStepsCnt();
	const float stepTime		= EngineSettings::phyFutureOrbitCalcStepDuration;

	for each (CPhysicsObject* obj1 in *physObjects)
	{
		if (obj1->isSmallMass()) continue;
		obj1->saveCurrPhyState();	
		obj1->lock();
	}

	////////////////////////////////////////////////////////////////////////////

	for (int step = 0; step < orbitSteps; step++)
	{
		for each (CPhysicsObject* obj1 in *physObjects)
		{			
			tmpAcc = glm::dvec2(0.0, 0.0);
			for each (CPhysicsObject* obj2 in *physObjects)
			{
				if (obj1 == obj2) continue;
				if (obj2->isSmallMass()) continue;

				tmpR = obj2->getPosUnsafe() - obj1->getPosUnsafe();
				tmpLen = glm::length(tmpR);
				tmpR = glm::normalize(tmpR);

				tmpAcc += tmpR * ((G * obj2->getMass()) / (tmpLen * tmpLen));
			}
			obj1->setAcceleration(tmpAcc);
		}

		for each (CPhysicsObject* obj1 in *physObjects)
		{
			COrbit* currOrbit = obj1->getOrbit();
			if (!currOrbit->getIsNeedToCalcOrbit()) continue;

			obj1->updatePhysics(stepTime);
			if (step % orbitDetailFactor == 0)
			{
				currOrbit->saveCurrPhyStateAsFutureOrbitData(step / orbitDetailFactor);				
			}
		}
	}
	
	CEventsManager::addEvent(new CGameEvent(Defines::EV_UPDATE_PHY_OBJ));
	
	////////////////////////////////////////////////////////////////////////////

	for each (CPhysicsObject* obj1 in *physObjects)
	{		
		obj1->restoreCurrPhyState();
		obj1->unlock();

		if (obj1->getOrbit()->getIsNeedToCalcOrbit())  obj1->getOrbit()->saveCurrPhyStateAsPastOrbitData();
	}
}

void CPhysicsSolver::setCircularOrbit(CPhysicsObject* planet, const CPhysicsObject* star)
{
	glm::dvec2 planetVel = glm::normalize(planet->getPos() - star->getPos());
	double oldX = planetVel.x;
	planetVel.x = planetVel.y;
	planetVel.y = -oldX;

	double dist = glm::distance(star->getPos(), planet->getPos());
	planetVel *= glm::sqrt(G * star->getMass() / dist);
	
	planetVel += star->getVelocity();
	planet->setVelocity(planetVel);
}