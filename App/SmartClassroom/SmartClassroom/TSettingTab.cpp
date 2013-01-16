// TSettingTab.cpp : implementation file
//

#include "stdafx.h"
#include "SmartClassroom.h"
#include "TSettingTab.h"
#include "afxdialogex.h"


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
