
// TMReceiverDemoDlg.h : header file
//

#pragma once
#include "TMReceiver.h"
#include "afxwin.h"

// CTMReceiverDemoDlg dialog
class CTMReceiverDemoDlg : public CDialogEx
{
// Construction
public:
	CTMReceiverDemoDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_TMRECEIVERDEMO_DIALOG };

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


private:
	TMRecieverHandle m_rcvrHandle;
	BOOL m_bRecording;
public:
	afx_msg void OnBnClickedButtonopenhttp();
	afx_msg void OnBnClickedButtonopenrtsp();
	afx_msg void OnBnClickedButtonplay();
	afx_msg void OnBnClickedButtonstop();
	afx_msg void OnBnClickedButtonrecord();

public:
	CEdit m_ctrlEditHttp;
	CEdit m_ctrlEditRtsp;

private:
	CString getCStringFromCEdit( CEdit *ctrlEdit );
	int doSomeSettings();
};
