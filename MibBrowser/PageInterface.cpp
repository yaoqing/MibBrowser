// PageInterface.cpp : implementation file
//

#include "stdafx.h"
#include "mibbrowser.h"
#include "PageInterface.h"
#include "StringEx.h"
#include "NewGuard.h"
#include "PropertyWnd.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPageInterface
IMPLEMENT_DYNCREATE(CPageInterface, CWnd)

CPageInterface::CPageInterface()
{
	font = new CFont();
	font->CreatePointFont(LoadAsInteger(_T("property.dlg.font.size"), 90), LoadAsString(_T("property.dlg.font.name"), _T("Arial")));
}

CPageInterface::~CPageInterface()
{
	font->DeleteObject();
	delete font;
}


BEGIN_MESSAGE_MAP(CPageInterface, CWnd)
	//{{AFX_MSG_MAP(CPageInterface)
	ON_WM_CREATE()
	ON_WM_ERASEBKGND()
	ON_BN_CLICKED(7, OnRefresh)
	ON_REGISTERED_MESSAGE(WM_INIT_PAGE, OnInitPage)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CPageInterface message handlers

int CPageInterface::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here
	m_ctrProc.Create(WS_CHILD|WS_VISIBLE|LVS_REPORT|LVS_SINGLESEL, CRect(15, 25, 380, 305), this, 0);
	m_ctrlRefesh.Create(LoadAsString(_T("property.page.refresh.text"), _T("&Refresh")), WS_CHILD|WS_VISIBLE, CRect(290, 326, 360, 350), this, 7);

	m_ctrProc.SetFont(font);
	m_ctrlRefesh.SetFont(font);

	m_ctrProc.InsertColumn(0, LoadAsString(_T("property.page.interface.index.text"), _T("Index")), LVCFMT_RIGHT, 64);
	m_ctrProc.InsertColumn(1, LoadAsString(_T("property.page.interface.descr.text"), _T("Descr")), LVCFMT_LEFT, 128);
	m_ctrProc.InsertColumn(2, LoadAsString(_T("property.page.interface.type.text"), _T("Type")), LVCFMT_LEFT, 128);
	m_ctrProc.InsertColumn(3, LoadAsString(_T("property.page.interface.mtu.text"), _T("Mtu")), LVCFMT_RIGHT, 64);
	m_ctrProc.InsertColumn(4, LoadAsString(_T("property.page.interface.speed.text"), _T("Speed")), LVCFMT_RIGHT, 64);
	m_ctrProc.InsertColumn(5, LoadAsString(_T("property.page.interface.mac.text"), _T("PhysAddress")), LVCFMT_RIGHT, 64);
	m_ctrProc.InsertColumn(6, LoadAsString(_T("property.page.interface.adminstatus.text"), _T("AdminStatus")), LVCFMT_RIGHT, 64);
	m_ctrProc.InsertColumn(7, LoadAsString(_T("property.page.interface.operstatus.text"), _T("OperStatus")), LVCFMT_RIGHT, 64);
	m_ctrProc.InsertColumn(8, LoadAsString(_T("property.page.interface.lastchange.text"), _T("LastChange")), LVCFMT_RIGHT, 64);
	m_ctrProc.InsertColumn(9, LoadAsString(_T("property.page.interface.inoctets.text"), _T("InOctets")), LVCFMT_RIGHT, 64);
	m_ctrProc.InsertColumn(10, LoadAsString(_T("property.page.interface.inucastpkts.text"), _T("InUcastPkts")), LVCFMT_RIGHT, 64);
	m_ctrProc.InsertColumn(11, LoadAsString(_T("property.page.interface.innucastpkts.text"), _T("InNUcastPkts")), LVCFMT_RIGHT, 64);
	m_ctrProc.InsertColumn(12, LoadAsString(_T("property.page.interface.indiscards.text"), _T("InDiscards")), LVCFMT_RIGHT, 64);
	m_ctrProc.InsertColumn(13, LoadAsString(_T("property.page.interface.inerrors.text"), _T("InErrors")), LVCFMT_RIGHT, 64);
	m_ctrProc.InsertColumn(14, LoadAsString(_T("property.page.interface.inunknownprotos.text"), _T("InUnknownProtos")), LVCFMT_RIGHT, 64);
	m_ctrProc.InsertColumn(15, LoadAsString(_T("property.page.interface.outoctets.text"), _T("OutOctets")), LVCFMT_RIGHT, 64);
	m_ctrProc.InsertColumn(16, LoadAsString(_T("property.page.interface.outucastpkts.text"), _T("OutUcastPkts")), LVCFMT_RIGHT, 64);
	m_ctrProc.InsertColumn(17, LoadAsString(_T("property.page.interface.outnucastpkts.text"), _T("OutNUcastPkts")), LVCFMT_RIGHT, 64);
	m_ctrProc.InsertColumn(18, LoadAsString(_T("property.page.interface.outdiscards.text"), _T("OutDiscards")), LVCFMT_RIGHT, 64);
	m_ctrProc.InsertColumn(19, LoadAsString(_T("property.page.interface.outerrors.text"), _T("OutErrors")), LVCFMT_RIGHT, 64);
	m_ctrProc.InsertColumn(20, LoadAsString(_T("property.page.interface.outqlen.text"), _T("OutQLen")), LVCFMT_RIGHT, 64);
	m_ctrProc.InsertColumn(21, LoadAsString(_T("property.page.interface.specific.text"), _T("Specific")), LVCFMT_RIGHT, 64);
	m_ctrProc.SetExtendedStyle(LVS_EX_FULLROWSELECT);
	m_ctrProc.ModifyStyle(0, WS_HSCROLL|WS_VSCROLL);
	m_ctrProc.ModifyStyleEx(0, WS_EX_CLIENTEDGE|WS_EX_NOPARENTNOTIFY);
	
	PostMessage(WM_INIT_PAGE);
	return 0;
}

