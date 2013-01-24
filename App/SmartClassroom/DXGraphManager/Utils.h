#include "DShow.h"
#include <atlcomcli.h>

#define SAFE_RELEASE(p)			{ if(p) { (p)->Release(); (p)=NULL; } }

class CUtils
{
public:
	static HRESULT CheckMediaType(CComPtr<IEnumMediaTypes> pEnumMedia, GUID MediaType);
	static HRESULT SaveGraphFile(CComPtr<IGraphBuilder> pGraph, WCHAR *wszPath);
	static HRESULT ConnectFilters(CComPtr<IGraphBuilder> pGraph, CComPtr<IBaseFilter> pSrc, CComPtr<IBaseFilter> pDest, GUID MediaType);
	static HRESULT GetUnconnectedPin(CComPtr<IBaseFilter> pFilter, PIN_DIRECTION PinDir, IPin **ppPin, GUID MediaType);
	static HRESULT AddFilter2(IGraphBuilder* pGraph, const GUID &clsid, LPCWSTR pName, IBaseFilter** ppFilter);
};