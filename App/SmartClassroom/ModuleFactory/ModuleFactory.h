// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the MODULEFACTORY_EXPORTS
// symbol defined on the command line. This symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// MODULEFACTORY_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
#ifdef MODULEFACTORY_EXPORTS
#define MODULEFACTORY_API __declspec(dllexport)
#else
#define MODULEFACTORY_API __declspec(dllimport)
#endif

#include "CameraController.h"
#include "ConfigManager.h"
#include "DXGraphManager.h"

// This class is exported from the ModuleFactory.dll
class MODULEFACTORY_API CModuleFactory {
public:
	static CModuleFactory* GetInstance();
	CCameraController * GetCameraController();
	CConfigManager * GetConfigManager();
	CDXGraphManager * GetGraphManager();

private:
	CModuleFactory(void);
	~CModuleFactory();
	static CModuleFactory *m_pInstance;

	CCameraController * m_pCameraController;
	CConfigManager * m_pConfigManager;
	CDXGraphManager * m_pGraphManager;
	// TODO: add your methods here.
};

