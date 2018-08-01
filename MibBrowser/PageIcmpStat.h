#if !defined(AFX_PAGEICMPSTAT_H__DCF1D1B1_24E3_47A2_813B_1A933EB9879B__INCLUDED_)
#define AFX_PAGEICMPSTAT_H__DCF1D1B1_24E3_47A2_813B_1A933EB9879B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PageIcmpStat.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPageIcmpStat window

class CPageIcmpStat : public CWnd
{
	DECLARE_DYNCREATE(CPageIcmpStat)
// Construction
public:
	CPageIcmpStat();

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
	//{{AFX_VIRTUAL(CPageIcmpStat)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CPageIcmpStat();

	// Generated message map functions
protected:
	//{{AFX_MSG(CPageIcmpStat)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PAGEICMPSTAT_H__DCF1D1B1_24E3_47A2_813B_1A933EB9879B__INCLUDED_)
