// PagePerformance.cpp : implementation file
//

#include "stdafx.h"
#include "mibbrowser.h"
#include "PagePerformance.h"
#include "StringEx.h"
#include "NewGuard.h"
#include "PropertyWnd.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPagePerformance
IMPLEMENT_DYNCREATE(CPagePerformance, CWnd)

CPagePerformance::CPagePerformance()
{
	font = new CFont();
	font->CreatePointFont(LoadAsInteger(_T("property.dlg.font.size"), 90), LoadAsString(_T("property.dlg.font.name"), _T("Arial")));
}

CPagePerformance::~CPagePerformance()
{
	font->DeleteObject();
	delete font;
}


BEGIN_MESSAGE_MAP(CPagePerformance, CWnd)
	//{{AFX_MSG_MAP(CPagePerformance)
	ON_WM_CREATE()
	ON_WM_TIMER()
	ON_WM_ERASEBKGND()
	ON_REGISTERED_MESSAGE(WM_INIT_PAGE, OnInitPage)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CPagePerformance message handlers

LRESULT CPagePerformance::OnInitPage(WPARAM wParam, LPARAM lParam)
{
	return TRUE;
}

int CPagePerformance::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here
	CRect rect1, rect2, rect3, rect4;
	m_wnd1.Create(LoadAsString(_T("property.page.performance.cpu.text"), _T("CPU")), WS_CHILD|WS_VISIBLE|BS_GROUPBOX, CRect(15,15,135,165), this, 1);
	m_wnd2.Create(LoadAsString(_T("property.page.performance.cpu.record.text"), _T("CPU Record")), WS_CHILD|WS_VISIBLE|BS_GROUPBOX, CRect(155,15,380,165), this, 1);
	m_wnd3.Create(LoadAsString(_T("property.page.performance.memory.text"), _T("Memory")), WS_CHILD|WS_VISIBLE|BS_GROUPBOX, CRect(15,175,135,325), this, 1);
	m_wnd4.Create(LoadAsString(_T("property.page.performance.memory.record.text"), _T("Memory Record")), WS_CHILD|WS_VISIBLE|BS_GROUPBOX, CRect(155,175,380,325), this, 1);

	m_wnd1.SetFont(font);
	m_wnd2.SetFont(font);
	m_wnd3.SetFont(font);
	m_wnd4.SetFont(font);
	m_wnd1.GetClientRect(rect1);
	m_wnd2.GetClientRect(rect2);
	m_wnd3.GetClientRect(rect3);
	m_wnd4.GetClientRect(rect4);
	rect1.DeflateRect(5, 15, 5, 5);
	rect2.DeflateRect(5, 15, 5, 5);
	rect3.DeflateRect(5, 15, 5, 5);
	rect4.DeflateRect(5, 15, 5, 5);

	m_wndCpuRate.Create(NULL, NULL, WS_CHILD|WS_VISIBLE, rect1, &m_wnd1, NULL);
	m_wndCpuRecord.Create(NULL, NULL, WS_CHILD|WS_VISIBLE, rect2, &m_wnd2, NULL);
	m_wndPfRate.Create(NULL, NULL, WS_CHILD|WS_VISIBLE, rect3, &m_wnd3, NULL);
	m_wndPageRecord.Create(NULL, NULL, WS_CHILD|WS_VISIBLE, rect4, &m_wnd4, NULL);

	SetTimer(1, 1000, NULL);
	PostMessage(WM_INIT_PAGE);
	return 0;
}

void CPagePerformance::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
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
	status = GetSNMPVal(pIp, 0, "1.3.6.1.2.1.25.3.3.1.2", pCommunity, pWnd->m_nPort, timeout, SNMP_PDU_GETNEXT, byoid, byval, null, valtype);
	if (status == SNMPAPI_FAILURE)
	{
		KillTimer(1);
		MsgBoxModal(LoadAsString(_T("snmp.error.text"), _T("SNMP not be supported or timeout!")), GetParent()->GetParent());
		return;
	}
	else
	{
		int percent = atoi(byval);
		m_wndCpuRate.SetPercent(percent);
		m_wndCpuRecord.AddPercent(percent);
	}
	free(byoid);
	free(byval);

	status = GetSNMPVal(pIp, 0, "1.3.6.1.2.1.25.2.2.0", pCommunity, pWnd->m_nPort, timeout, SNMP_PDU_GET, byoid, byval, null, valtype);
	if (status == SNMPAPI_FAILURE)
	{
		KillTimer(1);
		MsgBoxModal(LoadAsString(_T("snmp.error.text"), _T("SNMP not be supported or timeout!")), GetParent()->GetParent());
		return;
	}
	int memsize = atoi(byval);
	free(byoid);
	free(byval);

	CStringEx strOID;
	strOID = _T("1.3.6.1.2.1.25.5.1.1.2");
	CStringEx strRangeOID;
	strRangeOID = _T("1.3.6.1.2.1.25.5.1.1.2.");

	char *oid, *range = strRangeOID.toChar();	
	int procsize = 0, range_len = strlen(range);
	CNewGuard guard3(range);

	do
	{
		oid = strOID.toChar();
		CNewGuard guard(oid);
		status = GetSNMPVal(pIp, 0, oid, pCommunity, pWnd->m_nPort, timeout, SNMP_PDU_GETNEXT, byoid, byval, null, valtype);
		if (status == SNMPAPI_FAILURE)
		{
			KillTimer(1);
			MsgBoxModal(LoadAsString(_T("snmp.error.text"), _T("SNMP not be supported or timeout!")), GetParent()->GetParent());
			return;
		}
		if (strncmp(byoid, range, range_len) == 0)
		{
			strOID = byoid;
			procsize += atoi(byval);
			free(byoid);
			free(byval);
		}
		else
		{
			free(byoid);
			free(byval);
			break;
		}
	}while (TRUE);

	int percent = (memsize) ? procsize * 100 / memsize : 0;
	m_wndPfRate.SetPercent(percent);
	m_wndPageRecord.AddPercent(percent);

	CWnd::OnTimer(nIDEvent);
}

BOOL CPagePerformance::OnEraseBkgnd(CDC* pDC) 
{
	// TODO: Add your message handler code here and/or call default
	CRect rect;
	GetClientRect(&rect);
	pDC->FillSolidRect(rect, LoadAsRGB(_T("property.page.color"), _T("240,240,234")));
	return TRUE;
}
