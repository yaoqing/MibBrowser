#if !defined(AFX_PAGEIPSTAT_H__C1B2C720_47DD_490D_B77F_C23DC264FE66__INCLUDED_)
#define AFX_PAGEIPSTAT_H__C1B2C720_47DD_490D_B77F_C23DC264FE66__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PageIpStat.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPageIpStat window

class CPageIpStat : public CWnd
{
	DECLARE_DYNCREATE(CPageIpStat)
// Construction
public:
	CPageIpStat();

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
	//{{AFX_VIRTUAL(CPageIpStat)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CPageIpStat();

	// Generated message map functions
protected:
	//{{AFX_MSG(CPageIpStat)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PAGEIPSTAT_H__C1B2C720_47DD_490D_B77F_C23DC264FE66__INCLUDED_)
