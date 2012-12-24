#include "stdafx.h"
#include <streams.h>
#include "TransAlgFilter.h"

#include "opencv2/video/tracking.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"

const LONG DEFAULT_WIDTH = 720;
const LONG DEFUALT_HEIGHT = 576;

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
	sudNullIP = { &CLSID_TransAlg                 // clsID
	, L"Trans Alg"                // strName
	, MERIT_DO_NOT_USE                // dwMerit
	, 2                               // nPins
	, psudPins };                     // lpPin

CFactoryTemplate g_Templates[]=
{   {L"Trans-Alg"
, &CLSID_TransAlg
,   CTransAlgFilter::CreateInstance
, NULL
, &sudNullIP }
};
int g_cTemplates = sizeof(g_Templates)/sizeof(g_Templates[0]);


//
// initialise the static instance count.
//
int CTransAlgFilter::m_nInstanceCount = 0;


HRESULT CTransAlgInputPin::CheckMediaType(const CMediaType *pmt)
{
	CTransAlgFilter *pTransAlg = (CTransAlgFilter *)m_pTIPFilter;
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

HRESULT CTransAlgOutputPin::CheckMediaType(const CMediaType *pmt)
{
	CTransAlgFilter *pTransAlg = (CTransAlgFilter *)m_pTIPFilter;
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

CTransAlgFilter::CTransAlgFilter(TCHAR *tszName, LPUNKNOWN punk, HRESULT *phr)
	:CTransInPlaceFilter(tszName, punk, CLSID_TransAlg, phr)
{
	m_nInstanceCount = ++ m_nInstanceCount;
	m_mtPreferred.InitMediaType();
}

CUnknown * WINAPI CTransAlgFilter::CreateInstance(LPUNKNOWN punk, HRESULT *phr)
{
	ASSERT(phr);

	CTransAlgFilter *pNewObject = new CTransAlgFilter(NAME("Trans-Alg Filter"), punk, phr);
	if (pNewObject == NULL) 
	{
		if (phr)
			*phr = E_OUTOFMEMORY;
	}

	cv::namedWindow("OpenCV");
	return pNewObject;
}

CBasePin *CTransAlgFilter::GetPin(int n)
{
	if (m_pInput == NULL || m_pOutput == NULL)
	{
		HRESULT hr = S_OK;

		m_pInput = new CTransAlgInputPin(NAME("TransAlg input pin"), this, &hr, L"Input");

		if(FAILED(hr) || m_pInput == NULL)
		{
			delete m_pInput;
			m_pInput = NULL;
			return NULL;
		}

		m_pOutput = new CTransAlgOutputPin(NAME("TransAlg output pin"), this, &hr, L"Output");

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


STDMETHODIMP CTransAlgFilter::NonDelegatingQueryInterface(REFIID riid, void **ppv)
{
	CheckPointer(ppv,E_POINTER);

	//if (riid == IID_INullIPP) {
	//	return GetInterface((INullIPP *) this, ppv);
	//}
	//else if (riid == IID_ISpecifyPropertyPages) {
	//	return GetInterface((ISpecifyPropertyPages *) this, ppv);
	//}
	//else {
	//	return CTransformFilter::NonDelegatingQueryInterface(riid, ppv);
	//}
	return CTransformFilter::NonDelegatingQueryInterface(riid, ppv);
}

HRESULT CTransAlgFilter::Transform(IMediaSample *pSample)
{
	PBYTE p;
	pSample->GetPointer(&p);
	
	/*cv::Mat img(m_biHeight, m_biWidth, CV_8UC3, p);
	imshow("OpenCV", img);
	cv::waitKey(1);*/
	return NOERROR;
}

HRESULT CTransAlgFilter::SetResolution(LONG biWidth, LONG biHeight)
{
	if(m_biWidth > 0 && m_biHeight > 0)
	{
		m_biWidth = biWidth;
		m_biHeight = biHeight;
		return S_OK;
	}
	return E_FAIL;
}

LONG CTransAlgFilter::GetWidth()
{
	return m_biWidth > 0 ? m_biWidth : DEFAULT_WIDTH;
}

LONG CTransAlgFilter::GetHeight()
{
	return m_biHeight > 0 ? m_biHeight : DEFUALT_HEIGHT;
}