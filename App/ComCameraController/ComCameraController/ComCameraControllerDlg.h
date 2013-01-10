
// ComCameraControllerDlg.h : header file
//

#pragma once
#include "afxwin.h"


const CString StopCode = L"FF010000000001";

// CComCameraControllerDlg dialog
class CComCameraControllerDlg : public CDialogEx
{
// Construction
public:
	CComCameraControllerDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_COMCAMERACONTROLLER_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

// User Defined
private:
	HANDLE m_hSeries;
	int m_nComNum;
	int m_nPaultRate;


private:
	int initSeries();
	int getIntFromCEdit(CEdit *cedit);
	CString getCStringFromCEdit(CEdit *cedit);
	int sendCStringToComm(const CString& command);

	static DWORD ThreadFunction(CComCameraControllerDlg *msg);

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
	afx_msg void OnBnClickedButtonapply();
	afx_msg void OnBnClickedButtonsend();
	afx_msg void OnBnClickedButtonstop();
private:
	CEdit m_ctrlEditComNum;
	CEdit m_ctrlEditBaudRate;
	CEdit m_ctrlEditCommand;
};
