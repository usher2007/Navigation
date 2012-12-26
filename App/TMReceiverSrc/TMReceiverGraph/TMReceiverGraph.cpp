// TMReceiverGraph.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "TMReceiverGraph.h"


// This is an example of an exported variable
TMRECEIVERGRAPH_API int nTMReceiverGraph=0;

// This is an example of an exported function.
TMRECEIVERGRAPH_API int fnTMReceiverGraph(void)
{
	return 42;
}

// This is the constructor of a class that has been exported.
// see TMReceiverGraph.h for the class definition
CTMReceiverGraph::CTMReceiverGraph()
{
	Init();
	return;
}

HRESULT CTMReceiverGraph::Init()
{
	CoInitialize(NULL);
	m_pGraphBuilder = NULL;
	m_pMediaControl = NULL;
	m_pMediaEvent = NULL;
	m_pMeidaSeeking = NULL;
	m_pVideoWindow = NULL;
	m_pBasicVideo = NULL;
	m_pRecordStream = NULL;
	m_pSetCallBack = NULL;
	m_bDisplay = FALSE;
	return S_OK;
}

CTMReceiverGraph::~CTMReceiverGraph()
{
	return;
}

HRESULT CTMReceiverGraph::Create()
{
	HRESULT hr = S_FALSE;

	m_pGraphBuilder = NULL;
	hr = CoCreateInstance(CLSID_FilterGraph, NULL, CLSCTX_INPROC_SERVER, IID_IGraphBuilder, (void **)&m_pGraphBuilder);
	if(FAILED(hr)) return hr;

	m_pMediaControl = NULL;
	hr = m_pGraphBuilder->QueryInterface(IID_IMediaControl, (void **)&m_pMediaControl);
	if(FAILED(hr)) return hr;

	m_pMediaEvent = NULL;
	hr = m_pGraphBuilder->QueryInterface(IID_IMediaEventEx, (void **)&m_pMediaEvent);
	if(FAILED(hr)) return hr;

	m_pMeidaSeeking = NULL;
	hr = m_pGraphBuilder->QueryInterface(IID_IMediaSeeking, (void **)&m_pMeidaSeeking);
	if(FAILED(hr)) return hr;

	m_pVideoWindow = NULL;
	hr = m_pGraphBuilder->QueryInterface(IID_IVideoWindow, (void **)&m_pVideoWindow);
	if(FAILED(hr)) return hr;

	m_pBasicVideo = NULL;
	hr = m_pGraphBuilder->QueryInterface(IID_IBasicVideo, (void **)&m_pBasicVideo);
	if(FAILED(hr)) return hr;

	return S_OK;
}

HRESULT CTMReceiverGraph::BuildFilterGraph(const char* fileName, BOOL bDisplay)
{
	HRESULT hr = S_FALSE;
	if(m_pGraphBuilder != NULL)
	{
		m_bDisplay = bDisplay;

		CComPtr<IBaseFilter> pSrc;
		hr = CoCreateInstance(CLSID_TMReceiverSrc, NULL, CLSCTX_INPROC_SERVER, IID_IBaseFilter, (void **)&pSrc);
		if(FAILED(hr)) return hr;
		hr = pSrc->QueryInterface(IID_IRecordStream, (void **)&m_pRecordStream);
		if(FAILED(hr)) return hr;
		hr = pSrc->QueryInterface(IID_ISetCallBack, (void **)&m_pSetCallBack);
		if(FAILED(hr)) return hr;
		CComPtr<IFileSourceFilter> pFileSrc;
		hr = pSrc->QueryInterface(IID_IFileSourceFilter, (void **)&pFileSrc);
		if(FAILED(hr)) return hr;
		CString fileNameCStr(fileName);
		hr = pFileSrc->Load(fileNameCStr, NULL);
		if(FAILED(hr)) return hr;
		hr = m_pGraphBuilder->AddFilter(pSrc, L"Src Filter");
		if(FAILED(hr)) return hr;

		CComPtr<IBaseFilter> pRenderer;
		if(bDisplay)
		{
			hr = CoCreateInstance(CLSID_VideoRenderer, NULL, CLSCTX_INPROC_SERVER, IID_IBaseFilter, (void **)&pRenderer);
			if(FAILED(hr)) return hr;
		}
		else
		{
			hr = CoCreateInstance(CLSID_NullRenderer, NULL, CLSCTX_INPROC_SERVER, IID_IBaseFilter, (void **)&pRenderer);
			if(FAILED(hr)) return hr;
		}
		hr = m_pGraphBuilder->AddFilter(pRenderer, L"Renderer");
		if(FAILED(hr)) return hr;

		hr = ConnectFilters(m_pGraphBuilder, pSrc, pRenderer, MEDIATYPE_NULL);
		if(FAILED(hr)) return hr;

		hr = SaveGraphFile(m_pGraphBuilder, L"H:\\TMReceiver.grf");
		return S_OK;
	}
	return E_FAIL;
}

