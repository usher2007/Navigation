#include "stdafx.h"
#include <streams.h>
#include "StandUpDetectorFilter.h"

const LONG DEFAULT_WIDTH = 720;
const LONG DEFUALT_HEIGHT = 576;

static LONGLONG FREQ;

const AMOVIESETUP_MEDIATYPE
	sudPinTypes =   { &MEDIATYPE_NULL                // clsMajorType
	, &MEDIASUBTYPE_NULL }  ;       // clsMinorType

const AMOVIESETUP_PIN
	psudPins[] = { { L"Input"            // strName
	, FALSE               // bRendered
	, FALSE               // bOutput
	, FALSE               // bZero
	, FALSE               // bMany
	, &CLSID_NULL         // clsConnectsToFilter
	, L"Output"           // strConnectsToPin
	, 1                   // nTypes
	, &sudPinTypes }      // lpTypes
, { L"Output"           // strName
	, FALSE               // bRendered
	, TRUE                // bOutput
	, FALSE               // bZero
	, FALSE               // bMany
	, &CLSID_NULL         // clsConnectsToFilter
	, L"Input"            // strConnectsToPin
	, 1                   // nTypes
	, &sudPinTypes } };   // lpTypes


const AMOVIESETUP_FILTER
	sudNullIP = { &CLSID_StandUpDetector                 // clsID
	, L"StandUp Detector"                // strName
	, MERIT_DO_NOT_USE                // dwMerit
	, 2                               // nPins
	, psudPins };                     // lpPin

CFactoryTemplate g_Templates[]=
{   {L"StandUp-Detector"
, &CLSID_StandUpDetector
,   CStandUpDetectorFilter::CreateInstance
, NULL
, &sudNullIP }
};
int g_cTemplates = sizeof(g_Templates)/sizeof(g_Templates[0]);

int CStandUpDetectorFilter::m_nInstanceCount = 0;

HRESULT CStandUpDetectorInputPin::CheckMediaType(const CMediaType *pmt)
{
	CStandUpDetectorFilter *pStandUpDetector = (CStandUpDetectorFilter *)m_pTIPFilter;
	CheckPointer(pStandUpDetector, E_UNEXPECTED);

	if(*(pmt->Type()) !=  MEDIATYPE_Video)
	{
		return E_INVALIDARG;
	}

	if(!pmt->IsFixedSize())
	{
		return E_INVALIDARG;
	}

	if(*(pmt->Subtype()) != MEDIASUBTYPE_RGB24)
	{
		return E_INVALIDARG;
	}

	if(*(pmt->FormatType()) != FORMAT_VideoInfo)
	{
		return E_INVALIDARG;
	}

	VIDEOINFOHEADER *pvi = (VIDEOINFOHEADER *)pmt->Format();

	if(pvi == NULL)
	{
		return E_INVALIDARG;
	}

	if(pvi->bmiHeader.biHeight < 0 || pvi->bmiHeader.biWidth < 0)
	{
		return E_INVALIDARG;
	}

	pStandUpDetector->SetResolution(pvi->bmiHeader.biWidth, pvi->bmiHeader.biHeight);

	return S_OK;
}

HRESULT CStandUpDetectorOutputPin::CheckMediaType(const CMediaType *pmt)
{
	CStandUpDetectorFilter *pStandUpDetector = (CStandUpDetectorFilter *)m_pTIPFilter;
	CheckPointer(pStandUpDetector, E_UNEXPECTED);

	if(*(pmt->Type()) !=  MEDIATYPE_Video)
	{
		return E_INVALIDARG;
	}

	if(!pmt->IsFixedSize())
	{
		return E_INVALIDARG;
	}

	if(*(pmt->Subtype()) != MEDIASUBTYPE_RGB24)
	{
		return E_INVALIDARG;
	}

	if(*(pmt->FormatType()) != FORMAT_VideoInfo)
	{
		return E_INVALIDARG;
	}

	VIDEOINFOHEADER *pvi = (VIDEOINFOHEADER *)pmt->Format();

	if(pvi == NULL)
	{
		return E_INVALIDARG;
	}

	if(pvi->bmiHeader.biHeight < 0)
	{
		return E_INVALIDARG;
	}

	LONG height = pStandUpDetector->GetHeight();
	LONG width = pStandUpDetector->GetWidth();
	height = height > 0 ? height : DEFUALT_HEIGHT;
	width = width > 0 ? width : DEFAULT_WIDTH;

	if(pvi->bmiHeader.biWidth != width || pvi->bmiHeader.biHeight != height)
	{
		return E_INVALIDARG;
	}

	return S_OK;
}

