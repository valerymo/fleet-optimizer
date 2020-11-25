// BasePlan.h: interface for the CBasePlan class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BASEPLAN_H__172EFF6B_D178_464D_9E2A_78D0707C4263__INCLUDED_)
#define AFX_BASEPLAN_H__172EFF6B_D178_464D_9E2A_78D0707C4263__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CBasePlan  
{
public:
	CBasePlan();
	virtual ~CBasePlan();

	virtual void print(void) = 0;
	//virtual void append(CRouter *) = 0;
	virtual int CreateBasePlanInit(void) = 0;

	CRouter * GetRoutersListHead();

private:
   	CRouter *RoutersList;
	CRouter *at_end;

};

#endif // !defined(AFX_BASEPLAN_H__172EFF6B_D178_464D_9E2A_78D0707C4263__INCLUDED_)
