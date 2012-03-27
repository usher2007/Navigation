
// SWDemoDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SWDemo.h"
#include "SWDemoDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CSWDemoDlg dialog




CSWDemoDlg::CSWDemoDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CSWDemoDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CSWDemoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATICVIDEOWINDOW, m_VideoCtrl);
	DDX_Control(pDX, IDC_EDIT_HOST, m_hostCtrl);
	DDX_Control(pDX, IDC_EDIT_USERNAME, m_userNameCtrl);
	DDX_Control(pDX, IDC_EDIT_PASSWORD, m_passwordCtrl);
}

BEGIN_MESSAGE_MAP(CSWDemoDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_ERASEBKGND()
	ON_BN_CLICKED(IDC_BUTTON_PLAY, &CSWDemoDlg::OnBnClickedButtonPlay)
	ON_BN_CLICKED(IDC_BUTTON_STOP, &CSWDemoDlg::OnBnClickedButtonStop)
	ON_BN_CLICKED(IDC_BUTTON_RESOLUTION, &CSWDemoDlg::OnBnClickedButtonResolution)
	ON_BN_CLICKED(IDC_BUTTON_RECORD, &CSWDemoDlg::OnBnClickedButtonRecord)
	ON_BN_CLICKED(IDC_BUTTON_SNIPPING, &CSWDemoDlg::OnBnClickedButtonSnipping)
	ON_BN_CLICKED(IDC_BUTTON_LOGIN, &CSWDemoDlg::OnBnClickedButtonLogin)
	ON_BN_CLICKED(IDC_BUTTON_LOGOUT, &CSWDemoDlg::OnBnClickedButtonLogout)
END_MESSAGE_MAP()


// CSWDemoDlg message handlers

BOOL CSWDemoDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
	m_hostCtrl.SetWindowText(L"192.168.1.17");
	m_userNameCtrl.SetWindowText(L"admin");
	m_passwordCtrl.SetWindowText(L"888888");
	m_bRecord = FALSE;
	m_bPlay = FALSE;
	GetDlgItem(IDC_BUTTON_STOP)->EnableWindow(FALSE);
	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CSWDemoDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CSWDemoDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

BOOL CSWDemoDlg::OnEraseBkgnd(CDC* pDC)
{
	CRect rc;
	m_VideoCtrl.GetWindowRect(&rc);
	ScreenToClient(&rc);
	pDC->ExcludeClipRect(&rc);
	return CDialog::OnEraseBkgnd(pDC);
}

LRESULT CSWDemoDlg::OnGraphNotify(WPARAM inWParam, LPARAM inLParam)
{
	IMediaEventEx *pEvent = NULL;
	if (mFilterGraph && (pEvent = mFilterGraph->GetEventHandle()))
	{
		LONG eventCode = 0, eventParam1 = 0, eventParam2 = 0;
		while (SUCCEEDED(pEvent->GetEvent(&eventCode, &eventParam1, &eventParam2, 0)))
		{
			pEvent->FreeEventParams(eventCode, eventParam1, eventParam2);
			switch (eventCode)
			{
			case EC_COMPLETE:
				//OnBnClickedBtnpause();
				mFilterGraph->SetCurrentPosition(0);
				break;
			case EC_USERABORT:
			case EC_ERRORABORT:
				//OnBnClickedBtnstop();
				break;
			default:
				break;
			}
		}
	}
	return 0;
}

void CSWDemoDlg::DestroyGraph(void)
{
	if (mFilterGraph)
	{
		mFilterGraph->Stop();
		mFilterGraph->SetNotifyWindow(NULL);

		delete mFilterGraph;
		mFilterGraph = NULL;
	}
}

void CSWDemoDlg::CreateGraph(void)
{
	if (SUCCEEDED(mFilterGraph->Create(this->GetSafeHwnd())))
	{
		mFilterGraph->RenderFile(mSourceFile);
		mFilterGraph->SetDisplayWindow(m_VideoCtrl.GetSafeHwnd());
		mFilterGraph->SetNotifyWindow(this->GetSafeHwnd());
		mFilterGraph->Stop();
		mFilterGraph->Pause();
		mFilterGraph->SetCurrentPosition(0.0);
	}
}


void CSWDemoDlg::OnBnClickedButtonPlay()
{
	if(mFilterGraph && !m_bPlay)
	{
		OAFilterState fs;
		mFilterGraph->GetState(&fs);
		if(fs == State_Running)
		{
			//m_ctrlEditRate.SetText(1.0);
		}
		mFilterGraph->Run();
		m_bPlay = TRUE;
		GetDlgItem(IDC_BUTTON_PLAY)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_STOP)->EnableWindow(TRUE);
	}
}


