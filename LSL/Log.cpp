#include "Log.h"
#include "AppTime.h"
#include "SyncHelper.h"

#include <iostream>
#include <fstream>
#include <exception>

CLog* CLog::instance = 0;

CLog::CLog()
{
	//std::ios::sync_with_stdio(false);

	isLoggingEnabled = true;
	setLogOutputMode(LOG_OUTPUT_ALL);
	setLogFileName("log.txt");	
}

CLog::~CLog()
{
	saveLogFile();
}

CLog* CLog::getInstance()
{
	if (instance == 0)
	{
		instance = new CLog();
	}
	return instance;
}

void CLog::saveLogFile()
{

	//bool lockRes = CSyncHelper::getInstance()->getLogMutex()->try_lock();
	std::ofstream logFile;
	try
	{
		logFile.open(logFileName);
		for each (std::string message in logData)
		{
			logFile.write(message.c_str(), message.length());
		}
	}
	catch (std::exception &e)
	{
		//addError(e.what() + std::string(" | Cant create log file! \t| CLog::saveLogFile()"));
	}
	//if (lockRes) CSyncHelper::getInstance()->getLogMutex()->unlock();
	logFile.close();	
}

void CLog::setLogOutputMode(const int mode)
{
	logMode = mode;
}

void CLog::setLogFileName(const std::string logFileName)
{
	this->logFileName = logFileName;
}

void CLog::addInfo(const std::string infoMessage)
{
	bool lockRes = CSyncHelper::getInstance()->getLogMutex()->try_lock();
	if (!lockRes) return;

	std::string message = CAppTime::getInstance()->getLocalTime() + std::string(" INFO: ") + infoMessage + std::string("\n");
	logData.push_back(message);
	if (logMode == LOG_OUTPUT_ALL)
	{
		std::cout << "INFO: " << infoMessage << std::endl;
		std::flush(std::cout);
	}
	if (lockRes) CSyncHelper::getInstance()->getLogMutex()->unlock();
}

void CLog::addWarning(const std::string warningMessage)
{
	bool lockRes = CSyncHelper::getInstance()->getLogMutex()->try_lock();
	if (!lockRes) return;

	std::string message = CAppTime::getInstance()->getLocalTime() + std::string(" WARNING: ") + warningMessage + std::string("\n");
	logData.push_back(message);
	if (logMode == LOG_OUTPUT_ALL || logMode == LOG_OUTPUT_WARNINGS)
	{
		std::cout << "WARNING: " << warningMessage << std::endl;
		std::flush(std::cout);
	}
	if (lockRes) CSyncHelper::getInstance()->getLogMutex()->unlock();
}

void CLog::addError(const std::string errorMessage)
{
	bool lockRes = CSyncHelper::getInstance()->getLogMutex()->try_lock();
	if (!lockRes) return;

	std::string message = CAppTime::getInstance()->getLocalTime() + std::string(" ERROR: ") + errorMessage + std::string("\n");
	logData.push_back(message);
	if (logMode == LOG_OUTPUT_ALL || logMode == LOG_OUTPUT_WARNINGS || logMode == LOG_OUTPUT_ERRORS)
	{
		std::cout << "ERROR: " << errorMessage << std::endl;
		std::flush(std::cout);
	}
	if(lockRes) CSyncHelper::getInstance()->getLogMutex()->unlock();
}