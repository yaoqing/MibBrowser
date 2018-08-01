// MibLoader.h: interface for the CMibLoader class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MIBLOADER_H__C030D9C1_C107_42FF_A62E_C9D6F69802D4__INCLUDED_)
#define AFX_MIBLOADER_H__C030D9C1_C107_42FF_A62E_C9D6F69802D4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CMibSyntax
{
public:
	CString *SYNTAX;
	CString *ACCESS;
	CString *STATUS;
	CString *DESCRIPTION;
	CString *INDEX;
	CMibSyntax():SYNTAX(0),ACCESS(0),STATUS(0),DESCRIPTION(0),INDEX(0){}
	~CMibSyntax()
	{
		if (SYNTAX) delete SYNTAX;
		if (ACCESS) delete ACCESS;
		if (STATUS) delete STATUS;
		if (DESCRIPTION) delete DESCRIPTION;
		if (INDEX) delete INDEX;
	}
};

class CMibNode
{
public:
	int node_id;
	CString node_name;
	CMibSyntax *node_syntax;
	CMibNode* parent_node;
	CMap<int, int, CMibNode*, CMibNode*> *child_level;

	CMibNode();
	~CMibNode();
	CMibNode(int id, CString name);
	CMibNode(int id, CString name, CMibSyntax *syntax);

	CMibNode *InsertNode(int id, CString name, CMibSyntax *syntax, CMibNode *node);
	CMibNode *FindChild(const char *index, int &pos);
};

class CStringToken;
class CMibLoader
{
public:
	CMibLoader();
	virtual ~CMibLoader();
	
	BOOL LoadMib(LPCTSTR mib_file);
	BOOL Analyse(CArray<CString, char *> &lines);

	BOOL IsImportsEnd(CString &line);
	CMibNode *GetRoot();
	BOOL HasDef(CString &key, CString &value);

protected:
	CMibNode *root;
	CMap<CString, CString&, CString, CString&> map_def;
	CMap<CString, CString&, CMibNode *, CMibNode *> map_node;

	CString SubToken(CStringToken *token, int start, int end);
};

#endif // !defined(AFX_MIBLOADER_H__C030D9C1_C107_42FF_A62E_C9D6F69802D4__INCLUDED_)
