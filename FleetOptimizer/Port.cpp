// Port.cpp: implementation of the CPort class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "FleetOptimizer.h"
#include "Port.h"
#include "Cargo.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

IMPLEMENT_SERIAL(CPort,CObject,1)

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPort::CPort()
{
    next = NULL;
    for (int i = 0; i < MAXPNBNUMBER; i++){
        PNBList[i].id = 999;
	}
	LinesNumber = 0;


	//Land PP
    for (int i = 0; i < MAXPNBNUMBER; i++){
		PNBList[i].LandStructDataPPVirt.bACVCourtAvailable = 0;
		PNBList[i].LandStructDataPPVirt.bShipCourtAvailable = 0;
		PNBList[i].LandStructDataPPVirt.nACVCourtInvestRequere = 0;
		PNBList[i].LandStructDataPPVirt.nShipCourtInvestRequere = 0; 
		PNBList[i].LandStructDataPPVirt.bACVCourtUse = 0;
		PNBList[i].LandStructDataPPVirt.bShipCourtUse = 0;

		PNBList[i].LandStructDataPP.bACVCourtUse = 0;
		PNBList[i].LandStructDataPP.bShipCourtUse = 0;
	}

	//Land, virtual:
	m_LandStructDataVirt.nACVSpace = 0;
	m_LandStructDataVirt.nACVSpaceElemPrice = 0;
	m_LandStructDataVirt.nMoorLength = 0;
	m_LandStructDataVirt.nMoorElemPrice = 0;
	m_LandStructDataVirt.RepairDetailsACV.nCapability = 0;
	m_LandStructDataVirt.RepairDetailsACV.nVhehicleLoad = 0;
	m_LandStructDataVirt.RepairDetailsACV.a = 0;
	m_LandStructDataVirt.RepairDetailsACV.b = 0;
	m_LandStructDataVirt.RepairDetailsShip.nCapability = 0;
	m_LandStructDataVirt.RepairDetailsShip.nVhehicleLoad = 0;
	m_LandStructDataVirt.RepairDetailsShip.a = 0;
	m_LandStructDataVirt.RepairDetailsShip.b = 0;

	//Land, tmp:
	m_LandStructDataTmp.nACVSpace = 0;
	m_LandStructDataTmp.nACVSpaceElemPrice = 0;
	m_LandStructDataTmp.nMoorLength = 0;
	m_LandStructDataTmp.nMoorElemPrice = 0;
	m_LandStructDataTmp.RepairDetailsACV.nCapability = 0;
	m_LandStructDataTmp.RepairDetailsACV.nVhehicleLoad = 0;
	m_LandStructDataTmp.RepairDetailsACV.a = 0;
	m_LandStructDataTmp.RepairDetailsACV.b = 0;
	m_LandStructDataTmp.RepairDetailsShip.nCapability = 0;
	m_LandStructDataTmp.RepairDetailsShip.nVhehicleLoad = 0;
	m_LandStructDataTmp.RepairDetailsShip.a = 0;
	m_LandStructDataTmp.RepairDetailsShip.b = 0;


}

CPort::~CPort()
{

}



CPort::PNB_data CPort::GetPNB(int i){
    return PNBList[i];  
}

char * CPort::GetPortName()
{
    return name;
}

CPort * CPort::GetNextPort()
{
    return next;
}



int CPort::GetAvailableCargo(int cargo_type)
{
    int cargo;
    
    switch(cargo_type) 
    {
    case 1 :
            cargo = available_cargoes.coal;
            break;
    case 2 :
            cargo = available_cargoes.general;
            break;
    case 3 :
            cargo = available_cargoes.oil;
            break;
    case 4 :
            cargo = available_cargoes.vegetables_fruits;
    }
    
    return cargo;

}

