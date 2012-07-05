#include "stdafx.h"
#include "RecordGraph.h"

RecordGraph::RecordGraph(void)
{
	pBuilder = NULL;
	pControl = NULL;
}

RecordGraph::~RecordGraph(void)
{
	pControl->Release();
	pBuilder->Release();
}

HRESULT RecordGraph::Create(HWND hwnd)
{
	CoInitialize(NULL);
	owner = hwnd;
	pBuilder = NULL;
	HRESULT hr;
	hr = CoCreateInstance(CLSID_FilterGraph, NULL, CLSCTX_INPROC_SERVER, IID_IGraphBuilder, (void **)&pBuilder);
	if(FAILED(hr))
	{
		return hr;
	}

	pControl = NULL;
	hr = pBuilder->QueryInterface(IID_IMediaControl, (void **)&pControl);
	if(FAILED(hr))
	{
		return hr;
	}
}

HRESULT RecordGraph::BuildGraph(CString recordFileName)
{
	HRESULT hr;
	ICreateDevEnum *pSysAudioCaptureEnum = NULL;
	hr = CoCreateInstance(CLSID_SystemDeviceEnum, NULL, CLSCTX_INPROC_SERVER, IID_ICreateDevEnum, (void **)&pSysAudioCaptureEnum);
	if (FAILED(hr))
	{
		return hr;
	}

	IBaseFilter *pAudioCapture = NULL;
	IEnumMoniker *pEnumMoniker = NULL;
	hr = pSysAudioCaptureEnum->CreateClassEnumerator(CLSID_AudioInputDeviceCategory, &pEnumMoniker, 0);
	if (hr == S_OK)
	{
		IMoniker *pMoniker = NULL;
		ULONG fetched;
		BOOL findMicrophone = FALSE;
		while(pEnumMoniker->Next(1, &pMoniker, &fetched) == S_OK && !findMicrophone)
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
					if(friendlyName.Find(L"Microphone") >= 0)
					{
						hr = pMoniker->BindToObject(NULL, NULL, IID_IBaseFilter, (void **)&pAudioCapture);
						findMicrophone = TRUE;
					}
					VariantClear(&varName);
					pPropBag->Release();
				}
			}
		}
		pEnumMoniker->Release();
	}
	pSysAudioCaptureEnum->Release();
	if(pAudioCapture == NULL)
	{
		return S_FALSE;
	}
	pBuilder->AddFilter(pAudioCapture, L"Audio Capture");

	IBaseFilter *pWaveDest = NULL;
	hr = CoCreateInstance(CLSID_WavDest, NULL, CLSCTX_INPROC_SERVER, IID_IBaseFilter, (void **)&pWaveDest);
	if(FAILED(hr))
	{
		return hr;
	}
	pBuilder->AddFilter(pWaveDest, L"Wave Dest");

	IBaseFilter *pFileWriter = NULL;
	hr = CoCreateInstance(CLSID_FileWriter, NULL, CLSCTX_INPROC_SERVER, IID_IBaseFilter, (void **)&pFileWriter);
	if(FAILED(hr))
	{
		return hr;
	}
	pBuilder->AddFilter(pFileWriter, L"File Writer");
	IFileSinkFilter *pFileSetter = NULL;
	hr = pFileWriter->QueryInterface(IID_IFileSinkFilter, (void **)&pFileSetter);
	if(FAILED(hr))
	{
		return hr;
	}
	AM_MEDIA_TYPE pmt;
	pmt.majortype = MEDIATYPE_Stream;
	pmt.subtype = MEDIASUBTYPE_WAVE;
	pmt.formattype = FORMAT_WaveFormatEx;
	hr = pFileSetter->SetFileName(L"D:\\record.wav", &pmt);

	hr = ConnectFilters(pBuilder, pAudioCapture, pWaveDest, MEDIATYPE_NULL);if(FAILED(hr)) return hr;
	hr = ConnectFilters(pBuilder, pWaveDest, pFileWriter, MEDIATYPE_NULL);//if(FAILED(hr)) return hr;
	SaveGraphFile(pBuilder, L"D:\\Record.grf");

	pFileSetter->Release();
	pFileWriter->Release();
	pWaveDest->Release();
	pAudioCapture->Release();
}

HRESULT RecordGraph::Run()
{
	HRESULT hr = E_FAIL;
	if(pControl != NULL)
	{
		hr = pControl->Run();
	}
	return hr;
}

HRESULT RecordGraph::Stop()
{
	HRESULT hr = E_FAIL;
	if(pControl != NULL)
	{
		hr = pControl->Stop();
	}
	return hr;
}

HRESULT RecordGraph::CheckMediaType(IEnumMediaTypes *pEnumMedia, GUID MediaType)
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

HRESULT RecordGraph::ConnectFilters(IGraphBuilder *pGraph, IBaseFilter *pSrc, IBaseFilter *pDest, GUID MediaType)
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

HRESULT RecordGraph::GetUnconnectedPin(IBaseFilter *pFilter, PIN_DIRECTION PinDir, IPin **ppPin, GUID MediaType)
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

HRESULT RecordGraph::SaveGraphFile(IGraphBuilder *pGraph, WCHAR *wszPath) 
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
