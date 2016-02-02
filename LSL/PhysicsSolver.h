#pragma once

class CPhysicsObject;
#include <vector>
#include <thread>

class CPhysicsSolver
{
private:
	static double G;
	std::vector<CPhysicsObject*>* physObjects;
	float phyTimer;

	std::thread* solverThread;
	bool isSolverThreadRuning;
private:
	void solverThreadWorker();
public:
	const std::vector<CPhysicsObject*>* getPhysicsObjects()const;
	std::vector<CPhysicsObject*>* getPhysicsObjectsForModify()const;
	void clearPhysicsObjects();
	void addPhysicsObject(CPhysicsObject* obj);
	void addPhysicsObjects(const std::vector<CPhysicsObject*>* physObjects);
	void removePhysicsObjects(const CPhysicsObject* obj);
	
	void solve(const float dt = 0.005f);
	void startSolverThread();
	void stopSolverThread();
	void calcFutureOrbits();
	static void setCircularOrbit(CPhysicsObject* planet, const CPhysicsObject* star);

	void setIsSolverThreadRuning(const bool runing);
	bool getIsSolverThreadRuning() const;

	CPhysicsSolver();
	~CPhysicsSolver();
};

