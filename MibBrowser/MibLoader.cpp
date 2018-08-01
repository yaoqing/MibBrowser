// MibLoader.cpp: implementation of the CMibLoader class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MibBrowser.h"
#include "MibLoader.h"
#include "StringToken.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


CMibNode::CMibNode()
{
	node_id = -1;
	node_syntax = 0;
	parent_node = 0;
	child_level = 0;
}

CMibNode::CMibNode(int id, CString name)
{
	node_id = id;
	node_name = name;
	node_syntax = 0;
	parent_node = 0;
	child_level = 0;
}

CMibNode::CMibNode(int id, CString name, CMibSyntax *syntax)
{
	node_id = id;
	node_name = name;
	node_syntax = syntax;
	parent_node = 0;
	child_level = 0;
}

CMibNode::~CMibNode()
{
	if (node_syntax) delete node_syntax;
	if (child_level)
	{
		CMibNode *node;
		POSITION pos = child_level->GetStartPosition();
		while (pos)
		{
			child_level->GetNextAssoc(pos, node_id, node);
			delete node;
		}
		child_level->RemoveAll();
		delete child_level;
	}
}

CMibNode * CMibNode::InsertNode(int id, CString name, CMibSyntax *syntax, CMibNode *node)
{
	if (!node)
		return NULL;
	
	if (!child_level)
		child_level = new CMap<int, int, CMibNode*, CMibNode*>();
	
	CMibNode *n;
	if (child_level->Lookup(id, n))
	{
		if (n->parent_node == this)
		{
			if (!node->child_level)
				node->child_level = new CMap<int, int, CMibNode*, CMibNode*>();
			
			int key;
			CMibNode *value;
			POSITION pos = n->child_level->GetStartPosition();
			while (pos)
			{
				n->child_level->GetNextAssoc(pos, key, value);
				value->parent_node = node;
				node->child_level->SetAt(key, value);
			}

			n->child_level->RemoveAll();
			delete n;
		}
		else
		{
			return NULL;
		}
	}
	
	child_level->SetAt(id, node);
	node->node_id = id;
	node->node_name = name;
	if (node->node_syntax)
		delete node->node_syntax;
	node->node_syntax = syntax;
	node->parent_node = this;
	
	return node;
}

