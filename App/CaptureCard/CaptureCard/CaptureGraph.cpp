#include "stdafx.h"
#include "CaptureGraph.h"

CaptureGraph::CaptureGraph(void)
{
	CoInitialize(NULL);
	pGraph = NULL;
	pControl = NULL;
	pVideoWindow = NULL;
	pBasicVideo = NULL;
}

CaptureGraph::~CaptureGraph(void)
{
}

HRESULT CaptureGraph::Create(HWND hwnd)
{
	owner = hwnd;
	HRESULT hr = S_FALSE;
	hr = CoCreateInstance(CLSID_FilterGraph, NULL, CLSCTX_INPROC_SERVER, IID_IGraphBuilder, (void **)&pGraph);
	if(FAILED(hr)) return hr;

	hr = CoCreateInstance(CLSID_CaptureGraphBuilder2, NULL, CLSCTX_INPROC_SERVER, IID_ICaptureGraphBuilder2, (void **)&pCaptureBuilder2);
	if(FAILED(hr)) return hr;
	hr = pCaptureBuilder2->SetFiltergraph(pGraph);
	if(FAILED(hr)) return hr;

	hr = pGraph->QueryInterface(IID_IMediaControl, (void **)&pControl);
	if(FAILED(hr)) return hr;

	hr = pGraph->QueryInterface(IID_IVideoWindow, (void **)&pVideoWindow);
	if(FAILED(hr)) return hr;

	hr = pGraph->QueryInterface(IID_IBasicVideo, (void **)&pBasicVideo);
	if(FAILED(hr)) return hr;	

	return S_OK;
}

HRESULT CaptureGraph::BuildGraph(CString subName)
{
	if(pGraph != NULL)
	{
		HRESULT hr;
		CComPtr<ICreateDevEnum> pSysVideoCaptureEnum = NULL;
		hr = CoCreateInstance(CLSID_SystemDeviceEnum, NULL, CLSCTX_INPROC_SERVER, IID_ICreateDevEnum, (void **)&pSysVideoCaptureEnum);
		if(FAILED(hr)) return hr;

		CComPtr<IBaseFilter> pVideoCapture = NULL;
		CComPtr<IEnumMoniker> pEnumMoniker = NULL;
		hr = pSysVideoCaptureEnum->CreateClassEnumerator(CLSID_VideoInputDeviceCategory, &pEnumMoniker, 0);
		if(FAILED(hr)) return hr;

		CComPtr<IMoniker> pMoniker = NULL;
		ULONG fetched;

		while(pEnumMoniker->Next(1, &pMoniker, &fetched) == S_OK)
		{
			CComPtr<IPropertyBag> pPropBag;
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

					if(friendlyName.Find(subName) != -1)
					{
						pMoniker->BindToObject(NULL, NULL, IID_IBaseFilter, (void **)&pVideoCapture);
						break;
					}

				}
			}
		}

		if(pVideoCapture != NULL)
		{
			hr = pGraph->AddFilter(pVideoCapture, L"Video Capture");
			if(FAILED(hr)) return hr;
		}
		//Replace this to add nullip filter.
		hr = pCaptureBuilder2->RenderStream(&PIN_CATEGORY_PREVIEW, &MEDIATYPE_Video, pVideoCapture, NULL, NULL);
		if(FAILED(hr)) return hr;

		hr = SaveGraphFile(pGraph, L"D:\\VideoCapture.grf");
		return hr;
	}
	return E_FAIL;
}

HRESULT CaptureGraph::SetDisplayWindow(HWND windowHwnd)
{
	if(pVideoWindow != NULL)
	{
		pVideoWindow->put_Visible(OAFALSE);
		pVideoWindow->put_Owner((OAHWND)windowHwnd);
		RECT windowRect;
		::GetClientRect(windowHwnd, &windowRect);
		pVideoWindow->put_Left(0);
		pVideoWindow->put_Top(0);
		pVideoWindow->put_Width(windowRect.right - windowRect.left);
		pVideoWindow->put_Height(windowRect.bottom - windowRect.top);
		pVideoWindow->put_WindowStyle(WS_CHILD|WS_CLIPCHILDREN|WS_CLIPSIBLINGS);
		pVideoWindow->put_MessageDrain((OAHWND)windowHwnd);

		if(windowHwnd != NULL)
		{
			pVideoWindow->put_Visible(OATRUE);
		}
		else
		{
			pVideoWindow->put_Visible(OAFALSE);
		}
		return S_OK;
	}
	return E_FAIL;
}

HRESULT CaptureGraph::Run()
{
	HRESULT hr = E_FAIL;
	if(pControl != NULL)
	{
		hr = pControl->Run();
	}
	return hr;
}

HRESULT CaptureGraph::Pause()
{
	HRESULT hr = E_FAIL;
	if(pControl != NULL)
	{
		hr = pControl->Pause();
	}
	return hr;
}

HRESULT CaptureGraph::Stop()
{
	HRESULT hr = E_FAIL;
	if(pControl != NULL)
	{
		hr = pControl->Stop();
	}
	return hr;
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
