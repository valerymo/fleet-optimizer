// OptimizationSt3Dp.cpp: implementation of the COptimizationSt3Dp class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "FleetOptimizer.h"
#include "OptimizationSt3Dp.h"
#include "BasePlanSt3Dp.h"
#include "Cargo.h"
#include "OutputView.h"
#include "MarineTechEconomicEng.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


COptimizationSt3Dp* COptimizationSt3Dp::_instance = 0;

COptimizationSt3Dp* COptimizationSt3Dp::Instance(){
    if (_instance == 0){
        _instance = new COptimizationSt3Dp;
    }
    return _instance;
}


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

COptimizationSt3Dp::COptimizationSt3Dp()
{
    m_RoutersList = 0;
	m_DynamicRoutersList = 0;
	m_DynamicRoutersListPrev = 0;
	//MessageBox(NULL,"COptimizationSt3Dp()","COptimizationSt3Dp",MB_OK);

	POSITION pos = ((CFleetOptimizerApp*)AfxGetApp())->GetCurrFleetOptimizerDoc()->GetFirstViewPosition();
	m_pOutView = (COutputView*) ((CFleetOptimizerApp*)AfxGetApp())->GetCurrFleetOptimizerDoc()->GetNextView(pos);
	m_pOutView = (COutputView*) ((CFleetOptimizerApp*)AfxGetApp())->GetCurrFleetOptimizerDoc()->GetNextView(pos);
	m_pOutView->TextOut(" ");
	m_pOutView->TextOut("3rd stage of Optimization - Dynamic programming");
	m_pOutView->TextOut("Routes - list of PortPoints");


}

COptimizationSt3Dp::~COptimizationSt3Dp()
{

}

int COptimizationSt3Dp::OptimisationDynProgMethod()
{
	
	CRouterP *pRouter;
	//CRouterP *pRouterInitHead;
	CPort *pBasePort;
	int cargo_type, PNB_num;
	int BuildCoastYear, VehicleNum;
	CVehicle *pVehicle;

	CBasePlanSt3Dp::Instance()->CreateBasePlanInit(0);

	//pRouter = CBasePlanSt3Dp::Instance()->GetRoutersListHead();
	//pBasePort = pRouter->GetBasePort();
	//pRouterInitHead = CBasePlanSt3Dp::Instance()->GetRoutersListHead();
	//pBasePort = pRouterInitHead->GetBasePort();

	pBasePort = CPortList::Instance()->GetPortListHead();

//	while(pBasePort){

	long Investment = CFleetTechEconomicEng::Instance()->GetInvestmentYear();


	cargo_type = 0; //stab for router creation
	PNB_num = 0;	//stab for router creation
	int optimumFlag = 0;
	int step_SM = 0;

	if ((COptimizationSt1LP::Instance()->IsStepDone() == 0) && (COptimizationSt2LpDp::Instance()->IsStepDone() == 0))
		ResetExistVehicleNum();

	while(optimumFlag != 1)
	{
		step_SM++;
		if (step_SM > 100){
			MessageBox(NULL,"3-этап оптимизации: OptimisationDynProgMethod()","Остановка на шаге",MB_OK);
			m_pOutView->TextOut("Остановка на шаге. 3-этап оптимизации: OptimisationDynProgMethod()");
			break;
		}
		pRouter = new CRouterP(pBasePort, PNB_num, cargo_type);
        if (RouterCreate(pRouter) == 0){
				MessageBox(NULL,"Ошибка построения маршрута!","3-этап оптимизации: OptimisationDynProgMethod()",MB_OK);
				m_pOutView->TextOut("Ошибка построения маршрута!  3-этап оптимизации: OptimisationDynProgMethod()");
				return 0;
		}
		CBasePlanSt3Dp::Instance()->append(pRouter);
		UpdatePNBsCargoRequirments(pRouter);

		//investment for Year ...
		VehicleNum = pRouter->GetVehicleNumber();
		pVehicle =	pRouter->GetVehicle();
		BuildCoastYear = CFleetTechEconomicEng::Instance()->VehicleBuildCoastYearPay(pVehicle);
		Investment += (long)(VehicleNum*BuildCoastYear);
		
		//

		if (pBasePort->is_CargoRequired() == 0){ // 0: груз не требуется:
			optimumFlag = 1;					// все перевозки выполнены.
			//MessageBox(NULL,"Выполнено!","3-этап оптимизации. OptimisationDynProgMethod()",MB_OK);
			m_pOutView->TextOut("Completed - 3rd stage of Optimization -  DynamicProgrammingMethod");
		}
	}
 	
	print();
	return 1;
}

