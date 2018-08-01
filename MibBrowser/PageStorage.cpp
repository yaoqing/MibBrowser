// PageStorage.cpp : implementation file
//

#include "stdafx.h"
#include "mibbrowser.h"
#include "PageStorage.h"
#include "StringEx.h"
#include "NewGuard.h"
#include "PropertyWnd.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPageStorage
IMPLEMENT_DYNCREATE(CPageStorage, CWnd)

CPageStorage::CPageStorage()
{
	font = new CFont();
	font->CreatePointFont(LoadAsInteger(_T("property.dlg.font.size"), 90), LoadAsString(_T("property.dlg.font.name"), _T("Arial")));
}

CPageStorage::~CPageStorage()
{
	font->DeleteObject();
	delete font;
}


BEGIN_MESSAGE_MAP(CPageStorage, CWnd)
	//{{AFX_MSG_MAP(CPageStorage)
	ON_WM_CREATE()
	ON_WM_ERASEBKGND()
	ON_BN_CLICKED(7, OnRefresh)
	ON_REGISTERED_MESSAGE(WM_INIT_PAGE, OnInitPage)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CPageStorage message handlers

BOOL CPageStorage::OnEraseBkgnd(CDC* pDC) 
{
	// TODO: Add your message handler code here and/or call default
	CRect rect;
	GetClientRect(&rect);
	pDC->FillSolidRect(rect, LoadAsRGB(_T("property.page.color"), _T("240,240,234")));

	DrawLabel(pDC, CRect(15, 325, 95, 346), LoadAsString(_T("property.page.storage.memsize.text"), _T("Memory(MB)")));
	return TRUE;
}

int CPageStorage::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here
	m_ctrProc.Create(WS_CHILD|WS_VISIBLE|LVS_REPORT|LVS_SINGLESEL, CRect(15, 25, 380, 305), this, 0);
	m_ctrlMemSize.Create(WS_CHILD|WS_VISIBLE|ES_READONLY|ES_AUTOHSCROLL, CRect(100, 328, 200, 349), this, 0);
	m_ctrlRefesh.Create(LoadAsString(_T("property.page.refresh.text"), _T("&Refresh")), WS_CHILD|WS_VISIBLE, CRect(290, 326, 360, 350), this, 7);

	m_ctrlMemSize.SetBorderColor(LoadAsRGB(_T("property.page.storage.edit.border.color"), _T("49,105,198")));
	m_ctrProc.SetFont(font);
	m_ctrlMemSize.SetFont(font);
	m_ctrlRefesh.SetFont(font);

	m_ctrProc.InsertColumn(0, LoadAsString(_T("property.page.storage.index.text"), _T("Index")), LVCFMT_RIGHT, 64);
	m_ctrProc.InsertColumn(1, LoadAsString(_T("property.page.storage.type.text"), _T("Type")), LVCFMT_LEFT, 128);
	m_ctrProc.InsertColumn(2, LoadAsString(_T("property.page.storage.descr.text"), _T("Description")), LVCFMT_LEFT, 128);
	m_ctrProc.InsertColumn(3, LoadAsString(_T("property.page.storage.units.text"), _T("Units")), LVCFMT_RIGHT, 64);
	m_ctrProc.InsertColumn(4, LoadAsString(_T("property.page.storage.size.text"), _T("Size")), LVCFMT_RIGHT, 64);
	m_ctrProc.InsertColumn(5, LoadAsString(_T("property.page.storage.used.text"), _T("Used")), LVCFMT_RIGHT, 64);
	m_ctrProc.InsertColumn(6, LoadAsString(_T("property.page.storage.failures.text"), _T("Failures")), LVCFMT_RIGHT, 64);
	m_ctrProc.InsertColumn(7, LoadAsString(_T("property.page.storage.percent.text"), _T("Percent")), LVCFMT_RIGHT, 64);
	m_ctrProc.SetExtendedStyle(LVS_EX_FULLROWSELECT);
	m_ctrProc.ModifyStyle(0, WS_HSCROLL|WS_VSCROLL);
	m_ctrProc.ModifyStyleEx(0, WS_EX_CLIENTEDGE|WS_EX_NOPARENTNOTIFY);
	
	PostMessage(WM_INIT_PAGE);
	return 0;
}

