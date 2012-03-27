#include "StdAfx.h"
#include <initguid.h>
#include "DXGraph.h"
#include "afxdialogex.h"
#include <afxwin.h>


CDXGraph::CDXGraph(void)
{
	pGraph = NULL;
	pControl = NULL;
	pEvent = NULL;
	pSeeking = NULL;
	mVideoWindow = NULL;
	pBasicVideo = NULL;
	pVideoFrameStep = NULL;
}


CDXGraph::~CDXGraph(void)
{
	pGrabber->Release();
	pVideoGrabber->Release();
	pVideoFrameStep->Release();
	mVideoWindow->Release();
	pControl->Release();
	pBasicVideo->Release();
	pSeeking->Release();
	pEvent->Release();
	//pGraph->Release();
	//pGraph = NULL;
}

HRESULT CDXGraph::Create(HWND hwnd)
{
	owner = hwnd;
	bFullScreen = FALSE;
	pGraph = NULL;
	HRESULT hr = CoCreateInstance(CLSID_FilterGraph, NULL, CLSCTX_INPROC_SERVER, IID_IGraphBuilder, (void **)&pGraph);
	if(FAILED(hr))
	{
		return hr;
	}
	pGrabber = NULL;
	grabCount = 0;
	pControl = NULL;
	hr = pGraph->QueryInterface(IID_IMediaControl, (void **)&pControl);
	if(FAILED(hr))
	{
		return hr;
	}
	pEvent = NULL;
	hr = pGraph->QueryInterface(IID_IMediaEventEx, (void **)&pEvent);
	if(FAILED(hr))
	{
		return hr;
	}
	pSeeking = NULL;
	hr = pGraph->QueryInterface(IID_IMediaSeeking,(void **)&pSeeking);
	if(FAILED(hr))
	{
		return hr;
	}
	//don't need to set time format to frame, just use the default 100ns.
	//pSeeking->SetTimeFormat(&TIME_FORMAT_FRAME);

	mVideoWindow = NULL;
	hr = pGraph->QueryInterface(IID_IVideoWindow, (void **)&mVideoWindow);
	if (FAILED(hr)) return hr;

	pBasicVideo = NULL;
	aveTimePerFrame = 0.;
	duration = 0.;
	hr = pGraph->QueryInterface(IID_IBasicVideo, (void **)&pBasicVideo);
	if (FAILED(hr)) return hr;

	pVideoFrameStep = NULL;
	hr = pGraph->QueryInterface(IID_IVideoFrameStep, (void **)&pVideoFrameStep);
	return hr;
}

HRESULT CDXGraph::RenderFile(CString mSourceFile)
{
	if(pGraph != NULL)
	{
		//pGraph->RenderFile(mSourceFile, NULL);
		//add grabber filter
		IBaseFilter *pGrabberF = NULL;
		this->AddGrabber(&pGrabberF);
		//add source filter
		IBaseFilter *pSrc;
		HRESULT hr = pGraph->AddSourceFilter(mSourceFile, L"Source", &pSrc);
		if(FAILED(hr)) return hr;
		/*IBaseFilter *pSrc;
		HRESULT hr = CoCreateInstance(CLSID_FFSrcFilter, NULL, CLSCTX_INPROC_SERVER, IID_IBaseFilter, (void **)&pSrc);
		if (FAILED(hr)) return hr;
		hr = pGraph->AddFilter(pSrc, L"SrcFilter");
		if (FAILED(hr)) return hr;*/
		hr = pSrc->QueryInterface(IID_INetDvr, (void **)&pNetDvr);
		if (FAILED(hr)) return hr;
		

		//add renderer filter
		IBaseFilter *pRender;
		hr = CoCreateInstance(CLSID_VideoRenderer, NULL, CLSCTX_INPROC_SERVER, IID_IBaseFilter, (void **)&pRender);
		if (FAILED(hr)) return hr;
		hr = pGraph->AddFilter(pRender, L"Renderer");
		if (FAILED(hr)) return hr;

		
		hr = ConnectFilters(pGraph, pSrc, pGrabberF, MEDIATYPE_NULL);
		hr = ConnectFilters(pGraph, pGrabberF, pRender, MEDIATYPE_NULL);
		pGrabberF->Release();
		pSrc->Release();
		pRender->Release();
		//test the graph
		SaveGraphFile(pGraph, L"D:\\SliderPlayer_264.grf");
		return hr;
	}
	return E_FAIL;
}

