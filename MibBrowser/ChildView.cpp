// ChildView.cpp : implementation of the CChildView class
//

#include "stdafx.h"
#include "MibBrowser.h"
#include "ChildView.h"
#include "SpyedWnd.h"
#include "MenuEx.h"
#include "MacXFrame.h"
#include "AddIpConfWnd.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

int CChildView::SPY_X_MARGIN = 24;             //被监测窗口容器的X轴空白
int CChildView::SPY_Y_MARGIN = 24;             //被监测窗口容器的Y轴空白
int CChildView::SPY_WND_X_SPACE = 24;          //被监测窗口之间的水平间距
int CChildView::SPY_WND_Y_SPACE = 24;          //被监测窗口之间的垂直间距

IMPLEMENT_DYNCREATE(CChildView, CWnd)
/////////////////////////////////////////////////////////////////////////////
// CChildView

CChildView::CChildView()
{
	m_pFocusWnd = NULL;
	InitLocalHost();
}

CChildView::~CChildView()
{
	ClearContainer();
}


BEGIN_MESSAGE_MAP(CChildView, CWnd)
	//{{AFX_MSG_MAP(CChildView)
	ON_WM_SIZE()
	ON_WM_VSCROLL()
	ON_WM_PAINT()
	ON_WM_RBUTTONDOWN()
	ON_REGISTERED_MESSAGE(WM_MODAL_FINISHED, OnModalFinished)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CChildView message handlers

BOOL CChildView::PreCreateWindow(CREATESTRUCT& cs) 
{
	cs.style |= WS_VSCROLL | WS_CLIPCHILDREN;

	return CWnd::PreCreateWindow(cs);
}

void CChildView::CreateSpyWnds()
{
	//ClearContainer();
	//InitLocalHost();

	int size = m_arrayContainer.GetSize();
	if (size <= 0) return;

	CRect rect;
	GetClientRect(rect);

	int cols = (rect.Width() - SPY_X_MARGIN) / (CSpyedWnd::SPY_WND_WIDTH + SPY_WND_X_SPACE);
	if ((rect.Width() - SPY_X_MARGIN) - cols * (CSpyedWnd::SPY_WND_WIDTH + SPY_WND_X_SPACE) >= CSpyedWnd::SPY_WND_WIDTH)
		cols++;
	if (cols == 0) cols = 1;
	int rows = size / cols;
	while (rows * cols < size)
		rows++;

	int c = 0;
	int x = SPY_X_MARGIN;
	int y = SPY_Y_MARGIN;
	for (int m = 0; m < rows; m++)
	{
		x = SPY_X_MARGIN;
		for (int n = 0; n < cols; n++)
		{
			m_arrayContainer[c++]->m_pointWnd = CPoint(x, y);
			x += CSpyedWnd::SPY_WND_WIDTH + SPY_WND_X_SPACE;
			if (c >= size) break;
		}
		if (c >= size) break;
		y += CSpyedWnd::SPY_WND_HEIGHT + SPY_WND_Y_SPACE;
	}

	for (int i = 0; i < size; i++)
	{
		x = m_arrayContainer[i]->m_pointWnd.x;
		y = m_arrayContainer[i]->m_pointWnd.y - GetScrollPos(SB_VERT) * (CSpyedWnd::SPY_WND_HEIGHT+SPY_WND_Y_SPACE);
		m_arrayContainer[i]->m_nPos = i;
		CRect rectWnd = CRect(x, y, x+CSpyedWnd::SPY_WND_WIDTH, y+CSpyedWnd::SPY_WND_HEIGHT);
		if (m_arrayContainer[i]->GetSafeHwnd())
			m_arrayContainer[i]->MoveWindow(rectWnd);
		else
			m_arrayContainer[i]->CreateEx(0, 0, 0, WS_CHILD|WS_VISIBLE, rectWnd, this, 0);
	}

	SCROLLINFO info;
	info.cbSize = sizeof(SCROLLINFO);
	info.nMin = 0;
	info.nMax = rows-1;
	info.nPage = 1;
	info.nTrackPos = info.nMax;
	info.nPos = GetScrollPos(SB_VERT);
	info.fMask = SIF_ALL;
	SetScrollInfo(SB_VERT, &info);
}

void CChildView::OnSize(UINT nType, int cx, int cy) 
{
	CWnd ::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	SetScrollPos(SB_VERT, 0);
	CreateSpyWnds();
}