int COptimizationSt3Dp::RouterCreate(CRouterP *&pRouter)
{
	CVehicle* pVehicle;
	int res, routerIncomeBestOnStep, routerIncomeBestOnStepPrev;
	
	int flBestRouterFound = 0;
	m_RouterBest = 0;
	m_RouterBestOnStep = 0;
	m_RouterBestOnStepPrev = 0;
	routerIncomeBestOnStep = 0;
	routerIncomeBestOnStepPrev = -1000000;


    
	pVehicle = CVehiclesList::Instance()->GetVehicleListHead();

	//invest
	int BuildCoastYear;
	long lInvestmentYearLimit = CFleetTechEconomicEng::Instance()->GetInvestmentYearLimit();
	long lInvestmentYear = CFleetTechEconomicEng::Instance()->GetInvestmentYear();

	//Капиталовложения в береговые сооружения
	BOOL bIncludeLandStructCalc = FALSE;
	int LandStructYearPayBasePort = 0;
	int LandStructYearPayPPs = 0;
	bIncludeLandStructCalc = ((CFleetOptimizerApp*)AfxGetApp())->GetProfileInt("FleetOptimizer","IncludeLandStructCalc",0);	
	
    while(pVehicle){   		
		pRouter->SetVehicle(pVehicle);

		//Проверка возиожности добавления судна 
		//с точки зрения капиталовложений:
		if(lInvestmentYearLimit > 0){
			//Land:
			if (bIncludeLandStructCalc){
				LandStructYearPayBasePort = CMarineTechEconomicEng::Instance()->GetLandStructExpenciesYearDeltaSt3BasePort(pVehicle);
			}
			//end of Land

			BuildCoastYear = CFleetTechEconomicEng::Instance()->VehicleBuildCoastYearPay(pVehicle);
			if (((lInvestmentYear+BuildCoastYear+LandStructYearPayBasePort)> lInvestmentYearLimit) && (!(pVehicle->IsExistForce()))){
				pVehicle = pVehicle->GetNextVehicle();
				continue;			
			}
		}
		
		m_DynamicRoutersList = 0;
		m_DynamicRoutersListPrev = 0;

		if (CreateDynamicRoutersFirst(pRouter)==0){
			//MessageBox(NULL,"Dynamic routers creation problem!","STEP3. CreateDynamicRoutersFirst",MB_OK);
			pVehicle = pVehicle->GetNextVehicle();
			continue;			
		}
		
		//Выяснение количества пунктов в Районе:
		int number_OfPNB = pRouter->GetBasePort()->GetLinesNumber();
		int step = 1; //шаг добавления звена к маршруту
		while(1){
			//if (step > number_OfPNB)
			if (step > 50)
				break;	
			res = CreateDynamicRoutersNext();
			if	(res == 1){ //Переходим к следующему шагу построения маршрутов.
				step++; //step done
				CheckUpdateDynamicRoutersListOnStep(step);
				UpdateDynamicRoutersListPrev();
			}
			else if (res == 10)
				break;
			else {//(res == 0)
				MessageBox(NULL,"Ощибка построения динамических маршрутов!","3-этап оптимизации: RouterCreate(), CreateDynamicRoutersNext()",MB_OK);
				m_pOutView->TextOut("Ощибка построения динамических маршрутов!. 3-этап оптимизации: RouterCreate(), CreateDynamicRoutersNext()");
				return 0;
			}

		}

		//Выбрать лучший маршрут из построенного ряда допустимых маршрутов для заданного судна
		if (FindDynamicRouterBest1()==0){
			MessageBox(NULL,"Ощибка построения динамических маршрутов!","3-этап оптимизации: RouterCreate(), FindDynamicRouterBest()",MB_OK);
			m_pOutView->TextOut("Ощибка построения динамических маршрутов! 3-этап оптимизации: RouterCreate(), FindDynamicRouterBest()");
			return 0;
		}

		//Сравнить лучшие маршруты судов-претендентов данного и предыдущего шага и выбрать из них лучший
		routerIncomeBestOnStep = m_RouterBestOnStep->vehicleIncome;
		if (routerIncomeBestOnStep > routerIncomeBestOnStepPrev){
			if ((m_RouterBestOnStepPrev == 0) || (!(m_RouterBestOnStepPrev->GetVehicle()->IsExistForce()))
				|| ((m_RouterBestOnStepPrev->GetVehicle()->IsExistForce()) && (m_RouterBestOnStep->GetVehicle()->IsExistForce()))){
				m_RouterBest = m_RouterBestOnStep;
				routerIncomeBestOnStepPrev = routerIncomeBestOnStep;
			}
		}

		pVehicle = pVehicle->GetNextVehicle();

	}    

	//if (FindDynamicRouterBest1()==0){
	//	MessageBox(NULL,"Ощибка построения динамических маршрутов!","3-этап оптимизации: RouterCreate(), FindDynamicRouterBest()",MB_OK);
	//	m_pOutView->TextOut("Ощибка построения динамических маршрутов! 3-этап оптимизации: RouterCreate(), FindDynamicRouterBest()");
	//	return 0;
	//}


	pRouter = m_RouterBest;
	if (pRouter == 0)
		return 0;
	
	normRouter(pRouter);
	//ShipLoadingPlanCreate(pRouter);

	//investment data update
	if(lInvestmentYearLimit > 0){
		pVehicle = pRouter->GetVehicle();
		BuildCoastYear = CFleetTechEconomicEng::Instance()->VehicleBuildCoastYearPay(pVehicle);
		lInvestmentYear += BuildCoastYear;
		if (bIncludeLandStructCalc){
			LandStructYearPayPPs = CMarineTechEconomicEng::Instance()->GetLandStructExpenciesYearDeltaSt3PPs(pRouter);
			lInvestmentYear += LandStructYearPayBasePort;
			lInvestmentYear += LandStructYearPayPPs;
			if (lInvestmentYear > lInvestmentYearLimit){
				MessageBox(NULL,"Превышение лимитов капиталовложений в береговые структуры флота!","3-этап оптимизации: RouterCreate()",MB_OK);
				m_pOutView->TextOut("Превышение лимитов капиталовложений в береговые структуры флота! 3-этап оптимизации: RouterCreate(), FindDynamicRouterBest()");
				return 15;
			}
		}
		CFleetTechEconomicEng::Instance()->SetInvestmentYear(lInvestmentYear);
	}

	return 1;
}
/*
int COptimizationSt3Dp::CreateDynamicRoutersFirst(CRouterP *pRouterP)
{
	CPort	*pBasePort, *pBasePortCur;
	CVehicle *pVehicle;
	int cargo_type, PNB_num, PNBRequiredCargo, VehicleLoad;
	int vehicleOperationDaysInYearAll, daysRemain, RouterTime;
	
	int routers_count = 0;
	

	CRouterP *pRouter = CBasePlanSt3Dp::Instance()->GetRoutersListHead();
    if (pRouter == NULL){
        return 0;
    }

	pVehicle = pRouterP->GetVehicle();
	VehicleLoad = pVehicle->GetLoad();

	pBasePort = pRouterP->GetBasePort();

	vehicleOperationDaysInYearAll = CFleetTechEconomicEng::Instance()->GetVehicleOperationsDaysInYear(pVehicle->GetRegisterClass(),pBasePort->GetPortRegion());

// "requered cargo" array initialisation:
	int requered_cargo[MAXPNBNUMBER][5];
	int num_pnb, num_cargo, id;
	for (num_pnb = 0; num_pnb < MAXPNBNUMBER; num_pnb++){
		id = pBasePort->GetPNB(num_pnb).id;
		if (id == 999)
			continue;
		for (num_cargo = 1; num_cargo <= 4; num_cargo++){
			// ??? проверить ...pRouter или pRouterP BasePort//  requered_cargo[num_pnb][num_cargo] = pRouter->GetBasePort()->GetPNBRequiredCargo(num_pnb,num_cargo);
			requered_cargo[num_pnb][num_cargo] = pRouter->GetBasePort()->GetPNBRequiredCargo(num_pnb,num_cargo);			
			//requered_cargo[num_pnb][num_cargo] = pBasePort->GetPNBRequiredCargo(num_pnb,num_cargo);
		}
	}
// "requered cargo" array initialisation end.


    while(pRouter){
		pBasePortCur = pRouter->GetBasePort();
		if (pBasePortCur != pBasePort){
			pRouter = pRouter->GetNextRouter();
			continue;
		}
		
		PNB_num = pRouter->GetPPoitsListHead()->GetPNBnum();
		cargo_type =	pRouter->GetCargoType();	

		PNBRequiredCargo = requered_cargo[PNB_num][cargo_type];
		if (PNBRequiredCargo == 0){
			pRouter = pRouter->GetNextRouter();
			continue;	
		}

        if(CFleetTechEconomicEng::Instance()->VehicleRouterPLineValidation(pBasePort,PNB_num,cargo_type,pVehicle)==0){
			pRouter = pRouter->GetNextRouter();
			continue;
		}
		
//		//PNBRequiredCargo = pRouter->GetBasePort()->GetPNBRequiredCargo(PNB_num,cargo_type);
//		PNBRequiredCargo = requered_cargo[PNB_num][cargo_type];
//		if (PNBRequiredCargo == 0){
//			pRouter = pRouter->GetNextRouter();
//			continue;	
//		}		

		CRouterP *pDynRouter = new CRouterP(pBasePort, PNB_num, cargo_type);
		pDynRouter->SetVehicle(pVehicle);
		RouterTime = pDynRouter->GetRouterTime();
		if (RouterTime > vehicleOperationDaysInYearAll){
			delete pDynRouter;
			pRouter = pRouter->GetNextRouter();
			continue;	
		}
	
		daysRemain = vehicleOperationDaysInYearAll - RouterTime;
		pDynRouter->SetVehicleOperationsDaysRemain(daysRemain);
		appendDynRList(pDynRouter);

		requered_cargo[PNB_num][cargo_type] -= VehicleLoad;
		if (requered_cargo[PNB_num][cargo_type] < 0)
			requered_cargo[PNB_num][cargo_type] = 0;


		if (CheckPNBsCargoRequirments(pDynRouter) == 0){
			MessageBox(NULL,"CreateDynamicRoutersFirst:\nPNB Requered Cargo Problem","STEP3. CheckPNBsCargoRequirments",MB_OK);
			m_pOutView->TextOut("Requered Cargo Problem. OptimizationSt3Dp::CreateDynamicRoutersFirst");
		}


		pRouter = pRouter->GetNextRouter();
		routers_count++;
	}	

	if (routers_count == 0)
		return 0; // не построено ни одного маршрута
	
	return 1;
}

*/

