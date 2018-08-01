// MemoWnd.cpp : implementation file
//

#include "stdafx.h"
#include "mibbrowser.h"
#include "MemoWnd.h"
#include "MenuEx.h"
#include "MibLoader.h"
#include "StringEx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNCREATE(CMemoWnd, CWnd)
/////////////////////////////////////////////////////////////////////////////
// CMemoWnd

CMemoWnd::CMemoWnd()
{
	m_nLineNum = 0;
	m_nLineHeight = 0;
	m_nLineMaxLen = 0;
	first = 1;
	caret.x = caret.y = 0;
}

CMemoWnd::~CMemoWnd()
{
}


BEGIN_MESSAGE_MAP(CMemoWnd, CWnd)
	//{{AFX_MSG_MAP(CMemoWnd)
	ON_WM_KILLFOCUS()
	ON_WM_SETFOCUS()
	ON_WM_LBUTTONDOWN()
	ON_WM_VSCROLL()
	ON_WM_CREATE()
	ON_WM_PAINT()
	ON_WM_NCCALCSIZE()
	ON_WM_MOUSEWHEEL()
	ON_WM_HSCROLL()
	ON_WM_SIZE()
	ON_WM_RBUTTONDOWN()
	ON_REGISTERED_MESSAGE(WM_MEMO_ADD_LINE, OnAddLine)
	ON_WM_KEYDOWN()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMemoWnd message handlers

BOOL CMemoWnd::PreCreateWindow(CREATESTRUCT& cs) 
{
	// TODO: Add your specialized code here and/or call the base class
	cs.style |= WS_CHILD | WS_VISIBLE | WS_VSCROLL | WS_HSCROLL | WS_VSCROLL;
	cs.lpszClass = AfxRegisterWndClass(CS_HREDRAW|CS_VREDRAW|CS_DBLCLKS,
		::LoadCursor(NULL, IDC_IBEAM), (HBRUSH)0, NULL);

	return CWnd::PreCreateWindow(cs);
}

void CMemoWnd::OnKillFocus(CWnd* pNewWnd) 
{
	CWnd::OnKillFocus(pNewWnd);
	
	// TODO: Add your message handler code here
	DestroyCaret();
	first = 1;
}

void CMemoWnd::OnSetFocus(CWnd* pOldWnd) 
{
	CWnd::OnSetFocus(pOldWnd);
	
	// TODO: Add your message handler code here
	CPaintDC dc(this);
	CFont font;
	font.CreatePointFont(LoadAsInteger(_T("memo.font.size"), 90), LoadAsString(_T("memo.font.name"), _T("Arial Bold")));
	CFont *pOldFont = dc.SelectObject(&font);
	CSize size = dc.GetTextExtent(_T(" "));
	dc.SelectObject(pOldFont);
	m_nLineHeight = size.cy+2;
	::CreateCaret(GetSafeHwnd(), (HBITMAP)NULL, 1, size.cy);
}

void CMemoWnd::OnLButtonDown(UINT nFlags, CPoint point) 
{
	CWnd::OnLButtonDown(nFlags, point);
	// TODO: Add your message handler code here and/or call default
	if (first)
	{
		SetFocus();
		first = 0;
	}

	if (m_szMsgs.GetSize() <= 0)
	{
		m_nLineNum = 0;
		caret.x = caret.y = 0;
		point.x = point.y = 0;
		SetCaretPos(point);
		return;
	}

	CString line;
	CRect rect;
	GetClientRect(rect);
	int n = point.y/m_nLineHeight;
	int pos = GetScrollPos(SB_VERT);
	if (pos+n < m_szMsgs.GetSize())
	{
		line = m_szMsgs.GetAt(pos+n);
		point.y = n * m_nLineHeight;
		m_nLineNum = pos+n;
	}
	else
	{
		m_nLineNum = n = m_szMsgs.GetSize()-1;
		line = m_szMsgs.GetAt(n);
		point.y = (n-pos) * m_nLineHeight;
	}

	CPaintDC dc(this);
	CFont font;
	font.CreatePointFont(LoadAsInteger(_T("memo.font.size"), 90), LoadAsString(_T("memo.font.name"), _T("Arial Bold")));
	CFont *pOldFont = dc.SelectObject(&font);

	pos = GetScrollPos(SB_HORZ);
	CSize size;
	int x = -pos, pre = -pos;
	for (int i = 0; i < line.GetLength(); i++)
	{
		pre = x;
		size = dc.GetTextExtent(CString(line.GetAt(i)));
		x += size.cx;
		if (x > point.x)
		{
			if (point.x - pre < x - point.x)
				x = pre;
			break;
		}
	}
	point.x = x;	
	caret = point;
	caret.x += pos;
	SetCaretPos(point);
	dc.SelectObject(&pOldFont);
}