void CChildView::ClearContainer()
{
	CWnd *pWnd = NULL;
	int size = m_arrayContainer.GetSize();
	for (int i = 0; i < size; i++)
	{
		pWnd = m_arrayContainer[i];
		if (pWnd != NULL)
		{
			if (pWnd->GetSafeHwnd())
			{
				pWnd->ShowWindow(SW_HIDE);
				pWnd->SendMessage(WM_CLOSE);
			}
			delete pWnd;
		}
	}
	m_arrayContainer.RemoveAll();
}

void CChildView::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	// TODO: Add your message handler code here and/or call default
	
	CWnd::OnVScroll(nSBCode, nPos, pScrollBar);

	int iNowPos = 0;
	switch(nSBCode)
	{
	case SB_LINEDOWN:
	case SB_PAGEDOWN:
		iNowPos=GetScrollPos(SB_VERT);
		iNowPos=iNowPos+1;
		SetScrollPos(SB_VERT, iNowPos);
		break;
	case SB_LINEUP:
	case SB_PAGEUP:
		iNowPos=GetScrollPos(SB_VERT);
		iNowPos=iNowPos-1;
		SetScrollPos(SB_VERT, iNowPos);
		break;
	}

	CreateSpyWnds();
}

void CChildView::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	CRect rect;
	GetClientRect(&rect);
	dc.FillSolidRect(rect, LoadAsRGB(_T("view.frame.color"), _T("255,255,255")));
}

CString CChildView::GetLocalAddr()
{
	CString addr;
	if (m_arrayContainer.GetSize() > 0)
		addr = m_arrayContainer.GetAt(0)->m_strIpAddr;
	return addr;
}

void CChildView::InitLocalHost()
{
	char name[256];
	if (gethostname(name, 256) != SOCKET_ERROR)
	{
		hostent *he = gethostbyname(name);
		for (int i = 0; he->h_addr_list[i]!= NULL; i++)
		{
			in_addr addr = *(in_addr *)(he->h_addr_list)[i];
			CSpyedWnd *pWnd = new CSpyedWnd;
			pWnd->m_strIpAddr = inet_ntoa(addr);
			pWnd->m_strName = he->h_name;
			pWnd->m_strRemark = _T("localhost");
			m_arrayContainer.Add(pWnd);
		}
	}
}

void CChildView::OnRButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	CWnd::OnRButtonDown(nFlags, point);

	CMenuEx menu;
	ClientToScreen(&point);
	menu.CreatePopupMenu();
	menu.InsertMenu(0, MF_BYPOSITION|MF_STRING, 0, LoadAsString(_T("view.menu.add.text"), _T("&Add host ...")));
	menu.TrackPopupMenu(TPM_LEFTBUTTON|TPM_RIGHTBUTTON, point.x, point.y, this);
}

BOOL CChildView::OnCommand(WPARAM wParam, LPARAM lParam) 
{
	// TODO: Add your specialized code here and/or call the base class
	switch (wParam)
	{
	case 0:
		{
			CMacXFrame *dlg = new CMacXFrame();
			CAddIpConfWnd *pWnd = new CAddIpConfWnd(&m_szName, &m_szIpAddr, &m_szCommunity, &m_szRemark, &m_nPort, &m_isOkExit);
			dlg->SetChildFrame(pWnd);
			dlg->CreateModal(NULL, LoadAsString(_T("view.ip.add.dlg.caption"), _T("Add host ... ")), 0, CRect(0, 0, 300, 260), AfxGetMainWnd(), NULL, NULL, AfxGetMainWnd(), this, WM_MODAL_FINISHED);
		}
		break;
	}

	return CWnd::OnCommand(wParam, lParam);
}

LRESULT CChildView::OnModalFinished(WPARAM wParam, LPARAM lParam)
{
	if (!m_isOkExit)
		return TRUE;

	CSpyedWnd *pWnd = new CSpyedWnd;
	pWnd->m_strName = m_szName;
	pWnd->m_strIpAddr = m_szIpAddr;
	pWnd->m_nPort = m_nPort;
	pWnd->m_strCommunity = m_szCommunity;
	pWnd->m_strRemark = m_szRemark;
	m_arrayContainer.Add(pWnd);

	CreateSpyWnds();
	
	return TRUE;
}

void CChildView::DeleteSpyWnd(int index)
{
	CSpyedWnd *pWnd = m_arrayContainer[index];
	if (pWnd != NULL)
	{
		if (pWnd == m_pFocusWnd)
			m_pFocusWnd = NULL;

		if (pWnd->GetSafeHwnd())
		{
			pWnd->ShowWindow(SW_HIDE);
			pWnd->SendMessage(WM_CLOSE);
		}
		delete pWnd;
	}
	m_arrayContainer.RemoveAt(index);
	CreateSpyWnds();
}