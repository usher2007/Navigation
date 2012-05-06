
// SWDemoDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SWDemo.h"
#include "SWDemoDlg.h"
#include "afxdialogex.h"
#include <time.h>

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
	//ON_WM_TIMER()
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
	checkHLKey();
	SetTimer(1,5000,0);
	totalTime = 0;
	checkCount = 0;
	return TRUE;  // return TRUE  unless you set the focus to a control
}
void CSWDemoDlg::OnTimer(UINT_PTR nIDEvent)
{
	clock_t beginTime = clock();
	this->checkHLKey();
	clock_t endTime = clock();
	totalTime += (endTime - beginTime);
	checkCount ++;
	if (checkCount > 100)
	{
		double timePerCheck = (double)(totalTime) / checkCount;
	}
	CDialogEx::OnTimer(nIDEvent);
	return;
}

void CSWDemoDlg::checkHLKey()
{
	const hasp_feature_t feature = 26;

	hasp_handle_t handle = HASP_INVALID_HANDLE_VALUE;
	hasp_status_t status;

	/*unsigned char vendor_code[] =
		"AzIceaqfA1hX5wS+M8cGnYh5ceevUnOZIzJBbXFD6dgf3tBkb9cvUF/Tkd/iKu2fsg9wAysYKw7RMAsV"
		"vIp4KcXle/v1RaXrLVnNBJ2H2DmrbUMOZbQUFXe698qmJsqNpLXRA367xpZ54i8kC5DTXwDhfxWTOZrB"
		"rh5sRKHcoVLumztIQjgWh37AzmSd1bLOfUGI0xjAL9zJWO3fRaeB0NS2KlmoKaVT5Y04zZEc06waU2r6"
		"AU2Dc4uipJqJmObqKM+tfNKAS0rZr5IudRiC7pUwnmtaHRe5fgSI8M7yvypvm+13Wm4Gwd4VnYiZvSxf"
		"8ImN3ZOG9wEzfyMIlH2+rKPUVHI+igsqla0Wd9m7ZUR9vFotj1uYV0OzG7hX0+huN2E/IdgLDjbiapj1"
		"e2fKHrMmGFaIvI6xzzJIQJF9GiRZ7+0jNFLKSyzX/K3JAyFrIPObfwM+y+zAgE1sWcZ1YnuBhICyRHBh"
		"aJDKIZL8MywrEfB2yF+R3k9wFG1oN48gSLyfrfEKuB/qgNp+BeTruWUk0AwRE9XVMUuRbjpxa4YA67SK"
		"unFEgFGgUfHBeHJTivvUl0u4Dki1UKAT973P+nXy2O0u239If/kRpNUVhMg8kpk7s8i6Arp7l/705/bL"
		"Cx4kN5hHHSXIqkiG9tHdeNV8VYo5+72hgaCx3/uVoVLmtvxbOIvo120uTJbuLVTvT8KtsOlb3DxwUrwL"
		"zaEMoAQAFk6Q9bNipHxfkRQER4kR7IYTMzSoW5mxh3H9O8Ge5BqVeYMEW36q9wnOYfxOLNw6yQMf8f9s"
		"JN4KhZty02xm707S7VEfJJ1KNq7b5pP/3RjE0IKtB2gE6vAPRvRLzEohu0m7q1aUp8wAvSiqjZy7FLaT"
		"tLEApXYvLvz6PEJdj4TegCZugj7c8bIOEqLXmloZ6EgVnjQ7/ttys7VFITB3mazzFiyQuKf4J6+b/a/Y";*/

	unsigned char vendor_code[] =
		"AzIceaqfA1hX5wS+M8cGnYh5ceevUnOZIzJBbXFD6dgf3tBkb9cvUF/Tkd/iKu2fsg9wAysYKw7RMAsV"
		"vIp4KcXle/v1RaXrLVnNBJ2H2DmrbUMOZbQUFXe698qmJsqNpLXRA367xpZ54i8kC5DTXwDhfxWTOZrB"
		"rh5sRKHcoVLumztIQjgWh37AzmSd1bLOfUGI0xjAL9zJWO3fRaeB0NS2KlmoKaVT5Y04zZEc06waU2r6"
		"AU2Dc4uipJqJmObqKM+tfNKAS0rZr5IudRiC7pUwnmtaHRe5fgSI8M7yvypvm+13Wm4Gwd4VnYiZvSxf"
		"8ImN3ZOG9wEzfyMIlH2+rKPUVHI+igsqla0Wd9m7ZUR9vFotj1uYV0OzG7hX0+huN2E/IdgLDjbiapj1"
		"e2fKHrMmGFaIvI6xzzJIQJF9GiRZ7+0jNFLKSyzX/K3JAyFrIPObfwM+y+zAgE1sWcZ1YnuBhICyRHBh"
		"aJDKIZL8MywrEfB2yF+R3k9wFG1oN48gSLyfrfEKuB/qgNp+BeTruWUk0AwRE9XVMUuRbjpxa4YA67SK"
		"unFEgFGgUfHBeHJTivvUl0u4Dki1UKAT973P+nXy2O0u239If/kRpNUVhMg8kpk7s8i6Arp7l/705/bL"
		"Cx4kN5hHHSXIqkiG9tHdeNV8VYo5+72hgaCx3/uVoVLmtvxbOIvo120uTJbuLVTvT8KtsOlb3DxwUrwL"
		"zaEMoAQAFk6Q9bNipHxfkRQER4kR7IYTMzSoW5mxh3H9O8Ge5BqVeYMEW36q9wnOYfxOLNw6yQMf8f9s"
		"JN4KhZty02xm707S7VEfJJ1KNq7b5pP/3RjE0IKtB2gE6vAPRvRLzEohu0m7q1aUp8wAvSiqjZy7FLaT"
		"tLEApXYvLvz6PEJdj4TegCZugj7c8bIOEqLXmloZ6EgVnjQ7/ttys7VFITB3mazzFiyQuKf4J6+b/a/Y";
	status = hasp_login(feature, vendor_code, &handle);
	CString haspMsg;
	if (status != HASP_STATUS_OK)
	{
		switch (status)
		{
		case HASP_FEATURE_NOT_FOUND:
			haspMsg = "Feature not found!";
			break;
		case HASP_HASP_NOT_FOUND:
			haspMsg = "Required Sentinel HASP protection key not found!";
			break;
		case HASP_OLD_DRIVER:
			haspMsg = "Can't find request Feature!";
			break;
		case HASP_NO_DRIVER:
			haspMsg = "Driver not installed!";
			break;
		case HASP_INV_VCODE:
			haspMsg = "Invalid Vendor Code!";
			break;
		case HASP_FEATURE_TYPE_NOT_IMPL:
			haspMsg = "Requested Feature type not available!";
			break;
		case HASP_TMOF:
			haspMsg = "Too many open sessions!";
			break;
		case HASP_TS_DETECTED:
			haspMsg = "Program is running on a Terminal Server!";
			break;
		case HASP_FEATURE_EXPIRED:
			haspMsg = "Feature has been expired!";
			break;
		default:
			haspMsg = "Unknown Error!";
			break;
		}
		if(MessageBox(haspMsg, L"HASP Login Info", MB_OK) == IDOK)
		{
			this->DestroyWindow();
		}
	}
	return;
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
