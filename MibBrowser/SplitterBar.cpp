// SplitterBar.cpp : implementation file
//

#include "stdafx.h"
#include "MibBrowser.h"
#include "SplitterBar.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSplitterBar

CSplitterBar::CSplitterBar(BOOL isHorz,
		CRuntimeClass* wnd_left_or_top,
		CRuntimeClass* wnd_right_or_bottom,
		CWnd* wnd_parent)
{
	WIDTH_OR_HEIGHT = 2;
	m_isHorz = isHorz;
	m_bPressed = FALSE;
	m_wnd_parent = wnd_parent;
	
	m_wnd_left_or_top_splitter = NULL;
	m_wnd_right_or_bottom_splitter = NULL;

	m_wnd_left_or_top = (CWnd *)wnd_left_or_top->CreateObject();
	m_wnd_left_or_top->Create(NULL, NULL, WS_VISIBLE, CRect(0, 0, 0, 0), m_wnd_parent, 0);

	m_wnd_right_or_bottom = (CWnd *)wnd_right_or_bottom->CreateObject();
	m_wnd_right_or_bottom->Create(NULL, NULL, WS_VISIBLE, CRect(0, 0, 0, 0), m_wnd_parent, 0);
}

CSplitterBar::~CSplitterBar()
{
	m_wnd_left_or_top->DestroyWindow();
	m_wnd_right_or_bottom->DestroyWindow();
	delete m_wnd_left_or_top;
	delete m_wnd_right_or_bottom;
}


BEGIN_MESSAGE_MAP(CSplitterBar, CWnd)
	//{{AFX_MSG_MAP(CSplitterBar)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_REGISTERED_MESSAGE(WM_ADJUST_SPLITTER, OnAdjustWindow)
	ON_WM_WINDOWPOSCHANGED()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CSplitterBar message handlers

BOOL CSplitterBar::Create(int offset_left_or_top)
{
	if (m_isHorz)
		return CWnd::Create(NULL, NULL, 0, CRect(0, offset_left_or_top, 0, 0), m_wnd_parent, 0);
	else
		return CWnd::Create(NULL, NULL, 0, CRect(offset_left_or_top, 0, 0, 0), m_wnd_parent, 0);

}

LRESULT CSplitterBar::OnAdjustWindow(WPARAM wParam, LPARAM lParam)
{
	CRect rect_self;
	GetWindowRect(rect_self);
	m_wnd_parent->ScreenToClient(rect_self);

	CRect rect_parent;
	m_wnd_parent->GetClientRect(rect_parent);

	CRect rect;
	int left, top, right, bottom;
	if (m_isHorz)
	{
		left = rect_parent.left;
		right = rect_parent.right;
		top = rect_self.top;
		bottom = top + WIDTH_OR_HEIGHT;
		rect.SetRect(left, top, right, bottom);
		MoveWindow(rect);GetWindowRect(rect_self);
		m_wnd_parent->ScreenToClient(rect_self);

		CWnd *wnd_top = m_wnd_left_or_top;
		left = rect_parent.left;
		top = rect_parent.top;
		right = rect_parent.right;
		bottom = rect_self.top;
		rect.SetRect(left, top, right, bottom);
		wnd_top->MoveWindow(rect);

		CWnd *wnd_bottom = m_wnd_right_or_bottom;
		left = rect_parent.left;
		top = rect_self.bottom;
		right = rect_parent.right;
		bottom = rect_parent.bottom;
		rect.SetRect(left, top, right, bottom);
		wnd_bottom->MoveWindow(rect);
	}
	else
	{
		left = rect_self.left;
		right = left + WIDTH_OR_HEIGHT;
		top = rect_parent.top;
		bottom = rect_parent.bottom;
		rect.SetRect(left, top, right, bottom);
		MoveWindow(rect);GetWindowRect(rect_self);
		m_wnd_parent->ScreenToClient(rect_self);

		CWnd *wnd_left = m_wnd_left_or_top;
		left = rect_parent.left;
		top = rect_parent.top;
		right = rect_self.left;
		bottom = rect_parent.bottom;
		rect.SetRect(left, top, right, bottom);
		wnd_left->MoveWindow(rect);

		CWnd *wnd_right = m_wnd_right_or_bottom;
		left = rect_self.right;
		top = rect_parent.top;
		right = rect_parent.right;
		bottom = rect_parent.bottom;
		rect.SetRect(left, top, right, bottom);
		wnd_right->MoveWindow(rect);
	}

	if (m_wnd_left_or_top_splitter)
		m_wnd_left_or_top_splitter->PostMessage(WM_ADJUST_SPLITTER);
	if (m_wnd_right_or_bottom_splitter)
		m_wnd_right_or_bottom_splitter->PostMessage(WM_ADJUST_SPLITTER);
	
	return TRUE;
}

BOOL CSplitterBar::PreCreateWindow(CREATESTRUCT& cs) 
{
	CBrush brush;
	cs.style = WS_CHILD | WS_VISIBLE;
	brush.CreateSolidBrush(RGB(172, 168, 153));
	if (m_isHorz)
	cs.lpszClass = AfxRegisterWndClass(CS_HREDRAW|CS_VREDRAW|CS_DBLCLKS,
		::LoadCursor(NULL, IDC_SIZENS), (HBRUSH)brush.Detach(), NULL);
	else
	cs.lpszClass = AfxRegisterWndClass(CS_HREDRAW|CS_VREDRAW|CS_DBLCLKS,
		::LoadCursor(NULL, IDC_SIZEWE), (HBRUSH)brush.Detach(), NULL);

	return TRUE;
}

