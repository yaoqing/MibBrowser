// CaptionBar.cpp : implementation file
//

#include "stdafx.h"
#include "MibBrowser.h"
#include "CaptionBar.h"
#include "MacXFrame.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCaptionBar

CCaptionBar::CCaptionBar()
{
	CX_BOX = 16;
	CY_BOX = 16;
	CX_OFFSET_CLOSE = 6;
	CY_OFFSET_CLOSE = 3;
	CX_OFFSET_ICON = 3;
	m_bMaxed = false;
	m_bMined = false;
	m_bInMin = false;
	m_bInMax = false;
	m_bInClose = false;
	m_bPressdMin = false;
	m_bPressdMax = false;
	m_bPressdClose = false;
	LoadSizedBox();
}

CCaptionBar::~CCaptionBar()
{
}


BEGIN_MESSAGE_MAP(CCaptionBar, CWnd)
	//{{AFX_MSG_MAP(CCaptionBar)
	ON_WM_NCHITTEST()
	ON_WM_NCCALCSIZE()
	ON_WM_NCLBUTTONDOWN()
	ON_WM_ERASEBKGND()
	ON_WM_NCLBUTTONUP()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CCaptionBar message handlers

void CCaptionBar::LoadSizedBox()
{
	/*加载最小最大化及关闭图标*/
	BITMAP bmp;
	CBitmap img;
	img.LoadBitmap(IDB_TITLE_BOX);
	img.GetBitmap(&bmp);
	CX_BOX = bmp.bmWidth/12;
	CY_BOX = bmp.bmHeight;
	m_listImgs.Create(CX_BOX, CY_BOX, ILC_COLOR24|ILC_MASK, 12, 1);
	m_listImgs.Add(&img, RGB(255, 0, 255));
	img.DeleteObject();
	img.LoadBitmap(IDB_TITLE_ICO);
	m_listImgs.Add(&img, RGB(255, 0, 255));
	img.DeleteObject();
}

BOOL CCaptionBar::PreCreateWindow(CREATESTRUCT& cs) 
{
	cs.style = WS_CHILD | WS_VISIBLE;
	cs.lpszClass = AfxRegisterWndClass(CS_HREDRAW|CS_VREDRAW|CS_DBLCLKS,
		::LoadCursor(NULL, IDC_ARROW), NULL, NULL);

	return TRUE;
}

void CCaptionBar::DrawMinBox(CDC *pDC, bool bActive, bool bPress)
{
	CMacXFrame* pWnd = (CMacXFrame*)GetParent();
	if (!pWnd->m_bar_menu.GetSafeHwnd()) return;

	CPoint pos;
	/*画最小化图标*/
	pos.x = m_rectBoxes[0].left;
	pos.y = m_rectBoxes[0].top;
	int index = bActive ? 0*3+1 : 0*3;
	index = bPress ? index+1 : index;
	m_listImgs.Draw(pDC, index, pos, ILD_NORMAL);
}

void CCaptionBar::DrawMaxBox(CDC *pDC, bool bActive, bool bPress)
{
	CMacXFrame* pWnd = (CMacXFrame*)GetParent();
	if (!pWnd->m_bar_menu.GetSafeHwnd()) return;

	CPoint pos;
	/*画最小化图标*/
	pos.x = m_rectBoxes[1].left;
	pos.y = m_rectBoxes[1].top;
	int index = m_bMaxed ? 2 : 1;
	index = bActive ? index*3+1 : index*3;
	index = bPress ? index+1 : index;
	m_listImgs.Draw(pDC, index, pos, ILD_NORMAL);
}

void CCaptionBar::DrawCloseBox(CDC *pDC, bool bActive, bool bPress)
{
	CPoint pos;
	/*画关闭图标*/
	pos.x = m_rectBoxes[2].left;
	pos.y = m_rectBoxes[2].top;
	int index = bActive ? 3*3+1 : 3*3;
	index = bPress ? index+1 : index;
	m_listImgs.Draw(pDC, index, pos, ILD_NORMAL);
}

