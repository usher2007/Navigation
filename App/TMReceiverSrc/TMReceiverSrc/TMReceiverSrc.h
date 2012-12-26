#pragma once

#include <streams.h>
#include <list>
#include <rpcndr.h>
#include "PacketQueue.h"
#include "..\TMReceiver\TMReceiver.h"
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

// {7C85656E-D45D-4C6B-A825-4DF103639DD2}
DEFINE_GUID(CLSID_TMReceiverSrc, 
	0x7c85656e, 0xd45d, 0x4c6b, 0xa8, 0x25, 0x4d, 0xf1, 0x3, 0x63, 0x9d, 0xd2);

// {1E3C41C2-3872-44CB-83E4-3D14EE823688}
DEFINE_GUID(IID_IRecordStream, 
	0x1e3c41c2, 0x3872, 0x44cb, 0x83, 0xe4, 0x3d, 0x14, 0xee, 0x82, 0x36, 0x88);
DECLARE_INTERFACE_(IRecordStream, IUnknown)
{
	STDMETHOD(StartRecord)(THIS_ const char* fileName)PURE;
	STDMETHOD(StopRecord)(THIS_)PURE;
};

// {E74BC5A9-44AE-4AC4-8B26-6FE694940EA5}
DEFINE_GUID(IID_ISetCallBack, 
	0xe74bc5a9, 0x44ae, 0x4ac4, 0x8b, 0x26, 0x6f, 0xe6, 0x94, 0x94, 0xe, 0xa5);
DECLARE_INTERFACE_(ISetCallBack, IUnknown)
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
class CTMReceiverSrc : public CSource, public IFileSourceFilter, public IRecordStream, public ISetCallBack
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
		else if(riid == IID_IRecordStream)
		{
			return GetInterface((IRecordStream *)this, ppv);
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
	CTMReceiverOutputPin(HRESULT *phr, CTMReceiverSrc *pParent, LPCTSTR pPinName);
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

	int Init();
	void Release();
	int DecodeFrame(AVPicture *pic, unsigned char* img, unsigned char* data, unsigned long size);

};