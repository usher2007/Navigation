// TMGraph.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "TMGraph.h"


// This is an example of an exported variable
TMGRAPH_API int nTMGraph=0;

// This is an example of an exported function.
TMGRAPH_API int fnTMGraph(void)
{
	return 42;
}

// This is the constructor of a class that has been exported.
// see TMGraph.h for the class definition
CTMGraph::CTMGraph()
{
	Init();
	return;
}

HRESULT CTMGraph::Init()
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

CTMGraph::~CTMGraph()
{
	return;
}

HRESULT CTMGraph::Create()
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
	//hr = m_pGraphBuilder->QueryInterface(IID_IVideoWindow, (void **)&m_pVideoWindow);
	//if(FAILED(hr)) return hr;

	m_pBasicVideo = NULL;
	hr = m_pGraphBuilder->QueryInterface(IID_IBasicVideo, (void **)&m_pBasicVideo);
	if(FAILED(hr)) return hr;

	return S_OK;
}

HRESULT CTMGraph::BuildFilterGraph(const char* fileName, BOOL bDisplay)
{
	HRESULT hr = S_FALSE;
	if(m_pGraphBuilder != NULL)
	{
		m_bDisplay = bDisplay;
		BOOL hasAudio = FALSE;
		//Src Filter
		CComPtr<IBaseFilter> pSrc;
		hr = CoCreateInstance(CLSID_StreamReceiver, NULL, CLSCTX_INPROC_SERVER, IID_IBaseFilter, (void **)&pSrc);
		if(FAILED(hr)) return hr;
		hr = pSrc->QueryInterface(IID_IRecordTMStream, (void **)&m_pRecordStream);
		if(FAILED(hr)) return hr;
		hr = pSrc->QueryInterface(IID_ISetTMCallBack, (void **)&m_pSetCallBack);
		if(FAILED(hr)) return hr;
		CComPtr<IFileSourceFilter> pFileSrc;
		hr = pSrc->QueryInterface(IID_IFileSourceFilter, (void **)&pFileSrc);
		if(FAILED(hr)) return hr;
		CString fileNameCStr(fileName);
		hr = pFileSrc->Load(fileNameCStr, NULL);
		if(FAILED(hr)) return hr;
		hr = m_pGraphBuilder->AddFilter(pSrc, L"Src Filter");
		if(FAILED(hr)) return hr;

		//Inf Tee
		CComPtr<IBaseFilter> pInfTee;
		hr = CoCreateInstance(CLSID_InfTee, NULL, CLSCTX_INPROC_SERVER, IID_IBaseFilter, (void **)&pInfTee);
		if(FAILED(hr)) return hr;
		hr = m_pGraphBuilder->AddFilter(pInfTee, L"Inf Tee");
		if(FAILED(hr)) return hr;

		//Normal Renderer
		CComPtr<IBaseFilter> pNormalRenderer;
		if(bDisplay)
		{
			hr = CoCreateInstance(CLSID_VideoRenderer, NULL, CLSCTX_INPROC_SERVER, IID_IBaseFilter, (void **)&pNormalRenderer);
			if(FAILED(hr)) return hr;
			m_pVideoWindow = NULL;
			hr = pNormalRenderer->QueryInterface(IID_IVideoWindow, (void **)&m_pVideoWindow);
			if(FAILED(hr)) return hr;
		}
		else
		{
			hr = CoCreateInstance(CLSID_NullRenderer, NULL, CLSCTX_INPROC_SERVER, IID_IBaseFilter, (void **)&pNormalRenderer);
			if(FAILED(hr)) return hr;
		}
		hr = m_pGraphBuilder->AddFilter(pNormalRenderer, L"Renderer");
		if(FAILED(hr)) return hr;

		//Decklink Renderer
		CComPtr<IBaseFilter> pDecklinkRenderer;
		hr = CoCreateInstance(CLSID_DecklinkRenderer, NULL, CLSCTX_INPROC_SERVER, IID_IBaseFilter, (void **)&pDecklinkRenderer);
		if(FAILED(hr)) return hr;
		hr = m_pGraphBuilder->AddFilter(pDecklinkRenderer, L"Decklink Renderer");
		if(FAILED(hr)) return hr;


		CComPtr<IGetSrcStatus> pGetSrcStatus = NULL;
		hr = pSrc->QueryInterface(IID_IGetSrcStatus, (void **)&pGetSrcStatus);
		if(FAILED(hr)) return hr;
		hasAudio = SUCCEEDED(pGetSrcStatus->IsSourceHasAudio());

		CComPtr<IBaseFilter> pDecklinkAudioCapture = NULL;
		CComPtr<IBaseFilter> pAudioInfTee = NULL;
		CComPtr<IBaseFilter> pAudioRenderer = NULL;
		if(!hasAudio)
		{
			//Decklink Audio Capture
			hr = AddFilter2(m_pGraphBuilder, CLSID_AudioInputDeviceCategory, L"Decklink Audio Capture", &pDecklinkAudioCapture);
			if(FAILED(hr)) return hr;
		}
		else
		{
			//Audio Inf Tee
			hr = CoCreateInstance(CLSID_InfTee, NULL, CLSCTX_INPROC_SERVER, IID_IBaseFilter, (void **)&pAudioInfTee);
			if(FAILED(hr)) return hr;
			hr = m_pGraphBuilder->AddFilter(pAudioInfTee, L"Audio InfTee");
			if(FAILED(hr)) return hr;

			//Normal Audio Renderer
			hr = CoCreateInstance(CLSID_AudioRender, NULL, CLSCTX_INPROC_SERVER, IID_IBaseFilter, (void **)&pAudioRenderer);
			if(FAILED(hr)) return hr;
			hr = m_pGraphBuilder->AddFilter(pAudioRenderer, L"Audio Renderer");
			if(FAILED(hr)) return hr;
		}
		//Decklink Audio Renderer
		CComPtr<IBaseFilter> pDecklinkAudioRenderer = NULL;
		hr = AddFilter2(m_pGraphBuilder, CLSID_AudioRendererCategory, L"Decklink Audio Render", &pDecklinkAudioRenderer);
		if(FAILED(hr)) return hr;

		//Connect Filters
		hr = ConnectFilters(m_pGraphBuilder, pSrc, pInfTee, MEDIATYPE_NULL);
		if(FAILED(hr)) return hr;
		hr = ConnectFilters(m_pGraphBuilder, pInfTee, pNormalRenderer, MEDIATYPE_NULL);
		if(FAILED(hr)) return hr;
		hr = ConnectFilters(m_pGraphBuilder, pInfTee, pDecklinkRenderer, MEDIATYPE_NULL);
		if(FAILED(hr)) return hr;
		if(!hasAudio)
		{
			hr = ConnectFilters(m_pGraphBuilder, pDecklinkAudioCapture, pDecklinkAudioRenderer, MEDIATYPE_NULL);
			if(FAILED(hr)) return hr;
		}
		else
		{
			hr = ConnectFilters(m_pGraphBuilder, pSrc, pAudioInfTee, MEDIATYPE_NULL);
			if(FAILED(hr)) return hr;
			hr = ConnectFilters(m_pGraphBuilder, pAudioInfTee, pAudioRenderer, MEDIATYPE_NULL);
			if(FAILED(hr)) return hr;
			hr = ConnectFilters(m_pGraphBuilder, pAudioInfTee, pDecklinkAudioRenderer, MEDIATYPE_NULL);
			if(FAILED(hr)) return hr;

			//hr = ConnectFilters(m_pGraphBuilder, pSrc, pDecklinkAudioRenderer, MEDIATYPE_NULL);
		}

		hr = SaveGraphFile(m_pGraphBuilder, L"F:\\yubo\\DecklinkDemo.grf");
		return S_OK;
	}
	return E_FAIL;
}

