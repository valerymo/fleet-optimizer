// MarineTechEconomicEng.cpp: implementation of the CMarineTechEconomicEng class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "fleetoptimizer.h"
#include "MarineTechEconomicEng.h"
#include "BasePlanSt1LP.h"
#include "BasePlanSt2LpDp.h"
#include "BasePlanSt3Dp.h"
#include <math.h>


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

CMarineTechEconomicEng* CMarineTechEconomicEng::_instance = 0;

CMarineTechEconomicEng* CMarineTechEconomicEng::Instance(){
    if (_instance == 0){
        _instance = new CMarineTechEconomicEng;
    }
    return _instance;
}


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMarineTechEconomicEng::CMarineTechEconomicEng()
{
	m_StepOpt = 0;
	m_pRouter = NULL;
	m_pRouterL = NULL;
	m_pRouterP = NULL;

	POSITION pos = ((CFleetOptimizerApp*)AfxGetApp())->GetCurrFleetOptimizerDoc()->GetFirstViewPosition();
	m_pOutView = (COutputView*) ((CFleetOptimizerApp*)AfxGetApp())->GetCurrFleetOptimizerDoc()->GetNextView(pos);
	m_pOutView = (COutputView*) ((CFleetOptimizerApp*)AfxGetApp())->GetCurrFleetOptimizerDoc()->GetNextView(pos);

	InitLandStructurePPVirt();
}

CMarineTechEconomicEng::~CMarineTechEconomicEng()
{

}

int CMarineTechEconomicEng::GetLandStructExpenciesYearDeltaSt0(CPort *pBasePort, int PNB_num, CVehicle *pVehicleIn, int VehicleNum, int BasePlanNum)
{
	m_StepOpt = 0;
   /*********************************************\
	Функция используется на предварительном этапе 
	оптимизации:  Step 0.
   \*********************************************/

	int ExpencYear;
	
	CRouter	*pBasePlan = CInitialBasePlan::Instance()->GetRoutersListHead();
	m_pRouter = pBasePlan;
	
	int VehicleInLoad = pVehicleIn->GetLoad();
	CString strRegisterClass = pVehicleIn->GetRegisterClass();

	int Expencies = 0;

	if (strRegisterClass.Find("*ACV*") != -1){ //ACV
		//************Base Port **************
		// Береговые площадки:
		Expencies += GetExpenciesACVSpace(pBasePort, pVehicleIn, VehicleNum);		
		// Ремонтные базы: 
		Expencies += GetExpenciesACVRepair(pBasePort, VehicleInLoad, VehicleNum);

		//************ PNB **************
		Expencies += pBasePort->GetPNB(PNB_num).LandStructDataPPVirt.nACVCourtInvestRequere;

	}else { // Ship
		//************Base Port **************
		// Причал для судов-снабженцев в базовом пункте строится один
		// и расчитывается на судно наибольшей длины
		Expencies += GetExpenciesShipMoor(pBasePort, pVehicleIn);

		// Ремонтные базы:
		Expencies += GetExpenciesShipRepair(pBasePort, VehicleInLoad, VehicleNum);

		//************ PNB **************
		//1. Если судна типа Амгуэма (имя судна д.б. подобно),
		//то проверяется береговая площадка в ПНБ
		CString strShipName = pVehicleIn->GetTypeName();
		if(strShipName.Find("Amguema") != -1){	//Amguema
			if (pBasePort->GetPNB(PNB_num).LandStructDataPP.bShipCourtAvailable){
				Expencies += pBasePort->GetPNB(PNB_num).LandStructDataPP.nShipCourtInvestRequere;
			}
			else {
				Expencies += pBasePort->GetPNB(PNB_num).LandStructDataPPVirt.nShipCourtInvestRequere;
			}
		}
	}
	
	//защита для отладки: 
	if (Expencies > 9999999)
		Expencies = 9999999;

	ExpencYear = ExpenciesYear(Expencies);

	return ExpencYear;
}

int CMarineTechEconomicEng::GetLandStructExpenciesYearDeltaSt1(CPort *pBasePort, int PNB_num, CVehicle *pVehicleIn, int VehicleNum, CVehicle *pVehicleBP, int VehicleBPNum,  int BasePlanNum)
{
	m_StepOpt = 1;
	/********************************************************\
	 Функция используется на первом этапе оптимизации: Step 1
	\********************************************************/

	int Expencies = 0;
	int ExpencYear;
	int PricePer10M, PricePer100M2;
	CString strShipNameBP, strShipNameIn;

	CRouter *pBasePlan = CBasePlanSt1LP::Instance()->GetRoutersListHead();
	if (pBasePlan == NULL)
		return 0;
	m_pRouter = pBasePlan;
	//pBasePort->CleanLandDataUpdateStructure();

	int VehicleInLoad = pVehicleIn->GetLoad();
	int VehicleBPLoad = pVehicleBP->GetLoad();

	strShipNameIn = pVehicleIn->GetTypeName();
	strShipNameBP = pVehicleBP->GetTypeName();
	
	CString strRegisterClassVehicleIn = pVehicleIn->GetRegisterClass();
	CString strRegisterClassVehicleBP = pVehicleBP->GetRegisterClass();
	int res1 = strRegisterClassVehicleIn.Find("*ACV*");
	int res2 = strRegisterClassVehicleBP.Find("*ACV*");
	PricePer100M2 = pBasePort->m_LandStructDataVirt.nACVSpaceElemPrice;
	PricePer10M = pBasePort->m_LandStructDataVirt.nMoorElemPrice;


	if ((res1 != -1) && (res2 != -1)){
		//Оба судна - СВП
	
		//************Base Port **************
		//Court space:
		Expencies += GetExpenciesACVSpace(pBasePort, pVehicleIn, VehicleNum, pVehicleBP, VehicleBPNum);
		// Ремонтные базы:
		Expencies += GetExpenciesACVRepair(pBasePort, VehicleInLoad, VehicleNum);

		//*********** PNB: нет изменений

	}//end of: Оба судна - СВП
	else if ((res1 == -1) && (res2 == -1)){
		//Оба судна - водоизмещающие суда

		//************Base Port **************
		// Причал:
		Expencies += GetExpenciesShipMoor(pBasePort, pVehicleIn);
		// Ремонтная база:
		Expencies += GetExpenciesShipRepair(pBasePort, VehicleInLoad, VehicleNum);
		int LoadMax;
		if (VehicleInLoad < VehicleBPLoad){
			LoadMax = GetBiggestShipLoad();
			if(VehicleBPLoad >= LoadMax) {
				 // входящее судно замещает наибольшее судно
			// и расходы на ремонтные базы м.б. уменьшены
			}
		}//end of Ремонтная база

		//************ PNB **************
		//Если одно из судов - типа Амгуэма,
		//то проверяется береговая площадка в ПНБ
		int amg_resIn = strShipNameIn.Find("Amguema");
		int amg_resBP = strShipNameBP.Find("Amguema");
		int amg_numExistInPlan = IsExistInPlanAmguema();
		if (amg_resIn != -1){	
			//входящее - Amguema, то возможны инвестиции:
			Expencies += GetExpenciesShipCourt(pBasePort, PNB_num, pVehicleIn);
		}
		else if ((amg_resIn == -1)&& (amg_resBP != -1) && (amg_numExistInPlan == 1)){
		//"входящее - не Amguema, выходящее судно - Амгуема, единственное в плане:
		//расходы на стороительство береговой площадки д.б. вычтены
			Expencies -= pBasePort->GetPNB(PNB_num).LandStructDataPPVirt.nShipCourtInvestRequere;
		} // end of: Amguema

	} //end of: Оба судна - водоизмещающие суда

	else {//Входящее и выходящее суда - разного типа
		if ((IsUnicTypeVehicleOut(pVehicleBP) == 1) ||(IsUnicTypeVehicleIn(pVehicleIn) == 1) || 
			(IsBiggestVehicleOut(pVehicleBP) == 1) || (IsBiggestVehicleIn(pVehicleIn) == 1)) {
				Expencies += GetExpenciesUnicOrBiggestVehicle(pBasePort, PNB_num, pVehicleIn, VehicleNum, pVehicleBP, VehicleBPNum, res1);
		}
	}
	
	//защита для отладки: 
	if (Expencies > 9999999)
		Expencies = 9999999;
	
	ExpencYear = ExpenciesYear(Expencies);

	return ExpencYear;
}

