// PromptWnd.cpp : implementation file
//

#include "stdafx.h"
#include "mibbrowser.h"
#include "PromptWnd.h"
#include "StringEx.h"
#include "NewGuard.h"
#include "MemoWnd.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPromptWnd

CPromptWnd::CPromptWnd(CString ip, int port, CString community)
{
	m_szIp = ip;
	m_nPort = port;
	m_szCommunity = community;
	m_get_type = SNMP_PDU_GETNEXT;
	m_szOID = _T("0.0");

	font = new CFont();
	font->CreatePointFont(LoadAsInteger(_T("spy.prompt.dlg.font.size"), 90), LoadAsString(_T("spy.prompt.dlg.font.name"), _T("Arial Bold")));
}

CPromptWnd::~CPromptWnd()
{
	font->DeleteObject();
	delete font;
}


BEGIN_MESSAGE_MAP(CPromptWnd, CWnd)
	//{{AFX_MSG_MAP(CPromptWnd)
	ON_WM_ERASEBKGND()
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CPromptWnd message handlers

void CPromptWnd::SetOID(CString szOID)
{
	m_szOID = szOID;
}

void CPromptWnd::DrawLabel(CDC *pDC, CRect &rect, CString szText)
{
	CFont *pOldFont = pDC->SelectObject(font);
	
	pDC->SetTextColor(LoadAsRGB(_T("spy.prompt.dlg.font.color"), _T("8,0,0")));
	pDC->DrawText(szText, rect, DT_LEFT|DT_BOTTOM|DT_SINGLELINE);

	pDC->SelectObject(pOldFont);
}

BOOL CPromptWnd::OnEraseBkgnd(CDC* pDC) 
{
	// TODO: Add your message handler code here and/or call default
	CRect rect;
	GetClientRect(&rect);
	pDC->FillSolidRect(rect, LoadAsRGB(_T("spy.prompt.dlg.color"), _T("255,255,255")));

	DrawLabel(pDC, CRect(25, 19, 98, 40), LoadAsString(_T("spy.prompt.dlg.ip.text"), _T("IP address:")));
	DrawLabel(pDC, CRect(290, 19, 343, 40), LoadAsString(_T("spy.prompt.dlg.port.text"), _T("Port:")));
	DrawLabel(pDC, CRect(25, 52, 98, 73), LoadAsString(_T("spy.prompt.dlg.community.text"), _T("Community:")));
	DrawLabel(pDC, CRect(290, 52, 353, 73), LoadAsString(_T("spy.prompt.dlg.timeout.text"), _T("Timeout(s):")));
	DrawLabel(pDC, CRect(25, 85, 98, 106), LoadAsString(_T("spy.prompt.dlg.oid.text"), _T("Object ID:")));

	return TRUE;
}

