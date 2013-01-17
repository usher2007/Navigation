#include "stdafx.h"
#include <streams.h>
#include "TMReceiverSrc.h"

static const LONGLONG ONESEC = 10000000;
static const int FPS = 25;
static const int DEFAULT_WIDTH = 720;
static const int DEFAULT_HEIGHT = 576;
DWORD ReaderProc(LPVOID pParam);
const AMOVIESETUP_MEDIATYPE sudOpVideoPinTypes =
{
	&MEDIATYPE_Video,       // Major type
	&MEDIASUBTYPE_NULL      // Minor type
};
const AMOVIESETUP_MEDIATYPE sudOpAudioPinTypes =
{
	&MEDIATYPE_Audio,       // Major type
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
	},
	{
		L"Audio Output",              // Pin string name
			FALSE,                  // Is it rendered
			TRUE,                   // Is it an output
			FALSE,                  // Can we have none
			FALSE,                  // Can we have many
			&CLSID_NULL,            // Connects to filter
			NULL,                   // Connects to pin
			1,                      // Number of types
			&sudOpAudioPinTypes 
		}
};

const AMOVIESETUP_FILTER sudTMReceiverSrcax =
{
	&CLSID_TMStreamReceiver,    // Filter CLSID
	L"TMReceiver Source Filter",       // String name
	MERIT_DO_NOT_USE,       // Filter merit
	2,                      // Number pins
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
	: CSource(NAME("TMReceiver Source Filter"), lpunk, CLSID_TMStreamReceiver)
{
	ASSERT(phr);
	CAutoLock cAutoLock(&m_cStateLock);
	m_paStreams = (CSourceStream **)new CSourceStream*[2];
	if(m_paStreams == NULL)
	{
		if(phr)
			*phr = E_OUTOFMEMORY;
		return;
	}

	m_pVideoPin = new CTMReceiverVideoOutputPin(phr, this, L"TMReceiver Video Pin");

	m_paStreams[0] = m_pVideoPin;
	m_pFileName = NULL;
	m_bRecordStatus = FALSE;
	m_bHasAudio = FALSE;
	memset(m_recordFileName, 0x00, sizeof(m_recordFileName));
	beforeDecodeCB = NULL;
	beforeCBParam = NULL;
	afterDecodeCB = NULL;
	afterCBParam = NULL;
	m_bQuitReaderThread = FALSE;
}

CTMReceiverSrc::~CTMReceiverSrc()
{
	m_bQuitReaderThread = TRUE;
	while(m_bQuitReaderThread)
	{
		Sleep(100);
	}
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
	HRESULT hr = S_FALSE;
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
		else if(m_pFormatContext->streams[i]->codec->codec_type = AVMEDIA_TYPE_AUDIO)
		{
			m_audioStreamIndex = i;
			m_pAudioPin = new CTMReceiverAudioOutputPin(&hr, this, L"TMReceiver Audio Pin");
			m_paStreams[1] = m_pAudioPin;
			m_pAudioPin->m_pAudioCodecCtx = m_pFormatContext->streams[i]->codec;
			m_pAudioPin->m_pAudioCodec = avcodec_find_decoder((m_pAudioPin->m_pAudioCodecCtx->codec_id));
			if(m_pAudioPin->m_pAudioCodec == NULL)
			{
				continue;
			}
			if(avcodec_open(m_pAudioPin->m_pAudioCodecCtx, m_pAudioPin->m_pAudioCodec) < 0)
			{
				continue;
			}
			m_bHasAudio = TRUE;
			m_pAudioPin->m_rtAvgTimePerPts = m_pFormatContext->streams[i]->time_base.num * ONESEC / m_pFormatContext->streams[i]->time_base.den;
		}
	}

	if(m_resolution.width != DEFAULT_WIDTH || m_resolution.height != DEFAULT_HEIGHT)
	{
		return E_FAIL;
	}

	if(m_bHasAudio)
	{
		m_pAudioPin->m_pRelatedVideoPin = m_pVideoPin;
	}

	//TODO: Start a thread to read the packet.
	m_bQuitReaderThread = FALSE;
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
		m_pVideoPin->m_pDecoder = new H264Decoder;
		m_pVideoPin->m_pDecoder->Init(m_pFormatContext->streams[m_videoStreamIndex]->codec);
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
		if(m_pVideoPin->m_pDecoder != NULL){
			m_pVideoPin->m_pDecoder->Release();
			delete m_pVideoPin->m_pDecoder;
			m_pVideoPin->m_pDecoder = NULL;
		}
	}
	m_pVideoPin->m_rtFirstFrameTime = 0;
	m_pVideoPin->m_rtSampleTime = 0;
	return CBaseFilter::Stop();

}

