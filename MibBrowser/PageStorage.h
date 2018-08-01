#if !defined(AFX_PAGESTORAGE_H__43DBE4DC_781B_44D2_A41F_53C9576E9009__INCLUDED_)
#define AFX_PAGESTORAGE_H__43DBE4DC_781B_44D2_A41F_53C9576E9009__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PageStorage.h : header file
//
#include "EditEx.h"
/////////////////////////////////////////////////////////////////////////////
// CPageStorage window

class CPageStorage : public CWnd
{
	DECLARE_DYNCREATE(CPageStorage)
// Construction
public:
	CPageStorage();

// Attributes
public:

// Operations
public:

protected:
	CFont *font;
	CListCtrl m_ctrProc;
	CEditEx m_ctrlMemSize;
	CButton m_ctrlRefesh;

	void DrawLabel(CDC *pDC, CRect &rect, CString szText);
	LRESULT OnInitPage(WPARAM wParam, LPARAM lParam);
	void OnRefresh();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPageStorage)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CPageStorage();

	// Generated message map functions
protected:
	//{{AFX_MSG(CPageStorage)
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PAGESTORAGE_H__43DBE4DC_781B_44D2_A41F_53C9576E9009__INCLUDED_)