HRESULT CDXGraph::RenderAVIFile(CString mSourceFile)
{
	if(pGraph != NULL)
	{
		//Add Grabber Filter
		IBaseFilter *pGrabberF = NULL;
		this->AddGrabber(&pGrabberF);
		//Add src filter
		IBaseFilter *pSrc;
		HRESULT hr = pGraph->AddSourceFilter(mSourceFile, L"Source", &pSrc);
		if(FAILED(hr)) return hr;
		//Add Avi splitter filter
		IBaseFilter *pAviSplitter = NULL;
		hr = CoCreateInstance(CLSID_AviSplitter, NULL, CLSCTX_INPROC_SERVER, IID_IBaseFilter, (void **)&pAviSplitter);
		hr = pGraph->AddFilter(pAviSplitter, L"AVI Splitter");
		//Add Mpeg4s decoder dmo filter
		IBaseFilter *pMp4Decoder = NULL;
		hr = CoCreateInstance(CLSID_MPEG4SDecoder, NULL, CLSCTX_INPROC_SERVER, IID_IBaseFilter, (void **)&pMp4Decoder);
		hr = pGraph->AddFilter(pMp4Decoder, L"Mp4 Decoder");

		//Audio
		//add mp3 decoder
		IBaseFilter *pMP3Decoder = NULL;
		hr = CoCreateInstance(CLSID_MP3Decoder, NULL, CLSCTX_INPROC_SERVER, IID_IBaseFilter, (void **)&pMP3Decoder);
		hr = pGraph->AddFilter(pMP3Decoder, L"MP3 Decoder");

		//add direct sound device
		IBaseFilter *pDSoundDevice = NULL;
		hr = CoCreateInstance(CLSID_DSoundRender,NULL, CLSCTX_INPROC_SERVER, IID_IBaseFilter, (void **)&pDSoundDevice);
		hr = pGraph->AddFilter(pDSoundDevice, L"Sound Renderer");

		//connect filters via pins
		hr = ConnectFilters(pGraph, pSrc, pAviSplitter, MEDIATYPE_NULL);
		//Split to audio and video
		IEnumPins *pEnum = 0;
		IPin *pPin = 0;
		IEnumMediaTypes *pEnumMediaType = 0;
		hr = pAviSplitter->EnumPins(&pEnum);
		bool audio_connected = false, video_connected = false;
		while(pEnum->Next(1, &pPin, NULL) == S_OK)
		{
			hr = pPin->EnumMediaTypes(&pEnumMediaType);
			//if this is video output pin
			if(SUCCEEDED(this->CheckMediaType(pEnumMediaType, MEDIATYPE_Video)))
			{
				IPin *pMP4DecoderInput = NULL;
				hr = this->GetUnconnectedPin(pMp4Decoder, PINDIR_INPUT, &pMP4DecoderInput, MEDIATYPE_NULL);
				hr = pGraph->Connect(pPin, pMP4DecoderInput);
				PIN_INFO pInfo;
				pPin->QueryPinInfo(&pInfo);
				pMP4DecoderInput->QueryPinInfo(&pInfo);
				pMP4DecoderInput->Release();
				video_connected = true;
			}
			//if this is audio output pin
			else if (SUCCEEDED(this->CheckMediaType(pEnumMediaType, MEDIATYPE_Audio)))
			{
				IPin *pMP3DecoderInput = NULL;
				hr = this->GetUnconnectedPin(pMP3Decoder, PINDIR_INPUT, &pMP3DecoderInput, MEDIATYPE_NULL);
				hr = pGraph->Connect(pPin, pMP3DecoderInput);
				pMP3DecoderInput->Release();
				audio_connected = true;
			}
			
			pPin->Release();
			pEnumMediaType->Release();
			if (audio_connected && video_connected)
				break;
		}
		pEnum->Release();
		//Connect Audio Renderer
		hr = this->ConnectFilters(pGraph, pMP3Decoder, pDSoundDevice, MEDIATYPE_Audio);

		//Connect the Mp4Decoder to Sample Grabber
		hr = this->ConnectFilters(pGraph, pMp4Decoder, pGrabberF, MEDIATYPE_NULL);

		//get the output pin of grabber, then render pin
		IPin *pOutPin = NULL;
		this->GetUnconnectedPin(pGrabberF, PINDIR_OUTPUT, &pOutPin, MEDIATYPE_NULL);
		hr = pGraph->Render(pOutPin);

		pGrabberF->Release();
		pSrc->Release();
		pAviSplitter->Release();
		pMp4Decoder->Release();
		pMP3Decoder->Release();
		pDSoundDevice->Release();
		IEnumFilters *pEnumFilter;
		IBaseFilter *pRenderFilter;
		hr = pGraph->EnumFilters(&pEnumFilter);
		//test the graph
		SaveGraphFile(pGraph, L"D:\\SliderPlayer_264.grf");
		return S_OK;
	}
	return E_FAIL;
}

