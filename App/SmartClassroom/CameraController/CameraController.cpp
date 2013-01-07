// CameraController.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "CameraController.h"


// This is an example of an exported variable
CAMERACONTROLLER_API int nCameraController=0;

// This is an example of an exported function.
CAMERACONTROLLER_API int fnCameraController(void)
{
	return 42;
}

// This is the constructor of a class that has been exported.
// see CameraController.h for the class definition
CCameraController::CCameraController()
{
	return;
}
