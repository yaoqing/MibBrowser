// AddIpConfWnd.cpp : implementation file
//

#include "stdafx.h"
#include "mibbrowser.h"
#include "AddIpConfWnd.h"
#include "StringEx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAddIpConfWnd

CAddIpConfWnd::CAddIpConfWnd(CString *name, CString *ip, CString *community, CString *remark, int *port, bool *isokexit)
{
	m_ip = ip;
	m_port = port;
	m_name = name;
	m_remark = remark;
	m_isokexit = isokexit;
	m_community = community;

	*m_port = 0;
	m_ip->Empty();
	m_name->Empty();
	m_remark->Empty();
	m_community->Empty();
	*m_isokexit = false;

	font = new CFont();
	font->CreatePointFont(LoadAsInteger(_T("view.ip.add.dlg.font.size"), 90), LoadAsString(_T("view.ip.add.dlg.font.name"), _T("Arial Bold")));
}

CAddIpConfWnd::~CAddIpConfWnd()
{
	font->DeleteObject();
	delete font;
}


BEGIN_MESSAGE_MAP(CAddIpConfWnd, CWnd)
	//{{AFX_MSG_MAP(CAddIpConfWnd)
	ON_WM_CREATE()
	ON_WM_ERASEBKGND()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CAddIpConfWnd message handlers

int CAddIpConfWnd::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here
	m_ctrlName.Create(WS_CHILD | WS_VISIBLE, CRect(100, 21, 260, 42), this, 0);
	m_ctrlIp.Create(WS_CHILD | WS_VISIBLE, CRect(100, 53, 260, 74), this, 0);
	m_ctrlPort.Create(WS_CHILD | WS_VISIBLE|ES_NUMBER, CRect(100, 85, 260, 106), this, 1);
	m_ctrlCommunity.Create(WS_CHILD | WS_VISIBLE, CRect(100, 117, 260, 138), this, 2);
	m_ctrlRemark.Create(WS_CHILD | WS_VISIBLE, CRect(100, 149, 260, 170), this, 3);
	
	m_ctrlOk.Create(LoadAsString(_T("view.ip.add.dlg.ok.text"), _T("&Ok")), WS_CHILD | WS_VISIBLE, CRect(100, 190, 170, 214), this, 4);
	m_ctrlCancel.Create(LoadAsString(_T("view.ip.add.dlg.cancel.text"), _T("&Cancel")), WS_CHILD | WS_VISIBLE, CRect(190, 190, 260, 214), this, 5);
	
	COLORREF color = LoadAsRGB(_T("view.ip.add.dlg.edit.border.color"), _T("49,105,198"));
	m_ctrlName.SetBorderColor(color);
	m_ctrlIp.SetBorderColor(color);
	m_ctrlPort.SetBorderColor(color);
	m_ctrlCommunity.SetBorderColor(color);
	m_ctrlRemark.SetBorderColor(color);

	m_ctrlName.SetFont(font);
	m_ctrlIp.SetFont(font);
	m_ctrlPort.SetFont(font);
	m_ctrlCommunity.SetFont(font);
	m_ctrlRemark.SetFont(font);

	m_ctrlOk.SetFont(font);
	m_ctrlCancel.SetFont(font);

	m_ctrlPort.SetWindowText(LoadAsString(_T("snmp.udp.port"), _T("161")));
	m_ctrlCommunity.SetWindowText(LoadAsString(_T("snmp.community.read"), _T("public")));

	return ShowWindow(SW_SHOW);
}

BOOL CAddIpConfWnd::PreCreateWindow(CREATESTRUCT& cs) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CWnd::PreCreateWindow(cs);
}

BOOL CAddIpConfWnd::OnEraseBkgnd(CDC* pDC) 
{
	// TODO: Add your message handler code here
	CRect rect;
	GetClientRect(&rect);
	pDC->FillSolidRect(rect, LoadAsRGB(_T("view.ip.add.dlg.color"), _T("255,255,255")));

	DrawLabel(pDC, CRect(25, 20, 98, 41), LoadAsString(_T("view.ip.add.dlg.host.text"), _T("host name:")));
	DrawLabel(pDC, CRect(25, 52, 98, 73), LoadAsString(_T("view.ip.add.dlg.ip.text"), _T("ipaddr(*):")));
	DrawLabel(pDC, CRect(25, 84, 98, 105), LoadAsString(_T("view.ip.add.dlg.port.text"), _T("port(*):")));
	DrawLabel(pDC, CRect(25, 116, 98, 137), LoadAsString(_T("view.ip.add.dlg.community.text"), _T("community(*):")));
	DrawLabel(pDC, CRect(25, 148, 98, 169), LoadAsString(_T("view.ip.add.dlg.description.text"), _T("description:")));

	return TRUE;
}

void CAddIpConfWnd::DrawLabel(CDC *pDC, CRect &rect, CString szText)
{
	CFont *pOldFont = pDC->SelectObject(font);
	
	pDC->SetTextColor(LoadAsRGB(_T("view.ip.add.dlg.font.color"), _T("8,0,0")));
	pDC->DrawText(szText, rect, DT_LEFT|DT_BOTTOM|DT_SINGLELINE);

	pDC->SelectObject(pOldFont);
}

BOOL CAddIpConfWnd::OnCommand(WPARAM wParam, LPARAM lParam) 
{
	// TODO: Add your specialized code here and/or call the base class
	char *p;
	UINT addr;
	CString str;
	CStringEx strex;
	CRect rect(21, 190, 96, 211);
	switch (wParam)
	{
	case 4:
		m_ctrlIp.GetWindowText(*m_ip);
		strex = *m_ip;
		p = strex.toChar();
		addr = inet_addr(p);
		delete[] p;
		if ((INADDR_NONE == addr)|| (m_ip->GetLength() == 0))
		{
			MsgBoxModal(LoadAsString(_T("ip.addr.error.text"), _T("IP wrong!")), GetParent());
			break;
		}
		m_ctrlName.GetWindowText(*m_name);
		m_ctrlPort.GetWindowText(str);
		m_ctrlCommunity.GetWindowText(*m_community);
		m_ctrlRemark.GetWindowText(*m_remark);
		m_ctrlIp.GetWindowText(*m_ip);
		*m_port= _ttoi(str);
		if ((*m_port <= 0) || (*m_port >= 65536))
		{
			MsgBoxModal(LoadAsString(_T("port.error.text"), _T("Port wrong !")), GetParent());
			break;
		}
		*m_isokexit = true;
		GetParent()->PostMessage(WM_CLOSE);
		break;
	case 5:
		GetParent()->PostMessage(WM_CLOSE);
		break;
	}

	return CWnd::OnCommand(wParam, lParam);
}
