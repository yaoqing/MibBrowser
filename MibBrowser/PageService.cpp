// PageService.cpp : implementation file
//

#include "stdafx.h"
#include "mibbrowser.h"
#include "PageService.h"
#include "StringEx.h"
#include "NewGuard.h"
#include "PropertyWnd.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPageService
IMPLEMENT_DYNCREATE(CPageService, CWnd)

CPageService::CPageService()
{
	font = new CFont();
	font->CreatePointFont(LoadAsInteger(_T("property.dlg.font.size"), 90), LoadAsString(_T("property.dlg.font.name"), _T("Arial")));
}

CPageService::~CPageService()
{
	font->DeleteObject();
	delete font;
}


BEGIN_MESSAGE_MAP(CPageService, CWnd)
	//{{AFX_MSG_MAP(CPageService)
	ON_WM_CREATE()
	ON_WM_ERASEBKGND()
	ON_BN_CLICKED(7, OnRefresh)
	ON_REGISTERED_MESSAGE(WM_INIT_PAGE, OnInitPage)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CPageService message handlers

int CPageService::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here
	m_ctrProc.Create(WS_CHILD|WS_VISIBLE|LVS_REPORT|LVS_SINGLESEL, CRect(15, 25, 380, 305), this, 0);
	m_ctrlRefesh.Create(LoadAsString(_T("property.page.refresh.text"), _T("&Refresh")), WS_CHILD|WS_VISIBLE, CRect(290, 326, 360, 350), this, 7);

	m_ctrProc.SetFont(font);
	m_ctrlRefesh.SetFont(font);

	m_ctrProc.InsertColumn(0, LoadAsString(_T("property.page.service.name.text"), _T("Name")), LVCFMT_LEFT, 128);
	m_ctrProc.InsertColumn(1, LoadAsString(_T("property.page.service.installedstate.text"), _T("InstalledState")), LVCFMT_RIGHT, 64);
	m_ctrProc.InsertColumn(2, LoadAsString(_T("property.page.service.operatingstate.text"), _T("OperatingState")), LVCFMT_RIGHT, 64);
	m_ctrProc.InsertColumn(3, LoadAsString(_T("property.page.service.canbeuninstalled.text"), _T("CanBeUninstalled")), LVCFMT_RIGHT, 64);
	m_ctrProc.InsertColumn(4, LoadAsString(_T("property.page.service.canbepaused.text"), _T("CanBePaused")), LVCFMT_RIGHT, 64);
	m_ctrProc.SetExtendedStyle(LVS_EX_FULLROWSELECT);
	m_ctrProc.ModifyStyle(0, WS_HSCROLL|WS_VSCROLL);
	m_ctrProc.ModifyStyleEx(0, WS_EX_CLIENTEDGE|WS_EX_NOPARENTNOTIFY);
	
	PostMessage(WM_INIT_PAGE);
	return 0;
}

BOOL CPageService::OnEraseBkgnd(CDC* pDC) 
{
	// TODO: Add your message handler code here and/or call default
	CRect rect;
	GetClientRect(&rect);
	pDC->FillSolidRect(rect, LoadAsRGB(_T("property.page.color"), _T("240,240,234")));

	return TRUE;
}

LRESULT CPageService::OnInitPage(WPARAM wParam, LPARAM lParam)
{
	static char *install_type[] = {{"uninstalled(1)"}, {"install-pending(2)"}, {"uninstall-pending(3)"}, {"installed(4)"}};
	static char *operate_type[] = {{"active(1)"}, {"continue-pending(2)"}, {"pause-pending(3)"}, {"paused(4)"}};
	static char *uninstall_type[] = {{"cannot-be-uninstalled(1)"}, {"can-be-uninstalled(2)"}};
	static char *pause_type[] = {{"cannot-be-paused(1)"}, {"can-be-paused(2)"}};
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

	CStringEx strOID;
	strOID = _T("1.3.6.1.4.1.77.1.2.3.1.");
	CStringEx strRangeOID;
	strRangeOID = _T("1.3.6.1.4.1.77.1.2.3.1.");
	CStringEx strIndexOID;
	strIndexOID = _T("1.3.6.1.4.1.77.1.2.3.1.1.");

	char *oid, *range = strRangeOID.toChar();
	char *index = strIndexOID.toChar();
	int range_len = strlen(range);
	int index_len = strlen(index);
	CNewGuard guard3(range);
	CNewGuard guard4(index);

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
			strtooid(byoid+range_len, OID);
			if (memcmp(byoid, index, index_len) == 0)
			{
				int value = m_ctrProc.InsertItem(m_ctrProc.GetItemCount(), CString(byval));
				item_map.SetAt(CString(byoid+index_len), value);
			}
			else
			{
				int item_index = OID->ptr[0];
				if (item_index == 2)
					m_ctrProc.SetItemText(item_map[CString(byoid+index_len)], item_index-1, CString(install_type[atoi(byval)-1]));
				else if (item_index == 3)
					m_ctrProc.SetItemText(item_map[CString(byoid+index_len)], item_index-1, CString(operate_type[atoi(byval)-1]));
				else if (item_index == 4)
					m_ctrProc.SetItemText(item_map[CString(byoid+index_len)], item_index-1, CString(uninstall_type[atoi(byval)-1]));
				else if (item_index == 5)
					m_ctrProc.SetItemText(item_map[CString(byoid+index_len)], item_index-1, CString(pause_type[atoi(byval)-1]));
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

void CPageService::OnRefresh()
{
	OnInitPage(0, 0);
}