BOOL CPageInterface::OnEraseBkgnd(CDC* pDC) 
{
	// TODO: Add your message handler code here and/or call default
	CRect rect;
	GetClientRect(&rect);
	pDC->FillSolidRect(rect, LoadAsRGB(_T("property.page.color"), _T("240,240,234")));

	return TRUE;
}

LRESULT CPageInterface::OnInitPage(WPARAM wParam, LPARAM lParam)
{
	static char *interface_type[] = {
		{"other(1)"},
		{"regular1822(2)"},
		{"hdh1822(3)"},
		{"ddn-x25(4)"},
		{"rfc877-x25(5)"},
		{"ethernet-csmacd(6)"},
		{"iso88023-csmacd(7)"},
		{"iso88024-tokenBus(8)"},
		{"iso88025-tokenRing(9)"},
		{"iso88026-man(10)"},
		{"starLan(11)"},
		{"proteon-10Mbit(12)"},
		{"proteon-80Mbit(13)"},
		{"hyperchannel(14)"},
		{"fddi(15)"},
		{"lapb(16)"},
		{"sdlc(17)"},
		{"ds1(18)"},
		{"e1(19)"},
		{"basicISDN(20)"},
		{"primaryISDN(21)"},
		{"propPointToPointSerial(22)"},
		{"ppp(23)"},
		{"softwareLoopback(24)"},
		{"eon(25)"},
		{"ethernet-3Mbit(26)"},
		{"nsip(27)"},
		{"slip(28)"},
		{"ultra(29)"},
		{"ds3(30)"},
		{"sip(31)"},
		{"frame-relay(32)"}
	};
	static char *admin_status_type[] = {"up(1)", "down(2)", "testing(3)"};
	static char *oper_status_type[] = {"up(1)", "down(2)", "testing(3)", "unknown(4)", "dormant(5)", "notPresent(6)", "lowerLayerDown(7)"};
	CMap<smiUINT32, smiUINT32, int, int> item_map;

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

	CStringEx strOID;
	strOID = _T("1.3.6.1.2.1.2.2.1.");
	CStringEx strRangeOID;
	strRangeOID = _T("1.3.6.1.2.1.2.2.1.");

	char *oid, *range = strRangeOID.toChar();
	int range_len = strlen(range);
	CNewGuard guard3(range);

	do
	{
		oid = strOID.toChar();
		CNewGuard guard(oid);
		status = GetSNMPVal(pIp, 0, oid, pCommunity, pWnd->m_nPort, timeout, SNMP_PDU_GETNEXT, byoid, byval, null, valtype);
		if (status == SNMPAPI_FAILURE)
		{
			MsgBoxModal(LoadAsString(_T("snmp.error.text"), _T("SNMP not be supported or timeout!")), GetParent()->GetParent());
			return TRUE;
		}

		if (strncmp(byoid, range, range_len) == 0)
		{
			smiLPOID OID;
			strtooid(byoid, OID);
			int item_index = oidatindex(OID, -2);
			if (item_index == 1)
			{
				smiUINT32 key = oidatindex(OID, -1);
				int value = m_ctrProc.InsertItem(m_ctrProc.GetItemCount(), CString(byval));
				item_map.SetAt(key, value);
			}
			else
			{
				if (item_index == 3)
					m_ctrProc.SetItemText(item_map[oidatindex(OID, -1)], item_index-1, CString(interface_type[atoi(byval)-1]));
				else if (item_index == 7)
					m_ctrProc.SetItemText(item_map[oidatindex(OID, -1)], item_index-1, CString(admin_status_type[atoi(byval)-1]));
				else if (item_index == 8)
					m_ctrProc.SetItemText(item_map[oidatindex(OID, -1)], item_index-1, CString(oper_status_type[atoi(byval)-1]));
				else
					m_ctrProc.SetItemText(item_map[oidatindex(OID, -1)], item_index-1, CString(null ? "null" : byval));
			}
			FreeOid(OID);
			
			if (strcmp(oid, byoid) == 0)
			{
				free(byoid);
				free(byval);
				break;
			}
			else
			{				
				strOID = byoid;
				free(byoid);
				free(byval);
			}
		}
		else
		{
			free(byoid);
			free(byval);
			break;
		}
	}while (TRUE);
	
	return TRUE;
}

void CPageInterface::OnRefresh()
{
	OnInitPage(0, 0);
}
