
// DeckLinkDemoDlg.cpp : implementation file
//

#include "stdafx.h"
#include "DeckLinkDemo.h"
#include "DeckLinkDemoDlg.h"
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


// CDeckLinkDemoDlg dialog




CDeckLinkDemoDlg::CDeckLinkDemoDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDeckLinkDemoDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CDeckLinkDemoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDITUrl, m_ctrlEditUrl);
	DDX_Control(pDX, IDC_COMBOInputFormat, m_ctrlSelInputFormat);
	DDX_Control(pDX, IDC_CHECKPreview, m_ctrlCheckPreview);
	DDX_Control(pDX, IDC_STATICVideoWindow, m_ctrlVideoWindow);
}

BEGIN_MESSAGE_MAP(CDeckLinkDemoDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTONStart, &CDeckLinkDemoDlg::OnBnClickedButtonstart)
	ON_BN_CLICKED(IDC_BUTTONStop, &CDeckLinkDemoDlg::OnBnClickedButtonstop)
	ON_BN_CLICKED(IDC_BUTTONOpenUrl, &CDeckLinkDemoDlg::OnBnClickedButtonopenurl)
END_MESSAGE_MAP()


// CDeckLinkDemoDlg message handlers

BOOL CDeckLinkDemoDlg::OnInitDialog()
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
	pGraph = new CTMGraph;
	populateInputFormat();
	setDefaultParam();
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CDeckLinkDemoDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CDeckLinkDemoDlg::OnPaint()
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
HCURSOR CDeckLinkDemoDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CDeckLinkDemoDlg::OnBnClickedButtonstart()
{
	// TODO: Add your control notification handler code here
	if(pGraph != NULL)
	{
		pGraph->Run();
	}
}


void CDeckLinkDemoDlg::OnBnClickedButtonstop()
{
	// TODO: Add your control notification handler code here
	if(pGraph != NULL)
	{
		pGraph->Stop();
	}
}

CString CDeckLinkDemoDlg::getCStringFromCEdit(CEdit *pCEdit)
{
	int paramLen;
	CString param;
	paramLen = pCEdit->LineLength(0);
	pCEdit->GetLine(0, param.GetBuffer(paramLen), paramLen);
	param.ReleaseBuffer(paramLen);
	return param;
}

void CDeckLinkDemoDlg::setDefaultParam()
{
	//m_ctrlEditUrl.SetWindowTextW(L"rtsp://192.168.0.236:8554/test-0-1");
	m_ctrlEditUrl.SetWindowTextW(L"RGB Source 32bit");
	m_ctrlSelInputFormat.SetCurSel(0);
	m_ctrlCheckPreview.SetCheck(1);
}

void CDeckLinkDemoDlg::populateInputFormat()
{
	InputMediaType *pInputMT = new InputMediaType;
	pInputMT->width = 720;
	pInputMT->height = 576;
	pInputMT->fps = 25;
	int n = m_ctrlSelInputFormat.AddString(L"720x576 25FPS");
	m_ctrlSelInputFormat.SetItemData(n, (DWORD_PTR)pInputMT);
}

HRESULT CDeckLinkDemoDlg::createGraph(HWND graphOwner)
{
	CString url = getCStringFromCEdit(&m_ctrlEditUrl);
	BOOL bDisplay = (BST_CHECKED == m_ctrlCheckPreview.GetCheck());
	HRESULT hr = S_FALSE;
	if(pGraph != NULL)
	{
		hr = pGraph->Destroy();
		if(FAILED(hr)) return hr;
		TCHAR* tmpUrl = url.GetBuffer();
		char ctrTmpUrl[256];
		WideCharToMultiByte(0, 0, tmpUrl, -1, ctrTmpUrl, 100, NULL, NULL);
		hr = pGraph->Create();
		if(FAILED(hr)) return hr;
		hr = pGraph->BuildFilterGraph(ctrTmpUrl, bDisplay);
		if(FAILED(hr)) return hr;
		hr = pGraph->SetNotifyWindow(graphOwner);
		if(FAILED(hr)) return hr;
		resizePlayWindow();
		return S_OK;
	}
	return E_FAIL;
}

void CDeckLinkDemoDlg::resizePlayWindow()
{
	BOOL bFixWidth = TRUE;

	int index = m_ctrlSelInputFormat.GetCurSel();
	InputMediaType *pInputMT = (InputMediaType *)m_ctrlSelInputFormat.GetItemData(index);
	int width = pInputMT->width;
	int height = pInputMT->height;

	LPRECT pVideoWindowRect = new tagRECT();
	m_ctrlVideoWindow.GetWindowRect(pVideoWindowRect);
	ScreenToClient(pVideoWindowRect);
	int fixedWidth = pVideoWindowRect->right - pVideoWindowRect->left;
	int fixedHeight = pVideoWindowRect->bottom - pVideoWindowRect->top;

	if((width * 1.0 / height) > (fixedWidth * 1.0 / fixedHeight))
	{
		bFixWidth = TRUE;
	}
	else
	{
		bFixWidth = FALSE;
	}

	int adjustedLen = bFixWidth ? fixedWidth * height / width : fixedHeight * width / height;
	int topX = bFixWidth ? pVideoWindowRect->left : (pVideoWindowRect->left + (fixedWidth - adjustedLen)/2);
	int topY = bFixWidth ? (pVideoWindowRect->top + (fixedHeight - adjustedLen)/2) : pVideoWindowRect->top;
	if(bFixWidth)
	{
		m_ctrlVideoWindow.MoveWindow(topX, topY, fixedWidth, adjustedLen, TRUE);
	}
	else
	{
		m_ctrlVideoWindow.MoveWindow(topX, topY, adjustedLen, fixedHeight, TRUE);
	}
	if(pGraph != NULL)
	{
		pGraph->SetDisplayWindow(m_ctrlVideoWindow.GetSafeHwnd());
	}
}


void CDeckLinkDemoDlg::OnBnClickedButtonopenurl()
{
	createGraph(this->GetSafeHwnd());
}
