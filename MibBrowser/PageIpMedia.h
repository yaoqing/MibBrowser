#if !defined(AFX_PAGEIPMEDIA_H__173B4E8D_C157_4A9C_82D0_D001EFE7E08E__INCLUDED_)
#define AFX_PAGEIPMEDIA_H__173B4E8D_C157_4A9C_82D0_D001EFE7E08E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PageIpMedia.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPageIpMedia window

class CPageIpMedia : public CWnd
{
	DECLARE_DYNCREATE(CPageIpMedia)
// Construction
public:
	CPageIpMedia();

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
	//{{AFX_VIRTUAL(CPageIpMedia)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CPageIpMedia();

	// Generated message map functions
protected:
	//{{AFX_MSG(CPageIpMedia)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PAGEIPMEDIA_H__173B4E8D_C157_4A9C_82D0_D001EFE7E08E__INCLUDED_)
