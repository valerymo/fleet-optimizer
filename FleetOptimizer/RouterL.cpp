// RouterL.cpp: implementation of the CRouterL class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "FleetOptimizer.h"
#include "RouterL.h"
#include <math.h>


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CRouterL::CRouterL(CPort *pBasePort, int PNB_num, int cargo_type)
{

     basePort = pBasePort;
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

     CLine *pLine = new CLine(PNB_num, cargoType);
	 pLine->SetCount(1);
     LinesList = pLine;
	 at_endLine = pLine;
     next = 0;

}

CRouterL::~CRouterL()
{

}

void CRouterL::GetRouter()
{

}

CLine * CRouterL::GetLine()
{
	return LinesList;
}

void CRouterL::appendLine(CLine *pLine)
{
    if(CRouterL::LinesList == 0)
        LinesList = pLine;
    else
        at_endLine->next = pLine;
            
    at_endLine = pLine;
}


CRouterL* CRouterL::GetNextRouter()
{
	return next;
}

int CRouterL::GetRouterTime()
{
	int OperationDaysInYear;
	int PNB_num, PNB_id, DistancePortPNB, nLineCount, cargo_type;
	float LoadTimeDays, LoadTimeDaysAll, RouterTime, TripTimeDays;
	CVehicle *pVehicle;
	CPort *pBasePort;
	CLine *pLine;

	pVehicle = GetVehicle();
	pBasePort = GetBasePort();
	pLine = GetLine();

	DistancePortPNB = 0;
	LoadTimeDays = 1;  //stab
	LoadTimeDaysAll = 0;

	RouterTime = 0;

	while(pLine){
		PNB_num = pLine->GetPNBnum();
		PNB_id = pBasePort->GetPNB(PNB_num).id;
		nLineCount = pLine->GetCount();
		cargo_type = pLine->GetCargoType();
	    DistancePortPNB = pBasePort->GetDistancePortPNB(PNB_id);
		LoadTimeDays = pBasePort->GetPNBShipLoadingTime(PNB_num, pVehicle, cargo_type);
		TripTimeDays = ((2*(float)DistancePortPNB/pVehicle->GetSpeed())/24)+ LoadTimeDays ;
		RouterTime += TripTimeDays*nLineCount;
		pLine = pLine->GetNextLine();
	}
		
	OperationDaysInYear = CFleetTechEconomicEng::Instance()->GetVehicleOperationsDaysInYear(pVehicle->GetRegisterClass(), pBasePort->GetPortRegion());
	//if (daysRouterTime > OperationDaysInYear)
	//	return OperationDaysInYear;

	return (int)ceil(RouterTime);
}

int CRouterL::CopyDataFromRouter(CRouterL *SourceRouterL)
{
	CLine *pSourceLine;	
	int PNB_num, LineCount, nCargoType;
	
	basePort = SourceRouterL->basePort;
	cargoType = SourceRouterL->cargoType;
	next = SourceRouterL->next;
	vehicle = SourceRouterL->vehicle;
	vehicleNumber = SourceRouterL->vehicleNumber;
	dVehicleNumber = SourceRouterL->dVehicleNumber;
	vehicleIncome = SourceRouterL->vehicleIncome;
	vehicleOperationsDaysRemain = SourceRouterL->vehicleOperationsDaysRemain;

	pSourceLine = SourceRouterL->LinesList;
	PNB_num = pSourceLine->GetPNBnum();

	nCargoType = pSourceLine->GetCargoType();
	switch(nCargoType){
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

	CLine *pLine = new CLine(PNB_num, cargoType);
	LineCount = pSourceLine->GetCount();
	pLine->SetCount(LineCount);
    LinesList = pLine;
	at_endLine = pLine;
    //next = 0;

	pSourceLine = pSourceLine->GetNextLine();
	while(pSourceLine){
		PNB_num = pSourceLine->GetPNBnum();
		nCargoType = pSourceLine->GetCargoType();
		switch(nCargoType){
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

		CLine *pNewLine = new CLine(PNB_num, cargoType);
		LineCount = pSourceLine->GetCount();
		pNewLine->SetCount(LineCount);
		appendLine(pNewLine);
		pSourceLine = pSourceLine->GetNextLine();
	}

	return 1;
}