void CPort::GetAvailableCargoesAll (CCargo * availableCargoes)
{
    availableCargoes->coal = available_cargoes.coal;
    availableCargoes->general = available_cargoes.general;
    availableCargoes->oil = available_cargoes.oil;
    availableCargoes->vegetables_fruits = available_cargoes.vegetables_fruits;
}

int CPort::GetPNBRequiredCargo(int i, int cargo_type)
{
    int cargo;
    
    switch(cargo_type) 
    {
    case 1 :
            cargo = PNBList[i].required_cargoes.coal;
            break;
    case 2 :
            cargo = PNBList[i].required_cargoes.general;
            break;
    case 3 :
            cargo = PNBList[i].required_cargoes.oil;
            break;
    case 4 :
            cargo = PNBList[i].required_cargoes.vegetables_fruits;
            break;
    }
    
    return cargo;

}

//DEL char * CPort::GetPNBNameByID(int PNB_id)
//DEL {
//DEL     for (int i = 0; i<=MAXPNBNUMBER; i++){
//DEL         if (PNBList[i].id == PNB_id)
//DEL             return PNBList[i].name;
//DEL     }
//DEL     return "ERROR";
//DEL }

int CPort::GetDistancePortPNB(int PNB_id)
{
    //return  DistanceMatrix[0][PNB_id];
	if (PNB_id == id)
		return 0;
    return  DistanceMatrix[id][PNB_id];
}

char* CPort::GetPortRegion()
{
	return Region;
}

int CPort::GetDistancePNBPNB(int PNB_number1, int PNB_number2)
{
    if (PNB_number1 == PNB_number2)
		return 0;
	else
		return  DistanceMatrix[PNB_number1][PNB_number2];
}

int CPort::GetLinesNumber()
{
	return LinesNumber;
}

int CPort::SetPNBRequiredCargoRemain(int i, int cargo_type, int cargo)
{
  
    switch(cargo_type) 
    {
    case 1 :
            PNBList[i].required_cargoes.coal = cargo;
            break;
    case 2 :
            PNBList[i].required_cargoes.general = cargo;
            break;
    case 3 :
            PNBList[i].required_cargoes.oil = cargo;
            break;
    case 4 :
            PNBList[i].required_cargoes.vegetables_fruits = cargo;
            break;
    }
	
	return 1;

}

int CPort::GetPNBRequiredCargoFirst(int i, int cargo_type)
{
    int cargo;
    
    switch(cargo_type) 
    {
    case 1 :
            cargo = PNBList[i].required_cargoes_0.coal;
            break;
    case 2 :
            cargo = PNBList[i].required_cargoes_0.general;
            break;
    case 3 :
            cargo = PNBList[i].required_cargoes_0.oil;
            break;
    case 4 :
            cargo = PNBList[i].required_cargoes_0.vegetables_fruits;
            break;
    }
    
    return cargo;
}

int CPort::is_CargoRequired()
{
	int num, id, cargo_type, RequiredCargo;

	for (num = 0; num < MAXPNBNUMBER; num++){
		id = GetPNB(num).id;
		if (id == 999)
			continue;
		for (cargo_type = 1; cargo_type <= 4; cargo_type++){
			RequiredCargo = GetPNBRequiredCargo(num,cargo_type);
			if (RequiredCargo > 0)
				return 1;	
		}
	}

	return 0;
}

void CPort::RestoreCargoRequerements()
{
	int id;
	for (int i = 0; i < MAXPNBNUMBER; i++){
		id = GetPNB(i).id;
		if (id == 999)
			continue;
		PNBList[i].required_cargoes.coal = PNBList[i].required_cargoes_0.coal;
		PNBList[i].required_cargoes.general = PNBList[i].required_cargoes_0.general;
		PNBList[i].required_cargoes.oil = PNBList[i].required_cargoes_0.oil;
		PNBList[i].required_cargoes.vegetables_fruits = PNBList[i].required_cargoes_0.vegetables_fruits;
	}
}

