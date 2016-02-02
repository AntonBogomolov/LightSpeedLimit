#pragma once
#include <mutex>
#include <chrono>
#include <thread>

class CSyncHelper
{
private:
	std::mutex* mtxDraw;
	std::mutex* mtxEvents;
	std::mutex* mtxLogic;
	std::mutex* mtxPhysics;
	std::mutex* mtxInput;
	std::mutex* mtxLog;

	CSyncHelper();
public:
	static CSyncHelper* instance;

	static CSyncHelper* getInstance();
	std::mutex* getDrawMutex() const;
	std::mutex* getEventsMutex() const;
	std::mutex* getLogicMutex() const;
	std::mutex* getPhysicsMutex() const;
	std::mutex* getInputMutex() const;
	std::mutex* getLogMutex() const;
public:
	~CSyncHelper();
};