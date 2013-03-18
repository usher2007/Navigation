#include "stdafx.h"
#include <atlconv.h>
#include <tchar.h>
#include <streams.h>
#include <initguid.h>
#include "TransAlgFilter.h"

#pragma warning(disable:4710)

CFactoryTemplate g_Templates[]=
{   {L"Trans Alg"
, &CLSID_TransAlg
,   CTransAlgFilter::CreateInstance
, NULL
, &sudNullIP }
};
int g_cTemplates = sizeof(g_Templates)/sizeof(g_Templates[0]);

STDAPI DllRegisterServer()
{
	return AMovieDllRegisterServer2( TRUE );
}

STDAPI DllUnregisterServer()
{
	return AMovieDllRegisterServer2( FALSE );
}


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
