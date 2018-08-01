// MibInfoWnd.cpp : implementation file
//

#include "stdafx.h"
#include "mibbrowser.h"
#include "MibInfoWnd.h"
#include "MibTreeWnd.h"
#include "MibLoader.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNCREATE(CMibInfoWnd, CWnd)
/////////////////////////////////////////////////////////////////////////////
// CMibInfoWnd

CMibInfoWnd::CMibInfoWnd()
{
	CBitmap img;
	CX_PROP = 64;
	m_posArrow = 0;
	m_mib_tree_wnd = 0;
	img.LoadBitmap(IDB_ARROW);
	m_listImgs.Create(16, 16, ILC_COLOR24|ILC_MASK, 1, 1);
	m_listImgs.Add(&img, RGB(255, 0, 255));
}

CMibInfoWnd::~CMibInfoWnd()
{
}


BEGIN_MESSAGE_MAP(CMibInfoWnd, CWnd)
	//{{AFX_MSG_MAP(CMibInfoWnd)
	ON_WM_WINDOWPOSCHANGED()
	ON_WM_ERASEBKGND()
	ON_REGISTERED_MESSAGE(WM_SHOW_MIB_INFO, OnShowMibInfo)
	ON_WM_MOUSEMOVE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CMibInfoWnd message handlers

BOOL CMibInfoWnd::PreCreateWindow(CREATESTRUCT& cs) 
{
	// TODO: Add your specialized code here and/or call the base class
	cs.style |= WS_CHILD | WS_VISIBLE;

	return CWnd::PreCreateWindow(cs);
}

void CMibInfoWnd::OnWindowPosChanged(WINDOWPOS FAR* lpwndpos) 
{
	CWnd::OnWindowPosChanged(lpwndpos);
	
	// TODO: Add your message handler code here
	
}

void CMibInfoWnd::SetMibTreeWnd(CMibTreeWnd *mib_tree_wnd)
{
	m_mib_tree_wnd = mib_tree_wnd;
}

