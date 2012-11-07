#pragma once
#include "StdAfx.h"
//#include <initguid.h>
#include "CaptureGraph.h"
#include "afxdialogex.h"
#include <afxwin.h>


CaptureGraph::CaptureGraph(void)
{
	CoInitialize(NULL);
	pGraph = NULL;
	pControl = NULL;
	pEvent = NULL;
	pSeeking = NULL;
	mVideoWindow = NULL;
	pBasicVideo = NULL;
}


CaptureGraph::~CaptureGraph(void)
{
	mVideoWindow->Release();
	pControl->Release();
	pBasicVideo->Release();
	pSeeking->Release();
	pEvent->Release();
	//pGraph->Release();
	//pGraph = NULL;
}

HRESULT CaptureGraph::Create(HWND hwnd)
{
	owner = hwnd;
	bFullScreen = FALSE;
	pGraph = NULL;
	HRESULT hr = CoCreateInstance(CLSID_FilterGraph, NULL, CLSCTX_INPROC_SERVER, IID_IGraphBuilder, (void **)&pGraph);
	if(FAILED(hr))
	{
		return hr;
	}

	pCaptureBuilder2 = NULL;
	hr = CoCreateInstance(CLSID_CaptureGraphBuilder2, NULL, CLSCTX_INPROC_SERVER, IID_ICaptureGraphBuilder2, (void **)&pCaptureBuilder2);
	if(FAILED(hr))
	{
		return hr;
	}
	pCaptureBuilder2->SetFiltergraph(pGraph);
	pControl = NULL;
	hr = pGraph->QueryInterface(IID_IMediaControl, (void **)&pControl);
	if(FAILED(hr))
	{
		return hr;
	}
	pEvent = NULL;
	hr = pGraph->QueryInterface(IID_IMediaEventEx, (void **)&pEvent);
	if(FAILED(hr))
	{
		return hr;
	}
	pSeeking = NULL;
	hr = pGraph->QueryInterface(IID_IMediaSeeking,(void **)&pSeeking);
	if(FAILED(hr))
	{
		return hr;
	}

	mVideoWindow = NULL;
	hr = pGraph->QueryInterface(IID_IVideoWindow, (void **)&mVideoWindow);
	if (FAILED(hr)) return hr;

	pBasicVideo = NULL;
	hr = pGraph->QueryInterface(IID_IBasicVideo, (void **)&pBasicVideo);
	if (FAILED(hr)) return hr; 

	return hr;
}

HRESULT CaptureGraph::BuildGraphFromListBox(CListBox *captureFilterList, int selectedIndex)
{
	if(pGraph != NULL)
	{
		HRESULT hr;
		//dump the graph
		IMoniker *pSelectedCaptureMoniker = (IMoniker *)captureFilterList->GetItemDataPtr(selectedIndex);
		IBaseFilter *pCaptureFilter = NULL;
		hr = pSelectedCaptureMoniker->BindToObject(NULL, NULL, IID_IBaseFilter, (void **)&pCaptureFilter);
		if(FAILED(hr))
		{
			return hr;
		}
		pGraph->AddFilter(pCaptureFilter, L"Video Capture");
		hr = pCaptureBuilder2->RenderStream(&PIN_CATEGORY_PREVIEW, &MEDIATYPE_Video, pCaptureFilter, NULL, NULL);
		if(FAILED(hr))
		{
			return hr;
		}
		hr = SaveGraphFile(pGraph, L"D:\\VideoCapture.grf");
		return hr;
	}
	return E_FAIL;
}

