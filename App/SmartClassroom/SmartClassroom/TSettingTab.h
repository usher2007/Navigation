#pragma once

#include "APIController.h"

// CTSettingTab dialog

class CTSettingTab : public CDialog
{
	DECLARE_DYNAMIC(CTSettingTab)

// User Defined

private:
	CAPIController *m_pAPIController;
//System Defined
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
	afx_msg void OnBnClickedButtontup();
	afx_msg void OnBnClickedButtontdown();
	afx_msg void OnBnClickedButtontleft();
	afx_msg void OnBnClickedButtontright();
	afx_msg void OnBnClickedButtontstop();
	afx_msg void OnBnClickedButtontzoomin();
	afx_msg void OnBnClickedButtontzoomout();
};
