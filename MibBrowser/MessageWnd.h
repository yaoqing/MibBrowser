#if !defined(AFX_MESSAGEWND_H__DC5CED2F_8F2A_4001_A48B_659ACD3B0452__INCLUDED_)
#define AFX_MESSAGEWND_H__DC5CED2F_8F2A_4001_A48B_659ACD3B0452__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MessageWnd.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMessageWnd window

class CMessageWnd : public CWnd
{
// Construction
public:
	CMessageWnd(CString msg);

// Attributes
public:

protected:
	CString message;
	CFont *font;
	CButton m_ctrlOk;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMessageWnd)
	protected:
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CMessageWnd();

	// Generated message map functions
protected:
	//{{AFX_MSG(CMessageWnd)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MESSAGEWND_H__DC5CED2F_8F2A_4001_A48B_659ACD3B0452__INCLUDED_)
