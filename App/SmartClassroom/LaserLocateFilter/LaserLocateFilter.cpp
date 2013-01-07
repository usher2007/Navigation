// LaserLocateFilter.cpp : Defines the exported functions for the DLL application.
//


#include <streams.h>          // quartz, includes windows

// Eliminate two expected level 4 warnings from the Microsoft compiler.
// The class does not have an assignment or copy operator, and so cannot
// be passed by value.  This is normal.  This file compiles clean at the
// highest (most picky) warning level (-W4).
#pragma warning(disable: 4511 4512)

#include <measure.h>          // performance measurement (MSR_)

#include <initguid.h>
#if (1100 > _MSC_VER)
#include <olectlid.h>
#else
#include <olectl.h>
#endif

#include "nullUids.h"         // our own public guids

#include "inull.h"            // interface between filter and property sheet
#include "nullprop.h"         // property sheet implementatino class



// ----------------------------------------------------------------------------
// Class definitions of input pin, output pin and filter
// ----------------------------------------------------------------------------

class CLaserLocateFilter;
class CPoint
{
	friend class CLaserLocateFilter;
private:
	int x,y;
	CPoint * next;

};

class CLaserLocateFilterInputPin : public CTransInPlaceInputPin
{
public:
	CLaserLocateFilterInputPin( TCHAR *pObjectName
		, CTransInPlaceFilter *pTransInPlaceFilter
		, HRESULT * phr
		, LPCWSTR pName
		)
		: CTransInPlaceInputPin( pObjectName
		, pTransInPlaceFilter
		, phr
		, pName
		)
	{
	}

	HRESULT CheckMediaType(const CMediaType* pmt);
};


class CLaserLocateFilterOutputPin : public CTransInPlaceOutputPin
{
public:
	CLaserLocateFilterOutputPin( TCHAR *pObjectName
		, CTransInPlaceFilter *pTransInPlaceFilter
		, HRESULT * phr
		, LPCWSTR pName
		)
		: CTransInPlaceOutputPin( pObjectName
		, pTransInPlaceFilter
		, phr
		, pName
		)
	{
	}

	HRESULT CheckMediaType(const CMediaType* pmt);
};



// CLaserLocateFilter
//
class CLaserLocateFilter : public CTransInPlaceFilter
	, public INullIPP
	, public ISpecifyPropertyPages
{

	friend class CLaserLocateFilterInputPin;
	friend class CLaserLocateFilterOutputPin;

public:

	static CUnknown * WINAPI CreateInstance(LPUNKNOWN punk, HRESULT *phr);

	DECLARE_IUNKNOWN;

	//
	// --- CTransInPlaceFilter Overrides --
	//

	virtual CBasePin *GetPin( int n );

	HRESULT CheckInputType(const CMediaType* mtIn)
	{ UNREFERENCED_PARAMETER(mtIn);  return S_OK; }

	// Basic COM - used here to reveal our property interface.
	STDMETHODIMP NonDelegatingQueryInterface(REFIID riid, void ** ppv);

	STDMETHODIMP get_IPin (IPin **ppPin) ;
	STDMETHODIMP put_MediaType(CMediaType *pmt);
	STDMETHODIMP get_MediaType(CMediaType **pmt);
	STDMETHODIMP get_State(FILTER_STATE *state);


	//
	// --- ISpecifyPropertyPages ---
	//

	STDMETHODIMP GetPages(CAUUID *pPages);

private:

	CLaserLocateFilter(TCHAR *tszName, LPUNKNOWN punk, HRESULT *phr);

	//
	// Overrides the PURE virtual Transform of CTransInPlaceFilter base class
	// This is where the "real work" is done.
	//
	virtual HRESULT Transform(IMediaSample *pSample);
	//     { UNREFERENCED_PARAMETER(pSample); return NOERROR; }


	//
	// If there are multiple instances of this filter active, it's
	// useful for debug messages etc. to know which one this is.
	//
	static int m_nInstanceCount;   // total instances
	int m_nThisInstance;

	CMediaType m_mtPreferred;  // Media type chosen from property sheet

	CCritSec m_NullIPLock;     // To serialise access.

	void findRedPoint(tagRGBTRIPLE *prgb,long lData,int nWidth,int nHeight);
	int threshold;
	CPoint * point;
	CPoint pHead;
};


// ----------------------------------------------------------------------------
// Implementation of pins and filter
// ----------------------------------------------------------------------------


