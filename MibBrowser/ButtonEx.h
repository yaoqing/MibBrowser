#if !defined(AFX_BUTTONEX_H__B0DC6781_208B_4948_8241_536B924C03B5__INCLUDED_)
#define AFX_BUTTONEX_H__B0DC6781_208B_4948_8241_536B924C03B5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ButtonEx.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CButtonEx window

class CButtonEx : public CButton
{
// Construction
public:
	CButtonEx();

// Attributes
public:

// Operations
public:
	void SetColor(COLORREF clr);

protected:
	CBrush brush;
	COLORREF color;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CButtonEx)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CButtonEx();

	// Generated message map functions
protected:
	//{{AFX_MSG(CButtonEx)
	afx_msg HBRUSH CtlColor(CDC* pDC, UINT nCtlColor);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BUTTONEX_H__B0DC6781_208B_4948_8241_536B924C03B5__INCLUDED_)
