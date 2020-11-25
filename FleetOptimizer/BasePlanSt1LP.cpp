// BasePlanSt1LP.cpp: implementation of the CBasePlanSt1LP class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "FleetOptimizer.h"
#include "BasePlanSt1LP.h"
#include "OutputView.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


CBasePlanSt1LP* CBasePlanSt1LP::_instance = 0;

CBasePlanSt1LP* CBasePlanSt1LP::Instance(){
    if (_instance == 0){
        _instance = new CBasePlanSt1LP;
    }
    return _instance;
}


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CBasePlanSt1LP::CBasePlanSt1LP()
{
	RoutersList = 0;
    BasePlanSt1LP = 0;

    //MessageBox(NULL,"CBasePlanSt1LP()","CBasePlanSt1LP",MB_OK);

	POSITION pos = ((CFleetOptimizerApp*)AfxGetApp())->GetCurrFleetOptimizerDoc()->GetFirstViewPosition();
	COutputView* pView = (COutputView*) ((CFleetOptimizerApp*)AfxGetApp())->GetCurrFleetOptimizerDoc()->GetNextView(pos);
	pView = (COutputView*) ((CFleetOptimizerApp*)AfxGetApp())->GetCurrFleetOptimizerDoc()->GetNextView(pos);
	pView->TextOut("CBasePlanSt1LP()");

}

CBasePlanSt1LP::~CBasePlanSt1LP()
{

}


void CBasePlanSt1LP::append(CRouter *pRouter)
{
    if(CBasePlanSt1LP::RoutersList == 0)
        RoutersList = pRouter;
    else
        at_end->next = pRouter;
            
    at_end = pRouter;
}

CRouter * CBasePlanSt1LP::GetRoutersListHead()
{
    return CBasePlanSt1LP::RoutersList;
}

int CBasePlanSt1LP::CreateBasePlanInit()
{
	CPort *pBasePort;		//базовый пункт маршрута Базового Плана
	CVehicle *pVehicleBP;	//судно маршрута Базового Плана
	CLine *pLine;
	int PNB_num, cargo_type, vehicleIncome, vehicleNumber;
	double dVehicleNumber;


	
	CRouter *pBasePlan = CInitialBasePlan::Instance()->GetRoutersListHead();
	
	
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


            CRouterS *pRouter = new CRouterS(pBasePort,PNB_num,cargo_type);

			pRouter->SetVehicle(pVehicleBP);
			pRouter->vehicleIncome = vehicleIncome;
			pRouter->vehicleNumber = vehicleNumber;
			pRouter->dVehicleNumber = dVehicleNumber;

			_instance->append(pRouter);

            pBasePlan = pBasePlan->GetNextRouter();  
        }
    }


	return 1;
}

void CBasePlanSt1LP::print()
{
    CRouter *pRouter = RoutersList;
    
    FILE *stream;
    stream = fopen( "E:\\Users\\mv\\Log\\CBasePlanSt1LP.txt", "w" ); //TODO

    if (pRouter == NULL){
        fprintf (stream,"Список маршрутов пуст\n");        
    }
    else{
        while(pRouter){

            fprintf (stream,"Базовый порт: %s\n",pRouter->GetBasePort()->GetPortName());
            int PNBnum = pRouter->GetLine()->GetPNBnum();
            fprintf (stream,"ПНБ: %s\n",pRouter->GetBasePort()->GetPNB(PNBnum).name);
            int cargo_type = pRouter->GetCargoType();
            fprintf (stream,"Груз: %d\n", cargo_type);
            fprintf (stream,"Количество груза необходимое ПНБ: %d\n", pRouter->GetBasePort()->GetPNBRequiredCargo(PNBnum,cargo_type));
            fprintf (stream,"Судно: %s\n",pRouter->GetVehicle()->GetTypeName());
            fprintf (stream,"Грузоподъемность судна: %d\n",pRouter->GetVehicle()->GetLoad());
            //fprintf (stream,"Провозоспособность судна: %d\n",pRouter->GetVehicleCargoForNavigation());
            fprintf (stream,"Количество судов: %d\n", pRouter->GetVehicleNumber());
            fprintf (stream,"Доход судов на маршруте: %d\n\n", ((pRouter->GetVehicleIncome())*(pRouter->GetVehicleNumber())));
      
            pRouter = pRouter->next;  
        }
    }
	fclose(stream);
}

