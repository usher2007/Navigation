// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the APICONTROLLER_EXPORTS
// symbol defined on the command line. This symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// APICONTROLLER_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
#ifdef APICONTROLLER_EXPORTS
#define APICONTROLLER_API __declspec(dllexport)
#else
#define APICONTROLLER_API __declspec(dllimport)
#endif

#include "ModuleFactory.h"

// This class is exported from the APIController.dll
class APICONTROLLER_API CAPIController {
public:
	static CAPIController* GetInstance();
	int TeacherPTZUp();
	int TeacherPTZDown();
	int TeacherPTZLeft();
	int TeacherPTZRight();
	int TeacherPTZZoomIn();
	int TeacherPTZZoomOut();
	int TeacherPTZStop();

	int TeacherPTZSetPrePos(int id);
	int TeacherPTZRecallPrePos(int id);

private:
	CAPIController(void);
	~CAPIController();

	static CAPIController* m_pInstance;
	CModuleFactory* m_pModuleFactory;
	// TODO: add your methods here.
};

