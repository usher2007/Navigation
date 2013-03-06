// AdvSettingsDialog.cpp : implementation file
//

#include "stdafx.h"
#include "SmartClassroom.h"
#include "AdvSettingsDialog.h"
#include "afxdialogex.h"
#include "Utils.h"


// CAdvSettingsDialog dialog

IMPLEMENT_DYNAMIC(CAdvSettingsDialog, CDialogEx)

CAdvSettingsDialog::CAdvSettingsDialog(CWnd* pParent /*=NULL*/)
	: CDialogEx(CAdvSettingsDialog::IDD, pParent)
{
	m_pAPIController = CAPIController::GetInstance();
}

CAdvSettingsDialog::~CAdvSettingsDialog()
{
}

void CAdvSettingsDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDITRoomWidth, m_ctrlEditRoomWidth);
	DDX_Control(pDX, IDC_EDITCameraDistance, m_ctrlEditCameraDistance);
	DDX_Control(pDX, IDC_EDITLeastHumanGap, m_ctrlEditLeastHumanGap);
	DDX_Control(pDX, IDC_EDITHumanWidth, m_ctrlEditHumanWidth);
	DDX_Control(pDX, IDC_EDITFgLowThresh, m_ctrlEditFgLowThresh);
	DDX_Control(pDX, IDC_EDITFgUpThresh, m_ctrlEditFgUpThresh);
	DDX_Control(pDX, IDC_EDITFgHistThresh, m_ctrlEditFgHistThresh);
	DDX_Control(pDX, IDC_EDITPresetLocOverlap, m_ctrlEditPresetLocOverlap);
}


BEGIN_MESSAGE_MAP(CAdvSettingsDialog, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTONSaveAdvSettings, &CAdvSettingsDialog::OnBnClickedButtonsaveadvsettings)
	ON_BN_CLICKED(IDC_BUTTONCancelAdvSettings, &CAdvSettingsDialog::OnBnClickedButtoncanceladvsettings)
END_MESSAGE_MAP()


// CAdvSettingsDialog message handlers


void CAdvSettingsDialog::OnBnClickedButtonsaveadvsettings()
{
	if(m_pAPIController != NULL)
	{
		double roomWidth = getDoubleFromCEdit(&m_ctrlEditRoomWidth);
		double camDist = getDoubleFromCEdit(&m_ctrlEditCameraDistance);
		int leastHumanGap = getIntFromCEdit(&m_ctrlEditLeastHumanGap);
		int humanWidth = getIntFromCEdit(&m_ctrlEditHumanWidth);
		int fgLowThresh = getIntFromCEdit(&m_ctrlEditFgLowThresh);
		int fgUpThresh = getIntFromCEdit(&m_ctrlEditFgUpThresh);
		double fgHistThresh = getDoubleFromCEdit(&m_ctrlEditFgHistThresh);
		int presetLocOverlap = getIntFromCEdit(&m_ctrlEditPresetLocOverlap);

		m_pAPIController->TeacherSetDetailParams(presetLocOverlap, roomWidth, camDist, leastHumanGap, humanWidth, fgLowThresh, fgUpThresh, fgHistThresh);
	}

	CDialogEx::OnOK();
}


void CAdvSettingsDialog::OnBnClickedButtoncanceladvsettings()
{
	CDialogEx::OnCancel();
}
