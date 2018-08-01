// MibTreeWnd.cpp : implementation file
//

#include "stdafx.h"
#include "mibbrowser.h"
#include "MibTreeWnd.h"
#include "MibInfoWnd.h"
#include "MibLoader.h"
#include "StringEx.h"
#include "StringToken.h"
#include "MenuEx.h"
#include "MacXFrame.h"
#include "PromptWnd.h"
#include "ChildView.h"
#include "SpyedWnd.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNCREATE(CMibTreeWnd, CTreeCtrl)
/////////////////////////////////////////////////////////////////////////////
// CMibTreeWnd

CMibTreeWnd::CMibTreeWnd()
{
	m_mib_info_wnd = 0;
	m_mib_loader = new CMibLoader();
	m_listImgs.Create(16, 16, ILC_COLOR24|ILC_MASK, 3, 1);
	CBitmap img;
	img.LoadBitmap(IDB_LEAF_ADD);
	m_listImgs.Add(&img, RGB(255, 0, 255));
	img.DeleteObject();
	img.LoadBitmap(IDB_LEAF_SUB);
	m_listImgs.Add(&img, RGB(255, 0, 255));
	img.DeleteObject();
	img.LoadBitmap(IDB_LEAF);
	m_listImgs.Add(&img, RGB(255, 0, 255));
	img.DeleteObject();
}

CMibTreeWnd::~CMibTreeWnd()
{
	delete m_mib_loader;
}

BEGIN_MESSAGE_MAP(CMibTreeWnd, CTreeCtrl)
	//{{AFX_MSG_MAP(CMibTreeWnd)
	ON_NOTIFY_REFLECT(TVN_SELCHANGED, OnSelchanged)
	ON_WM_CONTEXTMENU()
	ON_NOTIFY_REFLECT(NM_RCLICK, OnRclick)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMibTreeWnd message handlers

BOOL CMibTreeWnd::PreCreateWindow(CREATESTRUCT& cs) 
{
	cs.style |= WS_CHILD | WS_VISIBLE | TVS_HASBUTTONS | TVS_FULLROWSELECT | TVS_LINESATROOT;
	cs.lpszClass = _T("SysTreeView32");

	return CTreeCtrl::PreCreateWindow(cs);
}

void CMibTreeWnd::CreateMibTree()
{
	SetImageList(&m_listImgs, TVSIL_NORMAL);

	HANDLE handle;
	WIN32_FIND_DATA	ffd;
	handle = FindFirstFile(_T(".\\mib\\*.mib"), &ffd);
	if( handle == INVALID_HANDLE_VALUE )
	{
		FindClose(handle);
		return;
	}
	
 	CString mib_name;
	do
	{
		mib_name.Format(_T(".\\mib\\%s"), ffd.cFileName);
		m_mib_loader->LoadMib(mib_name);
	} while (FindNextFile(handle, &ffd));
	FindClose(handle);
}

void CMibTreeWnd::SetMibInfoWnd(CMibInfoWnd *mib_info_wnd)
{
	m_mib_info_wnd = mib_info_wnd;
}

CMibNode *CMibTreeWnd::GetRoot()
{
	return m_mib_loader->GetRoot();
}

void CMibTreeWnd::OnSelchanged(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here

	HTREEITEM hItem = GetSelectedItem();
	CMibNode *node = (CMibNode *)GetItemData(hItem);

	m_mib_info_wnd->PostMessage(WM_SHOW_MIB_INFO);
	*pResult = TRUE;
}

void CMibTreeWnd::OnRclick(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	CMenuEx menu;
	CPoint point;
	GetCursorPos(&point);
	
	menu.CreatePopupMenu();
	menu.InsertMenu(0, MF_BYPOSITION|MF_STRING, 0, LoadAsString(_T("spy.menu.prompt.text"), _T("Prompt For &OID ... ")));
 	menu.TrackPopupMenu(TPM_LEFTALIGN|TPM_RIGHTBUTTON, point.x, point.y, this);
	
	*pResult = 0;
}

BOOL CMibTreeWnd::OnCommand(WPARAM wParam, LPARAM lParam) 
{
	// TODO: Add your specialized code here and/or call the base class
	CTreeCtrl::OnCommand(wParam, lParam);

	switch (wParam)
	{
	case 0:
		{
			CPoint point;
			GetCursorPos(&point);
			HTREEITEM hItem = GetSelectedItem();
			CMibNode *node = (CMibNode *)GetItemData(hItem);
			CString szOID, szTemp;
			while (node && node->parent_node)
			{
				szTemp.Format(_T("%u."), node->node_id);
				szOID.Insert(0, szTemp);
				node = node->parent_node;
			}
			if (!szOID.IsEmpty())
				szOID.SetAt(szOID.GetLength()-1, 0);
			
			CRect rect(point.x, point.y, point.x+472, point.y+200);
			CMacXFrame *dlg = new CMacXFrame();
			CString szIP;
			if (g_wndView->m_pFocusWnd) szIP = g_wndView->m_pFocusWnd->m_strIpAddr;
			CPromptWnd *pWnd = new CPromptWnd(szIP, LoadAsInteger(_T("snmp.udp.port"), 161), LoadAsString(_T("snmp.community.read"), _T("public")));
			pWnd->SetOID(szOID);
			dlg->SetChildFrame(pWnd);
			dlg->CreateModal(NULL, _T("Prompt For OID"), 0, rect, AfxGetMainWnd(), 0, NULL, NULL);
		}
		break;
	}

	return TRUE;
}
