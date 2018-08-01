#if !defined(AFX_PAGETCPTABLE_H__65100AB8_02B6_4A7A_A96F_430435D4379D__INCLUDED_)
#define AFX_PAGETCPTABLE_H__65100AB8_02B6_4A7A_A96F_430435D4379D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PageTcpTable.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPageTcpTable window

class CPageTcpTable : public CWnd
{
	DECLARE_DYNCREATE(CPageTcpTable)
// Construction
public:
	CPageTcpTable();

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
	//{{AFX_VIRTUAL(CPageTcpTable)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CPageTcpTable();

	// Generated message map functions
protected:
	//{{AFX_MSG(CPageTcpTable)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PAGETCPTABLE_H__65100AB8_02B6_4A7A_A96F_430435D4379D__INCLUDED_)