int CMarineTechEconomicEng::GetLandStructExpenciesYearDeltaSt2(CRouterL *pRouterBP, CVehicle *pVehicleIn, int VehicleNum)
{
	m_StepOpt = 2;
	/********************************************************\
	 Функция используется на втором этапе оптимизации: Step 2
	\********************************************************/

	CLine *pLine;
	int PNB_num, PNB_numBP, VehicleBPNum;
	CVehicle *pVehicleBP;
	int PricePer10M, PricePer100M2, ExpencYear;
	CString strShipNameBP, strShipNameIn;
	
	int Expencies = 0;

	CRouterL *pBasePlan = CBasePlanSt2LpDp::Instance()->GetRoutersListHead();
	if (pBasePlan == NULL)
		return 0;

	m_pRouter = pBasePlan;
	m_pRouterL = pBasePlan;
	CPort *pBasePort = pBasePlan->GetBasePort();
	//pBasePort->CleanLandDataUpdateStructure();
	
	PNB_numBP =		pBasePlan->GetLine()->GetPNBnum();
	PNB_num = PNB_numBP;
	pVehicleBP =	pBasePlan->GetVehicle();
	VehicleBPNum =  pBasePlan->GetVehicleNumber();
	
	int VehicleInLoad = pVehicleIn->GetLoad();
	int VehicleBPLoad = pVehicleBP->GetLoad();

	strShipNameIn = pVehicleIn->GetTypeName();
	strShipNameBP = pVehicleBP->GetTypeName();
	
	CString strRegisterClassVehicleIn = pVehicleIn->GetRegisterClass();
	CString strRegisterClassVehicleBP = pVehicleBP->GetRegisterClass();
	int res1 = strRegisterClassVehicleIn.Find("*ACV*");
	int res2 = strRegisterClassVehicleBP.Find("*ACV*");
	PricePer100M2 = pBasePort->m_LandStructDataVirt.nACVSpaceElemPrice;
	PricePer10M = pBasePort->m_LandStructDataVirt.nMoorElemPrice;


	if ((res1 != -1) && (res2 != -1)){
		//Оба судна - СВП
	
		//********** Base Port ******
		//Court space:
		Expencies += GetExpenciesACVSpace(pBasePort, pVehicleIn, VehicleNum, pVehicleBP, VehicleBPNum);
		// Ремонтные базы:		
		Expencies += GetExpenciesACVRepair(pBasePort, VehicleInLoad, VehicleNum);

		//*********** PNB ***********
		// нет изменений

	}//end of: Оба судна - СВП
	else if ((res1 == -1)&&(res2 == -1)){
	//Оба судна - водоизмещающие
		
		//************Base Port **************
		// Причал: 
		Expencies += GetExpenciesShipMoor(pBasePort, pVehicleIn);
		// Ремонтные базы:
		Expencies += GetExpenciesShipRepair(pBasePort, VehicleInLoad, VehicleNum);
		
		//*********** PNB: ***********
		//Изменения в ПНБ касаются только судов типа Амгуема
		//Если одно из судов - типа Амгуэма,
		//то проверяется береговая площадка в ПНБ
		int amg_resIn = strShipNameIn.Find("Amguema");
		int amg_resBP = strShipNameBP.Find("Amguema");
		int amg_numExistInPlan = IsExistInPlanAmguema();
		if (amg_resIn != -1){	
			//входящее - Amguema, то возможны инвестиции:
			Expencies += GetExpenciesShipCourt(pBasePort, PNB_num, pVehicleIn);
		}
		else if ((amg_resIn == -1)&& (amg_resBP != -1) && (amg_numExistInPlan == 1)){
		//"входящее - не Amguema, выходящее судно - Амгуема, единственное в плане:
		//расходы на стороительство береговой площадки д.б. вычтены
			pLine = pBasePlan->GetLine();
			while(pLine){
				PNB_num = pLine->GetPNBnum();
				Expencies -= pBasePort->GetPNB(PNB_num).LandStructDataPPVirt.nShipCourtInvestRequere;
				pLine = pLine->GetNextLine();
			}
		} // end of: Amguema
	} //end of: Оба судна - водоизмещающие суда
	
	else { //Входящее и выходящее суда - разного типа
		if ((IsUnicTypeVehicleOut(pVehicleBP) == 1) ||(IsUnicTypeVehicleIn(pVehicleIn) == 1) || 
			(IsBiggestVehicleOut(pVehicleBP) == 1) || (IsBiggestVehicleIn(pVehicleIn) == 1)) {
				Expencies += GetExpenciesUnicOrBiggestVehicle(pBasePort, PNB_num, pVehicleIn, VehicleNum, pVehicleBP, VehicleBPNum, res1);
		}
	}
	
	//защита для отладки: 
	if (Expencies > 9999999)
		Expencies = 9999999;
	
	ExpencYear = ExpenciesYear(Expencies);

	return ExpencYear;
}

int CMarineTechEconomicEng::GetLandStructExpenciesYearDeltaSt3BasePort(CVehicle *pVehicleIn)
{
	m_StepOpt = 3; 
	/******************************************************************\
	 Функция используется на втором этапе оптимизации: Step 3. BasePort
	\******************************************************************/
	
	int VehicleInLoad, ExpencYear;
	CString strShipNameIn, strRegisterClassVehicleIn;
	CPort *pBasePort;
		
	CRouterP *pt = CBasePlanSt3Dp::Instance()->GetRoutersListHead();
	if (pt == NULL)
		return 0;
	m_pRouter = pt;
	m_pRouterP = pt;
	pBasePort = pt->GetBasePort();
	VehicleInLoad = pVehicleIn->GetLoad();
	strRegisterClassVehicleIn = pVehicleIn->GetRegisterClass();

	int VehicleNum = 1; // 3rd step

	int Expencies = 0;

	//pBasePort->CleanLandDataUpdateStructure();

	if (strRegisterClassVehicleIn.Find("*ACV*") != -1){ // СВП
		// Береговые площадки:
		Expencies += GetExpenciesACVSpace(pBasePort, pVehicleIn, VehicleNum);		
		// Ремонтные базы: 
		Expencies += GetExpenciesACVRepair(pBasePort, VehicleInLoad, VehicleNum);


	}//end of: СВП
	else {// Водоизмещающее судно
		// Причал: 
		// причал для судов-снабженцев в базовом пункте строится один
		// и расчитывается на судно наибольшей длины.
		Expencies += GetExpenciesShipMoor(pBasePort, pVehicleIn);
		// Ремонтные базы:
		Expencies += GetExpenciesShipRepair(pBasePort, VehicleInLoad, VehicleNum);
		
	} //end of: Водоизмещающее судно
	
	//защита для отладки: 
	if (Expencies > 9999999)
		Expencies = 9999999;
	
	ExpencYear = ExpenciesYear(Expencies);

	return ExpencYear;

}

int CMarineTechEconomicEng::GetLandStructExpenciesYearAllSt1()
{	
	m_StepOpt = 1;
	int MaxShipLoad = 1000;
	int MaxACVLoad = 200;
	int id;

	CRouter *pRouter = CBasePlanSt1LP::Instance()->GetRoutersListHead();
	if (pRouter == NULL)
		return 0;
	m_pRouter = pRouter;


	int ExpencYear;
	int i;
	int SpaceACVCourt, SpaceAvailable, SpaceDiff, 
		MoorLength, MoorLengthAvailable, MoorLengthDiff,
		PricePer100M2, PricePer10M,
		VehicleLoad, VehicleNum;
	CString strShipName, strRegisterClass;
	CVehicle *pVehicle;
	int Expencies = 0;

	CPort *pBasePort = CPortList::Instance()->GetPortListHead();
	if (pBasePort == NULL)
		return 0;

	//****** Base Port *******
	// ACV Court:
	SpaceACVCourt = GetSpaceForACVs();
	SpaceAvailable = pBasePort->m_LandStructDataVirt.nACVSpace;
	SpaceDiff = SpaceACVCourt - SpaceAvailable;
	PricePer100M2 = pBasePort->m_LandStructDataVirt.nACVSpaceElemPrice;
	if (SpaceDiff>0){
		Expencies += (int)(PricePer100M2 * ((float)SpaceDiff/100));
	}
	// Ships Moor
	MoorLength = GetMoorLength();
	MoorLengthAvailable = pBasePort->m_LandStructDataVirt.nMoorLength;
	MoorLengthDiff = MoorLength - MoorLengthAvailable;
	PricePer10M = pBasePort->m_LandStructDataVirt.nMoorElemPrice;
	if (MoorLengthDiff > 0){
		Expencies += (int)(PricePer10M * ((float)MoorLengthDiff/10));
	}
	// Ремонтные базы в базовом порту
	while(pRouter){
		pVehicle = pRouter->GetVehicle();
		VehicleLoad = pRouter->GetVehicle()->GetLoad();
		VehicleNum = pRouter->GetVehicleNumber();
		strShipName = pVehicle->GetTypeName();
		strRegisterClass = pVehicle->GetRegisterClass();

		if (strRegisterClass.Find("*ACV*") != -1){ //ACV
			Expencies += GetExpenciesACVRepair(pBasePort, VehicleLoad, VehicleNum);
		}
		else{
			Expencies += GetExpenciesShipRepair(pBasePort, VehicleLoad, VehicleNum);
		}
		pRouter = pRouter->GetNextRouter();
	}
	//****** PNB *******
	for (i = 0; i < MAXPNBNUMBER; i++){
		id = pBasePort->GetPNB(i).id;
		if (id == 999)
			continue;
		if (pBasePort->PNBList[i].LandStructDataPPVirt.bACVCourtUse == 1)
			Expencies += pBasePort->PNBList[i].LandStructDataPPVirt.nACVCourtInvestRequere;
		if (pBasePort->PNBList[i].LandStructDataPPVirt.bShipCourtUse == 1)
			Expencies += pBasePort->PNBList[i].LandStructDataPPVirt.nShipCourtInvestRequere;
	}

	ExpencYear = ExpenciesYear(Expencies);
	
	return ExpencYear;
}

int CMarineTechEconomicEng::GetLandStructExpenciesYearAllSt2()
{	
	m_StepOpt = 2;
	int MaxShipLoad = 1000;
	int MaxACVLoad = 200;
	int id;

	CRouterL *pRouter = CBasePlanSt2LpDp::Instance()->GetRoutersListHead();
	if (pRouter == NULL)
		return 0;
	m_pRouterL = pRouter;

	int ExpencYear, i;
	int SpaceACVCourt, SpaceAvailable, SpaceDiff, 
		MoorLength, MoorLengthAvailable, MoorLengthDiff,
		PricePer100M2, PricePer10M,
		VehicleLoad, VehicleNum;
	CString strShipName, strRegisterClass;
	CVehicle *pVehicle;
	int Expencies = 0;

	CPort *pBasePort = CPortList::Instance()->GetPortListHead();
	if (pBasePort == NULL)
		return 0;

	//****** Base Port *******
	// ACV Court:
	SpaceACVCourt = GetSpaceForACVs();
	SpaceAvailable = pBasePort->m_LandStructDataVirt.nACVSpace;
	SpaceDiff = SpaceACVCourt - SpaceAvailable;
	PricePer100M2 = pBasePort->m_LandStructDataVirt.nACVSpaceElemPrice;
	if (SpaceDiff>0){
		Expencies += (int)(PricePer100M2 * ((float)SpaceDiff/100));
	}
	// Ships Moor
	MoorLength = GetMoorLength();
	MoorLengthAvailable = pBasePort->m_LandStructDataVirt.nMoorLength;
	MoorLengthDiff = MoorLength - MoorLengthAvailable;
	PricePer10M = pBasePort->m_LandStructDataVirt.nMoorElemPrice;
	if (MoorLengthDiff > 0){
		Expencies += (int)(PricePer10M * ((float)MoorLengthDiff/10));
	}
	// Ремонтные базы в базовом порту
	while(pRouter){
		pVehicle = pRouter->GetVehicle();
		VehicleLoad = pRouter->GetVehicle()->GetLoad();
		VehicleNum = pRouter->GetVehicleNumber();
		strShipName = pVehicle->GetTypeName();
		strRegisterClass = pVehicle->GetRegisterClass();
		if (strRegisterClass.Find("*ACV*") != -1){ //ACV
			Expencies += GetExpenciesACVRepair(pBasePort, VehicleLoad, VehicleNum);
		}
		else{
			Expencies += GetExpenciesShipRepair(pBasePort, VehicleLoad, VehicleNum);
		}
		pRouter = pRouter->GetNextRouter();
	}
	//****** PNB *******
	for (i = 0; i < MAXPNBNUMBER; i++){
		id = pBasePort->GetPNB(i).id;
		if (id == 999)
			continue;
		if (pBasePort->PNBList[i].LandStructDataPPVirt.bACVCourtUse == 1)
			Expencies += pBasePort->PNBList[i].LandStructDataPPVirt.nACVCourtInvestRequere;
		if (pBasePort->PNBList[i].LandStructDataPPVirt.bShipCourtUse == 1)
			Expencies += pBasePort->PNBList[i].LandStructDataPPVirt.nShipCourtInvestRequere;
	}
	ExpencYear = ExpenciesYear(Expencies);
	
	return ExpencYear;
}

