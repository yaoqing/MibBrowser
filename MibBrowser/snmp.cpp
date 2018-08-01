/*******************************************************************************
*
* Description:			This file defines snmp.
* file name:			snmp.cpp
* Vision:				1.0
* Date:					2002/07/01
* Author:				YaoQing
*
/******************************************************************************/
#include "stdafx.h"
#include "snmp.h"
#include <stdio.h>
//#include "EdiSystem.h"

smiINT32 snmp::m_sRequestId = 0;

/*******************************************************************************
*
* Function Name:		snmp
* Description:			
* Input Parameters:		
* Output Parameters:	
* Notes:				
*
/******************************************************************************/
snmp::snmp()
{
	m_sock = 0;

	m_nVer = 0;
	m_nPduType = 0;
	m_nRequestId = 0;
	m_nErrorStatus = 0;
	m_nErrorIndex = 0;

//	m_pBuf = NULL;
	m_hBuf.len = 0;
	m_hBuf.ptr = NULL;

	m_hCommunity.len = 0;
	m_hCommunity.ptr = NULL;

	m_hPdu.len = 0;
	m_hPdu.ptr = NULL;

	m_hVbl.len = 0;
	m_hVbl.ptr = NULL;

	m_netSrcIp = 0;
	m_netDstIp = 0;

	//SNMPv1 trap;
	m_pEnterpriseOid = NULL;
	m_netTrapIp = 0;
	m_nGenericTrap = 0;
	m_nSpecificTrap = 0;
	m_nTimeTicks = 0;
}

/*******************************************************************************
*
* Function Name:		~snmp
* Description:			
* Input Parameters:		
* Output Parameters:	
* Notes:				
*
/******************************************************************************/
snmp::~snmp()
{
	FreeResource();
}

/*******************************************************************************
*
* Function Name:		SendMsg
* Description:			
* Input Parameters:		srcIp, dstIp, community
* Output Parameters:	return
* Notes:				
*
/******************************************************************************/
SNMPAPI_STATUS snmp::SendMsg(/*const char *srcIp, */
							 const char *dstIp, 
							 smiINT ver, 
							 smiINT port, 
							 smiLPOCTETS community)
{
	char		buf[32];
	int			i, len, verlen, snmplen;
	smiLPBER	s;
	SOCKADDR_IN	name;

	if (m_hPdu.ptr == NULL)
		return SNMPAPI_FAILURE;

	//if ((m_netSrcIp = inet_addr(srcIp)) == INADDR_NONE)
	//	return SNMPAPI_FAILURE;

	if ((m_netDstIp = inet_addr(dstIp)) == INADDR_NONE)
		return SNMPAPI_FAILURE;

	m_nVer = ver;

	len = m_hPdu.len;

	if (community)
	{
		len += community->len;
		len += GetLiLen(community->len);
	}
	else
	{
		len += 6;
		len += GetLiLen(6);
	}
	len++;

	verlen = numtober(buf, m_nVer);
	len += verlen;
	len += GetLiLen(verlen);
	len++;

	snmplen = len;
	len += GetLiLen(snmplen);
	len++;

	s = (smiLPBER)malloc(len);
	if (s == NULL)
		return SNMPAPI_FAILURE;
//	m_pBuf = s;
	m_hBuf.len = len;
	m_hBuf.ptr = s;

	*s++ = SNMP_SYNTAX_SEQUENCE;
	s += SetLi(s, snmplen);

	*s++ = SNMP_SYNTAX_INT;
	s += SetLi(s, verlen);
	len = numtober(buf, m_nVer);
	for ( i = 0; i < len; i++)
		*s++ = buf[i];

	*s++ = SNMP_SYNTAX_OCTETS;
	if (community)
	{
		s += SetLi(s, community->len);
		memcpy(s, community->ptr, community->len);
		m_hCommunity.len = community->len;
	}
	else
	{
		s += SetLi(s, 6);
		memcpy(s, "public", 6);
		m_hCommunity.len = 6;
	}
	m_hCommunity.ptr = s;
	s += m_hCommunity.len;

	memcpy(s, m_hPdu.ptr, m_hPdu.len);
	if (m_hPdu.ptr)
	{
		FreePtr(m_hPdu.ptr);
	}
	m_hPdu.ptr = s;
	m_hVbl.ptr = s + (m_hPdu.len - m_hVbl.len);

	s += m_hPdu.len;

	name.sin_family = AF_INET;
	name.sin_port = htons((u_short)port);
	name.sin_addr.S_un.S_addr = m_netDstIp;
//	if (sendto(m_sock, (const char *)m_pBuf, m_hBuf.len, 0, (struct sockaddr *)&name, sizeof(name)) == SOCKET_ERROR)
	if (sendto(m_sock, (const char *)m_hBuf.ptr, m_hBuf.len, 0, (struct sockaddr *)&name, sizeof(name)) == SOCKET_ERROR)
	{
		char errbuf[64];
		sprintf(errbuf, "snmp::SendMsg() failed! Error code: %d", WSAGetLastError());
		SysDebugInfo(errbuf);
		return SNMPAPI_FAILURE;
	}

	FreePtr(m_hBuf.ptr);
	m_hBuf.len = 0;
	m_hPdu.len = 0;
	m_hPdu.ptr = NULL;
	m_hVbl.len = 0;
	m_hVbl.ptr = NULL;

	return SNMPAPI_SUCCESS;
}

void snmp::SetTimeout(smiINT timeout)
{
	if (timeout > 0)
	{
		timeout *= 1000;
		setsockopt(m_sock, SOL_SOCKET, SO_RCVTIMEO, (const char *)(&timeout), sizeof(smiINT));
	}
	else
	{
		timeout = 0;
		setsockopt(m_sock, SOL_SOCKET, SO_RCVTIMEO, (const char *)(&timeout), sizeof(smiINT));
	}
}

