// BasePlanSt3Dp.cpp: implementation of the CBasePlanSt3Dp class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "FleetOptimizer.h"
#include "BasePlanSt3Dp.h"
#include "OutputView.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


CBasePlanSt3Dp* CBasePlanSt3Dp::_instance = 0;

CBasePlanSt3Dp* CBasePlanSt3Dp::Instance(){
    if (_instance == 0){
        _instance = new CBasePlanSt3Dp;
    }
    return _instance;
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CBasePlanSt3Dp::CBasePlanSt3Dp()
{
	m_RoutersList = 0;
    m_BasePlanSt3Dp = 0;

    //MessageBox(NULL,"CBasePlanSt3Dp()","CBasePlanSt3Dp",MB_OK);
	POSITION pos = ((CFleetOptimizerApp*)AfxGetApp())->GetCurrFleetOptimizerDoc()->GetFirstViewPosition();
	m_pOutView = (COutputView*) ((CFleetOptimizerApp*)AfxGetApp())->GetCurrFleetOptimizerDoc()->GetNextView(pos);
	m_pOutView = (COutputView*) ((CFleetOptimizerApp*)AfxGetApp())->GetCurrFleetOptimizerDoc()->GetNextView(pos);
	//m_pOutView->TextOut("CBasePlanSt3Dp()");
}

CBasePlanSt3Dp::~CBasePlanSt3Dp()
{

}

void CBasePlanSt3Dp::print()
{

}

int CBasePlanSt3Dp::CreateBasePlanInit()
{
	CPort *pBasePort;		//базовый пункт маршрута Базового Плана
	CVehicle *pVehicle;	//судно маршрута Базового Плана
	int PNB_num, cargo_type, vehicleIncome, vehicleNumber, require_cargo;
	double dVehicleNumber;

	m_pOutView->TextOut("Creation of Initial Base Plan for 3rd stage of Optimization");
	
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
			
			require_cargo = pBasePort->GetPNBRequiredCargo(PNB_num, cargo_type);
			if (require_cargo <= 0){
				pBasePlan = pBasePlan->GetNextRouter(); 
				continue;
			}

			pVehicle =	pBasePlan->GetVehicle();
			vehicleNumber = pBasePlan->GetVehicleNumber();
			vehicleIncome = pBasePlan->GetVehicleIncome();
			dVehicleNumber = pBasePlan->GetDVehicleNumber();
			

            CRouterP *pRouter = new CRouterP(pBasePort,PNB_num,cargo_type);

			pRouter->SetVehicle(pVehicle);
			pRouter->vehicleIncome = vehicleIncome;
			pRouter->vehicleNumber = vehicleNumber;
			pRouter->dVehicleNumber = dVehicleNumber;

			_instance->append(pRouter);

            pBasePlan = pBasePlan->GetNextRouter();  
        }
    }



	
	return 1;
}

void CBasePlanSt3Dp::append(CRouterP *pRouter)
{
    if(CBasePlanSt3Dp::m_RoutersList == 0)
        m_RoutersList = pRouter;
    else
        m_at_end->next = pRouter;
            
    m_at_end = pRouter;
}

CRouterP* CBasePlanSt3Dp::GetRoutersListHead()
{
	return m_RoutersList;
}

int CBasePlanSt3Dp::CreateBasePlanInit(int param)
{
	if (param == 0)
		return 1;
	else
		return 0; //stub
}