#if !defined(AFX_PAGETCPSTAT_H__4C7C7305_B9A6_4BDC_8260_ABA86EFFB33A__INCLUDED_)
#define AFX_PAGETCPSTAT_H__4C7C7305_B9A6_4BDC_8260_ABA86EFFB33A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PageTcpStat.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPageTcpStat window

class CPageTcpStat : public CWnd
{
	DECLARE_DYNCREATE(CPageTcpStat)
// Construction
public:
	CPageTcpStat();

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
	//{{AFX_VIRTUAL(CPageTcpStat)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CPageTcpStat();

	// Generated message map functions
protected:
	//{{AFX_MSG(CPageTcpStat)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PAGETCPSTAT_H__4C7C7305_B9A6_4BDC_8260_ABA86EFFB33A__INCLUDED_)
