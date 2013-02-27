#pragma once
#include <initguid.h>

// {A31E6B72-C03B-42EE-BFDD-774F9A905984}
DEFINE_GUID(CLSID_TrackingAlg, 
	0xa31e6b72, 0xc03b, 0x42ee, 0xbf, 0xdd, 0x77, 0x4f, 0x9a, 0x90, 0x59, 0x84);

// {194E38FA-A8B6-4208-A72D-82AF191BA7D4}
DEFINE_GUID(IID_ITrackingControl, 
	0x194e38fa, 0xa8b6, 0x4208, 0xa7, 0x2d, 0x82, 0xaf, 0x19, 0x1b, 0xa7, 0xd4);
DECLARE_INTERFACE_(ITrackingControl, IUnknown)
{
	STDMETHOD(StartTracking)(THIS_ BOOL bShowTrackingRes)PURE;
	STDMETHOD(StopTracking)(THIS_)PURE;
};