int CMarineTechEconomicEng::GetLandStructExpenciesYearAllSt3()
{	
	m_StepOpt = 3;
	int MaxShipLoad = 1000;
	int MaxACVLoad = 200;
	int id;

	CRouterP *pRouter = CBasePlanSt3Dp::Instance()->GetRoutersListHead();
	if (pRouter == NULL)
		return 0;
	m_pRouterP = pRouter;

	int ExpencYear, i;
	int SpaceACVCourt, SpaceAvailable, SpaceDiff, 
		MoorLength, MoorLengthAvailable, MoorLengthDiff,
		PricePer100M2, PricePer10M,
		VehicleLoad, VehicleNum;
	CString strShipName, strRegisterClass;
	CVehicle *pVehicle;
	int Expencies = 0;

	CPort *pBasePort = CPortList::Instance()->GetPortListHead();
	if (pBasePort == NULL)
		return 0;

	//****** Base Port *******
	// ACV Court:
	SpaceACVCourt = GetSpaceForACVs();
	SpaceAvailable = pBasePort->m_LandStructDataVirt.nACVSpace;
	SpaceDiff = SpaceACVCourt - SpaceAvailable;
	PricePer100M2 = pBasePort->m_LandStructDataVirt.nACVSpaceElemPrice;
	if (SpaceDiff>0){
		Expencies += (int)(PricePer100M2 * ((float)SpaceDiff/100));
	}
	// Ships Moor
	MoorLength = GetMoorLength();
	MoorLengthAvailable = pBasePort->m_LandStructDataVirt.nMoorLength;
	MoorLengthDiff = MoorLength - MoorLengthAvailable;
	PricePer10M = pBasePort->m_LandStructDataVirt.nMoorElemPrice;
	if (MoorLengthDiff > 0){
		Expencies += (int)(PricePer10M * ((float)MoorLengthDiff/10));
	}
	// Ремонтные базы в базовом порту
	while(pRouter){
		pVehicle = pRouter->GetVehicle();
		VehicleLoad = pRouter->GetVehicle()->GetLoad();
		VehicleNum = pRouter->GetVehicleNumber();
		strShipName = pVehicle->GetTypeName();
		strRegisterClass = pVehicle->GetRegisterClass();
		if (strRegisterClass.Find("*ACV*") != -1){ //ACV
			Expencies += GetExpenciesACVRepair(pBasePort, VehicleLoad, VehicleNum);
		}
		else{
			Expencies += GetExpenciesShipRepair(pBasePort, VehicleLoad, VehicleNum);
		}
		pRouter = pRouter->GetNextRouter();
	}
	//****** PNB *******
	for (i = 0; i < MAXPNBNUMBER; i++){
		id = pBasePort->GetPNB(i).id;
		if (id == 999)
			continue;
		if (pBasePort->PNBList[i].LandStructDataPPVirt.bACVCourtUse == 1)
			Expencies += pBasePort->PNBList[i].LandStructDataPPVirt.nACVCourtInvestRequere;
		if (pBasePort->PNBList[i].LandStructDataPPVirt.bShipCourtUse == 1)
			Expencies += pBasePort->PNBList[i].LandStructDataPPVirt.nShipCourtInvestRequere;
	}
	ExpencYear = ExpenciesYear(Expencies);
	
	return ExpencYear;
}

int CMarineTechEconomicEng::GetSpaceForACVs(CVehicle *pVehicleIn, int VehicleNum)
{
	int distAA = 6;	// Расстояние между двумя СВП на стоянке,
					// необходимое для обслуживания
	int load = pVehicleIn->GetLoad();
	ACVMainDetails* acv_tmp = new ACVMainDetails;
	ACVPrototypeMainDetails* acv_prototype_tmp = new ACVPrototypeMainDetails;
	acv_prototype_tmp->name = pVehicleIn->GetPrototypeName();
	CFleetTechEconomicEng::Instance()->GetACVPrototypeMainDetails(acv_prototype_tmp);
	acv_tmp->name = pVehicleIn->GetTypeName();
	acv_tmp->speed = (float)pVehicleIn->GetSpeed();
	acv_tmp->Dgr = pVehicleIn->GetLoad();
	acv_tmp->r = pVehicleIn->GetRange();
	if (acv_tmp->r <= 0){
		CString strMessage = "CMarineTechEconomicEng: Ошибка в дальности хода СВП. \n";
		strMessage += pVehicleIn->GetTypeName();
		AfxMessageBox(strMessage);
		m_pOutView->TextOut(strMessage);
		return 0;
	}
	acv_tmp->h3 = pVehicleIn->GetH3();
	CACVDesign::Instance()->GetACVMainDetails(acv_prototype_tmp, acv_tmp);
	int Space = (int)acv_tmp->Sp;
	int SpaceAdd = (int)(pow(distAA, 2.0f) + distAA*1.1*(acv_tmp->Bp + acv_tmp->Lp));
	
	//  |  |	 |
	//  |  |ACV	 |
	//  |  |Space|
	//  |  |_____|
	//  |SpaceAdd
	//  |---------
	//
	Space += SpaceAdd;
	
	Space *= VehicleNum;

	return Space;

}

int CMarineTechEconomicEng::GetSpaceForACVs()
{
	CVehicle *pVehicle;
	int VehicleNum;
	CString sRegisterClass;
	int Space = 0;

	if ((m_StepOpt==0)||(m_StepOpt==1)){
		CRouter *pRouter = m_pRouter;
		while(pRouter){
			pVehicle = pRouter->GetVehicle();
			sRegisterClass = pVehicle->GetRegisterClass();
			if (sRegisterClass.Find("*ACV*") != -1){ //ACV
				VehicleNum = pRouter->GetVehicleNumber();
				Space += GetSpaceForACVs(pVehicle, VehicleNum);
			}
			pRouter = pRouter->GetNextRouter();
		}
	}
	else if(m_StepOpt==2){
		CRouterL *pRouter = m_pRouterL;
		while(pRouter){
			pVehicle = pRouter->GetVehicle();
			sRegisterClass = pVehicle->GetRegisterClass();
			if (sRegisterClass.Find("*ACV*") != -1){ //ACV
				VehicleNum = pRouter->GetVehicleNumber();
				Space += GetSpaceForACVs(pVehicle, VehicleNum);
			}
			pRouter = pRouter->GetNextRouter();
		}
	}
	else if(m_StepOpt==3){
		CRouterP *pRouter = m_pRouterP;
		while(pRouter){
			pVehicle = pRouter->GetVehicle();
			sRegisterClass = pVehicle->GetRegisterClass();
			if (sRegisterClass.Find("*ACV*") != -1){ //ACV
				VehicleNum = pRouter->GetVehicleNumber();
				Space += GetSpaceForACVs(pVehicle, VehicleNum);
			}
			pRouter = pRouter->GetNextRouter();
		}
	}

	return Space;
}

int CMarineTechEconomicEng::GetMoorLength(CVehicle *pVehicle)
{
	int addLength = 30;

	ShipMainDetails* ship_tmp = new ShipMainDetails;
	ShipPrototypeMainDetails* ship_prototype_tmp = new ShipPrototypeMainDetails;

	ship_prototype_tmp->name = pVehicle->GetPrototypeName();
	CFleetTechEconomicEng::Instance()->GetShipPrototypeMainDetails(ship_prototype_tmp);
	ship_tmp->speed = (float)pVehicle->GetSpeed();
	ship_tmp->Pgr = pVehicle->GetLoad();
	ship_tmp->name = pVehicle->GetTypeName();
	CShipDesign::Instance()->GetShipMainDetails(ship_prototype_tmp, ship_tmp);

	int MoorLength = (int)ship_tmp->L + 30;

	return MoorLength;

}

int CMarineTechEconomicEng::IsExistInPlanAmguema()
{
	CString strShipName;
	int count = 0;

	if ((m_StepOpt==0)||(m_StepOpt==1)){
		CRouter *pRouter = m_pRouter;
		while(pRouter){
			strShipName = pRouter->GetVehicle()->GetTypeName();
			if (strShipName.Find("Amguema") != -1){
				count++;
			}
			pRouter = pRouter->GetNextRouter();
		}	
	}
	else if(m_StepOpt==2){
		CRouterL *pRouter = m_pRouterL;
		while(pRouter){
			strShipName = pRouter->GetVehicle()->GetTypeName();
			if (strShipName.Find("Amguema") != -1){
				count++;
			}
			pRouter = pRouter->GetNextRouter();
		}	
	}
	else if(m_StepOpt==3){
		CRouterP *pRouter = m_pRouterP;
		while(pRouter){
			strShipName = pRouter->GetVehicle()->GetTypeName();
			if (strShipName.Find("Amguema") != -1){
				count++;
			}
			pRouter = pRouter->GetNextRouter();
		}	
	}



	return count;
}

