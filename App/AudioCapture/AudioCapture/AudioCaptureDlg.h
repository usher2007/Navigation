
// AudioCaptureDlg.h : header file
//

#pragma once
#include "RecordGraph.h"

// CAudioCaptureDlg dialog
class CAudioCaptureDlg : public CDialogEx
{
// Construction
public:
	CAudioCaptureDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_AUDIOCAPTURE_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;
	RecordGraph *pRecordGraph;
	BOOL m_bRecord;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
};