HRESULT CTMReceiverGraph::SetDisplayWindow(HWND windowHandle)
{
	if(m_pVideoWindow)
	{
		m_pVideoWindow->put_Visible(OAFALSE);
		m_pVideoWindow->put_Owner((OAHWND)windowHandle);
		RECT windowRect;
		::GetClientRect(windowHandle, &windowRect);
		m_pVideoWindow->put_Left(0);
		m_pVideoWindow->put_Top(0);
		m_pVideoWindow->put_Width(windowRect.right - windowRect.left);
		m_pVideoWindow->put_Height(windowRect.bottom - windowRect.top);
		m_pVideoWindow->put_WindowStyle(WS_CHILD|WS_CLIPCHILDREN|WS_CLIPSIBLINGS);
		m_pVideoWindow->put_MessageDrain((OAHWND)windowHandle);

		if(windowHandle != NULL)
		{
			m_pVideoWindow->put_Visible(OATRUE);
		}
		else
		{
			m_pVideoWindow->put_Visible(OAFALSE);
		}
		return S_OK;
	}
	return E_FAIL;
}

HRESULT CTMReceiverGraph::SetNotifyWindow(HWND windowHandle)
{
	HRESULT hr = E_FAIL;
	if(m_pMediaEvent != NULL)
	{
		hr = m_pMediaEvent->SetNotifyWindow((OAHWND)windowHandle, WM_GRAPHNOTIFY, 0);
		return hr;
	}
	return E_FAIL;
}

HRESULT CTMReceiverGraph::SetBeforeDecodeCB(TMReceiverCB cb, void* arg)
{
	HRESULT hr = E_FAIL;
	if(m_pSetCallBack != NULL)
	{
		hr = m_pSetCallBack->SetCallBackBeforeDecode(cb, arg);
		return hr;
	}
	return E_FAIL;
}

HRESULT CTMReceiverGraph::SetAfterDecodeCB(TMReceiverCB cb, void* arg)
{
	HRESULT hr = E_FAIL;
	if(m_pSetCallBack != NULL)
	{
		hr = m_pSetCallBack->SetCallBackAfterDecode(cb, arg);
		return hr;
	}
	return E_FAIL;
}

void CTMReceiverGraph::OnSize()
{
	HWND hwnd;
	CWnd *pcwnd;
	CRect rect;
	m_pVideoWindow->get_Owner((OAHWND*)&hwnd);
	pcwnd=CWnd::FromHandle(hwnd);
	pcwnd->GetClientRect(&rect);
	m_pVideoWindow->SetWindowPosition(rect.left ,rect.top ,
		rect.right ,rect.bottom );
}

HRESULT CTMReceiverGraph::Stop()
{
	HRESULT hr = E_FAIL;
	if(m_pMediaControl != NULL)
	{
		hr = m_pMediaControl->Stop();
		return hr;
	}
	return E_FAIL;
}

HRESULT CTMReceiverGraph::Pause()
{
	HRESULT hr = E_FAIL;
	if(m_pMediaControl != NULL)
	{
		hr = m_pMediaControl->Pause();
		return hr;
	}
	return E_FAIL;
}

HRESULT CTMReceiverGraph::Run()
{
	HRESULT hr = E_FAIL;
	if(m_pMediaControl != NULL)
	{
		hr = m_pMediaControl->Run();
		return hr;
	}
	return E_FAIL;
}

HRESULT CTMReceiverGraph::StartRecord(const char* storageFileName)
{
	HRESULT hr = E_FAIL;
	if(m_pRecordStream != NULL)
	{
		hr = m_pRecordStream->StartRecord(storageFileName);
		return hr;
	}
	return E_FAIL;
}

