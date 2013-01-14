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
END_MESSAGE_MAP()


// CTSettingTab message handlers


//void CTSettingTab::OnLButtonDown(UINT nFlags, CPoint point)
//{
//	// TODO: Add your message handler code here and/or call default
//
//	CDialog::OnLButtonDown(nFlags, point);
//	if(m_nCurrentPage != GetCurFocus())
//	{
//
//	}
//}
