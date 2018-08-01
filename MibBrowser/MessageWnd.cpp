// MessageWnd.cpp : implementation file
//

#include "stdafx.h"
#include "mibbrowser.h"
#include "MessageWnd.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMessageWnd

CMessageWnd::CMessageWnd(CString msg)
{
	message = msg;
	font = new CFont();
	font->CreatePointFont(LoadAsInteger(_T("msgbox.font.size"), 90), LoadAsString(_T("msgbox.font.name"), _T("Arial Bold")));
}

CMessageWnd::~CMessageWnd()
{
	font->DeleteObject();
	delete font;
}


BEGIN_MESSAGE_MAP(CMessageWnd, CWnd)
	//{{AFX_MSG_MAP(CMessageWnd)
	ON_WM_CREATE()
	ON_WM_ERASEBKGND()
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CMessageWnd message handlers

BOOL CMessageWnd::OnCommand(WPARAM wParam, LPARAM lParam) 
{
	// TODO: Add your specialized code here and/or call the base class
	switch (wParam)
	{
	case IDOK:
		return GetParent()->PostMessage(WM_CLOSE);
		break;
	}
	
	return CWnd::OnCommand(wParam, lParam);
}

int CMessageWnd::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here
	m_ctrlOk.Create(LoadAsString(_T("msgbox.ok.text"), _T("&Ok")), WS_CHILD|WS_VISIBLE, CRect(0,0,0,0), this, IDOK);
	m_ctrlOk.SetFont(font);

	return ShowWindow(SW_SHOW);
}

BOOL CMessageWnd::OnEraseBkgnd(CDC* pDC) 
{
	// TODO: Add your message handler code here and/or call default
	CRect rect;
	GetClientRect(rect);
	
	CFont *pOldFont = pDC->SelectObject(font);
	pDC->FillSolidRect(rect, LoadAsRGB(_T("msgbox.color"), _T("255,255,255")));
	pDC->SetTextColor(LoadAsRGB(_T("msgbox.font.color"), _T("8,0,0")));
	rect.bottom -= 40;
	pDC->DrawText(message, rect, DT_CENTER|DT_VCENTER|DT_SINGLELINE);
	pDC->SelectObject(pOldFont);
	
	return TRUE;
}

void CMessageWnd::OnSize(UINT nType, int cx, int cy) 
{
	CWnd::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	CRect rect;
	GetClientRect(rect);

	CSize size(75, 24);
	int x = (rect.Width()-size.cx)/2;
	int y = rect.bottom-24-16;
	if (x < 0) x =0;
	if (y < 0) y = 0;
	rect.SetRect(x, y, x+size.cx, y+size.cy);

	m_ctrlOk.MoveWindow(rect);
}
