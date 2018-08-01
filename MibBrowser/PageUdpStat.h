#if !defined(AFX_PAGEUDPSTAT_H__4A6232D7_DB63_4362_B72C_B436B1A41DA2__INCLUDED_)
#define AFX_PAGEUDPSTAT_H__4A6232D7_DB63_4362_B72C_B436B1A41DA2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PageUdpStat.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPageUdpStat window

class CPageUdpStat : public CWnd
{
	DECLARE_DYNCREATE(CPageUdpStat)
// Construction
public:
	CPageUdpStat();

// Attributes
public:

// Operations
public:

protected:
	CFont *font;
	CListCtrl m_ctrProc;
	CButton m_ctrlRefesh;

	LRESULT OnInitPage(WPARAM wParam, LPARAM lParam);
	void OnRefresh();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPageUdpStat)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CPageUdpStat();

	// Generated message map functions
protected:
	//{{AFX_MSG(CPageUdpStat)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PAGEUDPSTAT_H__4A6232D7_DB63_4362_B72C_B436B1A41DA2__INCLUDED_)
