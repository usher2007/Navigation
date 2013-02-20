// TSettingTab.cpp : implementation file
//

#include "stdafx.h"
#include "SmartClassroom.h"
#include "TSettingTab.h"
#include "afxdialogex.h"
#include <iostream>


// CTSettingTab dialog

IMPLEMENT_DYNAMIC(CTSettingTab, CDialog)

CTSettingTab::CTSettingTab(CWnd* pParent /*=NULL*/)
	: CDialog(CTSettingTab::IDD, pParent)
{
	m_pAPIController = CAPIController::GetInstance();
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
	ON_BN_CLICKED(IDC_BUTTONRecall, &CTSettingTab::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTONStartTracking, &CTSettingTab::OnBnClickedButtonstarttracking)
	ON_BN_CLICKED(IDC_BUTTONStopTracking, &CTSettingTab::OnBnClickedButtonstoptracking)
END_MESSAGE_MAP()


// CTSettingTab message handlers


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

void CTSettingTab::OnBnClickedButton1()
{
	// TODO: Remove this function
	int locId = getIntFromCEdit(&m_ctrlEditPosId);
	if(m_pAPIController != NULL)
	{
		m_pAPIController->TeacherPTZRecallPrePos(locId);
	}
	return;
}

int CTSettingTab::getIntFromCEdit( CEdit *ctrlEdit )
{
	int num;
	CString param;
	param = getCStringFromCEdit(ctrlEdit);
	num = _ttoi(param);
	return num;
}

CString CTSettingTab::getCStringFromCEdit( CEdit *ctrlEdit )
{
	int paramLen;
	CString param;
	paramLen = ctrlEdit->LineLength(0);
	ctrlEdit->GetLine(0, param.GetBuffer(paramLen), paramLen);
	param.ReleaseBuffer(paramLen);
	return param;
}

double CTSettingTab::getDoubleFromCEdit(CEdit *ctrlEdit)
{
	double ret = 0.0;
	CString param;
	param = getCStringFromCEdit(ctrlEdit);
	ret = _ttol(param);
	return ret;
}


