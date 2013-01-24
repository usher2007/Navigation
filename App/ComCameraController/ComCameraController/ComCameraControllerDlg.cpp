
// ComCameraControllerDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ComCameraController.h"
#include "ComCameraControllerDlg.h"
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


// CComCameraControllerDlg dialog




CComCameraControllerDlg::CComCameraControllerDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CComCameraControllerDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CComCameraControllerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDITComNum, m_ctrlEditComNum);
	DDX_Control(pDX, IDC_EDITPaultRate, m_ctrlEditBaudRate);
	DDX_Control(pDX, IDC_EDITCommand, m_ctrlEditCommand);
}

BEGIN_MESSAGE_MAP(CComCameraControllerDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTONApply, &CComCameraControllerDlg::OnBnClickedButtonapply)
	ON_BN_CLICKED(IDC_BUTTONSend, &CComCameraControllerDlg::OnBnClickedButtonsend)
	ON_BN_CLICKED(IDC_BUTTONStop, &CComCameraControllerDlg::OnBnClickedButtonstop)
END_MESSAGE_MAP()


// CComCameraControllerDlg message handlers

BOOL CComCameraControllerDlg::OnInitDialog()
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
	m_ctrlEditComNum.SetWindowTextW(L"1");
	m_ctrlEditBaudRate.SetWindowTextW(L"9600");
	m_ctrlEditCommand.SetWindowTextW(L"FF010000000001");
	m_hSeries = NULL;
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CComCameraControllerDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CComCameraControllerDlg::OnPaint()
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
HCURSOR CComCameraControllerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CComCameraControllerDlg::OnBnClickedButtonapply()
{
	initSeries();
}


void CComCameraControllerDlg::OnBnClickedButtonsend()
{
	CString command = getCStringFromCEdit(&m_ctrlEditCommand);
	sendCStringToComm(command);
}


void CComCameraControllerDlg::OnBnClickedButtonstop()
{
	// TODO: Add your control notification handler code here
	sendCStringToComm(StopCode);
}


int CComCameraControllerDlg::getIntFromCEdit(CEdit *cedit)
{
	int num;
	CString param;
	param = getCStringFromCEdit(cedit);
	num = _ttoi(param);
	return num;
}

CString CComCameraControllerDlg::getCStringFromCEdit(CEdit *cedit)
{
	int paramLen;
	CString param;
	paramLen = cedit->LineLength(0);
	cedit->GetLine(0, param.GetBuffer(paramLen), paramLen);
	param.ReleaseBuffer(paramLen);
	return param;
}

int CComCameraControllerDlg::initSeries()
{
	CString com("COM");
	CString comNum = getCStringFromCEdit(&m_ctrlEditComNum);
	com.Append(comNum);

	m_hSeries = CreateFile(com, GENERIC_READ|GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);

	if(m_hSeries == INVALID_HANDLE_VALUE)
	{
		::MessageBoxA(NULL, "¶Ë¿Ú´ò¿ªÊ§°Ü", NULL,NULL);
		return -1;
	}

	SetupComm(m_hSeries, 1024, 1024);

	DCB dcb;
	GetCommState(m_hSeries, &dcb);
	int baudRate = getIntFromCEdit(&m_ctrlEditBaudRate);
	dcb.BaudRate = baudRate;
	dcb.fBinary = TRUE;
	dcb.fParity = FALSE;
	dcb.ByteSize = 8;
	dcb.Parity = NOPARITY;
	dcb.StopBits = ONESTOPBIT;

	if(!SetCommState(m_hSeries, &dcb))
	{
		::MessageBoxA(NULL, "×´Ì¬ÉèÖÃÊ§°Ü", NULL, NULL);
		return -1;
	}

	if(!SetCommMask(m_hSeries, EV_RXCHAR | EV_TXEMPTY))
	{
		::MessageBoxA(NULL, "ÑÚÂëÉèÖÃÊ§°Ü", NULL, NULL);
		return -1;
	}

	COMMTIMEOUTS CommTimeouts;
	CommTimeouts.ReadIntervalTimeout = MAXDWORD;
	CommTimeouts.ReadTotalTimeoutMultiplier = 0;
	CommTimeouts.ReadTotalTimeoutConstant = 0;
	CommTimeouts.WriteTotalTimeoutMultiplier = 100;
	CommTimeouts.WriteTotalTimeoutConstant = 500;
	SetCommTimeouts(m_hSeries, &CommTimeouts);
	PurgeComm(m_hSeries, PURGE_RXCLEAR | PURGE_TXCLEAR);
	DWORD pram, ThreadId;
	if(CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)ThreadFunction,this,0,&ThreadId) == INVALID_HANDLE_VALUE)
	{
		::MessageBoxA(NULL, "Ïß³Ì´´½¨Ê§°Ü", NULL, NULL);
		return -1;
	}
	return 0;
}

int CComCameraControllerDlg::sendCStringToComm(const CString& command)
{
	unsigned char buffer[1024];
	CString commandCpy = command;
	commandCpy.Replace(_T(" "), _T(""));
	int len = commandCpy.GetLength();
	int i;
	for(i=0; i<len; i+=2)
	{
		char oneByteCommand[3];
		oneByteCommand[2] = '\0';
		oneByteCommand[0] = commandCpy[i];
		oneByteCommand[1] = commandCpy[i+1];
		int hexNum = strtol(oneByteCommand, NULL, 16);
		buffer[i/2] = (unsigned char)hexNum;
	}

	COMSTAT ComStat;
	DWORD dwErrorFlag;
	ClearCommError(m_hSeries, &dwErrorFlag, &ComStat);
	DWORD nWrite;
	if(!WriteFile(m_hSeries, buffer, i/2, &nWrite, NULL))
	{
		::MessageBoxA(NULL, "·¢ËÍÊ§°Ü", NULL, NULL);
	}
	FlushFileBuffers(m_hSeries);
	return 0;
}

DWORD CComCameraControllerDlg::ThreadFunction(CComCameraControllerDlg *msg)
{
	return 0;
}