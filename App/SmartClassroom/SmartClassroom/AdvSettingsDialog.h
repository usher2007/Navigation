#pragma once
#include "afxwin.h"
#include "APIController.h"

// CAdvSettingsDialog dialog

class CAdvSettingsDialog : public CDialogEx
{
	DECLARE_DYNAMIC(CAdvSettingsDialog)

public:
	CAdvSettingsDialog(CWnd* pParent = NULL);   // standard constructor
	virtual ~CAdvSettingsDialog();

// Dialog Data
	enum { IDD = IDD_DIALOGAdvSettings };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CEdit m_ctrlEditRoomWidth;
	CEdit m_ctrlEditCameraDistance;
	CEdit m_ctrlEditLeastHumanGap;
	CEdit m_ctrlEditHumanWidth;
	CEdit m_ctrlEditFgLowThresh;
	CEdit m_ctrlEditFgUpThresh;
	CEdit m_ctrlEditFgHistThresh;
	CEdit m_ctrlEditPresetLocOverlap;

public:
	afx_msg void OnBnClickedButtonsaveadvsettings();
// User Defined
private:
	CAPIController* m_pAPIController;

public:
	afx_msg void OnBnClickedButtoncanceladvsettings();
};