CStandUpDetectorFilter::CStandUpDetectorFilter(TCHAR *tszName, LPUNKNOWN punk, HRESULT *phr)
	:CTransInPlaceFilter(tszName, punk, CLSID_StandUpDetector, phr)
{
	m_nInstanceCount = ++ m_nInstanceCount;
	m_mtPreferred.InitMediaType();
	QueryPerformanceFrequency((LARGE_INTEGER *)&FREQ);
	m_bDetecting = FALSE;
	m_nFrameCount = 0;
	m_bSoftDogChecked = TRUE;
	m_pStandUpAlg = new CStandUpDetectAlg(-1);
}

CUnknown * WINAPI CStandUpDetectorFilter::CreateInstance(LPUNKNOWN punk, HRESULT *phr)
{
	ASSERT(phr);

	CStandUpDetectorFilter *pNewObject = new CStandUpDetectorFilter(NAME("StandUp-Detector Filter"), punk, phr);
	if (pNewObject == NULL) 
	{
		if (phr)
			*phr = E_OUTOFMEMORY;
	}

	return pNewObject;
}

CBasePin *CStandUpDetectorFilter::GetPin(int n)
{
	if (m_pInput == NULL || m_pOutput == NULL)
	{
		HRESULT hr = S_OK;

		m_pInput = new CStandUpDetectorInputPin(NAME("StandUpDetector input pin"), this, &hr, L"Input");

		if(FAILED(hr) || m_pInput == NULL)
		{
			delete m_pInput;
			m_pInput = NULL;
			return NULL;
		}

		m_pOutput = new CStandUpDetectorOutputPin(NAME("StandUpDetector output pin"), this, &hr, L"Output");

		if(FAILED(hr) || m_pOutput == NULL)
		{
			delete m_pInput;
			m_pInput = NULL;

			delete m_pOutput;
			m_pOutput = NULL;
			return NULL;
		}
	}

	switch(n)
	{
	case 0:
		return m_pInput;
	case 1:
		return m_pOutput;
	}
	return NULL;
}

STDMETHODIMP CStandUpDetectorFilter::NonDelegatingQueryInterface(REFIID riid, void **ppv)
{
	CheckPointer(ppv,E_POINTER);

	if (riid == IID_IStandUpControl) {
		return GetInterface((IStandUpControl *) this, ppv);
	}
	else {
		return CTransformFilter::NonDelegatingQueryInterface(riid, ppv);
	}
}

HRESULT CStandUpDetectorFilter::Transform(IMediaSample *pSample)
{
	if(!m_bDetecting)
	{
		return S_OK;
	}
	m_nFrameCount++;
	PBYTE p;
	pSample->GetPointer(&p);


	OutputDebugStringA("1ST FILTER\n");
	int stride = (m_biWidth * sizeof(RGBTRIPLE) + 3) & -4;
	cv::Mat img(m_biHeight, m_biWidth, CV_8UC3, p, stride);
	m_pStandUpAlg->Update(img);
	return S_OK;
}

HRESULT CStandUpDetectorFilter::SetResolution(LONG biWidth, LONG biHeight)
{
	if(biWidth > 0 && biHeight > 0)
	{
		m_biWidth = biWidth;
		m_biHeight = biHeight;
		return S_OK;
	}
	return E_FAIL;
}

LONG CStandUpDetectorFilter::GetWidth()
{
	return m_biWidth > 0 ? m_biWidth : DEFAULT_WIDTH;
}

LONG CStandUpDetectorFilter::GetHeight()
{
	return m_biHeight > 0 ? m_biHeight : DEFUALT_HEIGHT;
}


STDMETHODIMP CStandUpDetectorFilter::Start(BOOL bShowTrackingRes)
{
	if(m_bDetecting == FALSE)
	{
		m_pStandUpAlg = new CStandUpDetectAlg(0);
		m_bDetecting = TRUE;
	}
	return S_OK;
}

STDMETHODIMP CStandUpDetectorFilter::Stop()
{
	m_bDetecting = FALSE;
	Sleep(10);
	delete m_pStandUpAlg;
	m_pStandUpAlg = NULL;
	return S_OK;
}

STDMETHODIMP CStandUpDetectorFilter::SetParams(int leftBorder, int rightBorder, int totalRowNum, int totalColNum, int detectLine)
{
	m_pStandUpAlg->SetParams(leftBorder, rightBorder, totalRowNum, totalColNum, detectLine);
	return S_OK;
}
