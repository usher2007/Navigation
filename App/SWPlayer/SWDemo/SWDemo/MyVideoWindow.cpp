// MyVideoWindow.cpp : implementation file
//

#include "stdafx.h"
#include "MyVideoWindow.h"


// MyVideoWindow

IMPLEMENT_DYNAMIC(MyVideoWindow, CStatic)

	MyVideoWindow::MyVideoWindow()
{

}

MyVideoWindow::~MyVideoWindow()
{
}


BEGIN_MESSAGE_MAP(MyVideoWindow, CStatic)
	ON_WM_GETMINMAXINFO()
END_MESSAGE_MAP()



// MyVideoWindow message handlers




void MyVideoWindow::OnGetMinMaxInfo(MINMAXINFO* lpMMI)
{
	// TODO: Add your message handler code here and/or call default
	lpMMI->ptMaxSize.x = this->mVideoWidth;
	lpMMI->ptMaxSize.y = this->mVideoHeight;
	lpMMI->ptMaxTrackSize.x = this->mVideoWidth;
	lpMMI->ptMaxTrackSize.y = this->mVideoHeight;


	CStatic::OnGetMinMaxInfo(lpMMI);
}

void MyVideoWindow::SetVideoSize( int videoWidth, int videoHeight )
{
	this->mVideoWidth = videoWidth;
	this->mVideoHeight = videoHeight;
}
