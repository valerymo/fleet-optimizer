// VehiclesList.cpp: implementation of the CVehiclesList class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "FleetOptimizer.h"
#include "VehiclesList.h"
#include "Ship.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

CVehiclesList* CVehiclesList::_instance = 0;

CVehiclesList* CVehiclesList::Instance(){
    if (_instance == 0){
        _instance = new CVehiclesList;
    }
    return _instance;
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CVehiclesList::CVehiclesList()
{
    VehiclesList = 0;
    //MessageBox(NULL,"CVehiclesList()","CVehiclesList",MB_OK);
	POSITION pos = ((CFleetOptimizerApp*)AfxGetApp())->GetCurrFleetOptimizerDoc()->GetFirstViewPosition();
	COutputView* pView = (COutputView*) ((CFleetOptimizerApp*)AfxGetApp())->GetCurrFleetOptimizerDoc()->GetNextView(pos);
	pView = (COutputView*) ((CFleetOptimizerApp*)AfxGetApp())->GetCurrFleetOptimizerDoc()->GetNextView(pos);
	pView->TextOut("CVehiclesList()");
}

CVehiclesList::~CVehiclesList()
{

}

//////////

void CVehiclesList::append(CVehicle *pVehicle)
{
    if(CVehiclesList::VehiclesList == 0)
        VehiclesList = pVehicle;
    else
        at_end->next = pVehicle;
            
    at_end = pVehicle;

}



void CVehiclesList::print()
{
    CVehicle *pVehicle = VehiclesList;
    
    FILE *stream;
    //stream = fopen( "E:\\Users\\mv\\Log\\VehiclesList.txt", "w" );
	CString LogFile = ((CFleetOptimizerApp*)AfxGetApp())->GetProfileString("FleetOptimizer","LogFileVehiclesList");	
    stream = fopen(LogFile, "w" );

    while(pVehicle){
        int load  = pVehicle->GetLoad();
        int coast = pVehicle->GetCoast();
        int speed = pVehicle->GetSpeed();
        char *name = pVehicle->GetTypeName();
        //fprintf (stream,"Name: %s, Load: %d, Coast: %d, Speed: %d \n",name,load,coast,speed);
        fprintf (stream,"Судно: %s,\t\tГрузоподъемность: %d,\tСкорость: %d,\tСтроительная стоимость: %d\n",name,load,speed,coast);
        pVehicle = pVehicle->next;
    }
    fclose(stream);
}

CVehicle * CVehiclesList::GetVehicleListHead()
{
    return VehiclesList;
}
