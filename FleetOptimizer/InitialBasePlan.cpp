// InitialBasePlan.cpp: implementation of the CInitialBasePlan class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "FleetOptimizer.h"
#include "InitialBasePlan.h"
#include "Port.h"
#include "FleetTechEconomicEng.h"
#include "MarineTechEconomicEng.h"
#include "RouterS.h"
#include "OutputView.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


CInitialBasePlan* CInitialBasePlan::_instance = 0;

CInitialBasePlan* CInitialBasePlan::Instance(){
    if (_instance == 0){
        _instance = new CInitialBasePlan;
    }
    return _instance;
}
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CInitialBasePlan::CInitialBasePlan()
{
    
    RoutersList = 0;
    InitialBasePlan = 0;

    //MessageBox(NULL,"CInitialBasePlan()","CInitialBasePlan",MB_OK);

	POSITION pos = ((CFleetOptimizerApp*)AfxGetApp())->GetCurrFleetOptimizerDoc()->GetFirstViewPosition();
	m_pOutView = (COutputView*) ((CFleetOptimizerApp*)AfxGetApp())->GetCurrFleetOptimizerDoc()->GetNextView(pos);
	m_pOutView = (COutputView*) ((CFleetOptimizerApp*)AfxGetApp())->GetCurrFleetOptimizerDoc()->GetNextView(pos);
	m_pOutView->TextOut("CInitialBasePlan()");
}

/*CInitialBasePlan::~CInitialBasePlan()
{

}*/


