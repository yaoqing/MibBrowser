// stdafx.cpp : source file that includes just the standard includes
//	MibBrowser.pch will be the pre-compiled header
//	stdafx.obj will contain the pre-compiled type information

#include "stdafx.h"
#include "StringEx.h"
#include "StringToken.h"
#include "LangLoader.h"
#include "MessageWnd.h"
#include "MacXFrame.h"

volatile BOOL g_bIsRegist = FALSE;
CMemoWnd * g_wndMemo = 0;
CMibNode * g_mibRoot = 0;
CChildView * g_wndView = 0;
CLangLoader * g_strRes = 0;
UINT WM_INIT_PAGE = RegisterWindowMessage(_T("WM_INIT_PAGE"));
UINT WM_SHOW_MIB_INFO = RegisterWindowMessage(_T("WM_SHOW_MIB_INFO"));
UINT WM_ADJUST_SPLITTER = RegisterWindowMessage(_T("WM_ADJUST_SPLITTER"));
UINT WM_MEMO_ADD_LINE = RegisterWindowMessage(_T("WM_MEMO_ADD_LINE"));
UINT WM_MODAL_FINISHED = RegisterWindowMessage(_T("WM_MODAL_FINISHED"));
UINT WM_CAPTURED = RegisterWindowMessage(_T("WM_CAPTURED"));

SNMPAPI_STATUS GetSNMPVal(const char *sIp, int ver, const char *sOid, const char *sCommunity, int port, int timeout, int pdu_type, char *&byoid, char *&byval, bool &null, DWORD &valtype, SOCKET so)
{
	null = true;
	byoid = byval = 0;
	valtype = 0xFFFFFFFF;
	smiLPOID oid;
	smiLPVALUE val;
	smiUINT32 vallen;
	smiOCTETS oct;
	snmp data;
	HANDLE hSnmp = (so <= 0 )? ::SnmpCreateSession() : (HANDLE)so;
	
	SNMPAPI_STATUS count;
	char stroid[MAXOBJIDSTRSIZE], strval[MAXOBJIDSTRSIZE];
	*stroid = *strval = 0;
	
	strcpy(stroid, sOid);
	strtooid(stroid, oid);
	oct.len = strlen(sCommunity);
	oct.ptr = (smiLPBYTE)sCommunity;
	data.SetHandle(hSnmp);
	data.SetTimeout(timeout);
	
	data.CreateVbl(oid, NULL);
	data.CreatePdu(pdu_type, 0, 0, 0);
	if (data.SendMsg(sIp, ver, port, &oct) == SNMPAPI_FAILURE)
	{
		FreeOid(oid);
		SnmpDestorySession(hSnmp);
		return SNMPAPI_FAILURE;
	}
	FreeOid(oid);
	
	if (data.RecvMsg() == SNMPAPI_FAILURE) 
	{
		SnmpDestorySession(hSnmp);
		return SNMPAPI_FAILURE;
	}
	if (data.GetPduData() == SNMPAPI_FAILURE)
	{
		SnmpDestorySession(hSnmp);
		return SNMPAPI_FAILURE;
	}
	count = data.CountVbl();
	if (count == SNMPAPI_FAILURE)
	{
		SnmpDestorySession(hSnmp);
		return SNMPAPI_FAILURE;
	}
	for (unsigned int i = 0; i < count && i >= 0; i++)
	{
		data.GetVb(i, oid, val);
		oidtostr(oid, stroid);
		valtostr(val, strval, vallen, null, valtype);
		FreeOid(oid);
		FreeValue(val);
		break;
	}
	if (*stroid == 0)
	{
		byoid = 0;
		byval = 0;
	}
	else
	{
		byoid = (char*)malloc(strlen(stroid) + 1);
		strcpy(byoid, stroid);
		unsigned int len = strlen(strval);
		if (len != vallen && len+1 != vallen)
		{
			byval = (char*)malloc(vallen*4+1);
			byval[0] = '<';
			for (unsigned int i = 0; i < vallen; i++)
				sprintf(byval+i*3+1, "%02X ", (unsigned char)strval[i]);
			byval[vallen*3] = '>';
			memcpy(byval+vallen*3+1, strval, vallen);
		}
		else
		{
			byval = (char*)malloc(len + 1);
			strcpy(byval, strval);
		}
	}
	
	SnmpDestorySession(hSnmp);
	return SNMPAPI_SUCCESS;
}