/*******************************************************************************
*
* Function Name:		RecvMsg
* Description:			
* Input Parameters:		
* Output Parameters:	return
* Notes:				
*
/******************************************************************************/
SNMPAPI_STATUS snmp::RecvMsg()
{
	int			ret, fromlen;
	smiBYTE		tag;
	smiLPBER	s;
	smiUINT32	len, val;
	SOCKADDR_IN from, name;
	char		buf[MAXUDPSIZE];

	if (m_sock == 0)
	{
		SysDebugInfo("m_sock is zero.");
		return SNMPAPI_FAILURE;
	}
	
	fromlen = sizeof(struct sockaddr);
	if ((ret = recvfrom(m_sock, buf, sizeof(buf), 0, (struct sockaddr *)&from, &fromlen)) == SOCKET_ERROR)
		return SNMPAPI_FAILURE;
	else
		len = ret;

	ret = getsockname(m_sock, (struct sockaddr *)&name, &fromlen);
	if (ret == SOCKET_ERROR)
		return SNMPAPI_FAILURE;

	m_netSrcIp = from.sin_addr.S_un.S_addr;
	m_netDstIp = name.sin_addr.S_un.S_addr;

	s = (smiLPBER)malloc(len);
	if (s == NULL)
	{
		SysDebugInfo("snmp::RecvMsg() failed! Alloc memory failed!");
		return SNMPAPI_FAILURE;
	}
	memcpy(s, buf, len);
//	m_pBuf = s;
	if (m_hBuf.ptr)
		FreePtr(m_hBuf.ptr);
	m_hBuf.len = len;
	m_hBuf.ptr = s;

	tag = *s++;
	if (tag != SNMP_SYNTAX_SEQUENCE)
	{
		SysDebugInfo("snmp::RecvMsg() failed! Snmp tag is wrong!");
		return SNMPAPI_FAILURE;
	}

	val = GetLiVal(s, &len);
	s += len;
	if (m_hBuf.len - val - abs(s - m_hBuf.ptr))
	{
		SysDebugInfo("snmp::RecvMsg() failed! Snmp length is wrong!");
		return SNMPAPI_FAILURE;
	}

	tag = *s++;
	if (tag != SNMP_SYNTAX_INT)
	{
		SysDebugInfo("snmp::RecvMsg() failed! Ver tag is wrong!");
		return SNMPAPI_FAILURE;
	}

	val = GetLiVal(s, &len);
	s += len;

	len = val;
	val = bertonum(s, len, FALSE);
	s += len;
	if ((val != 0) && (val != 1))
	{
		SysDebugInfo("snmp::RecvMsg() failed! Ver is wrong!");
		return SNMPAPI_FAILURE;
	}
	m_nVer = val;

	tag = *s++;
	if (tag != SNMP_SYNTAX_OCTETS)
	{
		SysDebugInfo("snmp::RecvMsg() failed! Community tag is wrong!");
		return SNMPAPI_FAILURE;
	}

	val = GetLiVal(s, &len);
	s += len;

	m_hCommunity.len = val;
	m_hCommunity.ptr = s;

	s += val;

	m_hPdu.len = m_hBuf.len - abs(s - m_hBuf.ptr);
	m_hPdu.ptr = s;

	return SNMPAPI_SUCCESS;
}

/*******************************************************************************
*
* Function Name:		CreatePdu
* Description:			
* Input Parameters:		pdu_type, request_id, error_status, error_index
* Output Parameters:	return
* Notes:				
*
/******************************************************************************/
SNMPAPI_STATUS snmp::CreatePdu(smiINT pdu_type, 
							   smiINT32 request_id,
							   smiINT error_status, 
							   smiINT error_index)
{
	char		buf[32];
	smiLPBER	s;
	smiUINT32	i, len, pduvallen, request_id_len, error_status_len, error_index_len;

	if (m_hVbl.ptr == NULL) return SNMPAPI_FAILURE;

	len = m_hVbl.len;

	error_index_len = numtober(buf, error_index);
	len += error_index_len;
	len += GetLiLen(error_index_len);
	len++;

	error_status_len = numtober(buf, error_status);
	len += error_status_len;
	len += GetLiLen(error_status_len);
	len++;

	if (!request_id)
		m_nRequestId = ++snmp::m_sRequestId;
	else
		m_nRequestId = request_id;
	request_id_len = numtober(buf, m_nRequestId);
	len += request_id_len;
	len += GetLiLen(request_id_len);
	len++;

	pduvallen = len;
	len += GetLiLen(pduvallen);
	len++;

	s = (smiLPBER)malloc(len);
	if (s == NULL)
	{
		SysDebugInfo("snmp::CreatePdu() failed! Alloc memory failed!");
		return SNMPAPI_FAILURE;
	}
//	m_pBuf = s;
	m_hPdu.len = len;
	m_hPdu.ptr = s;

	*s++ = (smiBER)pdu_type;
	m_nPduType = pdu_type;
	s += SetLi(s, pduvallen);
	
	*s++ = SNMP_SYNTAX_INT32;
	s += SetLi(s, request_id_len);
	len = numtober(buf, m_nRequestId);
	for (i = 0; i < len; i++)
		*s++ = buf[i];

	*s++ = SNMP_SYNTAX_INT;
	s += SetLi(s, error_status_len);
	len = numtober(buf, error_status);
	for (i = 0; i < len; i++)
		*s++ = buf[i];
	m_nErrorStatus = error_status;

	*s++ = SNMP_SYNTAX_INT;
	s += SetLi(s, error_index_len);
	len = numtober(buf, error_index);
	for (i = 0; i < len; i++)
		*s++ = buf[i];
	m_nErrorIndex = error_index;

	memcpy(s, m_hVbl.ptr, m_hVbl.len);
	if (m_hVbl.ptr)
	{
		FreePtr(m_hVbl.ptr);
	}
	m_hVbl.ptr = s;

	s += m_hVbl.len;

	return SNMPAPI_SUCCESS;
}

