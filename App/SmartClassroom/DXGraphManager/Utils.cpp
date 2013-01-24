#include "stdafx.h"
#include "Utils.h"

HRESULT CUtils::CheckMediaType(CComPtr<IEnumMediaTypes> pEnumMedia, GUID MediaType)
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

HRESULT CUtils::SaveGraphFile(CComPtr<IGraphBuilder> pGraph, WCHAR *wszPath)
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

HRESULT CUtils::ConnectFilters(CComPtr<IGraphBuilder> pGraph, CComPtr<IBaseFilter> pSrc, CComPtr<IBaseFilter> pDest, GUID MediaType)
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

HRESULT CUtils::GetUnconnectedPin(CComPtr<IBaseFilter> pFilter, PIN_DIRECTION PinDir, IPin **ppPin, GUID MediaType)
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
				}else if(SUCCEEDED(CUtils::CheckMediaType(pEnumMediaType, MediaType)))
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

HRESULT CUtils::AddFilter2(IGraphBuilder* pGraph, const GUID &clsid, LPCWSTR pName, IBaseFilter** ppFilter)
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