CMibNode *CMibNode::FindChild(const char *index, int &pos)
{
	pos = 0;
	smiLPOID oid;
	if (strtooid(index, oid) == SNMPAPI_FAILURE)
		return NULL;
	CMibNode *node = this;

	while (true)
	{
		for (DWORD i = 0; i < oid->len; i++)
		{
			if (!node)
				break;
			if (!node->child_level)
				break;
			if (!node->child_level->Lookup(oid->ptr[i], node))
				break;
		}
		char buff[16];
		for (DWORD n = 0; n < i; n++)
		{
			sprintf(buff, "%u", oid->ptr[n]);
			pos += strlen(buff)+1;
		}
		pos--;
		FreeOid(oid);
		return node;
	}

	FreeOid(oid);
	return NULL;
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMibLoader::CMibLoader()
{
	CString name = _T("root");
	root = new CMibNode(0, name);
	map_node.SetAt(name, root);

	name = _T("ccitt");
	CMibNode *ccitt = root->InsertNode(0, name, NULL, new CMibNode());
	map_node.SetAt(name, ccitt);

	name = _T("null");
	CMibNode *null = ccitt->InsertNode(0, name, NULL, new CMibNode());
	map_node.SetAt(name, null);
	null->child_level = new CMap<int, int, CMibNode*, CMibNode*>();

	name = _T("iso");
	CMibNode *iso = root->InsertNode(1, name, NULL, new CMibNode());
	map_node.SetAt(name, iso);

	name = _T("org");
	CMibNode *org = iso->InsertNode(3, name, NULL, new CMibNode());
	map_node.SetAt(name, org);

	name = _T("dod");
	CMibNode *dod = org->InsertNode(6, name, NULL, new CMibNode());
	map_node.SetAt(name, dod);

	name = _T("internet");
	CMibNode *internet = dod->InsertNode(1, name, NULL, new CMibNode());
	map_node.SetAt(name, internet);

	name = _T("directory");
	CMibNode *directory = internet->InsertNode(1, name, NULL, new CMibNode());
	map_node.SetAt(name, directory);
	directory->child_level = new CMap<int, int, CMibNode*, CMibNode*>();

	name = _T("mgmt");
	CMibNode *mgmt = internet->InsertNode(2, name, NULL, new CMibNode());
	map_node.SetAt(name, mgmt);
	mgmt->child_level = new CMap<int, int, CMibNode*, CMibNode*>();

	name = _T("experimental");
	CMibNode *experimental = internet->InsertNode(3, name, NULL, new CMibNode());
	map_node.SetAt(name, experimental);
	experimental->child_level = new CMap<int, int, CMibNode*, CMibNode*>();

	name = _T("private");
	CMibNode *private_node = internet->InsertNode(4, name, NULL, new CMibNode());
	map_node.SetAt(name, private_node);

	name = _T("enterprises");
	CMibNode *enterprises = private_node->InsertNode(1, name, NULL, new CMibNode());
	map_node.SetAt(name, enterprises);
	enterprises->child_level = new CMap<int, int, CMibNode*, CMibNode*>();
}

CMibLoader::~CMibLoader()
{
	delete root;
}

BOOL CMibLoader::LoadMib(LPCTSTR mib_file)
{
	CFile fd;
	CFileException ex;
	if (!fd.Open(mib_file, CFile::modeRead, &ex))
	{
		TCHAR szError[1024];
		ex.GetErrorMessage(szError, 1024);
		AfxMessageBox(szError);
		return FALSE;
	}

	CString line;
	DWORD result, offset = 0;
	DWORD length = fd.GetLength();
	char *content = new char[length+1];
	while (result = fd.Read(content+offset, length-offset))
	{
		offset += result;
	}

	//CFile fd1;
	//fd1.Open(mib_file+3, CFile::modeCreate | CFile::modeWrite);

	content[length] = 0; offset = 0;
	CArray<CString, char *> lines;
	char *prev, *next, *p, ch = '\n';
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

		if (*prev)
		{
			p = prev;
			while (p = strchr(p, '-'))
			{
				if (*(p+1) == '-')
				{
					*p = 0;
					break;
				}
				p++;
			}

			for (int i = strlen(prev)-1; i >= 0; i--)
			{
				if (prev[i] == ' ' || prev[i] == '\t')
					prev[i] = 0;
				else
					break;
			}

			p = prev;
/*			while (*p)
			{
				if (*p == ' ' || *p == '\t')
					p++;
				else
					break;
			}
*/
			if (*p)
			{
				lines.Add(p);
				//fd1.Write(p, strlen(p));
				//fd1.Write("\r\n", 2);
			}
		}
		
		prev = next+1;
	}

	delete content;
	fd.Close();
	//fd1.Close();
	
	return Analyse(lines);
}


