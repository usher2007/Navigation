// ConfigManager.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "ConfigManager.h"


// This is an example of an exported variable
CONFIGMANAGER_API int nConfigManager=0;

// This is an example of an exported function.
CONFIGMANAGER_API int fnConfigManager(void)
{
	return 42;
}

// This is the constructor of a class that has been exported.
// see ConfigManager.h for the class definition
CConfigManager::CConfigManager()
{
	return;
}
