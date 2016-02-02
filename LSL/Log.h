#pragma once

#include <string>
#include <vector>

class CLog
{
public:
	static const int LOG_OUTPUT_ALL		 = 0;
	static const int LOG_OUTPUT_WARNINGS = 1;
	static const int LOG_OUTPUT_ERRORS	 = 2;
private:
	static CLog* instance;

	bool isLoggingEnabled;
	int  logMode;
	std::vector<std::string> logData;

	std::string logFileName;
private:
	CLog();
public:
	void saveLogFile();
	void setLogOutputMode(const int mode);
	void setLogFileName(const std::string logFileName);
	void addInfo(const std::string infoMessage);
	void addWarning(const std::string warningMessage);
	void addError(const std::string errorMessage);

	static CLog* getInstance();
	~CLog();
};

