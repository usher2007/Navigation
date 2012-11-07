
// VideoCaptureDlg.cpp : implementation file
//

#include "stdafx.h"
#include "VideoCapture.h"
#include "VideoCaptureDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CVideoCaptureDlg dialog




CVideoCaptureDlg::CVideoCaptureDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CVideoCaptureDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CVideoCaptureDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LISTFILTERS, m_ctrlFilterList);
	DDX_Control(pDX, IDC_STATICVIDEOWINDOW, m_ctrlVideoWindow);
}

BEGIN_MESSAGE_MAP(CVideoCaptureDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTONPLAY, &CVideoCaptureDlg::OnBnClickedButtonplay)
	ON_BN_CLICKED(IDC_BUTTONSTOP, &CVideoCaptureDlg::OnBnClickedButtonstop)
END_MESSAGE_MAP()


// CVideoCaptureDlg message handlers

BOOL CVideoCaptureDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
	pCaptureGraph = new CaptureGraph();
	pCaptureGraph->Create(this->GetSafeHwnd());
	fillFilterListBox();
	isGraphBuilded = FALSE;
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CVideoCaptureDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CVideoCaptureDlg::OnPaint()
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
HCURSOR CVideoCaptureDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CVideoCaptureDlg::fillFilterListBox()
{
	HRESULT hr;
	m_ctrlFilterList.ResetContent();
	int maxFilterCount = 5;
	CString *captureFilterNames = new CString[maxFilterCount];
	hr = pCaptureGraph->GetCaptureFilters(&maxFilterCount, &m_ctrlFilterList);
	if(FAILED(hr))
		return;
	m_ctrlFilterList.SetCurSel(0);
	return;
}



void CVideoCaptureDlg::OnBnClickedButtonplay()
{
	// TODO: Add your control notification handler code here
	if(!isGraphBuilded)
	{
		int selectedIndex = 0;
		selectedIndex = m_ctrlFilterList.GetCurSel();
		pCaptureGraph->BuildGraphFromListBox(&m_ctrlFilterList, selectedIndex);
		pCaptureGraph->SetDisplayWindow(m_ctrlVideoWindow.GetSafeHwnd());
		isGraphBuilded = TRUE;
	}
	pCaptureGraph->Run();
}


void CVideoCaptureDlg::OnBnClickedButtonstop()
{
	// TODO: Add your control notification handler code here
	pCaptureGraph->Stop();
}
