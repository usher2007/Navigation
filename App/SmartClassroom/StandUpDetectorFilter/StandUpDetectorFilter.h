#pragma once
#include "stdafx.h"
#include <streams.h>
#include "StandUpDetectorUIDs.h"
#include "StandUpDetectAlg.h"

class CStandUpDetectorInputPin : public CTransInPlaceInputPin
{
public:
	CStandUpDetectorInputPin(TCHAR *pObjectName, CTransInPlaceFilter *pTransInPlaceFilter, HRESULT *phr, LPCTSTR pName)
		: CTransInPlaceInputPin(pObjectName, pTransInPlaceFilter, phr, pName)
	{

	}

	HRESULT CheckMediaType(const CMediaType *pmt);
};

class CStandUpDetectorOutputPin : public CTransInPlaceOutputPin
{
public:
	CStandUpDetectorOutputPin(TCHAR *pObjectName, CTransInPlaceFilter *pTransInPlaceFilter, HRESULT *phr, LPCTSTR pName)
		: CTransInPlaceOutputPin(pObjectName, pTransInPlaceFilter, phr, pName)
	{

	}

	HRESULT CheckMediaType(const CMediaType *pmt);
};

class CStandUpDetectorFilter : public CTransInPlaceFilter
{
	friend class CStandUpDetectorInputPin;
	friend class CStandUpDetectorOutputPin;

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

	CStandUpDetectorFilter(TCHAR *tszName, LPUNKNOWN punk, HRESULT *phr);

	HRESULT Transform(IMediaSample *pSample);
	HRESULT checkSoftDog();
	BOOL m_bSoftDogChecked;
	int m_nFrameCount;

	static int m_nInstanceCount;
	int m_nThisInstance;

	CMediaType m_mtPreferred;
	CCritSec m_TransAlgLock;

	LONG m_biWidth;
	LONG m_biHeight;

	BOOL m_bDetecting;

	StandUpDetectAlg *m_pStandUpAlg;
};


