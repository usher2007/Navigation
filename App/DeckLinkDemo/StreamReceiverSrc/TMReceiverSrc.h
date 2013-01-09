#pragma once

#include <streams.h>
#include <list>
#include <rpcndr.h>
#include "PacketQueue.h"
#include "TMReceiver.h"
extern "C" {
#include "libavutil/avstring.h"
#include "libavutil/mathematics.h"
#include "libavutil/pixdesc.h"
#include "libavutil/imgutils.h"
#include "libavutil/dict.h"
#include "libavutil/parseutils.h"
#include "libavutil/samplefmt.h"
#include "libavutil/avassert.h"
#include "libavformat/avformat.h"
#include "libavdevice/avdevice.h"
#include "libswscale/swscale.h"
#include "libavutil/opt.h"
#include "libavcodec/avfft.h"
#include "libavcodec/avcodec.h"
};

// {6F568873-134B-4FF3-B0B2-D886D859A0C7}
DEFINE_GUID(CLSID_TMStreamReceiver, 
	0x6f568873, 0x134b, 0x4ff3, 0xb0, 0xb2, 0xd8, 0x86, 0xd8, 0x59, 0xa0, 0xc7);

// {24D64F98-1D26-4D8D-88FA-FF996215B864}
DEFINE_GUID(IID_IRecordTMStream, 
	0x24d64f98, 0x1d26, 0x4d8d, 0x88, 0xfa, 0xff, 0x99, 0x62, 0x15, 0xb8, 0x64);
DECLARE_INTERFACE_(IRecordTMStream, IUnknown)
{
	STDMETHOD(StartRecord)(THIS_ const char* fileName)PURE;
	STDMETHOD(StopRecord)(THIS_)PURE;
};

// {C456AF23-EF0E-4A49-A140-6AAC009975AF}
DEFINE_GUID(IID_ISetTMCallBack, 
	0xc456af23, 0xef0e, 0x4a49, 0xa1, 0x40, 0x6a, 0xac, 0x0, 0x99, 0x75, 0xaf);

DECLARE_INTERFACE_(ISetTMCallBack, IUnknown)
{
	STDMETHOD(SetCallBackBeforeDecode(TMReceiverCB cb, void* arg))PURE;
	STDMETHOD(SetCallBackAfterDecode(TMReceiverCB cb, void* arg))PURE;
};


extern const AMOVIESETUP_FILTER sudTMReceiverSrcax;

class CTMReceiverOutputPin;
struct Resolution_t
{
	int width;
	int height;
};
class CTMReceiverSrc : public CSource, public IFileSourceFilter, public IRecordTMStream, public ISetTMCallBack
{
	friend class CTMReceiverOutputPin;
public:
	~CTMReceiverSrc();
	static CUnknown * WINAPI CreateInstance(LPUNKNOWN lpunk, HRESULT *phr);

	DECLARE_IUNKNOWN;
	STDMETHODIMP NonDelegatingQueryInterface(REFIID riid, void **ppv)
	{
		if (riid == IID_IFileSourceFilter) 
		{
			return GetInterface((IFileSourceFilter *)this, ppv);
		}
		else if(riid == IID_IRecordTMStream)
		{
			return GetInterface((IRecordTMStream *)this, ppv);
		}
		else if(riid == IID_ISetTMCallBack)
		{
			return GetInterface((ISetTMCallBack *)this, ppv);
		}
		else
		{
			return CSource::NonDelegatingQueryInterface(riid, ppv);
		}
	}

	STDMETHODIMP Load(LPCOLESTR pszFileName, const AM_MEDIA_TYPE *pmt);
	STDMETHODIMP GetCurFile(LPOLESTR * ppszFileName,AM_MEDIA_TYPE * pmt);
	STDMETHODIMP Pause();
	STDMETHODIMP Stop();
	STDMETHODIMP Run(REFERENCE_TIME tStart);

	// IRecordStream functions
	STDMETHODIMP StartRecord(const char *fileName);
	STDMETHODIMP StopRecord();

	// ISetCallBack functions
	STDMETHODIMP SetCallBackBeforeDecode(TMReceiverCB cb, void* arg);
	STDMETHODIMP SetCallBackAfterDecode(TMReceiverCB cb, void* arg);

	int CallBeforeDecodeCB(TMFrame *pFrame);
	int CallAfterDecodeCB(TMFrame *pFrame);

	CTMReceiverOutputPin *m_pVideoPin;

private:
	CTMReceiverSrc(LPUNKNOWN lpunk, HRESULT *phr);
	LPWSTR      m_pFileName;
	PacketQueue m_queueBuffer;
	AVFormatContext *m_pFormatContext;
	int m_videoStreamIndex;
	struct Resolution_t m_resolution;
	HANDLE m_readerThread;
	BOOL m_bRecordStatus;
	char m_recordFileName[1024];

	TMReceiverCB beforeDecodeCB;
	TMReceiverCB afterDecodeCB;
	void *beforeCBParam;
	void *afterCBParam;

public:
	void ReadAndCachePreviewPackets();
	int GetImageWidth() { return m_resolution.width > 0 ? m_resolution.width : -1;}
	int GetImageHeight() { return m_resolution.height > 0 ? m_resolution.height : -1;}
};
class H264Decoder;
class CTMReceiverOutputPin : public CSourceStream
{
public:
	CTMReceiverOutputPin(HRESULT *phr, CTMReceiverSrc *pParent, LPCWSTR pPinName);
	~CTMReceiverOutputPin();

	STDMETHODIMP NonDelegatingQueryInterface(REFIID riid, void **ppv);
	HRESULT CheckMediaType(const CMediaType *pMediaType);
	HRESULT GetMediaType(int iPosition, CMediaType *pmt);
	HRESULT FillBuffer(IMediaSample *pms);
	HRESULT DecideBufferSize(IMemAllocator *pIMemAlloc, ALLOCATOR_PROPERTIES *pProperties);
	HRESULT GetMediaType(CMediaType *pmt);

	STDMETHODIMP Run();
	STDMETHODIMP Stop();
	STDMETHODIMP Notify(IBaseFilter * pSender, Quality q);

	HRESULT InitRecord(const char* fileName);
	HRESULT StopRecord();


	CRefTime					m_rtPosition;
	CRefTime					m_rtSampleTime;
	CRefTime					m_rtAvgTimePerFrame;
	CRefTime                    m_rtFirstFrameTime;
	BOOL						m_bWorking;
	BOOL                        m_bGetAvgFrameTime;
	BOOL                        m_bFPSGuessed;

	CCritSec m_csDecoder;
	CCritSec m_csBuffer;
	H264Decoder *m_pDecoder;
	BYTE *m_pData;
	BOOL m_pinIntialized;

	AVFormatContext *m_fileSaverCtx;
	AVOutputFormat *m_fileSaverFmt;
	AVStream *m_fileSaverStream;
	BOOL m_bRecordStatus;
	LONGLONG pts;
	BOOL m_bFindKeyFrame;

private:
	CCritSec m_cSharedState;            // Lock on m_rtSampleTime and m_Ball
	void UpdateFromSeek();
};

class H264Decoder
{
public:
	H264Decoder(void);
	~H264Decoder(void);


	AVCodec* m_pCodec;
	AVCodecContext* m_pCodecCtx; 
	AVFrame* m_pFrame;
	struct SwsContext *m_pImgConvertCtx;

	int Init(AVCodecContext *pCtx);
	void Release();
	int DecodeFrame(AVPicture *pic, unsigned char* img, unsigned char* data, unsigned long size);

private:
	BOOL m_bInitialized;

};