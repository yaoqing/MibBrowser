// MibBrowser.h : main header file for the MIBBROWSER application
//

#if !defined(AFX_MIBBROWSER_H__9A0C2C63_65AF_47BF_956C_378B96C250B3__INCLUDED_)
#define AFX_MIBBROWSER_H__9A0C2C63_65AF_47BF_956C_378B96C250B3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CMibBrowserApp:
// See MibBrowser.cpp for the implementation of this class
//
class CMacXFrame;
class CChildFrame;
class CMibBrowserApp : public CWinApp
{
public:
	CMibBrowserApp();
	~CMibBrowserApp();

protected:
	CMacXFrame *pWndMainFrame;
	CChildFrame *pWndChildFrame;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMibBrowserApp)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

// Implementation

public:
	//{{AFX_MSG(CMibBrowserApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MIBBROWSER_H__9A0C2C63_65AF_47BF_956C_378B96C250B3__INCLUDED_)
