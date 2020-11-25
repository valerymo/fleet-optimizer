// Optimizer.cpp: implementation of the COptimizer class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "FleetOptimizer.h"
#include "Optimizer.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

COptimizer* COptimizer::_instance = 0;

COptimizer* COptimizer::Instance(){
    if (_instance == 0){
        _instance = new COptimizer;
    }
    return _instance;
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

COptimizer::COptimizer()
{
    //CInitialBasePlan::Instance(); 
    InitialBasePlan->Instance();
    BasePlanSt1LP->Instance();
    Scheduler->Instance();
    PortList->Instance();
    PortFactory->Instance();
    VehiclesList->Instance();
    ShipFactory->Instance();
    FleetTechEconomicEng->Instance();
	OptimizationSt1LP->Instance();
}

COptimizer::~COptimizer()
{

}

