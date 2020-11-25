// BasePlanSt2LpDp.cpp: implementation of the CBasePlanSt2LpDp class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "FleetOptimizer.h"
#include "BasePlanSt2LpDp.h"
#include <math.h>
#include "OutputView.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif



CBasePlanSt2LpDp* CBasePlanSt2LpDp::_instance = 0;

CBasePlanSt2LpDp* CBasePlanSt2LpDp::Instance(){
    if (_instance == 0){
        _instance = new CBasePlanSt2LpDp;
    }
    return _instance;
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CBasePlanSt2LpDp::CBasePlanSt2LpDp()
{
	RoutersList = 0;
    BasePlanSt2LpDp = 0;

    //MessageBox(NULL,"CBasePlanSt2LpDp()","CBasePlanSt2LP",MB_OK);

	POSITION pos = ((CFleetOptimizerApp*)AfxGetApp())->GetCurrFleetOptimizerDoc()->GetFirstViewPosition();
	m_pOutView = (COutputView*) ((CFleetOptimizerApp*)AfxGetApp())->GetCurrFleetOptimizerDoc()->GetNextView(pos);
	m_pOutView = (COutputView*) ((CFleetOptimizerApp*)AfxGetApp())->GetCurrFleetOptimizerDoc()->GetNextView(pos);

}

CBasePlanSt2LpDp::~CBasePlanSt2LpDp()
{

}

void CBasePlanSt2LpDp::append(CRouterL *pRouter)
{
    if(CBasePlanSt2LpDp::RoutersList == 0)
        RoutersList = pRouter;
    else
        at_end->next = pRouter;
            
    at_end = pRouter;
}

int CBasePlanSt2LpDp::CreateBasePlanInit()
{
	m_pOutView->TextOut("Creation of Initial Base Plan for 2nd stage of Optimization");
	CRouter *pBasePlan = CBasePlanSt1LP::Instance()->GetRoutersListHead();
	if (pBasePlan == NULL){
		pBasePlan = CInitialBasePlan::Instance()->GetRoutersListHead();
	}

    if (pBasePlan == NULL){
        	return 0;
    }

	//DuplicatePlan(pBasePlan);
	DuplicateConvertPlan(pBasePlan);

	return 1;
}




CRouterL* CBasePlanSt2LpDp::DuplicatePlan(CRouter *pBasePlan)
{
	/////////

	CPort *pBasePort;		//базовый пункт маршрута Базового Плана
	CVehicle *pVehicleBP;	//судно маршрута Базового Плана
	CLine *pLine;

	int PNB_num, cargo_type, vehicleIncome, vehicleNumber;
	double dVehicleNumber;

	///////////
	
	
	
	CRouter *pRouter = pBasePlan;
    if (pBasePlan == NULL){
			//fprintf (stream,"BasePlan пуст\n");
        	return 0;
    }
    else{
        while(pBasePlan){	
			pBasePort =		pBasePlan->GetBasePort();
			PNB_num =		pBasePlan->GetLine()->GetPNBnum();
			cargo_type =	pBasePlan->GetCargoType();
			pVehicleBP =	pBasePlan->GetVehicle();
			vehicleNumber = pBasePlan->GetVehicleNumber();
			vehicleIncome = pBasePlan->GetVehicleIncome();
			dVehicleNumber = pBasePlan->GetDVehicleNumber();
			pLine = pBasePlan->GetLine();


			CRouterL *pRouter = new CRouterL(pBasePort, PNB_num, cargo_type);

			pRouter->SetVehicle(pVehicleBP);
			pRouter->vehicleIncome = vehicleIncome;
			pRouter->vehicleNumber = vehicleNumber;
			pRouter->dVehicleNumber = dVehicleNumber;

			CBasePlanSt2LpDp::_instance->append(pRouter);

            pBasePlan = pBasePlan->GetNextRouter();  
        }
    }


	return 0;
}


void CBasePlanSt2LpDp::print()
{
    CRouter *pRouter = RoutersList;
	CLine	*pLine;
	int PNBnum;
    
    FILE *stream;
    stream = fopen( "E:\\Users\\mv\\Log\\CBasePlanSt2LpDp.txt", "w" ); //TODO

    if (pRouter == NULL){
        fprintf (stream,"Список маршрутов пуст\n");        
    }
    else{
        while(pRouter){
			pLine = pRouter->GetLine();
			PNBnum = pLine->GetPNBnum();
            fprintf (stream,"Базовый порт: %s\n",pRouter->GetBasePort()->GetPortName());
            int cargo_type = pRouter->GetCargoType();
            fprintf (stream,"Груз: %d\n", cargo_type);
            fprintf (stream,"Количество груза необходимое ПНБ: %d\n", pRouter->GetBasePort()->GetPNBRequiredCargo(PNBnum,cargo_type));
            fprintf (stream,"Судно: %s\n",pRouter->GetVehicle()->GetTypeName());
            fprintf (stream,"Грузоподъемность судна: %d\n",pRouter->GetVehicle()->GetLoad());
            //fprintf (stream,"Провозоспособность судна: %d\n",pRouter->GetVehicleCargoForNavigation());
            fprintf (stream,"Количество судов: %d\n", pRouter->GetVehicleNumber());
            fprintf (stream,"Доход судов на маршруте: %d\n", ((pRouter->GetVehicleIncome())*(pRouter->GetVehicleNumber())));

			fprintf (stream,"Список линий (пунктов): \n");
			while(pLine){
				PNBnum = pLine->GetPNBnum();
				fprintf (stream,"ПНБ: %s\n",pRouter->GetBasePort()->GetPNB(PNBnum).name);
				pLine = pLine->GetNextLine();
			}
			fprintf (stream,"\n");
      
            pRouter = pRouter->next;  
        }
    }

}

CRouterL * CBasePlanSt2LpDp::GetRoutersListHead()
{
    return RoutersList;
}





CRouterL* CBasePlanSt2LpDp::DuplicateConvertPlan(CRouter *pBasePlan)
{
	CPort *pBasePort;		//базовый пункт маршрута Базового Плана
	CVehicle *pVehicle;	//судно маршрута Базового Плана
	//CLine *pLine;

	int PNB_num, cargo_type, VehicleIncome, VehicleNumber,
		VehicleLoad, LineCount, PNBRequiredCargo;
	double dVehicleNumber;
	float dLineCount;

	
	
	
	CRouter *pRouter = pBasePlan;
    if (pBasePlan == NULL){
			//fprintf (stream,"BasePlan пуст\n");
        	return 0;
    }
    else{
        while(pBasePlan){	
			pBasePort =		pBasePlan->GetBasePort();
			PNB_num =		pBasePlan->GetLine()->GetPNBnum();
			cargo_type =	pBasePlan->GetCargoType();
			pVehicle =	pBasePlan->GetVehicle();
			VehicleNumber = pBasePlan->GetVehicleNumber();
			VehicleIncome = pBasePlan->GetVehicleIncome();
			dVehicleNumber = pBasePlan->GetDVehicleNumber();
			VehicleLoad = pVehicle->GetLoad();
			

			CRouterL *pRouter = new CRouterL(pBasePort, PNB_num, cargo_type);

			pRouter->SetVehicle(pVehicle);
			pRouter->vehicleIncome = VehicleIncome;
			pRouter->vehicleNumber = VehicleNumber;
			pRouter->dVehicleNumber = dVehicleNumber;

			PNBRequiredCargo = pRouter->GetBasePort()->GetPNBRequiredCargo(PNB_num,cargo_type);
			if (VehicleNumber > 1)
				dLineCount = (float)(((float)PNBRequiredCargo/VehicleLoad)/dVehicleNumber);  // or /VehicleNumber ...?
			else 
				dLineCount = (float)PNBRequiredCargo/VehicleLoad;
			LineCount = (int)ceil(dLineCount);
			pRouter->GetLine()->SetCount(LineCount);

			CBasePlanSt2LpDp::_instance->append(pRouter);

            pBasePlan = pBasePlan->GetNextRouter();  
        }
    }


	return 0;
}

