// Router.cpp: implementation of the CRouter class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "FleetOptimizer.h"
#include "Router.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CRouter::CRouter()
{

}

CRouter::~CRouter()
{

}


CVehicle* CRouter::GetVehicle() const
{
    return vehicle;
}

void CRouter::GetRouter()
{
    
}

int CRouter::GetVehicleIncome()
{
    return vehicleIncome;
}

void CRouter::SetVehicle(CVehicle * pVehicle)
{
   vehicle = pVehicle;
}

//DEL void CRouter::SetBasePort(CPort *pBasePort)
//DEL  {
//DEL      basePort = pBasePort;
//DEL  }

 void CRouter::SetCargoType(int cargo_type)
 {
     switch(cargo_type){
     case 1:
         cargoType = CCargo::Coal;
         break;
     case 2:
         cargoType = CCargo::General;
         break;
     case 3:
         cargoType = CCargo::Oil;
         break;
     case 4:
         cargoType = CCargo::Vegetables_fruits;
         break;
     }
 }


int CRouter::GetCargoType()
{
    return cargoType;
}

CPort* CRouter::GetBasePort() const
{
    return basePort;
}


int CRouter::GetVehicleNumber()
{
    return vehicleNumber;
}

double CRouter::GetDVehicleNumber()
{
    return dVehicleNumber;
}

/*
int CRouter::GetVehicleCargoForNavigation()
{
	return vehicleCargoForNavigation;
}
*/

CRouter* CRouter::GetNextRouter()
{
	return next;
}


int CRouter::GetVehicleOperationsDaysRemain()
{
	return vehicleOperationsDaysRemain;
}

void CRouter::SetVehicleOperationsDaysRemain(int days_remain)
{
	vehicleOperationsDaysRemain = days_remain;
}

int CRouter::GetRouterTime()
{
	int daysRouterTime;

	daysRouterTime = CFleetTechEconomicEng::Instance()->GetVehicleOperationsDaysInYear(vehicle->GetRegisterClass(), basePort->GetPortRegion());

	return daysRouterTime;

}

void CRouter::appendLine(CLine *pLine){

}

CLine * CRouter::GetLine()
{
	return Line; //stab; not in use
}