/*******************************************************************************
*
* Function Name:		GetPduData
* Description:			
* Input Parameters:		
* Output Parameters:	return
* Notes:				
*
/******************************************************************************/
SNMPAPI_STATUS snmp::GetPduData()
{
	smiBYTE		tag;
	smiLPBER	s;
	smiUINT32	len, val;

	if ((s = m_hPdu.ptr) == NULL)
	{
		SysDebugInfo("snmp::GetPduData() failed! m_hPdu.ptr is NULL!");
		return SNMPAPI_FAILURE;
	}

	tag = *s++;
	if ((tag & (ASN_CONTEXT | ASN_CONSTRUCTOR)) != (ASN_CONTEXT | ASN_CONSTRUCTOR))
	{
		SysDebugInfo("snmp::GetPduData() failed! PDU tag is wrong!");
		return SNMPAPI_FAILURE;
	}
	m_nPduType = tag;

	val = GetLiVal(s, &len);
	s += len;

	if (tag == SNMP_PDU_V1TRAP)
	{
		tag = *s++;
		if (tag != SNMP_SYNTAX_OID)
		{
			SysDebugInfo("snmp::GetPduData(Trap) failed ! Enterprise tag is wrong!");
			return SNMPAPI_FAILURE;
		}

		val = GetLiVal(s, &len);
		s += len;

		if (GetSmiOID(s, val, m_pEnterpriseOid) == SNMPAPI_FAILURE)
		{
			SysDebugInfo("snmp::GetPduData(Trap) failed ! GetSmiOID(Enterprise) is wrong!");
			return SNMPAPI_FAILURE;
		}
		s += val;

		tag = *s++;
		if (tag != SNMP_SYNTAX_IPADDR)
		{
			FreeOid(m_pEnterpriseOid);
			SysDebugInfo("snmp::GetPduData(Trap) failed ! Ip tag is wrong!");
			return SNMPAPI_FAILURE;
		}

		val = GetLiVal(s, &len);
		s += len;
		if (val != 4)
		{
			FreeOid(m_pEnterpriseOid);
			SysDebugInfo("snmp::GetPduData(Trap) failed ! Ip length is wrong!");
			return SNMPAPI_FAILURE;
		}

		memcpy(&m_netTrapIp, s, val);
		s += val;

		tag = *s++;
		if (tag != SNMP_SYNTAX_INT)
		{
			FreeOid(m_pEnterpriseOid);
			SysDebugInfo("snmp::GetPduData(Trap) failed ! Gerenic Trap tag is wrong!");
			return SNMPAPI_FAILURE;
		}
		
		val = GetLiVal(s, &len);
		s += len;

		m_nGenericTrap = bertonum(s, val, FALSE);
		s += val;

		tag = *s++;
		if (tag != SNMP_SYNTAX_INT)
		{
			FreeOid(m_pEnterpriseOid);
			SysDebugInfo("snmp::GetPduData(Trap) failed ! Specific Trap tag is wrong!");
			return SNMPAPI_FAILURE;
		}
		
		val = GetLiVal(s, &len);
		s += len;

		m_nSpecificTrap = bertonum(s, val, FALSE);
		s += val;

		tag = *s++;
		if (tag != SNMP_SYNTAX_TIMETICKS)
		{
			FreeOid(m_pEnterpriseOid);
			SysDebugInfo("snmp::GetPduData(Trap) failed ! Time ticks tag is wrong!");
			return SNMPAPI_FAILURE;
		}
		
		val = GetLiVal(s, &len);
		s += len;

		m_nTimeTicks = bertonum(s, val, FALSE);
		s += val;
	}
	else
	{
		tag = *s++;
		if (tag != SNMP_SYNTAX_INT32)
		{
			SysDebugInfo("snmp::GetPduData() failed! request_id tag is wrong!");
			return SNMPAPI_FAILURE;
		}

		val = GetLiVal(s, &len);
		s += len;

		m_nRequestId = bertonum(s, val, FALSE);
		s += val;

		tag = *s++;
		if (tag != SNMP_SYNTAX_INT)
		{
			SysDebugInfo("snmp::GetPduData() failed! error_status tag is wrong!");
			return SNMPAPI_FAILURE;
		}

		val = GetLiVal(s, &len);
		s += len;

		m_nErrorStatus = bertonum(s, val, FALSE);
		s += val;

		tag = *s++;
		if (tag != SNMP_SYNTAX_INT)
		{
			SysDebugInfo("snmp::GetPduData() failed! error_index tag is wrong!");
			return SNMPAPI_FAILURE;
		}

		val = GetLiVal(s, &len);
		s += len;

		m_nErrorIndex = bertonum(s, val, FALSE);
		s += val;
	}

	m_hVbl.len = m_hPdu.len - abs(s - m_hPdu.ptr);
	m_hVbl.ptr = s;

	return SNMPAPI_SUCCESS;
}

