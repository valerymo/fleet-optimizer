// BasePlanSt1LP.h: interface for the CBasePlanSt1LP class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BASEPLANST1LP_H__95EFF6C6_3FB1_488B_B08C_4A268F9FE72C__INCLUDED_)
#define AFX_BASEPLANST1LP_H__95EFF6C6_3FB1_488B_B08C_4A268F9FE72C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "RouterS.h"
#include "BasePlan.h"

//class COutputView;

class CBasePlanSt1LP : public CBasePlan   
{
public:
	void print();
	static CBasePlanSt1LP* Instance();
   	void append(CRouter *);
	CRouter * GetRoutersListHead();
	int CreateBasePlanInit(void);


protected:
	CBasePlanSt1LP();
	virtual ~CBasePlanSt1LP();

private:
	static	CBasePlanSt1LP* _instance;
   	CRouter *RoutersList;
   	CRouter *BasePlanSt1LP;
	CRouter *at_end;
};

#endif // !defined(AFX_BASEPLANST1LP_H__95EFF6C6_3FB1_488B_B08C_4A268F9FE72C__INCLUDED_)
