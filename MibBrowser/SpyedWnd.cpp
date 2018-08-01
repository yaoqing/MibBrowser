// SpyedWnd.cpp : implementation file
//

#include "stdafx.h"
#include "MibBrowser.h"
#include "SpyedWnd.h"
#include "ChildView.h"
#include "MemoWnd.h"
#include "MenuEx.h"
#include "MacXFrame.h"
#include "StringEx.h"
#include "NewGuard.h"
#include "PromptWnd.h"
#include "PropertyWnd.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

int CSpyedWnd::SPY_WND_WIDTH = 100;           //被监测窗口的宽度
int CSpyedWnd::SPY_WND_HEIGHT = 100;          //被监测窗口的高度
int CSpyedWnd::SPY_WND_ICON_WIDTH = 64;       //被监测窗口中图标的宽度
int CSpyedWnd::SPY_WND_ICON_HEIGHT = 64;      //被监测窗口中图标的高度

IMPLEMENT_DYNCREATE(CSpyedWnd, CWnd)
/////////////////////////////////////////////////////////////////////////////
// CSpyedWnd

CSpyedWnd::CSpyedWnd()
{
	m_nPos = 0;
	m_pointWnd.x = 0;
	m_pointWnd.y = 0;
	m_bIsCapture = FALSE;
	m_bIsClicked = FALSE;

	m_nPort = LoadAsInteger(_T("snmp.udp.port"), 161);
	m_strCommunity = LoadAsString(_T("snmp.community.read"), _T("public"));

	BITMAP bmp;
	CBitmap bmpIcon;
	bmpIcon.LoadBitmap(IDB_COMPUTER);
	bmpIcon.GetBitmap(&bmp);
	SPY_WND_ICON_WIDTH = bmp.bmWidth;
	SPY_WND_ICON_HEIGHT = bmp.bmHeight;
	m_listImgs.Create(SPY_WND_ICON_WIDTH, SPY_WND_ICON_HEIGHT, ILC_COLOR32 | ILC_MASK, 1, 1);
	m_listImgs.Add(&bmpIcon, RGB(0, 0, 0));
}

CSpyedWnd::~CSpyedWnd()
{
}


BEGIN_MESSAGE_MAP(CSpyedWnd, CWnd)
	//{{AFX_MSG_MAP(CSpyedWnd)
	ON_WM_ERASEBKGND()
	ON_WM_RBUTTONDOWN()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_CREATE()
	ON_WM_TIMER()
	ON_WM_MOUSEMOVE()
	ON_REGISTERED_MESSAGE(WM_CAPTURED, OnCaptured)
	ON_WM_LBUTTONDOWN()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CSpyedWnd message handlers

BOOL CSpyedWnd::OnEraseBkgnd(CDC* pDC) 
{
	CRect rect;
	GetClientRect(rect);
	CDC dc;
	dc.CreateCompatibleDC(pDC);
	CBitmap bitmap;
	bitmap.CreateCompatibleBitmap(pDC, rect.Width(), rect.Height());
	dc.SelectObject(&bitmap);

	CFont font;
	font.CreatePointFont(LoadAsInteger(_T("spy.font.size"), 90), LoadAsString(_T("spy.font.name"), _T("Arial Bold")));
	CFont *pOldFont = dc.SelectObject(&font);
	dc.FillSolidRect(rect, RGB(255, 255, 255));
	
	CRect rectIcon = rect;
	rectIcon.DeflateRect((SPY_WND_WIDTH - SPY_WND_ICON_WIDTH) / 2, 0, 
		SPY_WND_WIDTH - (SPY_WND_WIDTH - SPY_WND_ICON_WIDTH) / 2 - SPY_WND_ICON_WIDTH, SPY_WND_HEIGHT - SPY_WND_ICON_HEIGHT);
	m_listImgs.Draw(&dc, 0, CPoint(rectIcon.left, rectIcon.top), ILD_NORMAL);

	dc.SelectObject(GetParent()->GetFont());
	CString strCaption;
	strCaption.Format(_T("%s"), m_strIpAddr);
	CSize round = dc.GetTextExtent(strCaption);
	dc.SetTextColor(GetSysColor(COLOR_WINDOWTEXT));
	dc.SetBkMode(TRANSPARENT);
	CRect rectText = rect;
	rectText.top = SPY_WND_ICON_HEIGHT;
	dc.DrawText(strCaption, rectText, DT_CENTER|DT_VCENTER|DT_WORDBREAK);
	//strCaption.Format(_T("%d"), m_nPort);
	strCaption.Format(_T("%s"), m_strName);
	rectText.top += round.cy;
	dc.DrawText(strCaption, rectText, DT_CENTER|DT_VCENTER|DT_WORDBREAK);
	
	pDC->BitBlt(0, 0, rect.Width(), rect.Height(), &dc, 0, 0, SRCCOPY);

	dc.SelectObject(pOldFont);
	ReleaseDC(&dc);

	if (m_bIsClicked) PostMessage(WM_CAPTURED, 2);
	return TRUE;
}

