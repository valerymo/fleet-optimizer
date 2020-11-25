// RouterL.h: interface for the CRouterL class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ROUTERL_H__217172D0_0200_4E56_893A_3D78006241A1__INCLUDED_)
#define AFX_ROUTERL_H__217172D0_0200_4E56_893A_3D78006241A1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Router.h"
#include "Line.h"

class CRouterL : public CRouter  
{
friend class CBasePlanSt2LpDp;
friend class COptimizationSt2LpDp;
public:
	int GetRouterTime();
	void appendLine(CLine*);
	CLine * GetLine(void);
	void GetRouter();
	CRouterL(CPort *pBasePort, int PNB_num, int cargo_type);
	virtual ~CRouterL();
	CRouterL* GetNextRouter();

protected:
    CRouterL *next;

private:
	int CopyDataFromRouter(CRouterL* SourceRouterL);
    CLine *LinesList;
	CLine* at_endLine;


};

#endif // !defined(AFX_ROUTERL_H__217172D0_0200_4E56_893A_3D78006241A1__INCLUDED_)
