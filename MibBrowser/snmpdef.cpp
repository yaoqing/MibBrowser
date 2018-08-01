/*******************************************************************************
*
* Description: This file implements snmp startup.
* file name: snmpdef.cpp
* Vision: 1.0
* Date: 2002/07/01
* Author: YaoQing
*
/******************************************************************************/
#include "stdafx.h"
#include "snmpdef.h"
#include "snmp.h"
//#include "Ntd.h"
//#include "sdm.h"
//#include "host.h"
//#include "comm.h"
//#include "EdiSystem.h"
//#include <stdio.h>

//HANDLE hComm, hHost, hNtd;
//extern HWND hWnd;
//extern char *gOid[];
//extern int gPduType[];
//extern UINT gPduMsg[];
//extern DWORD dwCommThreadId, NTDmanagerThreadId, NTDFindHostThreadId;

/*******************************************************************************
*
* Function Name: FreePtr
* Description:
* Input Parameters: p
* Output Parameters:
* Notes:
*
/******************************************************************************/
void FreePtr(smiLPBER &p)
{
	if (p)
	{
		free(p);
		p = NULL;
	}
	return;
}

/*******************************************************************************
*
* Function Name: FreePdu
* Description:
* Input Parameters: pdu
* Output Parameters:
* Notes:	
*
/******************************************************************************/
void FreePdu(smiLPBER &pdu)
{
	if (pdu)
	{
		free(pdu);
		pdu = NULL;
	}
	return;
}

/*******************************************************************************
*
* Function Name: FreeVbl
* Description:
* Input Parameters: vbl
* Output Parameters:
* Notes:
*
/******************************************************************************/
void FreeVbl(smiLPBER &vbl)
{
	if (vbl)
	{
		free(vbl);
		vbl = NULL;
	}
	return;
}

/*******************************************************************************
*
* Function Name: FreeOid
* Description:
* Input Parameters:oid
* Output Parameters:
* Notes:
*
/******************************************************************************/
void FreeOid(smiLPOID &oid)
{
	if (oid)
	{
		free(oid);
		oid = NULL;
	}
	return;
}

/*******************************************************************************
*
* Function Name: FreeOctets
* Description:
* Input Parameters: oct
* Output Parameters:
* Notes:
*
/******************************************************************************/
void FreeOctets(smiLPOCTETS &oct)
{
	if (oct)
	{
		free(oct);
		oct = NULL;
	}
	return;
}

/*******************************************************************************
*
* Function Name: FreeValue
* Description:
* Input Parameters:	val
* Output Parameters:
* Notes:
*
/******************************************************************************/
void FreeValue(smiLPVALUE &val)
{
	if (val)
	{
		free(val);
		val = NULL;
	}
	return;
}

/*******************************************************************************
*
* Function Name: strtooid
* Description: 字符串变换为smiOID格式
* Input Parameters:	str
* Output Parameters: oid, return
* Notes:
*
/******************************************************************************/
SNMPAPI_STATUS strtooid(const char *str, smiLPOID &oid)
{
	char		c, buf[12];
	long		i, len, count;
	LPCCH		p, q;
	smiLPBYTE	s;
	smiUINT32	oidval[MAXOBJIDSIZE];

	oid = NULL;

	p = str;
	len = strlen(str);
	for (i = 0; i < len; i++)
	{ 
		c = *p++;
		if (((c < '0') || (c > '9')) && (c != '.'))
		{
			return SNMPAPI_FAILURE;
		}
	}

	count = 0;
	p = str;
	q = str;
	while (p = strchr(p, '.'))
	{
		for (i = 0; i < abs(p - q); i++)
			*(buf + i) = *(q + i);
		*(buf + i) = 0;
		oidval[count++] = (smiUINT32)atoi((const char *)buf);
		q = ++p;
	}

	p = str + len;
	for (i = 0; i < abs(p - q); i++)
		*(buf + i) = *(q + i);
	*(buf + i) = 0;
	oidval[count++] = (smiUINT32)atoi((const char *)buf);

	if (count == 1) oidval[count++] = 0;
	s = (smiLPBYTE)malloc(sizeof(smiUINT32) + sizeof(smiLPUINT32) + sizeof(smiUINT32) * count);
	if (s == NULL)
	{
		SysDebugInfo("strtooid() failed! Alloc memory failed!");
		return SNMPAPI_FAILURE;
	}
	oid = (smiLPOID)s;
	oid->len = count;
	oid->ptr = (smiLPUINT32)(s + sizeof(smiUINT32) + sizeof(smiLPUINT32));
	for (i = 0; i < count; i++)
	{
		*(oid->ptr + i) = oidval[i];
	}

	return SNMPAPI_SUCCESS;
}

