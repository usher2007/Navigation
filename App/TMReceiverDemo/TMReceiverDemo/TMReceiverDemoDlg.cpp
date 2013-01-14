
// TMReceiverDemoDlg.cpp : implementation file
//

#include "stdafx.h"
#include "TMReceiverDemo.h"
#include "TMReceiverDemoDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CTMReceiverDemoDlg dialog

int CBBeforeDeocode(TMFrame* pFrame, void* arg);
int CBAfterDecode(TMFrame* pFrame, void* arg);

CTMReceiverDemoDlg::CTMReceiverDemoDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CTMReceiverDemoDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CTMReceiverDemoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDITHttp, m_ctrlEditHttp);
	DDX_Control(pDX, IDC_EDITRtsp, m_ctrlEditRtsp);
}

BEGIN_MESSAGE_MAP(CTMReceiverDemoDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTONOpenHttp, &CTMReceiverDemoDlg::OnBnClickedButtonopenhttp)
	ON_BN_CLICKED(IDC_BUTTONOpenRtsp, &CTMReceiverDemoDlg::OnBnClickedButtonopenrtsp)
	ON_BN_CLICKED(IDC_BUTTONPlay, &CTMReceiverDemoDlg::OnBnClickedButtonplay)
	ON_BN_CLICKED(IDC_BUTTONStop, &CTMReceiverDemoDlg::OnBnClickedButtonstop)
	ON_BN_CLICKED(IDC_BUTTONRecord, &CTMReceiverDemoDlg::OnBnClickedButtonrecord)
END_MESSAGE_MAP()


// CTMReceiverDemoDlg message handlers

BOOL CTMReceiverDemoDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
	m_rcvrHandle = TM_RecieverCreate();
	m_bRecording = FALSE;
	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CTMReceiverDemoDlg::OnPaint()
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
HCURSOR CTMReceiverDemoDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CTMReceiverDemoDlg::OnBnClickedButtonopenhttp()
{
	// TODO: Add your control notification handler code here
	doSomeSettings();
	USES_CONVERSION;
	CString httpUrl = getCStringFromCEdit(&m_ctrlEditHttp);
	TM_RecieverOpenHttpStream(m_rcvrHandle, NULL, T2A(httpUrl));
}


void CTMReceiverDemoDlg::OnBnClickedButtonopenrtsp()
{
	// TODO: Add your control notification handler code here
	TM_RecieverClose(m_rcvrHandle);
	doSomeSettings();
	USES_CONVERSION;
	CString rstpUrl = getCStringFromCEdit(&m_ctrlEditRtsp);
	int ret = TM_RecieverOpenRtspStream(m_rcvrHandle, NULL, T2A(rstpUrl));
	TM_RecieverEnableDisplay(m_rcvrHandle, GetDlgItem(IDC_STATICPlayWindow)->GetSafeHwnd());
}


void CTMReceiverDemoDlg::OnBnClickedButtonplay()
{
	// TODO: Add your control notification handler code here
	TM_RecieverStartDisplay(m_rcvrHandle);
}


void CTMReceiverDemoDlg::OnBnClickedButtonstop()
{
	// TODO: Add your control notification handler code here
	TM_RecieverStopDisplay(m_rcvrHandle);
}


void CTMReceiverDemoDlg::OnBnClickedButtonrecord()
{
	// TODO: Add your control notification handler code here
	if(m_bRecording)
	{
		TM_RecieverStopStorage(m_rcvrHandle);
		m_bRecording = FALSE;
		GetDlgItem(IDC_BUTTONRecord)->SetWindowTextW(L"Record");
	}
	else
	{
		TM_RecieverStartStorage(m_rcvrHandle);
		m_bRecording = TRUE;
		GetDlgItem(IDC_BUTTONRecord)->SetWindowTextW(L"StopRecord");
	}
}


CString CTMReceiverDemoDlg::getCStringFromCEdit( CEdit *ctrlEdit )
{
	int paramLen;
	CString param;
	paramLen = ctrlEdit->LineLength(0);
	ctrlEdit->GetLine(0, param.GetBuffer(paramLen), paramLen);
	param.ReleaseBuffer(paramLen);
	return param;
}

int CTMReceiverDemoDlg::doSomeSettings()
{
	TM_RecieverEnableDisplay(m_rcvrHandle, GetDlgItem(IDC_STATICPlayWindow)->GetSafeHwnd());
	TM_RecieverEnableStorage(m_rcvrHandle, "E:\\Example.mp4");
	TM_RecieverSetCallBackBeforeDecode(m_rcvrHandle, CBBeforeDeocode, (void *)this);
	TM_RecieverSetCallBackAfterDecode(m_rcvrHandle, CBAfterDecode, (void *)this);
	return 0;
}

int CBBeforeDeocode(TMFrame* pFrame, void* arg)
{
	return 0;
}

int CBAfterDecode(TMFrame* pFrame, void* arg)
{
	return 0;
}