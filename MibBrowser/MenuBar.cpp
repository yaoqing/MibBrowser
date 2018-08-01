// MenuBar.cpp : implementation file
//

#include "stdafx.h"
#include "MibBrowser.h"
#include "MenuBar.h"
#include "MacXFrame.h"
#include "AboutWnd.h"
#include "StringToken.h"
#include "LangLoader.h"
#include "RegisterWnd.h"
#include "ChildView.h"
#include "SpyedWnd.h"
#include "PromptWnd.h"
#include "PropertyWnd.h"
#include "AddIpConfWnd.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMenuBar

CMenuBar::CMenuBar()
{
	CX_OFFSET_MENU = 20;
	CX_OFFSET_GRIPE = 6;
	CX_MENU_ITEM_SPACE = 8;
	m_bInGripe = false;
	LoadGripe();
	hMenu = LoadDyncMenu();
	font.CreatePointFont(LoadAsInteger(_T("menu.font.size"), 90), LoadAsString(_T("menu.font.name"), _T("Arial")));
}

CMenuBar::~CMenuBar()
{
	font.DeleteObject();
	m_bmpHot.DeleteObject();
}


BEGIN_MESSAGE_MAP(CMenuBar, CWnd)
	//{{AFX_MSG_MAP(CMenuBar)
	ON_WM_NCCALCSIZE()
	ON_WM_ERASEBKGND()
	ON_WM_NCHITTEST()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CMenuBar message handlers

void CMenuBar::LoadGripe()
{
	BITMAP bmp;
	CBitmap img;
	img.LoadBitmap(IDB_GRIPE);
	img.GetBitmap(&bmp);
	m_listImgs.Create(bmp.bmWidth, bmp.bmHeight, ILC_COLOR24|ILC_MASK, 1, 1);
	m_listImgs.Add(&img, RGB(0, 0, 0));
	img.DeleteObject();
	img.LoadBitmap(IDB_GRIPE_HOT);
	m_listImgs.Add(&img, RGB(0, 0, 0));
	img.DeleteObject();

	m_bmpHot.LoadBitmap(IDB_MENU_HOT);
}

void CMenuBar::DrawGripe(CDC *pDC, bool bInGripe)
{
	CPoint pos;
	/*画把手图标*/
	pos.x = m_rectGripe.left;
	pos.y = m_rectGripe.top;
	int index = bInGripe ? 1 : 0;
	m_listImgs.Draw(pDC, index, pos, ILD_NORMAL);
}

void CMenuBar::DrawMenu(CDC *pDC, int index, bool bInHot)
{
	if (bInHot != m_bInMenus.GetAt(index))
	{
		CFont *pOldFont = pDC->SelectObject(&font);
		CRect rect = m_rectMenus.GetAt(index);
		m_bInMenus.SetAt(index, bInHot);

		if (bInHot)
		{
			rect.OffsetRect(1, 1);
			CPoint point = rect.TopLeft();
			rect = m_rectMenus.GetAt(index);
			rect.InflateRect(2, 2, 2, 3);
			CBrush brush;
			brush.CreatePatternBrush(&m_bmpHot);
			pDC->FillRect(rect, &brush);
			brush.DeleteObject();
			//pDC->FillSolidRect(rect, RGB(133, 133, 133));
			DrawMenuText(pDC, point.x, point.y, m_szMenus.GetAt(index));
		}
		else
		{
			rect.left -= 2;
			rect.top = 3;
			rect.bottom += 3;
			rect.right += 2;

			CBitmap bg;
			bg.LoadBitmap(IDB_MENU_BAR);

			CBrush brush;
			brush.CreatePatternBrush(&bg);
			pDC->FillRect(rect, &brush);

			pDC->SetBkMode(TRANSPARENT);
			rect = m_rectMenus.GetAt(index);
			DrawMenuText(pDC, rect.left, rect.top, m_szMenus.GetAt(index));
			//pDC->TextOut(rect.left, rect.top, m_szMenus.GetAt(index));

			brush.DeleteObject();
			bg.DeleteObject();			
		}

		pDC->SelectObject(pOldFont);
	}
}

void CMenuBar::DrawInHotGripe(CDC *pDC, bool &os, bool cs)
{
	if (os != cs)
	{
		os = cs;
		DrawGripe(pDC, cs);
	}
}