BOOL CSpyedWnd::PreCreateWindow(CREATESTRUCT& cs) 
{
	cs.dwExStyle = 0;
	cs.style &= ~WS_BORDER;

	return CWnd::PreCreateWindow(cs);
}

void CSpyedWnd::OnRButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	CWnd::OnRButtonDown(nFlags, point);
	
	OnClick();
 	CMenuEx menu;
	ClientToScreen(&point);
	menu.CreatePopupMenu();
	menu.InsertMenu(0, MF_BYPOSITION|MF_STRING, 0, LoadAsString(_T("spy.menu.touch.text"), _T("&Touch")));
	menu.InsertMenu(1, MF_BYPOSITION|MF_STRING, 1, LoadAsString(_T("spy.menu.walk.text"), _T("&Walk")));
	menu.InsertMenu(2, MF_BYPOSITION|MF_STRING, 2, LoadAsString(_T("spy.menu.prompt.text"), _T("Prompt For &OID ... ")));
	menu.InsertMenu(3, MF_BYPOSITION|MF_STRING, 3, LoadAsString(_T("spy.menu.delete.text"), _T("&Delete")));
	menu.InsertMenu(4, MF_BYPOSITION|MF_STRING, 4, LoadAsString(_T("spy.menu.property.text"), _T("&Property ... ")));
	
 	menu.TrackPopupMenu(TPM_LEFTALIGN|TPM_RIGHTBUTTON, point.x, point.y, this);
}

DWORD WINAPI CSpyedWnd::SNMPWalkProc(LPVOID lpParameter)
{
	CSpyedWnd *pthis = (CSpyedWnd *)lpParameter;
	bool null;
	int count = 0;
	int port = pthis->m_nPort;
	DWORD valtype;
	char *oid, *val, *ptr;
	char *sip = CStringEx(pthis->m_strIpAddr).toChar();
	char *scommunity = CStringEx(pthis->m_strCommunity).toChar();
	CNewGuard guard1(sip);
	CNewGuard guard2(scommunity);

	SNMPAPI_STATUS status;
	int timeout = LoadAsInteger(_T("snmp.udp.timeout"), 6);
	CStringEx sss, msg(CString("***** SNMP QUERY STARTED *****"));
	g_wndMemo->PostMessage(WM_MEMO_ADD_LINE, SET_TEXT, MakeMemoTextInfo(msg.toChar(), LoadAsRGB(_T("memo.mib.text.color"), _T("128,128,128"))));

	do 
	{
		ptr = sss.toChar();
		CNewGuard guard(ptr);
		status = GetSNMPVal(sip, 0, ptr, scommunity, port, timeout, SNMP_PDU_GETNEXT, oid, val, null, valtype);
		if ((status == SNMPAPI_SUCCESS) && oid && !null)
		{
			sss = oid;
			g_wndMemo->PostMessage(WM_MEMO_ADD_LINE, OID_VAL, MakeOidQueryInfo(++count, oid, val, valtype));
		}
		else
		{
			free(oid);
			free(val);
		}
	} while(!null);

	msg = "***** SNMP QUERY FINISHED *****";
	g_wndMemo->PostMessage(WM_MEMO_ADD_LINE, SET_TEXT, MakeMemoTextInfo(msg.toChar(), LoadAsRGB(_T("memo.mib.text.color"), _T("128,128,128"))));
	return g_wndMemo->PostMessage(WM_MEMO_ADD_LINE);
}

