// PropertyWnd.cpp : implementation file
//

#include "stdafx.h"
#include "mibbrowser.h"
#include "PropertyWnd.h"
#include "drawcube.h"
#include "PageProc.h"
#include "PageSystemDesc.h"
#include "PagePerformance.h"
#include "PageStorage.h"
#include "PageDevice.h"
#include "PageSoft.h"
#include "PageInterface.h"
#include "PageTcpTable.h"
#include "PageUdpTable.h"
#include "PageService.h"
#include "PageIpAddr.h"
#include "PageIpRoute.h"
#include "PageIpMedia.h"
#include "PageIpStat.h"
#include "PageIcmpStat.h"
#include "PageTcpStat.h"
#include "PageUdpStat.h"
#include "PageSnmpStat.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPropertyWnd

IMPLEMENT_DYNAMIC(CPropertyWnd, CWnd)

CPropertyWnd::CPropertyWnd(CString strIpAddr, UINT nPort, CString strName, 
					CString strRemark, CString strCommunity)
{
	LEFT_MARGIN_TAB = 25;
	RIGHT_MARGIN_TAB = 25;
	TOP_MARGIN_TAB = 15;
	BOTTOM_MARGIN_TAB = 20;
	CX_TAB_PAD = 50;
	CY_TAB_PAD = 8;
	CX_TAB_SPACE = 2;
	CY_TAB_SPACE = 0;
	m_nSelWndPos = -1;
	curr_track = -1;
	m_wndChildWnd = NULL;

	m_nPort = nPort;
	m_strIpAddr = strIpAddr;
	m_strName = strName;
	m_strRemark = strRemark;
	m_strCommunity = strCommunity;

	m_rectChildWnd.SetRectEmpty();
	font = new CFont();
	font->CreatePointFont(LoadAsInteger(_T("property.dlg.font.size"), 90), LoadAsString(_T("property.dlg.font.name"), _T("Arial")));
}

CPropertyWnd::~CPropertyWnd()
{
	font->DeleteObject();
	delete font;
	delete m_wndChildWnd;
}

BEGIN_MESSAGE_MAP(CPropertyWnd, CWnd)
	//{{AFX_MSG_MAP(CPropertyWnd)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_ERASEBKGND()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPropertyWnd message handlers

int CPropertyWnd::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO: Add your specialized creation code here

	AddPage(LoadAsString(_T("property.page.sysdesc.caption"), _T("System")), RUNTIME_CLASS(CPageSystemDesc));
	AddPage(LoadAsString(_T("property.page.process.caption"), _T("Process")), RUNTIME_CLASS(CPageProc));
	AddPage(LoadAsString(_T("property.page.performance.caption"), _T("Performance")), RUNTIME_CLASS(CPagePerformance));
	AddPage(LoadAsString(_T("property.page.storage.caption"), _T("Storage")), RUNTIME_CLASS(CPageStorage));
	AddPage(LoadAsString(_T("property.page.device.caption"), _T("Device")), RUNTIME_CLASS(CPageDevice));
	AddPage(LoadAsString(_T("property.page.soft.caption"), _T("Soft")), RUNTIME_CLASS(CPageSoft));
	AddPage(LoadAsString(_T("property.page.interface.caption"), _T("Interface")), RUNTIME_CLASS(CPageInterface));
	AddPage(LoadAsString(_T("property.page.tcptable.caption"), _T("TcpTable")), RUNTIME_CLASS(CPageTcpTable));
	AddPage(LoadAsString(_T("property.page.udptable.caption"), _T("UDPTable")), RUNTIME_CLASS(CPageUdpTable));
	AddPage(LoadAsString(_T("property.page.service.caption"), _T("Service")), RUNTIME_CLASS(CPageService));
	AddPage(LoadAsString(_T("property.page.ipaddr.caption"), _T("IPAddr")), RUNTIME_CLASS(CPageIpAddr));
	AddPage(LoadAsString(_T("property.page.iproute.caption"), _T("IPRoute")), RUNTIME_CLASS(CPageIpRoute));
	AddPage(LoadAsString(_T("property.page.ipmedia.caption"), _T("IPMedia")), RUNTIME_CLASS(CPageIpMedia));
	AddPage(LoadAsString(_T("property.page.ipstat.caption"), _T("IPStat")), RUNTIME_CLASS(CPageIpStat));
	AddPage(LoadAsString(_T("property.page.icmpstat.caption"), _T("ICMPStat")), RUNTIME_CLASS(CPageIcmpStat));
	AddPage(LoadAsString(_T("property.page.tcpstat.caption"), _T("TCPStat")), RUNTIME_CLASS(CPageTcpStat));
	AddPage(LoadAsString(_T("property.page.udpstat.caption"), _T("UDPStat")), RUNTIME_CLASS(CPageUdpStat));
	AddPage(LoadAsString(_T("property.page.snmpstat.caption"), _T("SNMPStat")), RUNTIME_CLASS(CPageSnmpStat));
	SetFont(font);
	AdjustSelWnd();

	return ShowWindow(SW_SHOW);
}