void CTMReceiverSrc::ReadAndCachePreviewPackets()
{
	//TODO: READ AND CACHE PACKETS.
	AVPacket packet;
	AVPacket packetToAbolish;
	while(!m_bQuitReaderThread)
	{
		if(m_pVideoPin->m_queueBuffer.nb_packets > 256)
		{
			for(int i=0; i<10; i++)
			{
				av_init_packet(&packetToAbolish);
				m_pVideoPin->m_queueBuffer.Get(&packetToAbolish, 1);
				//m_channelPts[channel] = packetToAbolish.pts;
				av_free_packet(&packetToAbolish);
			}
		}
		int ret = av_read_frame(m_pFormatContext, &packet);
		if(ret < 0)
		{
			/*av_init_packet(&packet);
			packet.data = NULL;
			packet.size = 0;
			packet.stream_index = m_videoStreamIndex;
			m_queueBuffer.Put(&packet);*/
			//For debug
			char tmp[1024];
			sprintf(tmp," ===================================BAD Put %d bad Packet!\n", ret);
			OutputDebugStringA(tmp);
			Sleep(10);
			continue;
		}
		if(packet.stream_index == m_videoStreamIndex)
		{
			//For debug
			char tmp[1024];
			sprintf(tmp," ===================================GOOD Put %d good Video Packet!DTS:%lld\n",m_pVideoPin->m_queueBuffer.nb_packets,packet.dts);
			OutputDebugStringA(tmp);
			//m_channelPts[channel] = packet.pts;
			m_pVideoPin->m_queueBuffer.Put(&packet);
			//char tmp[1024];
			//sprintf(tmp, "Channel %d__PTS %lld\n", channel, packet.pts);
			//OutputDebugStringA(tmp);
			//continue;
		}
		else if(m_bHasAudio && packet.stream_index == m_audioStreamIndex)
		{
			// TODO
			char tmp[1024];
			sprintf(tmp," ===================================GOOD Put %d good Audio Packet!DTS:%lld\n",m_pAudioPin->m_queueBuffer.nb_packets,packet.dts);
			OutputDebugStringA(tmp);
			m_pAudioPin->m_queueBuffer.Put(&packet);
		}
		Sleep(10);
	}
	m_bQuitReaderThread = FALSE;
	return;
}

STDMETHODIMP CTMReceiverSrc::StartRecord(const char *fileName)
{
	memcpy(m_recordFileName, fileName, strlen(fileName));
	m_pVideoPin->InitRecord(m_recordFileName);
	m_bRecordStatus = TRUE;
	return S_OK;
}

STDMETHODIMP CTMReceiverSrc::StopRecord()
{
	if(m_bRecordStatus == TRUE)
	{
		m_bRecordStatus = FALSE;
		m_pVideoPin->StopRecord();
		memset(m_recordFileName, 0x00, sizeof(m_recordFileName));
	}
	return S_OK;
}

STDMETHODIMP CTMReceiverSrc::SetCallBackBeforeDecode(TMReceiverCB cb, void* arg)
{
	beforeDecodeCB = cb;
	beforeCBParam = arg;
	return S_OK;
}

STDMETHODIMP CTMReceiverSrc::SetCallBackAfterDecode(TMReceiverCB cb, void* arg)
{
	afterDecodeCB = cb;
	afterCBParam = arg;
	return S_OK;
}

STDMETHODIMP CTMReceiverSrc::IsSourceHasAudio()
{
	if(m_bHasAudio)
	{
		return S_OK;
	}
	else
	{
		return E_FAIL;
	}
}

int CTMReceiverSrc::CallBeforeDecodeCB(TMFrame *pFrame)
{
	if(beforeDecodeCB != NULL)
	{
		return beforeDecodeCB(pFrame, beforeCBParam);
	}
	return -1;
}

int CTMReceiverSrc::CallAfterDecodeCB(TMFrame *pFrame)
{
	if(afterDecodeCB != NULL)
	{
		return afterDecodeCB(pFrame, afterCBParam);
	}
	return -1;
}

//////////////////////////////////////////////////////////////////
// Video Output Pin
//////////////////////////////////////////////////////////////////

CTMReceiverVideoOutputPin::CTMReceiverVideoOutputPin(HRESULT *phr, CTMReceiverSrc *pParent, LPCWSTR pPinName)
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
	m_pDecoder = NULL;
	m_bFPSGuessed = FALSE;
}

CTMReceiverVideoOutputPin::~CTMReceiverVideoOutputPin()
{
	m_pDecoder->Release();
	delete m_pDecoder;
	m_pDecoder = NULL;
}

