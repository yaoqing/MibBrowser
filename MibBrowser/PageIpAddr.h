#if !defined(AFX_PAGEIPADDR_H__0C0DD3BA_1122_4D2A_9836_262200FC2F69__INCLUDED_)
#define AFX_PAGEIPADDR_H__0C0DD3BA_1122_4D2A_9836_262200FC2F69__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PageIpAddr.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPageIpAddr window

class CPageIpAddr : public CWnd
{
	DECLARE_DYNCREATE(CPageIpAddr)
// Construction
public:
	CPageIpAddr();

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
	//{{AFX_VIRTUAL(CPageIpAddr)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CPageIpAddr();

	// Generated message map functions
protected:
	//{{AFX_MSG(CPageIpAddr)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PAGEIPADDR_H__0C0DD3BA_1122_4D2A_9836_262200FC2F69__INCLUDED_)