LRESULT CPageStorage::OnInitPage(WPARAM wParam, LPARAM lParam)
{
	static char *storage_oid[] = {
		{"1.3.6.1.2.1.25.2.1.1"},			//hrStorageOther
		{"1.3.6.1.2.1.25.2.1.2"},			//hrStorageRam
		{"1.3.6.1.2.1.25.2.1.3"},			//hrStorageVirtualMemory
		{"1.3.6.1.2.1.25.2.1.4"},			//hrStorageFixedDisk
		{"1.3.6.1.2.1.25.2.1.5"},			//hrStorageRemovableDisk
		{"1.3.6.1.2.1.25.2.1.6"},			//hrStorageFloppyDisk
		{"1.3.6.1.2.1.25.2.1.7"},			//hrStorageCompactDisc
		{"1.3.6.1.2.1.25.2.1.8"},			//hrStorageRamDisk
		{"1.3.6.1.2.1.25.2.1.9"},			//hrStorageFlashMemory
		{"1.3.6.1.2.1.25.2.1.10"}			//hrStorageNetworkDisk
	};
	static char *storage_type[] = {
		{"hrStorageOther"},
		{"hrStorageRam"},
		{"hrStorageVirtualMemory"},
		{"hrStorageFixedDisk"},
		{"hrStorageRemovableDisk"},
		{"hrStorageFloppyDisk"},
		{"hrStorageCompactDisc"},
		{"hrStorageRamDisk"},
		{"hrStorageFlashMemory"},
		{"hrStorageNetworkDisk"}
	};
	CMap<CString, CString&, CString, CString&> storage_map;
	for (int i = 0; i < sizeof(storage_oid)/sizeof(storage_oid[0]); i++)
		storage_map.SetAt(CString(storage_oid[i]), CString(storage_type[i]));

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
	strOID = _T("1.3.6.1.2.1.25.2.3.1.");
	CStringEx strRangeOID;
	strRangeOID = _T("1.3.6.1.2.1.25.2.3.1.");

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
				if (item_index == 2)
				{
					CString strValue;
					storage_map.Lookup(CString(byval), strValue);
					m_ctrProc.SetItemText(item_map[oidatindex(OID, -1)], item_index-1, strValue);
				}
				else if (item_index >= 5 && item_index <= 6)
				{
					TCHAR szText[256];
					m_ctrProc.GetItemText(item_map[oidatindex(OID, -1)], 3, szText, 256);
					int nSize = _ttoi((TCHAR *)szText);
					int nCount = atoi(byval);
					double s = 1.0 * nCount / 1024 / 1024;
					_stprintf(szText, _T("%.0f"), s * nSize);
					m_ctrProc.SetItemText(item_map[oidatindex(OID, -1)], item_index-1, null ? _T("null") : szText);
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

	TCHAR szText[256];
	TCHAR szText1[256];
	TCHAR szText2[256];
	int nText1, nText2;
	for (i = 0; i < m_ctrProc.GetItemCount(); i++)
	{
	
		m_ctrProc.GetItemText(i, 4, szText1, 256);
		m_ctrProc.GetItemText(i, 5, szText2, 256);
		nText1 = _ttoi(szText1);
		nText2 = _ttoi(szText2);
		if (nText1 == 0)
			_stprintf(szText, _T("%.0f%%"), 0);
		else
			_stprintf(szText, _T("%.0f%%"), nText2 * 100.0 / nText1);
		m_ctrProc.SetItemText(i, 7, szText);
	}

	status = GetSNMPVal(pIp, 0, "1.3.6.1.2.1.25.2.2.0", pCommunity, pWnd->m_nPort, timeout, SNMP_PDU_GET, byoid, byval, null, valtype);
	if (status == SNMPAPI_SUCCESS)
	{
		_stprintf(szText, _T("%d"), atoi(byval)/1024);
		m_ctrlMemSize.SetWindowText(szText);
	}
	free(byoid);
	free(byval);

	return TRUE;
}

void CPageStorage::DrawLabel(CDC *pDC, CRect &rect, CString szText)
{
	CFont *pOldFont = pDC->SelectObject(font);
	
	pDC->SetTextColor(LoadAsRGB(_T("property.dlg.font.color"), _T("8,0,0")));
	pDC->DrawText(szText, rect, DT_LEFT|DT_BOTTOM|DT_SINGLELINE);

	pDC->SelectObject(pOldFont);
}

void CPageStorage::OnRefresh()
{
	OnInitPage(0, 0);
}