int CInitialBasePlan::CreateInitialBasePlan()
{ 
    CVehicle *pVehicleBest;
    int VehicleBestIncome,VehicleBestIncomeAll, VehicleBestNum;
    int VehicleNum = 0; //количество судов на маршруте (целое)
    double dVehicleNum, dVehicleNumBest; //нецелое количество судов на маршруте
    int flagVehicleFound;
    int vehicleIncome, vehicleIncomeAll;
    int BuildCoastYearPay,BuildCoastYearPayBest, OperationalExpensesYear;
	int cargo_for_navigation, VehicleCargoForNavigationBest,
		nVechicleExist, nVechicleExistForce, flagGetVehicle, AdvantPrice, nVehicleBestExist,
		VehicleBestAdvantPrice, nVehicleExistNum, flagVehicleBestForce;
	int fl_noShipForLine = 0;
	int id;
	CString sCargoType = "";

    //FILE *stream;   //for debug
    //stream = fopen( "E:\\Users\\mv\\Log\\InitDBM.txt", "w" ); // for debug
    
    FILE *stream1;   //for debug
    //stream1 = fopen( "E:\\Users\\mv\\Log\\InitDBMVehiclesAll.txt", "w" ); // for debug
	CString LogFile = ((CFleetOptimizerApp*)AfxGetApp())->GetProfileString("FleetOptimizer","LogFileInitDBMVehiclesAll");	
    stream1 = fopen(LogFile, "w" );

	CString ErrorsLogFile = ((CFleetOptimizerApp*)AfxGetApp())->GetProfileString("FleetOptimizer","LogFileErrors");	
    FILE *errstream;
	errstream = fopen(ErrorsLogFile, "a" );


    CCargo *availableCargoes = new CCargo;

	POSITION pos = ((CFleetOptimizerApp*)AfxGetApp())->GetCurrFleetOptimizerDoc()->GetFirstViewPosition();
	COutputView * pOutView = (COutputView*) ((CFleetOptimizerApp*)AfxGetApp())->GetCurrFleetOptimizerDoc()->GetNextView(pos);
	pOutView = (COutputView*) ((CFleetOptimizerApp*)AfxGetApp())->GetCurrFleetOptimizerDoc()->GetNextView(pos);

    if ((CPortList::Instance()->is_empty())== 1) {
        MessageBox(NULL,"Error: Ports list is empty","Initial Base Plan",MB_OK);
		pOutView->TextOut(" ");
		pOutView->TextOut("Ports list is empty. Error in Initial Base Plan Creation.");
        return 0;
    }else{
        //MessageBox(NULL,"Initial Base Plan Creation","CInitialBasePlan::CreateInitialBasePlan()",MB_OK);
		pOutView->TextOut(" ");
		pOutView->TextOut("Initial Base Plan Creation...");
	}
    
    //Выбор первого Базового пункта из списка.
    CPort *pBasePort = CPortList::Instance()->GetPortListHead();

	//Ограничение капиталовложений в создание комплекса, за год.
	long lInvestmentYearLimit = CFleetTechEconomicEng::Instance()->GetInvestmentYearLimit();
	long lInvestmentYear = 0;
//	long lInvestmentYearTmp;
	//Капиталовложения в береговые сооружения
	BOOL bIncludeLandStructCalc = FALSE;
	int LandStructYearPay = 0;
	int LandStructYearPayBest = 0;
	bIncludeLandStructCalc = ((CFleetOptimizerApp*)AfxGetApp())->GetProfileInt("FleetOptimizer","IncludeLandStructCalc",0);	

    
	//Назначение Базового пункта.   
    while(pBasePort){
        
        //debug:
        pBasePort->GetAvailableCargoesAll(availableCargoes);
        //fprintf( stream, "Base Port: %s\n", pBasePort->GetPortName());
        int general = pBasePort->GetAvailableCargo(CCargo::General);
        //fprintf (stream,"general: %d\n",general); //for debug
        //fprintf (stream,"coal: %d\n",availableCargoes->coal); //for debug
        //debug end
        

        //Назначение груза. 
        for (int cargo_type = 1; cargo_type <= 4; cargo_type ++){ // cargo loop
            // 1 - coal; 2 - general; 3 - oil; 4 - vegetables_fruits

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
            
            //Назначение НБ-пункта. 
            //for (int PNB_num = 0;pBasePort->GetPNB(PNB_num).id != 999;PNB_num++){
            for (int PNB_num = 0;PNB_num < MAXPNBNUMBER;PNB_num++){
                //fprintf( stream, "\n\nPNB: %s\n", pBasePort->GetPNB(PNB_num).name);
                //fprintf( stream, "Required cargo: %d\n", pBasePort->GetPNBRequiredCargo(PNB_num, cargo_type));
				id = pBasePort->GetPNB(PNB_num).id;
				if (id == 999)
					continue;
				if (pBasePort->GetPNBRequiredCargo(PNB_num, cargo_type) == 0){
					continue;
				}

                CRouterS *pRouter = new CRouterS(pBasePort,PNB_num,cargo_type);
                //pRouter->SetBasePort(pBasePort);
                //pRouter->SetCargoType(cargo_type);
                //fprintf (stream,"RouterCargo Type: %d\n",pRouter->GetCargoType()); //for debug
                //fprintf (stream,"Line Cargo Type: %d\n",pRouter->GetLine()->GetCargoType()); //for debug

                pVehicleBest = 0;
                VehicleBestIncomeAll = -10000000;
				nVehicleBestExist = 0;
                flagVehicleFound = 0;
                
                //Выбор первого транспортного средства из списка.
                CVehicle *pVehicle = CVehiclesList::Instance()->GetVehicleListHead();
				int itmp = 1;
				flagVehicleBestForce = 0;
                while(pVehicle){
					AdvantPrice = 0;
					//if (itmp == 3){
					if ((itmp == 3)||(pVehicle->IsExist())){
						//Проверка совместимости судна и линии:
						if(CFleetTechEconomicEng::Instance()->VehicleLineValidation(pBasePort,PNB_num,cargo_type,pVehicle)==0){
							pVehicle = pVehicle->GetNextVehicle();               
							itmp++;
							if (itmp == 4)
								itmp = 1;
							continue;
						}
                    
						//flagVehicleFound = 1;
						//Определение Чистого Дохода судна на линии за навигацию.
						vehicleIncome = CFleetTechEconomicEng::Instance()->VehicleIncomeOnLineForYear(pBasePort,PNB_num,cargo_type,pVehicle,VehicleNum,dVehicleNum,BuildCoastYearPay, OperationalExpensesYear, cargo_for_navigation);


						//Проверка возиожности добавления судна 
						//с точки зрения капиталовложений:
						if(lInvestmentYearLimit > 0){	
							// lInvestmentYearLimit если == 0, то нет огрвничений
							//Land:
							if (bIncludeLandStructCalc){
								LandStructYearPay = CMarineTechEconomicEng::Instance()->GetLandStructExpenciesYearDeltaSt0(pBasePort,PNB_num,pVehicle, VehicleNum, 0);
							}
							//end of Land

							if((lInvestmentYear+LandStructYearPay+BuildCoastYearPay*VehicleNum) > lInvestmentYearLimit){
								pVehicle = pVehicle->GetNextVehicle();               
								itmp++;
								if (itmp == 4)
									itmp = 1;
								continue;
							}
						}
						//

						//09.08.03
						//vehicleIncomeAll = vehicleIncome * VehicleNum;
						vehicleIncomeAll = vehicleIncome; //функция VehicleIncomeOnLineForYear возвращает общий доход всех судов на маршруте

						//fprintf (stream1,"ПНБ: %s,\tГруз: %d,\tСудно: %s,\tГрузоподъемность: %d,\tКоличество: %d,\tСтроительная стоимость: %d,\tГодовой платеж строит.: %d,\tЭксплуатационные расходы за год: %d,\tДоход на маршруте: %d\n",pBasePort->GetPNB(PNB_num).name, cargo_type, pVehicle->GetTypeName(),pVehicle->GetLoad(),VehicleNum, pVehicle->GetCoast(),BuildCoastYearPay,OperationalExpensesYear,vehicleIncome);
						fprintf (stream1,"ПНБ: %s,\tГруз: %s,\tСудно: %s,\tГрузоподъемность: %d т,\tКоличество: %d,\tСтроительная стоимость: %d,\tГодовой платеж строит.: %d,\tЭксплуатационные расходы за год: %d,\tДоход на маршруте: %d\n",pBasePort->GetPNB(PNB_num).name, sCargoType.GetString(), pVehicle->GetTypeName(),pVehicle->GetLoad(),VehicleNum, pVehicle->GetCoast(),BuildCoastYearPay,OperationalExpensesYear,vehicleIncome);
						
						flagGetVehicle = 0;
						nVechicleExist = pVehicle->IsExist();
						nVechicleExistForce = pVehicle->IsExistForce();
						if (nVechicleExist)
							AdvantPrice = pVehicle->GetExistGenAdvantPrice();
						
						if (vehicleIncomeAll > VehicleBestIncomeAll){
							if (nVechicleExist) {//Ship Exist
								// Если судно существует, и это судно не в стадии проектирования,
								// то нужно проверить, достаточно ли количество судов и, если нет -
								// судно не принимается.
								// Если судно в стадии проектирования, то оно принимается.
								if (pVehicle->IsExistDesign())
									flagGetVehicle = 1;  
								else {
									nVehicleExistNum = pVehicle->GetExistNumRemain();
									if (VehicleNum <= nVehicleExistNum)
										flagGetVehicle = 1;
								}
							}
							else //Ship doesn't exist
							{ 
								if (nVehicleBestExist == 0){ //previouse(best) vehicle - doesn't exist
									flagGetVehicle = 1;					
								} 
								else { //previouse(best) vehicle - exists
									if (flagVehicleBestForce == 0) {	// если нет неоспоримого преимущества
																		// предыдущего существующего судна
										//If Current Vehicle MuchBetter ?..:
										if (vehicleIncomeAll > (VehicleBestAdvantPrice + VehicleBestIncomeAll))
											flagGetVehicle = 1;
									}
								}
							}
						} // if (vehicleIncomeAll > VehicleBestIncomeAll)
						else { // vehicleIncomeAll < VehicleBestIncomeAll
							if (nVechicleExist) { //Ship Exist; у него есть преимущество ...
								if ((flagVehicleBestForce == 1) || (AdvantPrice == -1) // существующее судно имеет неоспоримое преимущество
									|| ((vehicleIncomeAll + VehicleBestAdvantPrice) > VehicleBestIncomeAll)){
									if (pVehicle->IsExistDesign())
										flagGetVehicle = 1;  
									else {
										nVehicleExistNum = pVehicle->GetExistNumRemain();
										if (VehicleNum <= nVehicleExistNum)
											flagGetVehicle = 1;	
									}
								}
							}
						}
						
						if (flagGetVehicle == 1){
							flagVehicleFound = 1;
							if (nVechicleExist && (pVehicle->IsExistDesign() == 0)){
								pVehicle->UpdateExistNumRemainDecrem(VehicleNum);
							}
							if (nVehicleBestExist && (pVehicleBest->IsExistDesign() == 0)) { //выходящее судно - существующее
								pVehicleBest->UpdateExistNumRemainIncrem(VehicleNum);
							}
 							VehicleBestIncomeAll = vehicleIncomeAll;                       
							VehicleBestIncome = vehicleIncome;
							pVehicleBest = pVehicle;
							VehicleBestNum = VehicleNum;
							dVehicleNumBest = dVehicleNum;
							VehicleCargoForNavigationBest = cargo_for_navigation;
							nVehicleBestExist = nVechicleExist;
							VehicleBestAdvantPrice = AdvantPrice;
							BuildCoastYearPayBest = BuildCoastYearPay;
							LandStructYearPayBest = LandStructYearPay;
							if (pVehicleBest->IsExistForce() == 1)
								flagVehicleBestForce = 1;
							else
								flagVehicleBestForce = 0;
						}

					}
					itmp++;
					if (itmp == 4)
						itmp = 1;
                    pVehicle = pVehicle->GetNextVehicle();               

                }//Vehicle loop
                if (flagVehicleFound){
                    pRouter->SetVehicle(pVehicleBest);
                    pRouter->vehicleNumber = VehicleBestNum;
                    //pRouter->dVehicleNumber = VehicleBestNum;
                    pRouter->dVehicleNumber = dVehicleNumBest;
                    pRouter->vehicleIncome = VehicleBestIncome;
					//pRouter->vehicleCargoForNavigation = VehicleCargoForNavigationBest;
					lInvestmentYear += BuildCoastYearPayBest*VehicleBestNum;
					if (bIncludeLandStructCalc)
						lInvestmentYear += LandStructYearPayBest;
                    _instance->append(pRouter);

                    //fprintf (stream,"Судно: %s\n",pVehicleBest->GetTypeName());
                    //fprintf (stream,"Грузоподъемность Судна: %d\n",pVehicleBest->GetLoad());
                    //fprintf (stream,"Количество судов: %d\n", VehicleBestNum);
                    //fprintf (stream,"Количество судов нецелое: %f\n", dVehicleNumBest);
                    //fprintf (stream,"Доход одного судна за навигацию: %d \n",VehicleBestIncome);
                    //fprintf (stream,"Доход всех судов за навигацию: %d \n",(VehicleBestIncome * VehicleBestNum));
                }
                else{
                    fprintf (errstream,"Не найдено судно для линии: %s - %s, груз: %d . CInitialBasePlan()\n",pBasePort->GetPortName(), pBasePort->GetPNB(PNB_num).name, cargo_type );
					fl_noShipForLine = 1;
                    //fprintf (stream,"Не найдено судно для линии\n");
					pOutView->TextOut("Не найдено судно для линии. CInitialBasePlan()");
                }

            }//PNB loop

        }//cargo loop
        pBasePort = pBasePort->GetNextPort();
    }// BasePort loop
    _instance->print();

	CFleetTechEconomicEng::Instance()->SetInvestmentYear(lInvestmentYear);

	int ErrorStatus = CFleetTechEconomicEng::Instance()->GetErrorStatus();
	if (ErrorStatus == 0){
		ErrorStatus = CShipFactory::Instance()->GetErrorStatus();
		if (ErrorStatus == 0){
			ErrorStatus = CACVDesign::Instance()->GetErrorStatus();
		}
	}

	fclose(stream1);
	fclose(errstream);

	if (fl_noShipForLine){
		MessageBox(NULL,"Initial Base Plan: errors in plan creation\n[Not found ship for line]\nSee Results: \"Initial Plan\"","Initial Base Plan",MB_ICONWARNING);
		pOutView->TextOut("Initial Base Plan: errors in plan creation [Not found ship for line]; See Results: \"Initial Plan\"");
		return 3;
	}
	else if (ErrorStatus > 0){
		MessageBox(NULL,"Initial Base Plan: Completed\nSee logs: \"Results, Logs, errors\".","Initial Base Plan",MB_OK);
		pOutView->TextOut("Initial Base Plan: Completed. See logs: \"Results, Logs, errors\".");
		return 2;
	}
	//else{
	//	MessageBox(NULL,"Начальный Базисный План: Выполнено","Начальный Базисный План",MB_OK);
	//	pOutView->TextOut("Начальный Базисный План: Выполнено");
	//}
	return 1;

}