int COptimizationSt3Dp::CreateDynamicRoutersFirst(CRouterP *pRouterP)
{
	CPort	*pBasePort, *pBasePortPlan;
	CVehicle *pVehicle;
	//int cargo_type, PNB_num;
	int PNBRequiredCargo, VehicleLoad;
	int vehicleOperationDaysInYearAll, daysRemain, RouterTime;
	int load;
	
	int routers_count = 0;
	

	//CRouterP *pRouter = CBasePlanSt3Dp::Instance()->GetRoutersListHead();
    //if (pRouter == NULL){
    //    return 0;
    //}

	pBasePortPlan = CPortList::Instance()->GetPortListHead();
    if (pBasePortPlan == NULL){
        return 0;
    }

	pVehicle = pRouterP->GetVehicle();
	VehicleLoad = pVehicle->GetLoad();

	pBasePort = pRouterP->GetBasePort();

	vehicleOperationDaysInYearAll = CFleetTechEconomicEng::Instance()->GetVehicleOperationsDaysInYear(pVehicle->GetRegisterClass(),pBasePort->GetPortRegion());

// "requered cargo" array initialisation:
	int requered_cargo[MAXPNBNUMBER][5];
	int num_pnb, num_cargo, id;
	for (num_pnb = 0; num_pnb < MAXPNBNUMBER; num_pnb++){
		id = pBasePort->GetPNB(num_pnb).id;
		if (id == 999)
			continue;
		for (num_cargo = 1; num_cargo <= 4; num_cargo++){
			requered_cargo[num_pnb][num_cargo] = pBasePortPlan->GetPNBRequiredCargo(num_pnb,num_cargo);			
		}
	}
// "requered cargo" array initialisation end.


    while(pBasePortPlan){
		//pBasePortCur = pRouter->GetBasePort();
		if (pBasePortPlan != pBasePort){
			//pRouter = pRouter->GetNextRouter();
			continue;
		}
		
		for (num_pnb = 0; num_pnb < MAXPNBNUMBER; num_pnb++){
			id = pBasePort->GetPNB(num_pnb).id;
			if (id == 999)
				continue;
			for (num_cargo = 1; num_cargo <= 4; num_cargo++){
				
				PNBRequiredCargo = requered_cargo[num_pnb][num_cargo];
				if (PNBRequiredCargo == 0){
					continue;	
				}

				if(CFleetTechEconomicEng::Instance()->VehicleRouterPLineValidation(pBasePort,num_pnb,num_cargo,pVehicle)==0){
					continue;
				}
		
				CRouterP *pDynRouter = new CRouterP(pBasePort, num_pnb, num_cargo);
				pDynRouter->SetVehicle(pVehicle);
				RouterTime = pDynRouter->GetRouterTime();
				if (RouterTime > vehicleOperationDaysInYearAll){
					delete pDynRouter;
					continue;	
				}
				
				daysRemain = vehicleOperationDaysInYearAll - RouterTime;
				pDynRouter->SetVehicleOperationsDaysRemain(daysRemain);
				appendDynRList(pDynRouter);

				//Update ShipCargoPlan begin
				if (requered_cargo[num_pnb][num_cargo] < VehicleLoad)
					load = requered_cargo[num_pnb][num_cargo];
				else
					load = VehicleLoad;
				//pDynRouter->m_ShipLoadingPlan[num_pnb][num_cargo] = load; //14.05.05
				pDynRouter->m_ShipLoadingPlan[id][num_cargo] = load;
				//Update ShipCargoPlan end

				pDynRouter->GetPPoitsListHead()->SetCargoLoadInShipPlan(load);

				requered_cargo[num_pnb][num_cargo] -= VehicleLoad;
				if (requered_cargo[num_pnb][num_cargo] < 0)
					requered_cargo[num_pnb][num_cargo] = 0;


				if (CheckPNBsCargoRequirments(pDynRouter) == 0){
					MessageBox(NULL,"Пункту не нужен груз! Ощибка построения маршрута.","3-этап оптимизации: CreateDynamicRoutersFirst()",MB_OK);
					m_pOutView->TextOut("Пункту не нужен груз! Ощибка построения маршрута. 3-этап оптимизации: CreateDynamicRoutersFirst()");
				}
		
				routers_count++;
				
			}
		}
		pBasePortPlan= pBasePortPlan->GetNextPort();

	}	

	if (routers_count == 0)
		return 0; // не построено ни одного маршрута
	
	return 1;
}




