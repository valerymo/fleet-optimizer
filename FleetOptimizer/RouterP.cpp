// RouterP.cpp: implementation of the CRouterP class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "FleetOptimizer.h"
#include "RouterP.h"
#include <math.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CRouterP::CRouterP(CPort *pBasePort, int PNB_num, int cargo_type)
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

     CPortPoint *pPortPoint = new CPortPoint(PNB_num, cargoType);
     m_PPointsList = pPortPoint;
	 m_at_endPPoint = pPortPoint;
     next = 0;


	// План загрузки судна на маршруте 
	int PNB_ID, cargo_num; //cargo_num 1...4; 0 -не используется
	for (PNB_ID = 0; PNB_ID < MAXPNBNUMBER; PNB_ID++)
		for (cargo_num = 0; cargo_num <= 4; cargo_num++)
			m_ShipLoadingPlan[PNB_ID][cargo_num] = 0;

}

CRouterP::~CRouterP()
{

}

void CRouterP::GetRouter()
{

}

//DEL void CRouterP::appendLine(CLine *pLine)
//DEL {
//DEL 	Line = pLine; //stab; not in use
//DEL }

//DEL CLine * CRouterP::GetLine()
//DEL {
//DEL 	return Line; //stab; not in use
//DEL }

CPortPoint* CRouterP::GetPPoitsListHead()
{
	return m_PPointsList;
}

CRouterP* CRouterP::GetNextRouter()
{
	return next;
}

void CRouterP::appendPPoint(CPortPoint *pPPoint)
{
    if(CRouterP::m_PPointsList == 0)
        m_PPointsList = pPPoint;
    else
        m_at_endPPoint->next = pPPoint;
            
    m_at_endPPoint = pPPoint;
}

CPortPoint* CRouterP::GetPPointsListEnd()
{
	return m_at_endPPoint;
}

int CRouterP::GetRouterTime()
{
	int PNB_num, PNB_num_prev, PNB_id, PNB_id_prev, 
		cargo_type, cargo_type_prev;
	CVehicle *pVehicle;
	CPort *pBasePort;
	CPortPoint* pPPoint;
	int Distance, TripTimeDays;
	float LoadTimeDays, LoadTimeDaysAll, LoadTimeDaysPort;
	int load;

	if (m_PPointsList == 0){
		return 0;
	}

	pVehicle = GetVehicle();
	pBasePort = GetBasePort();
	pPPoint = m_PPointsList;
	PNB_num = pPPoint->GetPNBnum();
	if (PNB_num == -1)
		PNB_id = pBasePort->GetPortID();
	else
		PNB_id = pBasePort->GetPNB(PNB_num).id;
	
	LoadTimeDaysAll = 0.0;

	Distance = pBasePort->GetDistancePortPNB(PNB_id);
	//LoadTime += 0.3; //stab. TBD
	cargo_type = pPPoint->GetCargoType();
	//load = m_ShipLoadingPlan[PNB_num][cargo_type];  //14.05.05
	load = m_ShipLoadingPlan[PNB_id][cargo_type]; 
	//LoadTimeDaysAll += pBasePort->GetPNBShipLoadingTime(PNB_id, pVehicle, cargo_type);
	//LoadTimeDaysPort = pBasePort->GetPortShipLoadingTime(pVehicle, cargo_type);
	LoadTimeDaysPort = pBasePort->GetPortShipLoadingTime(pVehicle, cargo_type, load);
	LoadTimeDaysAll += LoadTimeDaysPort;
	LoadTimeDaysAll += pBasePort->GetPNBShipLoadingTime(PNB_id, pVehicle, cargo_type, load);
	pPPoint = pPPoint->GetNextPPoint();
	
	while(pPPoint){
		PNB_num_prev = PNB_num;
		PNB_id_prev = PNB_id;
		cargo_type_prev = cargo_type;
		PNB_num = pPPoint->GetPNBnum();
		if (PNB_num == -1)
			PNB_id = pBasePort->GetPortID();
		else
			PNB_id = pBasePort->GetPNB(PNB_num).id;
		cargo_type = pPPoint->GetCargoType();
		if ((PNB_id_prev == PNB_id)&& (cargo_type_prev == cargo_type )){  
					// Тот же пункт. Судно возвращается за грузом в базовый порт
			Distance += 2*pBasePort->GetDistancePortPNB(PNB_id);
			LoadTimeDaysAll += LoadTimeDaysPort;
		}
		else{
			Distance += pBasePort->GetDistancePNBPNB(PNB_id_prev,PNB_id);
		}
		
		if (PNB_num != -1){
			//load = m_ShipLoadingPlan[PNB_num][cargo_type]; //14.05.05
			load = m_ShipLoadingPlan[PNB_id][cargo_type];
			//LoadTimeDays = pBasePort->GetPNBShipLoadingTime(PNB_id, pVehicle, cargo_type);
			LoadTimeDays = pBasePort->GetPNBShipLoadingTime(PNB_id, pVehicle, cargo_type, load);
			LoadTimeDaysAll += LoadTimeDays;
		}

		pPPoint = pPPoint->GetNextPPoint();

		if ((PNB_num == -1) && (pPPoint != NULL)){
			LoadTimeDaysAll += LoadTimeDaysPort;
		}

	}
	
	TripTimeDays = (int)ceil((((double)Distance/pVehicle->GetSpeed())/24) + LoadTimeDaysAll);

	return TripTimeDays;

}

