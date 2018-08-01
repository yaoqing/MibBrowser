#if !defined(AFX_BGCHARTWND_H__90B71454_3CBF_4853_9990_5BD83049E0E0__INCLUDED_)
#define AFX_BGCHARTWND_H__90B71454_3CBF_4853_9990_5BD83049E0E0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// BGChartWnd.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CBGChartWnd window

class CBGChartWnd : public CWnd
{
// Construction
public:
	CBGChartWnd();

// Attributes
public:

// Operations
public:
	void AddPercent(int percent);

private:
	int array_size;
	CArray<int, int> arrayPercents;	

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBGChartWnd)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CBGChartWnd();

	// Generated message map functions
protected:
	//{{AFX_MSG(CBGChartWnd)
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BGCHARTWND_H__90B71454_3CBF_4853_9990_5BD83049E0E0__INCLUDED_)