HRESULT CaptureGraph::CheckMediaType(IEnumMediaTypes *pEnumMedia, GUID MediaType)
{
	AM_MEDIA_TYPE *MType = NULL;
	HRESULT hr=S_OK;
	while((hr=pEnumMedia->Next(1, &MType, NULL)) == S_OK)
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

HRESULT CaptureGraph::Stop()
{
	if(pControl != NULL)
	{
		pControl->Pause();
		HRESULT hr = pControl->Stop();
		if (pBasicVideo)
		{
			hr = pControl->Stop();
		}
		return hr;
	}
	return E_FAIL;
}

HRESULT CaptureGraph::Pause()
{
	if(pControl != NULL)
	{
		HRESULT hr = pControl->Pause();
		return hr;
	}
	return E_FAIL;
}

IMediaEventEx* CaptureGraph::GetEventHandle()
{
	return pEvent;
}

HRESULT CaptureGraph::SetNotifyWindow(HWND hwnd)
{
	if(pEvent != NULL)
	{
		HRESULT hr = pEvent->SetNotifyWindow((OAHWND)hwnd, WM_GRAPHNOTIFY, 0);
		return hr;
	}
	return E_FAIL;
}

HRESULT CaptureGraph::Run()
{
	if(pControl)
	{
		HRESULT hr = pControl->Run();
		char tmp[1024];
		sprintf(tmp,"%s","---------------------------------------------------------------------------------------");
		OutputDebugStringA(tmp);
		return hr;
	}
	return E_FAIL;
}

HRESULT CaptureGraph::FullScreen(HWND hwnd)
{
	if(mVideoWindow)
	{
		mVideoWindow->get_MessageDrain((OAHWND)NULL);
		mVideoWindow->put_MessageDrain((OAHWND)hwnd);
		mVideoWindow->put_Owner((OAHWND)NULL);

		mVideoWindow->put_WindowStyle(0);
		mVideoWindow->put_WindowStyleEx(WS_EX_TOPMOST | WS_POPUP);
		int width = GetSystemMetrics(SM_CXSCREEN);
		int height = GetSystemMetrics(SM_CYSCREEN);
		HRESULT hr = mVideoWindow->SetWindowPosition(0, 0, width, height);

		bFullScreen = TRUE;
		return S_OK;
	}
	return E_FAIL;
}


HRESULT CaptureGraph::SetDisplayWindow(HWND hwnd)
{
	if (mVideoWindow)
	{
		mVideoWindow->put_Visible(OAFALSE);
		mVideoWindow->put_Owner((OAHWND)hwnd);
		RECT windowRect;
		::GetClientRect(hwnd, &windowRect);
		mVideoWindow->put_Left(0);
		mVideoWindow->put_Top(0);
		mVideoWindow->put_Width(windowRect.right - windowRect.left);
		mVideoWindow->put_Height(windowRect.bottom - windowRect.top);
		mVideoWindow->put_WindowStyle(WS_CHILD|WS_CLIPCHILDREN|WS_CLIPSIBLINGS);
		mVideoWindow->put_MessageDrain((OAHWND)hwnd);
		
		if (hwnd != NULL)
		{
			mVideoWindow->put_Visible(OATRUE);
		}
		else
		{
			mVideoWindow->put_Visible(OAFALSE);
		}
		return S_OK;

	}
	return E_FAIL;
}

HRESULT CaptureGraph::ConnectFilters(IGraphBuilder *pGraph, IBaseFilter *pSrc, IBaseFilter *pDest, GUID MediaType)
{
	if(pGraph == NULL || pSrc == NULL || pDest == NULL)
		return E_POINTER;
	IPin *pOut = 0;
	HRESULT hr = GetUnconnectedPin(pSrc, PINDIR_OUTPUT, &pOut, MediaType);
	if (FAILED(hr))
		return hr;
	IPin *pIn = 0;
	hr = GetUnconnectedPin(pDest, PINDIR_INPUT, &pIn, MediaType);
	if (FAILED(hr))
		return hr;
	hr = pGraph->Connect(pOut, pIn);
	pOut->Release();
	pIn->Release();
	return hr;
}

HRESULT CaptureGraph::GetUnconnectedPin(IBaseFilter *pFilter, PIN_DIRECTION PinDir, IPin **ppPin, GUID MediaType)
{
	*ppPin = 0;
	IEnumPins *pEnum = 0;
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
			IPin *pTmp = 0;
			hr = pPin->ConnectedTo(&pTmp);
			if (SUCCEEDED(hr))
				pTmp->Release();
			else
			{	IEnumMediaTypes *pEnumMediaType = NULL;
				hr = pPin->EnumMediaTypes(&pEnumMediaType);
				if (MediaType == MEDIATYPE_NULL)
				{
					pEnum->Release();
					*ppPin = pPin;
					pPin->Release();
					return S_OK;
				}else if(SUCCEEDED(this->CheckMediaType(pEnumMediaType, MediaType)))
				{
					pEnum->Release();
					*ppPin = pPin;
					pPin->Release();
					return S_OK;
				} 
				pEnumMediaType->Release();
			}
		}
		pPin->Release();
	}
	pEnum->Release();
	return E_FAIL;
}

