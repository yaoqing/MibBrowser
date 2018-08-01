// PageIpStat.cpp : implementation file
//

#include "stdafx.h"
#include "mibbrowser.h"
#include "PageIpStat.h"
#include "StringEx.h"
#include "NewGuard.h"
#include "PropertyWnd.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPageIpStat
IMPLEMENT_DYNCREATE(CPageIpStat, CWnd)

CPageIpStat::CPageIpStat()
{
	font = new CFont();
	font->CreatePointFont(LoadAsInteger(_T("property.dlg.font.size"), 90), LoadAsString(_T("property.dlg.font.name"), _T("Arial")));
}

CPageIpStat::~CPageIpStat()
{
	font->DeleteObject();
	delete font;
}


BEGIN_MESSAGE_MAP(CPageIpStat, CWnd)
	//{{AFX_MSG_MAP(CPageIpStat)
	ON_WM_CREATE()
	ON_WM_ERASEBKGND()
	ON_BN_CLICKED(7, OnRefresh)
	ON_REGISTERED_MESSAGE(WM_INIT_PAGE, OnInitPage)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CPageIpStat message handlers

int CPageIpStat::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here
	m_ctrProc.Create(WS_CHILD|WS_VISIBLE|LVS_REPORT|LVS_SINGLESEL, CRect(15, 25, 380, 305), this, 0);
	m_ctrlRefesh.Create(LoadAsString(_T("property.page.refresh.text"), _T("&Refresh")), WS_CHILD|WS_VISIBLE, CRect(290, 326, 360, 350), this, 7);

	m_ctrProc.SetFont(font);
	m_ctrlRefesh.SetFont(font);

	m_ctrProc.InsertColumn(0, LoadAsString(_T("property.page.ipstat.key.text"), _T("Index")), LVCFMT_LEFT, 128);
	m_ctrProc.InsertColumn(1, LoadAsString(_T("property.page.ipstat.value.text"), _T("Value")), LVCFMT_LEFT, 192);
	m_ctrProc.SetExtendedStyle(LVS_EX_FULLROWSELECT);
	m_ctrProc.ModifyStyle(0, WS_HSCROLL|WS_VSCROLL);
	m_ctrProc.ModifyStyleEx(0, WS_EX_CLIENTEDGE|WS_EX_NOPARENTNOTIFY);
	
	PostMessage(WM_INIT_PAGE);
	return 0;
}

BOOL CPageIpStat::OnEraseBkgnd(CDC* pDC) 
{
	// TODO: Add your message handler code here and/or call default
	CRect rect;
	GetClientRect(&rect);
	pDC->FillSolidRect(rect, LoadAsRGB(_T("property.page.color"), _T("240,240,234")));

	return TRUE;
}

LRESULT CPageIpStat::OnInitPage(WPARAM wParam, LPARAM lParam)
{
	static char *ip_stat_index[] = {
		{"ipForwarding"},
		{"ipDefaultTTL"},
		{"ipInReceives"},
		{"ipInHdrErrors"},
		{"ipInAddrErrors"},
		{"ipForwDatagrams"},
		{"ipInUnknownProtos"},
		{"ipInDiscards"},
		{"ipInDelivers"},
		{"ipOutRequests"},
		{"ipOutDiscards"},
		{"ipOutNoRoutes"},
		{"ipReasmTimeout"},
		{"ipReasmReqds"},
		{"ipReasmOKs"},
		{"ipReasmFails"},
		{"ipFragOKs"},
		{"ipFragFails"},
		{"ipFragCreates"},
		{"ipRoutingDiscards"}
	};
	
	static char *ip_stat_oid[] = {
		{"1.3.6.1.2.1.4.1.0"},	//ipForwarding
		{"1.3.6.1.2.1.4.2.0"},	//ipDefaultTTL
		{"1.3.6.1.2.1.4.3.0"},	//ipInReceives
		{"1.3.6.1.2.1.4.4.0"},	//ipInHdrErrors
		{"1.3.6.1.2.1.4.5.0"},	//ipInAddrErrors
		{"1.3.6.1.2.1.4.6.0"},	//ipForwDatagrams
		{"1.3.6.1.2.1.4.7.0"},  //ipInUnknownProtos
		{"1.3.6.1.2.1.4.8.0"},	//ipInDiscards
		{"1.3.6.1.2.1.4.9.0"},	//ipInDelivers
		{"1.3.6.1.2.1.4.10.0"},	//ipOutRequests
		{"1.3.6.1.2.1.4.11.0"},	//ipOutDiscards
		{"1.3.6.1.2.1.4.12.0"},	//ipOutNoRoutes
		{"1.3.6.1.2.1.4.13.0"},	//ipReasmTimeout
		{"1.3.6.1.2.1.4.14.0"},	//ipReasmReqds
		{"1.3.6.1.2.1.4.15.0"},	//ipReasmOKs
		{"1.3.6.1.2.1.4.16.0"},	//ipReasmFails
		{"1.3.6.1.2.1.4.17.0"}, //ipFragOKs
		{"1.3.6.1.2.1.4.18.0"}, //ipFragFails
		{"1.3.6.1.2.1.4.19.0"}, //ipFragCreates
		{"1.3.6.1.2.1.4.23.0"}  //ipRoutingDiscards
	};

	CMap<CString, CString&, int, int> item_map;

	m_ctrProc.DeleteAllItems();

	CPropertyWnd *pWnd = (CPropertyWnd *)GetParent();
	CStringEx szIp(pWnd->m_strIpAddr);
	CStringEx szCommunity(pWnd->m_strCommunity);
	
	char *pIp = szIp.toChar();
	char *pCommunity = szCommunity.toChar();
	char *byoid, *byval;
	bool null = true, break_loop = false;
	DWORD valtype = 0;

	CNewGuard guard1(pIp);
	CNewGuard guard2(pCommunity);

	int timeout = LoadAsInteger(_T("snmp.udp.timeout"), 6);
	if (timeout < 0) timeout = 0;
	if (timeout > 60) timeout = 60;

	SNMPAPI_STATUS status;
	for (int i = 0; i < sizeof(ip_stat_oid)/sizeof(ip_stat_oid[0]); i++)
	{
		status = GetSNMPVal(pIp, 0, ip_stat_oid[i], pCommunity, pWnd->m_nPort, timeout, SNMP_PDU_GET, byoid, byval, null, valtype);
		if (status == SNMPAPI_FAILURE)
		{
			MsgBoxModal(LoadAsString(_T("snmp.error.text"), _T("SNMP not be supported or timeout!")), GetParent()->GetParent());
			return TRUE;
		}

		m_ctrProc.InsertItem(i, CString(ip_stat_index[i]));
		m_ctrProc.SetItemText(i, 1, CString(null ? "null" : byval));
			
		free(byoid);
		free(byval);
	}
	
	return TRUE;
}

void CPageIpStat::OnRefresh()
{
	OnInitPage(0, 0);
}