HRESULT CDXGraph::CheckMediaType(IEnumMediaTypes *pEnumMedia, GUID MediaType)
{
	AM_MEDIA_TYPE *MType = NULL;
	HRESULT hr=S_OK;
	while((hr=pEnumMedia->Next(1, &MType, NULL)) == S_OK)
	{
		if(MType->majortype == MediaType)
		{
			pEnumMedia->Reset();			
			return S_OK; 
		}
	}
	pEnumMedia->Reset();
	return E_FAIL;
}

HRESULT CDXGraph::AddGrabber(IBaseFilter **pGrabberF)
{
	HRESULT hr = CoCreateInstance(CLSID_SampleGrabber, NULL, CLSCTX_INPROC_SERVER, IID_IBaseFilter, (void **)pGrabberF);
	if(FAILED(hr)) return hr;
	hr = pGraph->AddFilter(*pGrabberF, L"Sample Grabber");
	if(FAILED(hr)) return hr;
	pGrabber = NULL;
	(*pGrabberF)->QueryInterface(IID_ISampleGrabber, (void **)&pGrabber);

	//set the mediatype to samplegrabber
	AM_MEDIA_TYPE mt;
	ZeroMemory(&mt, sizeof(AM_MEDIA_TYPE));
	//set the mediatype depend on display
	HDC hdc = GetDC(NULL);
	int iBitDepth = GetDeviceCaps(hdc, BITSPIXEL);
	ReleaseDC(NULL, hdc);
	mt.majortype = MEDIATYPE_Video;
	switch(iBitDepth)
	{
	case 8:
		mt.subtype = MEDIASUBTYPE_RGB8;
		break;
	case 16:
		mt.subtype = MEDIASUBTYPE_RGB555;
		break;
	case 24:
		mt.subtype = MEDIASUBTYPE_RGB24;
		break;
	case 32:
		mt.subtype = MEDIASUBTYPE_RGB32;
		break;
	default:
		return E_FAIL;
	}
	hr = pGrabber->SetMediaType(&mt);
	if(FAILED(hr)) return hr;
}

HRESULT CDXGraph::Stop()
{
	if(pControl != NULL)
	{
		pControl->Pause();
		HRESULT hr = SetCurrentPosition(0.0);
		hr = pControl->Stop();
		if (pBasicVideo)
		{
			//hack to make the stop action show the first frame.
			if(aveTimePerFrame == 0.0)
			{
				pBasicVideo->get_AvgTimePerFrame(&aveTimePerFrame);
			}
			//hr = pControl->Run();
			//Sleep(aveTimePerFrame*500);
			hr = pControl->Stop();
		}
		return hr;
	}
	return E_FAIL;
}

HRESULT CDXGraph::Pause()
{
	if(pControl != NULL)
	{
		HRESULT hr = pControl->Pause();
		return hr;
	}
	return E_FAIL;
}

IMediaEventEx* CDXGraph::GetEventHandle()
{
	return pEvent;
}

HRESULT CDXGraph::SetNotifyWindow(HWND hwnd)
{
	if(pEvent != NULL)
	{
		HRESULT hr = pEvent->SetNotifyWindow((OAHWND)hwnd, WM_GRAPHNOTIFY, 0);
		return hr;
	}
	return E_FAIL;
}

