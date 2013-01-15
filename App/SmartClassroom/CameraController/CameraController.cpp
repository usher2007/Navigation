// CameraController.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "CameraController.h"


// This is the constructor of a class that has been exported.
// see CameraController.h for the class definition
CCameraController::CCameraController()
{
	return;
}


int CCameraController::addCamera(int cameraId, int comNum, int baudRate)
{
	Camera *pCamera = new Camera(comNum, baudRate);
	cameraList[cameraId] = *pCamera;
	return S_OK;
}

int CCameraController::deleteCamera(int cameraId, Camera& camera)
{
	camera = cameraList[cameraId];
	cameraList.erase(cameraId);
	return S_OK;
}

int CCameraController::TurnLeft(int cameraId)
{
	if(cameraList.find(cameraId) != cameraList.end())
	{
		cameraList[cameraId].TurnLeft();
		return S_OK;
	}
	return E_FAIL;
}

int CCameraController::TurnRight(int cameraId)
{
	if(cameraList.find(cameraId) != cameraList.end())
	{
		cameraList[cameraId].TurnRight();
		return S_OK;
	}
	return E_FAIL;
}

int CCameraController::TurnUp(int cameraId)
{
	if(cameraList.find(cameraId) != cameraList.end())
	{
		cameraList[cameraId].TurnUp();
		return S_OK;
	}
	return E_FAIL;
}

int CCameraController::TurnDown(int cameraId)
{
	if(cameraList.find(cameraId) != cameraList.end())
	{
		cameraList[cameraId].TurnDown();
		return S_OK;
	}
	return E_FAIL;
}

int CCameraController::ZoomIn(int cameraId)
{
	if(cameraList.find(cameraId) != cameraList.end())
	{
		cameraList[cameraId].ZoomIn();
		return S_OK;
	}
	return E_FAIL;
}

int CCameraController::ZoomOut(int cameraId)
{
	if(cameraList.find(cameraId) != cameraList.end())
	{
		cameraList[cameraId].ZoomOut();
		return S_OK;
	}
	return E_FAIL;
}

int CCameraController::SetPreSetPos(int cameraId, int locId)
{
	Location loc(locId);
	if(cameraList.find(cameraId) != cameraList.end())
	{
		cameraList[cameraId].AddPreSetLocation(loc);
		return S_OK;
	}
	return E_FAIL;
}

int CCameraController::RecallPreSetPos(int cameraId, int locId)
{
	if(cameraList.find(cameraId) != cameraList.end())
	{
		cameraList[cameraId].TurnToSpecificLocation(locId);
		return S_OK;
	}
	return E_FAIL;
}

int CCameraController::GetSpecificCameraLocation(int cameraId, Location& loc)
{
	return -1; //NO IMP
}

int CCameraController::TurnToSpecificLocation(int cameraId, const Location& loc)
{
	return -1; //NO IMP
}