int CPromptWnd::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here
	m_ctrlIp.Create(WS_CHILD | WS_VISIBLE, CRect(100, 20, 260, 41), this, 0);
	m_ctrlCommunity.Create(WS_CHILD | WS_VISIBLE, CRect(100, 53, 260, 74), this, 1);
	m_ctrlPort.Create(WS_CHILD | WS_VISIBLE|ES_NUMBER, CRect(360, 20, 420, 41), this, 2);
	m_ctrlTimeout.Create(WS_CHILD | WS_VISIBLE|ES_NUMBER, CRect(360, 53, 420, 74), this, 3);
	m_ctrlOID.Create(WS_CHILD | WS_VISIBLE, CRect(100, 86, 420, 107), this, 4);

	m_ctrlUseResult.Create(LoadAsString(_T("spy.prompt.dlg.reuse.text"), _T("&Reuse result")),
		WS_CHILD|WS_VISIBLE|BS_CHECKBOX|BS_FLAT|BS_AUTOCHECKBOX, CRect(10, 116, 130, 140), this, 5);
	m_ctrlRecursive.Create(LoadAsString(_T("spy.prompt.dlg.recursive.text"), _T("&Recursive")),
		WS_CHILD|WS_VISIBLE|BS_CHECKBOX|BS_FLAT|BS_AUTOCHECKBOX, CRect(10, 140, 130, 164), this, 10);
	m_ctrlGet.Create(LoadAsString(_T("spy.prompt.dlg.get.text"), _T("&Get")), WS_CHILD | WS_VISIBLE, CRect(130, 128, 200, 152), this, 6);
	m_ctrlGetNext.Create(LoadAsString(_T("spy.prompt.dlg.getnext.text"), _T("Get&Next")), WS_CHILD | WS_VISIBLE, CRect(220, 128, 330, 152), this, 7);
	m_ctrlClose.Create(LoadAsString(_T("spy.prompt.dlg.close.text"), _T("&Close")), WS_CHILD | WS_VISIBLE, CRect(350, 128, 420, 152), this, 8);

	COLORREF color = LoadAsRGB(_T("spy.prompt.dlg.edit.border.color"), _T("49,105,198"));
	m_ctrlIp.SetBorderColor(color);
	m_ctrlCommunity.SetBorderColor(color);
	m_ctrlPort.SetBorderColor(color);
	m_ctrlTimeout.SetBorderColor(color);
	m_ctrlOID.SetBorderColor(color);

	m_ctrlIp.SetFont(font);
	m_ctrlCommunity.SetFont(font);
	m_ctrlPort.SetFont(font);
	m_ctrlTimeout.SetFont(font);
	m_ctrlOID.SetFont(font);

	CString sss;
	m_ctrlIp.SetWindowText(m_szIp);
	m_ctrlCommunity.SetWindowText(m_szCommunity);
	sss.Format(_T("%u"), m_nPort);
	m_ctrlPort.SetWindowText(sss);
	m_ctrlTimeout.SetWindowText(LoadAsString(_T("snmp.udp.timeout"), _T("6")));
	m_ctrlOID.SetWindowText(m_szOID);

	m_ctrlUseResult.SetColor(LoadAsRGB(_T("spy.prompt.dlg.color"), _T("255,255,255")));
	m_ctrlUseResult.SetCheck(TRUE);
	m_ctrlUseResult.SetFont(font);

	m_ctrlRecursive.SetColor(LoadAsRGB(_T("spy.prompt.dlg.color"), _T("255,255,255")));
	m_ctrlRecursive.SetCheck(FALSE);
	m_ctrlRecursive.SetFont(font);

	m_ctrlClose.SetFont(font);
	m_ctrlGet.SetFont(font);
	m_ctrlGetNext.SetFont(font);

	ShowWindow(SW_SHOW);
	
	return 0;
}

BOOL CPromptWnd::OnCommand(WPARAM wParam, LPARAM lParam) 
{
	// TODO: Add your specialized code here and/or call the base class
	switch (wParam)
	{
	case 6:
		m_get_type = SNMP_PDU_GET;
		OnQueryOID();
		break;
	case 7:
		m_get_type = SNMP_PDU_GETNEXT;
		OnQueryOID();
		break;
	case 8:
		GetParent()->PostMessage(WM_CLOSE);
		break;
	case 10:
		m_ctrlGet.EnableWindow(!m_ctrlRecursive.GetCheck());
	}
	return CWnd::OnCommand(wParam, lParam);
}