BOOL CaptureGraph::IsFullScreen()
{
	return bFullScreen;
}

HRESULT CaptureGraph::EscFullScreen(HWND hwnd)
{
	if(mVideoWindow)
	{
		mVideoWindow->put_MessageDrain((OAHWND)NULL);
		mVideoWindow->put_Visible(OAFALSE);
		mVideoWindow->put_Owner((OAHWND)hwnd);
		mVideoWindow->put_WindowStyle(WS_CHILD | WS_CLIPSIBLINGS|WS_CLIPCHILDREN);
		mVideoWindow->SetWindowForeground(OATRUE);
		OnSize();
		bFullScreen = FALSE;
		return S_OK;
	}
	return E_FAIL;
}

void CaptureGraph::OnSize()
{
	HWND hwnd;
    CWnd *pcwnd;
    CRect rect;
    mVideoWindow->get_Owner((OAHWND*)&hwnd);
    pcwnd=CWnd::FromHandle(hwnd);
    pcwnd->GetClientRect(&rect);
    mVideoWindow->SetWindowPosition(rect.left ,rect.top ,
                  rect.right ,rect.bottom );
}

HRESULT CaptureGraph::SaveGraphFile(IGraphBuilder *pGraph, WCHAR *wszPath) 
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


HRESULT CaptureGraph::GetVideoSize( long* pWidth, long* pHeight )
{
	if(pBasicVideo)
	{
		return pBasicVideo->GetVideoSize(pWidth, pHeight);
	}
	return E_FAIL;
}

HRESULT CaptureGraph::GetCaptureFilters(int *filterCount, CListBox *captureFilterList)
{
	HRESULT hr;
	int maxFilterCount = *filterCount;
	ICreateDevEnum *pSysVideoCaptureEnum = NULL;
	hr = CoCreateInstance(CLSID_SystemDeviceEnum, NULL, CLSCTX_INPROC_SERVER, IID_ICreateDevEnum, (void **)&pSysVideoCaptureEnum);
	if (FAILED(hr))
	{
		return hr;
	}
	IBaseFilter *pVideoCapture = NULL;
	IEnumMoniker *pEnumMoniker = NULL;
	hr = pSysVideoCaptureEnum->CreateClassEnumerator(CLSID_VideoInputDeviceCategory, &pEnumMoniker, 0);
	if (hr == S_OK)
	{
		IMoniker *pMoniker = NULL;
		ULONG fetched;
		int filterIndex = 0;
		while(pEnumMoniker->Next(1, &pMoniker, &fetched) == S_OK)
		{
			IPropertyBag *pPropBag;
			hr = pMoniker->BindToStorage(0, 0, IID_IPropertyBag, (void **)&pPropBag);
			if(SUCCEEDED(hr))
			{
				VARIANT varName;
				VariantInit(&varName);
				hr = pPropBag->Read(L"FriendlyName", &varName, 0);
				if(SUCCEEDED(hr))
				{
					ASSERT(varName.vt == VT_BSTR);
					CString friendlyName(varName.bstrVal);
					
					VariantClear(&varName);
					pPropBag->Release();

					filterIndex ++;
					captureFilterList->AddString(friendlyName);
					int currentFilterIndex = captureFilterList->FindStringExact(-1, friendlyName);
					captureFilterList->SetItemDataPtr(currentFilterIndex, pMoniker);
					if(filterIndex > maxFilterCount)
					{
						*filterCount = filterIndex;
						break;
					}
				}
			}
		}
		pEnumMoniker->Release();
	}
	pSysVideoCaptureEnum->Release();
}