int COptimizationSt3Dp::CreateDynamicRoutersNext()
{
	CRouterP *pRouter;
	CPortPoint* pPPoint;
	CPort *pBasePort;
	CVehicle *pVehicle;
	int PNB_num, PNB_numBest, PNB_id_Best, BasePort_id, PNB_id, id, num, fl, 
		cargo_type, cargo_typeBest, cargo_done_dynamic, cargoLoadInShipPlan,
		OperationDaysRemain, DistancePNB, 
		DistanceBest, DistanceInit,
		PNBRequiredCargo,
		VehicleLoad, TripTimeDays,
		load; //, delta_tmp;
    double TripTimeDaysStep;
	double LoadTimeDays = 1; //stab, TBD
	int routers_count = 0;

	int flag1, i1;
	int requiredCargo1, cargo_done_dynamic1;
	int num_tmp, test_flag1;
	int cargoDoneOnRouterForPNB = 0;


	if (m_DynamicRoutersList == 0)
	   return 0;


	pRouter = m_DynamicRoutersList;


	pBasePort = pRouter->GetBasePort();
	BasePort_id = pBasePort->id;
	pVehicle = pRouter->GetVehicle();
	VehicleLoad = pVehicle->GetLoad();
	int VehicleLoadAvailable = VehicleLoad;

	DistanceInit = 1000000;

// "requered cargo" array initialisation:
// a) according to "global" requerements for current optimisation step
	int requered_cargo[MAXPNBNUMBER][5];
	int num_pnb, num_cargo;
	for (num_pnb = 0; num_pnb < MAXPNBNUMBER; num_pnb++){
		id = pBasePort->GetPNB(num_pnb).id;
		if (id == 999)
			continue;
		for (num_cargo = 1; num_cargo <= 4; num_cargo++){
			requered_cargo[num_pnb][num_cargo] = pRouter->GetBasePort()->GetPNBRequiredCargo(num_pnb,num_cargo);
		}
	}
// b)	these values must be corrected: exclude cargoes delivered on current dynamic Router
//		it should be done for each pRouter (see below) in while(pRouter) loop
// "requered cargo" array initialisation end.

	fl = 0;

	while(pRouter){
		
		//Добавление следующего пункта.
		//Критерий: минимальное расстояние перехода между ПНБ: 
		//Проверяются ограничения: 
		//		совместимость линии и заданного судна маршрута;
		//		ограничение эксплуатационного периода;

		OperationDaysRemain = pRouter->GetVehicleOperationsDaysRemain();
		if (OperationDaysRemain <= 0){
			pRouter = pRouter->GetNextRouter();
			continue;
		}
		TripTimeDays = pRouter->GetRouterTime();
		if (TripTimeDays > OperationDaysRemain){ //дополнительная проверка
			pRouter = pRouter->GetNextRouter();
			continue;
		}
		
		pPPoint = pRouter->GetPPointsListEnd();
		cargo_type = pPPoint->GetCargoType();
		PNB_num = pPPoint->GetPNBnum();
		PNB_id = pBasePort->GetPNB(PNB_num).id;

		DistanceBest = DistanceInit;

		// Проверка загрузки судна: begin {
		// Если судно загружено - следующий пункт - базовый

		//Проверка количества груза на судне на маршруте:
		int PNB_ID, cargo_num; //cargo_num 1...4; 0 -не используется
		int cargoInPlan = 0;
		for (PNB_ID = 0; PNB_ID < MAXPNBNUMBER; PNB_ID++)
			for (cargo_num = 0; cargo_num <= 4; cargo_num++)
				cargoInPlan += pRouter->m_ShipLoadingPlan[PNB_ID][cargo_num];

		if(cargoInPlan >= VehicleLoad) { // седующий пункт - базовый
			//судно загружено для выполнения маршрута
			// следовательно следующий пункт д.б. базовый
			// В базовый пункт судно приходит пустое

			//Может ли судно вернуться на базу? 
			//(проверка допустимости маршрута по эксплуатационному периоду)
			DistancePNB = pBasePort->GetDistancePortPNB(PNB_id);
			TripTimeDaysStep = (((double)DistancePNB/pVehicle->GetSpeed())/24);
			if (OperationDaysRemain < (TripTimeDays + TripTimeDaysStep)){
				pRouter = pRouter->GetNextRouter();
				continue;
			}

			if(DistancePNB < DistanceBest){
				DistanceBest = 	DistancePNB;
				PNB_numBest = -1; // используем -1 для базового пункта маршрута (временно ...)
				cargo_typeBest = cargo_type;  // судно возвращается пустое
			}
			
			fl = 1;
		}
		//Проверка загрузки судна: end }

		else {// следующий пункт - м.б. НЕбазовый
			//судно еще не загружено полностью, 
			//и добавление пунктов маршрута может продолжаться

			fl = 0;
			for (num = 0; num < MAXPNBNUMBER; num++){
			
				id = pBasePort->GetPNB(num).id;
				if (id == 999)
					continue;
				
				//if (num == PNB_num)  //04.01.03
				//	continue;

				//requered_cargo[num][cargo_type] -= VehicleLoad; //Груз перевезенный на первой линии
				//if (requered_cargo[num][cargo_type] < 0)
				//	requered_cargo[num][cargo_type] = 0;
				//PNBRequiredCargo = pBasePort->GetPNBRequiredCargo(num,cargo_type);
				cargo_done_dynamic = pRouter->GetCargoDoneOnRouterForPNB(num, cargo_type);
				PNBRequiredCargo = requered_cargo[num][cargo_type] - cargo_done_dynamic;
				//if (PNBRequiredCargo <= 0)
				//	continue;	
				test_flag1 = 0;
				if (PNBRequiredCargo <= 0){
					flag1 = 0;
					num_tmp = 99;
					for (i1 = 1; i1 <= 4; i1++){
						if (i1 == cargo_type)
							continue;	
						else{
							if(CFleetTechEconomicEng::Instance()->VehicleRouterPLineValidation(pBasePort,num,i1,pVehicle)==0)
								continue;
							cargo_done_dynamic1 = pRouter->GetCargoDoneOnRouterForPNB(num, i1);
							//if(cargo_done_dynamic1 == 0)	//07.05.05
							//	continue;
							requiredCargo1 = requered_cargo[num][i1];
							requiredCargo1 -= cargo_done_dynamic1;
							if (requiredCargo1 <= 0)
								continue;
							else {
								cargo_type = i1;
								flag1 = 1;
								PNBRequiredCargo = requiredCargo1;
								num_tmp = num;
								test_flag1 = 1;
								break;
							}
						}
					}
					if (flag1 == 0)
						continue;
				}

	//			if(CFleetTechEconomicEng::Instance()->VehicleRouterPLineValidation(pBasePort,num,cargo_type,pVehicle)==0)
	//				continue;

				if (num != PNB_num) { //04.01.03
					DistancePNB = pBasePort->GetDistancePNBPNB(PNB_id,id);
				}else{ //Тот же пункт. Судно возвращается за грузом в базовый порт
					DistancePNB = 2*pBasePort->GetDistancePortPNB(PNB_id);
				}
				
				TripTimeDaysStep = (((double)DistancePNB/pVehicle->GetSpeed())/24)+ LoadTimeDays;
				if (OperationDaysRemain < (TripTimeDays + TripTimeDaysStep)) //09.01.03
					continue;
				
				if(DistancePNB < DistanceBest){
					DistanceBest = 	DistancePNB;
					PNB_numBest = num;
					cargo_typeBest = cargo_type;
				}
				
				fl = 1;
			}
		}
		
		if(fl == 1){
			PNB_id_Best = pBasePort->GetPNB(PNB_numBest).id;
			//Корректировка грузового плана судна:
			if (PNB_numBest == -1) { //Добавлен базовый порт
				//судно возвращается, т.к. было загружено полностью (полный маршрут)
				//Грузовой план судна обнуляется, и будет формироваться новый план 
				//для продолжения эксплуатации в течении навигационного периода
				
				for (PNB_ID = 0; PNB_ID < MAXPNBNUMBER; PNB_ID++)
					for (cargo_num = 0; cargo_num <= 4; cargo_num++)
						pRouter->m_ShipLoadingPlan[PNB_ID][cargo_num] = 0;
			}
			else{
				//Update ShipCargoPlan begin   
				VehicleLoadAvailable = VehicleLoad - cargoInPlan;
				cargoDoneOnRouterForPNB = pRouter->GetCargoDoneOnRouterForPNB(PNB_numBest, cargo_typeBest);
				PNBRequiredCargo = requered_cargo[PNB_numBest][cargo_typeBest] - cargoDoneOnRouterForPNB;
				if (PNBRequiredCargo < VehicleLoadAvailable)
					load = PNBRequiredCargo;
				else
					load = VehicleLoadAvailable;

				//pRouter->m_ShipLoadingPlan[PNB_numBest][cargo_typeBest] += load; //07.05.05	
				pRouter->m_ShipLoadingPlan[PNB_id_Best][cargo_typeBest] += load; //14.05.05

				/* if (pRouter->m_ShipLoadingPlan[PNB_numBest][cargo_typeBest] == 0)
					pRouter->m_ShipLoadingPlan[PNB_numBest][cargo_typeBest] = load;
				else {
					delta_tmp = load - pRouter->m_ShipLoadingPlan[PNB_numBest][cargo_typeBest];
					if ( delta_tmp < 0){
						pRouter->m_ShipLoadingPlan[PNB_numBest][cargo_typeBest] += delta_tmp;
					}
				} */ //07.05.05

				
				//Update ShipCargoPlan end   
			}

			CPortPoint *pPPointNew = new CPortPoint(PNB_numBest, cargo_typeBest);
			//cargoLoadInShipPlan = pRouter->m_ShipLoadingPlan[PNB_numBest][cargo_typeBest];
			cargoLoadInShipPlan = pRouter->m_ShipLoadingPlan[PNB_id_Best][cargo_typeBest];
			pPPointNew->SetCargoLoadInShipPlan(cargoLoadInShipPlan);
			pRouter->appendPPoint(pPPointNew);
			routers_count++;


			//requered_cargo[PNB_numBest][cargo_type] -= VehicleLoad;
			//if (requered_cargo[PNB_numBest][cargo_type] < 0)
			//	requered_cargo[PNB_numBest][cargo_type] = 0;

		}
		if (CheckPNBsCargoRequirments(pRouter) == 0){
			MessageBox(NULL,"Пункту не нужен груз! Ощибка построения маршрута.","3-этап оптимизации. CreateDynamicRoutersNext()",MB_OK);
			m_pOutView->TextOut("Пункту не нужен груз! Ощибка построения маршрута. 3-этап оптимизации: CreateDynamicRoutersNext()");
		}

		pRouter = pRouter->GetNextRouter();
		fl = 0;
	}

	if (routers_count == 0)
		return 10;  // не построено ни одного маршрута данного шага (звенности), 
					//т.е. лучший маршрут должен быть выбран из маршрутов 
					// предыдущего шага

	return 1;
}

