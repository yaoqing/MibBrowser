// snmp.h: interface for the snmp class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SNMP_H__F2EBEA5D_6BE0_4794_9DD0_E5F4F1AECADD__INCLUDED_)
#define AFX_SNMP_H__F2EBEA5D_6BE0_4794_9DD0_E5F4F1AECADD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "snmpdef.h"
#pragma comment(lib, "Ws2_32.lib")

class snmp
{
public:
	snmp();
	~snmp();
	static smiINT32 m_sRequestId;
	
private:
	SOCKET			m_sock;

	smiINT			m_nVer;
	smiINT			m_nPduType;
	smiINT32		m_nRequestId;
	smiINT			m_nErrorStatus;
	smiINT			m_nErrorIndex;

//	smiLPBER		m_pBuf;

	smiOCTETS		m_hBuf;
	smiOCTETS		m_hCommunity;
	smiOCTETS		m_hPdu;
	smiOCTETS		m_hVbl;

	smiUINT32		m_netSrcIp;
	smiUINT32		m_netDstIp;

	//SNMPv1 trap
	smiLPOID		m_pEnterpriseOid;
	smiUINT32		m_netTrapIp;
	smiINT			m_nGenericTrap;
	smiINT			m_nSpecificTrap;
	smiUINT32		m_nTimeTicks;

public:
	smiINT			GetPduType();
	smiINT			GetErrorStatus();
	smiINT			GetSpecificTrap();
	smiINT			GetGenericTrap();
	smiINT32		GetRequestId();
	smiUINT32		GetTimeTicks();
	smiLPOID		GetEnterprise();

	void			FreeResource();
	void			SetHandle(HANDLE handle);
	void			SetTimeout(smiINT timeout);
	SNMPAPI_STATUS	GetSrcIp(char ip[16]);
	SNMPAPI_STATUS	GetTrapIp(char ip[16]);
	SNMPAPI_STATUS	CreateVbl(smiLPCOID const oid, smiLPCVALUE const val);
	SNMPAPI_STATUS	CreatePdu(smiINT pdu_type, smiINT32 request_id, smiINT error_status, smiINT error_index);
	SNMPAPI_STATUS	SendMsg(/*const char *srcIp, */const char *dstIp, smiINT ver, smiINT port, smiLPOCTETS community);
	SNMPAPI_STATUS	RecvMsg();
	SNMPAPI_STATUS  GetPduData();
	SNMPAPI_STATUS	CountVbl();
	SNMPAPI_STATUS	GetVb(smiUINT32 index, smiLPOID &oid, smiLPVALUE &val);

private:
	long			bertonum(smiLPCBER ber, long len, BOOL sign);
	long			numtober(char buf[32], long num);
	long			numtooid(char buf[32], smiUINT32 num);

	smiUINT32		GetLiLen(smiUINT32 num);
	smiUINT32		GetLiVal(smiLPCBER ber, smiLPUINT32 len);
	smiUINT32		SetLi(smiLPBER ber, smiUINT32 val);
	smiUINT32		GetSmiOIDLen(smiLPCOID const oid);
	smiUINT32		GetVbLen(smiLPCOID const oid, smiUINT32 *oidlen, smiLPCVALUE const val, smiUINT32 *vallen, smiUINT32 *vbvallen);
	smiUINT32		GetSmiVALUELen(smiLPCVALUE const val);
	SNMPAPI_STATUS	GetSmiOID(smiLPCBER ber, smiUINT32 len, smiLPOID &oid);
	SNMPAPI_STATUS	GetSmiVALUE(smiLPCBER ber, smiUINT32 len, smiBYTE tag, smiLPVALUE &val);

	smiUINT32		FillVb(smiLPBER ber, smiLPCOID const oid, smiUINT32 oidlen, smiLPCVALUE const val, smiUINT32 vallen, smiUINT32 vbvallen);
};

#endif // !defined(AFX_SNMP_H__F2EBEA5D_6BE0_4794_9DD0_E5F4F1AECADD__INCLUDED_)