LRESULT CMemoWnd::OnAddLine(WPARAM wParam, LPARAM lParam)
{
	CPaintDC dc(this);
	MEMO_TYPE type = (MEMO_TYPE)wParam;
	OID_QUERY_INFO *oid_result = NULL;
	MEMO_TEXT_INFO *text_result = NULL;
	
	int pos;
	CString line;
	CMibNode *node;
	switch (type)
	{
	case SET_TEXT:
		text_result = (MEMO_TEXT_INFO *)lParam;
		line += text_result->line;
		m_szMsgs.Add(line);
		m_nTypes.Add(SET_TEXT);
		m_nColors.Add(text_result->color);
		delete [] text_result->line;
		delete text_result;
		break;
	case OID_VAL:
		oid_result = (OID_QUERY_INFO *)lParam;
		node = g_mibRoot->FindChild(oid_result->oid, pos);
		line.Format(_T("%u: "), oid_result->seq);
		line += node ? node->node_name+CString(oid_result->oid+pos) : CString(oid_result->oid, pos);
		line += oid_result->type;
		line += oid_result->val;
		m_szMsgs.Add(line);
		m_nTypes.Add(OID_VAL);
		m_nColors.Add(RGB(0, 0, 0));
		free(oid_result->oid);
		free(oid_result->val);
		delete [] oid_result->type;
		delete oid_result;
		break;
	case -1:
		line = _T(" ");
		break;
	default:
		line = _T(" ");
		m_szMsgs.Add(CString());
		m_nTypes.Add(SET_TEXT);
		m_nColors.Add(RGB(0, 0, 0));
	}

	CFont font;
	font.CreatePointFont(LoadAsInteger(_T("memo.font.size"), 90), LoadAsString(_T("memo.font.name"), _T("Arial Bold")));
	CFont *pOldFont = dc.SelectObject(&font);
	CSize size = dc.GetTextExtent(line);
	dc.SelectObject(pOldFont);

	CRect rect;
	GetClientRect(rect);
	if (m_nLineMaxLen < size.cx)
		m_nLineMaxLen = size.cx;
	m_nLineHeight = size.cy+2;

	int height = m_szMsgs.GetSize() * m_nLineHeight;
	SCROLLINFO info;
	if (height <= rect.Height() || rect.IsRectEmpty())
	{
		memset(&info, 0, sizeof(info));
		info.cbSize = sizeof(SCROLLINFO);
		info.fMask = SIF_ALL;
		SetScrollInfo(SB_VERT, &info);
	}
	else
	{
		info.cbSize = sizeof(SCROLLINFO);
		info.nMin = 0;
		info.nMax = m_szMsgs.GetSize()-1;
		info.nPage = rect.Height()/m_nLineHeight;
		info.nPos = info.nMax;
		info.nTrackPos = info.nMax;
		info.fMask = SIF_ALL;
		SetScrollInfo(SB_VERT, &info);
	}

	if (m_nLineMaxLen <= rect.Width() || rect.IsRectEmpty())
	{
		memset(&info, 0, sizeof(info));
		info.cbSize = sizeof(SCROLLINFO);
		info.fMask = SIF_ALL;
		SetScrollInfo(SB_HORZ, &info);
	}
	else
	{
		info.cbSize = sizeof(SCROLLINFO);
		info.nMin = 0;
		info.nMax = m_nLineMaxLen-1;
		info.nPage = rect.Width()/3;
		info.nPos = GetScrollPos(SB_HORZ);
		info.nTrackPos = info.nMax;
		info.fMask = SIF_ALL;
		SetScrollInfo(SB_HORZ, &info);
	}

	CaretFollow();
	Invalidate();
	return TRUE;
}

void CMemoWnd::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	// TODO: Add your message handler code here and/or call default
	CWnd::OnVScroll(nSBCode, nPos, pScrollBar);

	int iNowPos = 0;
	SCROLLINFO info;
	switch(nSBCode)
	{
	case SB_LINEDOWN:
		GetScrollInfo(SB_VERT, &info);
		iNowPos = info.nPos+1;
		SetScrollPos(SB_VERT, iNowPos);
		break;
	case SB_PAGEDOWN:
		GetScrollInfo(SB_VERT, &info);
		iNowPos=info.nPos+info.nPage;
		SetScrollPos(SB_VERT, iNowPos);
		break;
	case SB_LINEUP:
		GetScrollInfo(SB_VERT, &info);
		iNowPos = info.nPos-1;
		SetScrollPos(SB_VERT, iNowPos);
		break;
	case SB_PAGEUP:
		GetScrollInfo(SB_VERT, &info);
		iNowPos=info.nPos-info.nPage;
		SetScrollPos(SB_VERT, iNowPos);
		break;
	}

	CaretFollow();
	Invalidate();
}

