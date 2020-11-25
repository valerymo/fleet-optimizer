// BasePlanSt2LpDp.h: interface for the CBasePlanSt2LpDp class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BASEPLANST2LPDP_H__7F6C1A38_2BA3_4349_8DC4_1DAF29618388__INCLUDED_)
#define AFX_BASEPLANST2LPDP_H__7F6C1A38_2BA3_4349_8DC4_1DAF29618388__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "RouterL.h"
#include "BasePlan.h"


class CBasePlanSt2LpDp : public CBasePlan  
{
friend COptimizationSt2LpDp::SimplexMethod();
public:
	CRouterL * GetRoutersListHead();
	void print(void);
	static CBasePlanSt2LpDp* Instance();
   	void append(CRouterL *);
	//CRouter * GetRoutersListHead();
	int CreateBasePlanInit(void);
	


protected:
	CBasePlanSt2LpDp();
	virtual ~CBasePlanSt2LpDp();
   	CRouterL *RoutersList;

private:
	static	CBasePlanSt2LpDp* _instance;

   	CRouterL *BasePlanSt2LpDp;
	CRouterL *at_end;
	CRouterL * DuplicatePlan(CRouter *pBasePlan);
	CRouterL * DuplicateConvertPlan(CRouter *pBasePlan);
	COutputView* m_pOutView;

};

#endif // !defined(AFX_BASEPLANST2LPDP_H__7F6C1A38_2BA3_4349_8DC4_1DAF29618388__INCLUDED_)
