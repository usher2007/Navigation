#include "stdafx.h"
#include <streams.h>
#include "TMReceiverSrc.h"

static const LONGLONG ONESEC = 10000000;
static const int FPS = 25;
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
	m_bRecordStatus = FALSE;
	memset(m_recordFileName, 0x00, sizeof(m_recordFileName));
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
	AVPacket packet;
	AVPacket packetToAbolish;
	while(TRUE)
	{
		if(m_queueBuffer.nb_packets > 256)
		{
			for(int i=0; i<10; i++)
			{
				av_init_packet(&packetToAbolish);
				m_queueBuffer.Get(&packetToAbolish, 1);
				//m_channelPts[channel] = packetToAbolish.pts;
				av_free_packet(&packetToAbolish);
			}
		}
		int ret = av_read_frame(m_pFormatContext, &packet);
		if(ret < 0)
		{
			av_init_packet(&packet);
			packet.data = NULL;
			packet.size = 0;
			packet.stream_index = m_videoStreamIndex;
			m_queueBuffer.Put(&packet);
			//For debug
			/*char tmp[1024];
			sprintf(tmp,"!!!!!!!!!!!!!!!Channel: %d __Put %d bad Packet!\n",channel, ret);
			OutputDebugStringA(tmp);*/
			Sleep(10);
			continue;
		}
		if(packet.stream_index == m_videoStreamIndex)
		{
			//For debug
			/*char tmp[1024];
			sprintf(tmp,"Channel: %d __Put %d good Packet!DTS:%lld\n",channel, m_previewQueueBuffer[channel].nb_packets,packet.dts);
			OutputDebugStringA(tmp);*/
			//m_channelPts[channel] = packet.pts;
			m_queueBuffer.Put(&packet);
			//char tmp[1024];
			//sprintf(tmp, "Channel %d__PTS %lld\n", channel, packet.pts);
			//OutputDebugStringA(tmp);
			continue;
		}
		Sleep(10);
	}
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
	CTMReceiverSrc* pFilter = (CTMReceiverSrc*)m_pFilter;
	/*char tmp[1024];
	sprintf(tmp,"Channel:%d, Fill Buffer In!\n", pFilter->m_relatedChannel);
	OutputDebugStringA(tmp);*/
	AVPacket pkt, pktForRecord;
	BYTE *pData;
	long lDataLen;
	lDataLen = pms->GetSize();
	if (m_pData==NULL)
	{
		m_pData = new BYTE[lDataLen];
	}
	if(pFilter->m_queueBuffer.nb_packets<=0)
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
		//Sleep(100);
		//char tmp[1024];
		//sprintf(tmp,"====================No Data!====================\n");
		//OutputDebugStringA(tmp);
		return S_OK;
	}
	av_init_packet(&pkt);
	int maxPktNum = m_bGetAvgFrameTime ? 10 : 5;
	while (pFilter->m_queueBuffer.nb_packets > maxPktNum)
	{
		CAutoLock lock(&m_csBuffer);
		pFilter->m_queueBuffer.Get(&pkt,1);
		av_free_packet(&pkt);
	}

	{
		CAutoLock lock(&m_csBuffer);
		pFilter->m_queueBuffer.Get(&pkt, 1);
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
		pts++;
	}
	ret = -1;
	{
		CAutoLock lock(&m_csDecoder);
		if (hDecoder!=NULL)
		{
			ret = H264Dec_DecodeFrame(hDecoder,m_pData,pkt.data,pkt.size);			
		}		
	}

	if(ret <=0)
	{
		/*char tmp[1024];
		sprintf(tmp," ===================================BAD£¬rtSampleTime:%lld\n",m_rtSampleTime);
		OutputDebugStringA(tmp);*/
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
		rtStart = (pkt.pts  - m_rtFirstFrameTime)*100/9*10 - 1000;
		if(rtStart > 0 && !m_bGetAvgFrameTime)
		{
			m_rtAvgTimePerFrame = rtStart - 0;
			m_bGetAvgFrameTime = TRUE;
		}
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
frame_count ++;
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

HRESULT CTMReceiverOutputPin::InitRecord(const char* fileName)
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
	AVCodecContext *c;
	AVCodec *codec;
	c = m_fileSaverStream->codec;
	codec = avcodec_find_encoder(m_fileSaverFmt->video_codec);
	if(!codec)
	{
		return E_FAIL;
	}
	avcodec_get_context_defaults3(c, codec);
	c->codec_id = m_fileSaverFmt->video_codec;
	c->bit_rate = 400000;
	c->time_base.den = FPS;
	c->time_base.num = 1;
	c->gop_size = 12;
	c->pix_fmt = PIX_FMT_YUV420P;

	if(m_fileSaverCtx->oformat->flags & AVFMT_GLOBALHEADER)
	{
		c->flags |= CODEC_FLAG_GLOBAL_HEADER;
	}

	av_dump_format(m_fileSaverCtx, 0, fileName, 1);
	//Open Video
	int ret = avcodec_open2(c, codec, NULL);
	if(ret < 0)
	{
		return E_FAIL;
	}

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
}

HRESULT CTMReceiverOutputPin::StopRecord()
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

DWORD ReaderProc(LPVOID pParam)
{
	//USES_CONVERSION;
	CTMReceiverSrc *pReceiverSrc = (CTMReceiverSrc *)pParam;
	pReceiverSrc->ReadAndCachePreviewPackets();
	return 0;
}