/*******************************************************************************
*
* Function Name: oidtostr
* Description: smiOID格式变换为字符串
* Input Parameters:	oid
* Output Parameters: str
* Notes:
*
/******************************************************************************/
void oidtostr(smiLPCOID oid, char str[MAXOBJIDSTRSIZE])
{
	smiUINT32	i;
	char		buf[32];

	*str = 0;
	if (oid == NULL)return;
	for (i = 0; i < (oid->len); i++)
	{
		strcat(str, ultoa(*(oid->ptr + i), buf, 10));
		strcat(str, ".");
	}
	*(str + strlen(str) - 1) = 0;

	return;
}

/*******************************************************************************
*
* Function Name: valtostr
* Description: smiVALUE格式变换为字符串
* Input Parameters:	val
* Output Parameters: str, len
* Notes:
*
/******************************************************************************/
void valtostr(smiLPCVALUE val, char str[MAXOBJIDSTRSIZE], smiUINT32 &len, bool &null, smiUINT32 &type)
{
	*str = 0;
	len = 0;
	null = true;
	type = -1;
	if (val == NULL) return;
	null = false;
	type = val->syntax;
	switch (val->syntax)
	{
	case SNMP_SYNTAX_INT:
//	case SNMP_SYNTAX_INT32:
		sprintf(str, "%d", val->value.sNumber);
		len = strlen(str);
		break;
	case SNMP_SYNTAX_UINT32:
	case SNMP_SYNTAX_CNTR32:
	case SNMP_SYNTAX_GAUGE32:
		sprintf(str, "%u", val->value.uNumber);
		len = strlen(str);
		break;
	case SNMP_SYNTAX_TIMETICKS:
		int days, hours, minus, secs, ticks, tmpval;
		days = val->value.uNumber / (24 * 60 * 60 * 100);
		tmpval = val->value.uNumber % (24 * 60 * 60 * 100);
		hours = tmpval / (60 * 60 * 100);
		tmpval = tmpval % (60 * 60 * 100);
		minus = tmpval / (60 * 100);
		tmpval = tmpval % (60 * 100);
		secs = tmpval / 100;
		ticks = tmpval % 100;
		sprintf(str, "%02dd:%02dh:%02dm:%02ds:%02dt", days, hours, minus, secs, ticks);
		len = strlen(str);
		break;
	case SNMP_SYNTAX_SEQUENCE:
		*str = 0;
		len = strlen(str);
		break;
	case SNMP_SYNTAX_CNTR64:
		sprintf(str, "%u%u", val->value.hNumber.hipart, val->value.hNumber.lopart);
		len = strlen(str);
		break;
	case SNMP_SYNTAX_OCTETS:
	case SNMP_SYNTAX_BITS:
	case SNMP_SYNTAX_OPAQUE:
    case SNMP_SYNTAX_NSAPADDR:
		memcpy(str, val->value.string.ptr, val->value.string.len);
		str[val->value.string.len] = 0;
		len = val->value.string.len;
		break;
	case SNMP_SYNTAX_IPADDR:
		sprintf(str, "%d.%d.%d.%d", *(val->value.string.ptr), *(val->value.string.ptr + 1),
				*(val->value.string.ptr + 2), *(val->value.string.ptr + 3));
		len = strlen(str);
		break;
	case SNMP_SYNTAX_OID:
		oidtostr(&val->value.oid, str);
		len = strlen(str);
		break;
	case SNMP_SYNTAX_NULL:
	case SNMP_SYNTAX_NOSUCHOBJECT:
	case SNMP_SYNTAX_NOSUCHINSTANCE:
	case SNMP_SYNTAX_ENDOFMIBVIEW:
		*str = 0;
		len = strlen(str);
		null = true;
		break;
	default:
		*str = 0;
		len = strlen(str);
		break;
	}
	return;
}

