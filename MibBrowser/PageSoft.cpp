// PageSoft.cpp : implementation file
//

#include "stdafx.h"
#include "mibbrowser.h"
#include "PageSoft.h"
#include "StringEx.h"
#include "NewGuard.h"
#include "PropertyWnd.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPageSoft
IMPLEMENT_DYNCREATE(CPageSoft, CWnd)

CPageSoft::CPageSoft()
{
	font = new CFont();
	font->CreatePointFont(LoadAsInteger(_T("property.dlg.font.size"), 90), LoadAsString(_T("property.dlg.font.name"), _T("Arial")));
}

CPageSoft::~CPageSoft()
{
	font->DeleteObject();
	delete font;
}


BEGIN_MESSAGE_MAP(CPageSoft, CWnd)
	//{{AFX_MSG_MAP(CPageSoft)
	ON_WM_CREATE()
	ON_WM_ERASEBKGND()
	ON_BN_CLICKED(7, OnRefresh)
	ON_REGISTERED_MESSAGE(WM_INIT_PAGE, OnInitPage)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CPageSoft message handlers

int CPageSoft::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here
	m_ctrProc.Create(WS_CHILD|WS_VISIBLE|LVS_REPORT|LVS_SINGLESEL, CRect(15, 25, 380, 305), this, 0);
	m_ctrlRefesh.Create(LoadAsString(_T("property.page.refresh.text"), _T("&Refresh")), WS_CHILD|WS_VISIBLE, CRect(290, 326, 360, 350), this, 7);

	m_ctrProc.SetFont(font);
	m_ctrlRefesh.SetFont(font);

	m_ctrProc.InsertColumn(0, LoadAsString(_T("property.page.soft.index.text"), _T("Index")), LVCFMT_RIGHT, 64);
	m_ctrProc.InsertColumn(1, LoadAsString(_T("property.page.soft.name.text"), _T("Name")), LVCFMT_LEFT, 128);
	m_ctrProc.InsertColumn(2, LoadAsString(_T("property.page.soft.id.text"), _T("ID")), LVCFMT_LEFT, 128);
	m_ctrProc.InsertColumn(3, LoadAsString(_T("property.page.soft.type.text"), _T("Type")), LVCFMT_RIGHT, 64);
	m_ctrProc.InsertColumn(4, LoadAsString(_T("property.page.soft.date.text"), _T("Date")), LVCFMT_RIGHT, 64);
	m_ctrProc.SetExtendedStyle(LVS_EX_FULLROWSELECT);
	m_ctrProc.ModifyStyle(0, WS_HSCROLL|WS_VSCROLL);
	m_ctrProc.ModifyStyleEx(0, WS_EX_CLIENTEDGE|WS_EX_NOPARENTNOTIFY);
	
	PostMessage(WM_INIT_PAGE);
	return 0;
}

BOOL CPageSoft::OnEraseBkgnd(CDC* pDC) 
{
	// TODO: Add your message handler code here and/or call default
	CRect rect;
	GetClientRect(&rect);
	pDC->FillSolidRect(rect, LoadAsRGB(_T("property.page.color"), _T("240,240,234")));

	return TRUE;
}

LRESULT CPageSoft::OnInitPage(WPARAM wParam, LPARAM lParam)
{
	static char *install_type[] = {"unknown(1)", "operatingSystem(2)", "deviceDriver(3)", "application(4)"};
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
	strOID = _T("1.3.6.1.2.1.25.6.3.1.");
	CStringEx strRangeOID;
	strRangeOID = _T("1.3.6.1.2.1.25.6.3.1.");

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
				if (item_index == 4)
					m_ctrProc.SetItemText(item_map[oidatindex(OID, -1)], item_index-1, CString(install_type[atoi(byval)-1]));
				else if (item_index == 5)
				{
					int pos = 0;
					if (byval[0] == '<' && byval[24] == '>') pos = 25;
					if (byval[0] == '<' && byval[33] == '>') pos = 34;
					int year = (BYTE)byval[0+pos]*256 + (BYTE)byval[1+pos];
					int month = byval[2+pos];
					int day = byval[3+pos];
					int hour = byval[4+pos];
					int minute = byval[5+pos];
					int second = byval[6+pos];
					CString date;
					date.Format(_T("%04d-%02d-%02d %02d:%02d:%02d"), year, month, day, hour, minute, second);
					m_ctrProc.SetItemText(item_map[oidatindex(OID, -1)], item_index-1, date);
				}
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

void CPageSoft::OnRefresh()
{
	OnInitPage(0, 0);
}