CString LoadAsString(CString key, CString rval)
{
	CString val = g_strRes->LoadString(key);
	return val.GetLength() > 0 ? val: rval;
}

int LoadAsInteger(CString key, int rval)
{
	CString val = g_strRes->LoadString(key);
	return val.GetLength() > 0 ? _ttoi(val): rval;
}

int LoadAsRGB(CString key, CString rval)
{
	CString val = g_strRes->LoadString(key);
	if (val.GetLength() == 0) val = rval;

	CStringToken token(val, ',');
	int count = token.CountToke();
	int red, green, blue;
	red = green = blue = 0;
	if (count > 0)
		red = _ttoi(token.GetAt(0));
	if (count > 1)
		green = _ttoi(token.GetAt(1));
	if (count > 2)
		blue = _ttoi(token.GetAt(2));
	return RGB(red, green, blue);
}

LPARAM MakeMemoTextInfo(char *line, COLORREF color)
{
	MEMO_TEXT_INFO *result = new MEMO_TEXT_INFO();
	result->line = line;
	result->color = color;
	return (LPARAM)result;
}

LPARAM MakeOidQueryInfo(int seq, char *oid, char *val, smiUINT32 type)
{
	CStringEx sss;
	OID_QUERY_INFO *result = new OID_QUERY_INFO();
	result->seq = seq;
	result->oid = oid;
	result->val = val;

	switch (type)
	{
	case SNMP_SYNTAX_INT:
//	case SNMP_SYNTAX_INT32:
		sss = " (int32) ";
		break;
	case SNMP_SYNTAX_UINT32:
		sss = " (uint32) ";
		break;
	case SNMP_SYNTAX_CNTR32:
		sss = " (counter32) ";
		break;
	case SNMP_SYNTAX_GAUGE32:
		sss = " (gauge32) ";
		break;
	case SNMP_SYNTAX_TIMETICKS:
		sss = " (timeticks) ";
		break;
	case SNMP_SYNTAX_SEQUENCE:
		sss = " (sequence) ";
		break;
	case SNMP_SYNTAX_CNTR64:
		sss = " (counter64) ";
		break;
	case SNMP_SYNTAX_OCTETS:
		sss = " (octet string) ";
		break;
	case SNMP_SYNTAX_BITS:
		sss = " (bits) ";
		break;
	case SNMP_SYNTAX_OPAQUE:
		sss = " (opaque) ";
		break;
    case SNMP_SYNTAX_NSAPADDR:
		sss = " (nsapaddr) ";
		break;
	case SNMP_SYNTAX_IPADDR:
		sss = " (ip addr) ";
		break;
	case SNMP_SYNTAX_OID:
		sss = " (object identifier) ";
		break;
	case SNMP_SYNTAX_NULL:
		sss = " (null) ";
		break;
	case SNMP_SYNTAX_NOSUCHOBJECT:
		sss = " (no such object) ";
		break;
	case SNMP_SYNTAX_NOSUCHINSTANCE:
		sss = " (no such instance) ";
		break;
	case SNMP_SYNTAX_ENDOFMIBVIEW:
		sss = " (end of mib view) ";
		break;
	default:
		sss = " (unknown) ";
		break;
	}

	result->type = sss.toChar();
	return (LPARAM)result;
}

HWND MsgBoxModal(CString msg, CWnd *wnd, CRect *rect, CString *caption)
{
	CRect rectBox;
	if (rect)
		rectBox = rect;
	else
		rectBox.SetRect(0, 0, 240, 120);
	if (!wnd)
		wnd = AfxGetMainWnd();

	CMacXFrame *dlg = new CMacXFrame();
	CMessageWnd *pWnd = new CMessageWnd(msg);
	dlg->SetChildFrame(pWnd);
	return dlg->CreateModal(NULL, caption ? *caption: LoadAsString(_T("msgbox.caption.text"), _T("Maybe exception")), 0, rectBox, wnd, 0, NULL, wnd);
}