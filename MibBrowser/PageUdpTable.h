#if !defined(AFX_PAGEUDPTABLE_H__51BC6B7F_A194_4A46_A57E_D3A99146B6DF__INCLUDED_)
#define AFX_PAGEUDPTABLE_H__51BC6B7F_A194_4A46_A57E_D3A99146B6DF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PageUdpTable.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPageUdpTable window

class CPageUdpTable : public CWnd
{
	DECLARE_DYNCREATE(CPageUdpTable)
// Construction
public:
	CPageUdpTable();

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
	//{{AFX_VIRTUAL(CPageUdpTable)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CPageUdpTable();

	// Generated message map functions
protected:
	//{{AFX_MSG(CPageUdpTable)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PAGEUDPTABLE_H__51BC6B7F_A194_4A46_A57E_D3A99146B6DF__INCLUDED_)
