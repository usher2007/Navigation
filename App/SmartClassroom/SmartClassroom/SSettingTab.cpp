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

}

CSSettingTab::~CSSettingTab()
{
}

void CSSettingTab::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CSSettingTab, CDialogEx)
END_MESSAGE_MAP()


// CSSettingTab message handlers