LRESULT CMibInfoWnd::OnShowMibInfo(WPARAM wParam, LPARAM lParam)
{
	CRect rect;
	GetClientRect(rect);

	CFont font;
	font.CreatePointFont(LoadAsInteger(_T("info.font.size"), 90), LoadAsString(_T("info.font.name"), _T("Arial")));
	CPen pen;
	pen.CreatePen(PS_SOLID, 1, LoadAsRGB(_T("mib.info.line.color"), _T("172,168,153")));

	CDC *pDC = GetDC();
	CDC dcmem;
	dcmem.CreateCompatibleDC(pDC);

	CBitmap img;
	img.CreateCompatibleBitmap(pDC, rect.Width(), rect.Height());
	CBitmap *pOldBmp = dcmem.SelectObject(&img);
	CFont *pOldFont = dcmem.SelectObject(&font);
	CPen *pOldPen = dcmem.SelectObject(&pen);
	dcmem.SetTextColor(LoadAsRGB(_T("mib.info.pen.color"), _T("8,0,0")));
	dcmem.SetBkMode(TRANSPARENT);	
	dcmem.FillSolidRect(rect, RGB(255, 255, 255));

	CSize size = dcmem.GetTextExtent(_T(">>"));
	if (size.cx < 16) size.cx = 16;
	if (size.cy < 16) size.cy = 16; size.cy += 2;

	HTREEITEM hItem = m_mib_tree_wnd->GetSelectedItem();
	if (!hItem)
	{
		dcmem.SelectObject(&pOldFont);
		dcmem.SelectObject(pOldPen);
		dcmem.SelectObject(pOldBmp);
		ReleaseDC(pDC);
		return TRUE;
	}
	CMibNode *node = (CMibNode *)m_mib_tree_wnd->GetItemData(hItem);
	if (!node)
	{
		dcmem.SelectObject(&pOldFont);
		dcmem.SelectObject(pOldPen);
		dcmem.SelectObject(pOldBmp);
		ReleaseDC(pDC);
		return TRUE;
	}
	CMibNode *save = node;
	CString oid, oid_index;
	while (TRUE)
	{
		if (!node) break;
		if (!node->parent_node) break;
		oid_index.Format(_T("%d."), node->node_id);
		oid.Insert(0, oid_index);
		node = node->parent_node;
	}
	node = save;
	if (node->child_level)
		oid.Delete(oid.GetLength()-1);
	else
		oid += '0';
	int height = size.cy+1;

	CString name = node->node_name;
	height += size.cy+1;

	if (node->node_syntax)
	{
		CMibSyntax *s = node->node_syntax;
		if (s->SYNTAX)
			height += size.cy+1;
		if (s->ACCESS)
			height += size.cy+1;
		if (s->STATUS)
			height += size.cy+1;
		if (s->DESCRIPTION)
			height += size.cy+1;
		if (s->INDEX)
			height += size.cy+1;
	}

	dcmem.MoveTo(size.cx, 0);
	dcmem.LineTo(size.cx, rect.Height());
	dcmem.MoveTo(size.cx+1+CX_PROP, 0);
	dcmem.LineTo(size.cx+1+CX_PROP, height-1);

	m_rectProp.RemoveAll();
	for (int h = size.cy+1; h <= height; h += size.cy+1)
	{
		//dcmem.MoveTo(size.cx+1, h-1);
		//dcmem.LineTo(rect.Width(), h-1);
		CPairPropRect pair;
		pair.ind.SetRect(0, h-size.cy-1, size.cx, h-1);
		pair.key.SetRect(size.cx+1, h-size.cy-1, size.cx+1+CX_PROP, h-1);
		pair.prop.SetRect(size.cx+1+CX_PROP+1, h-size.cy-1, rect.Width(), h-1);
		m_rectProp.Add(pair);
	}

	int count = m_rectProp.GetSize();
	if (count > 0)
	{
		if (m_posArrow < count)
			DrawArrow(&dcmem, m_rectProp.GetAt(m_posArrow).ind);
		else
			DrawArrow(&dcmem, m_rectProp.GetAt(count-1).ind);
	}

	EraseRect(&dcmem, m_rectProp.GetAt(0).key);
	TextInRect(&dcmem, CString(_T("OID")), m_rectProp.GetAt(0).key);
	EraseRect(&dcmem, m_rectProp.GetAt(0).prop);
	TextInRect(&dcmem, oid, m_rectProp.GetAt(0).prop);

	EraseRect(&dcmem, m_rectProp.GetAt(1).key);
	TextInRect(&dcmem, CString(_T("NAME")), m_rectProp.GetAt(1).key);
	EraseRect(&dcmem, m_rectProp.GetAt(1).prop);
	TextInRect(&dcmem, node->node_name, m_rectProp.GetAt(1).prop);

	if (count > 2)
	{
		EraseRect(&dcmem, m_rectProp.GetAt(2).key);
		TextInRect(&dcmem, CString(_T("SYNTAX")), m_rectProp.GetAt(2).key);
		EraseRect(&dcmem, m_rectProp.GetAt(2).prop);
		TextInRect(&dcmem, *node->node_syntax->SYNTAX, m_rectProp.GetAt(2).prop);
	}
	if (count > 3)
	{
		EraseRect(&dcmem, m_rectProp.GetAt(3).key);
		TextInRect(&dcmem, CString(_T("ACCESS")), m_rectProp.GetAt(3).key);
		EraseRect(&dcmem, m_rectProp.GetAt(3).prop);
		TextInRect(&dcmem, *node->node_syntax->ACCESS, m_rectProp.GetAt(3).prop);
	}
	if (count > 4)
	{
		EraseRect(&dcmem, m_rectProp.GetAt(4).key);
		TextInRect(&dcmem, CString(_T("STATUS")), m_rectProp.GetAt(4).key);
		EraseRect(&dcmem, m_rectProp.GetAt(4).prop);
		TextInRect(&dcmem, *node->node_syntax->STATUS, m_rectProp.GetAt(4).prop);
	}
	if (count > 5)
	{
		EraseRect(&dcmem, m_rectProp.GetAt(5).key);
		TextInRect(&dcmem, CString(_T("DESCRIPTION")), m_rectProp.GetAt(5).key);
		EraseRect(&dcmem, m_rectProp.GetAt(5).prop);
		TextInRect(&dcmem, *node->node_syntax->DESCRIPTION, m_rectProp.GetAt(5).prop);
	}
	if (count > 6)
	{
		EraseRect(&dcmem, m_rectProp.GetAt(6).key);
		TextInRect(&dcmem, CString(_T("INDEX")), m_rectProp.GetAt(6).key);
		EraseRect(&dcmem, m_rectProp.GetAt(6).prop);
		TextInRect(&dcmem, *node->node_syntax->INDEX, m_rectProp.GetAt(6).prop);
	}

	pDC->BitBlt(0, 0, rect.Width(), rect.Height(), &dcmem, 0, 0, SRCCOPY);
	dcmem.SelectObject(pOldPen);
	dcmem.SelectObject(pOldFont);
	dcmem.SelectObject(pOldBmp);
	ReleaseDC(pDC);

	return TRUE;
}

void CMibInfoWnd::EraseRect(CDC *pDC, CRect &rect, COLORREF cr)
{
	pDC->FillSolidRect(rect, cr);
}

BOOL CMibInfoWnd::OnEraseBkgnd(CDC* pDC) 
{
	OnShowMibInfo(0, 0);
	return TRUE;
}

void CMibInfoWnd::TextInRect(CDC *pDC, CString &text, CRect &rect)
{
	CRect rc = rect;
	rc.DeflateRect(3, 0, 3, 0);
	pDC->DrawText(text, &rc, DT_LEFT|DT_BOTTOM|DT_SINGLELINE);
}

void CMibInfoWnd::DrawArrow(CDC *pDC, CRect &rect)
{
	int x = rect.right - 16;
	int y = rect.bottom - 16;
	m_listImgs.Draw(pDC, 0, CPoint(x, y), ILD_NORMAL);
}

void CMibInfoWnd::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	CPoint pos = point;
	point.x = 0;
	CRect rect;
	for (int i = 0; i < m_rectProp.GetSize(); i++)
	{
		rect = m_rectProp.GetAt(i).ind;
		if (rect.PtInRect(point))
		{
			m_posArrow = i;
			PostMessage(WM_SHOW_MIB_INFO);
			break;
		}
	}
	CWnd::OnMouseMove(nFlags, point);
}