int CMarineTechEconomicEng::GetMoorLength()
{
	CVehicle *pVehicle;
	CString sRegisterClass;
	int LMax = 0;
	CRouter *pBasePlan = m_pRouter;

	ShipMainDetails* ship_tmp = new ShipMainDetails;
	ShipPrototypeMainDetails* ship_prototype_tmp = new ShipPrototypeMainDetails;

	//ищется судно максимальной длины:
	if ((m_StepOpt==0)||(m_StepOpt==1)){
		CRouter *pRouter = m_pRouter;
		while(pRouter){
			pVehicle = pRouter->GetVehicle();
			sRegisterClass = pVehicle->GetRegisterClass();
			if (sRegisterClass.Find("*ACV*") == -1){ //Ship
				ship_prototype_tmp->name = pVehicle->GetPrototypeName();
				CFleetTechEconomicEng::Instance()->GetShipPrototypeMainDetails(ship_prototype_tmp);
				ship_tmp->speed = (float)pVehicle->GetSpeed();
				ship_tmp->Pgr = pVehicle->GetLoad();
				ship_tmp->name = pVehicle->GetTypeName();
				CShipDesign::Instance()->GetShipMainDetails(ship_prototype_tmp, ship_tmp);
				if (ship_tmp->L > LMax)
					LMax = (int)ship_tmp->L;
			}
			pRouter = pRouter->GetNextRouter();
		}
	}
	else if (m_StepOpt==2){
		CRouterL *pRouter = m_pRouterL;
		while(pRouter){
			pVehicle = pRouter->GetVehicle();
			sRegisterClass = pVehicle->GetRegisterClass();
			if (sRegisterClass.Find("*ACV*") == -1){ //Ship
				ship_prototype_tmp->name = pVehicle->GetPrototypeName();
				CFleetTechEconomicEng::Instance()->GetShipPrototypeMainDetails(ship_prototype_tmp);
				ship_tmp->speed = (float)pVehicle->GetSpeed();
				ship_tmp->Pgr = pVehicle->GetLoad();
				ship_tmp->name = pVehicle->GetTypeName();
				CShipDesign::Instance()->GetShipMainDetails(ship_prototype_tmp, ship_tmp);
				if (ship_tmp->L > LMax)
					LMax = (int)ship_tmp->L;
			}
			pRouter = pRouter->GetNextRouter();
		}
	}
	else if (m_StepOpt==3){
		CRouterP *pRouter = m_pRouterP;
		while(pRouter){
			pVehicle = pRouter->GetVehicle();
			sRegisterClass = pVehicle->GetRegisterClass();
			if (sRegisterClass.Find("*ACV*") == -1){ //Ship
				ship_prototype_tmp->name = pVehicle->GetPrototypeName();
				CFleetTechEconomicEng::Instance()->GetShipPrototypeMainDetails(ship_prototype_tmp);
				ship_tmp->speed = (float)pVehicle->GetSpeed();
				ship_tmp->Pgr = pVehicle->GetLoad();
				ship_tmp->name = pVehicle->GetTypeName();
				CShipDesign::Instance()->GetShipMainDetails(ship_prototype_tmp, ship_tmp);
				if (ship_tmp->L > LMax)
					LMax = (int)ship_tmp->L;
			}
			pRouter = pRouter->GetNextRouter();
		}
	}

	if (LMax == 0)
		LMax = 40;	// No ships found, but the moor shoul exist in the Base Port

	int MoorLength = LMax + 30;

	return MoorLength;

}



int  CMarineTechEconomicEng::GetBiggestShipLoad()
{

	CVehicle *pVehicleBP;
	int Load;
	int LoadMax = 0;
	CRouter *pBasePlan = m_pRouter;

	//ищется судно максимальной гркзоподъемности:
	if ((m_StepOpt==0)||(m_StepOpt==1)){
		CRouter *pRouter = m_pRouter;
		while(pRouter){
			pVehicleBP = pRouter->GetVehicle();
			Load = pVehicleBP->GetLoad();
			if (Load > LoadMax){
				LoadMax = Load;
			}
			pRouter = pRouter->GetNextRouter();
		}
	}
	else if (m_StepOpt==2){
		CRouterL *pRouter = m_pRouterL;
		while(pRouter){
			pVehicleBP = pRouter->GetVehicle();
			Load = pVehicleBP->GetLoad();
			if (Load > LoadMax){
				LoadMax = Load;
			}
			pRouter = pRouter->GetNextRouter();
		}
	}
	else if (m_StepOpt==3){
		CRouterP *pRouter = m_pRouterP;
		while(pRouter){
			pVehicleBP = pRouter->GetVehicle();
			Load = pVehicleBP->GetLoad();
			if (Load > LoadMax){
				LoadMax = Load;
			}
			pRouter = pRouter->GetNextRouter();
		}
	}


	return LoadMax;
}

int CMarineTechEconomicEng::IsUnicTypeVehicleIn(CVehicle *pVehicleIn)
{
	CVehicle *pVehicleBP;
	int flACVIn = 0;
	int flShipIn = 0;
	int isUnic = 1;
	CRouter *pBasePlan = m_pRouter;

	CString strRegisterClassIn = pVehicleIn->GetRegisterClass();
	if (strRegisterClassIn.Find("*ACV*") != -1){ //ACV
		flACVIn = 1;
	}
	else {
		flShipIn = 1;
	}
	CString strRegisterClassBP;
	
	if ((m_StepOpt==0)||(m_StepOpt==1)){
		CRouter *pRouter = m_pRouter;
		while(pRouter){
			pVehicleBP = pRouter->GetVehicle();
			strRegisterClassBP = pVehicleBP->GetRegisterClass();
			if(flACVIn){	
				if (strRegisterClassBP.Find("*ACV*") != -1){ //ACV
					isUnic = 0;
					break;
				}
			}else { //ship
				if (strRegisterClassBP.Find("*ACV*") == -1){ //SHIP
					isUnic = 0;
					break;
				}
			}
			pRouter = pRouter->GetNextRouter();
		}
	}
	else if (m_StepOpt==2){
		CRouterL *pRouter = m_pRouterL;
		while(pRouter){
			pVehicleBP = pRouter->GetVehicle();
			strRegisterClassBP = pVehicleBP->GetRegisterClass();
			if(flACVIn){	
				if (strRegisterClassBP.Find("*ACV*") != -1){ //ACV
					isUnic = 0;
					break;
				}
			}else { //ship
				if (strRegisterClassBP.Find("*ACV*") == -1){ //SHIP
					isUnic = 0;
					break;
				}
			}
			pRouter = pRouter->GetNextRouter();
		}
	}
	else if (m_StepOpt==3){
		CRouterP *pRouter = m_pRouterP;
		while(pRouter){
			pVehicleBP = pRouter->GetVehicle();
			strRegisterClassBP = pVehicleBP->GetRegisterClass();
			if(flACVIn){	
				if (strRegisterClassBP.Find("*ACV*") != -1){ //ACV
					isUnic = 0;
					break;
				}
			}else { //ship
				if (strRegisterClassBP.Find("*ACV*") == -1){ //SHIP
					isUnic = 0;
					break;
				}
			}
			pRouter = pRouter->GetNextRouter();
		}
	}


	return isUnic;
}

int CMarineTechEconomicEng::IsUnicTypeVehicleOut(CVehicle *pVehicleOut)
{
	CVehicle *pVehicleBP;
	int flACV = 0;
	int flShip = 0;
	int isUnic = 1;
	CRouter *pBasePlan = m_pRouter;

	CString strTypeNameVOut = pVehicleOut->GetTypeName();
	CString strTypeNameVBP;
	
	CString strRegisterClassOut = pVehicleOut->GetRegisterClass();
	if (strRegisterClassOut.Find("*ACV*") != -1){ //ACV
		flACV = 1;
	}
	else {
		flShip = 1;
	}
	CString strRegisterClassBP;
	if ((m_StepOpt==0)||(m_StepOpt==1)){
		CRouter *pRouter = m_pRouter;
		while(pRouter){
			pVehicleBP = pRouter->GetVehicle();
			strTypeNameVBP = pVehicleBP->GetTypeName();
			if (strcmp(strTypeNameVOut,strTypeNameVBP) == 0){
				pRouter = pRouter->GetNextRouter();
				continue;
			}
			strRegisterClassBP = pVehicleBP->GetRegisterClass();
			if(flACV){	
				if (strRegisterClassBP.Find("*ACV*") != -1){ //ACV
					isUnic = 0;
					break;
				}
			}else { //ship
				if (strRegisterClassBP.Find("*ACV*") == -1){ //SHIP
					isUnic = 0;
					break;
				}
			}
			pRouter = pRouter->GetNextRouter();
		}
	}
	else if (m_StepOpt==2){
		CRouterL *pRouter = m_pRouterL;
		while(pRouter){
			pVehicleBP = pRouter->GetVehicle();
			strTypeNameVBP = pVehicleBP->GetTypeName();
			if (strcmp(strTypeNameVOut,strTypeNameVBP) == 0){
				pRouter = pRouter->GetNextRouter();
				continue;
			}
			strRegisterClassBP = pVehicleBP->GetRegisterClass();
			if(flACV){	
				if (strRegisterClassBP.Find("*ACV*") != -1){ //ACV
					isUnic = 0;
					break;
				}
			}else { //ship
				if (strRegisterClassBP.Find("*ACV*") == -1){ //SHIP
					isUnic = 0;
					break;
				}
			}
			pRouter = pRouter->GetNextRouter();
		}
	}
	else if (m_StepOpt==3){
		CRouterP *pRouter = m_pRouterP;
		while(pRouter){
			pVehicleBP = pRouter->GetVehicle();
			strTypeNameVBP = pVehicleBP->GetTypeName();
			if (strcmp(strTypeNameVOut,strTypeNameVBP) == 0){
				pRouter = pRouter->GetNextRouter();
				continue;
			}
			strRegisterClassBP = pVehicleBP->GetRegisterClass();
			if(flACV){	
				if (strRegisterClassBP.Find("*ACV*") != -1){ //ACV
					isUnic = 0;
					break;
				}
			}else { //ship
				if (strRegisterClassBP.Find("*ACV*") == -1){ //SHIP
					isUnic = 0;
					break;
				}
			}
			pRouter = pRouter->GetNextRouter();
		}
	}

	return isUnic;
}