//
// DbgFunc
//
// Put out the name of the function and instance on the debugger.
// Call this at the start of interesting functions to help debug
//
#define DbgFunc(a) DbgLog(( LOG_TRACE                        \
	, 2                                \
	, TEXT("CLaserLocateFilter(Instance %d)::%s\0") \
	, m_nThisInstance                  \
	, TEXT(a)                          \
	));

//
// setup data
//

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
	sudNullIP = { &CLSID_NullInPlace                 // clsID
	, L"Null In Place"                // strName
	, MERIT_DO_NOT_USE                // dwMerit
	, 2                               // nPins
	, psudPins };                     // lpPin

//
// Needed for the CreateInstance mechanism
//
CFactoryTemplate g_Templates[]=
{   {L"Null-In-Place"
, &CLSID_NullInPlace
,   CLaserLocateFilter::CreateInstance
, NULL
, &sudNullIP }
,
{ L"Null IP Property Page"
, &CLSID_NullIPPropertyPage
, NullIPProperties::CreateInstance }

};
int g_cTemplates = sizeof(g_Templates)/sizeof(g_Templates[0]);


//
// initialise the static instance count.
//
int CLaserLocateFilter::m_nInstanceCount = 0;


// ----------------------------------------------------------------------------
//            Input pin implementation
// ----------------------------------------------------------------------------


// CheckMediaType
//
// Override CTransInPlaceInputPin method.
// If we have been given a preferred media type from the property sheet
// then only accept a type that is exactly that.
// else if there is nothing downstream, then accept anything
// else if there is a downstream connection then first check to see if
// the subtype (and implicitly the major type) are different from the downstream
// connection and if they are different, fail them
// else ask the downstream input pin if the type (i.e. all details of it)
// are acceptable and take that as our answer.
//
HRESULT CLaserLocateFilterInputPin::CheckMediaType( const CMediaType *pmt )
{   
	CLaserLocateFilter *pNull = (CLaserLocateFilter *) m_pTIPFilter;

	CheckPointer(pNull,E_UNEXPECTED);

#ifdef DEBUG
	DisplayType(TEXT("Input type proposed"),pmt);
#endif

	if (pNull->m_mtPreferred.IsValid() == FALSE)
	{
		if( pNull->m_pOutput->IsConnected() ) {

			//  We used to check here if the subtype of the proposed type
			//  matched the subtype of the type on the output pin
			//  but this broke as follows:
			//
			//  Renderering the output pin of a CODEC we picked up
			//  2 NULLIPs already in the graph:
			//
			//  Subtypes      Y41P       Y41P       RGB565
			//  Filters  CODEC---->NULLIP---->NULLIP------>RENDERER
			//
			//  Each NULLIP has scheduled a reconnect at this point
			//  and the reconnect on the first connection happens
			//  first:
			//
			//  Subtypes                 Y41P       RGB565
			//  Filters  CODEC     NULLIP---->NULLIP------>RENDERER
			//
			//  In trying to (re)connect the CODEC to the first NULLIP
			//  we first propose (say) Y41P and the first NULLIP
			//  checks that Y41P is the same as its output type
			//  so the call gets passed to the QueryAccept of
			//  the second NULLIP.  The second NULLIP rejected the
			//  call because the subtype on its output pin is not
			//  RGB565.  In a similar way the first NULLIP
			//  rejected Y41P.
			//
			//  By removing this optimization (checking the
			//  subtype before passing the call on) we avoided
			//  the problem.

			return pNull->m_pOutput->GetConnected()->QueryAccept( pmt );
		}
		return S_OK;
	}
	else
		if (*pmt == pNull->m_mtPreferred)
			return S_OK  ;
		else
			return VFW_E_TYPE_NOT_ACCEPTED;
}


// ----------------------------------------------------------------------------
//            Input pin implementation
// ----------------------------------------------------------------------------


// CheckMediaType
//
// Override CTransInPlaceOutputPin method.
// If we have ben given a media type from the property sheet, then insist on
// exactly that, else pass the request up to the base class implementation.
//
HRESULT CLaserLocateFilterOutputPin::CheckMediaType( const CMediaType *pmt )
{   
	CLaserLocateFilter *pNull = (CLaserLocateFilter *) m_pTIPFilter;

	CheckPointer(pNull,E_UNEXPECTED);

	if (pNull->m_mtPreferred.IsValid() == FALSE)
	{
		return CTransInPlaceOutputPin::CheckMediaType (pmt) ;
	}
	else
	{
		if (*pmt == pNull->m_mtPreferred)
			return S_OK  ;
		else
			return VFW_E_TYPE_NOT_ACCEPTED;
	}
}