int CRouterP::GetPPointsCount()
{
	CPortPoint* pPPoint;

	if (m_PPointsList == 0)
		return 0;

	pPPoint = m_PPointsList;
	int count = 0;

	while(pPPoint){
		count++;
		pPPoint = pPPoint->GetNextPPoint();
	}

	return count;
}

int CRouterP::GetCargoDoneOnRouterForPNB(int PNB_num, int Cargo_Type)
{
	
	int VehicleLoad, pnb_cargo_type,
		pnb_n, cargo_done, pbb_requered_cargo, cargoLoadInShipPlan ;

	cargo_done = 0;

	VehicleLoad = vehicle->GetLoad();
	CPortPoint* pPPoint = GetPPoitsListHead();
	while(pPPoint){
		pnb_n = pPPoint->GetPNBnum();

		if (pnb_n != PNB_num){
			pPPoint = pPPoint->GetNextPPoint();
			continue;
		}
		pnb_cargo_type = pPPoint->GetCargoType();
		if (pnb_cargo_type != Cargo_Type){
			pPPoint = pPPoint->GetNextPPoint();
			continue;
		}
		
		pbb_requered_cargo = this->GetBasePort()->GetPNBRequiredCargo(PNB_num,Cargo_Type);
		//if (pbb_requered_cargo < VehicleLoad)
		//	cargo_done += pbb_requered_cargo;
		//else
		//	cargo_done += VehicleLoad;
		cargoLoadInShipPlan = pPPoint->GetCargoLoadInShipPlan(); //количество груза на судне для данного пункта на маршруте
		cargo_done += cargoLoadInShipPlan;

				
		pPPoint = pPPoint->GetNextPPoint();
	}

	return cargo_done;
}


void CRouterP::normShipLoadingPlan()
{
// Формируется окончательный вариант загрузки судна, на базе предварительного плана загрузки.
// Предварительный план загрузки судна был сформирован 
// на этапе оптимизации - построения оптимального маршрута,
// по критерию кратчайшего пути при обходе пунктов в течении эксплуатационного периода.
// Возможна "перегрузка" судна, из-за особенностей алгоритма построения маршрута.
// Функция normShipLoadingPlan() предназначена для проверки и корректировки описанной проблемы.

	CVehicle *pVehicle;
	int VehicleLoad, exraLoad, cargoInPlan;
	float load_tmp;

	
	pVehicle = GetVehicle();
	VehicleLoad = pVehicle->GetLoad();

	//Проверка количества груза на судне на маршруте:
	int PNB_ID, cargo_num; //cargo_num 1...4; 0 -не используется
	cargoInPlan = 0;
	for (PNB_ID = 0; PNB_ID < MAXPNBNUMBER; PNB_ID++)
		for (cargo_num = 0; cargo_num <= 4; cargo_num++)
			cargoInPlan += m_ShipLoadingPlan[PNB_ID][cargo_num];

	if(cargoInPlan <= VehicleLoad) //судно не перегружено
		return;

	exraLoad = cargoInPlan - VehicleLoad;
	int fl = 0;
	for (PNB_ID = 0; PNB_ID < MAXPNBNUMBER; PNB_ID++){
		if (fl == 1)
			break;
		for (cargo_num = 0; cargo_num <= 4; cargo_num++){
			load_tmp = ((float)m_ShipLoadingPlan[PNB_ID][cargo_num])/2;
			if (exraLoad < load_tmp){
				m_ShipLoadingPlan[PNB_ID][cargo_num] -= exraLoad;
				fl = 1;
				break;
			}
			else {
				m_ShipLoadingPlan[PNB_ID][cargo_num] -= (int)load_tmp;
				exraLoad -= (int)load_tmp;
			}
		}
	}

}