DWORD WINAPI CSpyedWnd::SNMPTouchProc(LPVOID lpParameter)
{
	CSpyedWnd *pthis = (CSpyedWnd *)lpParameter;
	CChildView *pwnd = (CChildView *)pthis->pWnd;
	bool null;
	int count = 0;
	DWORD valtype;
	char *oid, *val, *ptr;
	char *sip = CStringEx(pthis->m_strIpAddr).toChar();
	char *scommunity = CStringEx(pthis->m_strCommunity).toChar();
	CNewGuard guard1(sip);
	CNewGuard guard2(scommunity);

	CStringEx msg, sss(CString("1.3.6.1.2.1.1.1"));
	int timeout = LoadAsInteger(_T("snmp.udp.timeout"), 6);
	COLORREF color = LoadAsRGB(_T("memo.mib.text.color"), _T("128,128,128"));
	CString raddr = LoadAsString(_T("memo.remote.addr.text"), _T("Remote address"));
	CString rport = LoadAsString(_T("memo.remote.port.text"), _T("port"));
	CString laddr = LoadAsString(_T("memo.local.addr.text"), _T("Local address"));
	CString lport = LoadAsString(_T("memo.local.port.text"), _T("port"));
	CString type = LoadAsString(_T("memo.protocol.type.text"), _T("transport"));
	CString verion = LoadAsString(_T("memo.protocol.version.text"), _T("Protocol version"));
	SOCKET so = (SOCKET)SnmpCreateSession();

	CString line;
	line.Format(_T("%s: %s %s: %d %s: IP/UDP"), raddr, pthis->m_strIpAddr, rport, pthis->m_nPort, type);
	msg = line;
	g_wndMemo->PostMessage(WM_MEMO_ADD_LINE, SET_TEXT, MakeMemoTextInfo(msg.toChar(), color));
	line.Format(_T("%s: %s %s: %d %s: IP/UDP"), laddr, pwnd->GetLocalAddr(), lport, so, type);
	msg = line;
	g_wndMemo->PostMessage(WM_MEMO_ADD_LINE, SET_TEXT, MakeMemoTextInfo(msg.toChar(), color));
	line.Format(_T("%s: SNMPv1"), verion);
	msg = line;
	g_wndMemo->PostMessage(WM_MEMO_ADD_LINE, SET_TEXT, MakeMemoTextInfo(msg.toChar(), color));

	ptr = sss.toChar();
	CNewGuard guard(ptr);
	SNMPAPI_STATUS status;
	status = GetSNMPVal(sip, 0, ptr, scommunity, pthis->m_nPort, timeout, SNMP_PDU_GETNEXT, oid, val, null, valtype, so);

	if ((status == SNMPAPI_SUCCESS) && oid && !null)
		g_wndMemo->PostMessage(WM_MEMO_ADD_LINE, OID_VAL, MakeOidQueryInfo(++count, oid, val, valtype));
	else
	{
		free(oid);
		free(val);
		msg = LoadAsString(_T("snmp.error.text"), _T("SNMP not be supported or timeout!"));
		g_wndMemo->PostMessage(WM_MEMO_ADD_LINE, SET_TEXT, MakeMemoTextInfo(msg.toChar(), color));
	}

	return g_wndMemo->PostMessage(WM_MEMO_ADD_LINE);
}

void CSpyedWnd::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	CWnd::OnLButtonDblClk(nFlags, point);
	CloseHandle(CreateThread(NULL, 0, SNMPWalkProc, this, 0, NULL));
}

int CSpyedWnd::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here
	EnableToolTips();
	m_tt.Create(this);
	m_tt.Activate(TRUE);
	CString tips;
	tips.Format(_T("%s:%d  %s  %s"), m_strIpAddr, m_nPort, m_strName, m_strRemark);
	//m_tt.AddTool(this, tips);
	m_tt.AddTool(this, LPSTR_TEXTCALLBACK, CRect(0, 0, SPY_WND_WIDTH, SPY_WND_HEIGHT), 1);

	if (m_strName.GetLength() == 0 || m_strRemark.GetLength() == 0)
		SetTimer(1, 1000, NULL);

	return 0;
}

BOOL CSpyedWnd::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	m_tt.RelayEvent(pMsg);
	
	return CWnd::PreTranslateMessage(pMsg);
}

BOOL CSpyedWnd::OnTipText(UINT id, NMHDR* pNMHDR, LRESULT* pResult)
{
	TOOLTIPTEXT *pTTT = (TOOLTIPTEXT *)pNMHDR; 
	CString tips;
	tips.Format(_T("%s:%d  %s  %s"), m_strIpAddr, m_nPort, m_strName, m_strRemark);
	_tcscpy(pTTT->lpszText, tips);
	return TRUE;
}

BOOL CSpyedWnd::OnCommand(WPARAM wParam, LPARAM lParam) 
{
	// TODO: Add your specialized code here and/or call the base class
	CWnd::OnCommand(wParam, lParam);
	
	pWnd = GetParent();
	switch (wParam)
	{
	case 0:
		CloseHandle(CreateThread(NULL, 0, SNMPTouchProc, this, 0, NULL));
		break;
	case 1:
		CloseHandle(CreateThread(NULL, 0, SNMPWalkProc, this, 0, NULL));
		break;
	case 2:
		{
			CRect rect;
			GetWindowRect(rect);
			rect.right = rect.left + 472;
			rect.bottom = rect.top + 200;
			CMacXFrame *dlg = new CMacXFrame();
			CPromptWnd *pWnd = new CPromptWnd(this->m_strIpAddr, m_nPort, m_strCommunity);
			dlg->SetChildFrame(pWnd);
			dlg->CreateModal(NULL, _T("Prompt For OID"), 0, rect, AfxGetMainWnd(), 0, NULL, NULL);
		}
		break;
	case 3:
		((CChildView *)GetParent())->DeleteSpyWnd(m_nPos);
		break;
	case 4:
		{
			CString szText = m_strName;
			if (szText.GetLength() > 0)
				szText += _T(" - ");
			szText += m_strIpAddr;

			CMacXFrame *dlg = new CMacXFrame();
			CPropertyWnd *pWnd = new CPropertyWnd(m_strIpAddr, m_nPort, m_strName, m_strRemark, m_strCommunity);
			dlg->SetChildFrame(pWnd);
			dlg->CreateModal(NULL, szText, 0, CRect(0, 0, 450, 520), AfxGetMainWnd(), 0, NULL, AfxGetMainWnd());
		}
		break;
	}

	return TRUE;
}

