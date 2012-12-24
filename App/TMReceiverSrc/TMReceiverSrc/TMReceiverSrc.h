#pragma once

#include <streams.h>
#include <list>
#include <rpcndr.h>
#include "H264Dec.h"
#include "PacketQueue.h"
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


extern const AMOVIESETUP_FILTER sudTMReceiverSrcax;

class CTMReceiverOutputPin;
struct Resolution_t
{
	int width;
	int height;
};
class CTMReceiverSrc : public CSource, public IFileSourceFilter
{
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

	CTMReceiverOutputPin *m_pVideoPin;

private:
	CTMReceiverSrc(LPUNKNOWN lpunk, HRESULT *phr);
	LPWSTR      m_pFileName;
	PacketQueue m_queueBuffer;
	AVFormatContext *m_pFormatContext;
	int m_videoStreamIndex;
	Resolution_t m_resolution;
	HANDLE m_readerThread;

private:
	void ReadAndCachePreviewPackets();
};

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

	CRefTime					m_rtPosition;
	CRefTime					m_rtSampleTime;
	CRefTime					m_rtAvgTimePerFrame;
	CRefTime                    m_rtFirstFrameTime;
	BOOL						m_bWorking;
	BOOL                        m_bGetAvgFrameTime;

	CCritSec m_csDecoder;
	CCritSec m_csBuffer;
	H264Dec_Handle hDecoder;
	BYTE *m_pData;
	struct Resolution_t resolution;
	BOOL m_pinIntialized;

private:
	CCritSec m_cSharedState;            // Lock on m_rtSampleTime and m_Ball
	void UpdateFromSeek();
};