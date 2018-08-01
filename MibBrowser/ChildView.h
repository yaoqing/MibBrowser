// ChildView.h : interface of the CChildView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_CHILDVIEW_H__D1E68341_1826_44DB_AF5B_388E48748352__INCLUDED_)
#define AFX_CHILDVIEW_H__D1E68341_1826_44DB_AF5B_388E48748352__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CChildView window
class CSpyedWnd;
class CChildView : public CWnd
{
	DECLARE_DYNCREATE(CChildView)
// Construction
public:
	CChildView();
	friend class CMenuBar;

// Attributes
protected:
	CArray<CSpyedWnd*, CSpyedWnd*> m_arrayContainer;
	
	static int SPY_X_MARGIN;             //被监测窗口容器的X轴空白
	static int SPY_Y_MARGIN;             //被监测窗口容器的Y轴空白
	static int SPY_WND_X_SPACE;          //被监测窗口之间的水平间距
	static int SPY_WND_Y_SPACE;          //被监测窗口之间的垂直间距

	bool m_isOkExit;
	int m_nPort;
	CString m_szName;
	CString m_szIpAddr;
	CString m_szCommunity;
	CString m_szRemark;

	void InitLocalHost();

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CChildView)
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
public:
	void ClearContainer();
	void CreateSpyWnds();
	virtual ~CChildView();

	void DeleteSpyWnd(int index);
	CString GetLocalAddr();
	LRESULT OnModalFinished(WPARAM wParam, LPARAM lParam);

	CSpyedWnd * m_pFocusWnd;
	// Generated message map functions
protected:
	//{{AFX_MSG(CChildView)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnPaint();
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CHILDVIEW_H__D1E68341_1826_44DB_AF5B_388E48748352__INCLUDED_)