int COptimizationSt3Dp::FindDynamicRouterBest()
{
	int mark = 0;
	int vehicleIncomeBest = 0;
	int vehicleIncomeBestOnStepPrev = 0;

	CRouterP *pt = m_DynamicRoutersList;
	if (pt == 0)
		return 0;

	CRouterP *pt_best = 0;
	int mark_best = -1000000;
	
	while (pt){
		mark = GetRouterPMarkStub(pt);
		//mark = GetRouterPMark(pt);
		if (mark > mark_best){
			pt_best = pt;
			mark_best = mark;
		}
		pt = pt->next;
	}

	pt_best->vehicleIncome = CFleetTechEconomicEng::Instance()->VehicleIncomeOnRouterPForYear(pt_best);
	vehicleIncomeBest = pt_best->vehicleIncome;


	if (m_RouterBestOnStepPrev == 0){
		m_RouterBestOnStep = m_RouterBestOnStepPrev = pt_best;
		vehicleIncomeBestOnStepPrev = m_RouterBestOnStepPrev->vehicleIncome;
	}
	else{
	//	if (pt_best->vehicleIncome > m_RouterBestOnStepPrev->vehicleIncome){
		vehicleIncomeBestOnStepPrev = m_RouterBestOnStepPrev->vehicleIncome;
		if (vehicleIncomeBest > vehicleIncomeBestOnStepPrev){
			m_RouterBestOnStepPrev = m_RouterBestOnStep;
			m_RouterBestOnStep = pt_best;
		}
	}
	if (CheckPNBsCargoRequirments(m_RouterBestOnStep) == 0){
		MessageBox(NULL,"Пункту не нужен груз! Ощибка построения маршрута.","3-этап оптимизации: FindDynamicRouterBest()",MB_OK);
		m_pOutView->TextOut("Пункту не нужен груз! Ощибка построения маршрута. 3-этап оптимизации:FindDynamicRouterBest()");
	}

	
	return 1;
}