void CPropertyWnd::AddPage(CString szCaption, CRuntimeClass *pChildWnd)
{
	m_arrayCaptions.Add(szCaption);
	m_arrayChildWnds.Add(pChildWnd);
	if (m_arrayChildWnds.GetSize() > 0 && m_nSelWndPos < 0)
		m_nSelWndPos = 0;
}

void CPropertyWnd::OnSize(UINT nType, int cx, int cy) 
{
	CWnd::OnSize(nType, cx, cy);

	CDC *pDC = GetDC();
	CFont *pOlodFont = pDC->SelectObject(font);
	CRect rect;
	CSize size;
	int x = LEFT_MARGIN_TAB+CX_TAB_SPACE;
	int y = TOP_MARGIN_TAB+CY_TAB_SPACE;
	int height = pDC->GetTextExtent(_T(" ")).cy;
	m_arrayRects.RemoveAll();

	int save, rows = 0;
	x = cx - RIGHT_MARGIN_TAB;
	CArray<int, int> m_arrayBreaks;
	m_arrayBreaks.InsertAt(0, m_arrayCaptions.GetSize());
	for (int i = m_arrayCaptions.GetSize()-1; i >= 0; i--)
	{
		size = pDC->GetTextExtent(m_arrayCaptions.GetAt(i));
		save = x - CX_TAB_PAD - CX_TAB_SPACE - size.cx;
		if (save > LEFT_MARGIN_TAB)
		{
			rect.SetRect(save, 0, x, 0);
			m_arrayRects.InsertAt(0, rect);
			x = save;
		}
		else
		{
			if (x == cx-RIGHT_MARGIN_TAB)
			{
				/*保证TAB每行最少有一个*/
				m_arrayBreaks.InsertAt(0, i);
				rect.SetRect(LEFT_MARGIN_TAB+CX_TAB_SPACE, 0, x, 0);
				m_arrayRects.InsertAt(0, rect);
			}
			else
			{
				/*TAB换行*/
				m_arrayBreaks.InsertAt(0, i+1);
				x = cx - RIGHT_MARGIN_TAB;
				i++; rows++; continue;
			}
		}
	}
	pDC->SelectObject(pOlodFont);
	ReleaseDC(pDC);

	/*调整TAB的上下左右值*/
	int index=0, cx_adjust=0, x_offset=0;
	if (m_arrayBreaks.GetSize() > 0)
	{
		bool first_cols = true;
		for (i = 0; i < m_arrayCaptions.GetSize(); i++)
		{
			rect = m_arrayRects[i];
			if (first_cols)
			{
				/*每行第一列调整为左对齐，并计算每一列需增加的长度*/
				x_offset = rect.left-LEFT_MARGIN_TAB-CX_TAB_SPACE;
				if (m_arrayBreaks[0]-i)
					cx_adjust = x_offset/(m_arrayBreaks[0]-i);
				else
					cx_adjust = 0;
			}

			if (i == m_arrayBreaks[0])
			{
				/*TAB换行*/
				first_cols = true;
				m_arrayBreaks.RemoveAt(0);
				y += height + CY_TAB_PAD + CY_TAB_SPACE;
				i--; index = 0; continue;
			}

			/*调整每一列的矩形范围*/
			rect.left += index * cx_adjust - x_offset;
			rect.right += (index+1) * cx_adjust - x_offset - CX_TAB_SPACE;
			if (i+1 == m_arrayBreaks[0])
				rect.right = cx - RIGHT_MARGIN_TAB;
			rect.top = y; index++;
			rect.bottom = y + height + CY_TAB_PAD;
			m_arrayRects[i] = rect;
			first_cols = false;
		}
	}
	
	y += height + CY_TAB_PAD;
	if (m_nSelWndPos < m_arrayChildWnds.GetSize())
	{
		m_rectChildWnd.SetRect(LEFT_MARGIN_TAB+1, y+1, cx-RIGHT_MARGIN_TAB, cy-BOTTOM_MARGIN_TAB);
		if (m_wndChildWnd) m_wndChildWnd->MoveWindow(m_rectChildWnd);
	}

	/*调整选中TAB与其它TAB的位置*/
	AdjustSelTabRect();
	Invalidate();
}

