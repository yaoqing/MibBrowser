#if !defined(AFX_PAGEPERFORMANCE_H__E4EDF8C8_1E72_4D83_8422_8CCA97E2966E__INCLUDED_)
#define AFX_PAGEPERFORMANCE_H__E4EDF8C8_1E72_4D83_8422_8CCA97E2966E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PagePerformance.h : header file
//
#include "BGChartWnd.h"
#include "PercentCharWnd.h"
/////////////////////////////////////////////////////////////////////////////
// CPagePerformance window

class CPagePerformance : public CWnd
{
	DECLARE_DYNCREATE(CPagePerformance)
// Construction
public:
	CPagePerformance();

// Attributes
public:

// Operations
public:

protected:
	CFont *font;
	CButton m_wnd1, m_wnd2, m_wnd3, m_wnd4;
	CPercentCharWnd m_wndCpuRate, m_wndPfRate;
	CBGChartWnd m_wndCpuRecord, m_wndPageRecord;

	LRESULT OnInitPage(WPARAM wParam, LPARAM lParam);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPagePerformance)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CPagePerformance();

	// Generated message map functions
protected:
	//{{AFX_MSG(CPagePerformance)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PAGEPERFORMANCE_H__E4EDF8C8_1E72_4D83_8422_8CCA97E2966E__INCLUDED_)