HRESULT CDXGraph::Run()
{
	if(pControl)
	{
		HRESULT hr = pControl->Run();
		return hr;
	}
	return E_FAIL;
}

HRESULT CDXGraph::FullScreen(HWND hwnd)
{
	if(mVideoWindow)
	{
		mVideoWindow->get_MessageDrain((OAHWND)NULL);
		mVideoWindow->put_MessageDrain((OAHWND)hwnd);
		mVideoWindow->put_Owner((OAHWND)NULL);

		mVideoWindow->put_WindowStyle(0);
		mVideoWindow->put_WindowStyleEx(WS_EX_TOPMOST | WS_POPUP);
		int width = GetSystemMetrics(SM_CXSCREEN);
		int height = GetSystemMetrics(SM_CYSCREEN);
		HRESULT hr = mVideoWindow->SetWindowPosition(0, 0, width, height);

		bFullScreen = TRUE;
		return S_OK;
	}
	return E_FAIL;
}

HRESULT CDXGraph::GetCurrentPosition(double *pos)
{
	if(pSeeking)
	{
		LONGLONG *pCurrent = new LONGLONG;
		HRESULT hr = pSeeking->GetCurrentPosition(pCurrent);
		if (FAILED(hr)) return hr;
		*pos = (double)*pCurrent;
		delete(pCurrent);
		return S_OK;
	}
	return E_FAIL;
}

HRESULT CDXGraph::GetDuration(double *duration)
{
	if(pSeeking)
	{
		LONGLONG *pDuration = new LONGLONG;
		HRESULT hr = pSeeking->GetDuration(pDuration);
		if (FAILED(hr)) return hr;
		*duration = *pDuration;
		delete(pDuration);
		return hr;
	}
	return E_FAIL;
}

HRESULT CDXGraph::SetCurrentPosition(double pos)
{
	if(pSeeking)
	{
		LONGLONG pCurrent = (LONGLONG)pos;
		LONGLONG *duration = new LONGLONG;
		HRESULT hr =pSeeking->GetDuration(duration);
		if (FAILED(hr)) return hr;
		if (pCurrent > *duration) pCurrent = *duration;
		if (pCurrent == *duration) pCurrent--;
		hr = pSeeking->SetPositions(&pCurrent, AM_SEEKING_AbsolutePositioning, duration, AM_SEEKING_AbsolutePositioning);
		delete(duration);
		return hr;
	}
	return E_FAIL;
}

HRESULT CDXGraph::SetDisplayWindow(HWND hwnd)
{
	if (mVideoWindow)
	{
		mVideoWindow->put_Visible(OAFALSE);
		mVideoWindow->put_Owner((OAHWND)hwnd);
		RECT windowRect;
		::GetClientRect(hwnd, &windowRect);
		mVideoWindow->put_Left(0);
		mVideoWindow->put_Top(0);
		mVideoWindow->put_Width(windowRect.right - windowRect.left);
		mVideoWindow->put_Height(windowRect.bottom - windowRect.top);
		mVideoWindow->put_WindowStyle(WS_CHILD|WS_CLIPCHILDREN|WS_CLIPSIBLINGS);
		mVideoWindow->put_MessageDrain((OAHWND)hwnd);
		
		if (hwnd != NULL)
		{
			mVideoWindow->put_Visible(OATRUE);
		}
		else
		{
			mVideoWindow->put_Visible(OAFALSE);
		}
		return S_OK;

	}
	return E_FAIL;
}

HRESULT CDXGraph::ConnectFilters(IGraphBuilder *pGraph, IBaseFilter *pSrc, IBaseFilter *pDest, GUID MediaType)
{
	if(pGraph == NULL || pSrc == NULL || pDest == NULL)
		return E_POINTER;
	IPin *pOut = 0;
	HRESULT hr = GetUnconnectedPin(pSrc, PINDIR_OUTPUT, &pOut, MediaType);
	if (FAILED(hr))
		return hr;
	IPin *pIn = 0;
	hr = GetUnconnectedPin(pDest, PINDIR_INPUT, &pIn, MediaType);
	if (FAILED(hr))
		return hr;
	hr = pGraph->Connect(pOut, pIn);
	pOut->Release();
	pIn->Release();
	return hr;
}