int CRouterP::GetRouterTimeFinal()
{
	int PNB_num, PNB_num_prev, PNB_id, PNB_id_prev, 
		cargo_type, cargo_type_prev;
	CVehicle *pVehicle;
	CPort *pBasePort;
	CPortPoint* pPPoint;
	int Distance;
	double TripTimeDays, TripTimeHours;
	float LoadTimeDays, LoadTimeDaysAll, LoadTimeDaysPort;
	int load;

    //FILE *stream;
    // stream = fopen("TestFile1.txt", "a" );
	//fprintf (stream,"************************\n");
	//fprintf (stream,"************************\n");


	if (m_PPointsList == 0){
			MessageBox(NULL,"3-этап оптимизации: GetRouterTime()","Ошибка (1) вычисления времени выполнения маршрута",MB_OK);

		return 0;
	}

	pVehicle = GetVehicle();
	pBasePort = GetBasePort();
	pPPoint = m_PPointsList;
	PNB_num = pPPoint->GetPNBnum();
	if (PNB_num == -1)
		PNB_id = pBasePort->GetPortID();
	else
		PNB_id = pBasePort->GetPNB(PNB_num).id;
	
	LoadTimeDaysAll = 0.0;

	Distance = pBasePort->GetDistancePortPNB(PNB_id);
	//LoadTime += 0.3; //stab. TBD
	cargo_type = pPPoint->GetCargoType();
	//load = m_ShipLoadingPlan[PNB_num][cargo_type]; // ??? где время погрузки в базовом порту???
	load = m_ShipLoadingPlan[PNB_id][cargo_type]; //14.05.05
	//LoadTimeDaysAll += pBasePort->GetPNBShipLoadingTime(PNB_id, pVehicle, cargo_type);
	//LoadTimeDaysPort = pBasePort->GetPortShipLoadingTime(pVehicle, cargo_type);
	LoadTimeDaysPort = pBasePort->GetPortShipLoadingTime(pVehicle, cargo_type, load);
	LoadTimeDaysAll += LoadTimeDaysPort;
	//fprintf (stream,"LoadTimeDaysAll: %f\n",LoadTimeDaysAll);
	//fprintf (stream,"LoadTimeDaysPort: %f\n",LoadTimeDaysPort);
	//LoadTimeDaysAll += pBasePort->GetPNBShipLoadingTime(PNB_id, pVehicle, cargo_type, load);
	LoadTimeDaysAll += pBasePort->GetPNBShipLoadingTime(PNB_num, pVehicle, cargo_type, load);
	//fprintf (stream,"LoadTimeDaysAll: %f\n",LoadTimeDaysAll);

	pPPoint = pPPoint->GetNextPPoint();
	//fprintf (stream,"************************\n");
	
	while(pPPoint){
		PNB_num_prev = PNB_num;
		PNB_id_prev = PNB_id;
		cargo_type_prev = cargo_type;
		PNB_num = pPPoint->GetPNBnum();
		if (PNB_num == -1)
			PNB_id = pBasePort->GetPortID();
		else
			PNB_id = pBasePort->GetPNB(PNB_num).id;
		cargo_type = pPPoint->GetCargoType();
		if ((PNB_id_prev == PNB_id)&& (cargo_type_prev == cargo_type )){  
					// Тот же пункт. Судно возвращается за грузом в базовый порт
			Distance += 2*pBasePort->GetDistancePortPNB(PNB_id);
			LoadTimeDaysAll += LoadTimeDaysPort;
		}
		else{
			Distance += pBasePort->GetDistancePNBPNB(PNB_id_prev,PNB_id);
		}
		
		if (PNB_num != -1){
			load = pPPoint->GetCargoLoadInShipPlan();
			//LoadTimeDays = pBasePort->GetPNBShipLoadingTime(PNB_id, pVehicle, cargo_type);
			
			//LoadTimeDays = pBasePort->GetPNBShipLoadingTime(PNB_id, pVehicle, cargo_type, load);
			LoadTimeDays = pBasePort->GetPNBShipLoadingTime(PNB_num, pVehicle, cargo_type, load);
			//fprintf (stream,"LoadTimeDays: %f\n",LoadTimeDays);
			//fprintf (stream,"LoadTimeDaysAll: %f\n",LoadTimeDaysAll);
			LoadTimeDaysAll += LoadTimeDays;
			//fprintf (stream,"LoadTimeDaysAll: %f\n",LoadTimeDaysAll);

		}

		pPPoint = pPPoint->GetNextPPoint();

		if ((PNB_num == -1) && (pPPoint != NULL)){
			LoadTimeDaysAll += LoadTimeDaysPort;
			//fprintf (stream,"LoadTimeDaysAll!!!: %f\n",LoadTimeDaysAll);

		}
		//fprintf (stream,"==========================\n");
	}
	
	//TripTimeDays = (int)(ceil((((double)Distance/pVehicle->GetSpeed())/24) + LoadTimeDaysAll));
	TripTimeHours = (double)Distance/pVehicle->GetSpeed();
	TripTimeDays = TripTimeHours/24;
	TripTimeDays += LoadTimeDaysAll;


	//fclose(stream);


	return (int)TripTimeDays;

}

int CRouterP::GetCargoLoadInPlanForPNB(int PNB_ID, int cargo_type)
{
	return m_ShipLoadingPlan[PNB_ID][cargo_type];
}