int CMarineTechEconomicEng::IsBiggestVehicleIn(CVehicle *pVehicle)
{
	CVehicle *pVehicleBP;
	int isBiggest = 1;
	int VehicleLoadBP, VehicleLoad, flACV, flShip;
	CRouter *pBasePlan = m_pRouter;

	VehicleLoad = pVehicle->GetLoad();

	CString strRegisterClass = pVehicle->GetRegisterClass();
	if (strRegisterClass.Find("*ACV*") != -1){ //ACV
		flACV = 1;
	}
	else {
		flShip = 1;
	}
	
	CString strRegisterClassBP;
	
	if ((m_StepOpt==0)||(m_StepOpt==1)){
		CRouter *pRouter = m_pRouter;
		while(pRouter){
			pVehicleBP = pRouter->GetVehicle();
			strRegisterClassBP = pVehicleBP->GetRegisterClass();
			if (flACV){
				if (strRegisterClassBP.Find("*ACV*") == -1){ //NOT ACV
					pRouter = pRouter->GetNextRouter();
					continue;
				}
			}else{ //Ship
				if (strRegisterClassBP.Find("*ACV*") != -1){ //ACV
					pRouter = pRouter->GetNextRouter();
					continue;
				}
			}
			VehicleLoadBP = pVehicleBP->GetLoad();
			if(VehicleLoadBP > VehicleLoad){
				isBiggest = 0;
				break;
			}

			pRouter = pRouter->GetNextRouter();
		}
	}
	else if (m_StepOpt==2){
		CRouterL *pRouter = m_pRouterL;
		while(pRouter){
			pVehicleBP = pRouter->GetVehicle();
			strRegisterClassBP = pVehicleBP->GetRegisterClass();
			if (flACV){
				if (strRegisterClassBP.Find("*ACV*") == -1){ //NOT ACV
					pRouter = pRouter->GetNextRouter();
					continue;
				}
			}else{ //Ship
				if (strRegisterClassBP.Find("*ACV*") != -1){ //ACV
					pRouter = pRouter->GetNextRouter();
					continue;
				}
			}
			VehicleLoadBP = pVehicleBP->GetLoad();
			if(VehicleLoadBP > VehicleLoad){
				isBiggest = 0;
				break;
			}

			pRouter = pRouter->GetNextRouter();
		}
	}
	else if (m_StepOpt==3){
		CRouterP *pRouter = m_pRouterP;
		while(pRouter){
			pVehicleBP = pRouter->GetVehicle();
			strRegisterClassBP = pVehicleBP->GetRegisterClass();
			if (flACV){
				if (strRegisterClassBP.Find("*ACV*") == -1){ //NOT ACV
					pRouter = pRouter->GetNextRouter();
					continue;
				}
			}else{ //Ship
				if (strRegisterClassBP.Find("*ACV*") != -1){ //ACV
					pRouter = pRouter->GetNextRouter();
					continue;
				}
			}
			VehicleLoadBP = pVehicleBP->GetLoad();
			if(VehicleLoadBP > VehicleLoad){
				isBiggest = 0;
				break;
			}

			pRouter = pRouter->GetNextRouter();
		}
	}

	return isBiggest;
}

int CMarineTechEconomicEng::IsBiggestVehicleOut(CVehicle *pVehicle)
{
	CVehicle *pVehicleBP;
	int isBiggest = 1;
	int VehicleLoadBP, VehicleLoad, flACV, flShip;

	VehicleLoad = pVehicle->GetLoad();

	CString strRegisterClass = pVehicle->GetRegisterClass();
	if (strRegisterClass.Find("*ACV*") != -1){ //ACV
		flACV = 1;
	}
	else {
		flShip = 1;
	}
	CString strTypeName = pVehicle->GetTypeName();
	CString strTypeNameBP;
	
	CString strRegisterClassBP;

	if ((m_StepOpt==0)||(m_StepOpt==1)){
		CRouter *pRouter = m_pRouter;
		while(pRouter){
			pVehicleBP = pRouter->GetVehicle();
			strTypeNameBP = pVehicleBP->GetTypeName();
			strRegisterClassBP = pVehicleBP->GetRegisterClass();
			if (strcmp(strTypeName,strTypeNameBP) == 0){ 
				//выходящее судно пропускается
				pRouter = pRouter->GetNextRouter();
				continue;
			}
			if (flACV){
				if (strRegisterClassBP.Find("*ACV*") == -1){ //NOT ACV
					pRouter = pRouter->GetNextRouter();
					continue;
				}
			}else{ //Ship
				if (strRegisterClassBP.Find("*ACV*") != -1){ //ACV
					pRouter = pRouter->GetNextRouter();
					continue;
				}
			}
			VehicleLoadBP = pVehicleBP->GetLoad();
			if(VehicleLoadBP > VehicleLoad){
				isBiggest = 0;
				break;
			}
			pRouter = pRouter->GetNextRouter();
		}
	}
	else if (m_StepOpt==2){
		CRouterL *pRouter = m_pRouterL;
		while(pRouter){
			pVehicleBP = pRouter->GetVehicle();
			strTypeNameBP = pVehicleBP->GetTypeName();
			strRegisterClassBP = pVehicleBP->GetRegisterClass();
			if (strcmp(strTypeName,strTypeNameBP) == 0){ 
				//выходящее судно пропускается
				pRouter = pRouter->GetNextRouter();
				continue;
			}
			if (flACV){
				if (strRegisterClassBP.Find("*ACV*") == -1){ //NOT ACV
					pRouter = pRouter->GetNextRouter();
					continue;
				}
			}else{ //Ship
				if (strRegisterClassBP.Find("*ACV*") != -1){ //ACV
					pRouter = pRouter->GetNextRouter();
					continue;
				}
			}
			VehicleLoadBP = pVehicleBP->GetLoad();
			if(VehicleLoadBP > VehicleLoad){
				isBiggest = 0;
				break;
			}
			pRouter = pRouter->GetNextRouter();
		}
	}
	else if (m_StepOpt==3){
		CRouterP *pRouter = m_pRouterP;
		while(pRouter){
			pVehicleBP = pRouter->GetVehicle();
			strTypeNameBP = pVehicleBP->GetTypeName();
			strRegisterClassBP = pVehicleBP->GetRegisterClass();
			if (strcmp(strTypeName,strTypeNameBP) == 0){ 
				//выходящее судно пропускается
				pRouter = pRouter->GetNextRouter();
				continue;
			}
			if (flACV){
				if (strRegisterClassBP.Find("*ACV*") == -1){ //NOT ACV
					pRouter = pRouter->GetNextRouter();
					continue;
				}
			}else{ //Ship
				if (strRegisterClassBP.Find("*ACV*") != -1){ //ACV
					pRouter = pRouter->GetNextRouter();
					continue;
				}
			}
			VehicleLoadBP = pVehicleBP->GetLoad();
			if(VehicleLoadBP > VehicleLoad){
				isBiggest = 0;
				break;
			}
			pRouter = pRouter->GetNextRouter();
		}
	}

	return isBiggest;
}


int CMarineTechEconomicEng::GetExpenciesACVSpace(CPort *pBasePort, CVehicle *pVehicleIn, int VehicleNum)
{
	int Space1,Space2, SpaceAvailable, SpaceDiff, PricePer100M2;
	int Expencies = 0;
	CRouter *pBasePlan = m_pRouter;

	// Площадка для СВП: добавление
	//a) space for current vehicles (суда претенденты на включение в план):
	Space1 = GetSpaceForACVs(pVehicleIn, VehicleNum);
	//b) space for vehicles, that already in Plan:
	Space2 = GetSpaceForACVs();
	SpaceAvailable = pBasePort->m_LandStructDataVirt.nACVSpace;
	SpaceDiff = (Space1+Space2)-SpaceAvailable;
	if (SpaceDiff>0){
		PricePer100M2 = pBasePort->m_LandStructDataVirt.nACVSpaceElemPrice;
		Expencies = (int)(PricePer100M2 * ((float)SpaceDiff/100));

		pBasePort->m_LandStructDataTmp.nACVSpace += SpaceDiff;
	}
	
	return Expencies;

}

int CMarineTechEconomicEng::GetExpenciesACVSpace(CPort *pBasePort, CVehicle *pVehicleIn, int VehicleNum, CVehicle *pVehicleBP, int VehicleBPNum)
{
	// Площадка для СВП

	int SpaceIn, SpaceBP, SpaceAllBP, SpaceAvailable, SpaceDiff, 
		PricePer100M2;
	CRouter *pBasePlan = m_pRouter;

	int Expencies = 0;

	//a) space for In and "Out" vehicles
	SpaceIn = GetSpaceForACVs(pVehicleIn, VehicleNum);
	SpaceBP = GetSpaceForACVs(pVehicleBP, VehicleBPNum);
	//b) space for all vehicles, that already in Plan:
	SpaceAllBP = GetSpaceForACVs();
	SpaceAvailable = pBasePort->m_LandStructDataVirt.nACVSpace;
	SpaceDiff = (SpaceAllBP-SpaceBP+SpaceIn)-SpaceAvailable;
	if (SpaceDiff>0){
		PricePer100M2 = pBasePort->m_LandStructDataVirt.nACVSpaceElemPrice;
		Expencies = (int)(PricePer100M2 * ((float)SpaceDiff/100));
	}
	
	return Expencies;
}

int CMarineTechEconomicEng::GetExpenciesACVRepair(CPort *pBasePort, int VehicleInLoad, int VehicleNum)
{
    int acv_num, acv_load, acv_a, acv_b;
	double acv_a1, acv_b1, Expencies, ExpencDelta;

	Expencies = 0;

	// Рем. база для СВП: добавление
	acv_num = pBasePort->m_LandStructDataVirt.RepairDetailsACV.nCapability;
	acv_load = pBasePort->m_LandStructDataVirt.RepairDetailsACV.nVhehicleLoad;
	acv_a = pBasePort->m_LandStructDataVirt.RepairDetailsACV.a;
	acv_b = pBasePort->m_LandStructDataVirt.RepairDetailsACV.b;
	if (acv_load <= 0)
		acv_load = 30; //stab for case of error in data
	acv_a1 = acv_a*sqrt((float)VehicleInLoad/acv_load);
	acv_b1 = acv_b*sqrt((float)VehicleInLoad/acv_load);
	if (acv_num == 0){ // Нет ремонтной базы
		Expencies = VehicleNum*acv_a1 + acv_b1;
	}
	else if (acv_num > 0){ // Ремонтная база существует и расчитана 
						   // на N(acv_num) судов грузоподъемности P(acv_load).
		ExpencDelta = (VehicleNum*acv_a1 + acv_b1) - (acv_num*acv_a + acv_b);
		if (ExpencDelta > 0)
			Expencies = ExpencDelta;
	}

	return (int)Expencies;
}

int CMarineTechEconomicEng::GetExpenciesShipMoor(CPort *pBasePort, CVehicle *pVehicleIn)
{
	int MoorLengthRequered, MoorLengthAvailable, MoorLengthDiff;
	int PricePer10M, Expencies;

	Expencies = 0;

	MoorLengthRequered = GetMoorLength(pVehicleIn);
	MoorLengthAvailable = pBasePort->m_LandStructDataVirt.nMoorLength;
	MoorLengthDiff = MoorLengthRequered - MoorLengthAvailable;
	if (MoorLengthDiff > 0){
		PricePer10M = pBasePort->m_LandStructDataVirt.nMoorElemPrice;
		Expencies = (int)(PricePer10M * ((float)MoorLengthDiff/10));
	}

	return Expencies;
}

