// PageUdpStat.cpp : implementation file
//

#include "stdafx.h"
#include "mibbrowser.h"
#include "PageUdpStat.h"
#include "StringEx.h"
#include "NewGuard.h"
#include "PropertyWnd.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPageUdpStat
IMPLEMENT_DYNCREATE(CPageUdpStat, CWnd)

CPageUdpStat::CPageUdpStat()
{
	font = new CFont();
	font->CreatePointFont(LoadAsInteger(_T("property.dlg.font.size"), 90), LoadAsString(_T("property.dlg.font.name"), _T("Arial")));
}

CPageUdpStat::~CPageUdpStat()
{
	font->DeleteObject();
	delete font;
}


BEGIN_MESSAGE_MAP(CPageUdpStat, CWnd)
	//{{AFX_MSG_MAP(CPageUdpStat)
	ON_WM_CREATE()
	ON_WM_ERASEBKGND()
	ON_BN_CLICKED(7, OnRefresh)
	ON_REGISTERED_MESSAGE(WM_INIT_PAGE, OnInitPage)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CPageUdpStat message handlers

int CPageUdpStat::OnCreate(LPCREATESTRUCT lpCreateStruct) 
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

BOOL CPageUdpStat::OnEraseBkgnd(CDC* pDC) 
{
	// TODO: Add your message handler code here and/or call default
	CRect rect;
	GetClientRect(&rect);
	pDC->FillSolidRect(rect, LoadAsRGB(_T("property.page.color"), _T("240,240,234")));

	return TRUE;
}

LRESULT CPageUdpStat::OnInitPage(WPARAM wParam, LPARAM lParam)
{
	static char *udp_stat_index[] = {
		{"udpInDatagrams"},
		{"udpNoPorts"},
		{"udpInErrors"},
		{"udpOutDatagrams"}
	};
	
	static char *udp_stat_oid[] = {
		{"1.3.6.1.2.1.7.1.0"},	//udpInDatagrams
		{"1.3.6.1.2.1.7.2.0"},	//udpNoPorts
		{"1.3.6.1.2.1.7.3.0"},	//udpInErrors
		{"1.3.6.1.2.1.7.4.0"}	//udpOutDatagrams
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
	for (int i = 0; i < sizeof(udp_stat_oid)/sizeof(udp_stat_oid[0]); i++)
	{
		status = GetSNMPVal(pIp, 0, udp_stat_oid[i], pCommunity, pWnd->m_nPort, timeout, SNMP_PDU_GET, byoid, byval, null, valtype);
		if (status == SNMPAPI_FAILURE)
		{
			MsgBoxModal(LoadAsString(_T("snmp.error.text"), _T("SNMP not be supported or timeout!")), GetParent()->GetParent());
			return TRUE;
		}

		m_ctrProc.InsertItem(i, CString(udp_stat_index[i]));
		m_ctrProc.SetItemText(i, 1, CString(null ? "null" : byval));
			
		free(byoid);
		free(byval);
	}
	
	return TRUE;
}

void CPageUdpStat::OnRefresh()
{
	OnInitPage(0, 0);
}
