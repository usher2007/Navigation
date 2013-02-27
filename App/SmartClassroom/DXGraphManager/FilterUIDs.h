#pragma once
#include <initguid.h>

// {6f814be9-9af6-43cf-9249-c03401000212}
DEFINE_GUID(CLSID_TWCapture01, 
	0x6f814be9, 0x9af6, 0x43cf, 0x92, 0x49, 0xc0, 0x34, 0x01, 0x00, 0x02, 0x12);

// {A31E6B72-C03B-42EE-BFDD-774F9A905984}
DEFINE_GUID(CLSID_TrackingAlg, 
	0xa31e6b72, 0xc03b, 0x42ee, 0xbf, 0xdd, 0x77, 0x4f, 0x9a, 0x90, 0x59, 0x84);

// {C1F400A4-3F08-11D3-9F0B-006008039E37}
DEFINE_GUID(CLSID_NullRenderer, 
	0xc1f400a4, 0x3f08, 0x11d3, 0x9f, 0x0b, 0x00, 0x60, 0x08, 0x03, 0x9e, 0x37);

// {194E38FA-A8B6-4208-A72D-82AF191BA7D4}
DEFINE_GUID(IID_ITrackingControl, 
	0x194e38fa, 0xa8b6, 0x4208, 0xa7, 0x2d, 0x82, 0xaf, 0x19, 0x1b, 0xa7, 0xd4);
MIDL_INTERFACE("194E38FA-A8B6-4208-A72D-82AF191BA7D4")
ITrackingControl : public IUnknown
{
public:
	//²¶»ñÍ¼Æ¬µÄÃüÁî
	virtual HRESULT STDMETHODCALLTYPE StartTracking(BOOL bShowTrackingRes) = 0;
	virtual HRESULT STDMETHODCALLTYPE StopTracking() = 0;
};