int CMarineTechEconomicEng::GetExpenciesShipRepair(CPort *pBasePort, int VehicleInLoad, int VehicleNum)
{
    int ship_num, ship_load, ship_a, ship_b;
	double ship_a1, ship_b1, Expencies, ExpencDelta;

	Expencies = 0;

	// Рем. база для Судов: добавление
	ship_num = pBasePort->m_LandStructDataVirt.RepairDetailsShip.nCapability;
	ship_load = pBasePort->m_LandStructDataVirt.RepairDetailsShip.nVhehicleLoad;
	ship_a = pBasePort->m_LandStructDataVirt.RepairDetailsShip.a;
	ship_b = pBasePort->m_LandStructDataVirt.RepairDetailsShip.b;
	if (ship_load <= 0)
		ship_load = 500; //stab for case of error in data
	ship_a1 = ship_a*sqrt((float)VehicleInLoad/ship_load);
	ship_b1 = ship_b*sqrt((float)VehicleInLoad/ship_load);
	if (ship_num == 0){ // Нет ремонтной базы
		Expencies = VehicleNum*ship_a1 + ship_b1;
	}
	else if (ship_num > 0){ // Ремонтная база существует и расчитана 
						   // на N(ship_num) судов грузоподъемности P(ship_load).
		ExpencDelta = (VehicleNum*ship_a1 + ship_b1) - (ship_num*ship_a + ship_b);
		if (ExpencDelta > 0)
			Expencies = ExpencDelta;
	}

	return (int)Expencies;

}


int CMarineTechEconomicEng::GetExpenciesShipCourt(CPort *pBasePort, int PNB_num, CVehicle *pVehicleIn)
{
	//судно Амгуема: добавление площадки в ПНБ

	CString	strShipName = pVehicleIn->GetTypeName();
	if (strShipName.Find("Amguema")== -1){
		//входящее  судно не Amguema
		return 0;
	}
	int Expencies = 0;
	CLine *pLine;
	CRouter *pBasePlan = m_pRouter;

	//если в ПНБ нет площадки, то требуются инвестиции:
	if ((m_StepOpt == 0) || (m_StepOpt == 1)){//steps: 0, 1
		if (pBasePort->GetPNB(PNB_num).LandStructDataPPVirt.bShipCourtAvailable == 0){
				Expencies = pBasePort->GetPNB(PNB_num).LandStructDataPPVirt.nShipCourtInvestRequere;
		}
	}
	else if (m_StepOpt == 2){ //step 2.
		pLine = m_pRouterL->GetLine();
		while(pLine){
			PNB_num = pLine->GetPNBnum();
			if (pBasePort->GetPNB(PNB_num).LandStructDataPPVirt.bShipCourtAvailable == 0){
				Expencies += pBasePort->GetPNB(PNB_num).LandStructDataPPVirt.nShipCourtInvestRequere;
			}
			pLine = pLine->GetNextLine();
		}
	}

	return Expencies;
}


int CMarineTechEconomicEng::GetExpenciesUnicOrBiggestVehicle(CPort *pBasePort, int PNB_num, CVehicle *pVehicleIn, int VehicleNum, CVehicle *pVehicleBP, int VehicleBPNum, int res1)
{		
	int Expencies = 0;
	CLine *pLine;
	CRouter *pBasePlan = m_pRouter;

	int PricePer10M = pBasePort->m_LandStructDataVirt.nMoorElemPrice;
	int PricePer100M2 = pBasePort->m_LandStructDataVirt.nACVSpaceElemPrice;
	int VehicleInLoad = pVehicleIn->GetLoad();
	int VehicleBPLoad = pVehicleBP->GetLoad();
	CString strShipNameIn = pVehicleIn->GetTypeName();
	CString strShipNameBP = pVehicleBP->GetTypeName();



	if (res1 != -1){//входящее судно - ACV, выходяшее - SHIP
		
		//********** Base Port **********
		
		//Причал для судов, отмена:
		Expencies -= (int)(PricePer10M *((float)(pBasePort->m_LandStructDataVirt.nMoorLength)/10));
		//Рем. база для судов, отмена:

		//Площадка для СВП, добавление:
		Expencies += GetExpenciesACVSpace(pBasePort, pVehicleIn, VehicleNum);
		//Рем. база для СВП, добавление:
		Expencies += GetExpenciesACVRepair(pBasePort, VehicleInLoad, VehicleNum);
		
		//************ PNB **************
		
		//Если выходящее судно Амгуема, отмена площадки:
		//strShipNameBP = pVehicleBP->GetTypeName();
		if(strShipNameBP.Find("Amguema") != 1) {//Amguema
			if ((m_StepOpt == 0) || (m_StepOpt == 1)){
				Expencies -= pBasePort->GetPNB(PNB_num).LandStructDataPPVirt.nShipCourtInvestRequere;
			}
			else if (m_StepOpt == 2){
				pLine = m_pRouterL->GetLine(); //!! RouterL
		        while(pLine){
					PNB_num = pLine->GetPNBnum();
					Expencies -= pBasePort->GetPNB(PNB_num).LandStructDataPPVirt.nShipCourtInvestRequere;
					pLine = pLine->GetNextLine();
				}
			}
		}
	}//end of: входящее судно - ACV, выходяшее - SHIP
	else {//входящее судно - SHIP, выходяшее - ACV
		
		//********** Base Port **********
		
		//Площадка для СВП, отмена:
		Expencies -= (int)(PricePer100M2 *((float)(pBasePort->m_LandStructDataVirt.nACVSpace)/100));
		//Рем. база для СВП, отмена:

		//Причал для судов, добавление:
		Expencies += GetExpenciesShipMoor(pBasePort, pVehicleIn);
		//Рем. база для судов, добавление:
		Expencies += GetExpenciesShipRepair(pBasePort, VehicleInLoad, VehicleNum);
		
		//************ PNB **************
	
		//Если входящее судно Амгуема, добавление площадки:
		if(strShipNameIn.Find("Amguema") != 1) //Amguema
			Expencies += GetExpenciesShipCourt(pBasePort, PNB_num, pVehicleIn);

	}//end of: входящее судно - SHIP, выходяшее - ACV

	return Expencies;
}


int CMarineTechEconomicEng::GetLandStructExpenciesYearDeltaSt3PPs(CRouterP *pRouter)
{
	/*************************************************************\
	 Функция используется на втором этапе оптимизации: Step 3. PNB
	\*************************************************************/

	m_StepOpt = 3;
	int PNB_num;
	CVehicle *pVehicle = pRouter->GetVehicle();
	CString strShipName = pVehicle->GetTypeName();
	CString strRegisterClass = pVehicle->GetRegisterClass();
	CPort *pBasePort = pRouter->GetBasePort();

	if ((strShipName.Find("Amguema") == -1) && (strRegisterClass.Find("*ACV*") == -1)){
		//Изменения в ПНБ касаются только судов типа Амгуема и СВП
		return 0;
	}

	int VehicleNum = 1;
	CPortPoint* pPPoint = pRouter->GetPPoitsListHead();
	int Expencies = 0;

	//pBasePort->CleanLandDataUpdateStructure();
	
	if (strRegisterClass.Find("*ACV*") != -1){ // СВП
		while(pPPoint){
			PNB_num = pPPoint->GetPNBnum();
			if (pBasePort->GetPNB(PNB_num).LandStructDataPPVirt.bACVCourtUse == 0){
				//площадка не используется, т.е. инвестирование не произведено
				Expencies += pBasePort->GetPNB(PNB_num).LandStructDataPPVirt.nACVCourtInvestRequere;
			}
			pPPoint = pPPoint->GetNextPPoint();
		}
	
	}else { // водоизмещающее судно типа Амгуэма
	
		if (strShipName.Find("Amguema") == -1) //не Amguema
			return 0;
		while(pPPoint){
			PNB_num = pPPoint->GetPNBnum();
			if (pBasePort->GetPNB(PNB_num).LandStructDataPPVirt.bShipCourtUse == 0){
				//площадка не используется, т.е. инвестирование не произведено
				Expencies += pBasePort->GetPNB(PNB_num).LandStructDataPPVirt.nShipCourtInvestRequere;
			}
			pPPoint = pPPoint->GetNextPPoint();
		}
	} //end if ACV


	//защита для отладки: 
	if (Expencies > 9999999)
		Expencies = 9999999;

	return Expencies;
}

int CMarineTechEconomicEng::InitLandStructurePPVirt()
{
	BOOL bACVCourtAvailable, bShipCourtAvailable;
	int nACVCourtInvestRequere, nShipCourtInvestRequere, id;
	
    CPort *pBasePort = CPortList::Instance()->GetPortListHead();
	if (pBasePort == NULL)
		return 0;

	pBasePort->m_LandStructDataVirt.nACVSpace = pBasePort->m_LandStructData.nACVSpace;
	pBasePort->m_LandStructDataVirt.nACVSpaceElemPrice = pBasePort->m_LandStructData.nACVSpaceElemPrice;
	pBasePort->m_LandStructDataVirt.nMoorElemPrice = pBasePort->m_LandStructData.nMoorElemPrice;
	pBasePort->m_LandStructDataVirt.nMoorLength = pBasePort->m_LandStructData.nMoorLength;
	pBasePort->m_LandStructDataVirt.RepairDetailsACV = pBasePort->m_LandStructData.RepairDetailsACV;
	pBasePort->m_LandStructDataVirt.RepairDetailsShip = pBasePort->m_LandStructData.RepairDetailsShip;
	
	for (int i = 0; i < MAXPNBNUMBER; i++){
		id = pBasePort->GetPNB(i).id;
		if (id == 999)
			continue;
		bACVCourtAvailable = pBasePort->PNBList[i].LandStructDataPP.bACVCourtAvailable;
		bShipCourtAvailable = pBasePort->PNBList[i].LandStructDataPP.bShipCourtAvailable;
		nACVCourtInvestRequere = pBasePort->PNBList[i].LandStructDataPP.nACVCourtInvestRequere;
		nShipCourtInvestRequere = pBasePort->PNBList[i].LandStructDataPP.nShipCourtInvestRequere; 

		if ((bACVCourtAvailable == 0) && (nACVCourtInvestRequere <= 0)){
			nACVCourtInvestRequere = CalculateACVCourtPricePP();
		}
		if ((bShipCourtAvailable == 0) && (nShipCourtInvestRequere <= 0)){
			nShipCourtInvestRequere = CalculateShipCourtPricePP();
		}
		
		pBasePort->PNBList[i].LandStructDataPPVirt.bACVCourtAvailable = bACVCourtAvailable;
		pBasePort->PNBList[i].LandStructDataPPVirt.bShipCourtAvailable = bShipCourtAvailable;
		pBasePort->PNBList[i].LandStructDataPPVirt.nACVCourtInvestRequere = nACVCourtInvestRequere;
		pBasePort->PNBList[i].LandStructDataPPVirt.nShipCourtInvestRequere = nShipCourtInvestRequere;
    }
   
	return 1;
}

