#if !defined(AFX_PROPERTYWND_H__646E5CB8_5B1C_450B_8A9E_885393CDC40C__INCLUDED_)
#define AFX_PROPERTYWND_H__646E5CB8_5B1C_450B_8A9E_885393CDC40C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PropertyWnd.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPropertyWnd

class CPropertyWnd : public CWnd
{
	DECLARE_DYNAMIC(CPropertyWnd)

// Construction
public:
	CPropertyWnd(CString strIpAddr, UINT nPort, CString strName, 
					CString strRemark, CString strCommunity);
	~CPropertyWnd();

	UINT m_nPort;
	CString m_strIpAddr;
	CString m_strName;
	CString m_strRemark;
	CString m_strCommunity;

// Attributes
public:

// Operations
public:
	void AddPage(CString szCaption, CRuntimeClass *pChildWnd);

protected:
	int curr_track;
	int LEFT_MARGIN_TAB, RIGHT_MARGIN_TAB;
	int TOP_MARGIN_TAB, BOTTOM_MARGIN_TAB;
	int CX_TAB_PAD, CY_TAB_PAD;
	int CX_TAB_SPACE, CY_TAB_SPACE;
	CFont *font;
	CArray<CString, CString> m_arrayCaptions;
	CArray<CRuntimeClass *, CRuntimeClass *> m_arrayChildWnds;
	CArray<CRect, CRect> m_arrayRects;
	int m_nSelWndPos;
	CRect m_rectChildWnd;
	CWnd *m_wndChildWnd;

	void DrawNormalTab(CDC *pDC, CRect &rect);
	void DrawTrackTab(CDC *pDC, CRect &rect);
	void DrawLabel(CDC *pDC, CRect &rect, CString szText);
	void AdjustSelTabRect();
	void AdjustSelWnd();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPropertyWnd)
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// Implementation
public:

	// Generated message map functions
protected:
	//{{AFX_MSG(CPropertyWnd)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PROPERTYWND_H__646E5CB8_5B1C_450B_8A9E_885393CDC40C__INCLUDED_)
