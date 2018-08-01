// MacXFrame.cpp : implementation file
//

#include "stdafx.h"
#include "MibBrowser.h"
#include "MacXFrame.h"
#include "LangLoader.h"
#include "md5.h"
#include "base64.h"
#include "RegKeyEx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

UINT CMacXFrame::WM_INITFRAME = RegisterWindowMessage(_T("WM_INITFRAME"));
/////////////////////////////////////////////////////////////////////////////
// CMacXFrame

CMacXFrame::CMacXFrame()
{
	CY_CAPTION = 25;
	CY_MENU_BAR = 25;
	m_client_wnd = 0;
	m_notify_wnd = 0;
	m_modal_owner_wnd = 0;
	m_notify_message = WM_NULL;
}

CMacXFrame::~CMacXFrame()
{
}


BEGIN_MESSAGE_MAP(CMacXFrame, CWnd)
	//{{AFX_MSG_MAP(CMacXFrame)
	ON_WM_NCCALCSIZE()
	ON_WM_NCPAINT()
	ON_WM_SIZE()
	ON_WM_NCHITTEST()
	ON_WM_SYSCOMMAND()
	ON_WM_NCLBUTTONDBLCLK()
	ON_WM_GETMINMAXINFO()
	ON_WM_CLOSE()
	ON_REGISTERED_MESSAGE(WM_INITFRAME, OnInitFrame)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CMacXFrame message handlers

void CMacXFrame::SetChildFrame(CWnd *child)
{
	m_client_wnd = child;
}

BOOL CMacXFrame::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext)
{
	if (!CreateEx(0, lpszClassName, lpszWindowName,	0,
		rect.left, rect.top,
		rect.right - rect.left, rect.bottom - rect.top,
		pParentWnd->GetSafeHwnd(), (HMENU)nID, (LPVOID)pContext))
		return FALSE;

	if (!m_bar_caption.Create(NULL, lpszWindowName, 0, CRect(0, 0, rect.right-rect.left, CY_CAPTION), this, 0))
		return FALSE;
	
	if (!pParentWnd && !m_bar_menu.Create(NULL, NULL, 0, CRect(0, CY_CAPTION, rect.right-rect.left, CY_CAPTION+CY_MENU_BAR), this, 0))
		return FALSE;

	if (m_client_wnd && !m_client_wnd->Create(NULL, NULL, 0, CRect(0, 0, 0, 0), this, 0))
		return FALSE;

	PostMessage(WM_INITFRAME, 0, 0);
	return TRUE;
}

HWND CMacXFrame::CreateModal(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext, CWnd *pOwnerWnd, CWnd *pNotifyWnd, UINT message)
{
	m_notify_wnd = pNotifyWnd;
	m_notify_message = message;
	m_modal_owner_wnd = pOwnerWnd;
	if (m_modal_owner_wnd && m_modal_owner_wnd->GetSafeHwnd())
	{
		CRect rectOwner;
		m_modal_owner_wnd->EnableWindow(FALSE);
		m_modal_owner_wnd->GetWindowRect(rectOwner);
		CRect rectSelf = rect;
		int cx = rectOwner.Width()-rectSelf.Width();
		int cy = rectOwner.Height()-rectSelf.Height();
		rectSelf.OffsetRect(cx/2, cy/2);
		m_modal_owner_wnd->ClientToScreen(rectSelf);
		rect = rectSelf;
	}
	if (!Create(lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, nID, pContext))
		return (HWND)INVALID_HANDLE_VALUE;
	return GetSafeHwnd();
}

BOOL CMacXFrame::PreCreateWindow(CREATESTRUCT& cs) 
{
	cs.lpszClass = AfxRegisterWndClass(CS_HREDRAW|CS_VREDRAW|CS_DBLCLKS,
		::LoadCursor(NULL, IDC_ARROW), NULL, NULL);

	return TRUE;
}

void CMacXFrame::OnNcCalcSize(BOOL bCalcValidRects, NCCALCSIZE_PARAMS FAR* lpncsp) 
{
	CRect rect = lpncsp->rgrc[0];
	rect.DeflateRect(1, 1, 1, 1);
	lpncsp->rgrc[0] = rect;
}

LRESULT CMacXFrame::OnInitFrame(WPARAM wParam, LPARAM lParam)
{
	DWORD dwStyle = GetWindowLong(m_hWnd, GWL_STYLE);
	dwStyle = WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_SYSMENU | WS_THICKFRAME | WS_MINIMIZEBOX | WS_MAXIMIZEBOX;
	SetWindowLong(m_hWnd, GWL_STYLE, dwStyle);
	//SetWindowLong(m_hWnd, GWL_EXSTYLE, 0x02000000);
	ShowWindow(SW_HIDE);
	ShowWindow(SW_SHOW);

	CRect rect;
	GetClientRect(rect);
	AdjustChildWnd(rect.Width(), rect.Height());

	CRegKeyEx reg;
	HKEY hKey = reg.OpenReg(_T("SOFTWARE\\AILK\\MibBrowser"));
	CString szText;
	szText.LoadString(IDR_MAINFRAME);
	CString szOwner = reg.GetKeyValue(hKey, _T("Owner"))+szText;
	CString szSerial = reg.GetKeyValue(hKey, _T("Serial"));
	CString szOrganization = reg.GetKeyValue(hKey, _T("Organization"));
	
	MD5_CTX md5;
	char buff[32]={0};
#ifdef _UNICODE
	int len = ::WideCharToMultiByte(CP_ACP, 0, szOwner, szOwner.GetLength(), NULL, 0, 0, 0);
	char *mbOwner = new char[len];
	WideCharToMultiByte(CP_ACP, 0, szOwner, szOwner.GetLength(), mbOwner, len, 0, 0);
	md5.MD5Update((BYTE *)mbOwner, len);
	delete [] mbOwner;
#else
	md5.MD5Update((BYTE *)(LPCTSTR)szOwner, szOwner.GetLength()*sizeof(TCHAR));
#endif	
	md5.MD5Final((PBYTE)buff);

	CString szEncrypt;
	PULONG pEncrpt = (PULONG)buff;
	for (int i = 0; i < 4; i++)
		szEncrypt += b64_encode(pEncrpt[i]);

	//g_bIsRegist = (szEncrypt == szSerial);
	g_bIsRegist = TRUE;
	reg.CloseReg(hKey);
	return TRUE;
}