UINT CCaptionBar::OnNcHitTest(CPoint point) 
{
	CRect rect;
	CWindowDC dc(this);
	ScreenToClient(&point);

	rect = m_rectBoxes[0];
	if (rect.PtInRect(point))
	{
		DrawInHotBoxes(&dc, m_bInMin, m_bInMax, m_bInClose, true, false, false);
		return HTMINBUTTON;
	}

	rect = m_rectBoxes[1];
	if (rect.PtInRect(point))
	{
		DrawInHotBoxes(&dc, m_bInMin, m_bInMax, m_bInClose, false, true, false);
		return HTMAXBUTTON;
	}

	rect = m_rectBoxes[2];
	if (rect.PtInRect(point))
	{
		DrawInHotBoxes(&dc, m_bInMin, m_bInMax, m_bInClose, false, false, true);
		return HTCLOSE;
	}

	DrawInHotBoxes(&dc, m_bInMin, m_bInMax, m_bInClose, false, false, false);
	DrawPressedBoxes(&dc, m_bPressdMin, m_bPressdMax, m_bPressdClose, false, false, false);
	
	/*rect.SetRect(0, 0, wight, height);
	if (rect.PtInRect(point))
		return HTCAPTION;*/

	return HTTRANSPARENT;
}

void CCaptionBar::OnNcCalcSize(BOOL bCalcValidRects, NCCALCSIZE_PARAMS FAR* lpncsp) 
{
	CWnd::OnNcCalcSize(bCalcValidRects, lpncsp);

	/*设置最小化最大化及关闭图标范围*/
	for (int i = 0; i < 3; i++)
	{
		CRect rect = lpncsp->rgrc[0];
		rect.left = rect.Width() - CX_OFFSET_CLOSE - (3-i)*CX_BOX;
		rect.right = rect.left + CX_BOX;
		rect.top = CY_OFFSET_CLOSE;
		rect.bottom = rect.top + CY_BOX;
		m_rectBoxes[i] = rect;
	}
	m_rectBoxes[i] = CRect(3, CY_OFFSET_CLOSE, 3+CX_BOX, CY_OFFSET_CLOSE+CY_BOX);
}

void CCaptionBar::DrawInHotBoxes(CDC *pDC, bool &os_min, bool &os_max, bool &os_close, bool cs_min, bool cs_max, bool cs_close)
{
	if (os_min != cs_min)
	{
		os_min = cs_min;
		DrawMinBox(pDC, cs_min, false);
	}
	if (os_max != cs_max)
	{
		os_max = cs_max;
		DrawMaxBox(pDC, cs_max, false);
	}
	if (os_close != cs_close)
	{
		os_close = cs_close;
		DrawCloseBox(pDC, cs_close, false);
	}
}

void CCaptionBar::DrawPressedBoxes(CDC *pDC, bool &os_min, bool &os_max, bool &os_close, bool cs_min, bool cs_max, bool cs_close)
{
	if (os_min != cs_min)
	{
		os_min = cs_min;
		DrawMinBox(pDC, m_bInMin, cs_min);
	}
	if (os_max != cs_max)
	{
		os_max = cs_max;
		DrawMaxBox(pDC, m_bInMax, cs_max);
	}
	if (os_close != cs_close)
	{
		os_close = cs_close;
		DrawCloseBox(pDC, m_bInClose, cs_close);
	}
}

void CCaptionBar::OnNcLButtonDown(UINT nHitTest, CPoint point) 
{
	CWindowDC dc(this);
	if (nHitTest == HTMINBUTTON)
		DrawPressedBoxes(&dc, m_bPressdMin, m_bPressdMax, m_bPressdClose, true, false, false);
	else if (nHitTest == HTMAXBUTTON)
		DrawPressedBoxes(&dc, m_bPressdMin, m_bPressdMax, m_bPressdClose, false, true, false);
	else if (nHitTest == HTCLOSE)
		DrawPressedBoxes(&dc, m_bPressdMin, m_bPressdMax, m_bPressdClose, false, false, true);
	else
		DrawPressedBoxes(&dc, m_bPressdMin, m_bPressdMax, m_bPressdClose, false, false, false);
	
	//CWnd::OnNcLButtonDown(nHitTest, point);
}

