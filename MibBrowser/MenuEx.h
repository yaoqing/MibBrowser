#if !defined(AFX_MENUEX_H__F01DD97C_EFD3_4B60_8420_EC4B4F2DC00F__INCLUDED_)
#define AFX_MENUEX_H__F01DD97C_EFD3_4B60_8420_EC4B4F2DC00F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MenuEx.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMenuEx window

class CMenuEx : public CMenu
{
// Construction
public:
	CMenuEx();

// Attributes
public:

// Operations
public:
	void EnableOwnerDraw(HMENU hMenu=NULL);
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	virtual void MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMenuEx)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CMenuEx();

	// Generated message map functions
protected:
	//{{AFX_MSG(CMenuEx)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MENUEX_H__F01DD97C_EFD3_4B60_8420_EC4B4F2DC00F__INCLUDED_)
