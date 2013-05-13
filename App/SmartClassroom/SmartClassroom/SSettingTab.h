#pragma once


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
};
