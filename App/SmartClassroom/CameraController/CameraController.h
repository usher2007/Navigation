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

// This class is exported from the CameraController.dll
class CAMERACONTROLLER_API CCameraController {
public:
	CCameraController(void);
	// TODO: add your methods here.
};

extern CAMERACONTROLLER_API int nCameraController;

CAMERACONTROLLER_API int fnCameraController(void);