void CInitialBasePlan::append(CRouter *pRouter)
{
    if(CInitialBasePlan::RoutersList == 0)
        RoutersList = pRouter;
    else
        at_end->next = pRouter;
            
    at_end = pRouter;
}


CRouter * CInitialBasePlan::GetRoutersListHead()
{
   return RoutersList;
}

void CInitialBasePlan::print()
{   
    CString sCargoType;

	CRouter *pRouter = RoutersList;

    FILE *stream;
    //stream = fopen( "E:\\Users\\mv\\Log\\InitialBasePlan.txt", "w" );
	CString LogFile = ((CFleetOptimizerApp*)AfxGetApp())->GetProfileString("FleetOptimizer","LogFileInitialBasePlan");	
    stream = fopen(LogFile, "w" );

    if (pRouter == NULL){
        fprintf (stream,"Список маршрутов пуст\n");        
    }
    else{
        while(pRouter){

            fprintf (stream,"Базовый порт: %s\n",pRouter->GetBasePort()->GetPortName());
            int PNBnum = pRouter->GetLine()->GetPNBnum();
            fprintf (stream,"ПНБ: %s\n",pRouter->GetBasePort()->GetPNB(PNBnum).name);
            int cargo_type = pRouter->GetCargoType();
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
			//fprintf (stream,"Груз: %d\n", cargo_type);
			fprintf (stream,"Груз: %s\n", sCargoType.GetString());
            fprintf (stream,"Количество груза необходимое ПНБ: %d т\n", pRouter->GetBasePort()->GetPNBRequiredCargo(PNBnum,cargo_type));
            fprintf (stream,"Судно: %s\n",pRouter->GetVehicle()->GetTypeName());
            fprintf (stream,"Грузоподъемность судна: %d т\n",pRouter->GetVehicle()->GetLoad());
            //fprintf (stream,"Провозоспособность судна: %d\n",pRouter->GetVehicleCargoForNavigation());
            fprintf (stream,"Количество судов: %d\n", pRouter->GetVehicleNumber());
			fprintf (stream,"Количество судов нецелое: %f\n", pRouter->GetDVehicleNumber());
            fprintf (stream,"Доход судов на маршруте: %d\n\n", ((pRouter->GetVehicleIncome())*(pRouter->GetVehicleNumber())));
      
            pRouter = pRouter->next;  
        }
    }
	fclose(stream);
}
