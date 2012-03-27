#pragma once


// MyVideoWindow

class MyVideoWindow : public CStatic
{
	DECLARE_DYNAMIC(MyVideoWindow)

public:
	MyVideoWindow();
	virtual ~MyVideoWindow();
	void SetVideoSize(int videoWidth, int videoHeight);

protected:
	DECLARE_MESSAGE_MAP()
	int mVideoWidth;
	int mVideoHeight;
public:
	afx_msg void OnGetMinMaxInfo(MINMAXINFO* lpMMI);
};

