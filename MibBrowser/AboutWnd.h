#if !defined(AFX_ABOUTWND_H__30BEDA80_9832_4ECD_BCC3_BE5FA0CE6B9A__INCLUDED_)
#define AFX_ABOUTWND_H__30BEDA80_9832_4ECD_BCC3_BE5FA0CE6B9A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AboutWnd.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CAboutWnd window

class CAboutWnd : public CWnd
{
// Construction
public:
	CAboutWnd();

// Attributes
public:

// Operations
public:
	void DrawLabel(CDC *pDC, CRect &rect, CString szText);

protected:
	CFont *font;
	CButton m_ctrlOk;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutWnd)
	protected:
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CAboutWnd();

	// Generated message map functions
protected:
	//{{AFX_MSG(CAboutWnd)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ABOUTWND_H__30BEDA80_9832_4ECD_BCC3_BE5FA0CE6B9A__INCLUDED_)