HRESULT CDXGraph::GetUnconnectedPin(IBaseFilter *pFilter, PIN_DIRECTION PinDir, IPin **ppPin, GUID MediaType)
{
	*ppPin = 0;
	IEnumPins *pEnum = 0;
	IPin *pPin = 0;
	HRESULT hr = pFilter->EnumPins(&pEnum);
	if(FAILED(hr))
		return hr;
	while (pEnum->Next(1, &pPin, NULL) == S_OK)
	{
		PIN_DIRECTION ThisPinDir;
		pPin->QueryDirection(&ThisPinDir);
		if (ThisPinDir == PinDir)
		{
			IPin *pTmp = 0;
			hr = pPin->ConnectedTo(&pTmp);
			if (SUCCEEDED(hr))
				pTmp->Release();
			else
			{	IEnumMediaTypes *pEnumMediaType = NULL;
				hr = pPin->EnumMediaTypes(&pEnumMediaType);
				if (MediaType == MEDIATYPE_NULL)
				{
					pEnum->Release();
					*ppPin = pPin;
					pPin->Release();
					return S_OK;
				}else if(SUCCEEDED(this->CheckMediaType(pEnumMediaType, MediaType)))
				{
					pEnum->Release();
					*ppPin = pPin;
					pPin->Release();
					return S_OK;
				} 
				pEnumMediaType->Release();
			}
		}
		pPin->Release();
	}
	pEnum->Release();
	return E_FAIL;
}

BOOL CDXGraph::IsFullScreen()
{
	return bFullScreen;
}

HRESULT CDXGraph::EscFullScreen(HWND hwnd)
{
	if(mVideoWindow)
	{
		mVideoWindow->put_MessageDrain((OAHWND)NULL);
		mVideoWindow->put_Visible(OAFALSE);
		mVideoWindow->put_Owner((OAHWND)hwnd);
		mVideoWindow->put_WindowStyle(WS_CHILD | WS_CLIPSIBLINGS|WS_CLIPCHILDREN);
		mVideoWindow->SetWindowForeground(OATRUE);
		OnSize();
		bFullScreen = FALSE;
		return S_OK;
	}
	return E_FAIL;
}

void CDXGraph::OnSize()
{
	HWND hwnd;
    CWnd *pcwnd;
    CRect rect;
    mVideoWindow->get_Owner((OAHWND*)&hwnd);
    pcwnd=CWnd::FromHandle(hwnd);
    pcwnd->GetClientRect(&rect);
    mVideoWindow->SetWindowPosition(rect.left ,rect.top ,
                  rect.right ,rect.bottom );
}

