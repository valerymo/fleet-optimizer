// Optimizer.h: interface for the COptimizer class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_OPTIMIZER_H__EDDCE1EA_F649_4790_8C76_3EFD6AFB5AFC__INCLUDED_)
#define AFX_OPTIMIZER_H__EDDCE1EA_F649_4790_8C76_3EFD6AFB5AFC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "InitialBasePlan.h"
#include "BasePlanSt1LP.h"
#include "Scheduler.h"
#include "PortFactory.h"
#include "ShipFactory.h"
#include "PortList.h"
#include "VehiclesList.h"
#include "FleetTechEconomicEng.h"
#include "OptimizationSt1LP.h"
#include "OptimizationSt2LpDp.h"
#include "OptimizationSt3Dp.h"



class COptimizer  
{
public:
    static COptimizer* Instance();   
protected:
    COptimizer();
    virtual ~COptimizer();
private:
    static COptimizer* _instance;

private:
	COptimizationSt2LpDp* COptimizationSt2LpDp;
	COptimizationSt1LP* OptimizationSt1LP;
    CInitialBasePlan*  InitialBasePlan;
    CBasePlanSt1LP*		BasePlanSt1LP;
    CScheduler*     Scheduler;
    CShipFactory*   ShipFactory;
    CPortFactory*   PortFactory;
    CPortList*      PortList;
    CVehiclesList*  VehiclesList;
    CFleetTechEconomicEng* FleetTechEconomicEng;
};

#endif // !defined(AFX_OPTIMIZER_H__EDDCE1EA_F649_4790_8C76_3EFD6AFB5AFC__INCLUDED_)
