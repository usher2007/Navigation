// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the CAMERACONTROLLER_EXPORTS
// symbol defined on the command line. This symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// CAMERACONTROLLER_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
#ifdef CAMERACONTROLLER_EXPORTS
#define CAMERACONTROLLER_API __declspec(dllexport)
#else
#define CAMERACONTROLLER_API __declspec(dllimport)
#endif

#include <iostream>
#include <vector>

#include "Camera.h"

class Location;
class Camera;
// This class is exported from the CameraController.dll
class CAMERACONTROLLER_API CCameraController {
public:
	CCameraController(void);
	CCameraController(int cameraNum);
	int addCamera(const Camera& camera);
	int deleteCamera(int cameraId, Camera& camera);

	int TurnLeft(int cameraId);
	int TurnRight(int cameraId);
	int TurnUp(int cameraId);
	int TurnDown(int cameraId);

	int TurnToSpecificLocation(int cameraId, const Location& loc);
	int GetSpecificCameraLocation(int cameraId, Location& loc);
private:
	std::vector<Camera> cameraList;
	int cameraNum;
};

