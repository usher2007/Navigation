#pragma once
#include "TMReceiverGraph.h"
#include "TMReceiver.h"


class TMReceiverImp
{
public:
	TMReceiverImp();
	HRESULT OpenRtspStream(const char* rtspUrl);
	HRESULT OpenHttpStream(const char* strFileName);
	HRESULT EnableDisplay(HWND hwnd);
	HRESULT DisableDisplay();
	HRESULT StartDisplay();
	HRESULT StopDisplay();
	HRESULT EnableStorage(const char* strFileName);
	HRESULT DisableStorage();
	HRESULT StartStorage();
	HRESULT StopStorage();
	HRESULT SetCallBackBeforeDecode(TMReceiverCB cb, void* arg);
	HRESULT SetCallBackAfterDecode(TMReceiverCB cb, void* arg);
	HRESULT Close();
private:
	CTMReceiverGraph *m_pGraph;
	BOOL m_bDisplay;
	BOOL m_bStorage;
	char m_storageFileName[1024];
	BOOL m_bRunning;
	BOOL m_bStoring;
	HWND m_hDisplayWnd;
private:
	HRESULT openStream(const char* streamName);
};