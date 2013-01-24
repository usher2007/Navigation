// TabSheet.cpp : implementation file
//

#include "stdafx.h"
#include "SmartClassroom.h"
#include "TabSheet.h"


// CTabSheet

IMPLEMENT_DYNAMIC(CTabSheet, CTabCtrl)

CTabSheet::CTabSheet()
{
	m_nNumofPages = 0;
}

CTabSheet::~CTabSheet()
{
}


BEGIN_MESSAGE_MAP(CTabSheet, CTabCtrl)
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()



// CTabSheet message handlers

BOOL CTabSheet::AddPage(LPCTSTR title, CDialog *pDialog, UINT ID)
{
	if(MAXPAGE == m_nNumofPages)
	{
		return FALSE;
	}

	m_nNumofPages++;
	m_pPages[m_nNumofPages-1] = pDialog;
	m_IID[m_nNumofPages-1] = ID;
	m_titles[m_nNumofPages-1] = title;
	return TRUE;
}

void CTabSheet::Show()
{
	for(int i=0; i<m_nNumofPages; i++)
	{
		m_pPages[i]->Create(m_IID[i], this);
		InsertItem(i, m_titles[i]);
	}

	m_pPages[0]->ShowWindow(SW_SHOW);
	for(int i=1; i<m_nNumofPages; i++)
	{
		m_pPages[i]->ShowWindow(SW_HIDE);
	}
	m_nCurrentPage = 0;
	SetRect();
}

void CTabSheet::SetRect()
{
	CRect tabRect, itemRect;
	int nX, nY, nXc, nYc;

	GetClientRect(&tabRect);
	GetItemRect(0, &itemRect);

	nX = itemRect.left;
	nY = itemRect.bottom + 1;
	nXc = tabRect.right - itemRect.left - 2;
	nYc = tabRect.bottom - nY - 2;
	
	m_pPages[0]->SetWindowPos(&wndTop, nX, nY, nXc, nYc, SWP_SHOWWINDOW);
	for(int i=1; i<m_nNumofPages; i++)
	{
		m_pPages[i]->SetWindowPos(&wndTop, nX, nY, nXc, nYc, SWP_HIDEWINDOW);
	}
}

void CTabSheet::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default

	CTabCtrl::OnLButtonDown(nFlags, point);

	if(m_nCurrentPage != GetCurFocus())
	{
		m_pPages[m_nCurrentPage]->ShowWindow(SW_HIDE);
		m_nCurrentPage = GetCurFocus();
		m_pPages[m_nCurrentPage]->ShowWindow(SW_SHOW);
	}
}
