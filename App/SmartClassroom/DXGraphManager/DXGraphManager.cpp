// DXGraphManager.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "DXGraphManager.h"


// This is an example of an exported variable
DXGRAPHMANAGER_API int nDXGraphManager=0;

// This is an example of an exported function.
DXGRAPHMANAGER_API int fnDXGraphManager(void)
{
	return 42;
}

// This is the constructor of a class that has been exported.
// see DXGraphManager.h for the class definition
CDXGraphManager::CDXGraphManager()
{
	return;
}
