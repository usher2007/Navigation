#include "stdafx.h"
#include <streams.h>
#include "TMReceiverSrc.h"

static const LONGLONG ONESEC = 10000000;
DWORD ReaderProc(LPVOID pParam);
const AMOVIESETUP_MEDIATYPE sudOpVideoPinTypes =
{
	&MEDIATYPE_Video,       // Major type
	&MEDIASUBTYPE_NULL      // Minor type
};

const AMOVIESETUP_PIN sudOpPins[] =
{
	{
		L"Video Output",              // Pin string name
			FALSE,                  // Is it rendered
			TRUE,                   // Is it an output
			FALSE,                  // Can we have none
			FALSE,                  // Can we have many
			&CLSID_NULL,            // Connects to filter
			NULL,                   // Connects to pin
			1,                      // Number of types
			&sudOpVideoPinTypes 
	}
};

const AMOVIESETUP_FILTER sudTMReceiverSrcax =
{
	&CLSID_TMReceiverSrc,    // Filter CLSID
	L"TMReceiver Source Filter",       // String name
	MERIT_DO_NOT_USE,       // Filter merit
	1,                      // Number pins
	sudOpPins               // Pin details
};

CUnknown * WINAPI CTMReceiverSrc::CreateInstance(LPUNKNOWN lpunk, HRESULT *phr)
{
	ASSERT(phr);
	CUnknown *punk = new CTMReceiverSrc(lpunk, phr);
	if(punk == NULL)
	{
		if(phr)
			*phr = E_OUTOFMEMORY;
	}
	return punk;
}

CTMReceiverSrc::CTMReceiverSrc(LPUNKNOWN lpunk, HRESULT *phr)
	: CSource(NAME("TMReceiver Source Filter"), lpunk, CLSID_TMReceiverSrc)
{
	ASSERT(phr);
	CAutoLock cAutoLock(&m_cStateLock);
	m_paStreams = (CSourceStream **)new CSourceStream*[1];
	if(m_paStreams == NULL)
	{
		if(phr)
			*phr = E_OUTOFMEMORY;
		return;
	}

	m_pVideoPin = new CTMReceiverOutputPin(phr, this, L"TMReceiver Video Pin");

	m_paStreams[0] = m_pVideoPin;
	m_pFileName = NULL;
	H264Dec_Init();
	m_pVideoPin->hDecoder = NULL;
}

CTMReceiverSrc::~CTMReceiverSrc()
{
	if(m_pFileName != NULL)
	{
		delete[] m_pFileName;
		m_pFileName = NULL;
	}
	if(m_paStreams != NULL)
	{
		delete[] m_paStreams;
		m_paStreams = NULL;
	}
	m_iPins = 0;
}

HRESULT CTMReceiverSrc::Load(LPCOLESTR pszFileName, const AM_MEDIA_TYPE *pmt)
{
	//TODO: Open the file using ffmpeg;
	USES_CONVERSION;
	AVCodecContext *pVideoCodecCtx = NULL;
	AVCodec *pVideoCodec = NULL;
	avcodec_register_all();
	av_register_all();
	avformat_network_init();

	int cch = lstrlenW(pszFileName) + 1;
	m_pFileName = new WCHAR[cch];
	if(m_pFileName != NULL)
	{
		CopyMemory(m_pFileName, pszFileName, cch*sizeof(WCHAR));
	}

	m_pFormatContext = avformat_alloc_context();
	int err = avformat_open_input(&m_pFormatContext, W2A(m_pFileName), NULL, NULL);
	if(err != 0)
	{
		return E_FAIL;
	}
	m_pFormatContext->flags |= AVFMT_FLAG_GENPTS;
	m_pFormatContext->flags |= AVFMT_GENERIC_INDEX;
	m_pFormatContext->max_index_size = 99;
	m_pFormatContext->probesize /= 3;
	m_pFormatContext->max_analyze_duration /= 3;

	if(av_find_stream_info(m_pFormatContext) < 0)
	{
		return E_FAIL;
	}

	av_dump_format(m_pFormatContext, 0, W2A(m_pFileName), false);

	for(int i=0; i<m_pFormatContext->nb_streams; i++)
	{
		if(m_pFormatContext->streams[i]->codec->codec_type == AVMEDIA_TYPE_VIDEO)
		{
			m_videoStreamIndex = i;
			pVideoCodecCtx = m_pFormatContext->streams[i]->codec;
			m_resolution.width = pVideoCodecCtx->width;
			m_resolution.height = pVideoCodecCtx->height;
		}
	}

	//TODO: Start a thread to read the packet.
	DWORD dwThreadId = 0;
	m_readerThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)ReaderProc, (LPVOID*)this, 0, &dwThreadId);
	return S_OK;
}

HRESULT CTMReceiverSrc::GetCurFile(LPOLESTR * ppszFileName,AM_MEDIA_TYPE * pmt){
	CheckPointer(ppszFileName, E_POINTER);
	*ppszFileName = NULL;
	if (m_pFileName!=NULL) {
		DWORD n = sizeof(WCHAR)*(1+lstrlenW(m_pFileName));
		*ppszFileName = (LPOLESTR) CoTaskMemAlloc( n );
		if (*ppszFileName!=NULL) {
			CopyMemory(*ppszFileName, m_pFileName, n);
		}
	}
	return S_OK;
}

