
// CaptureCardDlg.h : header file
//

#pragma once
#include "CaptureGraph.h"

// CCaptureCardDlg dialog
class CCaptureCardDlg : public CDialogEx
{
// Construction
public:
	CCaptureCardDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_CAPTURECARD_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();

private:
	CaptureGraph *pCaptureGraph;
};