/*******************************************************************************
*
* Function Name: SnmpStartup
* Description:
* Input Parameters:
* Output Parameters: return
* Notes:
*
/******************************************************************************/
BOOL SnmpStartup()
{
	int		ret;
	char	buf[64];
	WORD	ver;
	WSADATA	wsadata;

	ver = MAKEWORD(1, 1);
	ret = WSAStartup(ver, &wsadata);
	if (ret)
	{
		sprintf(buf, "WSAStartup() failed! Error code: %d", WSAGetLastError());
		SysDebugInfo(buf);
		WSACleanup();
		return FALSE;
	}

	if (wsadata.wVersion != ver)
	{
		sprintf(buf, "Version wrong!");
		SysDebugInfo(buf);
		WSACleanup();
		return FALSE;
	}

	/*if ((hHost = SnmpCreateSession(hWnd, WM_HOST, SnmpCallback)) == NULL)
	{
		sprintf(buf, "SnmpStartup()::SnmpCreateSession(hHost) failed!");
		SysDebugInfo(buf);
		WSACleanup();
		return FALSE;
	}

	if ((hComm = SnmpCreateSession(hWnd, WM_COMM, SnmpCallback)) == NULL)
	{
		sprintf(buf, "SnmpStartup()::SnmpCreateSession(hComm) failed!");
		SysDebugInfo(buf);
		WSACleanup();
		return FALSE;
	}

	if ((hNtd = SnmpCreateSession(hWnd, WM_NTD, SnmpCallback)) == NULL)
	{
		sprintf(buf, "SnmpStartup()::SnmpCreateSession(hNtd) failed!");
		SysDebugInfo(buf);
		WSACleanup();
		return FALSE;
	}*/

	return TRUE;
}

/*******************************************************************************
*
* Function Name: SnmpCleanup
* Description:
* Input Parameters:
* Output Parameters:
* Notes:
*
/******************************************************************************/
void SnmpCleanup()
{
//	WSACleanup();
}

/*******************************************************************************
*
* Function Name: SnmpCreateSession
* Description: Create socket for send snmpdata.
* Input Parameters:	hWnd, message, fCallback
* Output Parameters: return
* Notes:
*
/******************************************************************************/
HANDLE SnmpCreateSession(/*HWND hWnd, UINT message, SNMPAPI_CALLBACK fCallback*/)
{
	//int		ret;
	char	buf[64];
	SOCKET	SnmpSock;

	SnmpSock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (SnmpSock == INVALID_SOCKET)
	{
		sprintf(buf, "SnmpCreateSession()::socket() failed! Error code: %d", WSAGetLastError());
		SysDebugInfo(buf);
		return NULL;
	}

	/*ret = WSAAsyncSelect(SnmpSock, hWnd, message, FD_READ);
	if (ret == INVALID_SOCKET)
	{
		sprintf(buf, "SnmpCreateSession()::WSAAsyncSelect() failed! Error code: %d", WSAGetLastError());
		SysDebugInfo(buf);
		closesocket(SnmpSock);
		return NULL;
	}*/

	//SetWindowLong(hWnd, GWL_USERDATA, GetClassLong(hWnd, GCL_WNDPROC));
	//SetWindowLong(hWnd, GWL_WNDPROC, (LONG)SnmpCallback);

	return (HANDLE)SnmpSock;
}

void SnmpDestorySession(HANDLE hSnmp)
{
	closesocket((SOCKET)hSnmp);
}

/*******************************************************************************
*
* Function Name: SnmpCallback
* Description:
* Input Parameters: hWnd, message, wParam, lParam
* Output Parameters: return
* Notes:
*
/******************************************************************************/
/*LRESULT WINAPI SnmpCallback(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	LRESULT lResult = 0;

	switch (message)
	{
	case WM_COMM:
	case WM_HOST:
	case WM_NTD:
		switch (WSAGETSELECTEVENT(lParam))
		{
		case FD_READ:
			SnmpReceive(message, wParam, lParam);
			break;
		default:
			break;
		}
		break;
	case WM_TIMER:
		if (wParam == WM_PING_TIMER)
			PostThreadMessage(NTDFindHostThreadId, WM_PING_TIMER, 0, 0);
		if (wParam == WM_SNMPGETNEXT_TIMEOUT)
			PostThreadMessage(NTDmanagerThreadId, WM_SNMPGETNEXT_TIMEOUT, 0, 0);
		break;
	case WM_ACCEPT:
		CommAccept(wParam, lParam);
		break;
	case WM_READWRITE:
		switch (WSAGETSELECTEVENT(lParam))
		{
		case FD_READ:
			PostThreadMessage(dwCommThreadId, WM_REQURESTQUERY, wParam, lParam);
			break;
		case FD_CLOSE:
			CommCleanRow(wParam);
			break;
		default:
			break;
		}
		break;
	default:
		lResult = CallWindowProc((WNDPROC)GetWindowLong(hWnd, GWL_USERDATA), hWnd, message, wParam, lParam);
	}

	return lResult;
}*/

