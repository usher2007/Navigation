// TSettingTab.cpp : implementation file
//

#include "stdafx.h"
#include "SmartClassroom.h"
#include "TSettingTab.h"
#include "afxdialogex.h"
#include "resource.h"
#include <iostream>
#include "Utils.h"
#include "AdvSettingsDialog.h"


// CTSettingTab dialog

IMPLEMENT_DYNAMIC(CTSettingTab, CDialog)

CTSettingTab::CTSettingTab(CWnd* pParent /*=NULL*/)
	: CDialog(CTSettingTab::IDD, pParent)
{
	m_pAPIController = CAPIController::GetInstance();
	m_bSettingBlindZone = FALSE;
	m_nSavedPoints = 0;
}


CTSettingTab::~CTSettingTab()
{
}

void CTSettingTab::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDITPosId, m_ctrlEditPosId);
	DDX_Control(pDX, IDC_EDITTeaLeftBorder, m_ctrlEditTeaLeftBorder);
	DDX_Control(pDX, IDC_EDITTeaRightBorder, m_ctrlEditTeaRightBorder);
	DDX_Control(pDX, IDC_EDITDisFrameThresh, m_ctrlEditDisFrameThresh);
	DDX_Control(pDX, IDC_EDITcenterWeightThresh, m_ctrlEditCenterWeightThresh);
	DDX_Control(pDX, IDC_EDITLearningRate, m_ctrlEditLearningRate);
	DDX_Control(pDX, IDC_EDITTrackInterval, m_ctrlEditTrackInterval);
}


BEGIN_MESSAGE_MAP(CTSettingTab, CDialog)
//	ON_WM_LBUTTONDOWN()
	ON_BN_CLICKED(IDC_BUTTONTUP, &CTSettingTab::OnBnClickedButtontup)
	ON_BN_CLICKED(IDC_BUTTONTDOWN, &CTSettingTab::OnBnClickedButtontdown)
	ON_BN_CLICKED(IDC_BUTTONTLeft, &CTSettingTab::OnBnClickedButtontleft)
	ON_BN_CLICKED(IDC_BUTTONTRight, &CTSettingTab::OnBnClickedButtontright)
	ON_BN_CLICKED(IDC_BUTTONTStop, &CTSettingTab::OnBnClickedButtontstop)
	ON_BN_CLICKED(IDC_BUTTONTZoomIn, &CTSettingTab::OnBnClickedButtontzoomin)
	ON_BN_CLICKED(IDC_BUTTONTZoomOut, &CTSettingTab::OnBnClickedButtontzoomout)
	ON_BN_CLICKED(IDC_BUTTONSavePreSetPos, &CTSettingTab::OnBnClickedButtonsavepresetpos)
	ON_BN_CLICKED(IDC_BUTTONRecall, &CTSettingTab::OnBnClickedButtonRecall)
	ON_BN_CLICKED(IDC_BUTTONStartTracking, &CTSettingTab::OnBnClickedButtonstarttracking)
	ON_BN_CLICKED(IDC_BUTTONStopTracking, &CTSettingTab::OnBnClickedButtonstoptracking)
	ON_BN_CLICKED(IDC_BUTTONSaveCommonSettings, &CTSettingTab::OnBnClickedButtonsavecommonsettings)
	ON_BN_CLICKED(IDC_BUTTONAdvSettings, &CTSettingTab::OnBnClickedButtonadvsettings)
	ON_BN_CLICKED(IDC_BUTTONSetBlindZone, &CTSettingTab::OnBnClickedButtonsetblindzone)
	ON_BN_CLICKED(IDC_BUTTONSaveBlindZone, &CTSettingTab::OnBnClickedButtonsaveblindzone)
	ON_BN_CLICKED(IDC_BUTTONClearBlindZone2, &CTSettingTab::OnBnClickedButtonclearblindzone2)
	ON_BN_CLICKED(IDC_BUTTONClearPresetPos, &CTSettingTab::OnBnClickedButtonclearpresetpos)
END_MESSAGE_MAP()


// CTSettingTab message handlers

