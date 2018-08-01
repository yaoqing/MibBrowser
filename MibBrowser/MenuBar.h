#if !defined(AFX_MENUBAR_H__3370374F_5DD3_483F_BB14_536E73A1B98D__INCLUDED_)
#define AFX_MENUBAR_H__3370374F_5DD3_483F_BB14_536E73A1B98D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MenuBar.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMenuBar window

class CMenuBar : public CWnd
{
// Construction
public:
	CMenuBar();

protected:
	int CX_OFFSET_MENU;
	int CX_OFFSET_GRIPE;
	int CX_MENU_ITEM_SPACE;
	bool m_bInGripe;
	CImageList m_listImgs;
	CRect m_rectGripe;
	CArray<CRect, CRect> m_rectMenus;
	CArray<CString, CString> m_szMenus;
	CArray<bool, bool> m_bInMenus;
	CFont font;
	HMENU hMenu;
	CBitmap m_bmpHot;

	void LoadGripe();
	void DrawGripe(CDC *pDC, bool bInGripe);
	void DrawInHotGripe(CDC *pDC, bool &os, bool cs);
	void DrawMenu(CDC *pDC, int index, bool bInHot);
	void DrawMenuText(CDC *pDC, int x, int y, CString &szText);

	void ChangeLanguage();
	CSize GetMenuTextRect(CDC *pDC, CString &szText);
	HMENU LoadDyncMenu();
	
// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMenuBar)
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CMenuBar();

	// Generated message map functions
protected:
	//{{AFX_MSG(CMenuBar)
	afx_msg void OnNcCalcSize(BOOL bCalcValidRects, NCCALCSIZE_PARAMS FAR* lpncsp);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg UINT OnNcHitTest(CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MENUBAR_H__3370374F_5DD3_483F_BB14_536E73A1B98D__INCLUDED_)
