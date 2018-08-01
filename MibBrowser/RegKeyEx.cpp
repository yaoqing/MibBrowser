// RegKeyEx.cpp: implementation of the CRegKeyEx class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "RegKeyEx.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CRegKeyEx::CRegKeyEx()
{

}

CRegKeyEx::~CRegKeyEx()
{

}

HKEY CRegKeyEx::OpenReg(CString szSubKey)
{
	HKEY hKey;
	LONG rc = RegOpenKeyEx(HKEY_LOCAL_MACHINE, szSubKey, 0, KEY_ALL_ACCESS, &hKey);
	if (rc == ERROR_SUCCESS)
		return hKey;

	DWORD dwDisposition;
	rc = RegCreateKeyEx(HKEY_LOCAL_MACHINE, szSubKey, 0, NULL, REG_OPTION_NON_VOLATILE,
                KEY_ALL_ACCESS, NULL, &hKey, &dwDisposition);
	if (rc == ERROR_SUCCESS)
		return hKey;

	return (HKEY)INVALID_HANDLE_VALUE;
}

void CRegKeyEx::CloseReg(HKEY hKey)
{
	RegCloseKey(hKey);
}

void CRegKeyEx::SetKeyValue(HKEY hKey, CString szKeyName, CString szKeyValue)
{
	RegSetValueEx(hKey, szKeyName, 0, REG_SZ, (BYTE *)(LPCTSTR)szKeyValue, sizeof(TCHAR)*szKeyValue.GetLength());
}

CString CRegKeyEx::GetKeyValue(HKEY hKey, CString szKeyName)
{
	DWORD dwType, dwSize = MAX_PATH;
	TCHAR szKeyValue[MAX_PATH];
	szKeyValue[0] = 0;
	LONG rc = RegQueryValueEx(hKey, szKeyName, NULL, &dwType, (LPBYTE)szKeyValue, &dwSize);
	if (rc != ERROR_SUCCESS)
		RegSetValueEx(hKey, szKeyName, 0, REG_SZ, (BYTE *)szKeyValue, 0);
	return szKeyValue;	
}

void CRegKeyEx::DeleteReg(CString szSubKey)
{
	LONG rc = 0;
	TCHAR szName[MAX_PATH];
	TCHAR szValue[MAX_PATH];
	TCHAR szData[MAX_PATH];
	DWORD dwName, dwValue, dwData, dwType;

	HKEY hSubKey;
	rc = RegOpenKeyEx(HKEY_LOCAL_MACHINE, szSubKey, 0, KEY_ALL_ACCESS, &hSubKey);
	if (rc != ERROR_SUCCESS)
		return;
	
	while (TRUE)
	{
		dwName = MAX_PATH;
		rc = RegEnumKeyEx(hSubKey, 0, szName, &dwName, NULL, NULL, NULL, NULL);
		if (rc != ERROR_SUCCESS)
		{
			while (TRUE)
			{
				dwValue = dwData = MAX_PATH;
				rc = RegEnumValue(hSubKey, 0, szValue, &dwValue, NULL, &dwType, (LPBYTE)szData, &dwData);
				if (rc == ERROR_SUCCESS)
					RegDeleteValue(hSubKey, szValue);
				else
					break;
			}
			break;
		}
		
		if (szSubKey.GetAt(szSubKey.GetLength()-1) != _T('\\'))
			szSubKey += _T('\\');
		szSubKey += szName;
		
		DeleteReg(szSubKey);
		rc = RegDeleteKey(hSubKey, szName);
	}

	CloseReg(hSubKey);
}