// ButtonEx.cpp : implementation file
//

#include "stdafx.h"
#include "mibbrowser.h"
#include "ButtonEx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CButtonEx

CButtonEx::CButtonEx()
{
	brush.CreateSolidBrush(RGB(255, 255, 255));
}

CButtonEx::~CButtonEx()
{
}


BEGIN_MESSAGE_MAP(CButtonEx, CButton)
	//{{AFX_MSG_MAP(CButtonEx)
	ON_WM_CTLCOLOR_REFLECT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CButtonEx message handlers

void CButtonEx::SetColor(COLORREF clr)
{
	color = clr;
}

HBRUSH CButtonEx::CtlColor(CDC* pDC, UINT nCtlColor) 
{
	brush.DeleteObject();
	brush.CreateSolidBrush(color);
	pDC->SetBkMode(TRANSPARENT);
	return (HBRUSH)brush.GetSafeHandle();
}