void CMacXFrame::OnNcPaint()
{
	CRect rectWnd;
	GetWindowRect(rectWnd);
	rectWnd.OffsetRect(-rectWnd.left, -rectWnd.top);
	CWindowDC dcWnd(this);

	CPen pen;
	pen.CreatePen(PS_SOLID, 1, RGB(152, 152, 152));
	CPen *pOldPen = dcWnd.SelectObject(&pen);
	dcWnd.MoveTo(rectWnd.left, rectWnd.top);
	dcWnd.LineTo(rectWnd.right-1, rectWnd.top);
	dcWnd.LineTo(rectWnd.right-1, rectWnd.bottom-1);
	dcWnd.LineTo(rectWnd.left, rectWnd.bottom-1);
	dcWnd.LineTo(rectWnd.left, rectWnd.top);

	dcWnd.SelectObject(pOldPen);
	pen.DeleteObject();
}

void CMacXFrame::OnSize(UINT nType, int cx, int cy) 
{
	CWnd::OnSize(nType, cx, cy);
	AdjustChildWnd(cx, cy);
}

UINT CMacXFrame::OnNcHitTest(CPoint point) 
{
	CRect rect;
	rect = m_bar_caption.m_rectBoxes[3];
	rect.InflateRect(1, 1);
	ClientToScreen(rect);
	if (PtInRect(rect, point) && m_bar_menu.GetSafeHwnd())
		return HTSYSMENU;

	GetWindowRect(rect);
	rect.DeflateRect(1, 1, 1, 1);
	rect.bottom = rect.top + CY_CAPTION;
	if (PtInRect(rect, point))
		return HTCAPTION;
	
	return CWnd::OnNcHitTest(point);
}

void CMacXFrame::OnSysCommand(UINT nID, LPARAM lParam) 
{
	if (nID == SC_MAXIMIZE)
	{
		m_bar_caption.m_bMaxed = true;
		m_bar_caption.m_bInMax = false;
	}
	else if (nID == SC_RESTORE)
	{
		m_bar_caption.m_bMaxed = false;
		m_bar_caption.m_bInMax = false;
	}

	CWnd::OnSysCommand(nID, lParam);
}

void CMacXFrame::OnNcLButtonDblClk(UINT nHitTest, CPoint point) 
{
	if (!m_bar_menu.GetSafeHwnd()) return;
	if (m_bar_caption.m_bMaxed)
		PostMessage(WM_SYSCOMMAND, SC_RESTORE);
	else
		PostMessage(WM_SYSCOMMAND, SC_MAXIMIZE);
}

void CMacXFrame::OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI) 
{
	lpMMI->ptMaxPosition.x = 0;
	lpMMI->ptMaxPosition.y = 0;
	lpMMI->ptMinTrackSize.x = 240;
	lpMMI->ptMinTrackSize.y = CY_CAPTION;
	CRect rect;
	HWND hDesktop = ::GetDesktopWindow();
	::GetWindowRect(hDesktop, rect);
	lpMMI->ptMaxSize.x = rect.Width();
	lpMMI->ptMaxSize.y = rect.Height();
	lpMMI->ptMaxTrackSize.x = rect.Width();
	lpMMI->ptMaxTrackSize.y = rect.Height();
	return;
	HWND hWnd = ::FindWindowEx(hDesktop, NULL, _T("Shell_TrayWnd"), _T(""));
	::GetWindowRect(hWnd, rect);
	lpMMI->ptMaxSize.y -= rect.Height();
}

void CMacXFrame::AdjustChildWnd(int cx, int cy)
{
	if (m_bar_caption.GetSafeHwnd())
		m_bar_caption.MoveWindow(CRect(0, 0, cx, CY_CAPTION));
	if (m_bar_menu.GetSafeHwnd())
		m_bar_menu.MoveWindow(CRect(0, CY_CAPTION, cx, CY_CAPTION+CY_MENU_BAR));
	if (m_client_wnd->GetSafeHwnd())
	{
		if (m_bar_menu.GetSafeHwnd())
			m_client_wnd->MoveWindow(CRect(0, CY_CAPTION+CY_MENU_BAR, cx, cy));
		else
			m_client_wnd->MoveWindow(CRect(0, CY_CAPTION, cx, cy));
	}
}

void CMacXFrame::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	if (m_client_wnd && m_client_wnd->GetSafeHwnd())
	{
		m_client_wnd->SendMessage(WM_CLOSE);
		delete m_client_wnd;
	}

	if (m_modal_owner_wnd && m_modal_owner_wnd->GetSafeHwnd())
	{
		//m_modal_owner_wnd->SetForegroundWindow();
		m_modal_owner_wnd->EnableWindow(TRUE);
		if (m_notify_wnd && m_notify_wnd->GetSafeHwnd())
			m_notify_wnd->PostMessage(m_notify_message);
	}

	CWnd::OnClose();
	delete this;
	
	CWnd *pWnd = AfxGetMainWnd();
	if (pWnd->GetSafeHwnd())
		pWnd->SetForegroundWindow();
}
