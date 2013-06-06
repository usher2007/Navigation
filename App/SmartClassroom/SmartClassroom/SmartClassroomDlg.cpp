
// SmartClassroomDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SmartClassroom.h"
#include "SmartClassroomDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CSmartClassroomDlg dialog




CSmartClassroomDlg::CSmartClassroomDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CSmartClassroomDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CSmartClassroomDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB3, m_tabSheet);
}

BEGIN_MESSAGE_MAP(CSmartClassroomDlg, CDialogEx)
	ON_WM_CLOSE()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
//	ON_WM_LBUTTONDOWN()
	ON_STN_DBLCLK(IDC_STATICPlayWndTea, &CSmartClassroomDlg::OnStnDblclickStaticplaywndtea)
END_MESSAGE_MAP()


// CSmartClassroomDlg message handlers

BOOL CSmartClassroomDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
	m_pAPIController = CAPIController::GetInstance();
	m_tabSheet.AddPage(L"教师", &m_TSettingPage, IDD_DIALOGTSetting);
	m_tabSheet.AddPage(L"学生", &m_SSettingPage, IDD_DIALOGSSetting);
	/*m_tabSheet.AddPage(L"激光笔", &m_LSettingPage, IDD_DIALOGLSetting);*/
	m_tabSheet.Show();

	m_pAPIController->AddTeaCamera(1);
	m_pAPIController->BuildTeacherGraph(TRUE, GetDlgItem(IDC_STATICPlayWndTea)->GetSafeHwnd(), this->GetSafeHwnd());
	m_pAPIController->BuildTeacherPTZGraph(TRUE, GetDlgItem(IDC_STATICPlayWndTeaPTZ)->GetSafeHwnd(), this->GetSafeHwnd());
	m_pAPIController->TeacherGraphRun();
	m_pAPIController->TeacherPTZGraphRun();

	// Student Part
	m_pAPIController->AddStuCamera(5);
	m_pAPIController->BuildStudentGraph(TRUE, GetDlgItem(IDC_STUONE)->GetSafeHwnd(), this->GetSafeHwnd(), 1);
	m_pAPIController->BuildStudentGraph(TRUE, GetDlgItem(IDC_STUTWO)->GetSafeHwnd(), this->GetSafeHwnd(), 2);
	m_pAPIController->StudentGraphRun(1);
	m_pAPIController->StudentGraphRun(2);
	

	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CSmartClassroomDlg::OnPaint()
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
HCURSOR CSmartClassroomDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CSmartClassroomDlg::OnClose()
{
	if(m_pAPIController != NULL)
	{
		m_pAPIController->DumpConfiguration();
		delete m_pAPIController;
	}
	CDialogEx::OnClose();
	return;
}

void CSmartClassroomDlg::OnStnDblclickStaticplaywndtea()
{
	if(m_TSettingPage.IsSettingBlindZone())
	{
		CPoint pt;
		GetCursorPos(&pt);
		CRect rect;
		GetDlgItem(IDC_STATICPlayWndTea)->GetWindowRect(&rect);
		int x = pt.x - rect.left;
		int y = pt.y - rect.top;
		int width = rect.right - rect.left;
		int height = rect.bottom - rect.top;
		int xPix = x*720/width;
		int yPix = y*576/height;
		if(m_pAPIController != NULL)
		{
			HRESULT hr = m_TSettingPage.CacheBlindZoneVertex(xPix, yPix);
			if(SUCCEEDED(hr))
			{
				m_pAPIController->TeacherCachePointToShow(xPix, yPix);
			}
		}
	}
}