CWnd* CSplitterBar::GetLeftWnd()
{
	return m_wnd_left_or_top;
}

CWnd* CSplitterBar::GetRightWnd()
{
	return m_wnd_right_or_bottom;
}

CWnd* CSplitterBar::GetTopWnd()
{
	return m_wnd_left_or_top;
}

CWnd* CSplitterBar::GetBottomWnd()
{
	return m_wnd_right_or_bottom;
}

void CSplitterBar::SetHorizontal()
{
	m_isHorz = TRUE;
	PostMessage(WM_ADJUST_SPLITTER);
}

void CSplitterBar::SetVertical()
{
	m_isHorz = FALSE;
	PostMessage(WM_ADJUST_SPLITTER);
}

void CSplitterBar::OnLButtonDown(UINT nFlags, CPoint point) 
{
	SetCapture();
	m_bPressed = TRUE;
	m_saveRect.SetRectEmpty();
}

void CSplitterBar::OnLButtonUp(UINT nFlags, CPoint point) 
{
	ReleaseCapture();
	m_bPressed = FALSE;

	CDC bufferDC, *pDC = m_wnd_parent->GetDC();
	bufferDC.CreateCompatibleDC(pDC);
	CBitmap *hGdiObj = NULL;

	if (!m_saveRect.IsRectEmpty())
	{
		hGdiObj = bufferDC.SelectObject(&m_saveGdi);
		pDC->BitBlt(m_saveRect.left, m_saveRect.top, m_saveRect.Width(), m_saveRect.Height(), &bufferDC, 0, 0, SRCCOPY);
		bufferDC.SelectObject(hGdiObj);
	}
	bufferDC.DeleteDC();
	ReleaseDC(pDC);

	m_saveGdi.DeleteObject();
	CRect rect, rect_parent;
	m_wnd_parent->GetClientRect(rect_parent);
	ClientToScreen(&point);
	m_wnd_parent->ScreenToClient(&point);

	int left, top, right, bottom;
	if (m_isHorz)
	{
		left = rect_parent.left;
		right = rect_parent.right;
		top = point.y;
		bottom = top+WIDTH_OR_HEIGHT;
		rect.SetRect(left, top, right, bottom);
		MoveWindow(rect);
	}
	else
	{
		left = point.x;
		right = left+WIDTH_OR_HEIGHT;
		top = rect_parent.top;
		bottom = rect_parent.bottom;
		rect.SetRect(left, top, right, bottom);
		MoveWindow(rect);
	}
}

void CSplitterBar::OnMouseMove(UINT nFlags, CPoint point) 
{
	if (!m_bPressed) return;
	
	CRect rect;
	m_wnd_parent->GetClientRect(rect);
	ClientToScreen(&point);
	m_wnd_parent->ScreenToClient(&point);
	
	int left, top, right, bottom;
	if (m_isHorz)
	{
		left = rect.left;
		right = rect.right;
		top = point.y;
		bottom = top+WIDTH_OR_HEIGHT;
		rect.SetRect(left, top, right, bottom);
	}
	else
	{
		left = point.x;
		right = left+WIDTH_OR_HEIGHT;
		top = rect.top;
		bottom = rect.bottom;
		rect.SetRect(left, top, right, bottom);
	}

	CDC bufferDC, *pDC = m_wnd_parent->GetDC();
	bufferDC.CreateCompatibleDC(pDC);
	CBitmap *hGdiObj = NULL;

	if (!m_saveRect.IsRectEmpty())
	{
		hGdiObj = bufferDC.SelectObject(&m_saveGdi);
		pDC->BitBlt(m_saveRect.left, m_saveRect.top, m_saveRect.Width(), m_saveRect.Height(), &bufferDC, 0, 0, SRCCOPY);
		bufferDC.SelectObject(hGdiObj);
	}

	m_saveRect = rect;
	m_saveGdi.DeleteObject();
	m_saveGdi.CreateCompatibleBitmap(pDC, rect.Width(), rect.Height());

	hGdiObj = bufferDC.SelectObject(&m_saveGdi);
	bufferDC.BitBlt(0, 0, rect.Width(), rect.Height(), pDC, rect.left, rect.top, SRCCOPY);	
	bufferDC.SelectObject(hGdiObj);
	bufferDC.DeleteDC();

	pDC->FillSolidRect(rect, RGB(172, 168, 153));
	ReleaseDC(pDC);
	
	return;
}

void CSplitterBar::OnWindowPosChanged(WINDOWPOS FAR* lpwndpos) 
{
	CWnd::OnWindowPosChanged(lpwndpos);
	
	// TODO: Add your message handler code here
	//PostMessage(WM_ADJUST_SPLITTER);
	OnAdjustWindow(0, 0);
}

void CSplitterBar::SetLeftOrTopSplitter(CSplitterBar* wnd_left_or_top_splitter)
{
	m_wnd_left_or_top_splitter = wnd_left_or_top_splitter;
}

void CSplitterBar::SetRightOrBottomSplitter(CSplitterBar* wnd_right_or_bottom_splitter)
{
	m_wnd_right_or_bottom_splitter = wnd_right_or_bottom_splitter;
}