int CPort::SetPNBRequiredCargoRemain0(int i, int cargo_type, int cargo)
{
    switch(cargo_type) 
    {
    case 1 :
            PNBList[i].required_cargoes_0.coal = cargo;
            break;
    case 2 :
            PNBList[i].required_cargoes_0.general = cargo;
            break;
    case 3 :
            PNBList[i].required_cargoes_0.oil = cargo;
            break;
    case 4 :
            PNBList[i].required_cargoes_0.vegetables_fruits = cargo;
            break;
    }
	
	return 1;

}
/*
void CPort::Serialize(CArchive &ar)
{

}
*/

float CPort::GetPNBShipLoadingTime(int PNB_num, CVehicle *pVehicle, int cargo_type)
{
	int loading_productivity, ShipLoad;
	float loading_time;

	ShipLoad = pVehicle->GetLoad();
	
	//производительность погрузочно-разгрузочных работ в пункте (т/час)
	loading_productivity = GetPNBShipLoadingProductivity(PNB_num, pVehicle, cargo_type);
//	if(loading_productivity <= 0){
//		loading_productivity = 5; //значение по умолчанию. 
		//Вычисления не прерываются, но выдается сообщение об ощибке
		//AfxMessageBox("Ошибка вычисления производительность погрузочно-разгрузочных");
//	}
	
	loading_time = (float)ShipLoad/loading_productivity; //час
	loading_time = loading_time/24; //days

	loading_time = (float)1.3*loading_time;	// учет времени простоя в порту без грузовых операций 

/* //for debug
    FILE *stream;   
    stream = fopen( "E:\\Users\\mv\\Log\\GetPNBShipLoadingTime.txt", "a" );
    fprintf (stream,"ShipLoad: %d, loading_productivity: %d\n", ShipLoad, loading_productivity);
	fclose(stream);
*/


	if (loading_time < 1)
		loading_time = 1;
	
	//return (int)loading_time;
	//return 1.0; 
	return loading_time; 
}

float CPort::GetPNBShipLoadingTime(int PNB_num, CVehicle *pVehicle, int cargo_type, int load)
{
	int loading_productivity;
	double loading_time;

	if (load <= 0) //debug
		return 1;


	//производительность погрузочно-разгрузочных работ в пункте (т/час)
	loading_productivity = GetPNBShipLoadingProductivity(PNB_num, pVehicle, cargo_type);

	if (loading_productivity <= 0) //debug
		return 1;

	
	loading_time = (double)load/loading_productivity; //час
	loading_time = loading_time/24; //days

	loading_time = (double)1.3*loading_time;	// учет времени простоя в порту без грузовых операций 

	if (loading_time < 1)
		loading_time = 1;
	
	return (float)loading_time;
}

int CPort::GetPNBShipLoadingProductivity(int PNB_num, CVehicle *pVehicle, int cargo_type)
{
	int loading_productivity;
	CString strRegisterClass;

	strRegisterClass = pVehicle->GetRegisterClass();
	if (strRegisterClass.Find("*ACV*") != -1){ //ACV
		loading_productivity = PNBList[PNB_num].acv_loading_productivity[cargo_type];
	}
	else{
		loading_productivity = PNBList[PNB_num].ship_loading_productivity[cargo_type];
	}

	return loading_productivity;
}

