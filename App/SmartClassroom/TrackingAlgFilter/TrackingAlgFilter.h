#pragma once
#include "stdafx.h"
#include <streams.h>
#include "TrackingAlgUIDs.h"
#include "TrackingAlg.h"

class CTrackingAlgInputPin : public CTransInPlaceInputPin
{
public:
	CTrackingAlgInputPin(TCHAR *pObjectName, CTransInPlaceFilter *pTransInPlaceFilter, HRESULT *phr, LPCTSTR pName)
		: CTransInPlaceInputPin(pObjectName, pTransInPlaceFilter, phr, pName)
	{

	}

	HRESULT CheckMediaType(const CMediaType *pmt);
};

class CTrackingAlgOutputPin : public CTransInPlaceOutputPin
{
public:
	CTrackingAlgOutputPin(TCHAR *pObjectName, CTransInPlaceFilter *pTransInPlaceFilter, HRESULT *phr, LPCTSTR pName)
		: CTransInPlaceOutputPin(pObjectName, pTransInPlaceFilter, phr, pName)
	{

	}

	HRESULT CheckMediaType(const CMediaType *pmt);
};

class CTrackingAlgFilter : public CTransInPlaceFilter
{
	friend class CTrackingAlgInputPin;
	friend class CTrackingAlgOutputPin;

public:
	static CUnknown * WINAPI CreateInstance(LPUNKNOWN punk, HRESULT *phr);

	DECLARE_IUNKNOWN;

	//
	// --- CTransInPlaceFilter Overrides --
	//

	virtual CBasePin *GetPin(int n);
	HRESULT CheckInputType(const CMediaType *mtIn)
	{
		UNREFERENCED_PARAMETER(mtIn); return S_OK;
	}

	STDMETHODIMP NonDelegatingQueryInterface(REFIID riid, void ** ppv);

	HRESULT SetResolution(LONG biWidht, LONG biHeight);
	LONG GetWidth();
	LONG GetHeight();

private:

	CTrackingAlgFilter(TCHAR *tszName, LPUNKNOWN punk, HRESULT *phr);

	HRESULT Transform(IMediaSample *pSample);

	static int m_nInstanceCount;
	int m_nThisInstance;

	CMediaType m_mtPreferred;
	CCritSec m_TransAlgLock;

	LONG m_biWidth;
	LONG m_biHeight;

	TrackingAlg *m_pTrackingAlg;
};


