// EditEx.cpp : implementation file
//

#include "stdafx.h"
#include "mibbrowser.h"
#include "EditEx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CEditEx

CEditEx::CEditEx()
{
}

CEditEx::~CEditEx()
{
}


BEGIN_MESSAGE_MAP(CEditEx, CEdit)
	//{{AFX_MSG_MAP(CEditEx)
	ON_WM_NCCALCSIZE()
	ON_WM_NCPAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEditEx message handlers

void CEditEx::SetBorderColor(COLORREF clr)
{
	color = clr;
}

void CEditEx::OnNcCalcSize(BOOL bCalcValidRects, NCCALCSIZE_PARAMS FAR* lpncsp) 
{
	// TODO: Add your message handler code here and/or call default
	CRect rect = lpncsp->rgrc[0];
	rect.DeflateRect(2, 2, 1, 1);
	lpncsp->rgrc[0] = rect;
	
	CEdit::OnNcCalcSize(bCalcValidRects, lpncsp);
}

void CEditEx::OnNcPaint() 
{
	// TODO: Add your message handler code here
	// Do not call CEdit::OnNcPaint() for painting messages
	CRect rectWnd;
	GetWindowRect(rectWnd);
	rectWnd.OffsetRect(-rectWnd.left, -rectWnd.top);
	CWindowDC dcWnd(this);

	CPen pen;
	pen.CreatePen(PS_SOLID, 1, color);
	CPen *pOldPen = dcWnd.SelectObject(&pen);
	dcWnd.MoveTo(rectWnd.left, rectWnd.top);
	dcWnd.LineTo(rectWnd.right-1, rectWnd.top);
	dcWnd.LineTo(rectWnd.right-1, rectWnd.bottom-1);
	dcWnd.LineTo(rectWnd.left, rectWnd.bottom-1);
	dcWnd.LineTo(rectWnd.left, rectWnd.top);

	dcWnd.SelectObject(pOldPen);
	pen.DeleteObject();
}