BOOL CMenuBar::PreCreateWindow(CREATESTRUCT& cs) 
{
	CBrush brush;
	cs.style = WS_CHILD | WS_VISIBLE;
	brush.CreateSolidBrush(RGB(233, 233, 233));
	cs.lpszClass = AfxRegisterWndClass(CS_HREDRAW|CS_VREDRAW|CS_DBLCLKS,
		::LoadCursor(NULL, IDC_ARROW), (HBRUSH)brush.Detach(), NULL);

	return TRUE;
}

void CMenuBar::OnNcCalcSize(BOOL bCalcValidRects, NCCALCSIZE_PARAMS FAR* lpncsp) 
{
	CWnd::OnNcCalcSize(bCalcValidRects, lpncsp);
	CRect rect = lpncsp->rgrc[0];
	m_rectMenus.RemoveAll();
	m_szMenus.RemoveAll();
	m_bInMenus.RemoveAll();

	/*设定把手坐标范围*/
	IMAGEINFO ii;
	m_listImgs.GetImageInfo(0, &ii);
	int width = ii.rcImage.right-ii.rcImage.left;
	int height = ii.rcImage.bottom-ii.rcImage.top;
	int top = (rect.Height()-height)/2;
	rect.SetRect(CX_OFFSET_GRIPE, top, CX_OFFSET_GRIPE+width, top+height);
	m_rectGripe = rect;

	/*设定菜单项坐标范围*/
	CClientDC dc(this);
	CFont *pOldFont = dc.SelectObject(&font);

	CX_OFFSET_MENU = CX_OFFSET_GRIPE+width+CX_OFFSET_GRIPE;
	int x = CX_OFFSET_MENU;
	rect = lpncsp->rgrc[0];
	int cy = rect.Height();
	CSize size;
	CString text;

	CMenu *pMenu = CMenu::FromHandle(hMenu);
	UINT count = pMenu->GetMenuItemCount();
	for (UINT i = 0; i < count; i++)
	{
		pMenu->GetMenuString(i, text, MF_BYPOSITION);
		size = GetMenuTextRect(&dc, text);
		int top = (cy - size.cy) / 2;
		int bottom = top + size.cy;
		rect.SetRect(x, top, x+size.cx, bottom);
		x += size.cx + CX_MENU_ITEM_SPACE;
		m_rectMenus.Add(rect);
		m_szMenus.Add(text);
		m_bInMenus.Add(false);
	}

	dc.SelectObject(pOldFont);
}

BOOL CMenuBar::OnEraseBkgnd(CDC* pDC) 
{
	CRect rect;
	GetClientRect(rect);

	/*准备兼容DC*/
	CDC dcMem;
	dcMem.CreateCompatibleDC(pDC);
	dcMem.SetBkMode(TRANSPARENT);

	/*画背景*/
	CBitmap bg, menu;
	bg.CreateCompatibleBitmap(pDC, rect.Width(), rect.Height());
	menu.LoadBitmap(IDB_MENU_BAR);
	CBrush brush;
	brush.CreatePatternBrush(&menu);
	CBitmap *pOldBmp = dcMem.SelectObject(&bg);
	dcMem.FillRect(rect, &brush);
	//dcMem.FillSolidRect(rect, RGB(152, 152, 152));

	/*画把手*/
	DrawGripe(&dcMem, m_bInGripe);

	/*画菜单*/
	CFont *pOldFont = dcMem.SelectObject(&font);
	for (int i = 0; i < m_szMenus.GetSize(); i++)
		DrawMenuText(&dcMem, m_rectMenus[i].left, m_rectMenus[i].top, m_szMenus.GetAt(i));

	/*从内存中拷贝至屏幕*/
	pDC->BitBlt(0, 0, rect.Width(), rect.Height(), &dcMem, 0, 0, SRCCOPY);

	/*清理恢复工作*/
	dcMem.SelectObject(pOldBmp);
	dcMem.SelectObject(pOldFont);
	dcMem.DeleteDC();
	bg.DeleteObject();
	menu.DeleteObject();
	brush.DeleteObject();

	return TRUE;
}

