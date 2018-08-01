// ChildFrame.cpp : implementation file
//

#include "stdafx.h"
#include "MibBrowser.h"
#include "ChildFrame.h"
#include "PanelWnd.h"
#include "SplitterBar.h"
#include "MibTreeWnd.h"
#include "MibInfoWnd.h"
#include "MibLoader.h"
#include "ChildView.h"
#include "MemoWnd.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CChildFrame

CChildFrame::CChildFrame()
{
	m_split_wnd0 = NULL;
	m_split_wnd1 = NULL;
	m_split_wnd2 = NULL;
}

CChildFrame::~CChildFrame()
{
	if (m_split_wnd0)
		delete m_split_wnd0;
	if (m_split_wnd1)
		delete m_split_wnd1;
	if (m_split_wnd2)
		delete m_split_wnd2;
}


BEGIN_MESSAGE_MAP(CChildFrame, CWnd)
	//{{AFX_MSG_MAP(CChildFrame)
	ON_WM_CREATE()
	ON_WM_WINDOWPOSCHANGED()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CChildFrame message handlers

BOOL CChildFrame::PreCreateWindow(CREATESTRUCT& cs) 
{
	//CBrush brush;
	cs.style = WS_CHILD | WS_VISIBLE;
	//brush.CreateSolidBrush(RGB(255, 255, 255));
	/*cs.lpszClass = AfxRegisterWndClass(CS_HREDRAW|CS_VREDRAW|CS_DBLCLKS,
		::LoadCursor(NULL, IDC_ARROW), (HBRUSH)brush.Detach(), NULL);*/

	return CWnd::PreCreateWindow(cs);
}

int CChildFrame::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here
	m_split_wnd0 = new CSplitterBar(FALSE, RUNTIME_CLASS(CPanelWnd), RUNTIME_CLASS(CPanelWnd), this);
	m_split_wnd0->Create(220);

	m_split_wnd1 = new CSplitterBar(TRUE, RUNTIME_CLASS(CMibTreeWnd), RUNTIME_CLASS(CMibInfoWnd), m_split_wnd0->GetLeftWnd());
	m_split_wnd1->Create(320);

	m_split_wnd2 = new CSplitterBar(TRUE, RUNTIME_CLASS(CChildView), RUNTIME_CLASS(CMemoWnd), m_split_wnd0->GetRightWnd());
	m_split_wnd2->Create(320);

	m_split_wnd0->SetLeftOrTopSplitter(m_split_wnd1);
	m_split_wnd0->SetRightOrBottomSplitter(m_split_wnd2);
	g_wndView = (CChildView *)m_split_wnd2->GetTopWnd();
	g_wndMemo = (CMemoWnd *)m_split_wnd2->GetBottomWnd();

	CMibTreeWnd *mib_tree = (CMibTreeWnd *)m_split_wnd1->GetTopWnd();
	CMibInfoWnd *mib_info = (CMibInfoWnd *)m_split_wnd1->GetBottomWnd();
	mib_tree->SetMibInfoWnd(mib_info);
	mib_info->SetMibTreeWnd(mib_tree);	

	mib_tree->CreateMibTree();
	CreateTree(mib_tree, mib_tree->GetRootItem(), mib_tree->GetRoot());
	HTREEITEM hItem = mib_tree->GetRootItem();
	mib_tree->SelectItem(hItem);
	/*for (int i = 0; i < 4 && hItem; i++)
	{
		mib_tree->Expand(hItem, TVE_EXPAND);
		hItem = mib_tree->GetChildItem(hItem);
	}*/
	g_mibRoot = mib_tree->GetRoot();

	return 0;
}

void CChildFrame::OnWindowPosChanged(WINDOWPOS FAR* lpwndpos) 
{
	CWnd::OnWindowPosChanged(lpwndpos);
	
	m_split_wnd0->PostMessage(WM_ADJUST_SPLITTER);
}

void CChildFrame::CreateTree(CMibTreeWnd *mib_tree, HTREEITEM hItem, CMibNode *node)
{
	if (!node)
		return;

	if(!node->child_level)
		return;

	CArray<int, int> sort;
	int index;
	CMibNode *child_node;
	HTREEITEM hParent = hItem;
	POSITION pos = node->child_level->GetStartPosition();
	while (pos)
	{
		if (!node->child_level)
			break;

		node->child_level->GetNextAssoc(pos, index, child_node);

		if (sort.GetSize() == 0)
			sort.Add(index);
		else
		{
			for (int i = sort.GetSize()-1; i >= 0; i--)
			{
				if (index > sort.GetAt(i))
				{
					sort.InsertAt(i+1, index);
					break;
				}
				else
				{
					if (i == 0)
					{
						sort.InsertAt(i, index);
						break;
					}
				}
			}
		}
	}

	for (int i = 0; i < sort.GetSize(); i++)
	{
		if (!node->child_level)
			break;

		node->child_level->Lookup(sort.GetAt(i), child_node);
		if (hParent)
			if (child_node->child_level)
				hItem = mib_tree->InsertItem(child_node->node_name, 0, 1, hParent);
			else
				hItem = mib_tree->InsertItem(child_node->node_name, 2, 2, hParent);
		else
			hItem = mib_tree->InsertItem(child_node->node_name, 0, 1, NULL);
		mib_tree->SetItemData(hItem, (DWORD)child_node);
		
		CreateTree(mib_tree, hItem, child_node);
	}
}
