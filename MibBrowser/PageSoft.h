#if !defined(AFX_PAGESOFT_H__54440D1D_3338_40A4_8578_408926FB447E__INCLUDED_)
#define AFX_PAGESOFT_H__54440D1D_3338_40A4_8578_408926FB447E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PageSoft.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPageSoft window

class CPageSoft : public CWnd
{
	DECLARE_DYNCREATE(CPageSoft)
// Construction
public:
	CPageSoft();

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
	//{{AFX_VIRTUAL(CPageSoft)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CPageSoft();

	// Generated message map functions
protected:
	//{{AFX_MSG(CPageSoft)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PAGESOFT_H__54440D1D_3338_40A4_8578_408926FB447E__INCLUDED_)
