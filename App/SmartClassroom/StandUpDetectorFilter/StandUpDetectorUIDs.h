#pragma once
#include <initguid.h>

// {B3D39F8B-F0DF-4D48-ADB3-EEA1EBACCB71}
DEFINE_GUID(CLSID_StandUpDetector, 
	0xb3d39f8b, 0xf0df, 0x4d48, 0xad, 0xb3, 0xee, 0xa1, 0xeb, 0xac, 0xcb, 0x71);

// {A64EC696-5A13-45F8-A0D6-695DBFB13213}
DEFINE_GUID(IID_IStandUpControl, 
	0xa64ec696, 0x5a13, 0x45f8, 0xa0, 0xd6, 0x69, 0x5d, 0xbf, 0xb1, 0x32, 0x13);

DECLARE_INTERFACE_(IStandUpControl, IUnknown)
{
	STDMETHOD(Start)(THIS_ BOOL bShowTrackingRes)PURE;
	STDMETHOD(Stop)(THIS_)PURE;

	STDMETHOD(SetParams)(THIS_ int leftBorder, THIS_ int rightBorder, THIS_ int totalRowNum, THIS_ int totalColNum, THIS_ int detectLine)PURE;
};