/*******************************************************************************
*
* Function Name: SnmpReceive
* Description:
* Input Parameters: message, wParam, lParam
* Output Parameters:
* Notes:
*
/******************************************************************************/
/*void SnmpReceive(UINT message, WPARAM wParam, LPARAM lParam)
{
	char		ip[16], *buf;
	char		stroid[MAXOBJIDSTRSIZE];
	char		strval[MAXOBJIDSTRSIZE];
	snmp		snmpdata;
	smiUINT32	i, j, count, vallen;
	smiLPOID	oid;
	smiLPVALUE	val;

	snmpdata.SetHandle((HANDLE)wParam);
	if (snmpdata.RecvMsg() == SNMPAPI_FAILURE)
	{
		if (WSAGetLastError() == WSAECONNRESET)
			return;
		SysDebugInfo("RecvMsg() failed!");
		return;
	}
	if (snmpdata.GetPduData() == SNMPAPI_FAILURE)
	{
		SysDebugInfo("GetPduData() failed!");
		return;
	}
	if ((count = snmpdata.CountVbl()) == SNMPAPI_FAILURE)
	{
		SysDebugInfo("CountVbl() failed!");
		return;
	}
	if (snmpdata.GetSrcIp(ip) == SNMPAPI_FAILURE)
	{
		SysDebugInfo("GetSrcIp() failed!");
		return;
	}
	for (i = 1; i < count; i++)
	{
		if (snmpdata.GetVb(i, oid, val) == SNMPAPI_FAILURE)
		{
			SysDebugInfo("GetVb() failed!");
			return;
		}
		oidtostr(oid, stroid);
		valtostr(val, strval, vallen);
		switch(message)
		{
		case WM_HOST:
			for (j = OID_START; j <= OID_END; j++)
			{
				if (strstr(stroid, *(gOid + j)))
					break;
			}
			if (snmpdata.GetErrorStatus() == SNMP_ERROR_NOERROR)
				WriteToHostDB(gPduMsg[j], strval, vallen, ip);
			break;
		case WM_COMM:
			j = strlen(strval);
			if (j != vallen)
			{
				buf = (char *)malloc(vallen * 3 + vallen + 2);
				if (buf == NULL)
				{
					SysDebugInfo("SnmpReceive() failed! Alloc memory failed!");
					break;
				}
				*buf = '<';
				for (j = 0; j < vallen; j++)
					sprintf(buf + 1 + j * 3, "%X%X.", (BYTE)*(strval + j) >> 4 , (BYTE)*(strval + j) & 0x0F);
				*(buf + 1 + (j - 1) * 3 + 2) = '>';
				memcpy(buf + 1 + j * 3, strval, vallen);
				CommReturnMsg(ip, TRUE, snmpdata.GetErrorStatus(), stroid, val? val->syntax: SNMP_SYNTAX_NULL, buf);
				free(buf);
			}
			else
				CommReturnMsg(ip, TRUE, snmpdata.GetErrorStatus(), stroid, val? val->syntax: SNMP_SYNTAX_NULL, strval);
			break;
		case WM_NTD:
			NTDGetRouteInfo(stroid, strval);
			break;
		}
		FreeOid(oid);
		FreeValue(val);
	}

	return;
}*/

void SysDebugInfo(const char *info)
{
}

/*******************************************************************************
*
* Function Name: oidatindex
* Description:
* Input Parameters: oid, index
* Output Parameters: return
* Notes:
*
/******************************************************************************/
smiUINT32 oidatindex(smiLPCOID oid, int index)
{
	if (!oid) return 0;
	if (index < 0) index = oid->len+index;
	if (index < 0) index = 0;
	if ((smiUINT32)index >= oid->len) index = oid->len-1;
	return oid->ptr[index];
}