STDMETHODIMP CTMReceiverSrc::Run(REFERENCE_TIME tStart){

	CAutoLock cObjectLock(m_pLock);

	{
		CAutoLock lock(&(m_pVideoPin->m_csDecoder));	
		m_pVideoPin->hDecoder = H264Dec_Create();
	}
	m_pVideoPin->m_bWorking = TRUE;
	m_pVideoPin->m_rtFirstFrameTime = 0;
	m_pVideoPin->m_rtSampleTime = 0;
	m_pVideoPin->m_rtPosition = 0;
	m_pVideoPin->m_bGetAvgFrameTime = FALSE;
	return CBaseFilter::Run(tStart);
}

STDMETHODIMP CTMReceiverSrc::Pause(){
	CAutoLock cObjectLock(m_pLock);
	return CBaseFilter::Pause();
}

STDMETHODIMP CTMReceiverSrc::Stop(){
	CAutoLock cObjectLock(m_pLock);
	m_pVideoPin->m_bWorking = FALSE;
	int ret = 0;
	{
		CAutoLock lock(&(m_pVideoPin->m_csDecoder));
		if(m_pVideoPin->hDecoder != NULL){
			H264Dec_Delete(m_pVideoPin->hDecoder);
			m_pVideoPin->hDecoder = NULL;
		}
	}
	m_pVideoPin->m_rtFirstFrameTime = 0;
	m_pVideoPin->m_rtSampleTime = 0;
	return CBaseFilter::Stop();

}

void CTMReceiverSrc::ReadAndCachePreviewPackets()
{
	//TODO: READ AND CACHE PACKETS.
}


//////////////////////////////////////////////////////////////////
// Video Output Pin
//////////////////////////////////////////////////////////////////

CTMReceiverOutputPin::CTMReceiverOutputPin(HRESULT *phr, CTMReceiverSrc *pParent, LPCTSTR pPinName)
	: CSourceStream(NAME("TMReceiver Source Filter Output Pin"), phr, pParent, pPinName)
{
	m_rtPosition = 0;
	m_rtSampleTime = 0;
	m_bWorking = FALSE;
	m_bGetAvgFrameTime = FALSE;
	m_pData = NULL;
	m_rtAvgTimePerFrame = ONESEC/FPS;
	m_rtFirstFrameTime = 0;
	m_pinIntialized = FALSE;
}

CTMReceiverOutputPin::~CTMReceiverOutputPin()
{

}

HRESULT CTMReceiverOutputPin::FillBuffer(IMediaSample *pms)
{
	//TODO: Fill buffer with the decoded frames.
	return S_OK;
}

STDMETHODIMP CTMReceiverOutputPin::NonDelegatingQueryInterface(REFIID riid, void **ppv){
	return CSourceStream::NonDelegatingQueryInterface(riid, ppv);
}

STDMETHODIMP CTMReceiverOutputPin::Notify(IBaseFilter * pSender, Quality q){

	return S_OK;
}  

HRESULT CTMReceiverOutputPin::DecideBufferSize(IMemAllocator *pAlloc,ALLOCATOR_PROPERTIES *pProperties){

	CheckPointer(pAlloc,E_POINTER);
	CheckPointer(pProperties,E_POINTER);
	CAutoLock cAutoLock(m_pFilter->pStateLock());
	HRESULT hr = S_OK;
	pProperties->cBuffers = 1;
	pProperties->cbBuffer = resolution.width*resolution.height*sizeof(RGBQUAD);
	ASSERT(pProperties->cbBuffer);
	ALLOCATOR_PROPERTIES Actual;
	hr = pAlloc->SetProperties(pProperties,&Actual);
	if(FAILED(hr))
	{
		return hr;
	}
	if(Actual.cbBuffer < pProperties->cbBuffer)
	{
		return E_FAIL;
	}
	ASSERT(Actual.cBuffers == 1);

	return S_OK;	
}

HRESULT CTMReceiverOutputPin::CheckMediaType(const CMediaType *pMediaType){
	CheckPointer(pMediaType,E_POINTER);

	if((*(pMediaType->Type()) != MEDIATYPE_Video) ||   // we only output video
		!(pMediaType->IsFixedSize()))                   // in fixed size samples
	{                                                  
		return E_INVALIDARG;
	}

	// Check for the subtypes we support
	const GUID *SubType = pMediaType->Subtype();
	if (SubType == NULL)
		return E_INVALIDARG;

	if(*SubType != MEDIASUBTYPE_RGB32)
	{
		return E_INVALIDARG;
	}

	// Get the format area of the media type
	VIDEOINFO *pvi = (VIDEOINFO *) pMediaType->Format();

	if(pvi == NULL)
		return E_INVALIDARG;

	// Check the image size. As my default ball is 10 pixels big
	// look for at least a 20x20 image. This is an arbitary size constraint,
	// but it avoids balls that are bigger than the picture...

	// Check if the image width & height have changed
	if(pvi->bmiHeader.biWidth != resolution.width|| 
		pvi->bmiHeader.biHeight != resolution.height)
	{
		// If the image width/height is changed, fail CheckMediaType() to force
		// the renderer to resize the image.
		return E_INVALIDARG;
	}


	return S_OK;  // This format is acceptable.

}

