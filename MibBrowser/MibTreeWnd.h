#if !defined(AFX_MIBTREEWND_H__1C8B8C35_A6E3_4199_98E1_2349B3517A79__INCLUDED_)
#define AFX_MIBTREEWND_H__1C8B8C35_A6E3_4199_98E1_2349B3517A79__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MibTreeWnd.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMibTreeWnd window
class CMibNode;
class CMibLoader;
class CMibInfoWnd;
class CMibTreeWnd : public CTreeCtrl
{
	DECLARE_DYNCREATE(CMibTreeWnd)
// Construction
public:
	CMibTreeWnd();

// Attributes
public:

// Operations
public:
	CMibNode *GetRoot();
	void CreateMibTree();
	void SetMibInfoWnd(CMibInfoWnd *mib_info_wnd);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMibTreeWnd)
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CMibTreeWnd();

protected:
	CImageList m_listImgs;
	CMibLoader *m_mib_loader;
	CMibInfoWnd *m_mib_info_wnd;

	// Generated message map functions
protected:
	//{{AFX_MSG(CMibTreeWnd)
	afx_msg void OnSelchanged(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnRclick(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MIBTREEWND_H__1C8B8C35_A6E3_4199_98E1_2349B3517A79__INCLUDED_)
