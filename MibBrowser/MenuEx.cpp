// MenuEx.cpp : implementation file
//

#include "stdafx.h"
#include "mibbrowser.h"
#include "MenuEx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMenuEx

CMenuEx::CMenuEx()
{
}

CMenuEx::~CMenuEx()
{
}

/////////////////////////////////////////////////////////////////////////////
// CMenuEx message handlers

void CMenuEx::EnableOwnerDraw(HMENU hMenu)
{
	if (hMenu)
	{
		int size = ::GetMenuItemCount(hMenu);
		for (int i = 0; i < size; i++)
		{
			hMenu = ::GetSubMenu(hMenu, i);
			::ModifyMenu(hMenu, i, MF_BYPOSITION|MF_OWNERDRAW, 0, NULL);
			if (!hMenu) continue;
			EnableOwnerDraw(hMenu);
		}				
	}
	else
	{
		hMenu = GetSafeHmenu();
		if (hMenu) EnableOwnerDraw(hMenu);
	}
}

void CMenuEx::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	CMenu::DrawItem(lpDrawItemStruct);
}

void CMenuEx::MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct)
{
	CMenu::MeasureItem(lpMeasureItemStruct);
}