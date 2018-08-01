#if !defined(AFX_CHILDFRAME_H__E4D23EB1_279B_4EAC_B7B5_7CE6D615223E__INCLUDED_)
#define AFX_CHILDFRAME_H__E4D23EB1_279B_4EAC_B7B5_7CE6D615223E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ChildFrame.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CChildFrame window
class CMibNode;
class CMibTreeWnd;
class CSplitterBar;
class CChildFrame : public CWnd
{
// Construction
public:
	CChildFrame();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CChildFrame)
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CChildFrame();

protected:
	CSplitterBar *m_split_wnd0;
	CSplitterBar *m_split_wnd1;
	CSplitterBar *m_split_wnd2;

	void CreateTree(CMibTreeWnd *mib_tree, HTREEITEM hItem, CMibNode *node);

	// Generated message map functions
protected:
	//{{AFX_MSG(CChildFrame)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnWindowPosChanged(WINDOWPOS FAR* lpwndpos);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CHILDFRAME_H__E4D23EB1_279B_4EAC_B7B5_7CE6D615223E__INCLUDED_)
