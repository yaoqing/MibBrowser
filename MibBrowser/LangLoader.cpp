// LangLoader.cpp: implementation of the CLangLoader class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "mibbrowser.h"
#include "LangLoader.h"
#include "StringEx.h"
#include "StringToken.h"
#include "CSGuard.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

CLangLoader * CLangLoader::instance = 0;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CLangLoader::CLangLoader()
{
	InitializeCriticalSection(&m_cs);
}

CLangLoader::~CLangLoader()
{
	DeleteCriticalSection(&m_cs);
}

CLangLoader * CLangLoader::GetInstance()
{
	if (!instance)
	{
		instance = new CLangLoader();
		instance->EnumLangRes();
		if (!instance->InitStringRes(instance->GetDefaultLanguage()))
			instance->InitStringRes(1033);
	}
	return instance;
}

CString CLangLoader::LoadString(CString key)
{
	CCSGuard guard(&m_cs);
	CString value;
	string_res.Lookup(key, value);
	return value;
}

void CLangLoader::EnumLangRes()
{
	HANDLE handle;
	WIN32_FIND_DATA	ffd;
	handle = FindFirstFile(_T(".\\lang\\*.lng"), &ffd);
	if( handle == INVALID_HANDLE_VALUE )
	{
		FindClose(handle);
		return;
	}

	int lang_id;
	TCHAR lang_name[256];
	do
	{
		CStringToken token(ffd.cFileName, '.');
		lang_id = _ttoi(token.GetAt(0));
		if (lang_id > 0)
		{
			if (GetLocaleInfo(lang_id, LOCALE_SNATIVELANGNAME, lang_name, 256))
				lang_res.SetAt(lang_id, lang_name);
		}
	} while (FindNextFile(handle, &ffd));
	FindClose(handle);
}

LANGID CLangLoader::GetDefaultLanguage()
{
	return GetUserDefaultLangID();
}

bool CLangLoader::InitStringRes(LANGID lang_id)
{
	CCSGuard guard(&m_cs);
	m_lang_id = lang_id;
	TCHAR file[256];
	_stprintf(file, _T(".\\lang\\%u.lng"), lang_id);
	
	order.RemoveAll();
	string_res.RemoveAll();
	
	CFile fd;
	CFileException ex;
	if (!fd.Open(file, CFile::modeRead, &ex))
	{
		return false;
	}

	DWORD result, offset = 0;
	DWORD length = fd.GetLength();
	char *content = new char[length+1];
	while (result = fd.Read(content+offset, length-offset))
	{
		offset += result;
	}

	CStringToken token('=');
	content[length] = 0; offset = 0;
	char *prev, *next, ch = '\n';
	prev = next = content;
	while (offset < length)
	{
		next = strchr(prev, ch);
		if (next == NULL)
		{
			if (ch == '\n')
				ch = '\r';
			else if (ch == '\r')
				ch = 0;
			else
				break;
			continue;
		}

		*next = 0;
		offset += next - prev + 1;
		if (ch == '\n' && *(next-1) == '\r')
			*(next-1) = 0;

		token.SetString(prev);
		if (token.CountToke() > 1)
		{
			order.Add(token.GetAt(0));
			string_res.SetAt(token.GetAt(0), token.GetAt(1));
		}
		else if (token.CountToke() == 1)
			order.Add(token.GetAt(0));
		else
			order.Add(CString());
		
		prev = next+1;
	}

	delete content;
	fd.Close();
	return true;
}

void CLangLoader::SaveToFile()
{
	CCSGuard guard(&m_cs);
	TCHAR file[256];
	_stprintf(file, _T(".\\lang\\%u.lng"), m_lang_id);

	CFile fd;
	CFileException ex;
	if (!fd.Open(file, CFile::modeCreate | CFile::modeWrite, &ex))
	{
		return;
	}

	char *p, *q, ch = '=';
	char crlf[2] = {0xD, 0xA};
	CString key, value;
	CStringEx key1, value1;
	for (int i = 0; i < order.GetSize(); i++)
	{
		value.Empty();
		key = order.GetAt(i);
		string_res.Lookup(key, value);

		key1 = key;
		value1 = value;
		p = key1.toChar();
		q = value1.toChar();

		fd.Write(p, strlen(p));
		if (value.GetLength() > 0)
		{
			fd.Write(&ch, 1);
			fd.Write(q, strlen(q));
		}
		
		fd.Write(crlf, 2);
		delete [] p;
		delete [] q;		
	}
	
	fd.Close();
}

void CLangLoader::GetLangID(CArray<int, int> &array)
{
	int key;
	CString value;
	array.RemoveAll();
	POSITION pos = lang_res.GetStartPosition();
	while (pos)
	{
		key = 0;
		value.Empty();
		lang_res.GetNextAssoc(pos, key, value);
		array.Add(key);
	}
}

CString CLangLoader::GetLangName(int lang_id)
{
	CString value;
	lang_res.Lookup(lang_id, value);
	return value;
}

int CLangLoader::GetCurrLangID()
{
	return m_lang_id;
}