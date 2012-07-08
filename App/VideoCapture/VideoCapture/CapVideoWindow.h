#pragma once


// CapVideoWindow dialog

class CapVideoWindow : public CStatic
{
	DECLARE_DYNAMIC(CapVideoWindow)

public:
	CapVideoWindow();
	virtual ~CapVideoWindow();
	void SetVideoSize(int videoWidth, int videoHeight);

protected:
	DECLARE_MESSAGE_MAP()
	int mVideoWidth;
	int mVideoHeight;
public:
	afx_msg void OnGetMinMaxInfo(MINMAXINFO* lpMMI);
};