/*******************************************************************************
*
* Function Name:		CountVbl
* Description:			
* Input Parameters:		
* Output Parameters:	return
* Notes:				
*
/******************************************************************************/
SNMPAPI_STATUS snmp::CountVbl()
{
	char		buf[64];
	smiBYTE		tag;
	smiLPBER	s;
	smiUINT32	len, val, count;

	if ((s = m_hVbl.ptr) == NULL)
	{
		SysDebugInfo("snmp::CountVbl() failed! m_hVbl.ptr is NULL!");
		return SNMPAPI_FAILURE;
	}

	tag = *s++;
	if (tag != SNMP_SYNTAX_SEQUENCE)
	{
		SysDebugInfo("snmp::CountVbl() failed! VBL tag li wrong!");
		return SNMPAPI_FAILURE;
	}

	val = GetLiVal(s, &len);
	s += len;

	for (count = 0; m_hVbl.len - abs(s - m_hVbl.ptr); count++)
	{
		tag = *s++;
		if (tag != SNMP_SYNTAX_SEQUENCE)
		{
			sprintf(buf, "snmp::CountVbl() failed! var%d tag is wrong!", count);
			SysDebugInfo(buf);
			return SNMPAPI_FAILURE;
		}

		val = GetLiVal(s, &len);
		s += len;

		tag = *s++;
		if (tag != SNMP_SYNTAX_OID)
		{
			sprintf(buf, "snmp::CountVbl() failed! var%d OID tag is wrong!", count);
			SysDebugInfo(buf);
			return SNMPAPI_FAILURE;
		}

		val = GetLiVal(s, &len);
		s += len;

		s += val;

		tag = *s++;
		if (!(((tag & (ASN_UNIVERSAL | ASN_PRIMITIVE)) == (ASN_UNIVERSAL   | ASN_PRIMITIVE)) || 
			((tag & (ASN_APPLICATION | ASN_PRIMITIVE)) == (ASN_APPLICATION | ASN_PRIMITIVE)) || 
			((tag & (ASN_CONTEXT     | ASN_PRIMITIVE)) == (ASN_CONTEXT     | ASN_PRIMITIVE)) || 
			((tag & (ASN_UNIVERSAL   | ASN_CONSTRUCTOR))== (ASN_UNIVERSAL  | ASN_CONSTRUCTOR))))
		{
			sprintf(buf, "snmp::CountVbl() failed! var%d VALUE tag is wrong!", count);
			SysDebugInfo(buf);
			return SNMPAPI_FAILURE;
		}

		val = GetLiVal(s, &len);
		s += len;

		s += val;
	}

	return count;
}

/*******************************************************************************
*
* Function Name:		GetVb
* Description:			
* Input Parameters:		index, oid, val
* Output Parameters:	return
* Notes:				
*
/******************************************************************************/
SNMPAPI_STATUS snmp::GetVb(smiUINT32 index, smiLPOID &oid, smiLPVALUE &val)
{
	char		buf[64];
	smiBYTE		tag;
	smiLPBER	s;
	smiUINT32	len, value, count;

	oid = NULL;
	val = NULL;

	if ((s = m_hVbl.ptr) == NULL)
	{
		SysDebugInfo("snmp::GetVb() failed! m_hVbl.ptr is NULL!");
		return SNMPAPI_FAILURE;
	}

	tag = *s++;
	if (tag != SNMP_SYNTAX_SEQUENCE)
	{
		SysDebugInfo("snmp::GetVb() failed! VBL tag is wrong!");
		return SNMPAPI_FAILURE;
	}

	value = GetLiVal(s, &len);
	s += len;

	for (count = 0; m_hVbl.len - abs(s - m_hVbl.ptr); count++)
	{
		tag = *s++;
		if (tag != SNMP_SYNTAX_SEQUENCE)
		{
			sprintf(buf, "snmp::GetVb() failed! var%d tag is wrong!", count);
			SysDebugInfo(buf);
			return SNMPAPI_FAILURE;
		}

		value = GetLiVal(s, &len);
		s += len;

		if (index == count)
		{
			tag = *s++;
			if (tag != SNMP_SYNTAX_OID)
			{
				sprintf(buf, "snmp::GetVb() failed! var%d OID tag is wrong!", count);
				SysDebugInfo(buf);
				return SNMPAPI_FAILURE;
			}
			
			value = GetLiVal(s, &len);
			s += len;

			if (GetSmiOID(s, value, oid) == SNMPAPI_FAILURE)
			{
				sprintf(buf, "snmp::GetVb() failed! Get var%d OID failed!", count);
				SysDebugInfo(buf);
				return SNMPAPI_FAILURE;
			}
			s += value;

			tag = *s++;
			if (!(((tag & (ASN_UNIVERSAL | ASN_PRIMITIVE)) == (ASN_UNIVERSAL   | ASN_PRIMITIVE)) ||	
				((tag & (ASN_APPLICATION | ASN_PRIMITIVE)) == (ASN_APPLICATION | ASN_PRIMITIVE)) || 
				((tag & (ASN_CONTEXT     | ASN_PRIMITIVE)) == (ASN_CONTEXT     | ASN_PRIMITIVE)) || 
				((tag & (ASN_UNIVERSAL   | ASN_CONSTRUCTOR))== (ASN_UNIVERSAL  | ASN_CONSTRUCTOR))))
			{
				FreeOid(oid);
				sprintf(buf, "snmp::GetVb() failed! var%d VALUE tag is wrong!", count);
				SysDebugInfo(buf);
				return SNMPAPI_FAILURE;
			}

			value = GetLiVal(s, &len);
			s += len;

			if (GetSmiVALUE(s, value, tag, val) == SNMPAPI_FAILURE)
			{
				FreeOid(oid);
				sprintf(buf, "snmp::GetVb() failed! Get var%d VALUE failed!", count);
				SysDebugInfo(buf);
				return SNMPAPI_FAILURE;
			}
			s += value;
		}
		else
			s += value;
	}

	return SNMPAPI_SUCCESS;
}