BOOL CTSettingTab::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
	((CButton *)GetDlgItem(IDC_CHECKShowTracking))->SetCheck(BST_CHECKED);
	int disappearFrameThresh = 0, centerWeightThresh = 0, trackingInterval = 0;
	double gbmLearningRate = 0.0;
	m_pAPIController->TeacherGetCommonParams(disappearFrameThresh,
		                                     centerWeightThresh,
											 gbmLearningRate,
											 trackingInterval);

	wchar_t *stemp = new wchar_t[1024];

	memset(stemp, 0x00, 1024);
	double disappearFrameSecond = disappearFrameThresh * 1.0 / 25.0;
	swprintf(stemp, 1024, L"%f", disappearFrameSecond);
	m_ctrlEditDisFrameThresh.SetWindowTextW(stemp);

	memset(stemp, 0x00, 1024);
	centerWeightThresh /= 5;
	swprintf(stemp, 1024, L"%d", centerWeightThresh);
	m_ctrlEditCenterWeightThresh.SetWindowTextW(stemp);

	memset(stemp, 0x00, 1024);
	int gbmLearningRateLvl = (int)(gbmLearningRate / 0.005);
	swprintf(stemp, 1024, L"%d", gbmLearningRateLvl);
	m_ctrlEditLearningRate.SetWindowTextW(stemp);

	memset(stemp, 0x00, 1024);
	swprintf(stemp, 1024, L"%d", trackingInterval);
	m_ctrlEditTrackInterval.SetWindowTextW(stemp);

	delete stemp;

	return TRUE;  // return TRUE  unless you set the focus to a control
}
void CTSettingTab::OnBnClickedButtontup()
{
	// TODO: Add your control notification handler code here
	if(m_pAPIController != NULL)
	{
		m_pAPIController->TeacherPTZUp();
	}
	return;
}

void CTSettingTab::OnBnClickedButtontdown()
{
	// TODO: Add your control notification handler code here
	if(m_pAPIController != NULL)
	{
		m_pAPIController->TeacherPTZDown();
	}
	return;
}

void CTSettingTab::OnBnClickedButtontleft()
{
	// TODO: Add your control notification handler code here
	if(m_pAPIController != NULL)
	{
		m_pAPIController->TeacherPTZLeft();
	}
	return;
}


void CTSettingTab::OnBnClickedButtontright()
{
	// TODO: Add your control notification handler code here
	if(m_pAPIController != NULL)
	{
		m_pAPIController->TeacherPTZRight();
	}
	return;
}


void CTSettingTab::OnBnClickedButtontstop()
{
	// TODO: Add your control notification handler code here
	if(m_pAPIController != NULL)
	{
		m_pAPIController->TeacherPTZStop();
	}
	return;
}


void CTSettingTab::OnBnClickedButtontzoomin()
{
	// TODO: Add your control notification handler code here
	if(m_pAPIController != NULL)
	{
		m_pAPIController->TeacherPTZZoomIn();
	}
	return;
}


void CTSettingTab::OnBnClickedButtontzoomout()
{
	// TODO: Add your control notification handler code here
	if(m_pAPIController != NULL)
	{
		m_pAPIController->TeacherPTZZoomOut();
	}
	return;
}


void CTSettingTab::OnBnClickedButtonsavepresetpos()
{
	// TODO: Add your control notification handler code here
	int locId = getIntFromCEdit(&m_ctrlEditPosId);
	double leftBorder = getDoubleFromCEdit(&m_ctrlEditTeaLeftBorder);
	double rightBorder = getDoubleFromCEdit(&m_ctrlEditTeaRightBorder);
	double roomWidth = 0.0, cameraDistance = 0.0;
	if(m_pAPIController != NULL)
	{
		m_pAPIController->TeacherGetEnvParams(roomWidth, cameraDistance);
		int pixLeftBorder = 0, pixRightBorder = 0;
		if(roomWidth != 0)
		{
			pixLeftBorder = leftBorder / roomWidth * 720;
			pixRightBorder = rightBorder /  roomWidth * 720;
		}
		m_pAPIController->TeacherPTZSetPrePos(locId, pixLeftBorder, pixRightBorder, leftBorder, rightBorder);
	}
	return;
}