HRESULT CTMGraph::Destroy()
{
	HRESULT hr = S_OK;

	// release the outstanding interface reference
	m_pMediaEvent = NULL;
	m_pMeidaSeeking = NULL;
	m_pRecordStream = NULL;
	m_pSetCallBack = NULL;
	m_pVideoWindow = NULL;

	if (m_pGraphBuilder && m_pMediaControl)
	{
		m_pMediaControl->Stop();

		// enumerate all the filters in the graph
		CComPtr<IEnumFilters> pEnum = NULL;
		hr = m_pGraphBuilder->EnumFilters(&pEnum);
		if (SUCCEEDED(hr))
		{
			IBaseFilter* pFilter = NULL;
			while (S_OK == pEnum->Next(1, &pFilter, NULL))
			{
				FILTER_INFO filterInfo;
				if (SUCCEEDED(pFilter->QueryFilterInfo(&filterInfo)))
				{
					SAFE_RELEASE(filterInfo.pGraph);
					if ((NULL == wcsstr(filterInfo.achName, L"Src Filter")) 
						&& (NULL == wcsstr(filterInfo.achName, L"Decklink Renderer")) 
						&& (NULL == wcsstr(filterInfo.achName, L"Inf Tee")) 
						&& (NULL == wcsstr(filterInfo.achName, L"Renderer"))
						&& (NULL == wcsstr(filterInfo.achName, L"Decklink Audio Capture"))
						&& (NULL == wcsstr(filterInfo.achName, L"Decklink Audio Render"))
						&& (NULL == wcsstr(filterInfo.achName, L"Audio InfTee"))
						&& (NULL == wcsstr(filterInfo.achName, L"Audio Renderer")))
					{
						// not the push source, infinite tee or renderer filter so remove from graph
						hr = m_pGraphBuilder->RemoveFilter(pFilter);
						if (SUCCEEDED(hr))
						{
							hr = pEnum->Reset();
						}
					}
					else
					{
						// enumerate all the pins on the filter
						CComPtr<IEnumPins> pIEnumPins = NULL;
						hr = pFilter->EnumPins(&pIEnumPins);
						if (SUCCEEDED(hr))
						{
							IPin* pIPin = NULL;
							while (S_OK == pIEnumPins->Next(1, &pIPin, NULL))
							{
								m_pGraphBuilder->Disconnect(pIPin);
								SAFE_RELEASE(pIPin);
							}
						}
					}
				}
				SAFE_RELEASE(pFilter);
			}
		}
	}
	else
	{
		hr = S_FALSE;
	}

	return hr;
}

