// PageIcmpStat.cpp : implementation file
//

#include "stdafx.h"
#include "mibbrowser.h"
#include "PageIcmpStat.h"
#include "StringEx.h"
#include "NewGuard.h"
#include "PropertyWnd.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPageIcmpStat
IMPLEMENT_DYNCREATE(CPageIcmpStat, CWnd)

CPageIcmpStat::CPageIcmpStat()
{
	font = new CFont();
	font->CreatePointFont(LoadAsInteger(_T("property.dlg.font.size"), 90), LoadAsString(_T("property.dlg.font.name"), _T("Arial")));
}

CPageIcmpStat::~CPageIcmpStat()
{
	font->DeleteObject();
	delete font;
}


BEGIN_MESSAGE_MAP(CPageIcmpStat, CWnd)
	//{{AFX_MSG_MAP(CPageIcmpStat)
	ON_WM_CREATE()
	ON_WM_ERASEBKGND()
	ON_BN_CLICKED(7, OnRefresh)
	ON_REGISTERED_MESSAGE(WM_INIT_PAGE, OnInitPage)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CPageIcmpStat message handlers

int CPageIcmpStat::OnCreate(LPCREATESTRUCT lpCreateStruct) 
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

BOOL CPageIcmpStat::OnEraseBkgnd(CDC* pDC) 
{
	// TODO: Add your message handler code here and/or call default
	CRect rect;
	GetClientRect(&rect);
	pDC->FillSolidRect(rect, LoadAsRGB(_T("property.page.color"), _T("240,240,234")));

	return TRUE;
}

LRESULT CPageIcmpStat::OnInitPage(WPARAM wParam, LPARAM lParam)
{
	static char *icmp_stat_index[] = {
		{"icmpInMsgs"},
		{"icmpInErrors"},
		{"icmpInDestUnreachs"},
		{"icmpInTimeExcds"},
		{"icmpInParmProbs"},
		{"icmpInSrcQuenchs"},
		{"icmpInRedirects"},
		{"icmpInEchos"},
		{"icmpInEchoReps"},
		{"icmpInTimestamps"},
		{"icmpInTimestampReps"},
		{"icmpInAddrMasks"},
		{"icmpInAddrMaskReps"},
		{"icmpOutMsgs"},
		{"icmpOutErrors"},
		{"icmpOutDestUnreachs"},
		{"icmpOutTimeExcds"},
		{"icmpOutParmProbs"},
		{"icmpOutSrcQuenchs"},
		{"icmpOutRedirects"},
		{"icmpOutEchos"},
		{"icmpOutEchoReps"},
		{"icmpOutTimestamps"},
		{"icmpOutTimestampReps"},
		{"icmpOutAddrMasks"},
		{"icmpOutAddrMaskReps"}
	};
	
	static char *icmp_stat_oid[] = {
		{"1.3.6.1.2.1.5.1.0"},	//icmpInMsgs
		{"1.3.6.1.2.1.5.2.0"},	//icmpInErrors
		{"1.3.6.1.2.1.5.3.0"},	//icmpInDestUnreachs
		{"1.3.6.1.2.1.5.4.0"},	//icmpInTimeExcds
		{"1.3.6.1.2.1.5.5.0"},	//icmpInParmProbs
		{"1.3.6.1.2.1.5.6.0"},	//icmpInSrcQuenchs
		{"1.3.6.1.2.1.5.7.0"},  //icmpInRedirects
		{"1.3.6.1.2.1.5.8.0"},	//icmpInEchos
		{"1.3.6.1.2.1.5.9.0"},	//icmpInEchoReps
		{"1.3.6.1.2.1.5.10.0"},	//icmpInTimestamps
		{"1.3.6.1.2.1.5.11.0"},	//icmpInTimestampReps
		{"1.3.6.1.2.1.5.12.0"},	//icmpInAddrMasks
		{"1.3.6.1.2.1.5.13.0"},	//icmpInAddrMaskReps
		{"1.3.6.1.2.1.5.14.0"},	//icmpOutMsgs
		{"1.3.6.1.2.1.5.15.0"},	//icmpOutErrors
		{"1.3.6.1.2.1.5.16.0"},	//icmpOutDestUnreachs
		{"1.3.6.1.2.1.5.17.0"}, //icmpOutTimeExcds
		{"1.3.6.1.2.1.5.18.0"}, //icmpOutParmProbs
		{"1.3.6.1.2.1.5.19.0"}, //icmpOutSrcQuenchs
		{"1.3.6.1.2.1.5.20.0"}, //icmpOutRedirects
		{"1.3.6.1.2.1.5.21.0"},	//icmpOutEchos
		{"1.3.6.1.2.1.5.22.0"}, //icmpOutEchoReps
		{"1.3.6.1.2.1.5.23.0"}, //icmpOutTimestamps
		{"1.3.6.1.2.1.5.24.0"}, //icmpOutTimestampReps
		{"1.3.6.1.2.1.5.25.0"},	//icmpOutAddrMasks
		{"1.3.6.1.2.1.5.26.0"}  //icmpOutAddrMaskReps
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
	for (int i = 0; i < sizeof(icmp_stat_oid)/sizeof(icmp_stat_oid[0]); i++)
	{
		status = GetSNMPVal(pIp, 0, icmp_stat_oid[i], pCommunity, pWnd->m_nPort, timeout, SNMP_PDU_GET, byoid, byval, null, valtype);
		if (status == SNMPAPI_FAILURE)
		{
			MsgBoxModal(LoadAsString(_T("snmp.error.text"), _T("SNMP not be supported or timeout!")), GetParent()->GetParent());
			return TRUE;
		}

		m_ctrProc.InsertItem(i, CString(icmp_stat_index[i]));
		m_ctrProc.SetItemText(i, 1, CString(null ? "null" : byval));
			
		free(byoid);
		free(byval);
	}
	
	return TRUE;
}

void CPageIcmpStat::OnRefresh()
{
	OnInitPage(0, 0);
}