void CMarineTechEconomicEng::doInitLandStructurePPTmp()
{
	CPort *pBasePort = m_pRouter->GetBasePort();
	BOOL bACVCourtAvailable, bShipCourtAvailable;
	int nACVCourtInvestRequere, nShipCourtInvestRequere, id;
	
   for (int i = 0; i < MAXPNBNUMBER; i++){
		id = pBasePort->GetPNB(i).id;
		if (id == 999)
			continue;
	   bACVCourtAvailable = pBasePort->PNBList[i].LandStructDataPPVirt.bACVCourtAvailable;
		bShipCourtAvailable = pBasePort->PNBList[i].LandStructDataPPVirt.bShipCourtAvailable;
		nACVCourtInvestRequere = pBasePort->PNBList[i].LandStructDataPPVirt.nACVCourtInvestRequere;
		nShipCourtInvestRequere = pBasePort->PNBList[i].LandStructDataPPVirt.nShipCourtInvestRequere; 

		pBasePort->PNBList[i].LandStructDataPPTmp.bACVCourtAvailable = bACVCourtAvailable;
		pBasePort->PNBList[i].LandStructDataPPTmp.bShipCourtAvailable = bShipCourtAvailable;
		pBasePort->PNBList[i].LandStructDataPPTmp.nACVCourtInvestRequere = nACVCourtInvestRequere;
		pBasePort->PNBList[i].LandStructDataPPTmp.nShipCourtInvestRequere = nShipCourtInvestRequere;
    }
}

int CMarineTechEconomicEng::CalculateACVCourtPricePP()
{
	return 66666;	//stab, default value
}

int CMarineTechEconomicEng::CalculateShipCourtPricePP()
{
	return 88888; 	//stab, default value
}

int CMarineTechEconomicEng::ExpenciesYear(int Expencies)
{
	int ExpencYear;

	int Resourse; // срок службы берегового сооружения (лет)
    double Rent; // расчетный размер ренты 
    
    Resourse = 30;
    Rent = 0.06;

    ExpencYear = (int)(Expencies * Rent * (pow((1+ Rent),Resourse))/(pow((1+ Rent),Resourse) - 1));

	return ExpencYear;

}

/*
void CMarineTechEconomicEng::doUpdateLandStructuresVirt(CRouter *pRouter)
{
	CPort *pBasePort = m_pRouter->GetBasePort();
	BOOL bACVCourtAvailable, bShipCourtAvailable;
	int nACVCourtInvestRequere, nShipCourtInvestRequere;
	
   for (int i = 0;i<MAXPNBNUMBER;i++){
		bACVCourtAvailable = pBasePort->PNBList[i].LandStructDataPP.bACVCourtAvailable;
		bShipCourtAvailable = pBasePort->PNBList[i].LandStructDataPP.bShipCourtAvailable;
		nACVCourtInvestRequere = pBasePort->PNBList[i].LandStructDataPP.nACVCourtInvestRequere;
		nShipCourtInvestRequere = pBasePort->PNBList[i].LandStructDataPP.nShipCourtInvestRequere; 

		if ((bACVCourtAvailable == 0) && (nACVCourtInvestRequere <= 0)){
			nACVCourtInvestRequere = CalculateACVCourtPricePP();
		}
		if ((bShipCourtAvailable == 0) && (nShipCourtInvestRequere <= 0)){
			nShipCourtInvestRequere = CalculateShipCourtPricePP();
		}
		
		pBasePort->PNBList[i].LandStructDataPPVirt.bACVCourtAvailable = bACVCourtAvailable;
		pBasePort->PNBList[i].LandStructDataPPVirt.bShipCourtAvailable = bShipCourtAvailable;
		pBasePort->PNBList[i].LandStructDataPPVirt.nACVCourtInvestRequere = nACVCourtInvestRequere;
		pBasePort->PNBList[i].LandStructDataPPVirt.nShipCourtInvestRequere = nShipCourtInvestRequere;
    }

}
*/
/*
int CMarineTechEconomicEng::UpdateLandStructVirtualDataSt1(CRouter *pRouter, CVehicle *pVehicleIn, int VehicleInNumber)
{
	m_StepOpt = 1;
	/*********************************************\
	Замечание. Для	1-го этапа оптимизации, с маятниковыми 
	маршрутами, замена маршрута сводится к замене судна 
	на маршруте. Линия перевозки - единственная и неизменна.
	/*********************************************

	int PricePer10M, PricePer100M2;
	int SpaceVIn, SpaceVOut, SpaceALLBP, SpaceAvailable, SpaceDiff,
		amguema_In, amguema_Out, amg_numExistInPlan;
	int MoorLengthIn, MoorLengthOut, MoorLengthAvailable;
	    

	CRouter *pBasePlan = CBasePlanSt1LP::Instance()->GetRoutersListHead();
	if (pBasePlan == NULL)
		return 0;
	m_pRouter = pBasePlan;

	CPort *pBasePort = pRouter->GetBasePort();
	int PNB_num = pRouter->GetLine()->GetPNBnum();
	CVehicle *pVehicleOut = pRouter->GetVehicle();
	int VehicleOutNumber = pRouter->GetVehicleNumber();
	int VehicleInLoad = pVehicleIn->GetLoad();
	int VehicleOutLoad = pVehicleOut->GetLoad();

	CString strShipNameIn = pVehicleIn->GetTypeName();
	CString strShipNameOut = pVehicleOut->GetTypeName();
	
	CString strRegisterClassVehicleIn = pVehicleIn->GetRegisterClass();
	CString strRegisterClassVehicleOut = pVehicleOut->GetRegisterClass();
	int res1 = strRegisterClassVehicleIn.Find("*ACV*");
	int res2 = strRegisterClassVehicleOut.Find("*ACV*");
	PricePer100M2 = pBasePort->m_LandStructDataVirt.nACVSpaceElemPrice;
	PricePer10M = pBasePort->m_LandStructDataVirt.nMoorElemPrice;


	if ((res1 != -1) && (res2 != -1)){
		//Оба судна - СВП
	
		//************Base Port **************
		//Space delta:
		SpaceVIn = GetSpaceForACVs(pVehicleIn, VehicleInNumber);
		SpaceVOut = GetSpaceForACVs(pVehicleOut, VehicleOutNumber);
		//b) space for all vehicles, that already in Plan:
		SpaceALLBP = GetSpaceForACVs();
		SpaceAvailable = pBasePort->m_LandStructDataVirt.nACVSpace;
		SpaceDiff = (SpaceALLBP-SpaceVOut+SpaceVIn)-SpaceAvailable;
		if (SpaceDiff>0){
			pBasePort->m_LandStructDataVirt.nACVSpace += SpaceDiff;
		}
		// Ремонтные базы: нет изменений
		//???
		//*********** PNB: нет изменений
	}//end of: Оба судна - СВП
	else if ((res1 == -1) && (res2 == -1)){
		//Оба судна - водоизмещающие суда

		//Если одно из судов - типа Амгуэма,
		//то проверяется береговая площадка в ПНБ
		amguema_In = strShipNameIn.Find("Amguema");
		amguema_Out = strShipNameOut.Find("Amguema");
		amg_numExistInPlan = IsExistInPlanAmguema();
		if (amguema_In != -1) {	//входящее  судно - Amguema
			pBasePort->PNBList[PNB_num].LandStructDataPPVirt.bShipCourtAvailable = 1;
			pBasePort->PNBList[PNB_num].LandStructDataPPVirt.bShipCourtUse = 1;
		}
		else if ((amguema_In == -1)&& (amguema_Out != -1) && (amg_numExistInPlan == 1)){
		//"входящее - не Amguema, выходящее судно - Амгуема, единственное в плане:
		//береговой площадки д.б. отменена
			pBasePort->PNBList[PNB_num].LandStructDataPPVirt.bShipCourtUse = 0;
			pBasePort->PNBList[PNB_num].LandStructDataPPVirt.bShipCourtAvailable = pBasePort->PNBList[PNB_num].LandStructDataPP.bShipCourtAvailable;
			pBasePort->PNBList[PNB_num].LandStructDataPPVirt.nShipCourtInvestRequere = pBasePort->PNBList[PNB_num].LandStructDataPP.nShipCourtInvestRequere;
		} // end of: Amguema


		//************Base Port **************
		// Причал для судов-снабженцев в базовом пункте строится один
		// и расчитывается на судно наибольшей длины

		MoorLengthIn = GetMoorLength(pVehicleIn);
		MoorLengthOut = GetMoorLength(pVehicleOut);
		MoorLengthAvailable = pBasePort->m_LandStructDataVirt.nMoorLength;
		if ((MoorLengthIn > MoorLengthOut)&&(MoorLengthIn > MoorLengthAvailable)){
			pBasePort->m_LandStructDataVirt.nMoorLength = MoorLengthIn;
		}

		// Ремонтные базы:
		// изменение пока не реализовано

	} //end of: Оба судна - водоизмещающие суда

	else {//Входящее и выходящее суда - разного типа
		if ((IsUnicTypeVehicleOut(pVehicleOut) == 1) ||(IsUnicTypeVehicleIn(pVehicleIn) == 1) || 
			(IsBiggestVehicleOut(pVehicleOut) == 1) || (IsBiggestVehicleIn(pVehicleIn) == 1)) {
			if (res1 != -1){//входящее судно - ACV, выходяшее - SHIP
	
				//********** Base Port **********
				//Причал для судов:
				MoorLengthOut = GetMoorLength(pVehicleOut);
				MoorLengthAvailable = pBasePort->m_LandStructDataVirt.nMoorLength;
				if ((MoorLengthOut > MoorLengthAvailable)){
					pBasePort->m_LandStructDataVirt.nMoorLength = MoorLengthAvailable;
				}
				//Рем. база для судов, отмена:
				//????
				//Площадка для СВП, добавление:
				SpaceVIn = GetSpaceForACVs(pVehicleIn, VehicleInNumber);
				SpaceALLBP = GetSpaceForACVs();
				SpaceAvailable = pBasePort->m_LandStructDataVirt.nACVSpace;
				SpaceDiff = (SpaceALLBP+SpaceVIn)-SpaceAvailable;
				if (SpaceDiff>0){
					pBasePort->m_LandStructDataVirt.nACVSpace += SpaceDiff;
				}
				
				//Рем. база для СВП, добавление:
				//???

				//************ PNB **************
				//Если выходящее судно Амгуема, отмена площадки:
				//strShipNameBP = pVehicleBP->GetTypeName();
				if(strShipNameOut.Find("Amguema") != 1) {//Amguema
					pBasePort->PNBList[PNB_num].LandStructDataPPVirt.bShipCourtUse = 0;
					pBasePort->PNBList[PNB_num].LandStructDataPPVirt.bShipCourtAvailable = pBasePort->PNBList[PNB_num].LandStructDataPP.bShipCourtAvailable;
					pBasePort->PNBList[PNB_num].LandStructDataPPVirt.nShipCourtInvestRequere = pBasePort->PNBList[PNB_num].LandStructDataPP.nShipCourtInvestRequere;
				}
				// ACV - In:
				pBasePort->PNBList[PNB_num].LandStructDataPPVirt.bACVCourtUse = 1;
				pBasePort->PNBList[PNB_num].LandStructDataPPVirt.bACVCourtAvailable = 1;

			}//end of: входящее судно - ACV, выходяшее - SHIP
			else {//входящее судно - SHIP, выходяшее - ACV
				
				//********** Base Port **********
				//Причал для судов ("добавление"):
				MoorLengthIn = GetMoorLength(pVehicleIn);
				MoorLengthAvailable = pBasePort->m_LandStructDataVirt.nMoorLength;
				if (MoorLengthIn > MoorLengthAvailable)
					pBasePort->m_LandStructDataVirt.nMoorLength = MoorLengthIn;
				
				//Площадка для СВП ("отмена"):
				SpaceVOut = GetSpaceForACVs(pVehicleOut, VehicleOutNumber);
				SpaceALLBP = GetSpaceForACVs();
				SpaceALLBP -= SpaceVOut;
				SpaceAvailable = pBasePort->m_LandStructDataVirt.nACVSpace;
				if (SpaceALLBP > SpaceAvailable)
					pBasePort->m_LandStructDataVirt.nACVSpace = SpaceALLBP;
				
				//Рем. базы ???:
				
				//************ PNB **************
			
				//Если входящее судно Амгуема, добавление площадки:
				if(strShipNameIn.Find("Amguema") != 1) {//Amguema
					pBasePort->PNBList[PNB_num].LandStructDataPPVirt.bShipCourtAvailable = 1;
					pBasePort->PNBList[PNB_num].LandStructDataPPVirt.bShipCourtUse = 1;
				}

			}//end of: входящее судно - SHIP, выходяшее - ACV
		}
	}
	
	return 1;
}
*/

