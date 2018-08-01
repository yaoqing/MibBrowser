#if !defined(AFX_PAGEIPROUTE_H__BADC7198_6A8E_46C6_A336_37A89AC14B9B__INCLUDED_)
#define AFX_PAGEIPROUTE_H__BADC7198_6A8E_46C6_A336_37A89AC14B9B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PageIpRoute.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPageIpRoute window

class CPageIpRoute : public CWnd
{
	DECLARE_DYNCREATE(CPageIpRoute)
// Construction
public:
	CPageIpRoute();

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
	//{{AFX_VIRTUAL(CPageIpRoute)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CPageIpRoute();

	// Generated message map functions
protected:
	//{{AFX_MSG(CPageIpRoute)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PAGEIPROUTE_H__BADC7198_6A8E_46C6_A336_37A89AC14B9B__INCLUDED_)
