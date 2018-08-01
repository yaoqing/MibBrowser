// PageSnmpStat.cpp : implementation file
//

#include "stdafx.h"
#include "mibbrowser.h"
#include "PageSnmpStat.h"
#include "StringEx.h"
#include "NewGuard.h"
#include "PropertyWnd.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPageSnmpStat
IMPLEMENT_DYNCREATE(CPageSnmpStat, CWnd)

CPageSnmpStat::CPageSnmpStat()
{
	font = new CFont();
	font->CreatePointFont(LoadAsInteger(_T("property.dlg.font.size"), 90), LoadAsString(_T("property.dlg.font.name"), _T("Arial")));
}

CPageSnmpStat::~CPageSnmpStat()
{
	font->DeleteObject();
	delete font;
}


BEGIN_MESSAGE_MAP(CPageSnmpStat, CWnd)
	//{{AFX_MSG_MAP(CPageSnmpStat)
	ON_WM_CREATE()
	ON_WM_ERASEBKGND()
	ON_BN_CLICKED(7, OnRefresh)
	ON_REGISTERED_MESSAGE(WM_INIT_PAGE, OnInitPage)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CPageSnmpStat message handlers

int CPageSnmpStat::OnCreate(LPCREATESTRUCT lpCreateStruct) 
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

BOOL CPageSnmpStat::OnEraseBkgnd(CDC* pDC) 
{
	// TODO: Add your message handler code here and/or call default
	CRect rect;
	GetClientRect(&rect);
	pDC->FillSolidRect(rect, LoadAsRGB(_T("property.page.color"), _T("240,240,234")));

	return TRUE;
}

LRESULT CPageSnmpStat::OnInitPage(WPARAM wParam, LPARAM lParam)
{
	static char *snmp_stat_index[] = {
		{"snmpInPkts"},
		{"snmpOutPkts"},
		{"snmpInBadVersions"},
		{"snmpInBadCommunityNames"},
		{"snmpInBadCommunityUses"},
		{"snmpInASNParseErrs"},
		{"snmpInTooBigs"},
		{"snmpInNoSuchNames"},
		{"snmpInBadValues"},
		{"snmpInReadOnlys"},
		{"snmpInGenErrs"},
		{"snmpInTotalReqVars"},
		{"snmpInTotalSetVars"},
		{"snmpInGetRequests"},
		{"snmpInGetNexts"},
		{"snmpInSetRequests"},
		{"snmpInGetResponses"},
		{"snmpInTraps"},
		{"snmpOutTooBigs"},
		{"snmpOutNoSuchNames"},
		{"snmpOutBadValues"},
		{"snmpOutGenErrs"},
		{"snmpOutGetRequests"},
		{"snmpOutGetNexts"},
		{"snmpOutSetRequests"},
		{"snmpOutGetResponses"},
		{"snmpOutTraps"},
		{"snmpEnableAuthenTraps"},
		{"snmpSilentDrops"},
		{"snmpProxyDrops"}
	};
	
	static char *snmp_stat_oid[] = {
		{"1.3.6.1.2.1.11.1.0"},	    //snmpInPkts
		{"1.3.6.1.2.1.11.2.0"},     //snmpOutPkts
		{"1.3.6.1.2.1.11.3.0"},     //snmpInBadVersions
		{"1.3.6.1.2.1.11.4.0"},     //snmpInBadCommunityNames
		{"1.3.6.1.2.1.11.5.0"},	    //snmpInBadCommunityUses
		{"1.3.6.1.2.1.11.6.0"},     //snmpInASNParseErrs
		{"1.3.6.1.2.1.11.8.0"},	    //snmpInTooBigs
		{"1.3.6.1.2.1.11.9.0"},	    //snmpInNoSuchNames
		{"1.3.6.1.2.1.11.10.0"},	//snmpInBadValues
		{"1.3.6.1.2.1.11.11.0"},	//snmpInReadOnlys
		{"1.3.6.1.2.1.11.12.0"},	//snmpInGenErrs
		{"1.3.6.1.2.1.11.13.0"},	//snmpInTotalReqVars
		{"1.3.6.1.2.1.11.14.0"},	//snmpInTotalSetVars
		{"1.3.6.1.2.1.11.15.0"},	//snmpInGetRequests
		{"1.3.6.1.2.1.11.16.0"},	//snmpInGetNexts
		{"1.3.6.1.2.1.11.17.0"},	//snmpInSetRequests
		{"1.3.6.1.2.1.11.18.0"},	//snmpInGetResponses
		{"1.3.6.1.2.1.11.19.0"},	//snmpInTraps
		{"1.3.6.1.2.1.11.20.0"},	//snmpOutTooBigs
		{"1.3.6.1.2.1.11.21.0"},	//snmpOutNoSuchNames
		{"1.3.6.1.2.1.11.22.0"},	//snmpOutBadValues
		{"1.3.6.1.2.1.11.24.0"},	//snmpOutGenErrs
		{"1.3.6.1.2.1.11.25.0"},	//snmpOutGetRequests
		{"1.3.6.1.2.1.11.26.0"},	//snmpOutGetNexts
		{"1.3.6.1.2.1.11.27.0"},	//snmpOutSetRequests
		{"1.3.6.1.2.1.11.28.0"},    //snmpOutGetResponses
		{"1.3.6.1.2.1.11.29.0"},    //snmpOutTraps
		{"1.3.6.1.2.1.11.30.0"},    //snmpEnableAuthenTraps
		{"1.3.6.1.2.1.11.31.0"},    //snmpProxyDrops
		{"1.3.6.1.2.1.11.32.0"},    //snmpSilentDrops
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
	for (int i = 0; i < sizeof(snmp_stat_oid)/sizeof(snmp_stat_oid[0]); i++)
	{
		status = GetSNMPVal(pIp, 0, snmp_stat_oid[i], pCommunity, pWnd->m_nPort, timeout, SNMP_PDU_GET, byoid, byval, null, valtype);
		if (status == SNMPAPI_FAILURE)
		{
			MsgBoxModal(LoadAsString(_T("snmp.error.text"), _T("SNMP not be supported or timeout!")), GetParent()->GetParent());
			return TRUE;
		}

		m_ctrProc.InsertItem(i, CString(snmp_stat_index[i]));
		m_ctrProc.SetItemText(i, 1, CString(null ? "null" : byval));
			
		free(byoid);
		free(byval);
	}
	
	return TRUE;
}

void CPageSnmpStat::OnRefresh()
{
	OnInitPage(0, 0);
}
