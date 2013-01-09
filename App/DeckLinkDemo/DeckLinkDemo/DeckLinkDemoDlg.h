
// DeckLinkDemoDlg.h : header file
//

#pragma once
#include "afxwin.h"
#include "..\TMGraph\TMGraph.h"

// CDeckLinkDemoDlg dialog
class CDeckLinkDemoDlg : public CDialogEx
{
// Construction
public:
	CDeckLinkDemoDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_DECKLINKDEMO_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation

//User Defined
private:
	CTMGraph *pGraph;
	int currentWidth;
	int currentHeight;
private:
	CString getCStringFromCEdit(CEdit *pCEdit);
	void setDefaultParam();
	void resizePlayWindow();
	void populateInputFormat();
	HRESULT createGraph(HWND graphOwner);
	HRESULT destroyGraph();


protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CEdit m_ctrlEditUrl;
	CComboBox m_ctrlSelInputFormat;
	CButton m_ctrlCheckPreview;
	afx_msg void OnBnClickedButtonstart();
	afx_msg void OnBnClickedButtonstop();
	CStatic m_ctrlVideoWindow;
};

class InputMediaType
{
public:
	int width;
	int height;
	double fps;
};
