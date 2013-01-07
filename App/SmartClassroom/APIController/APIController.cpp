// APIController.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "APIController.h"


// This is an example of an exported variable
APICONTROLLER_API int nAPIController=0;

// This is an example of an exported function.
APICONTROLLER_API int fnAPIController(void)
{
	return 42;
}

// This is the constructor of a class that has been exported.
// see APIController.h for the class definition
CAPIController::CAPIController()
{
	return;
}
