#if !defined(AFX_PAGESNMPSTAT_H__26A1E4EE_42B1_4395_B97B_C44575028A49__INCLUDED_)
#define AFX_PAGESNMPSTAT_H__26A1E4EE_42B1_4395_B97B_C44575028A49__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PageSnmpStat.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPageSnmpStat window

class CPageSnmpStat : public CWnd
{
	DECLARE_DYNCREATE(CPageSnmpStat)
// Construction
public:
	CPageSnmpStat();

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
	//{{AFX_VIRTUAL(CPageSnmpStat)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CPageSnmpStat();

	// Generated message map functions
protected:
	//{{AFX_MSG(CPageSnmpStat)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PAGESNMPSTAT_H__26A1E4EE_42B1_4395_B97B_C44575028A49__INCLUDED_)