void CSpyedWnd::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	bool bRefresh = false;
	int timeout = LoadAsInteger(_T("snmp.udp.timeout"), 6);
	char *ip = CStringEx(m_strIpAddr).toChar();
	CNewGuard guard1 = CNewGuard(ip);
	char *community = CStringEx(m_strCommunity).toChar();
	CNewGuard guard2 = CNewGuard(community);
	if (m_strName.GetLength() == 0)
	{	
		char *oid = "1.3.6.1.2.1.1.5.0";
		char *byoid, *byval;
		bool null = true;
		DWORD valtype = 0;
		SNMPAPI_STATUS status;
		status = ::GetSNMPVal(ip, 0, oid, community, m_nPort, timeout, SNMP_PDU_GET, byoid, byval, null, valtype);
		if ((status == SNMPAPI_SUCCESS) && oid && !null)
		{
			m_strName = byval;
			free(byoid);
			free(byval);
			bRefresh = true;
		}
	}
	
	if (m_strRemark.GetLength() == 0)
	{
		char *oid = "1.3.6.1.2.1.1.1.0";
		char *byoid, *byval;
		bool null = true;
		DWORD valtype = 0;
		SNMPAPI_STATUS status;
		status = ::GetSNMPVal(ip, 0, oid, community, m_nPort, timeout, SNMP_PDU_GET, byoid, byval, null, valtype);
		if ((status == SNMPAPI_SUCCESS) && oid && !null)
		{
			m_strRemark = byval;
			free(byoid);
			free(byval);
			bRefresh = true;
		}
	}

	CWnd::OnTimer(nIDEvent);
	KillTimer(1);
	if (bRefresh) Invalidate();
}

void CSpyedWnd::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	if (!m_bIsCapture)
	{
		SetCapture();
		m_bIsCapture = TRUE;
		if (!m_bIsClicked)
			PostMessage(WM_CAPTURED, 1);
	}
	else
	{
		CRect rect;
		GetClientRect(&rect);
		if (!PtInRect(&rect, point))
		{
			ReleaseCapture();
			m_bIsCapture = FALSE;
			if (!m_bIsClicked)
				PostMessage(WM_CAPTURED, 0);
		}
	}

	CWnd::OnMouseMove(nFlags, point);
}

void CSpyedWnd::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	OnClick();

	CWnd::OnLButtonDown(nFlags, point);
}

void CSpyedWnd::OnClick()
{
	CChildView *pWnd = ((CChildView *)GetParent());
	if (pWnd->m_pFocusWnd != NULL)
	{
		pWnd->m_pFocusWnd->SendMessage(WM_CAPTURED, 0);
		pWnd->m_pFocusWnd->m_bIsCapture = FALSE;
		pWnd->m_pFocusWnd->m_bIsClicked = FALSE;
	}

	m_bIsClicked = TRUE;
	pWnd->m_pFocusWnd = this;
	SendMessage(WM_CAPTURED, 2);
}

LRESULT CSpyedWnd::OnCaptured(WPARAM wParam, LPARAM lParam)
{
	CDC *pDC = GetDC();
	CRect rect;
	GetClientRect(&rect);
	rect.DeflateRect(1, 1, 1, 1);
	CPen pen, *pOldPen;
	
	switch (wParam)
	{
	case 0://鼠标离开
		pen.CreatePen(PS_SOLID, 1, LoadAsRGB(_T("view.frame.color"), _T("255,255,255")));
		break;
	case 1://鼠标划过
		pen.CreatePen(PS_DOT, 1, LoadAsRGB(_T("view.spy.border.move.corlor"), _T("184,204,228")));
		break;
	case 2://左键点击
		pen.CreatePen(PS_SOLID, 1, LoadAsRGB(_T("view.spy.border.click.corlor"), _T("184,204,228")));
		break;
	default:
		break;
	}

	pOldPen = pDC->SelectObject(&pen);
	pDC->MoveTo(rect.TopLeft());
	pDC->LineTo(rect.right, rect.top);
	pDC->LineTo(rect.BottomRight());
	pDC->LineTo(rect.left, rect.bottom);
	pDC->LineTo(rect.TopLeft());
	pDC->SelectObject(pOldPen);
	ReleaseDC(pDC);

	return TRUE;
}