int COptimizationSt3Dp::FindDynamicRouterBest1()
{
// Критерий: Доход на маршруте
	
	int mark = 0;
	int vehicleIncomeBest = 0;
	int vehicleIncomeBestOnStepPrev = 0;

	CRouterP *pt = m_DynamicRoutersList;
	if (pt == 0)
		return 0;

	CRouterP *pt_best = 0;
	int mark_best = -1000000;
	
	while (pt){
		mark = CFleetTechEconomicEng::Instance()->VehicleIncomeOnRouterPForYear(pt);
		pt->vehicleIncome = mark;

		if (mark > mark_best){
			pt_best = pt;
			vehicleIncomeBest = mark_best = mark;
		}
		else {
			pt = pt->next;
			continue;
		}


		if (m_RouterBestOnStepPrev == 0){
			m_RouterBestOnStep = m_RouterBestOnStepPrev = pt_best;
			vehicleIncomeBestOnStepPrev = m_RouterBestOnStepPrev->vehicleIncome;
		}
		else{
			vehicleIncomeBestOnStepPrev = m_RouterBestOnStepPrev->vehicleIncome;
			if (vehicleIncomeBest > vehicleIncomeBestOnStepPrev){
				m_RouterBestOnStepPrev = m_RouterBestOnStep;
				m_RouterBestOnStep = pt_best;
			}
		}
		if (CheckPNBsCargoRequirments(m_RouterBestOnStep) == 0){
			MessageBox(NULL,"Пункту не нужен груз! Ощибка построения маршрута.","3-этап оптимизации: FindDynamicRouterBest()",MB_OK);
			m_pOutView->TextOut("Пункту не нужен груз! Ощибка построения маршрута. 3-этап оптимизации:FindDynamicRouterBest()");
		}
	
		pt = pt->next;

	}

	
	return 1;
}



void COptimizationSt3Dp::RemoveDynamicRoutersListPrev()
{
	CRouterP *pt = m_DynamicRoutersListPrev;
	
	while (pt){
		CRouterP *tmp = pt;
		pt = pt->next;
		delete tmp;
	}
	m_DynamicRoutersListPrev = m_at_endDynRListPrev = 0;
}


void COptimizationSt3Dp::RemoveDynamicRoutersList()
{
	CRouterP *pt = m_DynamicRoutersList;
	
	while (pt){
		CRouterP *tmp = pt;
		pt = pt->next;
		delete tmp;
	}
	m_DynamicRoutersList = m_at_endDynRList = 0;
}


void COptimizationSt3Dp::appendDynRList(CRouterP *pRouter)
{
    if(COptimizationSt3Dp::m_DynamicRoutersList == 0)
        m_DynamicRoutersList = pRouter;
    else
        m_at_endDynRList->next = pRouter;
            
    m_at_endDynRList = pRouter;
}

void COptimizationSt3Dp::appendDynRListPrev(CRouterP *pRouter)
{
    if(m_DynamicRoutersListPrev == 0)
        m_DynamicRoutersListPrev = pRouter;
    else
        m_at_endDynRListPrev->next = pRouter;
            
    m_at_endDynRListPrev = pRouter;
}

int COptimizationSt3Dp::UpdateDynamicRoutersListPrev()
{
	int PNB_num, PNB_id, cargo_type, cargoLoad;
	CPortPoint* pPPoint;
	CVehicle* pVehicle;
	int cargo_num; //cargo_num 1...4; 0 -не используется
	int testCargoInPPoint;


	//RemoveDynamicRoutersListPrev(); //15.10.04

	CRouterP *pt = m_DynamicRoutersList;
	CPort *pBasePort = pt->GetBasePort();


	while (pt){
		pVehicle = pt->GetVehicle();
		pPPoint = pt->GetPPoitsListHead();
		PNB_num = pPPoint->GetPNBnum();
		cargo_type = pPPoint->GetCargoType();
		cargoLoad = pPPoint->GetCargoLoadInShipPlan();
		CRouterP *pRouterNew = new CRouterP(pBasePort, PNB_num, cargo_type);
		pRouterNew->GetPPoitsListHead()->SetCargoLoadInShipPlan(cargoLoad);
		
		// План загрузки судна на маршруте - инициализация	14.05.05
		for (PNB_id = 0; PNB_id < MAXPNBNUMBER; PNB_id++)	
			for (cargo_num = 0; cargo_num <= 4; cargo_num++)
				pRouterNew->m_ShipLoadingPlan[PNB_id][cargo_num] = 0;

		pPPoint = pPPoint->GetNextPPoint();
		while(pPPoint){
			PNB_num = pPPoint->GetPNBnum();
			PNB_id = pBasePort->GetPNB(PNB_num).id;
			cargo_type = pPPoint->GetCargoType();
			cargoLoad = pPPoint->GetCargoLoadInShipPlan();
			CPortPoint *pPPointNew = new CPortPoint(PNB_num, cargo_type);
			pPPointNew->SetCargoLoadInShipPlan(cargoLoad);
			pRouterNew->m_ShipLoadingPlan[PNB_id][cargo_type] = cargoLoad;
			testCargoInPPoint = pt->m_ShipLoadingPlan[PNB_id][cargo_type];
			pRouterNew->appendPPoint(pPPointNew);
			pPPoint = pPPoint->GetNextPPoint();
		}
		pRouterNew->SetVehicle(pVehicle);

		if (CheckPNBsCargoRequirments(pRouterNew) == 0){
				MessageBox(NULL,"Пункту не нужен груз! Ощибка построения маршрута","3-этап оптимизации: UpdateDynamicRoutersListPrev()",MB_OK);
				m_pOutView->TextOut("Пункту не нужен груз! Ощибка построения маршрута. 3-этап оптимизации: UpdateDynamicRoutersListPrev(), CheckPNBsCargoRequirments");
		}

		appendDynRListPrev(pRouterNew);

		// План загрузки судна на маршруте: begin
		//for (PNB_ID = 0; PNB_ID < MAXPNBNUMBER; PNB_ID++)
		//	for (cargo_num = 0; cargo_num <= 4; cargo_num++)
		//		pRouterNew->m_ShipLoadingPlan[PNB_ID][cargo_num] = pt->m_ShipLoadingPlan[PNB_ID][cargo_num];
		// План загрузки судна на маршруте: end

		pt = pt->next;	


	}

	
	return 1;
}

