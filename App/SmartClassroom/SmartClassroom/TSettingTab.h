#pragma once


// CTSettingTab dialog

class CTSettingTab : public CDialog
{
	DECLARE_DYNAMIC(CTSettingTab)

public:
	CTSettingTab(CWnd* pParent = NULL);   // standard constructor
	virtual ~CTSettingTab();

// Dialog Data
	enum { IDD = IDD_DIALOGTSetting };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
//	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
};