void CTSettingTab::OnBnClickedButtonstarttracking()
{
	if(m_pAPIController != NULL)
	{
		BOOL bShowTracking = (BST_CHECKED == IsDlgButtonChecked(IDC_CHECKShowTracking));
		m_pAPIController->TeacherSetShowTracking(bShowTracking);
		m_pAPIController->TeacherTrackingStart();
	}
}

void CTSettingTab::OnBnClickedButtonstoptracking()
{
	if(m_pAPIController != NULL)
	{
		m_pAPIController->TeacherTrackingStop();
	}
}

void CTSettingTab::OnBnClickedButtonRecall()
{
	// TODO: Remove this function
	int locId = getIntFromCEdit(&m_ctrlEditPosId);
	if(m_pAPIController != NULL)
	{
		m_pAPIController->TeacherPTZRecallPrePos(locId);
	}
	return;
}

void CTSettingTab::OnBnClickedButtonsavecommonsettings()
{
	if(m_pAPIController != NULL)
	{
		int disFrameThresh = 25 * getDoubleFromCEdit(&m_ctrlEditDisFrameThresh);
		int centerWeigthThresh = 5 * getIntFromCEdit(&m_ctrlEditCenterWeightThresh);
		double gbmLearningRate = 0.005 * getIntFromCEdit(&m_ctrlEditLearningRate);
		int trackInterval = getIntFromCEdit(&m_ctrlEditTrackInterval);

		m_pAPIController->TeacherSetCommonParams(disFrameThresh, centerWeigthThresh, gbmLearningRate, trackInterval);
	}
}

void CTSettingTab::OnBnClickedButtonadvsettings()
{
	CAdvSettingsDialog advSettingsDlg;
	if(advSettingsDlg.DoModal() == IDOK)
	{
		// Popup a message box to indicate that setting success.
		::MessageBox(NULL, _T("设置成功！"), _T("提示"), MB_OK);
		return;
	}
}

void CTSettingTab::OnBnClickedButtonsetblindzone()
{
	if(!m_bSettingBlindZone)
	{
		m_bSettingBlindZone = TRUE;
		GetDlgItem(IDC_BUTTONSetBlindZone)->SetWindowTextW(_T("清除"));
	}
	else
	{
		m_bSettingBlindZone = FALSE;
		if(m_pAPIController != NULL)
		{
			m_pAPIController->TeacherEraseCurrentBlindZone();
		}
		m_nSavedPoints = 0;
		GetDlgItem(IDC_BUTTONSetBlindZone)->SetWindowTextW(_T("设置屏蔽区"));
	}
}

BOOL CTSettingTab::IsSettingBlindZone()
{
	return m_bSettingBlindZone;
}


void CTSettingTab::OnBnClickedButtonsaveblindzone()
{
	if(m_nSavedPoints == 4 && m_pAPIController)
	{
		m_pAPIController->TeacherSaveBlindZone(m_BlindZoneVertexes[0].x, m_BlindZoneVertexes[0].y,
			                                   m_BlindZoneVertexes[1].x, m_BlindZoneVertexes[1].y,
											   m_BlindZoneVertexes[2].x, m_BlindZoneVertexes[2].y,
											   m_BlindZoneVertexes[3].x, m_BlindZoneVertexes[3].y);
		m_bSettingBlindZone = FALSE;
		GetDlgItem(IDC_BUTTONSetBlindZone)->SetWindowTextW(_T("设置屏蔽区"));
		m_nSavedPoints = 0;
	}
	return;
}

HRESULT CTSettingTab::CacheBlindZoneVertex(int x, int y)
{
	if(m_nSavedPoints >=0 && m_nSavedPoints <=3)
	{
		m_BlindZoneVertexes[m_nSavedPoints].x = x;
		m_BlindZoneVertexes[m_nSavedPoints].y = y;
		m_nSavedPoints++;
		return S_OK;
	}
	return E_FAIL;
}


void CTSettingTab::OnBnClickedButtonclearblindzone2()
{
	if(m_pAPIController)
	{
		m_pAPIController->TeacherClearBlindZones();
	}
}


void CTSettingTab::OnBnClickedButtonclearpresetpos()
{
	if(m_pAPIController)
	{
		m_pAPIController->TeacherPTZClearPrePos();
	}
}
