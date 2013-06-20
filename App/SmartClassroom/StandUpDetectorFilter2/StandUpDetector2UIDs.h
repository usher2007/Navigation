#pragma once
#include <initguid.h>

// {B8353A2A-B0D4-4B79-B381-0BB1E925CC19}
DEFINE_GUID(CLSID_StandUpDetector2, 
	0xb8353a2a, 0xb0d4, 0x4b79, 0xb3, 0x81, 0xb, 0xb1, 0xe9, 0x25, 0xcc, 0x19);
// {A64EC696-5A13-45F8-A0D6-695DBFB13213}
DEFINE_GUID(IID_IStandUpControl, 
	0xa64ec696, 0x5a13, 0x45f8, 0xa0, 0xd6, 0x69, 0x5d, 0xbf, 0xb1, 0x32, 0x13);

DECLARE_INTERFACE_(IStandUpControl, IUnknown)
{
	STDMETHOD(Start)(THIS_ BOOL bShowTrackingRes)PURE;
	STDMETHOD(Stop)(THIS_)PURE;

	STDMETHOD(SetParams)(THIS_ int leftBorder, THIS_ int rightBorder, THIS_ int totalRowNum, THIS_ int totalColNum, THIS_ int detectLine)PURE;
};