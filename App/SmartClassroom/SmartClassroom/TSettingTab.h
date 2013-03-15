#pragma once

#include "APIController.h"
#include "afxwin.h"

// CTSettingTab dialog

class CTSettingTab : public CDialog
{
	DECLARE_DYNAMIC(CTSettingTab)

// User Defined
public:
	BOOL IsSettingBlindZone();
	HRESULT CacheBlindZoneVertex(int x, int y);
private:
	CAPIController *m_pAPIController;
	BOOL m_bSettingBlindZone;
	CPoint m_BlindZoneVertexes[4];
	int m_nSavedPoints;
//System Defined
public:
	CTSettingTab(CWnd* pParent = NULL);   // standard constructor
	virtual ~CTSettingTab();

// Dialog Data
	enum { IDD = IDD_DIALOGTSetting };

protected:
	HICON m_hIcon;
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()
public:
//	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnBnClickedButtontup();
	afx_msg void OnBnClickedButtontdown();
	afx_msg void OnBnClickedButtontleft();
	afx_msg void OnBnClickedButtontright();
	afx_msg void OnBnClickedButtontstop();
	afx_msg void OnBnClickedButtontzoomin();
	afx_msg void OnBnClickedButtontzoomout();
	afx_msg void OnBnClickedButtonsavepresetpos();
	CEdit m_ctrlEditPosId;
	afx_msg void OnBnClickedButtonRecall();
	afx_msg void OnBnClickedButtonstarttracking();
	afx_msg void OnBnClickedButtonstoptracking();
	CEdit m_ctrlEditTeaLeftBorder;
	CEdit m_ctrlEditTeaRightBorder;
	afx_msg void OnBnClickedButtonsavecommonsettings();
	CEdit m_ctrlEditDisFrameThresh;
	CEdit m_ctrlEditCenterWeightThresh;
	CEdit m_ctrlEditLearningRate;
	CEdit m_ctrlEditTrackInterval;
	afx_msg void OnBnClickedButtonadvsettings();
	afx_msg void OnBnClickedButtonsetblindzone();
	afx_msg void OnBnClickedButtonsaveblindzone();
	afx_msg void OnBnClickedButtonclearblindzone2();
};