UINT CMenuBar::OnNcHitTest(CPoint point) 
{
	CRect rect;
	CWindowDC dc(this);
	ScreenToClient(&point);

	if (m_rectGripe.PtInRect(point))
	{
		DrawInHotGripe(&dc, m_bInGripe, true);
		return HTCLIENT;
	}

	CRect rectWnd;
	GetWindowRect(rectWnd);
	DrawInHotGripe(&dc, m_bInGripe, false);	

	for (int i = 0; i < m_rectMenus.GetSize(); i++)
	{
		rect = m_rectMenus.GetAt(i);
		if (PtInRect(rect, point))
		{
			DrawMenu(&dc, i, true);
			ClientToScreen(rect);
			CMenu *pMenu = CMenu::FromHandle(hMenu);
			if (i == 1)
			{
				CMenu *subMenu = pMenu->GetSubMenu(i);
				if (g_wndView->m_pFocusWnd)
					subMenu->EnableMenuItem(IDM_PROPERTY, MF_BYCOMMAND|MF_ENABLED);
				else
					subMenu->EnableMenuItem(IDM_PROPERTY, MF_BYCOMMAND|MF_DISABLED|MF_GRAYED);
			}
			pMenu->GetSubMenu(i)->TrackPopupMenu(TPM_LEFTBUTTON|TPM_RIGHTBUTTON, rect.left, rectWnd.bottom, this);
		}
		else
		{
			DrawMenu(&dc, i, false);
		}
	}

	return CWnd::OnNcHitTest(point);
}

