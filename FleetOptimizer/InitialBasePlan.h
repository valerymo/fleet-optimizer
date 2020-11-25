// InitialBasePlan.h: interface for the CInitialBasePlan class.
//
//////////////////////////////////////////////////////////////////////
#include "Router.h"

#if !defined(AFX_InitialBasePlan_H__F664AA30_C08F_4F91_B44B_1FA7A0FF91FC__INCLUDED_)
#define AFX_InitialBasePlan_H__F664AA30_C08F_4F91_B44B_1FA7A0FF91FC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class COutputView;

class CInitialBasePlan  
{
public:
	void print();
	CRouter * GetRoutersListHead();
	static int CreateInitialBasePlan();
    static CInitialBasePlan* Instance();
   	void append(CRouter *);
protected:
    CInitialBasePlan();
	//virtual ~CInitialBasePlan();
private:
    static CInitialBasePlan* _instance;
   	CRouter *RoutersList;
   	CRouter *InitialBasePlan;
	CRouter *at_end;
	COutputView *m_pOutView;
};

#endif // !defined(AFX_InitialBasePlan_H__F664AA30_C08F_4F91_B44B_1FA7A0FF91FC__INCLUDED_)