int CMemoWnd::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here
	CStringEx sss(LoadAsString(_T("memo.mib.welcome.text"), _T(" Welcome to SNMP MibBrowse !")));
	return PostMessage(WM_MEMO_ADD_LINE, SET_TEXT, MakeMemoTextInfo(sss.toChar(), LoadAsRGB(_T("memo.mib.welcome.color"), _T("0,0,128"))));
}

void CMemoWnd::OnPaint() 
{
	CRect rect;
	GetClientRect(rect);
	CRect save = rect;
	CPaintDC dc(this);

	CDC dcmem;
	dcmem.CreateCompatibleDC(&dc);
	CBitmap img;
	img.CreateCompatibleBitmap(&dc, rect.Width(), rect.Height());
	CBitmap *pOldBitmap = dcmem.SelectObject(&img);
	dcmem.FillSolidRect(rect, RGB(255, 255, 255));
	CFont font;
	font.CreatePointFont(LoadAsInteger(_T("memo.font.size"), 90), LoadAsString(_T("memo.font.name"), _T("Arial Bold")));
	CFont *pOldFont = dcmem.SelectObject(&font);

	rect.left -= GetScrollPos(SB_HORZ);
	int height = m_szMsgs.GetSize() * m_nLineHeight;
	if (height < rect.Height())
	{
		for (int i = 0; i < m_szMsgs.GetSize(); i++)
		{
			rect.top = i * m_nLineHeight;
			rect.bottom = rect.top + m_nLineHeight;
			DrawLine(&dcmem, i, rect);
			//dcmem.DrawText(line, rect, DT_LEFT|DT_BOTTOM|DT_SINGLELINE);
		}
	}
	else
	{
		int n = rect.Height()/m_nLineHeight+1;
		int pos = GetScrollPos(SB_VERT);
		for (int i = pos; i < m_szMsgs.GetSize() && i < pos+n; i++)
		{
			rect.top = (i-pos) * m_nLineHeight;
			rect.bottom = rect.top + m_nLineHeight;
			DrawLine(&dcmem, i, rect);
			//dcmem.DrawText(line, rect, DT_LEFT|DT_BOTTOM|DT_SINGLELINE);
		}
	}

	dc.BitBlt(0, 0, save.Width(), save.Height(), &dcmem, 0, 0, SRCCOPY);
	dcmem.SelectObject(pOldFont);
	dcmem.SelectObject(pOldBitmap);
}

void CMemoWnd::OnNcCalcSize(BOOL bCalcValidRects, NCCALCSIZE_PARAMS FAR* lpncsp) 
{
	// TODO: Add your message handler code here and/or call default
	CRect rect = lpncsp->rgrc[0];
	rect.DeflateRect(2, 2, 2, 2);
	lpncsp->rgrc[0] = rect;

	CWnd::OnNcCalcSize(bCalcValidRects, lpncsp);
}

void CMemoWnd::CaretFollow()
{
	if (m_szMsgs.GetSize() <= 0)
		caret.x=caret.y=m_nLineNum=0;

	int pos = GetScrollPos(SB_VERT);
	CPoint point;
	point.x = caret.x;
	point.y = (m_nLineNum-pos) * m_nLineHeight;
	point.x -= GetScrollPos(SB_HORZ);

	SetCaretPos(point);
}

BOOL CMemoWnd::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt) 
{
	// TODO: Add your message handler code here and/or call default
	if (zDelta > 0)
		PostMessage(WM_VSCROLL, MAKEWPARAM(0, 0), 0);
	else
		PostMessage(WM_VSCROLL, MAKEWPARAM(1, 0), 0);
	
	return CWnd::OnMouseWheel(nFlags, zDelta, pt);
}

void CMemoWnd::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	// TODO: Add your message handler code here and/or call default
	CWnd::OnHScroll(nSBCode, nPos, pScrollBar);

	int iNowPos = 0;
	SCROLLINFO info;
	switch(nSBCode)
	{
	case SB_LINERIGHT:
		GetScrollInfo(SB_HORZ, &info);
		iNowPos = info.nPos+8;
		SetScrollPos(SB_HORZ, iNowPos);
		break;
	case SB_PAGERIGHT:
		GetScrollInfo(SB_HORZ, &info);
		iNowPos=info.nPos+info.nPage;
		SetScrollPos(SB_HORZ, iNowPos);
		break;
	case SB_LINELEFT:
		GetScrollInfo(SB_HORZ, &info);
		iNowPos = info.nPos-8;
		SetScrollPos(SB_HORZ, iNowPos);
		break;
	case SB_PAGELEFT:
		GetScrollInfo(SB_HORZ, &info);
		iNowPos=info.nPos-info.nPage;
		SetScrollPos(SB_HORZ, iNowPos);
		break;
	}

	CaretFollow();
	Invalidate();
}

