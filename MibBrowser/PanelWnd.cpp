// PanelWnd.cpp : implementation file
//

#include "stdafx.h"
#include "MibBrowser.h"
#include "PanelWnd.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNCREATE(CPanelWnd, CWnd)
/////////////////////////////////////////////////////////////////////////////
// CPanelWnd

CPanelWnd::CPanelWnd()
{
}

CPanelWnd::~CPanelWnd()
{
}


BEGIN_MESSAGE_MAP(CPanelWnd, CWnd)
	//{{AFX_MSG_MAP(CPanelWnd)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CPanelWnd message handlers

BOOL CPanelWnd::PreCreateWindow(CREATESTRUCT& cs) 
{
	//CBrush brush;
	cs.style = WS_CHILD | WS_VISIBLE;
	/*brush.CreateSolidBrush(RGB(255, 255, 255));
	cs.lpszClass = AfxRegisterWndClass(CS_HREDRAW|CS_VREDRAW|CS_DBLCLKS,
		::LoadCursor(NULL, IDC_ARROW), (HBRUSH)brush.Detach(), NULL);*/

	return CWnd::PreCreateWindow(cs);
}

void CPanelWnd::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	CRect rect;
	GetClientRect(&rect);
	dc.FillSolidRect(rect, RGB(255, 255, 255));
}