HRESULT CTMReceiverVideoOutputPin::FillBuffer(IMediaSample *pms)
{
	//TODO: Fill buffer with the decoded frames.
	CTMReceiverSrc* pFilter = (CTMReceiverSrc*)m_pFilter;
	char tmp[1024];
	sprintf(tmp," ===================================Fill Buffer In!\n");
	OutputDebugStringA(tmp);
	AVPacket pkt, pktForRecord;
	AVPicture pic;
	BYTE *pData;
	long lDataLen;
	lDataLen = pms->GetSize();
	if (m_pData==NULL)
	{
		m_pData = new BYTE[lDataLen];
	}
	if(m_queueBuffer.nb_packets<=0)
	{
		REFERENCE_TIME rtStart, rtStop, rtMediaStart, rtMediaStop;
		// The sample times are modified by the current rate.
		rtStart = static_cast<REFERENCE_TIME>(m_rtSampleTime);
		rtStop  = rtStart + static_cast<int>(m_rtAvgTimePerFrame );
		rtMediaStart = static_cast<REFERENCE_TIME>(m_rtPosition);
		rtMediaStop  = rtMediaStart + static_cast<int>(m_rtAvgTimePerFrame );
		pms->SetTime(&rtStart, &rtStop);
		pms->SetMediaTime(&rtMediaStart, &rtMediaStop);
		m_rtSampleTime = m_rtSampleTime + static_cast<int>(m_rtAvgTimePerFrame );
		m_rtPosition = m_rtPosition + m_rtAvgTimePerFrame;
		pms->SetSyncPoint(TRUE);
		Sleep(10);
		//char tmp[1024];
		//sprintf(tmp,"====================No Data!====================\n");
		//OutputDebugStringA(tmp);
		return S_OK;
	}
	av_init_packet(&pkt);
	int maxPktNum = m_bGetAvgFrameTime ? 10 : 5;
	while (m_queueBuffer.nb_packets > maxPktNum)
	{
		CAutoLock lock(&m_csBuffer);
		m_queueBuffer.Get(&pkt,1);
		av_free_packet(&pkt);
	}

	{
		CAutoLock lock(&m_csBuffer);
		m_queueBuffer.Get(&pkt, 1);
		if(pkt.flags & AV_PKT_FLAG_KEY)
		{
	/*char tmp[1024];
	sprintf(tmp,"Key Frame!\n");
	OutputDebugStringA(tmp);*/
		}
	}
	int ret = -1;
	//Record Video
	if(m_bRecordStatus == TRUE)
	{
		if(pkt.flags & AV_PKT_FLAG_KEY)
		{
			m_bFindKeyFrame = TRUE;
		}
		if(m_bFindKeyFrame)
		{
			av_init_packet(&pktForRecord);
			pktForRecord.size = pkt.size;
			pktForRecord.flags = pkt.flags;
			pktForRecord.pts = pts;
			pktForRecord.dts = pts;
			pktForRecord.data = new uint8_t[pktForRecord.size];
			memcpy(pktForRecord.data, pkt.data, pktForRecord.size);
			ret = av_interleaved_write_frame(m_fileSaverCtx, &pktForRecord);
			delete [] pktForRecord.data;
			pktForRecord.data = NULL;
			pktForRecord.size = 0;
			av_init_packet(&pktForRecord);
			av_free_packet(&pktForRecord);
			//pts += m_rtAvgTimePerFrame/1000*9;
			pts++;
		}
	}

	// BEFORE DECODE CB
	TMFrame beforeDecodeFrame;
	beforeDecodeFrame.data = (char *)pkt.data;
	beforeDecodeFrame.len = pkt.size;
	beforeDecodeFrame.decoded = FALSE;
	beforeDecodeFrame.error = FALSE;
	pFilter->CallBeforeDecodeCB(&beforeDecodeFrame);

	ret = -1;
	{
		CAutoLock lock(&m_csDecoder);
		if (m_pDecoder!=NULL)
		{			
			ret = m_pDecoder->DecodeFrame(&pic, m_pData, pkt.data, pkt.size);
		}		
	}

	// AFTER DECODE CB
	TMFrame afterDecodeFrame;
	afterDecodeFrame.data = (char *)pkt.data;
	afterDecodeFrame.len = pkt.size;
	afterDecodeFrame.decoded = TRUE;
	afterDecodeFrame.error = ret <= 0 ? TRUE : FALSE;
	// TODO: construct the pic
	for(int ptr_i=0; ptr_i<AV_NUM_DATA_POINTERS; ptr_i++)
	{
		afterDecodeFrame.pic.data[ptr_i] = pic.data[ptr_i];
		afterDecodeFrame.pic.linesize[ptr_i] = pic.linesize[ptr_i];
	}
	pFilter->CallAfterDecodeCB(&afterDecodeFrame);

	if(ret <=0)
	{
		char tmp[1024];
		sprintf(tmp," ===================================BAD£¬rtSampleTime:%lld\n",m_rtSampleTime);
		OutputDebugStringA(tmp);
		REFERENCE_TIME rtStart, rtStop, rtMediaStart, rtMediaStop;
		// The sample times are modified by the current rate.
		rtStart = static_cast<REFERENCE_TIME>(m_rtSampleTime);
		rtStop  = rtStart + static_cast<int>(m_rtAvgTimePerFrame );
		rtMediaStart = static_cast<REFERENCE_TIME>(m_rtPosition);
		rtMediaStop  = rtMediaStart + static_cast<int>(m_rtAvgTimePerFrame );
		pms->SetTime(&rtStart, &rtStop);
		pms->SetMediaTime(&rtMediaStart, &rtMediaStop);
		m_rtSampleTime = rtStop;
		m_rtPosition = m_rtPosition + m_rtAvgTimePerFrame;
		pms->SetSyncPoint(TRUE);
		return S_OK;
	}


	pms->GetPointer(&pData);

	USES_CONVERSION;
	ZeroMemory(pData, lDataLen);	
	{
		CAutoLock cAutoLockShared(&m_cSharedState);	
		memcpy(pData,m_pData,lDataLen);
		//hack the 1920*1088, the last 8 line should be set to 0.
		if(pFilter->GetImageHeight() == 1088)
		{
			memset(pData, 0, pFilter->GetImageWidth()*8*sizeof(RGBQUAD));
		}
		//hack the 720*576, the first and last 2 lines should be set to 0.
		if(pFilter->GetImageHeight() == 576)
		{
			memset(pData, 0, pFilter->GetImageWidth()*2*sizeof(RGBQUAD));
			memset(pData + pFilter->GetImageWidth()*(pFilter->GetImageHeight()-2)*sizeof(RGBQUAD), 0, pFilter->GetImageWidth()*2*sizeof(RGBQUAD));
		}
		REFERENCE_TIME rtStart, rtStop, rtMediaStart, rtMediaStop;
		// The sample times are modified by the current rate.
		//rtStart = static_cast<REFERENCE_TIME>(m_rtSampleTime);
		if(m_rtFirstFrameTime == 0)
		{
			m_rtFirstFrameTime = pkt.pts ;
		}
		rtStart = (pkt.pts  - m_rtFirstFrameTime)*100/9*10;
		rtStart = rtStart < m_rtPosition ? rtStart : m_rtPosition;
		rtStop  = rtStart + static_cast<int>(m_rtAvgTimePerFrame );
		rtMediaStart = static_cast<REFERENCE_TIME>(m_rtPosition);
		rtMediaStop  = rtMediaStart + static_cast<int>(m_rtAvgTimePerFrame );
		pms->SetTime(&rtStart, &rtStop);
		pms->SetMediaTime(&rtMediaStart, &rtMediaStop);
		m_rtSampleTime = rtStop;
		m_rtPosition = m_rtPosition + m_rtAvgTimePerFrame; 
		//char tmp[1024];
		//sprintf(tmp," Src Filter:Channel:%d__PTS:%lld__rtStart:%lld\n", pFilter->m_relatedChannel, pkt.pts, rtStart);
		//OutputDebugStringA(tmp);
	}
	pms->SetSyncPoint(TRUE);

//For debug
//char tmp2[1024];
//sprintf(tmp2,"Channel %d__Fill Buffer Finallly %d!\n", pFilter->m_relatedChannel, frame_count);
//OutputDebugStringA(tmp2);

	av_free_packet(&pkt);

	//CallBack
	//DecodeCallback decodeCB = NULL;
	//void *pCBParam = NULL;
	//HRESULT hr = pFilter->m_pConfigManager->GetDecodeCB(pFilter->m_relatedChannel, &decodeCB, &pCBParam);
	//if(SUCCEEDED(hr) && decodeCB != NULL)
	//{
	//	decodeCB(m_pData, lDataLen, pCBParam);
	//}
	return S_OK;
	return S_OK;
}