HRESULT CDXGraph::Grab(CString strFilePath)
{
	if (pControl && pGrabber)
	{
		
		long evCode;
		double cPosition = 0;
		GetCurrentPosition(&cPosition);
		//pControl->Stop();
		HRESULT hr = pGrabber->SetOneShot(FALSE);
		hr = pGrabber->SetBufferSamples(TRUE);
		pControl->Run();
		//don't add the following code, or it will pause the stream
		//pEvent->WaitForCompletion(INFINITE, &evCode);

		//Grab it
		hr = SetCurrentPosition(cPosition);
		//graph must be stopped if you want to call getcurrentbuffer.
		pControl->StopWhenReady();
		while(TRUE)
		{
			OAFilterState fs;
			pControl->GetState(100, &fs);
			if (fs == State_Stopped)
			{
				break;
			}
			Sleep(100);
		}
		long cbBuffer = 0;
		hr = pGrabber->GetCurrentBuffer(&cbBuffer, NULL);
		char *pBuffer = new char[cbBuffer];
		if(!pBuffer)
		{
			return E_FAIL;
		}
		hr = pGrabber->GetCurrentBuffer(&cbBuffer, (long*)pBuffer);
		//Display it
		AM_MEDIA_TYPE mt;
		hr = pGrabber->GetConnectedMediaType(&mt);
		if (FAILED(hr)) return hr;
		VIDEOINFOHEADER *pVih;
		if((mt.formattype == FORMAT_VideoInfo) && (mt.cbFormat >= sizeof(VIDEOINFOHEADER)) && (mt.pbFormat != NULL))
		{
			pVih = (VIDEOINFOHEADER*)mt.pbFormat;
		}
		else
		{
			ZeroMemory(&mt, sizeof(AM_MEDIA_TYPE));
			return VFW_E_INVALIDMEDIATYPE;
		}
		//CString fileName;
		//fileName.Format(_T("D://Example%d.bmp"), grabCount++);

		if(pVih->bmiHeader.biBitCount==32){
			HANDLE hf = CreateFile(strFilePath, GENERIC_WRITE, FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, 0, NULL);
			if (hf == INVALID_HANDLE_VALUE)
			{
				return E_FAIL;
			}
			BITMAPFILEHEADER bfh;
			ZeroMemory(&bfh, sizeof(bfh));
			bfh.bfType = 'MB';
			bfh.bfSize = sizeof(bfh) + cbBuffer*3/4 + sizeof(BITMAPINFOHEADER);
			bfh.bfOffBits = sizeof(BITMAPFILEHEADER) +sizeof(BITMAPINFOHEADER);
			DWORD dwWritten = 0;
			WriteFile(hf, &bfh, sizeof(bfh), &dwWritten, NULL);

			BITMAPINFOHEADER bih;
			ZeroMemory(&bih, sizeof(bih));
			bih.biSize = sizeof(bih);
			bih.biWidth = pVih->bmiHeader.biWidth;
			bih.biHeight = pVih->bmiHeader.biHeight;
			bih.biPlanes = pVih->bmiHeader.biPlanes;
			bih.biBitCount = 24 ;//pVih->bmiHeader.biBitCount;			
			dwWritten = 0;
			WriteFile(hf, &bih, sizeof(bih), &dwWritten, NULL);
			BYTE* tmpBuf = new BYTE[cbBuffer];
			int tmpCbBuffer = 0;
			for (int i = 0; i<cbBuffer;i++)
			{
				if((i&0x3)==0x3){
					continue;
				}
				tmpBuf[tmpCbBuffer++] = pBuffer[i];
			}
			dwWritten = 0;
			WriteFile(hf, tmpBuf, tmpCbBuffer, &dwWritten, NULL);
			delete tmpBuf;
			CloseHandle(hf);
		}
		ZeroMemory(&mt, sizeof(AM_MEDIA_TYPE));
		delete(pBuffer);
		hr = SetCurrentPosition(cPosition);
		return hr;
	}
	return E_FAIL;
}

HRESULT CDXGraph::SaveGraphFile(IGraphBuilder *pGraph, WCHAR *wszPath) 
{
    const WCHAR wszStreamName[] = L"ActiveMovieGraph"; 
    HRESULT hr;
    
    IStorage *pStorage = NULL;
    hr = StgCreateDocfile(
        wszPath,
        STGM_CREATE | STGM_TRANSACTED | STGM_READWRITE | STGM_SHARE_EXCLUSIVE,
        0, &pStorage);
    if(FAILED(hr)) 
    {
        return hr;
    }

    IStream *pStream;
    hr = pStorage->CreateStream(
        wszStreamName,
        STGM_WRITE | STGM_CREATE | STGM_SHARE_EXCLUSIVE,
        0, 0, &pStream);
    if (FAILED(hr)) 
    {
        pStorage->Release();    
        return hr;
    }

    IPersistStream *pPersist = NULL;
    pGraph->QueryInterface(IID_IPersistStream, (void**)&pPersist);
    hr = pPersist->Save(pStream, TRUE);
    pStream->Release();
    pPersist->Release();
    if (SUCCEEDED(hr)) 
    {
        hr = pStorage->Commit(STGC_DEFAULT);
    }
    pStorage->Release();
    return hr;
}

HRESULT CDXGraph::StepForward(int step)
{
	if(pBasicVideo &&pControl && pSeeking)
	{
		OAFilterState pfs;
		pControl->GetState(100, &pfs); 
		if (pfs == State_Paused)  
		{
			if(aveTimePerFrame == 0.0)
			{
				pBasicVideo->get_AvgTimePerFrame(&aveTimePerFrame);
			}
			double current = 0.;
			this->GetCurrentPosition(&current);
			current = current + 10000000 * aveTimePerFrame * step;
			HRESULT hr = this->SetCurrentPosition(current);
			return S_OK;
		}
		else if (pfs == State_Stopped)
		{
			pControl->Pause();
			if(aveTimePerFrame == 0.0)
			{
				pBasicVideo->get_AvgTimePerFrame(&aveTimePerFrame);
			}
			double current = 0.0;			
			current = current + 10000000 * aveTimePerFrame * step;
			HRESULT hr = this->SetCurrentPosition(current);
			return S_OK;
		}
	}
	return E_FAIL;
}

