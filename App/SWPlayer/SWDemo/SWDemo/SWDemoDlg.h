
// SWDemoDlg.h : header file
//

#pragma once
#include "MyVideoWindow.h"
#include "DXGraph.h"
#include "afxwin.h"
#define  UNICODE
#define  _UNICODE
// CSWDemoDlg dialog
class CSWDemoDlg : public CDialogEx
{
// Construction
public:
	CSWDemoDlg(CWnd* pParent = NULL);	// standard constructor
	void DestroyGraph(void);
	void CreateGraph(void);
// Dialog Data
	enum { IDD = IDD_SWDEMO_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;
	CString mSourceFile;
	BOOL m_bRecord;
	BOOL m_bPlay;
	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg LRESULT OnGraphNotify(WPARAM inWParam, LPARAM inLParam);
	DECLARE_MESSAGE_MAP()
public:
	MyVideoWindow m_VideoCtrl;
	CEdit m_hostCtrl;
	CEdit m_userNameCtrl;
	CEdit m_passwordCtrl;
	CDXGraph* mFilterGraph;
	afx_msg void OnBnClickedButtonPlay();
	afx_msg void OnBnClickedButtonStop();
	afx_msg void OnBnClickedButtonResolution();
	afx_msg void OnBnClickedButtonRecord();
	afx_msg void OnBnClickedButtonSnipping();
	afx_msg void OnBnClickedButtonLogin();
	afx_msg void OnBnClickedButtonLogout();
};