/*******************************************************************************
*
* Function Name:		GetSmiOID
* Description:			
* Input Parameters:		ber, len, oid
* Output Parameters:	oid
* Notes:				
*
/******************************************************************************/
SNMPAPI_STATUS snmp::GetSmiOID(smiLPCBER ber, smiUINT32 len, smiLPOID &oid)
{
	smiLPCBER	s;
	smiUINT32	i, val, buf[MAXOBJIDSIZE];

	oid = NULL;

	s = ber;
	
	for (val = 0, i= 0; len - abs(s - ber); val = 0, i++)
	{
		if (*s & 0x80)
		{
			while (*s & 0x80)
			{
				val += *s++ & 0x7F;
				val <<= 7;
			}
			val += *s++;
		}
		else
		{
			val = *s++;
		}
		
		buf[i] = val;
	}

	s = (smiLPBER)malloc(sizeof(smiUINT32) + sizeof(smiLPUINT32) + sizeof(smiUINT32) * (i + 1));
	if (s == NULL)
	{
		SysDebugInfo("snmp::GetSmiOID() failed! Alloc memory failed!");
		return SNMPAPI_FAILURE;
	}
	oid = (smiLPOID)s;
	oid->len = i + 1;
	oid->ptr = (smiLPUINT32)(s + sizeof(smiUINT32) + sizeof(smiLPUINT32));

	if (i <= 0)
		*(oid->ptr) = 0;
	else
	{
		*(oid->ptr) = 1;
		*(oid->ptr + 1) = 3;
	}
	for (val = 1; val < i; val++)
	{
		*(oid->ptr + val + 1) = buf[val];
	}
	if (oid->len == 2)
	{
		*(oid->ptr) = 0;
		*(oid->ptr + 1) = 0;
	}

	return SNMPAPI_SUCCESS;
}

/*******************************************************************************
*
* Function Name:		GetSmiVALUE
* Description:			
* Input Parameters:		ber, len, tag
* Output Parameters:	return, val
* Notes:				
*
/******************************************************************************/
SNMPAPI_STATUS snmp::GetSmiVALUE(smiLPCBER ber, smiUINT32 len, smiBYTE tag, smiLPVALUE &val)
{
	int			i;
	char		ch;
	__int64		i64;
	smiLPBER	s;
	smiLPOID	oid;

	val = NULL;

	switch(tag)
	{
	case SNMP_SYNTAX_INT:
//	case SNMP_SYNTAX_INT32:
		s = (smiLPBER)malloc(sizeof(smiUINT32) + sizeof(smiINT));
		if (s == NULL)
		{
			SysDebugInfo("snmp::GetSmiVALUE() failed! Alloc memory failed!");
			return SNMPAPI_FAILURE;
		}
		val = (smiLPVALUE)s;
		val->syntax = tag;
		val->value.sNumber = bertonum(ber, len, TRUE);
		break;
	case SNMP_SYNTAX_SEQUENCE:
		val = NULL;
		break;
	case SNMP_SYNTAX_UINT32:
	case SNMP_SYNTAX_CNTR32:
	case SNMP_SYNTAX_GAUGE32:
	case SNMP_SYNTAX_TIMETICKS:
		s = (smiLPBER)malloc(sizeof(smiUINT32) + sizeof(smiUINT32));
		if (s == NULL)
		{
			SysDebugInfo("snmp::GetSmiVALUE() failed! Alloc memory failed!");
			return SNMPAPI_FAILURE;
		}
		val = (smiLPVALUE)s;
		val->syntax = tag;
		val->value.uNumber = bertonum(ber, len, FALSE);
		break;
	case SNMP_SYNTAX_CNTR64:
		memcpy((char *)&i64 + (sizeof(__int64) - len), ber, len);
		for (i = 0; i < sizeof(__int64)/2; i++)
		{
			ch = *((char *)&i64 + i);
			*((char *)&i64 + i) = *((char *)&i64 + sizeof(__int64) - i - 1);
			*((char *)&i64 + sizeof(__int64) - i - 1) = ch;
		}
		s = (smiLPBER)malloc(sizeof(smiUINT32) + sizeof(smiCNTR64));
		if (s == NULL)
		{
			SysDebugInfo("snmp::GetSmiVALUE() failed! Alloc memory failed!");
			return SNMPAPI_FAILURE;
		}
		val = (smiLPVALUE)s;
		val->syntax = tag;
		val->value.hNumber.lopart = *(smiLPUINT32)&i64;
		val->value.hNumber.hipart = *((smiLPUINT32)&i64 + 1);
		break;
	case SNMP_SYNTAX_OCTETS:
	case SNMP_SYNTAX_BITS:
	case SNMP_SYNTAX_OPAQUE:
    case SNMP_SYNTAX_NSAPADDR:
	case SNMP_SYNTAX_IPADDR:
		s = (smiLPBER)malloc(sizeof(smiUINT32) + sizeof(smiUINT32) + sizeof(smiLPBYTE) + sizeof(smiBYTE) * len);
		if (s == NULL)
		{
			SysDebugInfo("snmp::GetSmiVALUE() failed! Alloc memory failed!");
			return SNMPAPI_FAILURE;
		}
		val = (smiLPVALUE)s;
		val->syntax = tag;
		val->value.string.len = len;
		val->value.string.ptr = (smiLPBYTE)(s + sizeof(smiUINT32) + sizeof(smiUINT32) + sizeof(smiLPBYTE));
		memcpy(val->value.string.ptr, ber, len);
		break;
	case SNMP_SYNTAX_OID:
		if (GetSmiOID(ber, len, oid) == SNMPAPI_FAILURE)
		{
			return SNMPAPI_FAILURE;
		}
		s = (smiLPBER)malloc(sizeof(smiUINT32) + sizeof(smiUINT32) + sizeof(smiLPUINT32) + sizeof(smiUINT32) * (oid->len));
		if (s == NULL)
		{
			SysDebugInfo("snmp::GetSmiVALUE() failed! Alloc memory failed!");
			return SNMPAPI_FAILURE;
		}
		val = (smiLPVALUE)s;
		val->syntax = tag;
		val->value.oid.len = oid->len;
		val->value.oid.ptr = (smiLPUINT32)(s + sizeof(smiUINT32) + sizeof(smiUINT32) + sizeof(smiLPUINT32));
		memcpy(val->value.oid.ptr, oid->ptr, sizeof(smiUINT32) * (oid->len));
		FreeOid(oid);
		break;
	case SNMP_SYNTAX_NULL:
	case SNMP_SYNTAX_NOSUCHOBJECT:
	case SNMP_SYNTAX_NOSUCHINSTANCE:
	case SNMP_SYNTAX_ENDOFMIBVIEW:
		s = (smiLPBER)malloc(sizeof(smiUINT32) + sizeof(smiBYTE));
		if (s == NULL)
		{
			SysDebugInfo("snmp::GetSmiVALUE() failed! Alloc memory failed!");
			return SNMPAPI_FAILURE;
		}
		val = (smiLPVALUE)s;
		val->syntax = tag;
		val->value.empty = NULL;
		break;
	default:
		val = NULL;
		break;
	}

	return SNMPAPI_SUCCESS;
}