STDMETHODIMP CTMReceiverVideoOutputPin::NonDelegatingQueryInterface(REFIID riid, void **ppv){
	return CSourceStream::NonDelegatingQueryInterface(riid, ppv);
}

STDMETHODIMP CTMReceiverVideoOutputPin::Notify(IBaseFilter * pSender, Quality q){

	return S_OK;
}  

HRESULT CTMReceiverVideoOutputPin::DecideBufferSize(IMemAllocator *pAlloc,ALLOCATOR_PROPERTIES *pProperties){

	CheckPointer(pAlloc,E_POINTER);
	CheckPointer(pProperties,E_POINTER);
	CAutoLock cAutoLock(m_pFilter->pStateLock());
	HRESULT hr = S_OK;
	pProperties->cBuffers = 1;
	int imageWidth = ((CTMReceiverSrc *)m_pFilter)->GetImageWidth();
	int imageHeight = ((CTMReceiverSrc *)m_pFilter)->GetImageHeight();
	if(imageWidth < 0 || imageHeight < 0)
	{
		return E_FAIL;
	}
	pProperties->cbBuffer = imageWidth*imageHeight*sizeof(RGBQUAD);
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

HRESULT CTMReceiverVideoOutputPin::CheckMediaType(const CMediaType *pMediaType){
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
	if(pvi->bmiHeader.biWidth != ((CTMReceiverSrc *)m_pFilter)->GetImageWidth()|| 
		pvi->bmiHeader.biHeight != ((CTMReceiverSrc *)m_pFilter)->GetImageHeight())
	{
		// If the image width/height is changed, fail CheckMediaType() to force
		// the renderer to resize the image.
		return E_INVALIDARG;
	}


	return S_OK;  // This format is acceptable.

}

HRESULT CTMReceiverVideoOutputPin::GetMediaType(int iPosition, CMediaType *pmt){
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
	lpBitmapInfoHeader->biWidth = ((CTMReceiverSrc *)m_pFilter)->GetImageWidth()/4*4;
	lpBitmapInfoHeader->biHeight = ((CTMReceiverSrc *)m_pFilter)->GetImageHeight();
	lpBitmapInfoHeader->biPlanes = 1;
	lpBitmapInfoHeader->biCompression = BI_RGB;
	lpBitmapInfoHeader->biSizeImage = ((CTMReceiverSrc *)m_pFilter)->GetImageWidth() / 4 * 4 * ((CTMReceiverSrc *)m_pFilter)->GetImageHeight() * 4;
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

HRESULT CTMReceiverVideoOutputPin::GetMediaType(CMediaType *pmt){
	VIDEOINFO* pvih = (VIDEOINFO*)pmt->AllocFormatBuffer(sizeof(VIDEOINFO));
	LPBITMAPINFOHEADER lpBitmapInfoHeader = &(pvih->bmiHeader);
	lpBitmapInfoHeader->biSize = sizeof(BITMAPINFOHEADER);
	lpBitmapInfoHeader->biBitCount = 32;
	lpBitmapInfoHeader->biWidth = ((CTMReceiverSrc *)m_pFilter)->GetImageWidth()/4*4;
	lpBitmapInfoHeader->biHeight = ((CTMReceiverSrc *)m_pFilter)->GetImageHeight();
	lpBitmapInfoHeader->biPlanes = 1;
	lpBitmapInfoHeader->biCompression = BI_RGB;
	lpBitmapInfoHeader->biSizeImage = ((CTMReceiverSrc *)m_pFilter)->GetImageWidth() / 4 * 4 * ((CTMReceiverSrc *)m_pFilter)->GetImageHeight() * 4;
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

STDMETHODIMP CTMReceiverVideoOutputPin::Run()
{
	m_rtFirstFrameTime = 0;
	return CSourceStream::Run();
}

STDMETHODIMP CTMReceiverVideoOutputPin::Stop()
{
	m_rtFirstFrameTime = 0;
	return CSourceStream::Stop();
}

HRESULT CTMReceiverVideoOutputPin::InitRecord(const char* fileName)
{
	avcodec_register_all();
	av_register_all();

	avformat_alloc_output_context2(&m_fileSaverCtx, NULL, NULL, fileName);
	if(!m_fileSaverCtx)
	{
		return E_FAIL;
	}
	m_fileSaverFmt = m_fileSaverCtx->oformat;
	m_fileSaverFmt->video_codec = CODEC_ID_H264;
	m_fileSaverStream = avformat_new_stream(m_fileSaverCtx, NULL);
	if(!m_fileSaverStream)
	{
		return E_FAIL;
	}
	AVCodecContext *outputCodecContext;
	outputCodecContext = m_fileSaverStream->codec;
	CTMReceiverSrc *pFilter = (CTMReceiverSrc *)m_pFilter;
	AVStream *pInputStream = pFilter->m_pFormatContext->streams[pFilter->m_videoStreamIndex];
	AVCodecContext *inputCodecContext = pFilter->m_pFormatContext->streams[pFilter->m_videoStreamIndex]->codec;

	outputCodecContext->codec_id = m_fileSaverFmt->video_codec;
	outputCodecContext->codec_type = inputCodecContext->codec_type;
	outputCodecContext->codec_tag = inputCodecContext->codec_tag;
	outputCodecContext->bit_rate = 400000;
	outputCodecContext->extradata = inputCodecContext->extradata;
	outputCodecContext->extradata_size = inputCodecContext->extradata_size;
	outputCodecContext->width = ((CTMReceiverSrc *)m_pFilter)->GetImageWidth();
	outputCodecContext->height = ((CTMReceiverSrc *)m_pFilter)->GetImageHeight();
	outputCodecContext->time_base.den = FPS;
	outputCodecContext->time_base.num = 1;
	outputCodecContext->gop_size = inputCodecContext->gop_size;
	outputCodecContext->pix_fmt = inputCodecContext->pix_fmt;
	outputCodecContext->has_b_frames = inputCodecContext->has_b_frames;


	//DIFF
	outputCodecContext->bit_rate = inputCodecContext->bit_rate;
	/*outputCodecContext->time_base = inputCodecContext->time_base;
	m_fileSaverStream->time_base = pInputStream->time_base;
	if(av_q2d(inputCodecContext->time_base) * inputCodecContext->ticks_per_frame > av_q2d(pInputStream->time_base) && av_q2d(pInputStream->time_base) < 1.0/1000) 
	{
		outputCodecContext->time_base = inputCodecContext->time_base;
		outputCodecContext->time_base.num *= inputCodecContext->ticks_per_frame;
	}
	av_reduce(&outputCodecContext->time_base.num, &outputCodecContext->time_base.den,outputCodecContext->time_base.num, outputCodecContext->time_base.den, INT_MAX);
*/
	if(m_fileSaverCtx->oformat->flags & AVFMT_GLOBALHEADER)
	{
		outputCodecContext->flags |= CODEC_FLAG_GLOBAL_HEADER;
	}
	m_fileSaverStream->codec = outputCodecContext;

	av_dump_format(m_fileSaverCtx, 0, fileName, 1);
	//Open Video
	int ret = -1;

	if(!(m_fileSaverFmt->flags & AVFMT_NOFILE))
	{
		if(avio_open(&m_fileSaverCtx->pb, fileName, AVIO_FLAG_WRITE)<0)
		{
			return E_FAIL;
		}
	}

	ret = avformat_write_header(m_fileSaverCtx, NULL);
	pts = 1;
	m_bRecordStatus = TRUE;
	m_bFindKeyFrame = FALSE;
	return S_OK;
}

HRESULT CTMReceiverVideoOutputPin::StopRecord()
{
	m_bRecordStatus = FALSE;
	int ret = -1;
	Sleep(100);
	ret = av_write_trailer(m_fileSaverCtx);
	ret = avcodec_close(m_fileSaverStream->codec);
	for(int i=0; i<m_fileSaverCtx->nb_streams; i++)
	{
		av_freep(&(m_fileSaverCtx->streams[i]->codec));
		av_freep(&(m_fileSaverCtx->streams[i]));
	}
	if(!(m_fileSaverFmt->flags & AVFMT_NOFILE))
	{
		ret = avio_close(m_fileSaverCtx->pb);
	}
	av_free(m_fileSaverCtx);
	pts = 1;
	return S_OK;
}

//////////////////////////////////////////////////////////////////
// Audio Output Pin
//////////////////////////////////////////////////////////////////

CTMReceiverAudioOutputPin::CTMReceiverAudioOutputPin(HRESULT *phr, CTMReceiverSrc *pParent, LPCWSTR pPinName)
	: CSourceStream(NAME("TMReceiver Source Audio Output Pin"), phr, pParent, pPinName)
{
	m_pFrame = avcodec_alloc_frame();
	m_rtSampleTime = 0;
	m_remainData = new BYTE[AVCODEC_MAX_AUDIO_FRAME_SIZE*3/2];
	m_remainDataSize = 0;
}

CTMReceiverAudioOutputPin::~CTMReceiverAudioOutputPin()
{
	av_free(m_pFrame);
}

STDMETHODIMP CTMReceiverAudioOutputPin::NonDelegatingQueryInterface(REFIID riid, void **ppv)
{
	return CSourceStream::NonDelegatingQueryInterface(riid, ppv);
}

HRESULT CTMReceiverAudioOutputPin::DecideBufferSize(IMemAllocator *pAlloc, ALLOCATOR_PROPERTIES *pProperties)
{
	CheckPointer(pAlloc,E_POINTER);
	CheckPointer(pProperties,E_POINTER);
	CAutoLock cAutoLock(m_pFilter->pStateLock());
	HRESULT hr = NOERROR;
	WAVEFORMATEX *pwi = (WAVEFORMATEX *) m_mt.Format();
	pProperties->cBuffers = 1;
	pProperties->cbBuffer = pwi->nSamplesPerSec;
	pProperties->cbAlign = pwi->nBlockAlign;
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
	return NOERROR;
}

HRESULT CTMReceiverAudioOutputPin::CheckMediaType(const CMediaType *pMediaType)
{
	CheckPointer(pMediaType, E_POINTER);
	if((*(pMediaType->Type())) != MEDIATYPE_Audio || !(pMediaType->IsFixedSize()))
	{
		return E_INVALIDARG;
	}
	const GUID *SubType = pMediaType->Subtype();
	if(SubType == NULL)
		return E_INVALIDARG;
	if(*SubType != MEDIASUBTYPE_PCM)
	{
		return E_INVALIDARG;
	}

	WAVEFORMATEX *pwi = (WAVEFORMATEX *)pMediaType->Format();
	int wBitPerSampleFromFFmpeg = 16;
	switch(m_pAudioCodecCtx->sample_fmt)
	{
	case AV_SAMPLE_FMT_U8:
		wBitPerSampleFromFFmpeg = 8; break;
	case AV_SAMPLE_FMT_S32:
		wBitPerSampleFromFFmpeg = 32; break;
	default:
		wBitPerSampleFromFFmpeg = 16;
	}

	if((pwi->nChannels != m_pAudioCodecCtx->channels) || 
		(pwi->nSamplesPerSec != m_pAudioCodecCtx->sample_rate) ||
		(pwi->wBitsPerSample != wBitPerSampleFromFFmpeg))
	{
		return E_INVALIDARG;
	}

	return S_OK;
}

HRESULT CTMReceiverAudioOutputPin::GetMediaType(int iPosition, CMediaType *pMediaType)
{
	if   (iPosition   <   0   ||   iPosition   >=   25)
	{
		return   E_FAIL;
	}

	pMediaType-> SetType(&MEDIATYPE_Audio);
	pMediaType-> SetSubtype(&MEDIASUBTYPE_PCM);
	pMediaType-> SetFormatType(&FORMAT_WaveFormatEx);

	WAVEFORMATEX       format;
	format.cbSize                     =   0;
	format.wFormatTag             =   WAVE_FORMAT_PCM;
	switch   (iPosition)
	{
	case   0:
		format.nSamplesPerSec     =   44100;   //   44.1k,   stereo,   16-bit
		format.nChannels               =   2;
		format.wBitsPerSample     =   16;
		break;
	case   1:
		format.nSamplesPerSec     =   44100;
		format.nChannels               =   1;
		format.wBitsPerSample     =   16;
		break;
	case   2:
		format.nSamplesPerSec     =   44100;
		format.nChannels               =   2;
		format.wBitsPerSample     =   8;
		break;
	case   3:
		format.nSamplesPerSec     =   44100;
		format.nChannels               =   1;
		format.wBitsPerSample     =   8;
		break;
	case   4:
		format.nSamplesPerSec     =   22050;   //   22.050k,   stereo,   16-bit
		format.nChannels               =   2;
		format.wBitsPerSample     =   16;
		break;
	case   5:
		format.nSamplesPerSec     =   22050;
		format.nChannels               =   1;
		format.wBitsPerSample     =   16;
		break;
	case   6:
		format.nSamplesPerSec     =   22050;
		format.nChannels               =   2;
		format.wBitsPerSample     =   8;
		break;
	case   7:
		format.nSamplesPerSec     =   22050;
		format.nChannels               =   1;
		format.wBitsPerSample     =   8;
		break;
	case   8:
		format.nSamplesPerSec     =   11025;   //   11.025k,   stereo,   16-bit
		format.nChannels               =   2;
		format.wBitsPerSample     =   16;
		break;
	case   9:
		format.nSamplesPerSec     =   11025;
		format.nChannels               =   1;
		format.wBitsPerSample     =   16;
		break;
	case   10:
		format.nSamplesPerSec     =   11025;
		format.nChannels               =   2;
		format.wBitsPerSample     =   8;
		break;
	case   11:
		format.nSamplesPerSec     =   11025;
		format.nChannels               =   1;
		format.wBitsPerSample     =   8;
		break;
	case   12:
		format.nSamplesPerSec     =   48000;   //   48k,   stereo,   16-bit
		format.nChannels               =   2;
		format.wBitsPerSample     =   16;
		break;
	case   13:
		format.nSamplesPerSec     =   48000;
		format.nChannels               =   1;
		format.wBitsPerSample     =   16;
		break;
	case   14:
		format.nSamplesPerSec     =   48000;
		format.nChannels               =   2;
		format.wBitsPerSample     =   8;
		break;
	case   15:
		format.nSamplesPerSec     =   48000;
		format.nChannels               =   1;
		format.wBitsPerSample     =   8;
		break;
	case   16:
		format.nSamplesPerSec     =   32000;   //   32k,   stereo,   16-bit
		format.nChannels               =   2;
		format.wBitsPerSample     =   16;
		break;
	case   17:
		format.nSamplesPerSec     =   32000;
		format.nChannels               =   1;
		format.wBitsPerSample     =   16;
		break;
	case   18:
		format.nSamplesPerSec     =   32000;
		format.nChannels               =   2;
		format.wBitsPerSample     =   8;
		break;
	case   19:
		format.nSamplesPerSec     =   32000;
		format.nChannels               =   1;
		format.wBitsPerSample     =   8;
		break;
	case   20:
		format.nSamplesPerSec     =   8000;   //   8k,   stereo,   16-bit
		format.nChannels               =   2;
		format.wBitsPerSample     =   16;
		break;
	case   21:
		format.nSamplesPerSec     =   8000;
		format.nChannels               =   1;
		format.wBitsPerSample     =   16;
		break;
	case   22:
		format.nSamplesPerSec     =   8000;
		format.nChannels               =   2;
		format.wBitsPerSample     =   8;
		break;
	case   23:
		format.nSamplesPerSec     =   8000;
		format.nChannels               =   1;
		format.wBitsPerSample     =   8;
		break;
	case 24:
		format.nSamplesPerSec     =   16000;     // for rtmp streams
		format.nChannels          =   2;
		format.wBitsPerSample     =   16;
	}

	format.nBlockAlign           =   format.nChannels   *   format.wBitsPerSample   /   8;
	format.nAvgBytesPerSec   =   format.nSamplesPerSec   *   format.nBlockAlign;
	pMediaType-> SetFormat(PBYTE(&format),   sizeof(WAVEFORMATEX));
	return   NOERROR; 
}

HRESULT CTMReceiverAudioOutputPin::GetMediaType(CMediaType *pmt)
{
	CheckPointer(pmt,E_POINTER);
	CAutoLock cAutoLock(m_pFilter->pStateLock());
	WAVEFORMATEX *pwi = (WAVEFORMATEX *) pmt->AllocFormatBuffer(sizeof(WAVEFORMATEX));
	if(NULL == pwi)
		return(E_OUTOFMEMORY);
	ZeroMemory(pwi, sizeof(WAVEFORMATEX));
	int nFrequency = 48000;
	int nChannels = 2;
	int nBytesPerSample = 2;
	long lBytesPerSecond = (long) (nBytesPerSample * nFrequency * nChannels);
	long lBufferSize = (long) ((float) lBytesPerSecond);
	pwi->wFormatTag = WAVE_FORMAT_PCM;
	pwi->nChannels = (unsigned short)nChannels;
	pwi->nSamplesPerSec = nFrequency;
	pwi->nAvgBytesPerSec = lBytesPerSecond;
	pwi->wBitsPerSample = (WORD) (nBytesPerSample * 8);
	pwi->nBlockAlign = (WORD) (nBytesPerSample * nChannels);
	pmt->SetType(&MEDIATYPE_Audio);
	pmt->SetFormatType(&WMFORMAT_WaveFormatEx);
	pmt->SetTemporalCompression(FALSE);
	pmt->SetSubtype(&MEDIASUBTYPE_PCM);
	pmt->SetSampleSize(lBufferSize);
	return NOERROR;
}

HRESULT CTMReceiverAudioOutputPin::FillBuffer(IMediaSample *pms)
{
	CheckPointer(pms, E_POINTER);
	AVPacket pkt, nextPkt;
	BYTE *pData, *pActualPMSData, *pDataOrigin;
	REFERENCE_TIME rtStart, rtStop, rtMediaStart,rtMediaStop;
	bool setStart = false;
	int lDataLen = pms->GetSize();
	pData = new BYTE[lDataLen];
	pDataOrigin = pData;

	BYTE *outBuffer = new BYTE[AVCODEC_MAX_AUDIO_FRAME_SIZE*3/2];
	int outBufferSize = AVCODEC_MAX_AUDIO_FRAME_SIZE*3/2;
	int curDataLength = 0;

	if(m_remainDataSize > 0)
	{
		memcpy(pData, m_remainData, m_remainDataSize);
		pData += m_remainDataSize;
		curDataLength += m_remainDataSize;
		m_remainDataSize = 0;
	}

	while(curDataLength < lDataLen)
	{
		av_init_packet(&pkt);
		int pktQueueRet = m_queueBuffer.Get(&pkt, 1);
		if(pktQueueRet == 0)
		{
			continue;
		}
		if(pktQueueRet < 0)
		{
			return S_FALSE;
		}
		if(pkt.data == NULL)
		{
			continue;
		}

		if(!setStart)
		{
			rtStart = pkt.pts * m_rtAvgTimePerPts;
			setStart = true;
		}
		outBufferSize = AVCODEC_MAX_AUDIO_FRAME_SIZE*3/2;
		ZeroMemory(outBuffer, outBufferSize);
		{
			int len = avcodec_decode_audio3(m_pAudioCodecCtx, (int16_t *)outBuffer, &outBufferSize, &pkt);
			if(len <= 0 || outBufferSize <= 0 || outBufferSize > lDataLen)
			{
				continue;
			}
			if(curDataLength + outBufferSize <= lDataLen)
			{
				curDataLength += outBufferSize;
				memcpy(pData, outBuffer, outBufferSize);
				pData += outBufferSize;
			}
			else
			{
				memcpy(pData, outBuffer, lDataLen-curDataLength);
				outBuffer += (lDataLen - curDataLength);
				ZeroMemory(m_remainData, AVCODEC_MAX_AUDIO_FRAME_SIZE*3/2);
				m_remainDataSize = outBufferSize - (lDataLen - curDataLength);
				memcpy(m_remainData, outBuffer, m_remainDataSize);
				curDataLength = lDataLen;
			}
		}
	}

	pms->GetPointer(&pActualPMSData);
	ZeroMemory(pActualPMSData, lDataLen);
	{
		CAutoLock cAutoLockShared(&m_cSharedState);
		memcpy(pActualPMSData, pDataOrigin, lDataLen);
	}

	while(true)
	{
		av_init_packet(&nextPkt);
		int pktQueueRet = m_queueBuffer.QueryFirst(&nextPkt, 1);
		if(pktQueueRet == 0)
		{
			continue;
		}
		if(pktQueueRet < 0)
		{
			return S_FALSE;
		}
		if(pkt.data == NULL)
		{
			return S_OK;
		}
		break;
	}

	rtStop = nextPkt.pts * m_rtAvgTimePerPts;
	HRESULT hr = pms->SetTime(&rtStart, &rtStop);

	hr = pms->SetSyncPoint(TRUE);
	// TODO:Discontinuity?
	hr = pms->SetPreroll(FALSE);
	hr = pms->SetActualDataLength(pms->GetSize());
	return NOERROR;
}


HRESULT InitRecord(const char* fileName)
{
	return S_OK;
}
HRESULT StopRecord()
{
	return S_OK;
}


//////////////////////////////////////////////////////////////////
// H264 Video Decoder
//////////////////////////////////////////////////////////////////

H264Decoder::H264Decoder(void)
{
	m_bInitialized = FALSE;

}


H264Decoder::~H264Decoder(void)
{
}

int H264Decoder::Init(AVCodecContext *pCtx){
	av_register_all();
	avcodec_register_all();
	m_pCodec = avcodec_find_decoder(CODEC_ID_H264); 
	m_pCodecCtx = avcodec_alloc_context3(m_pCodec);
	(*m_pCodecCtx) = (*pCtx);
	if (avcodec_open2(m_pCodecCtx, m_pCodec,NULL) < 0) { 
		return 0; 
	}
	m_pFrame = avcodec_alloc_frame();
	m_pImgConvertCtx = NULL;
	m_bInitialized = TRUE;
	return 0;
}
void H264Decoder::Release(){
	if(m_pCodecCtx) { 
		avcodec_close(m_pCodecCtx); 
		av_free(m_pCodecCtx); 
		m_pCodecCtx = NULL; 
	} 
	if(m_pFrame) { 
		av_free(m_pFrame); 
		m_pFrame = NULL; 
	}
	if (m_pImgConvertCtx)
	{
		sws_freeContext(m_pImgConvertCtx);
		m_pImgConvertCtx=NULL;
	}
}
int H264Decoder::DecodeFrame(AVPicture *pic, unsigned char* img, unsigned char* data, unsigned long size){
	if(!m_bInitialized)
	{
		return 0;
	}
	AVPacket pkt;
	int got_picture;
	pkt.data = data;
	pkt.size = size;
	int consumed=-1;
	consumed = avcodec_decode_video2(m_pCodecCtx,m_pFrame,&got_picture,&pkt);
	if(!got_picture){
		//Release();
		//Init();
		return 0;
	}
	//if(m_pCodecCtx->width!=720||m_pCodecCtx->height!=576){
	//	return 0;
	//}
	if(m_pImgConvertCtx==NULL){
		m_pImgConvertCtx = sws_getContext(m_pCodecCtx->width, m_pCodecCtx->height, m_pCodecCtx->pix_fmt, m_pCodecCtx->width, m_pCodecCtx->height, PIX_FMT_RGB32, SWS_BICUBIC, NULL, NULL, NULL);
	}
	avpicture_fill(pic, img, PIX_FMT_RGB32, m_pCodecCtx->width, m_pCodecCtx->height);

	m_pFrame->data[0] += m_pFrame->linesize[0] * (m_pCodecCtx->height - 1); 
	m_pFrame->linesize[0] *= -1; 
	m_pFrame->data[1] += m_pFrame->linesize[1] * (m_pCodecCtx->height / 2 - 1); 
	m_pFrame->linesize[1] *= -1; 
	m_pFrame->data[2] += m_pFrame->linesize[2] * (m_pCodecCtx->height / 2 - 1); 
	m_pFrame->linesize[2] *= -1; 
	sws_scale(m_pImgConvertCtx, m_pFrame->data, m_pFrame->linesize, 0, m_pCodecCtx->height, pic->data, pic->linesize); 

	return consumed;
}


DWORD ReaderProc(LPVOID pParam)
{
	//USES_CONVERSION;
	CTMReceiverSrc *pReceiverSrc = (CTMReceiverSrc *)pParam;
	pReceiverSrc->ReadAndCachePreviewPackets();
	return 0;
}