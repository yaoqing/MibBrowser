#if !defined(AFX_PAGEDEVICE_H__12CF9CC9_81BD_40E3_912C_E904A28D554B__INCLUDED_)
#define AFX_PAGEDEVICE_H__12CF9CC9_81BD_40E3_912C_E904A28D554B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PageDevice.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPageDevice window

class CPageDevice : public CWnd
{
	DECLARE_DYNCREATE(CPageDevice)
// Construction
public:
	CPageDevice();

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
	//{{AFX_VIRTUAL(CPageDevice)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CPageDevice();

	// Generated message map functions
protected:
	//{{AFX_MSG(CPageDevice)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PAGEDEVICE_H__12CF9CC9_81BD_40E3_912C_E904A28D554B__INCLUDED_)
