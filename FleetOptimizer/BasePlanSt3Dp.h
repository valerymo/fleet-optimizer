// BasePlanSt3Dp.h: interface for the CBasePlanSt3Dp class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BASEPLANST3DP_H__D972FF3F_17F1_46E4_AD40_4B68CD62F193__INCLUDED_)
#define AFX_BASEPLANST3DP_H__D972FF3F_17F1_46E4_AD40_4B68CD62F193__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "BasePlan.h"
#include "RouterP.h"

class CBasePlanSt3Dp : public CBasePlan  
{
public:
	CRouterP* GetRoutersListHead();
	void append(CRouterP *);
	int CreateBasePlanInit();
	int CreateBasePlanInit(int param);
	void print();
	static CBasePlanSt3Dp* Instance();

protected:	
	CBasePlanSt3Dp();
	virtual ~CBasePlanSt3Dp();
private:
	static	CBasePlanSt3Dp* _instance;
   	CRouterP* m_RoutersList;
   	CRouterP* m_BasePlanSt3Dp;
	CRouterP* m_at_end;
	COutputView* m_pOutView;

};

#endif // !defined(AFX_BASEPLANST3DP_H__D972FF3F_17F1_46E4_AD40_4B68CD62F193__INCLUDED_)
