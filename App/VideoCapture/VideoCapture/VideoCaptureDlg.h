
// VideoCaptureDlg.h : header file
//

#pragma once
#include "afxwin.h"
#include "CaptureGraph.h"
#include "CapVideoWindow.h"


// CVideoCaptureDlg dialog
class CVideoCaptureDlg : public CDialogEx
{
// Construction
public:
	CVideoCaptureDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_VIDEOCAPTURE_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CListBox m_ctrlFilterList;

private:
	CaptureGraph *pCaptureGraph;
	BOOL isGraphBuilded;

	void fillFilterListBox();
public:
	afx_msg void OnBnClickedButtonplay();
	CapVideoWindow m_ctrlVideoWindow;
	afx_msg void OnBnClickedButtonstop();
};
