// CapVideoWindow.cpp : implementation file
//

#include "stdafx.h"
#include "VideoCapture.h"
#include "CapVideoWindow.h"
#include "afxdialogex.h"


// CapVideoWindow dialog

IMPLEMENT_DYNAMIC(CapVideoWindow, CDialogEx)

CapVideoWindow::CapVideoWindow()
{

}

CapVideoWindow::~CapVideoWindow()
{
}



BEGIN_MESSAGE_MAP(CapVideoWindow, CStatic)
	ON_WM_GETMINMAXINFO()
END_MESSAGE_MAP()



// MyVideoWindow message handlers




void CapVideoWindow::OnGetMinMaxInfo(MINMAXINFO* lpMMI)
{
	// TODO: Add your message handler code here and/or call default
	lpMMI->ptMaxSize.x = this->mVideoWidth;
	lpMMI->ptMaxSize.y = this->mVideoHeight;
	lpMMI->ptMaxTrackSize.x = this->mVideoWidth;
	lpMMI->ptMaxTrackSize.y = this->mVideoHeight;


	CStatic::OnGetMinMaxInfo(lpMMI);
}

void CapVideoWindow::SetVideoSize( int videoWidth, int videoHeight )
{
	this->mVideoWidth = videoWidth;
	this->mVideoHeight = videoHeight;
}