BOOL CPropertyWnd::OnEraseBkgnd(CDC* pDC) 
{
	// TODO: Add your message handler code here and/or call default
	CDC dcmem;
	dcmem.CreateCompatibleDC(pDC);

	CRect rect, save;
	GetClientRect(&rect);
	save = rect;

	CBitmap bmp;
	bmp.CreateCompatibleBitmap(pDC, rect.Width(), rect.Height());
	CBitmap *pOldBmp = dcmem.SelectObject(&bmp);
	dcmem.FillSolidRect(rect, LoadAsRGB(_T("property.dlg.color"), _T("236,233,216")));

	int i;
	CArray<int, int> arrayTrack;
	for (save = rect, i = 0; i < m_arrayRects.GetSize(); i++)
	{
		rect = m_arrayRects.GetAt(i);
		if (curr_track == i || m_nSelWndPos == i)
			arrayTrack.Add(i);
		else
		{
			PaintGradiantRect(&dcmem, rect, RGB(255,255,255), RGB(240,240,234), false, true);
			DrawNormalTab(&dcmem, rect);
			DrawLabel(&dcmem, rect, m_arrayCaptions.GetAt(i));
		}
	}
	for (i = 0; i < arrayTrack.GetSize(); i++)
	{
		rect = m_arrayRects.GetAt(arrayTrack.GetAt(i));
		rect.InflateRect(0, 2, 0, 0);
		PaintGradiantRect(&dcmem, rect, RGB(255,255,255), RGB(240,240,234), false, true);
		DrawTrackTab(&dcmem, rect);
		DrawLabel(&dcmem, rect, m_arrayCaptions.GetAt(arrayTrack.GetAt(i)));
	}

	CPen pen;
	pen.CreatePen(PS_SOLID, 1, RGB(145,167,180));
	CPen *pOldPen = dcmem.SelectObject(&pen);
	
	CRect rectSel;
	rectSel.SetRectEmpty();
	if (m_nSelWndPos >= 0)
		rectSel = m_arrayRects.GetAt(m_nSelWndPos);
	else
		rectSel.SetRect(LEFT_MARGIN_TAB, TOP_MARGIN_TAB, LEFT_MARGIN_TAB, TOP_MARGIN_TAB);
	rect.SetRect(LEFT_MARGIN_TAB, rectSel.bottom, save.Width()-RIGHT_MARGIN_TAB, save.Height()-BOTTOM_MARGIN_TAB);
	if (rect.top > rect.bottom) rect.bottom = rect.top;

	dcmem.MoveTo(rect.left, rect.top);
	dcmem.LineTo(rectSel.left+1, rect.top);
	dcmem.MoveTo(rectSel.right, rect.top);
	dcmem.LineTo(rect.right, rect.top);
	dcmem.LineTo(rect.right, rect.bottom);
	dcmem.LineTo(rect.left, rect.bottom);
	dcmem.LineTo(rect.left, rect.top);

	pDC->BitBlt(0, 0, save.Width(), save.Height(), &dcmem, 0, 0, SRCCOPY);
	pDC->SelectObject(pOldPen);
	dcmem.SelectObject(pOldBmp);
	
	return TRUE;
}

void CPropertyWnd::DrawLabel(CDC *pDC, CRect &rect, CString szText)
{
	CFont *pOldFont = pDC->SelectObject(font);
	
	pDC->SetBkMode(TRANSPARENT);
	pDC->SetTextColor(LoadAsRGB(_T("property.dlg.font.color"), _T("8,0,0")));
	pDC->DrawText(szText, rect, DT_CENTER|DT_VCENTER|DT_SINGLELINE);

	pDC->SelectObject(pOldFont);
}

