#if !defined(AFX_PAGEPROC_H__64957BF2_3F81_4713_996F_EEDDEF738416__INCLUDED_)
#define AFX_PAGEPROC_H__64957BF2_3F81_4713_996F_EEDDEF738416__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PageProc.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPageProc window

class CPageProc : public CWnd
{
	DECLARE_DYNCREATE(CPageProc)
// Construction
public:
	CPageProc();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPageProc)
	protected:
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CPageProc();

protected:
	CFont *font;
	CListCtrl m_ctrProc;
	CButton m_ctrlRefesh;
	
	LRESULT OnInitPage(WPARAM wParam, LPARAM lParam);
	void OnRefresh();

	// Generated message map functions
protected:
	//{{AFX_MSG(CPageProc)
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PAGEPROC_H__64957BF2_3F81_4713_996F_EEDDEF738416__INCLUDED_)
