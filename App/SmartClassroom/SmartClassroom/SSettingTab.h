#pragma once
#include "APIController.h"
#include "afxwin.h"

// CSSettingTab dialog

class CSSettingTab : public CDialogEx
{
	DECLARE_DYNAMIC(CSSettingTab)

public:
	CSSettingTab(CWnd* pParent = NULL);   // standard constructor
	virtual ~CSSettingTab();

// Dialog Data
	enum { IDD = IDD_DIALOGSSetting };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

private:
	CAPIController *m_pAPIController;
public:
	afx_msg void OnBnClickedButtonsup();
	afx_msg void OnBnClickedButtonsdown();
	afx_msg void OnBnClickedButtonsleft();
	afx_msg void OnBnClickedButtonsright();
	afx_msg void OnBnClickedButtonsstop();
	afx_msg void OnBnClickedButtonszoomin();
	afx_msg void OnBnClickedButtonszoomout();
	CEdit m_ctrlEditSPosId;
	CEdit m_ctrlEditSRowNum;
	CEdit m_ctrlEditColNum;
	afx_msg void OnBnClickedButtonsrecall();
	afx_msg void OnBnClickedButtonsclearpresetpos();
	afx_msg void OnBnClickedButtonssavepresetpos();
};
