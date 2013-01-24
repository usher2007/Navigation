
// SmartClassroomDlg.h : header file
//

#pragma once
#include "afxcmn.h"
#include "TabSheet.h"
#include "TSettingTab.h"


// CSmartClassroomDlg dialog
class CSmartClassroomDlg : public CDialogEx
{
// Construction
public:
	CSmartClassroomDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_SMARTCLASSROOM_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

// User Defined
	CAPIController *m_pAPIController;
// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CTabSheet m_tabSheet;
	CTSettingTab m_TSettingPage;
	//CSSettingTab m_SSettingPage;
	//CLSettingTab m_LSettingPage;
};
