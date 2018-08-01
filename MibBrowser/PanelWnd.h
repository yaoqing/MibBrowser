#if !defined(AFX_PANELWND_H__9A16EC1B_81B2_4235_913E_AEEC2E82056F__INCLUDED_)
#define AFX_PANELWND_H__9A16EC1B_81B2_4235_913E_AEEC2E82056F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PanelWnd.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPanelWnd window

class CPanelWnd : public CWnd
{
	DECLARE_DYNCREATE(CPanelWnd)
// Construction
public:
	CPanelWnd();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPanelWnd)
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CPanelWnd();

	// Generated message map functions
protected:
	//{{AFX_MSG(CPanelWnd)
	afx_msg void OnPaint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PANELWND_H__9A16EC1B_81B2_4235_913E_AEEC2E82056F__INCLUDED_)