/*******************************************************************************
*
* Function Name:		CreateVbl
* Description:			
* Input Parameters:		oid, val
* Output Parameters:	
* Notes:				
*
/******************************************************************************/
SNMPAPI_STATUS snmp::CreateVbl(smiLPCOID const oid, smiLPCVALUE const val)
{
	smiLPBER	s;
	smiUINT32	oidlen, vallen, vbvallen, vblen, len;

	if (oid == NULL) return SNMPAPI_FAILURE;

	vblen = GetVbLen(oid, &oidlen, val, &vallen, &vbvallen);
	len = vblen;
	len += GetLiLen(len);
	len++;

	s = (smiLPBER)malloc(len);
	if (s == NULL)
	{
		SysDebugInfo("snmp::CreateVbl() failed! Alloc memory failed!");
		return SNMPAPI_FAILURE;
	}
//	m_pBuf = s;
	m_hVbl.len = len;
	m_hVbl.ptr = s;
	
	*s++ = SNMP_SYNTAX_SEQUENCE;
	s += SetLi(s, vblen);

	s += FillVb(s, oid, oidlen, val, vallen ,vbvallen);

	return SNMPAPI_SUCCESS;
}

/*******************************************************************************
*
* Function Name:		GetVbLen
* Description:			得到smiOID和smiVALUE的长度
* Input Parameters:		oid, val
* Output Parameters:	return(长度), oidlen, vallen, vbvallen
* Notes:				
*
/******************************************************************************/
smiUINT32 snmp::GetVbLen(smiLPCOID const oid, 
						 smiUINT32 *oidlen, 
						 smiLPCVALUE const val, 
						 smiUINT32 *vallen, 
						 smiUINT32 *vbvallen)
{
	smiUINT32	len, totallen;

	len = GetSmiOIDLen(oid);
	*oidlen = len;
	len += GetLiLen(len);
	len++;
	totallen = len;

	len = GetSmiVALUELen(val);
	*vallen = len;
	len += GetLiLen(len);
	len++;
	totallen += len;
	*vbvallen = totallen;

	totallen += GetLiLen(totallen);
	totallen++;

	return totallen;
}

/*******************************************************************************
*
* Function Name:		GetSmiOIDLen
* Description:			得到SmiOID的长度
* Input Parameters:		oid
* Output Parameters:	return
* Notes:				不包括标签和长度标识符的长度
*
/******************************************************************************/
smiUINT32 snmp::GetSmiOIDLen(smiLPCOID const oid)
{
	char		buf[32];
	smiUINT32	i, len;

	for (len = 0, i = 0; i < oid->len; i++)
		{
			len += numtooid(buf, *(oid->ptr + i));
		}

	return len - 1;
}

/*******************************************************************************
*
* Function Name:		GetSmiVALUELen
* Description:			得到SmiVALUE的长度
* Input Parameters:		val
* Output Parameters:	return
* Notes:				不包括标签和长度标识符的长度
*
/******************************************************************************/
smiUINT32 snmp::GetSmiVALUELen(smiLPCVALUE const val)
{
	char		buf[32];
	smiUINT32	len;

	if (val == NULL)
		return 0;

	switch(val->syntax)
	{
	case SNMP_SYNTAX_INT:
//	case SNMP_SYNTAX_INT32:
		len = numtober(buf, val->value.sNumber);
		break;
	case SNMP_SYNTAX_SEQUENCE:
		len = 0;
		break;
	case SNMP_SYNTAX_UINT32:
	case SNMP_SYNTAX_CNTR32:
	case SNMP_SYNTAX_GAUGE32:
	case SNMP_SYNTAX_TIMETICKS:
		len = numtober(buf, val->value.uNumber);
		break;
	case SNMP_SYNTAX_CNTR64:
		len = numtober(buf, val->value.hNumber.hipart);
		len += numtober(buf, val->value.hNumber.lopart);
		break;
	case SNMP_SYNTAX_OCTETS:
	case SNMP_SYNTAX_BITS:
	case SNMP_SYNTAX_OPAQUE:
    case SNMP_SYNTAX_NSAPADDR:
	case SNMP_SYNTAX_IPADDR:
		len = val->value.string.len;
		break;
	case SNMP_SYNTAX_OID:
		len = GetSmiOIDLen(&(val->value.oid));
		break;
	case SNMP_SYNTAX_NULL:
	case SNMP_SYNTAX_NOSUCHOBJECT:
	case SNMP_SYNTAX_NOSUCHINSTANCE:
	case SNMP_SYNTAX_ENDOFMIBVIEW:
		len = 0;
		break;
	default:
		len = 0;
		break;
	}

	return len;
}

