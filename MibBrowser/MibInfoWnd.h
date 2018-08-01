#if !defined(AFX_MIBINFOWND_H__C4D864C8_A2CC_4EF5_BB07_19397ED79097__INCLUDED_)
#define AFX_MIBINFOWND_H__C4D864C8_A2CC_4EF5_BB07_19397ED79097__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MibInfoWnd.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMibInfoWnd window
class CMibTreeWnd;
class CMibInfoWnd : public CWnd
{
	DECLARE_DYNCREATE(CMibInfoWnd)
// Construction
public:
	CMibInfoWnd();

// Attributes
public:

// Operations
public:
	void SetMibTreeWnd(CMibTreeWnd *mib_tree_wnd);
	LRESULT OnShowMibInfo(WPARAM wParam, LPARAM lParam);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMibInfoWnd)
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CMibInfoWnd();

protected:
	class CPairPropRect
	{
	public:
		CRect ind;
		CRect key;
		CRect prop;
	};

	int CX_PROP;
	int m_posArrow;
	CMibTreeWnd *m_mib_tree_wnd;
	CArray<CPairPropRect, CPairPropRect&> m_rectProp;
	CImageList m_listImgs;

	void EraseRect(CDC *pDC, CRect &rect, COLORREF cr = RGB(224, 224, 224));
	void TextInRect(CDC *pDC, CString &text, CRect &rect);
	void DrawArrow(CDC *pDC, CRect &rect);

	// Generated message map functions
protected:
	//{{AFX_MSG(CMibInfoWnd)
	afx_msg void OnWindowPosChanged(WINDOWPOS FAR* lpwndpos);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MIBINFOWND_H__C4D864C8_A2CC_4EF5_BB07_19397ED79097__INCLUDED_)
