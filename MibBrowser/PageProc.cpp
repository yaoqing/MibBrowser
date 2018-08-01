// PageProc.cpp : implementation file
//

#include "stdafx.h"
#include "mibbrowser.h"
#include "PageProc.h"
#include "StringEx.h"
#include "NewGuard.h"
#include "PropertyWnd.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPageProc
IMPLEMENT_DYNCREATE(CPageProc, CWnd)

CPageProc::CPageProc()
{
	font = new CFont();
	font->CreatePointFont(LoadAsInteger(_T("property.dlg.font.size"), 90), LoadAsString(_T("property.dlg.font.name"), _T("Arial")));
}

CPageProc::~CPageProc()
{
	font->DeleteObject();
	delete font;
}


BEGIN_MESSAGE_MAP(CPageProc, CWnd)
	//{{AFX_MSG_MAP(CPageProc)
	ON_WM_CREATE()
	ON_WM_ERASEBKGND()
	ON_BN_CLICKED(7, OnRefresh)
	ON_REGISTERED_MESSAGE(WM_INIT_PAGE, OnInitPage)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CPageProc message handlers

BOOL CPageProc::OnEraseBkgnd(CDC* pDC) 
{
	// TODO: Add your message handler code here and/or call default
	CRect rect;
	GetClientRect(&rect);
	pDC->FillSolidRect(rect, LoadAsRGB(_T("property.page.color"), _T("240,240,234")));
	return TRUE;
}

int CPageProc::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here
	m_ctrProc.Create(WS_CHILD|WS_VISIBLE|LVS_REPORT|LVS_SINGLESEL, CRect(15, 25, 380, 305), this, 0);
	m_ctrlRefesh.Create(LoadAsString(_T("property.page.refresh.text"), _T("&Refresh")), WS_CHILD|WS_VISIBLE, CRect(290, 326, 360, 350), this, 7);

	m_ctrProc.SetFont(font);
	m_ctrlRefesh.SetFont(font);

	m_ctrProc.InsertColumn(0, LoadAsString(_T("property.page.process.index.text"), _T("PID")), LVCFMT_RIGHT, 64);
	m_ctrProc.InsertColumn(1, LoadAsString(_T("property.page.process.name.text"), _T("Name")), LVCFMT_LEFT, 128);
	m_ctrProc.InsertColumn(2, LoadAsString(_T("property.page.process.id.text"), _T("ProductID")), LVCFMT_RIGHT, 64);
	m_ctrProc.InsertColumn(3, LoadAsString(_T("property.page.process.path.text"), _T("Path")), LVCFMT_LEFT, 64);
	m_ctrProc.InsertColumn(4, LoadAsString(_T("property.page.process.parameter.text"), _T("Parameter")), LVCFMT_LEFT, 64);
	m_ctrProc.InsertColumn(5, LoadAsString(_T("property.page.process.type.text"), _T("Type")), LVCFMT_RIGHT, 64);
	m_ctrProc.InsertColumn(6, LoadAsString(_T("property.page.process.status.text"), _T("Status")), LVCFMT_RIGHT, 64);
	m_ctrProc.InsertColumn(7, LoadAsString(_T("property.page.process.perfcpu.text"), _T("CPU(ms)")), LVCFMT_RIGHT, 64);
	m_ctrProc.InsertColumn(8, LoadAsString(_T("property.page.process.perfmem.text"), _T("Memory(KB):")), LVCFMT_RIGHT, 64);
	m_ctrProc.SetExtendedStyle(LVS_EX_FULLROWSELECT);
	m_ctrProc.ModifyStyle(0, WS_HSCROLL|WS_VSCROLL);
	m_ctrProc.ModifyStyleEx(0, WS_EX_CLIENTEDGE|WS_EX_NOPARENTNOTIFY);

	PostMessage(WM_INIT_PAGE);
	return 0;
}

LRESULT CPageProc::OnInitPage(WPARAM wParam, LPARAM lParam)
{
	static char *run_type[] = {"unknown(1)", "operatingSystem(2)", "deviceDriver(3)", "application(4)"};
	static char *run_status[] = {"running(1)", "runnable(2)", "notRunnable(3)", "invalid(4)"};
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
	strOID = _T("1.3.6.1.2.1.25.4.2.");
	CStringEx strRangeOID;
	strRangeOID = _T("1.3.6.1.2.1.25.");

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
			if (strncmp(byoid, "1.3.6.1.2.1.25.4.2.", 19) == 0)
			{
				if (item_index == 1)
				{
					smiUINT32 key = oidatindex(OID, -1);
					int value = m_ctrProc.InsertItem(m_ctrProc.GetItemCount(), CString(byval));
					item_map.SetAt(key, value);
				}
				else
				{
					if (item_index == 6)
						m_ctrProc.SetItemText(item_map[oidatindex(OID, -1)], item_index-1, CString(run_type[atoi(byval)-1]));
					else if (item_index == 7)
						m_ctrProc.SetItemText(item_map[oidatindex(OID, -1)], item_index-1, CString(run_status[atoi(byval)-1]));
					else
						m_ctrProc.SetItemText(item_map[oidatindex(OID, -1)], item_index-1, CString(null ? "null" : byval));
				}
			}
			else if (strncmp(byoid, "1.3.6.1.2.1.25.5.", 17) == 0)
			{
				int item_index = oidatindex(OID, -2);
				m_ctrProc.SetItemText(item_map[oidatindex(OID, -1)], item_index+6, CString(null ? "null" : byval));
			}
			else
				break_loop = true;
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

			if (break_loop) break;
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

void CPageProc::OnRefresh()
{
	OnInitPage(0, 0);
}
