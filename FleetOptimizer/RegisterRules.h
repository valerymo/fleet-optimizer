// RegisterRules.h: interface for the CRegisterRules class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_REGISTERRULES_H__A79F785F_9C25_4045_A03A_1FD33BBFC649__INCLUDED_)
#define AFX_REGISTERRULES_H__A79F785F_9C25_4045_A03A_1FD33BBFC649__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CRegisterRules  
{
public:
	static CRegisterRules* Instance();
protected:
	CRegisterRules();
	virtual ~CRegisterRules();
private:
    static CRegisterRules* _instance;

};


#endif // !defined(AFX_REGISTERRULES_H__A79F785F_9C25_4045_A03A_1FD33BBFC649__INCLUDED_)
