#pragma once


// CTabSheet
const static int MAXPAGE = 5;
class CTabSheet : public CTabCtrl
{
	DECLARE_DYNAMIC(CTabSheet)

public:
	CTabSheet();
	virtual ~CTabSheet();

public:
	BOOL AddPage(LPCTSTR title, CDialog *pDialog, UINT ID);
	void Show();
	void SetRect();

private:
	CDialog* m_pPages[MAXPAGE];
	UINT m_IID[MAXPAGE];
	LPCTSTR m_titles[MAXPAGE];
	int m_nNumofPages;
	int m_nCurrentPage;

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
};


