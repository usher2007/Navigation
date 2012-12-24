// dllmain.cpp : Defines the entry point for the DLL application.
#include "stdafx.h"
#include <atlconv.h>
#include <tchar.h>
#include <streams.h>
#include <initguid.h>
#include "TMReceiverSrc.h"


#pragma warning(disable:4710)  // 'function': function not inlined (optimzation)

// Setup data
// COM global table of objects in this dll

CFactoryTemplate g_Templates[] = {
	{ L"TMReceiver Source Filter", &CLSID_TMReceiverSrc, CTMReceiverSrc::CreateInstance, NULL, &sudTMReceiverSrcax }
};
int g_cTemplates = sizeof(g_Templates) / sizeof(g_Templates[0]);


////////////////////////////////////////////////////////////////////////
//
// Exported entry points for registration and unregistration 
// (in this case they only call through to default implementations).
//
////////////////////////////////////////////////////////////////////////

//
// DllRegisterServer
//
// Exported entry points for registration and unregistration
//
STDAPI DllRegisterServer()
{
	return AMovieDllRegisterServer2(TRUE);
} // DllRegisterServer


//
// DllUnregisterServer
//
STDAPI DllUnregisterServer()
{
	return AMovieDllRegisterServer2(FALSE);
} // DllUnregisterServer


//
// DllEntryPoint
//
extern "C" BOOL WINAPI DllEntryPoint(HINSTANCE, ULONG, LPVOID);

BOOL APIENTRY DllMain(HANDLE hModule, 
	DWORD  dwReason, 
	LPVOID lpReserved)
{
	return DllEntryPoint((HINSTANCE)(hModule), dwReason, lpReserved);
}
