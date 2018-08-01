// AboutWnd.cpp : implementation file
//

#include "stdafx.h"
#include "mibbrowser.h"
#include "AboutWnd.h"
#include "RegKeyEx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAboutWnd

CAboutWnd::CAboutWnd()
{
	font = new CFont();
	font->CreatePointFont(LoadAsInteger(_T("about.dlg.font.size"), 90), LoadAsString(_T("about.dlg.font.name"), _T("Arial Bold")));
}

CAboutWnd::~CAboutWnd()
{
	font->DeleteObject();
	delete font;
}


BEGIN_MESSAGE_MAP(CAboutWnd, CWnd)
	//{{AFX_MSG_MAP(CAboutWnd)
	ON_WM_CREATE()
	ON_WM_ERASEBKGND()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CAboutWnd message handlers

int CAboutWnd::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here
	m_ctrlOk.Create(LoadAsString(_T("about.dlg.ok.text"), _T("&Ok")), WS_VISIBLE|WS_CHILD, CRect(270, 16, 345, 40), this, IDOK);
	m_ctrlOk.SetFont(font);
		
	return ShowWindow(SW_SHOW);
}

BOOL CAboutWnd::OnEraseBkgnd(CDC* pDC) 
{
	// TODO: Add your message handler code here and/or call default
	CDC dcmem;
	dcmem.CreateCompatibleDC(pDC);

	CRect rect;
	GetClientRect(&rect);

	CBitmap bmp;
	bmp.CreateCompatibleBitmap(pDC, rect.Width(), rect.Width());
	CBitmap *pOldBmp = dcmem.SelectObject(&bmp);
	dcmem.FillSolidRect(rect, LoadAsRGB(_T("about.dlg.color"), _T("255,255,255")));

	CString szText, sss;
	sss.LoadString(IDR_MAINFRAME);
	szText.Format(_T("%s 1.0.1 %s"), sss, LoadAsString(_T("about.dlg.version.text"), _T("version")));
	DrawLabel(&dcmem, CRect(65, 12, 265, 33), szText);
	szText.Format(_T("%s 2012 Asiainfo-Linkage Corporation"), LoadAsString(_T("about.dlg.copyright.text"), _T("Copyright (C)")));
	DrawLabel(&dcmem, CRect(65, 44, 335, 65), szText);
	szText.Format(_T("%s: YaoQing"), LoadAsString(_T("about.dlg.author.text"), _T("Author")));
	DrawLabel(&dcmem, CRect(65, 76, 335, 97), szText);
	
	CRegKeyEx reg;
	HKEY hKey = reg.OpenReg(_T("SOFTWARE\\AILK\\MibBrowser"));
	CString szOwner = reg.GetKeyValue(hKey, _T("Owner"));
	reg.CloseReg(hKey);
	if (g_bIsRegist)
		szText.Format(_T("%s%s"), LoadAsString(_T("about.dlg.registered.text"), _T("This software is registered to: ")), szOwner);
	else
		szText.Format(_T("%s"), LoadAsString(_T("about.dlg.unregistered.text"), _T("Unregistered version")));
	DrawLabel(&dcmem, CRect(65, 108, 335, 129), szText);

	HICON hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_MAINFRAME));
	dcmem.DrawIcon(CPoint(16, 28), hIcon);

	pDC->BitBlt(0, 0, rect.Width(), rect.Height(), &dcmem, 0, 0, SRCCOPY);

	dcmem.SelectObject(pOldBmp);
	return TRUE;
}

void CAboutWnd::DrawLabel(CDC *pDC, CRect &rect, CString szText)
{
	CFont *pOldFont = pDC->SelectObject(font);
	
	pDC->SetTextColor(LoadAsRGB(_T("about.dlg.font.color"), _T("8,0,0")));
	pDC->DrawText(szText, rect, DT_LEFT|DT_BOTTOM|DT_SINGLELINE);

	pDC->SelectObject(pOldFont);
}

BOOL CAboutWnd::OnCommand(WPARAM wParam, LPARAM lParam) 
{
	// TODO: Add your specialized code here and/or call the base class
	if (wParam == IDOK)
		return GetParent()->PostMessage(WM_CLOSE);
	
	return CWnd::OnCommand(wParam, lParam);
}