BOOL CMibLoader::Analyse(CArray<CString, char *> &lines)
{
	CStringToken token;
	CString &line = lines.GetAt(0);
	
	int pos0, pos1, pos2, pos3;
	token.SetString(line);
	if (!token.HasKey(_T("DEFINITIONS"), pos1))
		return FALSE;
	if (!token.HasKey(_T("::="), pos2))
		return FALSE;
	if (!token.HasKey(_T("BEGIN"), pos3))
		return FALSE;
	if (pos1 != 1 && pos2 != 2 && pos3 != 3 && token.CountToke() != 4)
		return FALSE;

	line = lines.GetAt(lines.GetSize()-1);
	token.SetString(line);
	if (!token.HasKey(_T("END"), pos0))
		return FALSE;
	if (pos0 != 0 && token.CountToke() != 1)
		return FALSE;

	lines.RemoveAt(0);
	lines.RemoveAt(lines.GetSize()-1);

	int i = 0;
	CArray<int, int> beDel;
	line = lines.GetAt(i);
	token.SetString(line);
	if (token.HasKey(_T("IMPORTS"), pos0))
	{
		do
		{
			beDel.Add(i);
			line = lines.GetAt(i++);
			if (IsImportsEnd(line))
				break;
		} while(i < lines.GetSize());
	}

	for (i = beDel.GetSize()-1; i >= 0; i--)
		lines.RemoveAt(beDel.GetAt(i));
	beDel.RemoveAll();

	i = 0;
	CString temp, name;
	while (TRUE)
	{
		temp.Empty();

		for (; i < lines.GetSize(); i++)
		{
			line = lines.GetAt(i);
			token.SetString(line);
			temp += line;
			
			if (token.HasKey(_T("::="), pos0))
				break;
			else
				continue;
		}

		for (i++; i < lines.GetSize(); i++)
		{
			line = lines.GetAt(i);
			token.SetString(line);
			
			if (token.HasKey(_T("::="), pos0) || token.HasKey(_T("OBJECT-TYPE"), pos0))
				break;
			else
			{
				temp += line;
				continue;
			}
		}

		token.SetString(temp);
		if (!token.HasKey(_T("::="), pos0))
			return FALSE;

		if (i >= lines.GetSize())
			break;

		if (token.CountToke() > pos0+1)
		{
			if (token.GetAt(pos0+1) != _T("{"))
			{
				temp.Empty();
				for (int c = 0; ; c++)
				{
					temp += token.GetAt(c);
					if (c < token.CountToke()-1)
						temp += ' ';
					else
						break;
				}
				map_def.SetAt(token.GetAt(0), temp);
			}
			else
			{
				if (token.CountToke()-1 != pos0 + 4)
					return FALSE;

				if (token.GetAt(pos0+4) != _T("}"))
					return FALSE;

				temp = token.GetAt(pos0+2);
				int index = _ttoi(token.GetAt(pos0+3));

				CMibNode *node = NULL;
				if (!map_node.Lookup(temp, node))
				{
					CString msg;
					msg.Format(LoadAsString(_T("mib.unknow.node"), _T("Unknow node %s, load MIB library fail!")), temp);
					AfxMessageBox(msg);
					return FALSE;
				}

				name = token.GetAt(0);
				node = node->InsertNode(index, name, NULL, new CMibNode());
				if (!node)
				{
					CString msg;
					msg.Format(LoadAsString(_T("mib.node.exist"), _T("node %s has exist, load MIB library fail!")), name);
					AfxMessageBox(msg);
					return FALSE;
				}

				if (token.HasKey(_T("OBJECT-TYPE"), pos1) && pos1 == 1)
				{
					if (!token.HasKey(_T("SYNTAX"), pos0))
						return FALSE;
					if (!token.HasKey(_T("ACCESS"), pos1))
						return FALSE;
					CMibSyntax *syntax = new CMibSyntax();
					syntax->SYNTAX = new CString(SubToken(&token, pos0, pos1));

					pos0 = pos1;
					if (!token.HasKey(_T("STATUS"), pos1))
					{
						delete syntax;
						return FALSE;
					}
					syntax->ACCESS = new CString(SubToken(&token, pos0, pos1));

					pos0 = pos1;
					if (!token.HasKey(_T("DESCRIPTION"), pos1))
					{
						delete syntax;
						return FALSE;
					}
					syntax->STATUS = new CString(SubToken(&token, pos0, pos1));

					pos0 = pos1;
					if (token.HasKey(_T("INDEX"), pos1))
					{
						temp = SubToken(&token, pos0, pos1);
						temp.Remove('"');
						syntax->DESCRIPTION = new CString(temp);

						pos0 = pos1;
						token.HasKey(_T("::="), pos1);
						CStringToken st(SubToken(&token, pos0, pos1));
						if (st.HasKey(_T("{"), pos2) && st.HasKey(_T("}"), pos3))
							syntax->INDEX = new CString(SubToken(&st, pos2, pos3));
						else
							syntax->INDEX = new CString(SubToken(&token, pos0, pos1));
					}
					else
					{
						token.HasKey(_T("::="), pos1);
						temp = SubToken(&token, pos0, pos1);
						temp.Remove('"');
						syntax->DESCRIPTION = new CString(temp);
					}

					node->node_syntax = syntax;
				}
				else
				{
					token.HasKey(_T("::="), pos0);
					if (token.GetAt(pos0-2) == _T("OBJECT") && token.GetAt(pos0-1) == _T("IDENTIFIER"))
						if (!node->child_level)	node->child_level = new CMap<int, int, CMibNode*, CMibNode*>();
				}
								
				map_node.SetAt(name, node);
			}
		}
		else
			return FALSE;
	}

	return TRUE;
}

BOOL CMibLoader::IsImportsEnd(CString &line)
{
	line.Replace('\t', ' ');
	CStringToken token;
	token.SetString(line);

	int pos;
	if (!token.HasKey(_T("FROM"), pos))
		return FALSE;
	
	if (pos+1 == token.CountToke())
		return FALSE;
	
	CString value = token.GetAt(pos+1);
	if (value.Find(';') == -1)
		return FALSE;
		
	return TRUE;
}

CMibNode *CMibLoader::GetRoot()
{
	return root;
}

BOOL CMibLoader::HasDef(CString &key, CString &value)
{
	return map_def.Lookup(key, value);
}

CString CMibLoader::SubToken(CStringToken *token, int start, int end)
{
	CString temp;
	for (int i = start+1; i < end; i++)
	{
		temp += token->GetAt(i);
		if (i >= end-1) break;
		temp += ' ';
	}
	return temp;
}