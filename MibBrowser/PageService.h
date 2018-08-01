#if !defined(AFX_PAGESERVICE_H__5E47230C_44BC_4EA0_A745_605F7EB365D3__INCLUDED_)
#define AFX_PAGESERVICE_H__5E47230C_44BC_4EA0_A745_605F7EB365D3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PageService.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPageService window

class CPageService : public CWnd
{
	DECLARE_DYNCREATE(CPageService)
// Construction
public:
	CPageService();

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
	//{{AFX_VIRTUAL(CPageService)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CPageService();

	// Generated message map functions
protected:
	//{{AFX_MSG(CPageService)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PAGESERVICE_H__5E47230C_44BC_4EA0_A745_605F7EB365D3__INCLUDED_)
