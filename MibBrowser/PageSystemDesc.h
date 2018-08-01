#if !defined(AFX_PAGESYSTEMDESC_H__14D86501_C5DF_4E1B_89CA_52402D4A4391__INCLUDED_)
#define AFX_PAGESYSTEMDESC_H__14D86501_C5DF_4E1B_89CA_52402D4A4391__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PageSystemDesc.h : header file
//
#include "EditEx.h"
/////////////////////////////////////////////////////////////////////////////
// CPageSystemDesc window

class CPageSystemDesc : public CWnd
{
	DECLARE_DYNCREATE(CPageSystemDesc)
// Construction
public:
	CPageSystemDesc();

// Attributes
public:

// Operations
public:

protected:
	CFont *font;
	CEditEx m_ctrlDesc;
	CEditEx m_ctrlOID;
	CEditEx m_ctrlUpTime;
	CEditEx m_ctrlContact;
	CEditEx m_ctrlName;
	CEditEx m_ctrlLocation;
	CEditEx m_ctrlServices;

	CButton m_ctrlRefesh;

	void DrawLabel(CDC *pDC, CRect &rect, CString szText);
	void OnRefresh();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPageSystemDesc)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CPageSystemDesc();
	LRESULT OnInitPage(WPARAM wParam, LPARAM lParam);

	// Generated message map functions
protected:
	//{{AFX_MSG(CPageSystemDesc)
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PAGESYSTEMDESC_H__14D86501_C5DF_4E1B_89CA_52402D4A4391__INCLUDED_)
