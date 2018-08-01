// RegisterWnd.cpp : implementation file
//

#include "stdafx.h"
#include "mibbrowser.h"
#include "RegisterWnd.h"
#include "RegKeyEx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRegisterWnd

CRegisterWnd::CRegisterWnd()
{
	font = new CFont();
	font->CreatePointFont(LoadAsInteger(_T("register.dlg.font.size"), 90), LoadAsString(_T("register.dlg.font.name"), _T("Arial Bold")));
}

CRegisterWnd::~CRegisterWnd()
{
	font->DeleteObject();
	delete font;
}


BEGIN_MESSAGE_MAP(CRegisterWnd, CWnd)
	//{{AFX_MSG_MAP(CRegisterWnd)
	ON_WM_ERASEBKGND()
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CRegisterWnd message handlers

void CRegisterWnd::DrawLabel(CDC *pDC, CRect &rect, CString szText)
{
	CFont *pOldFont = pDC->SelectObject(font);
	
	pDC->SetTextColor(LoadAsRGB(_T("register.dlg.font.color"), _T("8,0,0")));
	pDC->DrawText(szText, rect, DT_LEFT|DT_BOTTOM|DT_SINGLELINE);

	pDC->SelectObject(pOldFont);
}

BOOL CRegisterWnd::OnEraseBkgnd(CDC* pDC) 
{
	// TODO: Add your message handler code here and/or call default
	CRect rect;
	GetClientRect(&rect);
	pDC->FillSolidRect(rect, LoadAsRGB(_T("register.dlg.color"), _T("255,255,255")));

	CString szText, sss;
	sss.LoadString(IDR_MAINFRAME);
	DrawLabel(pDC, CRect(25, 20, 265, 41), LoadAsString(_T("register.dlg.owner.text"), _T("Owner(*):")));
	DrawLabel(pDC, CRect(25, 52, 335, 73), LoadAsString(_T("register.dlg.serial.text"), _T("Serial(*):")));
	DrawLabel(pDC, CRect(25, 84, 335, 105), LoadAsString(_T("register.dlg.organization.text"), _T("Organization:")));
	
	return TRUE;
}

int CRegisterWnd::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here
	m_ctrlOwner.Create(WS_CHILD | WS_VISIBLE, CRect(100, 21, 315, 42), this, 0);
	m_ctrlSerial.Create(WS_CHILD | WS_VISIBLE, CRect(100, 53, 315, 74), this, 0);
	m_ctrlOrganization.Create(WS_CHILD | WS_VISIBLE, CRect(100, 85, 315, 106), this, 0);

	m_ctrlOk.Create(LoadAsString(_T("register.dlg.ok.text"), _T("&Ok")), WS_CHILD | WS_VISIBLE, CRect(155, 117, 225, 141), this, IDOK);
	m_ctrlOk.SetFont(font);
	m_ctrlCancel.Create(LoadAsString(_T("register.dlg.cancel.text"), _T("&Cancel")), WS_CHILD | WS_VISIBLE, CRect(245, 117, 315, 141), this, IDCANCEL);
	m_ctrlCancel.SetFont(font);

	COLORREF color = LoadAsRGB(_T("register.dlg.edit.border.color"), _T("49,105,198"));
	m_ctrlOwner.SetBorderColor(color);
	m_ctrlSerial.SetBorderColor(color);
	m_ctrlOrganization.SetBorderColor(color);

	m_ctrlOwner.SetFont(font);
	m_ctrlSerial.SetFont(font);
	m_ctrlOrganization.SetFont(font);

	CRegKeyEx reg;
	HKEY hKey = reg.OpenReg(_T("SOFTWARE\\AILK\\MibBrowser"));
	m_ctrlOwner.SetWindowText(reg.GetKeyValue(hKey, _T("Owner")));
	m_ctrlSerial.SetWindowText(reg.GetKeyValue(hKey, _T("Serial")));
	m_ctrlOrganization.SetWindowText(reg.GetKeyValue(hKey, _T("Organization")));
	reg.CloseReg(hKey);
	
	return ShowWindow(SW_SHOW);
}

BOOL CRegisterWnd::OnCommand(WPARAM wParam, LPARAM lParam) 
{
	// TODO: Add your specialized code here and/or call the base class
	switch (wParam)
	{
	case IDOK:
		{
			CRegKeyEx reg;
			HKEY hKey = reg.OpenReg(_T("SOFTWARE\\AILK\\MibBrowser"));
			CString szOwner, szSerial, szOrganization;
			m_ctrlOwner.GetWindowText(szOwner);
			m_ctrlSerial.GetWindowText(szSerial);
			m_ctrlOrganization.GetWindowText(szOrganization);
			reg.SetKeyValue(hKey, _T("Owner"), szOwner);
			reg.SetKeyValue(hKey, _T("Serial"), szSerial);
			reg.SetKeyValue(hKey, _T("Organization"), szOrganization);
			reg.CloseReg(hKey);

			GetParent()->PostMessage(WM_CLOSE);
			MsgBoxModal(LoadAsString(_T("register.dlg.thanks.text"), _T("Thanks for your register!")), AfxGetMainWnd(), NULL, &LoadAsString(_T("register.dlg.caption.text"), _T("SNMP MibBrowser")));
			return TRUE;
		}
	case IDCANCEL:
		return GetParent()->PostMessage(WM_CLOSE);
	}
	
	return CWnd::OnCommand(wParam, lParam);
}