/*******************************************************************************
*
* Function Name:		FillVb
* Description:			填入一个oid和值
* Input Parameters:		ber, oid, oidlen, val, vallen, vbvallen
* Output Parameters:	ber, return(长度)
* Notes:				从指定的位置(ber)开始填写
*
/******************************************************************************/
smiUINT32 snmp::FillVb(smiLPBER ber, 
					   smiLPCOID const oid, 
					   smiUINT32 oidlen, 
					   smiLPCVALUE const val, 
					   smiUINT32 vallen, 
					   smiUINT32 vbvallen)
{
	char		buf[32];
	smiLPBER	s;
	smiUINT32	i, j, len;

	s = ber;

	*s++ = SNMP_SYNTAX_SEQUENCE;
	s += SetLi(s, vbvallen);

	*s++ = SNMP_SYNTAX_OID;
	s += SetLi(s, oidlen);

	for (i = 1; i < oid->len; i++)
	{
		if (i == 1) 
			j = numtooid(buf, 43);
		else
			j = numtooid(buf, *(oid->ptr + i));
		while (j--)
		{
			*s++ = buf[j];
		}
	}

	if (val == NULL)
	{
		*s++ = SNMP_SYNTAX_NULL;
		s += SetLi(s, 0);
		return abs(s - ber);
	}

	*s++ = (smiBYTE)(val->syntax);
	s += SetLi(s, vallen);

	switch(val->syntax)
	{
	case SNMP_SYNTAX_INT:
//	case SNMP_SYNTAX_INT32:
		len = numtober(buf, val->value.sNumber);
		for (i = 0; i < len; i++)
		{
			*s++ = buf[i];
		}
		break;
	case SNMP_SYNTAX_SEQUENCE:
		break;
	case SNMP_SYNTAX_UINT32:
	case SNMP_SYNTAX_CNTR32:
	case SNMP_SYNTAX_GAUGE32:
	case SNMP_SYNTAX_TIMETICKS:
		len = numtober(buf, val->value.uNumber);
		for (i = 0; i < len; i++)
		{
			*s++ = buf[i];
		}
		break;
	case SNMP_SYNTAX_CNTR64:
		len = numtober(buf, val->value.hNumber.hipart);
		for (i = 0; i < len; i++)
		{
			*s++ = buf[i];
		}
		len = numtober(buf, val->value.hNumber.lopart);
		for (i = 0; i < len; i++)
		{
			*s++ = buf[i];
		}
		break;
	case SNMP_SYNTAX_OCTETS:
	case SNMP_SYNTAX_BITS:
	case SNMP_SYNTAX_OPAQUE:
    case SNMP_SYNTAX_NSAPADDR:
	case SNMP_SYNTAX_IPADDR:
		len = val->value.string.len;
		for (i = 0; i < len; i++)
		{
			*s++ = *(val->value.string.ptr + i);
		}
		break;
	case SNMP_SYNTAX_OID:
		len = val->value.oid.len;
		for (i = 1; i < len; i++)
		{
			if (i == 1) 
				j = numtooid(buf, 43);
			else
				j = numtooid(buf, *(val->value.oid.ptr + i));
			while (j--)
			{
				*s++ = buf[j];
			}
		}
		break;
	case SNMP_SYNTAX_NULL:
	case SNMP_SYNTAX_NOSUCHOBJECT:
	case SNMP_SYNTAX_NOSUCHINSTANCE:
	case SNMP_SYNTAX_ENDOFMIBVIEW:
		break;
	default:
		break;
	}

	return abs(s - ber);
}

/*******************************************************************************
*
* Function Name:		GetLiLen
* Description:			得到长度标识符的长度
* Input Parameters:		num
* Output Parameters:	return
* Notes:				如果长度为1，则采用短标识符，否则采用长标识符，长度加1
*
/******************************************************************************/
smiUINT32 snmp::GetLiLen(smiUINT32 num)
{
	char		buf[32];
	smiUINT32	len;

	len = numtober(buf, num);
	return (len - 1)? len + 1: len;
}

/*******************************************************************************
*
* Function Name:		GetLiVal
* Description:			得到长度标识符的值和长度
* Input Parameters:		ber
* Output Parameters:	len, return(长度标识符的值)
* Notes:				ber指向长度标识符的头
*
/******************************************************************************/
smiUINT32 snmp::GetLiVal(smiLPCBER ber, smiLPUINT32 len)
{
	smiUINT32	i, val;

	if (*ber & 0x80L)
	{
		i = *ber & 0x7FL;
		val = bertonum((smiLPCBER)(ber + 1), i, FALSE);
		if (len) *len = i + 1;
	}
	else
	{
		val = *ber;
		if (len) *len = 1;
	}

	return val;
}

/*******************************************************************************
*
* Function Name:		SetLi
* Description:			设置长度标识符
* Input Parameters:		ber, val
* Output Parameters:	return(长度标识符的长度)
* Notes:				如果长度为1，则采用短标识符，否则采用长标识符，长度加1
*
/******************************************************************************/
smiUINT32 snmp::SetLi(smiLPBER ber, smiUINT32 val)
{
	char		buf[32];
	smiUINT32	i, len;

	len = numtober(buf, val);
	if (len - 1)
	{
		*ber = (smiBER)(len & 0x80L);
		for (i = 0; i < len; i++)
		{
			*(ber + i + 1) = buf[i];
		}
		len++;
	}
	else
	{
		*ber = (smiBER)val;
	}

	return len;
}

/*******************************************************************************
*
* Function Name:		numtober
* Description:			十进制编码变换为BER编码
* Input Parameters:		num
* Output Parameters:	buf, return
* Notes:				BER编码：负数用二进制补码表示，非负数用无符号表示法表示
*
/******************************************************************************/
long snmp::numtober(char buf[32], long num)
{
	long	i, sign;
	long	netnum, mask;

	sign = (num < 0);
	for (i = sizeof(long) - 1; i > 0; i--)
	{
		mask = (0xffL << (i * 8));
		if (sign)
		{
			if ((num & mask) != mask) break;
		}
		else
		{
			if (num & mask)	break;
		}
	}

	mask = (num & (0x80L << (i * 8)));
	if ((mask && !sign) || (sign && !mask))	i++;
	i++;
	netnum = htonl(num);
	memcpy(buf, (char *)&netnum + (sizeof(long) - i), i);

	return i;
}

