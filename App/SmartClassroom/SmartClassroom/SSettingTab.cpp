// SSettingTab.cpp : implementation file
//

#include "stdafx.h"
#include "SmartClassroom.h"
#include "SSettingTab.h"
#include "afxdialogex.h"


// CSSettingTab dialog

IMPLEMENT_DYNAMIC(CSSettingTab, CDialogEx)

CSSettingTab::CSSettingTab(CWnd* pParent /*=NULL*/)
	: CDialogEx(CSSettingTab::IDD, pParent)
{
	m_pAPIController = CAPIController::GetInstance();
}

CSSettingTab::~CSSettingTab()
{
}

void CSSettingTab::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CSSettingTab, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTONSUP, &CSSettingTab::OnBnClickedButtonsup)
	ON_BN_CLICKED(IDC_BUTTONSDOWN, &CSSettingTab::OnBnClickedButtonsdown)
	ON_BN_CLICKED(IDC_BUTTONSLEFT, &CSSettingTab::OnBnClickedButtonsleft)
	ON_BN_CLICKED(IDC_BUTTONSRIGHT, &CSSettingTab::OnBnClickedButtonsright)
	ON_BN_CLICKED(IDC_BUTTONSStop, &CSSettingTab::OnBnClickedButtonsstop)
	ON_BN_CLICKED(IDC_BUTTONSZoomIn, &CSSettingTab::OnBnClickedButtonszoomin)
	ON_BN_CLICKED(IDC_BUTTONSZoomOut, &CSSettingTab::OnBnClickedButtonszoomout)
END_MESSAGE_MAP()


// CSSettingTab message handlers


void CSSettingTab::OnBnClickedButtonsup()
{
	if(m_pAPIController)
	{
		m_pAPIController->StudentPTZUp();
	}
	return;
}


void CSSettingTab::OnBnClickedButtonsdown()
{
	if(m_pAPIController)
	{
		m_pAPIController->StudentPTZDown();
	}
	return;
}


void CSSettingTab::OnBnClickedButtonsleft()
{
	if(m_pAPIController)
	{
		m_pAPIController->StudentPTZLeft();
	}
	return;
}


void CSSettingTab::OnBnClickedButtonsright()
{
	if(m_pAPIController)
	{
		m_pAPIController->StudentPTZRight();
	}
	return;
}


void CSSettingTab::OnBnClickedButtonsstop()
{
	if(m_pAPIController)
	{
		m_pAPIController->StudentPTZStop();
	}
	return;
}


void CSSettingTab::OnBnClickedButtonszoomin()
{
	if(m_pAPIController)
	{
		m_pAPIController->StudentPTZZoomIn();
	}
	return;
}


void CSSettingTab::OnBnClickedButtonszoomout()
{
	if(m_pAPIController)
	{
		m_pAPIController->StudentPTZZoomOut();
	}
	return;
}
