#if !defined(AFX_MACXFRAME_H__C5AE354C_D8D6_4E6A_BB3F_7B96A2B336F9__INCLUDED_)
#define AFX_MACXFRAME_H__C5AE354C_D8D6_4E6A_BB3F_7B96A2B336F9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MacXFrame.h : header file
//

#include "CaptionBar.h"
#include "MenuBar.h"

/////////////////////////////////////////////////////////////////////////////
// CMacXFrame window

class CMacXFrame : public CWnd
{
// Construction
public:
	CMacXFrame();

// Attributes
public:
	friend class CCaptionBar;
	friend class CMenuBar;
	static UINT WM_INITFRAME;

// Operations
public:
	LRESULT OnInitFrame(WPARAM wParam, LPARAM lParam);
	void SetChildFrame(CWnd *child);
	void AdjustChildWnd(int cx, int cy);
	HWND CreateModal(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext, CWnd *pOwnerWnd, CWnd *pNotifyWnd = NULL, UINT message = 0);

protected:
	int CY_CAPTION;
	int CY_MENU_BAR;
	UINT m_notify_message;
	CCaptionBar m_bar_caption;
	CMenuBar m_bar_menu;
	CWnd *m_client_wnd;
	CWnd *m_notify_wnd;
	CWnd *m_modal_owner_wnd;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMacXFrame)
	public:
	virtual BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext = NULL);
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CMacXFrame();

	// Generated message map functions
protected:
	//{{AFX_MSG(CMacXFrame)
	afx_msg void OnNcCalcSize(BOOL bCalcValidRects, NCCALCSIZE_PARAMS FAR* lpncsp);
	afx_msg void OnNcPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg UINT OnNcHitTest(CPoint point);
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnNcLButtonDblClk(UINT nHitTest, CPoint point);
	afx_msg void OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI);
	afx_msg void OnClose();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MACXFRAME_H__C5AE354C_D8D6_4E6A_BB3F_7B96A2B336F9__INCLUDED_)