/*******************************************************************************
*
* Function Name:		bertonum
* Description:			BER编码变换为十进制编码
* Input Parameters:		ber, len
* Output Parameters:	return
* Notes:				
*
/******************************************************************************/
long snmp::bertonum(smiLPCBER ber, long len, BOOL sign)
{
	long num, netnum = 0;

	memcpy((char *)&netnum + (sizeof(long) - len), ber, len);
	num = ntohl(netnum);

	if (sign)
	{
		if ((*ber & 0x80) && (len < sizeof(long)))
		{
			num |= 0xFFFFFFFFL << (len * 8);
		}
	}
	else
		return num;

	return num;
}

/*******************************************************************************
*
* Function Name:		numtooid
* Description:			数值转换为SNMP数据报中OID的表示方法
* Input Parameters:		num
* Output Parameters:	buf(单个OID变换后的结果), return(单个OID变换后的长度)
* Notes:				一个无符号数缩占位数超过一个字节，则从最高字节往后排，
*						每个字节的最高位位1，直至最后一位的最高位才为0。
*						每个字节的后7位从高到低排列组成它的值。
*
/******************************************************************************/
long snmp::numtooid(char buf[32], smiUINT32 num)
{
	smiUINT32 i;

	if (num)
	{
		for(i = 0; num; i++)
		{
			buf[i] = (char)(num & 0x7FL);
			if (i) buf[i] |= 0x80L;
			num >>= 7;
		}
	}
	else
	{
		buf[0] = 0;
		return 1;
	}

	return i;
}


/*******************************************************************************
*
* Function Name:		SetHandle
* Description:			
* Input Parameters:		handle
* Output Parameters:	
* Notes:				
*
/******************************************************************************/
void snmp::SetHandle(HANDLE handle)
{
	m_sock = (SOCKET)handle;
	return;
}

/*******************************************************************************
*
* Function Name:		FreeResource
* Description:			
* Input Parameters:		
* Output Parameters:	
* Notes:				
*
/******************************************************************************/
void snmp::FreeResource()
{
	/*
	if (m_pBuf)
		FreePtr(m_pBuf);
	*/
	if (m_hBuf.ptr)
		FreePtr(m_hBuf.ptr);

	if (m_pEnterpriseOid)
		FreeOid(m_pEnterpriseOid);

	m_sock = 0;

	m_nVer = 0;
	m_nPduType = 0;
	m_nRequestId = 0;
	m_nErrorStatus = 0;
	m_nErrorIndex = 0;

//	m_pBuf = NULL;
	m_hBuf.len = 0;
	m_hBuf.ptr = NULL;

	m_hCommunity.len = 0;
	m_hCommunity.ptr = NULL;

	m_hPdu.len = 0;
	m_hPdu.ptr = NULL;

	m_hVbl.len = 0;
	m_hVbl.ptr = NULL;

	m_netSrcIp = 0;
	m_netDstIp = 0;

	//SNMPv1 trap;
	m_pEnterpriseOid = NULL;
	m_netTrapIp = 0;
	m_nGenericTrap = 0;
	m_nSpecificTrap = 0;
	m_nTimeTicks = 0;
}

/*******************************************************************************
*
* Function Name:		GetSrcIp
* Description:			
* Input Parameters:		
* Output Parameters:	return, ip
* Notes:				
*
/******************************************************************************/
SNMPAPI_STATUS snmp::GetSrcIp(char ip[16])
{
	char *p;

	p = inet_ntoa(*(in_addr *)&m_netSrcIp);
	if (p == NULL)
		return SNMPAPI_FAILURE;

	strcpy(ip, p);
	return SNMPAPI_SUCCESS;
}

/*******************************************************************************
*
* Function Name:		GetRequestId
* Description:			
* Input Parameters:		
* Output Parameters:	return
* Notes:				
*
/******************************************************************************/
smiINT32 snmp::GetRequestId()
{
	return m_nRequestId;
}

/*******************************************************************************
*
* Function Name:		GetEnterprise
* Description:			
* Input Parameters:		
* Output Parameters:	return
* Notes:				
*
/******************************************************************************/
smiLPOID snmp::GetEnterprise()
{
	return m_pEnterpriseOid;
}

/*******************************************************************************
*
* Function Name:		GetTrapIp
* Description:			
* Input Parameters:		
* Output Parameters:	return, ip
* Notes:				
*
/******************************************************************************/
SNMPAPI_STATUS snmp::GetTrapIp(char ip[])
{
	char *p;

	p = inet_ntoa(*(in_addr *)&m_netTrapIp);
	if (p == NULL)
		return SNMPAPI_FAILURE;

	strcpy(ip, p);
	return SNMPAPI_SUCCESS;
}

/*******************************************************************************
*
* Function Name:		GetGenericTrap
* Description:			
* Input Parameters:		
* Output Parameters:	return
* Notes:				
*
/******************************************************************************/
smiINT snmp::GetGenericTrap()
{
	return m_nGenericTrap;
}

/*******************************************************************************
*
* Function Name:		GetSpecificTrap
* Description:			
* Input Parameters:		
* Output Parameters:	return
* Notes:				
*
/******************************************************************************/
smiINT snmp::GetSpecificTrap()
{
	return m_nSpecificTrap;
}

/*******************************************************************************
*
* Function Name:		GetTimeTicks
* Description:			
* Input Parameters:		
* Output Parameters:	return
* Notes:				
*
/******************************************************************************/
smiUINT32 snmp::GetTimeTicks()
{
	return m_nTimeTicks;
}

/*******************************************************************************
*
* Function Name:		GetErrorStatus
* Description:			
* Input Parameters:		
* Output Parameters:	return
* Notes:				
*
/******************************************************************************/
smiINT snmp::GetErrorStatus()
{
	return m_nErrorStatus;
}

/*******************************************************************************
*
* Function Name:		GetPduType
* Description:			
* Input Parameters:		
* Output Parameters:	return
* Notes:				
*
/******************************************************************************/
smiINT snmp::GetPduType()
{
	return m_nPduType;
}
