#if !defined(AFX_PAGEINTERFACE_H__0AF15599_2654_4C44_A5D7_139C56867242__INCLUDED_)
#define AFX_PAGEINTERFACE_H__0AF15599_2654_4C44_A5D7_139C56867242__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PageInterface.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPageInterface window

class CPageInterface : public CWnd
{
	DECLARE_DYNCREATE(CPageInterface)
// Construction
public:
	CPageInterface();

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
	//{{AFX_VIRTUAL(CPageInterface)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CPageInterface();

	// Generated message map functions
protected:
	//{{AFX_MSG(CPageInterface)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PAGEINTERFACE_H__0AF15599_2654_4C44_A5D7_139C56867242__INCLUDED_)