void CPromptWnd::OnQueryOID()
{
	CString szOid, szPort, szTimeout;
	m_ctrlOID.GetWindowText(szOid);
	m_ctrlIp.GetWindowText(m_szIp);
	m_ctrlPort.GetWindowText(szPort);
	m_ctrlTimeout.GetWindowText(szTimeout);
	m_ctrlCommunity.GetWindowText(m_szCommunity);

	char *sip = CStringEx(m_szIp).toChar();
	CNewGuard guard1(sip);
	if ((inet_addr(sip) == INADDR_NONE) || (m_szIp.GetLength() <= 0))
	{
		MsgBoxModal(LoadAsString(_T("ip.addr.error.text"), _T("IP wrong!")), GetParent());
		return;
	}

	m_nPort = _ttoi(szPort);
	if ((m_nPort <= 0) || (m_nPort >= 65536))
	{
		MsgBoxModal(LoadAsString(_T("port.error.text"), _T("Port wrong !")), GetParent());
		return;
	}

	int timeout = _ttoi(szTimeout);
	if (timeout < 0) timeout = 0;
	if (timeout > 60) timeout = 60;
	
	int count = 0;
	CString szBreakOid = szOid;
	do
	{
		smiLPOID OID;
		CStringEx sss(szOid);
		char *ptr = sss.toChar();
		SNMPAPI_STATUS status = strtooid(ptr, OID);
		FreeOid(OID);
		CNewGuard guard2(ptr);
		if (status == SNMPAPI_FAILURE)
		{
			MsgBoxModal(LoadAsString(_T("oid.error.text"), _T("OID wrong!")), GetParent());
			return;
		}
		
		SOCKET so = (SOCKET)SnmpCreateSession();
		char *scommunity = CStringEx(m_szCommunity).toChar();
		CNewGuard guard3(scommunity);
		COLORREF color = LoadAsRGB(_T("memo.mib.text.color"), _T("128,128,128"));
		CString raddr = LoadAsString(_T("memo.remote.addr.text"), _T("Remote address"));
		CString rport = LoadAsString(_T("memo.remote.port.text"), _T("port"));
		CString laddr = LoadAsString(_T("memo.local.addr.text"), _T("Local address"));
		CString lport = LoadAsString(_T("memo.local.port.text"), _T("port"));
		CString type = LoadAsString(_T("memo.protocol.type.text"), _T("transport"));
		CString verion = LoadAsString(_T("memo.protocol.version.text"), _T("Protocol version"));
		
		bool null;
		DWORD valtype;
		char *oid, *val;
		CString line;
		CStringEx msg;
		
		if (!m_ctrlRecursive.GetCheck())
		{
			line.Format(_T("%s: %s %s: %d %s: IP/UDP"), raddr, m_szIp, rport, m_nPort, type);
			msg = line;
			g_wndMemo->PostMessage(WM_MEMO_ADD_LINE, SET_TEXT, MakeMemoTextInfo(msg.toChar(), color));
			line.Format(_T("%s: %s %s: %d %s: IP/UDP"), laddr, m_szIp, lport, so, type);
			msg = line;
			g_wndMemo->PostMessage(WM_MEMO_ADD_LINE, SET_TEXT, MakeMemoTextInfo(msg.toChar(), color));
			line.Format(_T("%s: SNMPv1"), verion);
			msg = line;
			g_wndMemo->PostMessage(WM_MEMO_ADD_LINE, SET_TEXT, MakeMemoTextInfo(msg.toChar(), color));
		}
		
		status = GetSNMPVal(sip, 0, ptr, scommunity, m_nPort, timeout, m_get_type, oid, val, null, valtype, so);
		if (m_ctrlUseResult.GetCheck())
			m_ctrlOID.SetWindowText(CString(oid));
		
		if (oid && status)
		{
			if (szOid.Left(szBreakOid.GetLength()) != szBreakOid)
			{
				free(oid);
				free(val);
				break;
			}
			if (szOid.Compare(CString(oid)))
				szOid = oid;
			else
			{
				if (m_ctrlRecursive.GetCheck())	
				{
					free(oid);
					free(val);
					break;
				}
			}
			g_wndMemo->PostMessage(WM_MEMO_ADD_LINE, OID_VAL, MakeOidQueryInfo(++count, oid, val, valtype));
		}
		else
		{
			free(oid);
			free(val);
			msg = LoadAsString(_T("snmp.error.text"), _T("SNMP not be supported or timeout!"));
			g_wndMemo->PostMessage(WM_MEMO_ADD_LINE, SET_TEXT, MakeMemoTextInfo(msg.toChar(), color));
		}
		
		if (!m_ctrlRecursive.GetCheck())
			break;
		if (m_ctrlRecursive.GetCheck() && m_get_type == SNMP_PDU_GET)
			break;
	}while (true);
	g_wndMemo->PostMessage(WM_MEMO_ADD_LINE);
}