void CPort::CleanLandDataUpdateStructure()
{
    for (int i = 0; i < MAXPNBNUMBER; i++){
		PNBList[i].LandStructDataPPVirt.bACVCourtAvailable = 0;
		PNBList[i].LandStructDataPPVirt.bShipCourtAvailable = 0;
		PNBList[i].LandStructDataPPVirt.nACVCourtInvestRequere = 0;
		PNBList[i].LandStructDataPPVirt.nShipCourtInvestRequere = 0; 

		PNBList[i].LandStructDataPPTmp.bACVCourtAvailable = 0;
		PNBList[i].LandStructDataPPTmp.bShipCourtAvailable = 0;
		PNBList[i].LandStructDataPPTmp.nACVCourtInvestRequere = 0;
		PNBList[i].LandStructDataPPTmp.nShipCourtInvestRequere = 0; 

    }

	m_LandStructDataVirt.nACVSpace = 0;
	m_LandStructDataVirt.nACVSpaceElemPrice = 0;
	m_LandStructDataVirt.nMoorLength = 0;
	m_LandStructDataVirt.nMoorElemPrice = 0;
	m_LandStructDataVirt.RepairDetailsACV.nCapability = 0;
	m_LandStructDataVirt.RepairDetailsACV.nVhehicleLoad = 0;
	m_LandStructDataVirt.RepairDetailsACV.a = 0;
	m_LandStructDataVirt.RepairDetailsACV.b = 0;
	m_LandStructDataVirt.RepairDetailsShip.nCapability = 0;
	m_LandStructDataVirt.RepairDetailsShip.nVhehicleLoad = 0;
	m_LandStructDataVirt.RepairDetailsShip.a = 0;
	m_LandStructDataVirt.RepairDetailsShip.b = 0;

	m_LandStructDataVirt.nACVSpace = 0;
	m_LandStructDataVirt.nACVSpaceElemPrice = 0;
	m_LandStructDataVirt.nMoorLength = 0;
	m_LandStructDataVirt.nMoorElemPrice = 0;
	m_LandStructDataVirt.RepairDetailsACV.nCapability = 0;
	m_LandStructDataVirt.RepairDetailsACV.nVhehicleLoad = 0;
	m_LandStructDataVirt.RepairDetailsACV.a = 0;
	m_LandStructDataVirt.RepairDetailsACV.b = 0;
	m_LandStructDataVirt.RepairDetailsShip.nCapability = 0;
	m_LandStructDataVirt.RepairDetailsShip.nVhehicleLoad = 0;
	m_LandStructDataVirt.RepairDetailsShip.a = 0;
	m_LandStructDataVirt.RepairDetailsShip.b = 0;


}

int CPort::GetPortID()
{
	return id;
}

float CPort::GetPortShipLoadingTime(CVehicle* pVehicle, int cargo_type)
{
	int loading_productivity, ShipLoad;
	CString strRegisterClass;
	float loading_time;

	strRegisterClass = pVehicle->GetRegisterClass();

	//производительность погрузочно-разгрузочных работ в порту (т/час)
	if (strRegisterClass.Find("*ACV*") != -1){ //ACV
		loading_productivity = acv_loading_productivity[cargo_type];
	}
	else{
		loading_productivity = ship_loading_productivity[cargo_type];
	}


	ShipLoad = pVehicle->GetLoad();
	
	loading_time = (float)ShipLoad/loading_productivity; //час
	loading_time = loading_time/24; //days

	loading_time = (float)1.3*loading_time;	// учет времени простоя в порту без грузовых операций 
	
	if (loading_time < 1)
		loading_time = 1;

	return loading_time;

}

float CPort::GetPortShipLoadingTime(CVehicle* pVehicle, int cargo_type, int load)
{
	int loading_productivity;
	CString strRegisterClass;
	float loading_time;

	if (load == 0) //debug
		return 1;

	strRegisterClass = pVehicle->GetRegisterClass();

	//производительность погрузочно-разгрузочных работ в порту (т/час)
	if (strRegisterClass.Find("*ACV*") != -1){ //ACV
		loading_productivity = acv_loading_productivity[cargo_type];
	}
	else{
		loading_productivity = ship_loading_productivity[cargo_type];
	}


	loading_time = (float)load/loading_productivity; //час
	loading_time = loading_time/24; //days

	loading_time = (float)1.3*loading_time;	// учет времени простоя в порту без грузовых операций 
	
	if (loading_time < 1)
		loading_time = 1;

	return loading_time;

}
