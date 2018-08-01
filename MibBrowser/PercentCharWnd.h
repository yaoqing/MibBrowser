#if !defined(AFX_PERCENTCHARWND_H__F7F82937_AA04_4A17_8F06_2122833E98B8__INCLUDED_)
#define AFX_PERCENTCHARWND_H__F7F82937_AA04_4A17_8F06_2122833E98B8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PercentCharWnd.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPercentCharWnd window

class CPercentCharWnd : public CWnd
{
// Construction
public:
	CPercentCharWnd();

// Attributes
public:

// Operations
public:
	void SetPercent(int percent);

private:
	int m_nPercent;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPercentCharWnd)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CPercentCharWnd();

	// Generated message map functions
protected:
	//{{AFX_MSG(CPercentCharWnd)
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PERCENTCHARWND_H__F7F82937_AA04_4A17_8F06_2122833E98B8__INCLUDED_)
