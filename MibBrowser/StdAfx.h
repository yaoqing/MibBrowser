// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__DBE1E9C7_BB7B_4051_98A5_9E46E5E594EB__INCLUDED_)
#define AFX_STDAFX_H__DBE1E9C7_BB7B_4051_98A5_9E46E5E594EB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers
#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions
#include <afxdisp.h>        // MFC Automation classes
#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Common Controls
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT
#include <afxsock.h>
#include <afxtempl.h>

#include "snmpdef.h"
#include "snmp.h"

class CMemoWnd;
class CMibNode;
class CChildView;
class CMacXFrame;
class CLangLoader;
extern volatile BOOL g_bIsRegist;
extern UINT WM_INIT_PAGE;
extern UINT WM_SHOW_MIB_INFO;
extern UINT WM_ADJUST_SPLITTER;
extern UINT WM_MEMO_ADD_LINE;
extern UINT WM_MODAL_FINISHED;
extern UINT WM_CAPTURED;

extern CMemoWnd * g_wndMemo;
extern CMibNode * g_mibRoot;
extern CChildView * g_wndView;
extern CLangLoader * g_strRes;

typedef struct
{
	int seq;
	char *oid;
	char *val;
	char *type;
}OID_QUERY_INFO;

typedef struct
{
	char *line;
	COLORREF color;
}MEMO_TEXT_INFO;

typedef enum {SET_TEXT=1, OID_VAL}MEMO_TYPE;

SNMPAPI_STATUS GetSNMPVal(const char *sIp, int ver, const char *sOid, const char *sCommunity, int port, int timeout, int pdu_type, char *&byoid, char *&byval, bool &null, DWORD &valtype, SOCKET so = 0);
CString LoadAsString(CString key, CString rval);
int LoadAsInteger(CString key, int rval);
int LoadAsRGB(CString key, CString rval);
LPARAM MakeMemoTextInfo(char *line, COLORREF color);
LPARAM MakeOidQueryInfo(int seq, char *oid, char *val, smiUINT32 type);
HWND MsgBoxModal(CString msg, CWnd *wnd = NULL, CRect *rect = NULL, CString *caption = NULL);

AFX_INLINE UINT AFXAPI HashKey(CString &key)
{
	return HashKey((LPCTSTR)key);
}

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__DBE1E9C7_BB7B_4051_98A5_9E46E5E594EB__INCLUDED_)
