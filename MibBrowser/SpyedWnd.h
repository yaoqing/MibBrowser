#if !defined(AFX_SPYEDWND_H__866F2943_28D9_4CC4_B39A_16546C823FCB__INCLUDED_)
#define AFX_SPYEDWND_H__866F2943_28D9_4CC4_B39A_16546C823FCB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SpyedWnd.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSpyedWnd window

class CSpyedWnd : public CWnd
{
	DECLARE_DYNCREATE(CSpyedWnd)
// Construction
public:
	CSpyedWnd();

// Attributes
public:
	CPoint m_pointWnd;
	CString m_strIpAddr;
	CString m_strName;
	CString m_strRemark;
	CString m_strCommunity;
	UINT m_nPos;
	UINT m_nPort;
	CToolTipCtrl m_tt;
	CImageList m_listImgs;
	BOOL m_bIsCapture;
	BOOL m_bIsClicked;

	static int SPY_WND_WIDTH;            //被监测窗口的宽度
	static int SPY_WND_HEIGHT;           //被监测窗口的高度
	static int SPY_WND_ICON_WIDTH;       //被监测窗口中图标的宽度
	static int SPY_WND_ICON_HEIGHT;      //被监测窗口中图标的高度

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSpyedWnd)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CSpyedWnd();
	BOOL OnTipText(UINT id, NMHDR* pNMHDR, LRESULT* pResult);
	LRESULT OnCaptured(WPARAM wParam, LPARAM lParam);
	void OnClick();

	static DWORD WINAPI SNMPWalkProc(LPVOID lpParameter);
	static DWORD WINAPI SNMPTouchProc(LPVOID lpParameter);

	CWnd *pWnd;

	// Generated message map functions
protected:
	//{{AFX_MSG(CSpyedWnd)
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SPYEDWND_H__866F2943_28D9_4CC4_B39A_16546C823FCB__INCLUDED_)
