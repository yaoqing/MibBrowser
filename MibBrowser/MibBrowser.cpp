// MibBrowser.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "MibBrowser.h"
#include "MacXFrame.h"
#include "ChildFrame.h"
#include "LangLoader.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMibBrowserApp

BEGIN_MESSAGE_MAP(CMibBrowserApp, CWinApp)
	//{{AFX_MSG_MAP(CMibBrowserApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMibBrowserApp construction

CMibBrowserApp::CMibBrowserApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
	pWndMainFrame = 0;
	pWndChildFrame = 0;
}

CMibBrowserApp::~CMibBrowserApp()
{
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CMibBrowserApp object

CMibBrowserApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CMibBrowserApp initialization

BOOL CMibBrowserApp::InitInstance()
{
	AfxInitRichEdit();

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.

#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif

	// Change the registry key under which our settings are stored.
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization.
	// SetRegistryKey(_T("Local AppWizard-Generated Applications"));


	// To create the main window, this code creates a new frame window
	// object and then sets it as the application's main window object.

	SnmpStartup();
	g_strRes = CLangLoader::GetInstance();
	
	pWndMainFrame = new CMacXFrame();
	pWndChildFrame = new CChildFrame();
	pWndMainFrame->SetChildFrame(pWndChildFrame);
	m_pMainWnd = pWndMainFrame;

	// create and load the frame with its resources
	int cx = GetSystemMetrics(SM_CXSCREEN);
	int cy = GetSystemMetrics(SM_CYSCREEN);
	int width = GetSystemMetrics(SM_CXSCREEN)*2/3;
	int heigth = GetSystemMetrics(SM_CYSCREEN)*2/3;
	int x = (cx-width)/2;
	int y = (cy-heigth)/2;
	CString szName;
	szName.LoadString(IDR_MAINFRAME);
	pWndMainFrame->Create(NULL, szName, NULL, CRect(x, y, x+width, y+heigth), NULL, NULL);
	
	HICON hIcon = LoadIcon(IDR_MAINFRAME);
	pWndMainFrame->SetIcon(hIcon, TRUE);
	pWndMainFrame->SetIcon(hIcon, FALSE);
	
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CMibBrowserApp message handlers

int CMibBrowserApp::ExitInstance() 
{
	// TODO: Add your specialized code here and/or call the base class
	SnmpCleanup();
	delete g_strRes;
	return CWinApp::ExitInstance();
}
