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
	DDX_Control(pDX, IDC_COMBOProtocol, m_ctrlComboProtocol);
	DDX_Control(pDX, IDC_EDITVelocity, m_ctrlEditVelocity);
}


BEGIN_MESSAGE_MAP(CAdvSettingsDialog, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTONSaveAdvSettings, &CAdvSettingsDialog::OnBnClickedButtonsaveadvsettings)
	ON_BN_CLICKED(IDC_BUTTONCancelAdvSettings, &CAdvSettingsDialog::OnBnClickedButtoncanceladvsettings)
END_MESSAGE_MAP()

BOOL CAdvSettingsDialog::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
	m_ctrlComboProtocol.ResetContent();
	m_ctrlComboProtocol.InsertString(0, _T("Pelco-D"));
	m_ctrlComboProtocol.InsertString(1, _T("VISCA"));
	
	int pixOverlap=0, leastHumanGap=0, humanWidth=0, fgLowThresh=0, fgUpThresh=0, protocol=0, velocity=0;
	double classroomWidth=0.0, cameraDistance=0.0, fgHistThresh = 0.0;
	m_pAPIController->TeacherGetDetailParams(pixOverlap,
		                                     classroomWidth,
											 cameraDistance,
											 leastHumanGap,
											 humanWidth,
											 fgLowThresh,
											 fgUpThresh,
											 fgHistThresh,
											 protocol,
											 velocity);
	wchar_t *stemp = new wchar_t[1024];

	memset(stemp, 0x00, 1024);
	swprintf(stemp, 1024, L"%d", pixOverlap);
	m_ctrlEditPresetLocOverlap.SetWindowTextW(stemp);

	memset(stemp, 0x00, 1024);
	swprintf(stemp, 1024, L"%f", classroomWidth);
	m_ctrlEditRoomWidth.SetWindowTextW(stemp);

	memset(stemp, 0x00, 1024);
	swprintf(stemp, 1024, L"%f", cameraDistance);
	m_ctrlEditCameraDistance.SetWindowTextW(stemp);

	memset(stemp, 0x00, 1024);
	swprintf(stemp, 1024, L"%d", leastHumanGap);
	m_ctrlEditLeastHumanGap.SetWindowTextW(stemp);

	memset(stemp, 0x00, 1024);
	swprintf(stemp, 1024, L"%d", humanWidth);
	m_ctrlEditHumanWidth.SetWindowTextW(stemp);

	memset(stemp, 0x00, 1024);
	swprintf(stemp, 1024, L"%d", fgLowThresh);
	m_ctrlEditFgLowThresh.SetWindowTextW(stemp);

	memset(stemp, 0x00, 1024);
	swprintf(stemp, 1024, L"%d", fgUpThresh);
	m_ctrlEditFgUpThresh.SetWindowTextW(stemp);

	memset(stemp, 0x00, 1024);
	int fgHistThreshLvl = (int)(fgHistThresh);
	swprintf(stemp, 1024, L"%d", fgHistThreshLvl);
	m_ctrlEditFgHistThresh.SetWindowTextW(stemp);

	memset(stemp, 0x00, 1024);
	swprintf(stemp, 1024, L"%d", velocity);
	m_ctrlEditVelocity.SetWindowTextW(stemp);

	m_ctrlComboProtocol.SetCurSel(protocol);
	delete[] stemp;
	return TRUE;  // return TRUE  unless you set the focus to a control
}

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
		int fgHistThresh = getIntFromCEdit(&m_ctrlEditFgHistThresh);
		int presetLocOverlap = getIntFromCEdit(&m_ctrlEditPresetLocOverlap);
		int protocol = m_ctrlComboProtocol.GetCurSel();
		int velocity = getIntFromCEdit(&m_ctrlEditVelocity);

		m_pAPIController->TeacherSetDetailParams(presetLocOverlap, roomWidth, camDist, leastHumanGap, humanWidth, fgLowThresh, fgUpThresh, fgHistThresh);
		m_pAPIController->SetTeaCameraProtocol(protocol);
		m_pAPIController->SetTeaCameraVelocity(velocity);
	}

	CDialogEx::OnOK();
}


void CAdvSettingsDialog::OnBnClickedButtoncanceladvsettings()
{
	CDialogEx::OnCancel();
}