// ----------------------------------------------------------------------------
//            Filter implementation
// ----------------------------------------------------------------------------


//
// CLaserLocateFilter::Constructor
//
CLaserLocateFilter::CLaserLocateFilter(TCHAR *tszName, LPUNKNOWN punk, HRESULT *phr)
	: CTransInPlaceFilter (tszName, punk, CLSID_NullInPlace, phr)
{
	m_nThisInstance = ++m_nInstanceCount;
	m_mtPreferred.InitMediaType () ;

	DbgFunc("CLaserLocateFilter");

} // (CLaserLocateFilter constructor)


//
// CreateInstance
//
// Override CClassFactory method.
// Provide the way for COM to create a CLaserLocateFilter object
//
CUnknown * WINAPI CLaserLocateFilter::CreateInstance(LPUNKNOWN punk, HRESULT *phr) 
{
	ASSERT(phr);

	CLaserLocateFilter *pNewObject = new CLaserLocateFilter(NAME("Null-In-Place Filter"), punk, phr );
	if (pNewObject == NULL) 
	{
		if (phr)
			*phr = E_OUTOFMEMORY;
	}

	return pNewObject;

} // CreateInstance


//
// GetPin
//
// Override CBaseFilter method.
// Return a non-addrefed CBasePin * for the user to addref if he holds onto it
// for longer than his pointer to us.  This is part of the implementation of
// EnumMediaTypes.  All attempts to refer to our pins from the outside have
// to come through here, so it's a valid place to create them.
//
CBasePin *CLaserLocateFilter::GetPin(int n)
{
	// Create the single input pin and the single output pin
	// If anything fails, fail the whole lot and clean up.

	if (m_pInput == NULL || m_pOutput == NULL) 
	{
		HRESULT hr = S_OK;

		m_pInput = new CLaserLocateFilterInputPin(NAME("Null input pin"),
			this,              // Owner filter
			&hr,               // Result code
			L"Input");         // Pin name

		// a failed return code should delete the object

		if (FAILED(hr) || m_pInput == NULL) 
		{
			delete m_pInput;
			m_pInput = NULL;
			return NULL;
		}

		m_pOutput = new CLaserLocateFilterOutputPin(NAME("Null output pin"),
			this,            // Owner filter
			&hr,             // Result code
			L"Output");      // Pin name

		// failed return codes cause both objects to be deleted

		if (FAILED(hr) || m_pOutput == NULL) 
		{
			delete m_pInput;
			m_pInput = NULL;

			delete m_pOutput;
			m_pOutput = NULL;
			return NULL;
		}
	}

	/* Find which pin is required */

	switch(n) 
	{
	case 0:
		return m_pInput;
	case 1:
		return m_pOutput;
	}

	return NULL;

} // GetPin


//
// NonDelegatingQueryInterface
//
// Override CUnknown method.
// Part of the basic COM (Compound Object Model) mechanism.
// This is how we expose our interfaces.
//
STDMETHODIMP CLaserLocateFilter::NonDelegatingQueryInterface(REFIID riid, void **ppv)
{
	CheckPointer(ppv,E_POINTER);

	if (riid == IID_INullIPP) {
		return GetInterface((INullIPP *) this, ppv);
	}
	else if (riid == IID_ISpecifyPropertyPages) {
		return GetInterface((ISpecifyPropertyPages *) this, ppv);
	}
	else {
		return CTransformFilter::NonDelegatingQueryInterface(riid, ppv);
	}

} // NonDelegatingQueryInterface


// get_IPin
//
// INull method.
// Set *ppPin to the upstream output pin which supplies us
// or to NULL if there is no upstream pin connected to us.
//
STDMETHODIMP CLaserLocateFilter::get_IPin (IPin **ppPin)
{
	CheckPointer(ppPin,E_POINTER);
	CAutoLock l(&m_NullIPLock);

	if (!m_pInput)
	{
		*ppPin = NULL;
		return NOERROR;
	}

	if (!m_pInput->IsConnected())
	{
		*ppPin = NULL;
	}
	else
	{
		*ppPin = m_pInput->GetConnected ();
		(*ppPin)->AddRef();
	}

	return NOERROR ;

} // get_IPin