BOOL CMenuBar::OnCommand(WPARAM wParam, LPARAM lParam) 
{
	// TODO: Add your specialized code here and/or call the base class
	Invalidate(TRUE);
	switch (wParam)
	{
	case IDM_EXIT:
		return GetParent()->PostMessage(WM_CLOSE);
	case IDM_ABOUT:
		{
			CMacXFrame *dlg = new CMacXFrame();
			CAboutWnd *pWnd = new CAboutWnd();
			dlg->SetChildFrame(pWnd);
			dlg->CreateModal(NULL, LoadAsString(_T("about.dlg.caption.text"), _T("SNMP MibBrowse")), 0, CRect(0, 0, 360, 180), AfxGetMainWnd(), 0, NULL, AfxGetMainWnd());
			return TRUE;
		}
	case IDM_REGISTER:
		{
			CMacXFrame *dlg = new CMacXFrame();
			CRegisterWnd *pWnd = new CRegisterWnd();
			dlg->SetChildFrame(pWnd);
			dlg->CreateModal(NULL, LoadAsString(_T("register.dlg.caption.text"), _T("Register")), 0, CRect(0, 0, 360, 180), AfxGetMainWnd(), 0, NULL, AfxGetMainWnd());
			return TRUE;
		}
	/*
	case IDM_BUYNOW:
		{
			CString url;
			if (g_strRes->GetCurrLangID() == 2052)
				url = _T("http://shareware.skycn.com/orderform.php?purtype=unreg&soft_id=11057");
			else
				url = _T("https://secure.shareit.com/shareit/checkout.html?productid=300119764&quickbuy=1");
			ShellExecute(NULL, _T("open"), url, NULL, NULL, SW_SHOWNORMAL);
			return TRUE;
		}
	case IDM_HOMEPAGE:
		{
			CString url;
			if (g_strRes->GetCurrLangID() == 2052)
				url = _T("http://www.pease-soft.com/2052/index.htm");
			else
				url = _T("http://www.pease-soft.com/1033/index.htm");
			ShellExecute(NULL, _T("open"), url, NULL, NULL, SW_SHOWNORMAL);
			return TRUE;
		}
	*/
	case IDM_PROMPT:
		{
			CRect rect;
			CWnd *pMainWnd = AfxGetMainWnd();
			pMainWnd->GetClientRect(rect);
			rect.left = (rect.Width() - 472) / 2;
			rect.top = (rect.Height() - 200) / 2;
			if (rect.left < 0) rect.left = 0;
			if (rect.top < 0) rect.top = 0;
			rect.right = rect.left + 472;
			rect.bottom = rect.top + 200;
			pMainWnd->ClientToScreen(rect);

			CMacXFrame *dlg = new CMacXFrame();
			CString szIP;
			if (g_wndView->m_pFocusWnd) szIP = g_wndView->m_pFocusWnd->m_strIpAddr;
			CPromptWnd *pWnd = new CPromptWnd(szIP, LoadAsInteger(_T("snmp.udp.port"), 161), LoadAsString(_T("snmp.community.read"), _T("public")));
			dlg->SetChildFrame(pWnd);
			dlg->CreateModal(NULL, _T("Prompt For OID"), 0, rect, AfxGetMainWnd(), 0, NULL, NULL);
			return TRUE;
		}
	case IDM_PROPERTY:
		{
			CSpyedWnd *pSpyedWnd = g_wndView->m_pFocusWnd;
			CString szText = pSpyedWnd->m_strName;
			if (szText.GetLength() > 0)
				szText += _T(" - ");
			szText += pSpyedWnd->m_strIpAddr;

			CMacXFrame *dlg = new CMacXFrame();
			CPropertyWnd *pWnd = new CPropertyWnd(pSpyedWnd->m_strIpAddr, pSpyedWnd->m_nPort, pSpyedWnd->m_strName, pSpyedWnd->m_strRemark, pSpyedWnd->m_strCommunity);
			dlg->SetChildFrame(pWnd);
			dlg->CreateModal(NULL, szText, 0, CRect(0, 0, 450, 520), AfxGetMainWnd(), 0, NULL, AfxGetMainWnd());
			return TRUE;
		}
	case IDM_ADDHOST:
		{
			CMacXFrame *dlg = new CMacXFrame();
			CAddIpConfWnd *pWnd = new CAddIpConfWnd(&g_wndView->m_szName, &g_wndView->m_szIpAddr, &g_wndView->m_szCommunity, &g_wndView->m_szRemark, &g_wndView->m_nPort, &g_wndView->m_isOkExit);
			dlg->SetChildFrame(pWnd);
			dlg->CreateModal(NULL, LoadAsString(_T("view.ip.add.dlg.caption"), _T("Add host ... ")), 0, CRect(0, 0, 300, 260), AfxGetMainWnd(), NULL, NULL, AfxGetMainWnd(), g_wndView, WM_MODAL_FINISHED);
			return TRUE;
		}
	default:
		{
			CMenu *pMenu = CMenu::FromHandle(hMenu);
			CMenu *pSubMenu = pMenu->GetSubMenu(1);
			pSubMenu = pSubMenu->GetSubMenu(pSubMenu->GetMenuItemCount()-1);
			for (UINT i = 0; i < pSubMenu->GetMenuItemCount(); i++)
			{
				if (wParam != pSubMenu->GetMenuItemID(i))
					continue;

				BOOL bChecked = pSubMenu->GetMenuState(wParam, MF_CHECKED);
				if (bChecked == MF_UNCHECKED)
				{
					pSubMenu->CheckMenuItem(wParam, MF_CHECKED);
					g_strRes->InitStringRes(wParam);
					
					for (UINT j = 0; j < pSubMenu->GetMenuItemCount(); j++)
					{
						if (i == j) continue;
						bChecked = pSubMenu->GetMenuState(wParam, MF_CHECKED);
						if (bChecked == MF_CHECKED)
							pSubMenu->CheckMenuItem(pSubMenu->GetMenuItemID(j), MF_UNCHECKED);
					}
					
					ChangeLanguage();
					break;
				}
			}
		}
	}
	
	return CWnd::OnCommand(wParam, lParam);
}

void CMenuBar::DrawMenuText(CDC *pDC, int x, int y, CString &szText)
{
	CStringToken token(szText, _T('&'));
	if (token.CountToke() <= 0) return;

	CSize size;
	CString szSub = token.GetAt(0);
	size = pDC->GetTextExtent(szSub);
	pDC->SetBkMode(TRANSPARENT);
	pDC->TextOut(x, y, szSub);
	if (token.CountToke() == 1)
	{
		y += size.cy-2;
		pDC->MoveTo(x, y);
		size = pDC->GetTextExtent(szSub, 1);
		x += size.cx;
		pDC->LineTo(x, y);
	}
	else
	{
		x += size.cx;
		szSub = token.GetAt(1);
		pDC->TextOut(x, y, szSub);
		y += size.cy-2;
		pDC->MoveTo(x, y);
		size = pDC->GetTextExtent(szSub, 1);
		x += size.cx;
		pDC->LineTo(x, y);
	}
}