HRESULT CTMReceiverGraph::StopRecord()
{
	HRESULT hr = E_FAIL;
	if(m_pRecordStream != NULL)
	{
		hr = m_pRecordStream->StopRecord();
		return hr;
	}
	return E_FAIL;
}

IMediaEventEx* CTMReceiverGraph::GetEventHandle()
{
	return m_pMediaEvent;
}

HRESULT CTMReceiverGraph::CheckMediaType(CComPtr<IEnumMediaTypes> pEnumMedia, GUID MediaType)
{
	AM_MEDIA_TYPE *MType = NULL;
	HRESULT hr = E_FAIL;
	while((hr = pEnumMedia->Next(1, &MType, NULL)) == S_OK)
	{
		if(MType->majortype == MediaType)
		{
			pEnumMedia->Reset();
			return S_OK;
		}
	}
	pEnumMedia->Reset();
	return E_FAIL;
}

HRESULT CTMReceiverGraph::ConnectFilters(CComPtr<IGraphBuilder> pGraph, CComPtr<IBaseFilter> pSrc, CComPtr<IBaseFilter> pDest, GUID MediaType)
{
	if(pGraph == NULL || pSrc == NULL || pDest == NULL)
		return E_POINTER;
	CComPtr<IPin> pOut = 0;
	HRESULT hr = GetUnconnectedPin(pSrc, PINDIR_OUTPUT, &pOut, MediaType);
	if (FAILED(hr))
		return hr;
	CComPtr<IPin> pIn = 0;
	hr = GetUnconnectedPin(pDest, PINDIR_INPUT, &pIn, MediaType);
	if (FAILED(hr))
		return hr;
	hr = pGraph->Connect(pOut, pIn);
	return hr;
}

HRESULT CTMReceiverGraph::GetUnconnectedPin(CComPtr<IBaseFilter> pFilter, PIN_DIRECTION PinDir, IPin **ppPin, GUID MediaType)
{
	*ppPin = 0;
	CComPtr<IEnumPins> pEnum = 0;
	CComPtr<IPin> pPin = 0;
	HRESULT hr = pFilter->EnumPins(&pEnum);
	if(FAILED(hr))
		return hr;
	while (pEnum->Next(1, &pPin, NULL) == S_OK)
	{
		PIN_DIRECTION ThisPinDir;
		pPin->QueryDirection(&ThisPinDir);
		if (ThisPinDir == PinDir)
		{
			CComPtr<IPin> pTmp = 0;
			hr = pPin->ConnectedTo(&pTmp);
			if (SUCCEEDED(hr))
			{}
			else
			{	
				CComPtr<IEnumMediaTypes> pEnumMediaType = NULL;
				hr = pPin->EnumMediaTypes(&pEnumMediaType);
				if (MediaType == MEDIATYPE_NULL)
				{
					*ppPin = pPin;
					return S_OK;
				}else if(SUCCEEDED(this->CheckMediaType(pEnumMediaType, MediaType)))
				{
					*ppPin = pPin;
					return S_OK;
				} 
			}
		}
	}
	return E_FAIL;
}

HRESULT CTMReceiverGraph::SaveGraphFile(CComPtr<IGraphBuilder> pGraph, WCHAR *wszPath) 
{
	const WCHAR wszStreamName[] = L"ActiveMovieGraph"; 
	HRESULT hr;

	IStorage *pStorage = NULL;
	hr = StgCreateDocfile(
		wszPath,
		STGM_CREATE | STGM_TRANSACTED | STGM_READWRITE | STGM_SHARE_EXCLUSIVE,
		0, &pStorage);
	if(FAILED(hr)) 
	{
		return hr;
	}

	IStream *pStream;
	hr = pStorage->CreateStream(
		wszStreamName,
		STGM_WRITE | STGM_CREATE | STGM_SHARE_EXCLUSIVE,
		0, 0, &pStream);
	if (FAILED(hr)) 
	{
		pStorage->Release();    
		return hr;
	}

	IPersistStream *pPersist = NULL;
	pGraph->QueryInterface(IID_IPersistStream, (void**)&pPersist);
	hr = pPersist->Save(pStream, TRUE);
	pStream->Release();
	pPersist->Release();
	if (SUCCEEDED(hr)) 
	{
		hr = pStorage->Commit(STGC_DEFAULT);
	}
	pStorage->Release();
	return hr;
}



