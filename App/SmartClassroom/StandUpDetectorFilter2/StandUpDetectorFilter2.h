#pragma once
#include "stdafx.h"
#include <streams.h>
#include "StandUpDetector2UIDs.h"
#include "StandUpDetectAlg.h"

class CStandUpDetector2InputPin : public CTransInPlaceInputPin
{
public:
	CStandUpDetector2InputPin(TCHAR *pObjectName, CTransInPlaceFilter *pTransInPlaceFilter, HRESULT *phr, LPCTSTR pName)
		: CTransInPlaceInputPin(pObjectName, pTransInPlaceFilter, phr, pName)
	{

	}

	HRESULT CheckMediaType(const CMediaType *pmt);
};

class CStandUpDetector2OutputPin : public CTransInPlaceOutputPin
{
public:
	CStandUpDetector2OutputPin(TCHAR *pObjectName, CTransInPlaceFilter *pTransInPlaceFilter, HRESULT *phr, LPCTSTR pName)
		: CTransInPlaceOutputPin(pObjectName, pTransInPlaceFilter, phr, pName)
	{

	}

	HRESULT CheckMediaType(const CMediaType *pmt);
};

class CStandUpDetectorFilter2 : public CTransInPlaceFilter, public IStandUpControl
{
	friend class CStandUpDetector2InputPin;
	friend class CStandUpDetector2OutputPin;

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


	//
	// --- IStandUpControl Interface --
	//
	STDMETHODIMP Start(BOOL bShowTrackingRes);
	STDMETHODIMP Stop();
	STDMETHODIMP SetParams(int leftBorder, int rightBorder, int totalRowNum, int totalColNum, int detectLine);
private:

	CStandUpDetectorFilter2(TCHAR *tszName, LPUNKNOWN punk, HRESULT *phr);

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

	CStandUpDetectAlg *m_pStandUpAlg;
};