void CPropertyWnd::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	CWnd::OnMouseMove(nFlags, point);

	CRect rect;
	for (int i = 0; i < m_arrayRects.GetSize(); i++)
	{
		rect = m_arrayRects.GetAt(i);
		if (rect.PtInRect(point))
		{
			if (curr_track != i)
			{
				curr_track = i;
				Invalidate();
				break;
			}
			else break;
		}
	}

	if (i == m_arrayRects.GetSize() && curr_track != -1)
	{
		curr_track = -1;
		Invalidate();
	}
}

void CPropertyWnd::DrawNormalTab(CDC *pDC, CRect &rect)
{
	CPen pen;
	pen.CreatePen(PS_SOLID, 1, RGB(145,167,180));
	CPen *pOldPen = pDC->SelectObject(&pen);

	pDC->MoveTo(rect.left+3, rect.top);
	pDC->LineTo(rect.right-2, rect.top);
	pDC->MoveTo(rect.right, rect.top+3);
	pDC->LineTo(rect.right, rect.bottom);
	pDC->MoveTo(rect.left, rect.top+3);
	pDC->LineTo(rect.left, rect.bottom);

	pDC->SelectObject(pOldPen);
	pen.DeleteObject();

	pen.CreatePen(PS_SOLID, 1, RGB(255,255,255));
	pOldPen = pDC->SelectObject(&pen);
	pDC->MoveTo(rect.left+3, rect.top+1);
	pDC->LineTo(rect.right-2, rect.top+1);

	pDC->SelectObject(pOldPen);
	pen.DeleteObject();

	pen.CreatePen(PS_SOLID, 1, RGB(254,254,254));
	pOldPen = pDC->SelectObject(&pen);
	pDC->MoveTo(rect.left+2, rect.top+2);
	pDC->LineTo(rect.right-1, rect.top+2);

	pDC->SelectObject(pOldPen);
	pen.DeleteObject();
	
	pDC->SetPixel(rect.left+1, rect.top+1, RGB(182,196,205));
	pDC->SetPixel(rect.right-1, rect.top+1, RGB(182,196,205));
	pDC->SetPixel(rect.left+2, rect.top, RGB(169,185,191));
	pDC->SetPixel(rect.right-2, rect.top, RGB(169,185,191));
	pDC->SetPixel(rect.left, rect.top+2, RGB(169,185,191));
	pDC->SetPixel(rect.right, rect.top+2, RGB(169,185,191));
	
	pDC->SetPixel(rect.left+1, rect.top, RGB(208,214,208));
	pDC->SetPixel(rect.right-1, rect.top, RGB(208,214,208));
	pDC->SetPixel(rect.left, rect.top+1, RGB(208,214,208));
	pDC->SetPixel(rect.right, rect.top+1, RGB(208,214,208));
	pDC->SetPixel(rect.left, rect.top, RGB(228,233,236));
	pDC->SetPixel(rect.right, rect.top, RGB(228,233,236));
	
	pDC->SetPixel(rect.left+2, rect.top+1, RGB(228,233,236));
	pDC->SetPixel(rect.right-2, rect.top+1, RGB(228,233,236));
	pDC->SetPixel(rect.left+1, rect.top+2, RGB(228,233,236));
	pDC->SetPixel(rect.right-1, rect.top+2, RGB(228,233,236));
}