void CMemoWnd::DrawLine(CDC *pDC, int index, CRect rect)
{
	CString &line = m_szMsgs.GetAt(index);
	MEMO_TYPE type = (MEMO_TYPE)m_nTypes.GetAt(index);

	COLORREF seq_color = LoadAsRGB(_T("memo.mib.seq.color"), _T("8,0,0"));
	COLORREF oid_color = LoadAsRGB(_T("memo.mib.oid.color"), _T("8,0,0"));
	COLORREF val_color = LoadAsRGB(_T("memo.mib.val.color"), _T("0,0,128"));
	COLORREF type_color = LoadAsRGB(_T("memo.mib.type.color"), _T("0,128,0"));
	COLORREF old_color = RGB(0, 0, 0);

	switch (type)
	{
	case SET_TEXT:
		old_color = pDC->SetTextColor(m_nColors.GetAt(index));
		pDC->DrawText(line, rect, DT_LEFT|DT_BOTTOM|DT_SINGLELINE);
		pDC->SetTextColor(old_color);
		break;
	case OID_VAL:
		old_color = pDC->SetTextColor(seq_color);
		int pos1 = line.Find(_T(": "));
		CString sub = line.Left(pos1+2);
		CSize size = pDC->GetTextExtent(sub);
		CRect subrect = rect;
		subrect.right = subrect.left+size.cx;
		pDC->DrawText(sub, subrect, DT_LEFT|DT_BOTTOM|DT_SINGLELINE);

		pDC->SetTextColor(oid_color);
		int pos2 = line.Find(_T(" ("), pos1+2);
		sub = line.Mid(pos1+2, pos2-pos1-2);
		size = pDC->GetTextExtent(sub);
		subrect.left = subrect.right;
		subrect.right += size.cx;
		pDC->DrawText(sub, subrect, DT_LEFT|DT_BOTTOM|DT_SINGLELINE);
		
		pos1 = pos2;
		pDC->SetTextColor(type_color);
		pos2 = line.Find(_T(") "), pos1);
		sub = line.Mid(pos1, pos2-pos1+2);
		size = pDC->GetTextExtent(sub);
		subrect.left = subrect.right;
		subrect.right += size.cx;
		pDC->DrawText(sub, subrect, DT_LEFT|DT_BOTTOM|DT_SINGLELINE);

		pDC->SetTextColor(val_color);
		sub = line.Right(line.GetLength()-pos2-2);
		size = pDC->GetTextExtent(sub);
		subrect.left = subrect.right;
		subrect.right += size.cx;
		pDC->DrawText(sub, subrect, DT_LEFT|DT_BOTTOM|DT_SINGLELINE);
		pDC->SetTextColor(old_color);

		size = pDC->GetTextExtent(line);
		break;
	}
}

void CMemoWnd::OnSize(UINT nType, int cx, int cy) 
{
	CWnd::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	PostMessage(WM_MEMO_ADD_LINE, -1);
}

void CMemoWnd::OnRButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default	
	CWnd::OnRButtonDown(nFlags, point);

	CMenuEx menu;
	ClientToScreen(&point);
	menu.CreatePopupMenu();
	menu.InsertMenu(0, MF_BYPOSITION|MF_STRING, 0, LoadAsString(_T("memo.menu.clear.text"), _T("&Clear")));
	menu.TrackPopupMenu(TPM_LEFTBUTTON|TPM_RIGHTBUTTON, point.x, point.y, this);
}

BOOL CMemoWnd::OnCommand(WPARAM wParam, LPARAM lParam) 
{
	// TODO: Add your specialized code here and/or call the base class
	switch (wParam)
	{
	case 0:
		m_nLineMaxLen = 0;
		m_szMsgs.RemoveAll();
		m_nColors.RemoveAll();
		m_nTypes.RemoveAll();
		PostMessage(WM_MEMO_ADD_LINE, -1);
		break;
	}
	
	return CWnd::OnCommand(wParam, lParam);
}

void CMemoWnd::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	// TODO: Add your message handler code here and/or call default
	CWnd::OnKeyDown(nChar, nRepCnt, nFlags);

	switch (nChar)
	{
	case 38:
		PostMessage(WM_VSCROLL, MAKEWPARAM(0, 0), 0);
		break;
	case 40:
		PostMessage(WM_VSCROLL, MAKEWPARAM(1, 0), 0);
		break;
	}
}
