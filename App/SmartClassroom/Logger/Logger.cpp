// Logger.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "Logger.h"


// This is an example of an exported variable
LOGGER_API int nLogger=0;

// This is an example of an exported function.
LOGGER_API int fnLogger(void)
{
	return 42;
}

// This is the constructor of a class that has been exported.
// see Logger.h for the class definition
CLogger::CLogger()
{
	return;
}