//
// put_MediaType
//
// INull method.
//
STDMETHODIMP CLaserLocateFilter::put_MediaType(CMediaType *pmt)
{
	CheckPointer(pmt,E_POINTER);
	CAutoLock l(&m_NullIPLock);

	//
	// if the state of the graph is running, fail the call.
	//
	if (m_State == State_Running)
		return E_UNEXPECTED ;

	//
	// check the source and sink filters like this media type
	//
	if (pmt == NULL)
	{
		m_mtPreferred.InitMediaType ();
	}
	else 
	{
		IPin *pPin= m_pInput->GetConnected();
		if (pPin) 
		{
			if (pPin->QueryAccept(pmt) != NOERROR) 
			{
				MessageBox(NULL,TEXT("Upstream filter cannot provide this type"),
					TEXT("Format Selection"),
					MB_OK | MB_ICONEXCLAMATION);
				return VFW_E_TYPE_NOT_ACCEPTED;
			}
		}

		pPin= m_pOutput->GetConnected();
		if (pPin) 
		{
			if (pPin->QueryAccept(pmt) != NOERROR) 
			{
				MessageBox(NULL,TEXT("Downstream filter cannot accept this type"),
					TEXT("Format Selection"),
					MB_OK | MB_ICONEXCLAMATION);
				return VFW_E_TYPE_NOT_ACCEPTED;
			}
		}

		m_mtPreferred = *pmt ;
	}

	//
	// force reconnect of input if the media type of connection does not match.
	//
	if( m_pInput->IsConnected() )
	{
		if (m_pInput->CurrentMediaType()!= m_mtPreferred)
			m_pGraph->Reconnect(m_pInput);
	}

	return NOERROR;

} // put_MediaType


//
// get_MediaType
//
// INull method.
// Set *pmt to the current preferred media type.
//
STDMETHODIMP CLaserLocateFilter::get_MediaType(CMediaType **pmt)
{
	CheckPointer(pmt,E_POINTER);
	CAutoLock l(&m_NullIPLock);

	*pmt = &m_mtPreferred;
	return NOERROR;

} // get_MediaType


//
// get_State
//
// INull method
// Set *state to the current state of the filter (State_Stopped etc)
//
STDMETHODIMP CLaserLocateFilter::get_State(FILTER_STATE *pState)
{
	CheckPointer(pState,E_POINTER);
	CAutoLock l(&m_NullIPLock);

	*pState = m_State;
	return NOERROR;

} // get_State


HRESULT CLaserLocateFilter::Transform(IMediaSample *pSample)
{		
	BYTE *pData = NULL;
	long lData;
	threshold = 240;

	CMediaType mt = m_pInput->CurrentMediaType();

	VIDEOINFOHEADER *pVih = (VIDEOINFOHEADER*) mt.pbFormat;
	int nWidth = pVih->bmiHeader.biWidth;
	int nHeight = pVih->bmiHeader.biHeight;
	pSample->GetPointer(&pData);
	lData = pSample->GetActualDataLength();
	tagRGBTRIPLE *prgb;
	prgb = (tagRGBTRIPLE *)pData;
	pHead.next = NULL;
	point = &pHead;
	findRedPoint(prgb,lData/3,nWidth,nHeight);
	return NOERROR;
}

void CLaserLocateFilter::findRedPoint(tagRGBTRIPLE *prgb, long lData,int nWidth,int nHeight)
{	
	CPoint* header = point;
	for(long i=0;i<lData;i++)
	{
		if(prgb[i].rgbtRed>threshold)
		{
			CPoint * pointTemp = new CPoint;
			point->next = pointTemp;
			pointTemp->x = i/nWidth;
			pointTemp->y = i%nWidth;
			pointTemp->next = NULL;
			point = point->next;
		}
	}
	point = header->next;
	int xSum=0;
	int ySum=0;
	int count=0;
	while(point!=NULL)
	{
		xSum += point->x;
		ySum += point->y;
		count++;
		point = point->next;
	}
	if(count)
	{
		int xAvg = xSum/count;
		int yAvg = ySum/count;
		for(int i =0;i<=5;i++)
		{
			if((xAvg+i)<nHeight)
			{prgb[(xAvg+i)*nWidth+yAvg].rgbtRed=0;}
			if(xAvg-i>=0)
			{prgb[(xAvg-i)*nWidth+yAvg].rgbtRed=0;}
			if(yAvg+i<nWidth)
			{prgb[xAvg*nWidth+yAvg+i].rgbtRed=0;}
			if(yAvg-i>=0)
			{prgb[xAvg*nWidth+yAvg-i].rgbtRed=0;}
		}
	}
}