int CMarineTechEconomicEng::UpdateLandStructVirtualDataSt0()
{
	m_StepOpt = 0;
	CVehicle *pVehicle;
	CString sShipName, sRegisterClass;
	BOOL is_Amguema, is_ACV;
	int SpaceACVCourt, SpaceAvailable, SpaceDiff;
	int MoorLength, MoorLengthAvailable, MoorLengthDiff;
	int id;


	CRouter *pRouter = CInitialBasePlan::Instance()->GetRoutersListHead();
	if (pRouter == NULL)
		return 0;

	CPort *pBasePort = pRouter->GetBasePort();
	
	//****** Base Port *******
	// ACV Court:
	SpaceACVCourt = GetSpaceForACVs();
	SpaceAvailable = pBasePort->m_LandStructDataVirt.nACVSpace;
	SpaceDiff = SpaceACVCourt - SpaceAvailable;
	if (SpaceDiff>0){
		pBasePort->m_LandStructDataVirt.nACVSpace = SpaceAvailable+SpaceDiff;
	}
	
	// Ships Moor
	MoorLength = GetMoorLength();
	MoorLengthAvailable = pBasePort->m_LandStructDataVirt.nMoorLength;
	MoorLengthDiff = MoorLength - MoorLengthAvailable;
	if (MoorLengthDiff > 0){
		pBasePort->m_LandStructDataVirt.nMoorLength = MoorLengthDiff;
	}

	// Ремонтные базы в базовом порту
	//???

	//****** PNB *******
	for (int PNB_num = 0; PNB_num < MAXPNBNUMBER; PNB_num++){
		id = pBasePort->GetPNB(PNB_num).id;
		if (id == 999)
			continue;
		while(pRouter){
			pVehicle = pRouter->GetVehicle();
			sShipName = pVehicle->GetTypeName();
			sRegisterClass = pVehicle->GetRegisterClass();
			is_Amguema = sShipName.Find("Amguema");
			is_ACV = sRegisterClass.Find("*ACV*");
			if(is_ACV){
				pBasePort->PNBList[PNB_num].LandStructDataPPVirt.bACVCourtUse = 1;
				pBasePort->PNBList[PNB_num].LandStructDataPPVirt.bACVCourtAvailable = 1;
			}
			else if(is_Amguema){
				pBasePort->PNBList[PNB_num].LandStructDataPPVirt.bShipCourtAvailable = 1;
				pBasePort->PNBList[PNB_num].LandStructDataPPVirt.bShipCourtUse = 1;
			}

			pRouter = pRouter->GetNextRouter();
		}
	}

	return 1;

}

int CMarineTechEconomicEng::UpdateLandStructVirtualDataSt1()
{
	m_StepOpt = 1;
	CVehicle *pVehicle;
	CString sShipName, sRegisterClass;
	BOOL is_Amguema, is_ACV;
	int SpaceACVCourt, SpaceAvailable, SpaceDiff;
	int MoorLength, MoorLengthAvailable, MoorLengthDiff;
	int id;


	CRouter *pRouter = CBasePlanSt1LP::Instance()->GetRoutersListHead();
	if (pRouter == NULL)
		return 0;

	CPort *pBasePort = pRouter->GetBasePort();
	
	//****** Base Port *******
	// ACV Court:
	SpaceACVCourt = GetSpaceForACVs();
	SpaceAvailable = pBasePort->m_LandStructDataVirt.nACVSpace;
	SpaceDiff = SpaceACVCourt - SpaceAvailable;
	if (SpaceDiff>0){
		pBasePort->m_LandStructDataVirt.nACVSpace = SpaceAvailable+SpaceDiff;
	}
	
	// Ships Moor
	MoorLength = GetMoorLength();
	MoorLengthAvailable = pBasePort->m_LandStructDataVirt.nMoorLength;
	MoorLengthDiff = MoorLength - MoorLengthAvailable;
	if (MoorLengthDiff > 0){
		pBasePort->m_LandStructDataVirt.nMoorLength = MoorLengthDiff;
	}

	// Ремонтные базы в базовом порту
	//???

	//****** PNB *******
	for (int PNB_num = 0; PNB_num < MAXPNBNUMBER; PNB_num++){
		id = pBasePort->GetPNB(PNB_num).id;
		if (id == 999)
			continue;
		while(pRouter){
			pVehicle = pRouter->GetVehicle();
			sShipName = pVehicle->GetTypeName();
			sRegisterClass = pVehicle->GetRegisterClass();
			is_Amguema = sShipName.Find("Amguema");
			is_ACV = sRegisterClass.Find("*ACV*");
			if(is_ACV){
				pBasePort->PNBList[PNB_num].LandStructDataPPVirt.bACVCourtUse = 1;
				pBasePort->PNBList[PNB_num].LandStructDataPPVirt.bACVCourtAvailable = 1;
			}
			else if(is_Amguema){
				pBasePort->PNBList[PNB_num].LandStructDataPPVirt.bShipCourtAvailable = 1;
				pBasePort->PNBList[PNB_num].LandStructDataPPVirt.bShipCourtUse = 1;
			}

			pRouter = pRouter->GetNextRouter();
		}
	}

	return 1;

}

int CMarineTechEconomicEng::UpdateLandStructVirtualDataSt2()
{
	//Функция должна вызываться из COptimizationSt2LpDp::SimplexMethod()
	//после каждого изменения маршрута базового плана.
	//Просматривается весь план.


	m_StepOpt = 2;
	CLine *pLine;
	CVehicle *pVehicle;
	CString sShipName, sRegisterClass;
	BOOL is_Amguema, is_ACV;
	int SpaceACVCourt, SpaceAvailable, SpaceDiff;
	int MoorLength, MoorLengthAvailable, MoorLengthDiff;
	int PNB_num;
	int id;


	CRouterL *pRouter = CBasePlanSt2LpDp::Instance()->GetRoutersListHead();
	if (pRouter == NULL)
		return 0;

	CPort *pBasePort = pRouter->GetBasePort();
	
	//****** Base Port *******
	// ACV Court:
	SpaceACVCourt = GetSpaceForACVs();
	SpaceAvailable = pBasePort->m_LandStructDataVirt.nACVSpace;
	SpaceDiff = SpaceACVCourt - SpaceAvailable;
	if (SpaceDiff>0){
		pBasePort->m_LandStructDataVirt.nACVSpace = SpaceAvailable+SpaceDiff;
	}
	
	// Ships Moor
	MoorLength = GetMoorLength();
	MoorLengthAvailable = pBasePort->m_LandStructDataVirt.nMoorLength;
	MoorLengthDiff = MoorLength - MoorLengthAvailable;
	if (MoorLengthDiff > 0){
		pBasePort->m_LandStructDataVirt.nMoorLength = MoorLengthDiff;
	}

	// Ремонтные базы в базовом порту
	//???

	//****** PNB *******
	for (int num = 0; num < MAXPNBNUMBER; num++){
		id = pBasePort->GetPNB(num).id;
		if (id == 999)
			continue;
		while(pRouter){
			pLine = pRouter->GetLine();
			pVehicle = pRouter->GetVehicle();
			sShipName = pVehicle->GetTypeName();
			sRegisterClass = pVehicle->GetRegisterClass();
			is_Amguema = sShipName.Find("Amguema");
			is_ACV = sRegisterClass.Find("*ACV*");
			while(pLine){
				PNB_num = pLine->GetPNBnum();
				if(PNB_num == num){
					if(is_ACV){
						pBasePort->PNBList[PNB_num].LandStructDataPPVirt.bACVCourtUse = 1;
						pBasePort->PNBList[PNB_num].LandStructDataPPVirt.bACVCourtAvailable = 1;
					}
					else if(is_Amguema){
						pBasePort->PNBList[PNB_num].LandStructDataPPVirt.bShipCourtAvailable = 1;
						pBasePort->PNBList[PNB_num].LandStructDataPPVirt.bShipCourtUse = 1;
					}
				}
				pLine = pLine->GetNextLine();
			}
			pRouter = pRouter->GetNextRouter();
		}

	}

	return 1;
}


