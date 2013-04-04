#include "stdafx.h"
#include <streams.h>
#include "TrackingAlgFilter.h"

#include "opencv2/video/tracking.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"

#include "Analyzer.h"
#include "hasp_api.h"

unsigned char vendor_code[] =
	"JU4Y6fjHvwcWe4F9pe4xVQOBeo9QU7nTXt4A/bChE2Hgi5AyqN8K60CFBuywB/KLwPbZPdwDXeEe1+fP"
	"hfTsH077DvFXtIrPRfdk3hU6FEGL6Gpf1asD9+ElM3ZWCKrGWKkAj3qtoEssvGHs9psl7ngIkJMqjY43"
	"2tUmjw4SGyQyk3fPWKI4LFPCuyijkOzsiLd+FExyO7PblGBfjYEdn9/JcWjhwopwgWXtbqTpZm3rDIQS"
	"ixEfuaRV8yDKeoePDo23O0pruYlo0d931/Z4iGER5fCw0HIWerYsnDb8Q34Wc+ERvGo5K+aYrffa4IXa"
	"ECW+41bNNRCOoeTqPJbkV5AcR61yZEzq8RddvsKV7W2w4u0SCBEr0aHHXVVKTdqKfLQDLyMCfQ4cDVlj"
	"FZor1UQrBE4PSGn8R8FbPwBo1hoRZaCI8PaENrJD7TNafABaoPrKt8T/qNHBHrgNlU3XzzNaw4J//+Vo"
	"hA1/JNl3MCJ4Lcfqey+qHiZs/XxS9ckPaaNCpRK8U9+ytXCAX5TAhXannUrOQdGN+CY7ybD3e1Lzq95q"
	"gKx0uxinA+6ncd7KmhDrxDi9IxfllgnqFIsxPbNhCljp9vNFwx+ziUPvr73np1il5L/Gj5gb/CXyLMaA"
	"MClue3n61V8RrQSTSH1RTfZJ9BXk5cwSeJEZ/XsKyVpeqgqlY479AetK2Lo1Sc6ywtfMnUObcGIqfxqA"
	"rX3VlqQYbisFvFhVHblkru3qqsotbuvZfauTiv6Vi2kq1RuQaE6CF8I0y+A8y9EiecauTPFAbLaqIjDZ"
	"X56W/P8qmL7ZUZSphm07lU5ONlXYGfqdbksLJ9TP2zDCVJ3BdA+G8rAFyoR7QoSvU6quCBeY3c4uXUVE"
	"oHmHpg/0WQsrq+BfXDZaeibIjC0HyunQ5umzLxtvoYic0FNjA03ENsSrVLU=";

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
	m_nFrameCount = 0;
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
	m_nFrameCount++;
	if(m_nFrameCount == CheckSoftDogInterval)
	{
		m_nFrameCount = 0;
		if(FAILED(checkSoftDog()))
		{
			return S_OK;
		}
	}
	PBYTE p;
	pSample->GetPointer(&p);

	int stride = (m_biWidth * sizeof(RGBTRIPLE) + 3) & -4;
	cv::Mat img(m_biHeight, m_biWidth, CV_8UC3, p, stride);
	m_pTrackingAlg->DrawBZoneVertexes(img);
	m_pTrackingAlg->DrawBZones(img);
	if(m_bTracking)
	{
		QueryPerformanceCounter((LARGE_INTEGER *)&T1);

		m_pTrackingAlg->Update(img);

		QueryPerformanceCounter((LARGE_INTEGER *)&T2);

		dfTime = (T2-T1)*1000/FREQ;
		char tmp[1024];
		sprintf(tmp, "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!Time Performance: %f!\n", dfTime);
		OutputDebugStringA(tmp);

		((CPositionAnalyzer *)m_pPosAnalyzer)->AnalyzeTeacherPositions(m_pTrackingAlg->GetTrackedPerson());
		cv::waitKey(1);
	}
	else
	{
		((CPositionAnalyzer *)m_pPosAnalyzer)->ResetParameters();
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

STDMETHODIMP CTrackingAlgFilter::StartTracking(BOOL bShowTrackingRes)
{
	m_pTrackingAlg->SetShowTracking(bShowTrackingRes);
	m_bTracking = TRUE;
	return S_OK;
}

STDMETHODIMP CTrackingAlgFilter::StopTracking()
{
	m_bTracking = FALSE;
	return S_FALSE;
}

STDMETHODIMP CTrackingAlgFilter::ConfigTrackingArea(int beginX, int beginY, int beginWidth, int beginHeight, 
	                                                int stopX, int stopY, int stopWidth, int stopHeight)
{
	if(beginX >= 0 && beginY >= 0 && beginWidth > 0 && stopHeight > 0)
	{
		TrackingConfig::BEGIN_TRACKING_AREA = Rect(beginX, beginY, beginWidth, beginHeight);
	}
	if(stopX >= 0 && stopY >= 0 && stopWidth > 0 && stopHeight > 0)
	{
		TrackingConfig::STOP_TRACKING_AREA = Rect(stopX, stopY, stopWidth, stopHeight);
	}
	return S_OK;
}

STDMETHODIMP CTrackingAlgFilter::ConfigHuman(int leastHumanGap, int humanWidth)
{
	if(leastHumanGap > 0)
	{
		TrackingConfig::LEAST_HUMAN_GAP = leastHumanGap;
	}
	if(humanWidth > 0)
	{
		TrackingConfig::HUMAN_WIDTH = humanWidth;
	}
	return S_OK;
}

STDMETHODIMP CTrackingAlgFilter::ConfigVariousThresh(int disappearFrameThresh, int centerWeightThresh, 
	                                                 int fgLowThresh, int fgHighThresh, double fgHistThresh)
{
	if(disappearFrameThresh > 0)
	{
		TrackingConfig::DISAPPEAR_FRAME_THRESH = disappearFrameThresh;
	}
	if(centerWeightThresh > 0)
	{
		TrackingConfig::CENTER_WEIGHT_THRESH = centerWeightThresh;
	}
	if(fgLowThresh > 0)
	{
		TrackingConfig::FG_LOW_THRESH = fgLowThresh;
	}
	if(fgHighThresh > 0)
	{
		TrackingConfig::FG_UP_THRESH = fgHighThresh;
	}
	if(fgHistThresh > 0)
	{
		TrackingConfig::FG_HIST_THRESH = fgHistThresh;
	}

	return S_OK;
}

STDMETHODIMP CTrackingAlgFilter::ConfigMiscellaneous(double gbmLearningRate, int trackInterval)
{
	if(gbmLearningRate > 0)
	{
		TrackingConfig::GBM_LEARNING_RATE = gbmLearningRate;
	}
	if(trackInterval > 0)
	{
		TrackingConfig::TRACK_INTERVAL = trackInterval;
	}

	return S_OK;
}

STDMETHODIMP CTrackingAlgFilter::CacheAndShowBZoneVertex(int xPix, int yPix)
{
	return m_pTrackingAlg->CacheAndShowBZoneVertex(xPix, yPix);
}

STDMETHODIMP CTrackingAlgFilter::EraseCachedVertexes()
{
	return m_pTrackingAlg->EraseCachedVertexes();
}

STDMETHODIMP CTrackingAlgFilter::AddBZone(int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4)
{
	return m_pTrackingAlg->AddBZone(x1, y1, x2, y2, x3, y3, x4, y4);
}

STDMETHODIMP CTrackingAlgFilter::ClearBlindZones()
{
	return m_pTrackingAlg->ClearBZones();
}

HRESULT CTrackingAlgFilter::checkSoftDog()
{
	hasp_status_t status;
	hasp_handle_t handle;
	HRESULT result = S_OK;
	status = hasp_login(HASP_DEFAULT_FID, (hasp_vendor_code_t *)vendor_code, &handle);
	if (status != HASP_STATUS_OK)
	{
		switch (status)
		{
		case HASP_FEATURE_NOT_FOUND:
			printf("login to default feature failed!\n");
			break;
		case HASP_CONTAINER_NOT_FOUND:
			printf("no sentinel key with vendor code CACOM found\n");
			break;
		default:
			printf("%d\n", status);
			printf("unknown error!\n");

		}
		result = E_FAIL;
	}
	status = hasp_logout(handle);
	return result;
}