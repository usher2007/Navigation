#include "stdafx.h"
#include <streams.h>
#include "TrackingAlgFilter.h"

#include "opencv2/video/tracking.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"

#include "Analyzer.h"

const LONG DEFAULT_WIDTH = 720;
const LONG DEFUALT_HEIGHT = 576;

static LONGLONG T1 = 0, T2 = 0;
static LONGLONG FREQ;
static double dfTime = 0.0;

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
	sudNullIP = { &CLSID_TrackingAlg                 // clsID
	, L"Tracking Alg"                // strName
	, MERIT_DO_NOT_USE                // dwMerit
	, 2                               // nPins
	, psudPins };                     // lpPin

CFactoryTemplate g_Templates[]=
{   {L"Tracking-Alg"
, &CLSID_TrackingAlg
,   CTrackingAlgFilter::CreateInstance
, NULL
, &sudNullIP }
};
int g_cTemplates = sizeof(g_Templates)/sizeof(g_Templates[0]);


//
// initialise the static instance count.
//
int CTrackingAlgFilter::m_nInstanceCount = 0;


HRESULT CTrackingAlgInputPin::CheckMediaType(const CMediaType *pmt)
{
	CTrackingAlgFilter *pTransAlg = (CTrackingAlgFilter *)m_pTIPFilter;
	CheckPointer(pTransAlg, E_UNEXPECTED);

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

	pTransAlg->SetResolution(pvi->bmiHeader.biWidth, pvi->bmiHeader.biHeight);

	return S_OK;
}

HRESULT CTrackingAlgOutputPin::CheckMediaType(const CMediaType *pmt)
{
	CTrackingAlgFilter *pTransAlg = (CTrackingAlgFilter *)m_pTIPFilter;
	CheckPointer(pTransAlg, E_UNEXPECTED);

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

	LONG height = pTransAlg->GetHeight();
	LONG width = pTransAlg->GetWidth();
	height = height > 0 ? height : DEFUALT_HEIGHT;
	width = width > 0 ? width : DEFAULT_WIDTH;

	if(pvi->bmiHeader.biWidth != width || pvi->bmiHeader.biHeight != height)
	{
		return E_INVALIDARG;
	}

	return S_OK;
}

CTrackingAlgFilter::CTrackingAlgFilter(TCHAR *tszName, LPUNKNOWN punk, HRESULT *phr)
	:CTransInPlaceFilter(tszName, punk, CLSID_TrackingAlg, phr)
{
	m_nInstanceCount = ++ m_nInstanceCount;
	m_mtPreferred.InitMediaType();
	m_pTrackingAlg = new TrackingAlg;
	QueryPerformanceFrequency((LARGE_INTEGER *)&FREQ);
	m_bTracking = FALSE;
	m_pPosAnalyzer = (void *)(new CPositionAnalyzer);
	namedWindow("Debug");
}

CUnknown * WINAPI CTrackingAlgFilter::CreateInstance(LPUNKNOWN punk, HRESULT *phr)
{
	ASSERT(phr);

	CTrackingAlgFilter *pNewObject = new CTrackingAlgFilter(NAME("Trans-Alg Filter"), punk, phr);
	if (pNewObject == NULL) 
	{
		if (phr)
			*phr = E_OUTOFMEMORY;
	}

	return pNewObject;
}

CBasePin *CTrackingAlgFilter::GetPin(int n)
{
	if (m_pInput == NULL || m_pOutput == NULL)
	{
		HRESULT hr = S_OK;

		m_pInput = new CTrackingAlgInputPin(NAME("TrackingAlg input pin"), this, &hr, L"Input");

		if(FAILED(hr) || m_pInput == NULL)
		{
			delete m_pInput;
			m_pInput = NULL;
			return NULL;
		}

		m_pOutput = new CTrackingAlgOutputPin(NAME("TrackingAlg output pin"), this, &hr, L"Output");

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


STDMETHODIMP CTrackingAlgFilter::NonDelegatingQueryInterface(REFIID riid, void **ppv)
{
	CheckPointer(ppv,E_POINTER);

	if (riid == IID_ITrackingControl) {
		return GetInterface((ITrackingControl *) this, ppv);
	}
	//else if (riid == IID_ISpecifyPropertyPages) {
	//	return GetInterface((ISpecifyPropertyPages *) this, ppv);
	//}
	else {
		return CTransformFilter::NonDelegatingQueryInterface(riid, ppv);
	}
}


HRESULT CTrackingAlgFilter::Transform(IMediaSample *pSample)
{
	if(m_bTracking)
	{
		QueryPerformanceCounter((LARGE_INTEGER *)&T1);

		PBYTE p;
		pSample->GetPointer(&p);

		int stride = (m_biWidth * sizeof(RGBTRIPLE) + 3) & -4;
		cv::Mat img(m_biHeight, m_biWidth, CV_8UC3, p, stride);
		imshow("Debug", img);
		m_pTrackingAlg->Update(img);

		QueryPerformanceCounter((LARGE_INTEGER *)&T2);

		dfTime = (T2-T1)*1000/FREQ;
		char tmp[1024];
		sprintf(tmp, "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!Time Performance: %f!\n", dfTime);
		OutputDebugStringA(tmp);

		((CPositionAnalyzer *)m_pPosAnalyzer)->AnalyzeTeacherPositions(m_pTrackingAlg->GetTrackedPerson());
		cv::waitKey(1);
	}
	return NOERROR;
}

HRESULT CTrackingAlgFilter::SetResolution(LONG biWidth, LONG biHeight)
{
	if(biWidth > 0 && biHeight > 0)
	{
		m_biWidth = biWidth;
		m_biHeight = biHeight;
		return S_OK;
	}
	return E_FAIL;
}

LONG CTrackingAlgFilter::GetWidth()
{
	return m_biWidth > 0 ? m_biWidth : DEFAULT_WIDTH;
}

LONG CTrackingAlgFilter::GetHeight()
{
	return m_biHeight > 0 ? m_biHeight : DEFUALT_HEIGHT;
}

STDMETHODIMP CTrackingAlgFilter::StartTracking()
{
	m_bTracking = TRUE;
	return S_OK;
}

STDMETHODIMP CTrackingAlgFilter::StopTracking()
{
	m_bTracking = FALSE;
	return S_FALSE;
}