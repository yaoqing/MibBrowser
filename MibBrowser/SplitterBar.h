#if !defined(AFX_SPLITTERBAR_H__190F45EB_9261_485A_BB02_804EC946502F__INCLUDED_)
#define AFX_SPLITTERBAR_H__190F45EB_9261_485A_BB02_804EC946502F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SplitterBar.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSplitterBar window

class CSplitterBar : public CWnd
{
// Construction
public:
	CSplitterBar(BOOL isHorz,
		CRuntimeClass* wnd_left_or_top,
		CRuntimeClass* wnd_right_or_bottom,
		CWnd* wnd_parent);

// Attributes
public:

// Operations
public:
	BOOL Create(int offset_left_or_top);
	LRESULT OnAdjustWindow(WPARAM wParam, LPARAM lParam);
	CWnd* GetLeftWnd();
	CWnd* GetRightWnd();
	CWnd* GetTopWnd();
	CWnd* GetBottomWnd();
	void SetHorizontal();
	void SetVertical();

	void SetLeftOrTopSplitter(CSplitterBar* wnd_left_or_top_splitter);
	void SetRightOrBottomSplitter(CSplitterBar* wnd_right_or_bottom_splitter);

protected:
	int  WIDTH_OR_HEIGHT;
	BOOL m_isHorz;
	BOOL m_bPressed;
	CWnd* m_wnd_left_or_top;
	CWnd* m_wnd_right_or_bottom;
	CWnd* m_wnd_parent;

	CRect m_saveRect;
	CBitmap m_saveGdi;

	CSplitterBar* m_wnd_left_or_top_splitter;
	CSplitterBar* m_wnd_right_or_bottom_splitter;


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSplitterBar)
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CSplitterBar();

	// Generated message map functions
protected:
	//{{AFX_MSG(CSplitterBar)
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnWindowPosChanged(WINDOWPOS FAR* lpwndpos);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SPLITTERBAR_H__190F45EB_9261_485A_BB02_804EC946502F__INCLUDED_)
