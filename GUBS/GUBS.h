#pragma once

#include "resource.h"


#include "logog.hpp"

class LoggingInitializer
{
	logog::LogFile* _LogFile;

public:
	LoggingInitializer(const std::string logFile)
	{
		LOGOG_INITIALIZE();
		_LogFile = new logog::LogFile(logFile.c_str() , true);
	}

	~LoggingInitializer()
	{
		delete _LogFile;
		LOGOG_SHUTDOWN();
	}
};