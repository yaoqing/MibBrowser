

#ifndef _SNMPDEF_H_
#define _SNMPDEF_H_

#include <windows.h>

///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// WinSNMP API Type Definitions                                              //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

typedef HANDLE              HSNMP_SESSION,  FAR *LPHSNMP_SESSION;
typedef HANDLE              HSNMP_ENTITY,   FAR *LPHSNMP_ENTITY;
typedef HANDLE              HSNMP_CONTEXT,  FAR *LPHSNMP_CONTEXT;
typedef HANDLE              HSNMP_PDU,      FAR *LPHSNMP_PDU;
typedef HANDLE              HSNMP_VBL,      FAR *LPHSNMP_VBL;
typedef unsigned char       smiBYTE,        FAR *smiLPBYTE;
typedef const smiBYTE						FAR *smiLPCBYTE;
typedef signed long         smiINT,         FAR *smiLPINT;
typedef smiINT              smiINT32,       FAR *smiLPINT32;
typedef unsigned long       smiUINT32,      FAR *smiLPUINT32;
typedef struct {
    smiUINT32 len;
    smiLPBYTE ptr;
}                           smiOCTETS,      FAR *smiLPOCTETS;
typedef const smiOCTETS                     FAR *smiLPCOCTETS;
typedef smiOCTETS           smiBITS,        FAR *smiLPBITS;
typedef struct {
    smiUINT32   len;                        
    smiLPUINT32 ptr;
}                           smiOID,         FAR *smiLPOID;
typedef const smiOID                        FAR *smiLPCOID;
typedef smiOCTETS           smiIPADDR,      FAR *smiLPIPADDR;
typedef smiUINT32           smiCNTR32,      FAR *smiLPCNTR32;
typedef smiUINT32           smiGAUGE32,     FAR *smiLPGAUGE32;
typedef smiUINT32           smiTIMETICKS,   FAR *smiLPTIMETICKS;
typedef smiOCTETS           smiOPAQUE,      FAR *smiLPOPAQUE;
typedef smiOCTETS           smiNSAPADDR,    FAR *smiLPNSAPADDR;
typedef struct {
     smiUINT32 hipart;
     smiUINT32 lopart;
}                           smiCNTR64,      FAR *smiLPCNTR64;
    
typedef struct {       
    smiUINT32 syntax;  
    union {
    smiINT    sNumber;      // SNMP_SYNTAX_INT
                            // SNMP_SYNTAX_INT32 
    smiUINT32 uNumber;      // SNMP_SYNTAX_UINT32
                            // SNMP_SYNTAX_CNTR32
                            // SNMP_SYNTAX_GAUGE32
                            // SNMP_SYNTAX_TIMETICKS
    smiCNTR64 hNumber;      // SNMP_SYNTAX_CNTR64 
    smiOCTETS string;       // SNMP_SYNTAX_OCTETS
                            // SNMP_SYNTAX_BITS
                            // SNMP_SYNTAX_OPAQUE
                            // SNMP_SYNTAX_IPADDR
                            // SNMP_SYNTAX_NSAPADDR
    smiOID    oid;          // SNMP_SYNTAX_OID 
    smiBYTE   empty;        // SNMP_SYNTAX_NULL
                            // SNMP_SYNTAX_NOSUCHOBJECT
                            // SNMP_SYNTAX_NOSUCHINSTANCE
                            // SNMP_SYNTAX_ENDOFMIBVIEW 
    } value;      
}                           smiVALUE,       FAR *smiLPVALUE;
typedef const  smiVALUE                     FAR *smiLPCVALUE;

typedef smiBYTE				smiBER,			FAR *smiLPBER;
typedef const smiBER						FAR *smiLPCBER;

///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// ASN/BER Base Types                                                        //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

#define ASN_UNIVERSAL               (0x00)
#define ASN_APPLICATION             (0x40)
#define ASN_CONTEXT                 (0x80)
#define ASN_PRIVATE                 (0xC0)
#define ASN_PRIMITIVE               (0x00)
#define ASN_CONSTRUCTOR             (0x20)

///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// SNMP ObjectSyntax Values                                                  //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////
                                    
#define SNMP_SYNTAX_INT             (ASN_UNIVERSAL   | ASN_PRIMITIVE   | 0x02)
#define SNMP_SYNTAX_BITS            (ASN_UNIVERSAL   | ASN_PRIMITIVE   | 0x03)
#define SNMP_SYNTAX_OCTETS          (ASN_UNIVERSAL   | ASN_PRIMITIVE   | 0x04)
#define SNMP_SYNTAX_NULL            (ASN_UNIVERSAL   | ASN_PRIMITIVE   | 0x05)
#define SNMP_SYNTAX_OID             (ASN_UNIVERSAL   | ASN_PRIMITIVE   | 0x06)
#define SNMP_SYNTAX_INT32           SNMP_SYNTAX_INT

#define SNMP_SYNTAX_SEQUENCE        (ASN_UNIVERSAL   | ASN_CONSTRUCTOR | 0x10)

