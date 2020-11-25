// RouterP.h: interface for the CRouterP class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ROUTERP_H__BEEA0695_DDEB_4AE3_A528_3BCE6955B63C__INCLUDED_)
#define AFX_ROUTERP_H__BEEA0695_DDEB_4AE3_A528_3BCE6955B63C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Router.h"
#include "PortPoint.h"

class CRouterP : public CRouter  
{
friend class CBasePlanSt3Dp;
friend class COptimizationSt3Dp;
public:
	int GetCargoLoadInPlanForPNB(int PNB_num, int cargo_type);
	int GetRouterTimeFinal();
	void normShipLoadingPlan();
	int GetCargoDoneOnRouterForPNB(int PNB_num, int cargo_type);
	int GetPPointsCount();
	int GetRouterTime();
	CPortPoint* GetPPointsListEnd();
	CRouterP* GetNextRouter();
	void GetRouter();
	CRouterP(CPort *pBasePort, int PNB_num, int cargo_type);
	virtual ~CRouterP();
	CPortPoint* GetPPoitsListHead();
	void appendPPoint(CPortPoint *pPPoint);


protected:
    CRouterP *next;

private:
	CPortPoint* m_PPointsList;
	CPortPoint* m_at_endPPoint;
	//CShipCargoPlan* m_pShipCargoPlan ;

	int m_ShipLoadingPlan[MAXPNBNUMBER][5];


};

#endif // !defined(AFX_ROUTERP_H__BEEA0695_DDEB_4AE3_A528_3BCE6955B63C__INCLUDED_)
