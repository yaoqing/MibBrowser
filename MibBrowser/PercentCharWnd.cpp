// PercentCharWnd.cpp : implementation file
//

#include "stdafx.h"
#include "mibbrowser.h"
#include "PercentCharWnd.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPercentCharWnd

CPercentCharWnd::CPercentCharWnd():m_nPercent(0)
{
}

CPercentCharWnd::~CPercentCharWnd()
{
}


BEGIN_MESSAGE_MAP(CPercentCharWnd, CWnd)
	//{{AFX_MSG_MAP(CPercentCharWnd)
	ON_WM_ERASEBKGND()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CPercentCharWnd message handlers

BOOL CPercentCharWnd::OnEraseBkgnd(CDC* pDC) 
{
	// TODO: Add your message handler code here and/or call default
	CRect rect;
	GetClientRect(rect);

	CDC dcMemory;
	dcMemory.CreateCompatibleDC(pDC);

	CBitmap memBitmap;
	memBitmap.CreateCompatibleBitmap(pDC, rect.Width(), rect.Height());
	CBitmap *pOldBitmap = dcMemory.SelectObject(&memBitmap);

	CBitmap bgPattern;
	bgPattern.LoadBitmap(IDB_PATTERN1);
	CBrush brush;
	brush.CreatePatternBrush(&bgPattern);
	
	BITMAP bitmap;
	bgPattern.GetBitmap(&bitmap);
	CSize size(bitmap.bmWidth, bitmap.bmHeight);
	int left_margin = (rect.Width()-1)/2 - size.cx;
	int top_margin = 6;
	int rect_height = rect.Height() - top_margin - 24;
	if (rect_height < 0) rect_height = 0;
	rect_height = (rect_height/size.cy) * size.cy;
	
	CRect rect_left(left_margin, top_margin, left_margin + size.cx, top_margin + rect_height);
	dcMemory.FillRect(rect_left, &brush);

	CRect rect_right(left_margin + size.cx + 1, top_margin, left_margin + size.cx * 2 + 1, top_margin + rect_height);
	dcMemory.FillRect(rect_right, &brush);

	bgPattern.DeleteObject();
	bgPattern.LoadBitmap(IDB_PATTERN2);
	brush.DeleteObject();
	brush.CreatePatternBrush(&bgPattern);
	
	int percent_height = (int)(m_nPercent * (rect_height / 100.0));
	percent_height = (percent_height / size.cy) * size.cy;
	rect_left.SetRect(left_margin, top_margin + (rect_height - percent_height), left_margin + size.cx, top_margin + rect_height);
	dcMemory.FillRect(rect_left, &brush);

	rect_right.SetRect(left_margin + size.cx + 1, top_margin + (rect_height - percent_height), left_margin + size.cx * 2 + 1, top_margin + rect_height);
	dcMemory.FillRect(rect_right, &brush);

	CFont *pFont = GetParent()->GetFont();
	CFont *pOldFont = dcMemory.SelectObject(pFont);

	CString strCaption;
	strCaption.Format(_T("%d %%"), m_nPercent);
	
	CSize round = dcMemory.GetTextExtent(strCaption);
	int font_height = rect.Height() - (top_margin + rect_height);
	if (font_height < 0) font_height = 0;
	int font_top_margin = (font_height - round.cy) * 2 / 3;
	if (font_top_margin < 0) font_top_margin = 0;

	dcMemory.SetTextColor(RGB(0, 255, 0));
	dcMemory.SetBkMode(TRANSPARENT);
	dcMemory.TextOut(left_margin + (size.cx * 2 + 1 - round.cx) / 2, 
		top_margin + rect_height + font_top_margin, strCaption);

	pDC->BitBlt(0, 0, rect.Width(), rect.Height(), &dcMemory, 0, 0, SRCCOPY);
	
	dcMemory.SelectObject(pOldFont);
	brush.DeleteObject();
	memBitmap.DeleteObject();
	dcMemory.SelectObject(pOldBitmap);
	dcMemory.DeleteDC();
	
	return TRUE;
}

void CPercentCharWnd::SetPercent(int percent)
{
	if (percent < 0) percent = 0;
	if (percent > 100) percent = 100;
	m_nPercent = percent;
	Invalidate();
}