#define SNMP_SYNTAX_IPADDR          (ASN_APPLICATION | ASN_PRIMITIVE   | 0x00)
#define SNMP_SYNTAX_CNTR32          (ASN_APPLICATION | ASN_PRIMITIVE   | 0x01)
#define SNMP_SYNTAX_GAUGE32         (ASN_APPLICATION | ASN_PRIMITIVE   | 0x02)
#define SNMP_SYNTAX_TIMETICKS       (ASN_APPLICATION | ASN_PRIMITIVE   | 0x03)
#define SNMP_SYNTAX_OPAQUE          (ASN_APPLICATION | ASN_PRIMITIVE   | 0x04)
#define SNMP_SYNTAX_NSAPADDR        (ASN_APPLICATION | ASN_PRIMITIVE   | 0x05)
#define SNMP_SYNTAX_CNTR64          (ASN_APPLICATION | ASN_PRIMITIVE   | 0x06)
#define SNMP_SYNTAX_UINT32          (ASN_APPLICATION | ASN_PRIMITIVE   | 0x07)

#define SNMP_SYNTAX_NOSUCHOBJECT    (ASN_CONTEXT     | ASN_PRIMITIVE   | 0x00)
#define SNMP_SYNTAX_NOSUCHINSTANCE  (ASN_CONTEXT     | ASN_PRIMITIVE   | 0x01)
#define SNMP_SYNTAX_ENDOFMIBVIEW    (ASN_CONTEXT     | ASN_PRIMITIVE   | 0x02)

///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// SNMP Limits                                                               //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

#define MAXOBJIDSIZE     128    // Max number of components in an OID   
#define MAXOBJIDSTRSIZE  1408   // Max len of decoded MAXOBJIDSIZE OID   
#define MAXSNMPSIZE		 484	// Max len of snmp datagram
#define MAXUDPSIZE		 512	// Max len of udp datagram // Add by YaoQing

///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// PDU Type Values                                                           //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////
                                    
#define SNMP_PDU_GET                (ASN_CONTEXT | ASN_CONSTRUCTOR | 0x0)
#define SNMP_PDU_GETNEXT            (ASN_CONTEXT | ASN_CONSTRUCTOR | 0x1)
#define SNMP_PDU_RESPONSE           (ASN_CONTEXT | ASN_CONSTRUCTOR | 0x2)
#define SNMP_PDU_SET                (ASN_CONTEXT | ASN_CONSTRUCTOR | 0x3)
#define SNMP_PDU_V1TRAP             (ASN_CONTEXT | ASN_CONSTRUCTOR | 0x4)
#define SNMP_PDU_GETBULK            (ASN_CONTEXT | ASN_CONSTRUCTOR | 0x5)
#define SNMP_PDU_INFORM             (ASN_CONTEXT | ASN_CONSTRUCTOR | 0x6)
#define SNMP_PDU_TRAP               (ASN_CONTEXT | ASN_CONSTRUCTOR | 0x7)

///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// SNMPv1 Trap Values                                                        //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////
                                    
#define SNMP_TRAP_COLDSTART             0
#define SNMP_TRAP_WARMSTART             1
#define SNMP_TRAP_LINKDOWN              2
#define SNMP_TRAP_LINKUP                3
#define SNMP_TRAP_AUTHFAIL              4
#define SNMP_TRAP_EGPNEIGHBORLOSS       5
#define SNMP_TRAP_ENTERPRISESPECIFIC    6

///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// SNMP Error Codes Returned in Error_status Field of PDU                    //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

#define SNMP_ERROR_NOERROR              0
#define SNMP_ERROR_TOOBIG               1
#define SNMP_ERROR_NOSUCHNAME           2
#define SNMP_ERROR_BADVALUE             3
#define SNMP_ERROR_READONLY             4
#define SNMP_ERROR_GENERR               5
#define SNMP_ERROR_NOACCESS             6
#define SNMP_ERROR_WRONGTYPE            7
#define SNMP_ERROR_WRONGLENGTH          8
#define SNMP_ERROR_WRONGENCODING        9
#define SNMP_ERROR_WRONGVALUE           10
#define SNMP_ERROR_NOCREATION           11
#define SNMP_ERROR_INCONSISTENTVALUE    12
#define SNMP_ERROR_RESOURCEUNAVAILABLE  13
#define SNMP_ERROR_COMMITFAILED         14
#define SNMP_ERROR_UNDOFAILED           15
#define SNMP_ERROR_AUTHORIZATIONERROR   16
#define SNMP_ERROR_NOTWRITABLE          17
#define SNMP_ERROR_INCONSISTENTNAME     18

///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// Values used to indicate entity/context translation modes                  //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

#define SNMPAPI_TRANSLATED         0
#define SNMPAPI_UNTRANSLATED_V1    1
#define SNMPAPI_UNTRANSLATED_V2    2

///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// Values used to indicate "SNMP level" supported by the implementation      //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

#define SNMPAPI_NO_SUPPORT         0
#define SNMPAPI_V1_SUPPORT         1
#define SNMPAPI_V2_SUPPORT         2
#define SNMPAPI_M2M_SUPPORT        3