HRESULT CTMGraph::SetDisplayWindow(HWND windowHandle)
{
	if(m_pVideoWindow && m_bDisplay)
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

HRESULT CTMGraph::SetNotifyWindow(HWND windowHandle)
{
	HRESULT hr = E_FAIL;
	if(m_pMediaEvent != NULL)
	{
		hr = m_pMediaEvent->SetNotifyWindow((OAHWND)windowHandle, WM_GRAPHNOTIFY, 0);
		return hr;
	}
	return E_FAIL;
}

HRESULT CTMGraph::SetBeforeDecodeCB(TMReceiverCB cb, void* arg)
{
	HRESULT hr = E_FAIL;
	if(m_pSetCallBack != NULL)
	{
		hr = m_pSetCallBack->SetCallBackBeforeDecode(cb, arg);
		return hr;
	}
	return E_FAIL;
}

HRESULT CTMGraph::SetAfterDecodeCB(TMReceiverCB cb, void* arg)
{
	HRESULT hr = E_FAIL;
	if(m_pSetCallBack != NULL)
	{
		hr = m_pSetCallBack->SetCallBackAfterDecode(cb, arg);
		return hr;
	}
	return E_FAIL;
}

void CTMGraph::OnSize()
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

HRESULT CTMGraph::Stop()
{
	HRESULT hr = E_FAIL;
	if(m_pMediaControl != NULL)
	{
		hr = m_pMediaControl->Stop();
		return hr;
	}
	return E_FAIL;
}

HRESULT CTMGraph::Pause()
{
	HRESULT hr = E_FAIL;
	if(m_pMediaControl != NULL)
	{
		hr = m_pMediaControl->Pause();
		return hr;
	}
	return E_FAIL;
}

HRESULT CTMGraph::Run()
{
	HRESULT hr = E_FAIL;
	if(m_pMediaControl != NULL)
	{
		hr = m_pMediaControl->Run();
		return hr;
	}
	return E_FAIL;
}

HRESULT CTMGraph::StartRecord(const char* storageFileName)
{
	HRESULT hr = E_FAIL;
	/*char tmpFileName[1024];
	memset(tmpFileName, 0x00, 1024);
	memcpy(tmpFileName, storageFileName, strlen(storageFileName));*/
	if(m_pRecordStream != NULL)
	{
		hr = m_pRecordStream->StartRecord(storageFileName);
		return hr;
	}
	return E_FAIL;
}

HRESULT CTMGraph::StopRecord()
{
	HRESULT hr = E_FAIL;
	if(m_pRecordStream != NULL)
	{
		hr = m_pRecordStream->StopRecord();
		return hr;
	}
	return E_FAIL;
}

IMediaEventEx* CTMGraph::GetEventHandle()
{
	return m_pMediaEvent;
}

HRESULT CTMGraph::CheckMediaType(CComPtr<IEnumMediaTypes> pEnumMedia, GUID MediaType)
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

HRESULT CTMGraph::ConnectFilters(CComPtr<IGraphBuilder> pGraph, CComPtr<IBaseFilter> pSrc, CComPtr<IBaseFilter> pDest, GUID MediaType)
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

HRESULT CTMGraph::GetUnconnectedPin(CComPtr<IBaseFilter> pFilter, PIN_DIRECTION PinDir, IPin **ppPin, GUID MediaType)
{
	*ppPin = 0;
	CComPtr<IEnumPins> pEnum = 0;
	IPin *pPin = 0;
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
		SAFE_RELEASE(pPin);
	}
	return E_FAIL;
}