HRESULT CDXGraph::StepBackward(int step)
{
	if(pBasicVideo &&pControl && pSeeking)
	{
		OAFilterState pfs;
		pControl->GetState(100, &pfs); 
		if (pfs == State_Paused)  
		{
			if(aveTimePerFrame == 0.0)
			{
				pBasicVideo->get_AvgTimePerFrame(&aveTimePerFrame);
			}
			double current = 0.;
			this->GetCurrentPosition(&current);
			current = current - 10000000 * aveTimePerFrame * step;
			if (current < 0) current = 0.0;
			HRESULT hr = this->SetCurrentPosition(current);
			return S_OK;
		}

	}
	return E_FAIL;
}



HRESULT CDXGraph::SetRate( double rate )
{
	if(pSeeking)
	{
		HRESULT hr = pSeeking->SetRate(rate);
		return hr;
	}
	return E_FAIL;
}

HRESULT CDXGraph::GetState( OAFilterState* pfs )
{
	if(pControl)
	{
		return pControl->GetState(100, pfs);
	}
}

HRESULT CDXGraph::GetVideoSize( long* pWidth, long* pHeight )
{
	if(pBasicVideo)
	{
		return pBasicVideo->GetVideoSize(pWidth, pHeight);
	}
	return E_FAIL;
}

HRESULT CDXGraph::GetCurrentFrameNum( int *frame )
{
	if(pBasicVideo && pControl && pSeeking)
	{
		double currentTime = 0.0;
		this->GetCurrentPosition(&currentTime);
		if (aveTimePerFrame == 0.0)
		{
			pBasicVideo->get_AvgTimePerFrame(&aveTimePerFrame);
		}
		*frame = currentTime / (aveTimePerFrame * 10000000);
		return S_OK;
	}
	return E_FAIL;

}

HRESULT CDXGraph::GetLastFrameNum( int *frame )
{
	if(pBasicVideo && pControl && pSeeking)
	{
		double durationTime = 0.0;
		this->GetDuration(&durationTime);
		if (aveTimePerFrame == 0.0)
		{
			pBasicVideo->get_AvgTimePerFrame(&aveTimePerFrame);
		}
		*frame = durationTime / (aveTimePerFrame * 10000000);
		return S_OK;
	}
	return E_FAIL;
}

HRESULT CDXGraph::GetAveTimePerFrame( double *aveTimePerFrame )
{
	if(this->aveTimePerFrame > 0)
	{
		*aveTimePerFrame = this->aveTimePerFrame;
		return S_OK;
	}
	return E_FAIL;
}

HRESULT CDXGraph::GrabVideo( LONGLONG *start, LONGLONG *end, CString strPathName )
{
	if(pVideoGrabber != NULL)
	{
		wchar_t *pFile = strPathName.GetBuffer(strPathName.GetLength());
		char *filePathName = (char*)malloc(100);
		wcstombs(filePathName, pFile, 100); 
		HRESULT hr = pVideoGrabber->SaveSpecifiedFragment(start, end, filePathName);
		free(filePathName);
		return hr;
	}
	return E_FAIL;
}

HRESULT CDXGraph::SetRecordFile( char* strPathname )
{
	if (pNetDvr != NULL)
	{
		pNetDvr->SetRecordFile(strPathname);
		return S_OK;
	}
	return S_FALSE;
}

HRESULT CDXGraph::StartRecord()
{
	if(pNetDvr != NULL)
	{
		pNetDvr->StartRecord();
		return S_OK;
	}
	return S_FALSE;
}

HRESULT CDXGraph::StopRecord()
{
	if(pNetDvr != NULL)
	{
		pNetDvr->StopRecord();
		return S_OK;
	}
	return S_FALSE;
}


