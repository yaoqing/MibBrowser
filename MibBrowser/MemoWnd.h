#if !defined(AFX_MEMOWND_H__C304BA9F_BE74_47D5_A4AE_95E8D88EA5FF__INCLUDED_)
#define AFX_MEMOWND_H__C304BA9F_BE74_47D5_A4AE_95E8D88EA5FF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MemoWnd.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMemoWnd window

class CMemoWnd : public CWnd
{
	/*class CElement
	{
	public:
		CString oid;
		CString val;
	};*/

	DECLARE_DYNCREATE(CMemoWnd)
// Construction
public:
	CMemoWnd();

// Attributes
public:

protected:
	int m_nLineMaxLen;
	int m_nLineHeight;
	CArray<int, int> m_nColors;
	CArray<int, int> m_nTypes;
	CArray<CString, CString&> m_szMsgs;

	int first;
	CPoint caret;
	int m_nLineNum;

	void CaretFollow();
	void DrawLine(CDC *pDC, int index, CRect rect);

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMemoWnd)
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
public:
	LRESULT OnAddLine(WPARAM wParam, LPARAM lParam);
	virtual ~CMemoWnd();

	// Generated message map functions
protected:
	//{{AFX_MSG(CMemoWnd)
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnPaint();
	afx_msg void OnNcCalcSize(BOOL bCalcValidRects, NCCALCSIZE_PARAMS FAR* lpncsp);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MEMOWND_H__C304BA9F_BE74_47D5_A4AE_95E8D88EA5FF__INCLUDED_)
