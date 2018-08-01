// BGChartWnd.cpp : implementation file
//

#include "stdafx.h"
#include "mibbrowser.h"
#include "BGChartWnd.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBGChartWnd

CBGChartWnd::CBGChartWnd():array_size(0)
{
}

CBGChartWnd::~CBGChartWnd()
{
}


BEGIN_MESSAGE_MAP(CBGChartWnd, CWnd)
	//{{AFX_MSG_MAP(CBGChartWnd)
	ON_WM_ERASEBKGND()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CBGChartWnd message handlers

BOOL CBGChartWnd::OnEraseBkgnd(CDC* pDC) 
{
	// TODO: Add your message handler code here and/or call default
	CRect rect;
	GetClientRect(rect);

	CDC dcMemory;
	dcMemory.CreateCompatibleDC(pDC);

	CBitmap memBitmap;
	memBitmap.CreateCompatibleBitmap(pDC, rect.Width(), rect.Height());
	CBitmap *pOldBitmap = dcMemory.SelectObject(&memBitmap);

	CPen pen;
	pen.CreatePen(PS_SOLID, 1, RGB(0, 128, 64));
	CPen *pOldPen = dcMemory.SelectObject(&pen);
	//dcMemory.FillSolidRect(rect, RGB(0,0,0));

	int WIDTH = 12;
	int ELEMENT_WIDTH = 2;
	for (int x = rect.right; x > 0; x -= WIDTH)
	{
		dcMemory.MoveTo(x - array_size%6*ELEMENT_WIDTH, 0);
		dcMemory.LineTo(x - array_size%6*ELEMENT_WIDTH, rect.bottom);
	}
	for (int y = WIDTH; y < rect.bottom; y += WIDTH)
	{
		dcMemory.MoveTo(0, y);
		dcMemory.LineTo(rect.right, y);
	}

	dcMemory.SelectObject(pOldPen);
	pen.DeleteObject();
	pen.CreatePen(PS_SOLID, 1, RGB(0, 255, 0));
	pOldPen = dcMemory.SelectObject(&pen);

	int elements = rect.Width() / ELEMENT_WIDTH;
	while (arrayPercents.GetSize() > elements)
		arrayPercents.RemoveAt(0);

	int size = arrayPercents.GetSize();
	if (size > 0)
	{
		x = rect.right - size * ELEMENT_WIDTH;
		y = rect.bottom - (int)(arrayPercents[0] * (rect.bottom/100.0));
		dcMemory.MoveTo(x, y);
	}
	for (int i = 1; i < size; i++)
	{	
		x = rect.right - (size - i) * ELEMENT_WIDTH;
		y = rect.bottom - (int)(arrayPercents[i] * (rect.bottom /100.0));
		dcMemory.LineTo(x, y);
	}

	pDC->BitBlt(0, 0, rect.Width(), rect.Height(), &dcMemory, 0, 0, SRCCOPY);

	dcMemory.SelectObject(pOldPen);
	dcMemory.SelectObject(pOldBitmap);
	memBitmap.DeleteObject();
	pen.DeleteObject();
	dcMemory.DeleteDC();
	
	return TRUE;
}

void CBGChartWnd::AddPercent(int percent)
{
	if (percent < 0) percent = 0;
	if (percent > 100) percent = 100;
	arrayPercents.Add(percent);
	array_size++;
	Invalidate();
}