HRESULT CTMGraph::SaveGraphFile(CComPtr<IGraphBuilder> pGraph, WCHAR *wszPath) 
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

HRESULT CTMGraph::AddFilter2(IGraphBuilder* pGraph, const GUID &clsid, LPCWSTR pName, IBaseFilter** ppFilter)
{
	HRESULT hr = S_OK;

	if (pGraph && pName && ppFilter)
	{
		// first enumerate the system devices for the specifed class and filter name
		CComPtr<ICreateDevEnum> pSysDevEnum = NULL;
		hr = CoCreateInstance(CLSID_SystemDeviceEnum, NULL, CLSCTX_INPROC_SERVER, IID_ICreateDevEnum, reinterpret_cast<void**>(&pSysDevEnum));

		if (SUCCEEDED(hr))
		{
			CComPtr<IEnumMoniker> pEnumCat = NULL;
			hr = pSysDevEnum->CreateClassEnumerator(clsid, &pEnumCat, 0);

			if (S_OK == hr)
			{
				IMoniker* pMoniker = NULL;
				bool Loop = true;
				while ((S_OK == pEnumCat->Next(1, &pMoniker, NULL)) && Loop)
				{
					IPropertyBag* pPropBag = NULL;
					hr = pMoniker->BindToStorage(0, 0, IID_IPropertyBag, reinterpret_cast<void**>(&pPropBag));

					if (SUCCEEDED(hr))
					{
						VARIANT varName;
						VariantInit(&varName);
						hr = pPropBag->Read(L"FriendlyName", &varName, 0);
						if (SUCCEEDED(hr))
						{
							if (0 == wcscmp(varName.bstrVal, pName))
							{
								hr = pMoniker->BindToObject(NULL, NULL, IID_IBaseFilter, reinterpret_cast<void**>(ppFilter));
								Loop = false;
							}
						}

						VariantClear(&varName);

						// contained within a loop, decrement the reference count
						SAFE_RELEASE(pPropBag);
					}
					SAFE_RELEASE(pMoniker);
				}
			}
		}

		// if a filter has been located add it to the graph
		if (*ppFilter)
		{
			hr = pGraph->AddFilter(reinterpret_cast<IBaseFilter*>(*ppFilter), pName);
		}
		else
		{
			hr = E_FAIL;
		}
	}
	else
	{
		hr = E_INVALIDARG;
	}

	return hr;
}