// PageSystemDesc.cpp : implementation file
//

#include "stdafx.h"
#include "mibbrowser.h"
#include "PageSystemDesc.h"
#include "StringEx.h"
#include "NewGuard.h"
#include "PropertyWnd.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPageSystemDesc
IMPLEMENT_DYNCREATE(CPageSystemDesc, CWnd)

CPageSystemDesc::CPageSystemDesc()
{
	font = new CFont();
	font->CreatePointFont(LoadAsInteger(_T("property.dlg.font.size"), 90), LoadAsString(_T("property.dlg.font.name"), _T("Arial")));
}

CPageSystemDesc::~CPageSystemDesc()
{
	font->DeleteObject();
	delete font;
}


BEGIN_MESSAGE_MAP(CPageSystemDesc, CWnd)
	//{{AFX_MSG_MAP(CPageSystemDesc)
	ON_WM_ERASEBKGND()
	ON_WM_CREATE()
	ON_BN_CLICKED(7, OnRefresh)
	ON_REGISTERED_MESSAGE(WM_INIT_PAGE, OnInitPage)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CPageSystemDesc message handlers

BOOL CPageSystemDesc::OnEraseBkgnd(CDC* pDC) 
{
	// TODO: Add your message handler code here and/or call default	
	CRect rect;
	GetClientRect(&rect);
	pDC->FillSolidRect(rect, LoadAsRGB(_T("property.page.color"), _T("240,240,234")));

	DrawLabel(pDC, CRect(25, 20, 168, 41), LoadAsString(_T("property.page.sysdesc.desc.text"), _T("SysDesc:")));
	DrawLabel(pDC, CRect(25, 52, 168, 73), LoadAsString(_T("property.page.sysdesc.oid.text"), _T("SysOID:")));
	DrawLabel(pDC, CRect(25, 84, 168, 105), LoadAsString(_T("property.page.sysdesc.uptime.text"), _T("SysUpTime:")));
	DrawLabel(pDC, CRect(25, 116, 168, 137), LoadAsString(_T("property.page.sysdesc.contact.text"), _T("SysContact:")));
	DrawLabel(pDC, CRect(25, 148, 168, 169), LoadAsString(_T("property.page.sysdesc.name.text"), _T("SysName:")));
	DrawLabel(pDC, CRect(25, 180, 168, 201), LoadAsString(_T("property.page.sysdesc.location.text"), _T("SysLocation:")));
	DrawLabel(pDC, CRect(25, 212, 168, 233), LoadAsString(_T("property.page.sysdesc.services.text"), _T("SysServices:")));

	return TRUE;
}

int CPageSystemDesc::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here
	m_ctrlDesc.Create(WS_CHILD|WS_VISIBLE|ES_READONLY|ES_AUTOHSCROLL, CRect(180, 21, 360, 42), this, 0);
	m_ctrlOID.Create(WS_CHILD|WS_VISIBLE|ES_READONLY|ES_AUTOHSCROLL, CRect(180, 53, 360, 74), this, 1);
	m_ctrlUpTime.Create(WS_CHILD|WS_VISIBLE|ES_READONLY|ES_AUTOHSCROLL, CRect(180, 85, 360, 106), this, 2);
	m_ctrlContact.Create(WS_CHILD|WS_VISIBLE|ES_READONLY|ES_AUTOHSCROLL, CRect(180, 117, 360, 138), this, 3);
	m_ctrlName.Create(WS_CHILD|WS_VISIBLE|ES_READONLY|ES_AUTOHSCROLL, CRect(180, 149, 360, 170), this, 4);
	m_ctrlLocation.Create(WS_CHILD|WS_VISIBLE|ES_READONLY|ES_AUTOHSCROLL, CRect(180, 181, 360, 202), this, 5);
	m_ctrlServices.Create(WS_CHILD|WS_VISIBLE|ES_READONLY|ES_AUTOHSCROLL, CRect(180, 213, 360, 234), this, 6);
	m_ctrlRefesh.Create(LoadAsString(_T("property.page.refresh.text"), _T("&Refresh")), WS_CHILD | WS_VISIBLE, CRect(290, 326, 360, 350), this, 7);

	COLORREF color = LoadAsRGB(_T("property.page.sysdesc.edit.border.color"), _T("49,105,198"));
	m_ctrlDesc.SetBorderColor(color);
	m_ctrlOID.SetBorderColor(color);
	m_ctrlUpTime.SetBorderColor(color);
	m_ctrlContact.SetBorderColor(color);
	m_ctrlName.SetBorderColor(color);
	m_ctrlLocation.SetBorderColor(color);
	m_ctrlServices.SetBorderColor(color);

	m_ctrlDesc.SetFont(font);
	m_ctrlOID.SetFont(font);
	m_ctrlUpTime.SetFont(font);
	m_ctrlContact.SetFont(font);
	m_ctrlName.SetFont(font);
	m_ctrlLocation.SetFont(font);
	m_ctrlServices.SetFont(font);
	m_ctrlRefesh.SetFont(font);
	
	PostMessage(WM_INIT_PAGE);
	return 0;
}

void CPageSystemDesc::DrawLabel(CDC *pDC, CRect &rect, CString szText)
{
	CFont *pOldFont = pDC->SelectObject(font);
	
	pDC->SetTextColor(LoadAsRGB(_T("property.dlg.font.color"), _T("8,0,0")));
	pDC->DrawText(szText, rect, DT_LEFT|DT_BOTTOM|DT_SINGLELINE);

	pDC->SelectObject(pOldFont);
}

LRESULT CPageSystemDesc::OnInitPage(WPARAM wParam, LPARAM lParam)
{
	static char *SystemOid[] = {
		{"1.3.6.1.2.1.1.1.0"},			//SysDescr
		{"1.3.6.1.2.1.1.2.0"},			//SysDescr
		{"1.3.6.1.2.1.1.3.0"},			//SysDescr
		{"1.3.6.1.2.1.1.4.0"},			//SysContact
		{"1.3.6.1.2.1.1.5.0"},			//SysName
		{"1.3.6.1.2.1.1.6.0"},			//SysLocation
		{"1.3.6.1.2.1.1.7.0"}			//SysServices
		};

	CPropertyWnd *pWnd = (CPropertyWnd *)GetParent();
	CStringEx szIp(pWnd->m_strIpAddr);
	CStringEx szCommunity(pWnd->m_strCommunity);
	
	char *pIp = szIp.toChar();
	char *pCommunity = szCommunity.toChar();
	char *byoid, *byval;
	bool null = true;
	DWORD valtype = 0;

	CNewGuard guard1(pIp);
	CNewGuard guard2(pCommunity);

	int timeout = LoadAsInteger(_T("snmp.udp.timeout"), 6);
	if (timeout < 0) timeout = 0;
	if (timeout > 60) timeout = 60;

	SNMPAPI_STATUS status;
	for (int i = 0; i < 7; i++)
	{
		status = GetSNMPVal(pIp, 0, SystemOid[i], pCommunity, pWnd->m_nPort, timeout, SNMP_PDU_GET, byoid, byval, null, valtype);
		if (status == SNMPAPI_FAILURE)
		{
			MsgBoxModal(LoadAsString(_T("snmp.error.text"), _T("SNMP not be supported or timeout!")), GetParent()->GetParent());
			free(byoid);
			free(byval);
			break;
		}
		else
			GetDlgItem(i)->SetWindowText(CString(byval));
		free(byoid);
		free(byval);
	}

	return TRUE;
}

void CPageSystemDesc::OnRefresh()
{
	OnInitPage(0, 0);
}
