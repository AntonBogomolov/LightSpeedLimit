#include "SyncHelper.h"

CSyncHelper* CSyncHelper::instance = 0;

CSyncHelper::CSyncHelper()
{
	mtxDraw = new std::mutex();
	mtxEvents = new std::mutex();
	mtxLogic = new std::mutex();
	mtxPhysics = new std::mutex();
	mtxInput = new std::mutex();
	mtxLog = new std::mutex();
}

CSyncHelper::~CSyncHelper()
{
	delete mtxDraw;
	delete mtxEvents;
	delete mtxLogic;
	delete mtxPhysics;
	delete mtxInput;
	delete mtxLog;
}

CSyncHelper* CSyncHelper::getInstance()
{
	if (instance == 0)
	{
		instance = new CSyncHelper();
	}
	return instance;
}

std::mutex* CSyncHelper::getDrawMutex() const
{
	return mtxDraw;
}

std::mutex* CSyncHelper::getEventsMutex() const
{
	return mtxEvents;
}

std::mutex* CSyncHelper::getLogicMutex() const
{
	return mtxLogic;
}

std::mutex* CSyncHelper::getPhysicsMutex() const
{
	return mtxPhysics;
}

std::mutex* CSyncHelper::getInputMutex() const
{
	return mtxInput;
}

std::mutex* CSyncHelper::getLogMutex() const
{
	return mtxLog;
}