int COptimizationSt3Dp::CheckUpdateDynamicRoutersListOnStep(int step)
{
	CRouterP *pt = m_DynamicRoutersList;
	int count;
	CRouterP *ptmp;

	while (pt){
		count = pt->GetPPointsCount();
		if (count != step){
			ptmp = pt;
			pt = pt->next;
			RemoveDynamicRoutersListNode(ptmp);
		}else
			pt = pt->next;
	}

	return 1;
}


int COptimizationSt3Dp::GetRouterPMarkStub(CRouterP * pRouter)
{
	return 100;
}

int COptimizationSt3Dp::GetRouterPMark(CRouterP * pRouter)
{
	return 100;
}


int COptimizationSt3Dp::normRouter(CRouterP *&pt)
{
	if (pt == 0)
		return 0;

	CPort *pBasePort = pt->GetBasePort();
	CVehicle* pVehicle = pt->GetVehicle();
	CPortPoint* pPPoint = pt->GetPPoitsListHead();
	int PNB_num = pPPoint->GetPNBnum();
	int cargo_type = pPPoint->GetCargoType();
	int	cargoLoad = pPPoint->GetCargoLoadInShipPlan();
	CRouterP *pRouter = new CRouterP(pBasePort, PNB_num, cargo_type);
	pRouter->SetVehicle(pVehicle);
	pRouter->GetPPoitsListHead()->SetCargoLoadInShipPlan(cargoLoad);
	pPPoint = pPPoint->GetNextPPoint();
	while(pPPoint){
		PNB_num = pPPoint->GetPNBnum();
		cargo_type = pPPoint->GetCargoType();
		cargoLoad = pPPoint->GetCargoLoadInShipPlan();
		CPortPoint *pPPointNew = new CPortPoint(PNB_num, cargo_type);
		pPPointNew->SetCargoLoadInShipPlan(cargoLoad);
		pRouter->appendPPoint(pPPointNew);
		pPPoint = pPPoint->GetNextPPoint();
	}

	// План загрузки судна на маршруте: begin
	int PNB_ID, cargo_num; //cargo_num 1...4; 0 -не используется
	for (PNB_ID = 0; PNB_ID < MAXPNBNUMBER; PNB_ID++)
		for (cargo_num = 0; cargo_num <= 4; cargo_num++)
			pRouter->m_ShipLoadingPlan[PNB_ID][cargo_num] = pt->m_ShipLoadingPlan[PNB_ID][cargo_num];
	// План загрузки судна на маршруте: end


	CRouterP *tmp = pt;
	pt = pRouter;
	delete tmp;
	
	//pt->normShipLoadingPlan();

	if (CheckPNBsCargoRequirments(pt) == 0){
		MessageBox(NULL,"Пункту не нужен груз! Ощибка построения маршрута","3-этап оптимизации: normRouter()",MB_OK);
		m_pOutView->TextOut("Пункту не нужен груз! Ощибка построения маршрута. 3-этап оптимизации: normRouter()");
	}
	

	return 1;
}

int COptimizationSt3Dp::UpdatePNBsCargoRequirments(CRouterP *pt)
{
	//MessageBox(NULL,"UpdatePNBsCargoRequirments","STEP3. UpdatePNBsCargoRequirments",MB_OK);

	
	int VehicleLoad, cargo_type, PNB_num, BasePort_id, /*PNB_id,*/  
		PNBRequiredCargo, PNBRequiredCargoRemain, cargo_done_dynamic;

	CPort *pBasePort = pt->GetBasePort();
	BasePort_id = pBasePort->id;
	CVehicle* pVehicle = pt->GetVehicle();
	VehicleLoad = pVehicle->GetLoad();
	CPortPoint* pPPoint = pt->GetPPoitsListHead();
	while(pPPoint){
		PNB_num = pPPoint->GetPNBnum();
		//PNB_id = pBasePort->GetPNB(PNB_num).id;
		if (PNB_num == -1){ //базовый порт, не делаем проверку (на данном этапе ...)
			pPPoint = pPPoint->GetNextPPoint();
			continue;
		}
		cargo_type = pPPoint->GetCargoType();
		PNBRequiredCargo = pBasePort->GetPNBRequiredCargo(PNB_num,cargo_type);
		if (PNBRequiredCargo > 0){
		/*	if (PNBRequiredCargo <= 0){
				//Это ошибка, т.к. такой ситуации не должно быть. 
				//Если маршрут построен, то пункты его требуют доставки груза.
				//Необходимо проверить процедуру построения маршрута, 
				//и в ней - проверку на необходимость груза для пункта.
				MessageBox(NULL,"PNB Requered Cargo Problem","STEP3. UpdatePNBsCargoRequirments",MB_OK);
				return 0;
			}		
		*/		
			//PNBRequiredCargoRemain = PNBRequiredCargo - VehicleLoad; //old old

			//PNB_id = pBasePort->GetPNB(PNB_num).id;
			
			//PNBRequiredCargoRemain = PNBRequiredCargo - pt->m_ShipLoadingPlan[PNB_num][cargo_type]; //old

			//cargo_done_dynamic = pt->GetCargoDoneOnRouterForPNB(PNB_num, cargo_type); 06.05.05
			cargo_done_dynamic = pPPoint->GetCargoLoadInShipPlan(); //06.05.05
			PNBRequiredCargoRemain = PNBRequiredCargo - cargo_done_dynamic;
			
			if (PNBRequiredCargoRemain < 0)
				PNBRequiredCargoRemain = 0;
			pBasePort->SetPNBRequiredCargoRemain(PNB_num, cargo_type, PNBRequiredCargoRemain); 
		}
		
		pPPoint = pPPoint->GetNextPPoint();
	}

	return 1;
}



