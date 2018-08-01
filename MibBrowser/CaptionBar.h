#if !defined(AFX_CAPTIONBAR_H__291E96AD_3D1E_4814_A3DB_92B811FF2E3C__INCLUDED_)
#define AFX_CAPTIONBAR_H__291E96AD_3D1E_4814_A3DB_92B811FF2E3C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CaptionBar.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CCaptionBar window

class CCaptionBar : public CWnd
{
// Construction
public:
	CCaptionBar();

// Attributes
public:
	friend class CMacXFrame;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCaptionBar)
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CCaptionBar();

protected:
	int CX_BOX;
	int CY_BOX;
	int CX_OFFSET_CLOSE;
	int CY_OFFSET_CLOSE;
	int CX_OFFSET_ICON;
	CRect m_rectBoxes[4];
	CImageList m_listImgs;
	bool m_bMaxed;
	bool m_bMined;
	bool m_bInMin;
	bool m_bInMax;
	bool m_bInClose;
	bool m_bPressdMin;
	bool m_bPressdMax;
	bool m_bPressdClose;

	void LoadSizedBox();
	void DrawMinBox(CDC *pDC, bool bActive, bool bPress);
	void DrawMaxBox(CDC *pDC, bool bActive, bool bPress);
	void DrawCloseBox(CDC *pDC, bool bActive, bool bPress);
	void DrawInHotBoxes(CDC *pDC, bool &os_min, bool &os_max, bool &os_close, bool cs_min, bool cs_max, bool cs_close);
	void DrawPressedBoxes(CDC *pDC, bool &os_min, bool &os_max, bool &os_close, bool cs_min, bool cs_max, bool cs_close);

	// Generated message map functions
protected:
	//{{AFX_MSG(CCaptionBar)
	afx_msg UINT OnNcHitTest(CPoint point);
	afx_msg void OnNcCalcSize(BOOL bCalcValidRects, NCCALCSIZE_PARAMS FAR* lpncsp);
	afx_msg void OnNcLButtonDown(UINT nHitTest, CPoint point);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnNcLButtonUp(UINT nHitTest, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CAPTIONBAR_H__291E96AD_3D1E_4814_A3DB_92B811FF2E3C__INCLUDED_)
