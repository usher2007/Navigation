// TMReceiver.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "TMReceiver.h"
#include "TMReceiverImp.h"

// This is an example of an exported variable
TMRECEIVER_API int nTMReceiver=0;

// This is an example of an exported function.
TMRECEIVER_API int fnTMReceiver(void)
{
	return 42;
}

TMRECEIVER_API TMRecieverHandle TM_RecieverCreate()
{
	TMReceiverImp* pReceiverImp = new TMReceiverImp;
	return(TMRecieverHandle)pReceiverImp;
}

TMRECEIVER_API int TM_RecieverOpenRtspStream(TMRecieverHandle hReciever, TMHandle h, char* rtspUrl)
{
	TMReceiverImp* pReceiverImp = (TMReceiverImp *)hReciever;
	return pReceiverImp->OpenRtspStream(rtspUrl);
}

TMRECEIVER_API int TM_RecieverOpenHttpStream(TMRecieverHandle hReciever, TMHandle h, char* strFileName)
{
	TMReceiverImp* pReceiverImp = (TMReceiverImp *)hReciever;
	return pReceiverImp->OpenHttpStream(strFileName);
}

TMRECEIVER_API int TM_RecieverEnableDisplay(TMRecieverHandle hReciever, HWND hWnd)
{
	TMReceiverImp* pReceiverImp = (TMReceiverImp *)hReciever;
	return pReceiverImp->EnableDisplay(hWnd);
}

TMRECEIVER_API int TM_RecieverDisableDisplay(TMRecieverHandle hReciever)
{
	TMReceiverImp* pReceiverImp = (TMReceiverImp *)hReciever;
	return pReceiverImp->DisableDisplay();
}

TMRECEIVER_API int TM_RecieverStartDisplay(TMRecieverHandle hReciever)
{
	TMReceiverImp* pReceiverImp = (TMReceiverImp *)hReciever;
	return pReceiverImp->StartDisplay();
}

TMRECEIVER_API int TM_RecieverStopDisplay(TMRecieverHandle hReciever)
{
	TMReceiverImp* pReceiverImp = (TMReceiverImp *)hReciever;
	return pReceiverImp->StopDisplay();
}

TMRECEIVER_API int TM_RecieverEnableStorage(TMRecieverHandle hReciever, const char* strFileName)
{
	TMReceiverImp* pReceiverImp = (TMReceiverImp *)hReciever;
	return pReceiverImp->EnableStorage(strFileName);
}

TMRECEIVER_API int TM_RecieverDisableStorage(TMRecieverHandle hReciever)
{
	TMReceiverImp* pReceiverImp = (TMReceiverImp *)hReciever;
	return pReceiverImp->DisableStorage();
}

TMRECEIVER_API int TM_RecieverStartStorage(TMRecieverHandle hReciever)
{
	TMReceiverImp* pReceiverImp = (TMReceiverImp *)hReciever;
	return pReceiverImp->StartStorage();
}

TMRECEIVER_API int TM_RecieverStopStorage(TMRecieverHandle hReciever)
{
	TMReceiverImp* pReceiverImp = (TMReceiverImp *)hReciever;
	return pReceiverImp->StopStorage();
}

TMRECEIVER_API int TM_RecieverSetCallBackBeforeDecode(TMRecieverHandle hReciever, TMReceiverCB* cb, void* arg)
{
	TMReceiverImp* pReceiverImp = (TMReceiverImp *)hReciever;
	return pReceiverImp->SetCallBackBeforeDecode(cb, arg);
}

TMRECEIVER_API int TM_RecieverSetCallBackAfterDecode(TMRecieverHandle hReciever, TMReceiverCB* cb, void* arg)
{
	TMReceiverImp* pReceiverImp = (TMReceiverImp *)hReciever;
	return pReceiverImp->SetCallBackAfterDecode(cb, arg);
}

TMRECEIVER_API int TM_RecieverClose(TMRecieverHandle hReciever)
{
	TMReceiverImp* pReceiverImp = (TMReceiverImp *)hReciever;
	return pReceiverImp->Close();
}

// This is the constructor of a class that has been exported.
// see TMReceiver.h for the class definition
CTMReceiver::CTMReceiver()
{
	return;
}