int COptimizationSt3Dp::CheckPNBsCargoRequirments(CRouterP *pt)
{
	int VehicleLoad, cargo_type, PNB_num, BasePort_id, /*PNB_id,*/
		PNBRequiredCargo;

	CPort *pBasePort = pt->GetBasePort();
	BasePort_id = pBasePort->id;
	CVehicle* pVehicle = pt->GetVehicle();
	VehicleLoad = pVehicle->GetLoad();
	CPortPoint* pPPoint = pt->GetPPoitsListHead();
	while(pPPoint){
		PNB_num = pPPoint->GetPNBnum();
		//PNB_id = pBasePort->GetPNB(PNB_num).id;
		if (PNB_num == -1){ //базовый порт, не делаем проверку (на данном этапе ...)
			pPPoint = pPPoint->GetNextPPoint();
			continue;
		}

		cargo_type = pPPoint->GetCargoType();
		PNBRequiredCargo = pBasePort->GetPNBRequiredCargo(PNB_num,cargo_type);
		if (PNBRequiredCargo <= 0){ // <= replace by <
			//Это ошибка, т.к. такой ситуации не должно быть. 
			//Если маршрут построен, то пункты его требуют доставки груза
			//Необходимо проверить процедуру построения маршрута 
			//и в ней - проверку на необходимость груза для пункта.
			MessageBox(NULL,"Пункту не нужен груз! Ощибка построения маршрута","3-этап оптимизации: CheckPNBsCargoRequirments()",MB_OK);
			return 0;
		}		
		
		pPPoint = pPPoint->GetNextPPoint();
	}

	return 1;
}

int COptimizationSt3Dp::print()
{
	int PNB_num, cargo_type, cargoLoad, vehicleOperationDaysInYearAll, vehicleIncome, nRouterTime;
	CPortPoint* pPPoint;
	CVehicle* pVehicle;
	CString sCargoType = "";

    FILE *stream;
    //stream = fopen( "E:\\Users\\mv\\Log\\Step3Plan.txt", "w" );
	CString LogFile = ((CFleetOptimizerApp*)AfxGetApp())->GetProfileString("FleetOptimizer","LogFileOptimizStep3Plan");	
    stream = fopen(LogFile, "w" );

	CRouterP *pt = CBasePlanSt3Dp::Instance()->GetRoutersListHead();
    if (pt == NULL){
		fclose(stream);
        return 0;
    }


	while (pt){
		pVehicle = pt->GetVehicle();
		pPPoint = pt->GetPPoitsListHead();
        fprintf (stream,"Судно: %s\n",pVehicle->GetTypeName());
        fprintf (stream,"Грузоподъемность судна: %d т\n",pVehicle->GetLoad());

		while(pPPoint){
			PNB_num = pPPoint->GetPNBnum();
			cargo_type = pPPoint->GetCargoType();
			cargoLoad = pPPoint->GetCargoLoadInShipPlan();
			if (PNB_num == -1) //base port
				fprintf (stream,"Базовый пункт: %s\n",pt->GetBasePort()->GetPortName());
			else{
				fprintf (stream,"ПНБ: %s ,   ",pt->GetBasePort()->GetPNB(PNB_num).name);
				switch(cargo_type){
				 case 1:
					 sCargoType = "уголь";
					 break;
				 case 2:
					 sCargoType = "генеральный";
					 break;
				 case 3:
					 sCargoType = "ГСМ";
					 break;
				 case 4:
					 sCargoType = "овоши-фрукты";
					 break;
				}
				//fprintf (stream,"Груз: %d", cargo_type);
				fprintf (stream,"Груз: %s", sCargoType.GetString());
				fprintf (stream," - количество: %d т\n", cargoLoad);
			}

			pPPoint = pPPoint->GetNextPPoint();
		}

		vehicleOperationDaysInYearAll = CFleetTechEconomicEng::Instance()->GetVehicleOperationsDaysInYear(pVehicle->GetRegisterClass(),pt->GetBasePort()->GetPortRegion());
		fprintf (stream,"Эксплуатационный период (дней): %d\n",vehicleOperationDaysInYearAll);
		//fprintf (stream,"Время выполнения маршрута (дней): %d\n",pt->GetRouterTime());
		nRouterTime = 33;
		nRouterTime = pt->GetRouterTimeFinal();
		//fprintf (stream,"Время выполнения маршрута (дней): %d\n",pt->GetRouterTimeFinal());
		fprintf (stream,"Время выполнения маршрута (дней): %d\n",nRouterTime);

		//old: fprintf (stream,"Время выполнения маршрута (дней): %d\n",pt->GetRouterTime());
		vehicleIncome = CFleetTechEconomicEng::Instance()->VehicleIncomeOnRouterPForYear(pt);
        fprintf (stream,"Доход судна на маршруте: %d\n", vehicleIncome);

		fprintf (stream,"****************************\n\n");
		pt = pt->next;	
	}
	
	fclose(stream);
	return 1;

}

int COptimizationSt3Dp::RemoveDynamicRoutersListNode(CRouterP *pNodeRm)
{
    if(COptimizationSt3Dp::m_DynamicRoutersList == 0)
		return 0;

	CRouterP *pt;
	CRouterP *pPrev;
	
	if(pNodeRm == m_DynamicRoutersList){
		pt = m_DynamicRoutersList;
		m_DynamicRoutersList = m_DynamicRoutersList->next;
		delete pt;
		return 1;
	}

	pt = m_DynamicRoutersList;
	pPrev = pt;
	while(pt){
		if (pt == pNodeRm){
			pPrev->next = pt->next;
			pt->next = NULL;
			delete pt;
			break;
		}
		else{
			pPrev = pt;
			pt = pt->next;
		}
	}
	
	return 1;
}

int COptimizationSt3Dp::ShipLoadingPlanCreate(CRouterP *pt)
{
/*	if (pt == 0)
		return 0;

	int PNB_num, PNB_id, cargo_num;
	CPortPoint* pPPoint = pt->GetPPoitsListHead();

	while(pPPoint){
		PNB_num = pPPoint->GetPNBnum();
		PNB_id = pBasePort->GetPNB(PNB_num).id;
		for (cargo_num = 1; cargo_num <= 4; cargo_num++){
			m_ShipLoadingPlan[PNB_id][cargo_num] = pRouter->GetCargoDoneOnRouterForPNB(num, cargo_num);
		}
		pPPoint = pPPoint->GetNextPPoint();
	}
*/
	return 1;
}



void COptimizationSt3Dp::ResetExistVehicleNum()
{
	CVehicle* pVehicle;

    pVehicle = CVehiclesList::Instance()->GetVehicleListHead();
    while(pVehicle){   		
		if (pVehicle->IsExistForce()){
			pVehicle->UpdateExistNumRemainReset();
		}
		pVehicle = pVehicle->GetNextVehicle();
	}
}