void CMenuBar::ChangeLanguage()
{
	UINT count = m_rectMenus.GetSize();
	if (count == 0) return;

	CSize size;
	CRect rect;
	CString text;
	GetClientRect(rect);
	int cy = rect.Height();
	int x = m_rectMenus.GetAt(0).left;
	
	m_szMenus.RemoveAll();
	m_bInMenus.RemoveAll();
	m_rectMenus.RemoveAll();
	
	DestroyMenu(hMenu);
	hMenu = LoadDyncMenu();
	CMenu *pMenu = CMenu::FromHandle(hMenu);

	CDC *pDC = GetDC();
	CFont *pOldFont = pDC->SelectObject(&font);
	for (UINT i = 0; i < count; i++)
	{
		pMenu->GetMenuString(i, text, MF_BYPOSITION);
		size = GetMenuTextRect(pDC, text);
		int top = (cy - size.cy) / 2;
		int bottom = top + size.cy;
		rect.SetRect(x, top, x+size.cx, bottom);
		x += size.cx + CX_MENU_ITEM_SPACE;
		m_rectMenus.Add(rect);
		m_szMenus.Add(text);
		m_bInMenus.Add(false);
	}
	pDC->SelectObject(pOldFont);
	ReleaseDC(pDC);
	Invalidate();
}

CSize CMenuBar::GetMenuTextRect(CDC *pDC, CString &szText)
{
	CString sss;
	CStringToken token(szText, _T('&'));
	for (int i = 0; i < token.CountToke(); i++)
		sss += token.GetAt(i);
	return pDC->GetTextExtent(sss);
}

HMENU CMenuBar::LoadDyncMenu()
{
	CMenu menu, submenu, lang_menu;
	menu.CreateMenu();
	submenu.CreateMenu();
	lang_menu.CreateMenu();
	submenu.AppendMenu(MF_STRING, IDM_EXIT, LoadAsString(_T("menu.file.exit.text"), _T("E&xit")));
	menu.AppendMenu(MF_STRING|MF_POPUP, (UINT)submenu.GetSafeHmenu(), LoadAsString(_T("menu.file.text"), _T("&File")));
	submenu.Detach();
	
	int lang_id;
	CString lang_name;
	CArray<int, int> array;
	g_strRes->GetLangID(array);
	for (int i = 0; i < array.GetSize(); i++)
	{
		lang_id = array.GetAt(i);
		lang_name = g_strRes->GetLangName(lang_id);
		if (g_strRes->GetCurrLangID() == lang_id)
			lang_menu.AppendMenu(MF_STRING|MF_CHECKED, lang_id, lang_name);
		else
			lang_menu.AppendMenu(MF_STRING, lang_id, lang_name);
	}

	submenu.CreateMenu();
	submenu.AppendMenu(MF_STRING, IDM_PROMPT, LoadAsString(_T("spy.menu.prompt.text"), _T("Prompt For &OID ... ")));
	submenu.AppendMenu(MF_STRING, IDM_PROPERTY, LoadAsString(_T("spy.menu.property.text"), _T("Host &Property ... ")));
	submenu.AppendMenu(MF_STRING, IDM_ADDHOST, LoadAsString(_T("view.menu.add.text"), _T("Add host ... ")));
	submenu.AppendMenu(MF_STRING|MF_POPUP, (UINT)lang_menu.GetSafeHmenu(), _T("&Language"));
	menu.AppendMenu(MF_STRING|MF_POPUP, (UINT)submenu.GetSafeHmenu(), LoadAsString(_T("menu.view.text"), _T("&View")));

	
	lang_menu.Detach();
	submenu.Detach();

	submenu.CreateMenu();
	// submenu.AppendMenu(MF_STRING, IDM_HOMEPAGE, LoadAsString(_T("menu.help.homepage.text"), _T("&Homepage ... ")));
	submenu.AppendMenu(MF_STRING, IDM_REGISTER, LoadAsString(_T("menu.help.register.text"), _T("&Register ... ")));
	// submenu.AppendMenu(MF_STRING, IDM_BUYNOW, LoadAsString(_T("menu.help.buynow.text"), _T("&Buy now ... ")));
	submenu.AppendMenu(MF_STRING, IDM_ABOUT, LoadAsString(_T("menu.help.about.text"), _T("&About ... ")));
	menu.AppendMenu(MF_STRING|MF_POPUP, (UINT)submenu.GetSafeHmenu(), LoadAsString(_T("menu.help.text"), _T("&Help")));
	submenu.Detach();

	return menu.Detach();
}
