// SSettingTab.cpp : implementation file
//

#include "stdafx.h"
#include "SmartClassroom.h"
#include "SSettingTab.h"
#include "afxdialogex.h"
#include "Utils.h"


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
	DDX_Control(pDX, IDC_EDITSPosId, m_ctrlEditSPosId);
	DDX_Control(pDX, IDC_EDITSRowNum, m_ctrlEditSRowNum);
	DDX_Control(pDX, IDC_EDITSColNum, m_ctrlEditColNum);
	DDX_Control(pDX, IDC_EDITStuDetectLine, m_ctrlEditSDetectLine);
	DDX_Control(pDX, IDC_EDITStuLeftBorder, m_ctrlEditSLeftBorder);
	DDX_Control(pDX, IDC_EDITStuRightBorder, m_ctrlEditSRightBorder);
	DDX_Control(pDX, IDC_EDITStuTotalRowNum, m_ctrlEditSTotalRowNum);
	DDX_Control(pDX, IDC_EDITStuTotalColNum, m_ctrlEditSTotalColNum);
}


BEGIN_MESSAGE_MAP(CSSettingTab, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTONSUP, &CSSettingTab::OnBnClickedButtonsup)
	ON_BN_CLICKED(IDC_BUTTONSDOWN, &CSSettingTab::OnBnClickedButtonsdown)
	ON_BN_CLICKED(IDC_BUTTONSLEFT, &CSSettingTab::OnBnClickedButtonsleft)
	ON_BN_CLICKED(IDC_BUTTONSRIGHT, &CSSettingTab::OnBnClickedButtonsright)
	ON_BN_CLICKED(IDC_BUTTONSStop, &CSSettingTab::OnBnClickedButtonsstop)
	ON_BN_CLICKED(IDC_BUTTONSZoomIn, &CSSettingTab::OnBnClickedButtonszoomin)
	ON_BN_CLICKED(IDC_BUTTONSZoomOut, &CSSettingTab::OnBnClickedButtonszoomout)
	ON_BN_CLICKED(IDC_BUTTONSRecall, &CSSettingTab::OnBnClickedButtonsrecall)
	ON_BN_CLICKED(IDC_BUTTONSClearPresetPos, &CSSettingTab::OnBnClickedButtonsclearpresetpos)
	ON_BN_CLICKED(IDC_BUTTONSSavePresetPos, &CSSettingTab::OnBnClickedButtonssavepresetpos)
	ON_BN_CLICKED(IDC_BUTTONSSaveParams, &CSSettingTab::OnBnClickedButtonssaveparams)
	ON_BN_CLICKED(IDC_BUTTONSStartDetect, &CSSettingTab::OnBnClickedButtonsstartdetect)
	ON_BN_CLICKED(IDC_BUTTONSStopDetect, &CSSettingTab::OnBnClickedButtonsstopdetect)
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


void CSSettingTab::OnBnClickedButtonsrecall()
{
	if(m_pAPIController)
	{
		int locId = getIntFromCEdit(&m_ctrlEditSPosId);
		m_pAPIController->StudentPTZRecallPrePos(locId);
	}
	return;
}


void CSSettingTab::OnBnClickedButtonsclearpresetpos()
{
	if(m_pAPIController)
	{
		m_pAPIController->StudentPTZClearPrePos();
	}
	return;
}


void CSSettingTab::OnBnClickedButtonssavepresetpos()
{
	if(m_pAPIController)
	{
		int locId = getIntFromCEdit(&m_ctrlEditSPosId);
		int rowNum = getIntFromCEdit(&m_ctrlEditSRowNum);
		int colNum = getIntFromCEdit(&m_ctrlEditColNum);
		m_pAPIController->StudentPTZSetPrePos(locId, rowNum, colNum);
	}
	return;
}


void CSSettingTab::OnBnClickedButtonssaveparams()
{
	if(m_pAPIController)
	{
		int leftBorder = getIntFromCEdit(&m_ctrlEditSLeftBorder);
		int rightBorder = getIntFromCEdit(&m_ctrlEditSRightBorder);
		int totalRowNum = getIntFromCEdit(&m_ctrlEditSTotalRowNum);
		int totalColNum = getIntFromCEdit(&m_ctrlEditSTotalColNum);
		int detectLine = getIntFromCEdit(&m_ctrlEditSDetectLine);
		m_pAPIController->StudentStandUpSetParams(leftBorder, rightBorder, totalRowNum, totalColNum, detectLine);
	}
	return;
}


void CSSettingTab::OnBnClickedButtonsstartdetect()
{
	if(m_pAPIController)
	{
		m_pAPIController->StudentStartDetect(true);
	}
	return;
}


void CSSettingTab::OnBnClickedButtonsstopdetect()
{
	if(m_pAPIController)
	{
		m_pAPIController->StudentStopDetect();
	}
	return;
}