BOOL CCaptionBar::OnEraseBkgnd(CDC* pDC)
{
	CRect rect;
	GetClientRect(rect);
	//CMacXFrame *pWnd = (CMacXFrame *)GetParent();
	//rect.bottom = rect.top + pWnd->CY_CAPTION;

	/*准备兼容DC*/
	CDC dcMem;
	dcMem.CreateCompatibleDC(pDC);
	dcMem.SetBkMode(TRANSPARENT);

	/*画背景*/
	CBitmap bg, title;
	bg.CreateCompatibleBitmap(pDC, rect.Width(), rect.Height());
	title.LoadBitmap(IDB_TITLE_BAR);
	CBrush brush;
	brush.CreatePatternBrush(&title);
	CBitmap *pOldBmp = dcMem.SelectObject(&bg);
	dcMem.FillRect(rect, &brush);

	/*画左上角图标和窗口文本*/
	CString szCaption;
	GetWindowText(szCaption);
	m_listImgs.Draw(&dcMem, 12, CPoint(CX_OFFSET_ICON, CY_OFFSET_CLOSE), ILD_NORMAL);
	CFont font;
	font.CreatePointFont(LoadAsInteger(_T("caption.font.size"), 100), LoadAsString(_T("caption.font.name"), _T("Comic Sans MS Bold")));
	CFont *pOldFont = dcMem.SelectObject(&font);
	CSize size = dcMem.GetTextExtent(szCaption);
	int x = (rect.Width()-size.cx-CX_OFFSET_CLOSE)/2;
	int y = (rect.Height()-4-size.cy)/2;
	dcMem.TextOut(x, y, szCaption);

	/*画最小最大化及关闭图标*/
	DrawMinBox(&dcMem, m_bInMin, m_bPressdMin);
	DrawMaxBox(&dcMem, m_bInMax, m_bPressdMax);
	DrawCloseBox(&dcMem, m_bInClose, m_bPressdClose);

	/*从内存中拷贝至屏幕*/
	pDC->BitBlt(0, 0, rect.Width(), rect.Height(), &dcMem, 0, 0, SRCCOPY);

	/*清理恢复工作*/
	dcMem.SelectObject(pOldBmp);
	dcMem.SelectObject(pOldFont);
	dcMem.DeleteDC();
	bg.DeleteObject();
	title.DeleteObject();
	brush.DeleteObject();
	font.DeleteObject();

	return TRUE;
}

void CCaptionBar::OnNcLButtonUp(UINT nHitTest, CPoint point) 
{
	CMacXFrame* pWnd = (CMacXFrame*)GetParent();
	if (nHitTest == HTMINBUTTON && pWnd->m_bar_menu.GetSafeHwnd())
	{		
		GetParent()->PostMessage(WM_SYSCOMMAND, SC_MINIMIZE);
		m_bMined = true;
		m_bInMin = false;
	}
	if (nHitTest == HTMAXBUTTON && pWnd->m_bar_menu.GetSafeHwnd())
	{
		if (m_bMaxed)
		{
			GetParent()->PostMessage(WM_SYSCOMMAND, SC_RESTORE);
			m_bMaxed = false;
			m_bInMax = false;
		}
		else
		{
			GetParent()->PostMessage(WM_SYSCOMMAND, SC_MAXIMIZE);
			m_bMaxed = true;
			m_bInMax = false;
		}
	}
	if (nHitTest == HTCLOSE)
	{
		GetParent()->PostMessage(WM_SYSCOMMAND, SC_CLOSE);
	}

	CWindowDC dc(this);
	DrawPressedBoxes(&dc, m_bPressdMin, m_bPressdMax, m_bPressdClose, false, false, false);

	//CWnd::OnNcLButtonUp(nHitTest, point);
}