void CPropertyWnd::DrawTrackTab(CDC *pDC, CRect &rect)
{
	CPen pen;
	pen.CreatePen(PS_SOLID, 1, RGB(230,139,44));
	CPen *pOldPen = pDC->SelectObject(&pen);

	pDC->MoveTo(rect.left+3, rect.top);
	pDC->LineTo(rect.right-2, rect.top);

	pDC->SelectObject(pOldPen);
	pen.DeleteObject();

	pen.CreatePen(PS_SOLID, 1, RGB(145,167,180));
	pOldPen = pDC->SelectObject(&pen);

	pDC->MoveTo(rect.right, rect.top+3);
	pDC->LineTo(rect.right, rect.bottom);
	pDC->MoveTo(rect.left, rect.top+3);
	pDC->LineTo(rect.left, rect.bottom);

	pDC->SelectObject(pOldPen);
	pen.DeleteObject();

	pen.CreatePen(PS_SOLID, 1, RGB(255,200,60));
	pOldPen = pDC->SelectObject(&pen);

	pDC->MoveTo(rect.left+3, rect.top+1);
	pDC->LineTo(rect.right-2, rect.top+1);

	pDC->SelectObject(pOldPen);
	pen.DeleteObject();

	pen.CreatePen(PS_SOLID, 1, RGB(255,200,60));
	pOldPen = pDC->SelectObject(&pen);

	pDC->MoveTo(rect.left+2, rect.top+2);
	pDC->LineTo(rect.right-1, rect.top+2);

	pDC->SelectObject(pOldPen);
	pen.DeleteObject();

	pDC->SetPixel(rect.left+1, rect.top, RGB(225,198,142));
	pDC->SetPixel(rect.right-1, rect.top, RGB(225,198,142));
	pDC->SetPixel(rect.left, rect.top+1, RGB(225,198,142));
	pDC->SetPixel(rect.right, rect.top+1, RGB(225,198,142));
	pDC->SetPixel(rect.left, rect.top, RGB(228,233,236));
	pDC->SetPixel(rect.right, rect.top, RGB(228,233,236));

	pDC->SetPixel(rect.left+2, rect.top, RGB(235,151,46));
	pDC->SetPixel(rect.right-2, rect.top, RGB(235,151,46));

	pDC->SetPixel(rect.left+1, rect.top+1, RGB(238,159,49));
	pDC->SetPixel(rect.right-1, rect.top+1, RGB(238,159,49));

	pDC->SetPixel(rect.left, rect.top+2, RGB(221,161,83));
	pDC->SetPixel(rect.right, rect.top+2, RGB(221,161,83));

	pDC->SetPixel(rect.left+2, rect.top+1, RGB(249,185,56));
	pDC->SetPixel(rect.right-2, rect.top+1, RGB(249,185,56));

	pDC->SetPixel(rect.left+1, rect.top+2, RGB(249,184,56));
	pDC->SetPixel(rect.right-1, rect.top+2, RGB(249,184,56));
}

void CPropertyWnd::AdjustSelTabRect()
{
	if (m_nSelWndPos >= 0 && m_nSelWndPos < m_arrayRects.GetSize())
	{
		CRect rect = m_arrayRects.GetAt(m_nSelWndPos);
		int i, top = rect.top;
		int height = rect.Height();
		int topest = top;
		for (i = 0; i < m_arrayRects.GetSize(); i++)
		{
			rect = m_arrayRects.GetAt(i);
			if (topest < rect.top) topest = rect.top;
		}

		if (top == topest) return;
		for (i = 0; i < m_arrayRects.GetSize(); i++)
		{
			rect = m_arrayRects.GetAt(i);
			if (top == rect.top)
			{
				rect.top = topest;
				rect.bottom = topest+height;
				m_arrayRects.SetAt(i, rect);
			}
			else if (topest == rect.top)
			{
				rect.top = top;
				rect.bottom = rect.top + height;
				m_arrayRects.SetAt(i, rect);
			}
		}
	}
}

void CPropertyWnd::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	CWnd::OnLButtonDown(nFlags, point);

	CRect rect;
	for (int i = 0; i < m_arrayRects.GetSize(); i++)
	{
		rect = m_arrayRects.GetAt(i);
		if (rect.PtInRect(point))
		{
			if (m_nSelWndPos != i)
			{
				m_nSelWndPos = i;
				AdjustSelWnd();
			}
			AdjustSelTabRect();
			Invalidate();
			break;
		}
	}
}

void CPropertyWnd::AdjustSelWnd()
{
	if (m_nSelWndPos >= 0)
	{
		if (m_wndChildWnd)
		{
			m_wndChildWnd->SendMessage(WM_CLOSE);
			delete m_wndChildWnd;
		}

		CRuntimeClass *rt = m_arrayChildWnds.GetAt(m_nSelWndPos);
		m_wndChildWnd = (CWnd *)rt->CreateObject();
		m_wndChildWnd->Create(NULL, NULL, WS_CHILD|WS_VISIBLE, m_rectChildWnd, this, 0, NULL);
	}
}

BOOL CPropertyWnd::PreCreateWindow(CREATESTRUCT& cs) 
{
	// TODO: Add your specialized code here and/or call the base class
	cs.style |= WS_CLIPCHILDREN;
	return CWnd::PreCreateWindow(cs);
}