void CSWDemoDlg::OnBnClickedButtonStop()
{
	if(mFilterGraph && m_bPlay)
	{
		mFilterGraph->Stop();
		m_bPlay = FALSE;
		GetDlgItem(IDC_BUTTON_STOP)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_PLAY)->EnableWindow(TRUE);
	}
}


void CSWDemoDlg::OnBnClickedButtonResolution()
{
	// TODO: Add your control notification handler code here
}


void CSWDemoDlg::OnBnClickedButtonRecord()
{
	m_bRecord = !m_bRecord;
	if (m_bRecord)
	{
		//TODO: Add the logic to record the video.
		CFileDialog dlg(FALSE,_T("*.h264"), _T("example.h264"), OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, _T("RECORD Files(*.h264)|*.h264||"));
		if(dlg.DoModal()==IDOK){	
			USES_CONVERSION;
			mFilterGraph->SetRecordFile(T2A((LPTSTR)(LPCTSTR)dlg.GetPathName()));
			mFilterGraph->StartRecord();
			GetDlgItem(IDC_BUTTON_RECORD)->SetWindowText(L"Í£Ö¹Â¼Ïñ(²»²¥·ÅÊÓÆµ£©");
		}
		
	} 
	else
	{
		//TODO: Add the logic to stop recording the video.
		mFilterGraph->StopRecord();
		GetDlgItem(IDC_BUTTON_RECORD)->SetWindowText(L"Â¼Ïñ(ÏÈ°´²¥·Å¼ü)");
	}
}


void CSWDemoDlg::OnBnClickedButtonSnipping()
{
	OAFilterState pfs = 2;
	if(mFilterGraph)
	{
		mFilterGraph->GetState(&pfs);
		

		CFileDialog dlg(FALSE,_T("*.bmp"), _T("example.bmp"), OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, _T("BMP Files(*.bmp)|*.bmp||"));
		if(dlg.DoModal()==IDOK){		
			mFilterGraph->Grab(dlg.GetPathName());
		}
		if(pfs == 0)
			mFilterGraph->Stop();
		else if(pfs == 1)
			mFilterGraph->Pause();
		else if (pfs == 2)
			mFilterGraph->Run();
	}
}


void CSWDemoDlg::OnBnClickedButtonLogin()
{
	// TODO: Add your control notification handler code here
	USES_CONVERSION;
	CString userName;
	int len = m_userNameCtrl.LineLength(0);
	m_userNameCtrl.GetLine(0,userName.GetBuffer(len), len);
	userName.ReleaseBuffer(len);
	CString passwd;
	len = m_passwordCtrl.LineLength(0);
	m_passwordCtrl.GetLine(0,passwd.GetBuffer(len),len);
	passwd.ReleaseBuffer(len);
	CString addr;
	len = m_hostCtrl.LineLength(0);
	m_hostCtrl.GetLine(0,addr.GetBuffer(len),len);
	addr.ReleaseBuffer(len);
	mSourceFile.Format(L"netdvr://%s:%s@%s:12345", userName,passwd,addr);
	mFilterGraph = new CDXGraph();
	CreateGraph();
	m_bPlay = FALSE;
	GetDlgItem(IDC_BUTTON_STOP)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_PLAY)->EnableWindow(TRUE);
}


void CSWDemoDlg::OnBnClickedButtonLogout()
{
	// TODO: Add your control notification handler code here
}