///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// Values used to indicate retransmit mode in the implementation             //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

#define SNMPAPI_OFF                0    // Refuse support 
#define SNMPAPI_ON                 1    // Request support 

///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// WinSNMP API Function Return Codes                                         //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

typedef smiUINT32 SNMPAPI_STATUS;       // Used for function ret values 

#define SNMPAPI_FAILURE            0    // Generic error code 
#define SNMPAPI_SUCCESS            1    // Generic success code

///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// WinSNMP API Error Codes (for SnmpGetLastError)                            //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

#define SNMPAPI_ALLOC_ERROR        2    // Error allocating memory 
#define SNMPAPI_CONTEXT_INVALID    3    // Invalid context parameter 
#define SNMPAPI_CONTEXT_UNKNOWN    4    // Unknown context parameter 
#define SNMPAPI_ENTITY_INVALID     5    // Invalid entity parameter 
#define SNMPAPI_ENTITY_UNKNOWN     6    // Unknown entity parameter 
#define SNMPAPI_INDEX_INVALID      7    // Invalid VBL index parameter 
#define SNMPAPI_NOOP               8    // No operation performed 
#define SNMPAPI_OID_INVALID        9    // Invalid OID parameter 
#define SNMPAPI_OPERATION_INVALID  10   // Invalid/unsupported operation 
#define SNMPAPI_OUTPUT_TRUNCATED   11   // Insufficient output buf len 
#define SNMPAPI_PDU_INVALID        12   // Invalid PDU parameter 
#define SNMPAPI_SESSION_INVALID    13   // Invalid session parameter 
#define SNMPAPI_SYNTAX_INVALID     14   // Invalid syntax in smiVALUE 
#define SNMPAPI_VBL_INVALID        15   // Invalid VBL parameter 
#define SNMPAPI_MODE_INVALID       16   // Invalid mode parameter 
#define SNMPAPI_SIZE_INVALID       17   // Invalid size/length parameter 
#define SNMPAPI_NOT_INITIALIZED    18   // SnmpStartup failed/not called 
#define SNMPAPI_MESSAGE_INVALID    19   // Invalid SNMP message format 
#define SNMPAPI_HWND_INVALID       20   // Invalid Window handle 
#define SNMPAPI_OTHER_ERROR        99   // For internal/undefined errors 
#define SNMPAPI_TL_NOT_INITIALIZED 100  // TL not initialized 
#define SNMPAPI_TL_NOT_SUPPORTED   101  // TL does not support protocol 
#define SNMPAPI_TL_NOT_AVAILABLE   102  // Network subsystem has failed 
#define SNMPAPI_TL_RESOURCE_ERROR  103  // TL resource error 
#define SNMPAPI_TL_UNDELIVERABLE   104  // Destination unreachable 
#define SNMPAPI_TL_SRC_INVALID     105  // Source endpoint invalid 
#define SNMPAPI_TL_INVALID_PARAM   106  // Input parameter invalid 
#define SNMPAPI_TL_IN_USE          107  // Source endpoint in use 
#define SNMPAPI_TL_TIMEOUT         108  // No response before timeout 
#define SNMPAPI_TL_PDU_TOO_BIG     109  // PDU too big for send/receive 
#define SNMPAPI_TL_OTHER           199  // Undefined TL error 

///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// WinSNMP API Function Prototypes                                           //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

#define IN
#define OUT
#define SNMPAPI_CALL WINAPI



//#define	WM_HOST		WM_USER + 1200
//#define	WM_COMM		WM_USER + 1201
//#define WM_NTD		WM_USER + 1202

/*typedef LRESULT (CALLBACK *SNMPAPI_CALLBACK)(
							HWND hWnd,
                            UINT wMsg,
                            WPARAM wParam,
                            LPARAM lParam
                            //IN LPVOID lClientpData
                            );*/
void			FreePtr(smiLPBER &p);
void			FreeOid(smiLPOID &oid);
void			FreeVbl(smiLPBER &vbl);
void			FreePdu(smiLPBER &pdu);
void			FreeOctets(smiLPOCTETS &oct);
void			FreeValue(smiLPVALUE &val);
void			valtostr(smiLPCVALUE val, char str[MAXOBJIDSTRSIZE], smiUINT32 &strlen, bool &null, smiUINT32 &type);
void			oidtostr(smiLPCOID oid, char str[MAXOBJIDSTRSIZE]);
SNMPAPI_STATUS	strtooid(const char *str, smiLPOID &oid);
smiUINT32		oidatindex(smiLPCOID oid, int index);

BOOL			SnmpStartup();
void			SnmpCleanup();
HANDLE			SnmpCreateSession(/*HWND hWnd, UINT message, SNMPAPI_CALLBACK fCallback*/);
void			SnmpDestorySession(HANDLE hSnmp);
LRESULT CALLBACK SnmpCallback(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

/*void			SnmpReceive(UINT message, WPARAM wParam, LPARAM lParam);*/

void SysDebugInfo(const char *info);

#endif