HRESULT CTMReceiverOutputPin::GetMediaType(int iPosition, CMediaType *pmt){
	CheckPointer(pmt,E_POINTER);

	CAutoLock cAutoLock(m_pFilter->pStateLock());
	if(iPosition < 0)
	{
		return E_INVALIDARG;
	}
	if(iPosition > 0)
	{
		return VFW_S_NO_MORE_ITEMS;
	}
	VIDEOINFO* pvih = (VIDEOINFO*)pmt->AllocFormatBuffer(sizeof(VIDEOINFO));
	LPBITMAPINFOHEADER lpBitmapInfoHeader = &(pvih->bmiHeader);
	lpBitmapInfoHeader->biSize = sizeof(BITMAPINFOHEADER);
	lpBitmapInfoHeader->biBitCount = 32;
	lpBitmapInfoHeader->biWidth = resolution.width/4*4;
	lpBitmapInfoHeader->biHeight = resolution.height;
	lpBitmapInfoHeader->biPlanes = 1;
	lpBitmapInfoHeader->biCompression = BI_RGB;
	lpBitmapInfoHeader->biSizeImage = resolution.width / 4 * 4 * resolution.height * 4;
	lpBitmapInfoHeader->biXPelsPerMeter = 0;
	lpBitmapInfoHeader->biYPelsPerMeter =0;
	lpBitmapInfoHeader->biClrUsed = 0;
	lpBitmapInfoHeader->biClrImportant = 0;
	pvih->AvgTimePerFrame = m_rtAvgTimePerFrame;
	pmt->SetFormatType(&FORMAT_VideoInfo);
	pmt->SetTemporalCompression(FALSE);

	SetRectEmpty(&(pvih->rcSource)); // we want the whole image area rendered.
	SetRectEmpty(&(pvih->rcTarget)); // no particular destination rectangle

	pmt->SetType(&MEDIATYPE_Video);

	// Work out the GUID for the subtype from the header info.
	const GUID SubTypeGUID = GetBitmapSubtype(&pvih->bmiHeader);
	pmt->SetSubtype(&SubTypeGUID);
	pmt->SetSampleSize(pvih->bmiHeader.biSizeImage);

	return S_OK;

}

HRESULT CTMReceiverOutputPin::GetMediaType(CMediaType *pmt){
	VIDEOINFO* pvih = (VIDEOINFO*)pmt->AllocFormatBuffer(sizeof(VIDEOINFO));
	LPBITMAPINFOHEADER lpBitmapInfoHeader = &(pvih->bmiHeader);
	lpBitmapInfoHeader->biSize = sizeof(BITMAPINFOHEADER);
	lpBitmapInfoHeader->biBitCount = 32;
	lpBitmapInfoHeader->biWidth = resolution.width/4*4;
	lpBitmapInfoHeader->biHeight = resolution.height;
	lpBitmapInfoHeader->biPlanes = 1;
	lpBitmapInfoHeader->biCompression = BI_RGB;
	lpBitmapInfoHeader->biSizeImage = resolution.width / 4 * 4 * resolution.height * 4;
	lpBitmapInfoHeader->biXPelsPerMeter = 0;
	lpBitmapInfoHeader->biYPelsPerMeter =0;
	lpBitmapInfoHeader->biClrUsed = 0;
	lpBitmapInfoHeader->biClrImportant = 0;
	pvih->AvgTimePerFrame = m_rtAvgTimePerFrame;
	pmt->SetFormatType(&FORMAT_VideoInfo);
	pmt->SetTemporalCompression(FALSE);

	SetRectEmpty(&(pvih->rcSource)); // we want the whole image area rendered.
	SetRectEmpty(&(pvih->rcTarget)); // no particular destination rectangle

	pmt->SetType(&MEDIATYPE_Video);

	// Work out the GUID for the subtype from the header info.
	const GUID SubTypeGUID = GetBitmapSubtype(&pvih->bmiHeader);
	pmt->SetSubtype(&SubTypeGUID);
	pmt->SetSampleSize(pvih->bmiHeader.biSizeImage);

	return S_OK;
}

STDMETHODIMP CTMReceiverOutputPin::Run()
{
	m_rtFirstFrameTime = 0;
	return CSourceStream::Run();
}

STDMETHODIMP CTMReceiverOutputPin::Stop()
{
	m_rtFirstFrameTime = 0;
	return CSourceStream::Stop();
}

DWORD ReaderProc(LPVOID pParam)
{
	//USES_CONVERSION;
	CTMReceiverSrc *pReceiverSrc = (CTMReceiverSrc *)pParam;
	pReceiverSrc->ReadAndCachePreviewPackets();
	return 0;
}