// dllmain.cpp : Defines the entry point for the DLL application.
#include "stdafx.h"



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


// Microsoft C Compiler will give hundreds of warnings about
// unused inline functions in header files.  Try to disable them.
#pragma warning( disable:4514)
