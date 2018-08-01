#if !defined(AFX_REGISTERWND_H__EB8D7C05_F1E1_4D5B_B968_5ABEFA8C8271__INCLUDED_)
#define AFX_REGISTERWND_H__EB8D7C05_F1E1_4D5B_B968_5ABEFA8C8271__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// RegisterWnd.h : header file
//
#include "EditEx.h"
/////////////////////////////////////////////////////////////////////////////
// CRegisterWnd window

class CRegisterWnd : public CWnd
{
// Construction
public:
	CRegisterWnd();

// Attributes
public:

// Operations
public:

protected:
	CFont *font;
	void DrawLabel(CDC *pDC, CRect &rect, CString szText);
	CEditEx m_ctrlOwner;
	CEditEx m_ctrlSerial;
	CEditEx m_ctrlOrganization;
	CButton m_ctrlOk;
	CButton m_ctrlCancel;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRegisterWnd)
	protected:
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CRegisterWnd();

	// Generated message map functions
protected:
	//{{AFX_MSG(CRegisterWnd)
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_REGISTERWND_H__EB8D7C05_F1E1_4D5B_B968_5ABEFA8C8271__INCLUDED_)
