// FleetTechEconomicEng.cpp: implementation of the CFleetTechEconomicEng class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "FleetOptimizer.h"
#include "FleetTechEconomicEng.h"
#include "Port.h"
#include "Vehicle.h"
#include "Ship.h"
#include <math.h>
#include "RouterL.h"
#include "RouterP.h"
#include "OutputView.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

CFleetTechEconomicEng* CFleetTechEconomicEng::_instance = 0;

CFleetTechEconomicEng* CFleetTechEconomicEng::Instance(){
    if (_instance == 0){
        _instance = new CFleetTechEconomicEng;
    }
    return _instance;
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CFleetTechEconomicEng::CFleetTechEconomicEng()
{
	for (int i = 0; i < MAX_PROTOTYPES_RECORDS_IN_SHIPDB; i++)
		m_aShipPrototype[i] = new ShipPrototypeMainDetails;

	POSITION pos = ((CFleetOptimizerApp*)AfxGetApp())->GetCurrFleetOptimizerDoc()->GetFirstViewPosition();
	m_pOutView = (COutputView*) ((CFleetOptimizerApp*)AfxGetApp())->GetCurrFleetOptimizerDoc()->GetNextView(pos);
	m_pOutView = (COutputView*) ((CFleetOptimizerApp*)AfxGetApp())->GetCurrFleetOptimizerDoc()->GetNextView(pos);
	//m_pOutView->TextOut("CFleetTechEconomicEng()");

	m_InvestmentYearLimit = 0;

	m_sLogFNameError = ((CFleetOptimizerApp*)AfxGetApp())->GetLogFNameError();
	m_nErrorStatus = 0;

}

CFleetTechEconomicEng::~CFleetTechEconomicEng()
{

}

int CFleetTechEconomicEng::VehicleRouterNumberForNavigation(CPort *pBasePort, int PNB_num, int cargo_type, CVehicle *pVehicle)
{
    int VehicleRouterNumber; //���������� ������ ����� �� ���������.
	int DistancePortPNB; //���������� ����� ������� �����-������� � ���.
    int OperationDaysInYear; //���������� ���� � ����, ������� ����� ����� ��������� � ������������ (� �����)
                           //�� ������� ����� ������ � �������.
    float TripTimeDays;
	float LoadTimeDays, LoadTimeDaysPort, LoadTimeDaysPNB;
	int speed;
	
	OperationDaysInYear = GetVehicleOperationsDaysInYear(pVehicle->GetRegisterClass(),pBasePort->GetPortRegion());

	int PNB_id = pBasePort->GetPNB(PNB_num).id;
    DistancePortPNB = pBasePort->GetDistancePortPNB(PNB_id);
	LoadTimeDaysPort = pBasePort->GetPortShipLoadingTime(pVehicle, cargo_type);
	//LoadTimeDaysPNB = pBasePort->GetPNBShipLoadingTime(PNB_id, pVehicle, cargo_type);
	LoadTimeDaysPNB = pBasePort->GetPNBShipLoadingTime(PNB_num, pVehicle, cargo_type);
	LoadTimeDays = LoadTimeDaysPort + LoadTimeDaysPNB;
	//TripTimeDays = ((2*(float)DistancePortPNB/pVehicle->GetSpeed())/24)+ LoadTimeDays ;
	speed = pVehicle->GetSpeed();
	TripTimeDays = ((2*(float)DistancePortPNB/speed)/24)+ LoadTimeDays ;
	VehicleRouterNumber = (int)ceil((float)OperationDaysInYear/TripTimeDays);
 
/*
    FILE *stream;   //for debug
    stream = fopen( "E:\\Users\\mv\\Log\\Economic_Validate1.txt", "a" ); // for debug
    fprintf (stream,"PNB_id: %d, cargo_type: %d, Vehicle: %s, speed: %d, LoadTimeDaysPort: %f, LoadTimeDaysPNB: %f, TripTimeDays: %f,OperationDaysInYear: %d,VehicleRouterNumber: %d\n",PNB_id, cargo_type, pVehicle->GetTypeName(),speed, LoadTimeDaysPort,LoadTimeDaysPNB, TripTimeDays,OperationDaysInYear,VehicleRouterNumber);
    fprintf (stream,"Speed(): %d DistancePortPNB: %d\n",pVehicle->GetSpeed(), DistancePortPNB);
	fclose(stream);

	//return 5;
*/
    return VehicleRouterNumber;
}

int CFleetTechEconomicEng::VehicleLineValidation(CPort *pBasePort, int PNB_num, int cargo_type, CVehicle *pVehicle)
{

  ///////////////////////////////////////////////////////////////
  ////�������� �� ������������������. ����������� ������� ����.
  ///////////////////////////////////////////////////////////////
  
    //���������� ������ ����� �� ���������.  
    int router_number = VehicleRouterNumberForNavigation(pBasePort, PNB_num, cargo_type, pVehicle);
    
    //������������������ �����:
    //���������� ����� ���� cargo_type ������� ����� ����� ��������� �� ���������.
    int cargo_for_navigation = (pVehicle->GetLoad())* router_number;
    
    //���������� ����� ����������� ��� ���������� �������������� �� �����.
    double dVehicleNum = ((double)pBasePort->GetPNBRequiredCargo(PNB_num,cargo_type))/cargo_for_navigation;

	/*
    FILE *stream;   //for debug
    stream = fopen( "E:\\Users\\mv\\Log\\Economic_Validate.txt", "w" ); // for debug
    fprintf (stream,"dVehicleNum: %f\n",dVehicleNum);
    fclose(stream);
    */
    if (dVehicleNum < 0.01)
        return 0;

	if (!(pVehicle->GetCargoCompatibility(cargo_type))){
		return 0;
	}

	// �������� ���������� ������������ �����
	int nVechicleExist = pVehicle->IsExist();
	int nVechicleExistDesign = pVehicle->IsExistDesign();
	if ((nVechicleExist == 1) && (nVechicleExistDesign == 0)){
		int nVehicleExistNum = pVehicle->GetExistNumRemain();
		if (dVehicleNum > nVehicleExistNum)
			return 0;
	}

    return 1;
}

int CFleetTechEconomicEng::VehicleIncomeOnLineForYear(CPort *pBasePort, int PNB_num, int cargo_type, CVehicle *pVehicle, int &vehicles_number, double &dVehicleNum, int &BuildCoastYearPay, int &OperationalExpensesYear, int &cargo_for_navigation)
{
    int OverallIncomeYear;

    //���������� ������ ����� �� ���������.  
    int router_number = VehicleRouterNumberForNavigation(pBasePort, PNB_num, cargo_type, pVehicle);
    
    //���������� ����� ���� cargo_type ������������ ������ �� ���������.
    cargo_for_navigation = (pVehicle->GetLoad())* router_number;
    
    //���������� ����� ���� pVehicle->GetTypeName(), ����������� ��� ���������� �������������� �� �����.
    //vehicles_number = ceil((pBasePort->GetAvailableCargo(cargo_type))/cargo_for_navigation);
    //int income = 
    dVehicleNum = ((float)pBasePort->GetPNBRequiredCargo(PNB_num,cargo_type))/cargo_for_navigation;
    vehicles_number = (int)ceil(dVehicleNum);
    
	/*
    FILE *stream;   //for debug
    stream = fopen( "E:\\Users\\mv\\Log\\DebugVicNum.txt", "w" ); // for debug
    fprintf (stream,"dVehicleNum: %f\tvehicles_number: %d\t PNB_number: %d\t router_number: %d\n",dVehicleNum,vehicles_number,PNB_num,router_number); //for debug
    fclose(stream);
    */

    BuildCoastYearPay = VehicleBuildCoastYearPay(pVehicle);

	//����� {begin}:
	int Income, Distance, DistancePortPNB, tariff, load;

	int PNB_id = pBasePort->GetPNB(PNB_num).id;
	DistancePortPNB = pBasePort->GetDistancePortPNB(PNB_id); //one router
	Distance = DistancePortPNB*router_number; //miles for year
	tariff = GetVehicleTariff(pVehicle,cargo_type); //$ for t*mile
	load = pVehicle->GetLoad();
	Income = tariff * load * Distance;
	//����� {end}

   	int VehicleTripForYear = Distance*2;
	OperationalExpensesYear = VehicleOperationalExpensesYear(pVehicle, pBasePort->GetPortRegion(), VehicleTripForYear);

    OverallIncomeYear = Income -(BuildCoastYearPay + OperationalExpensesYear);


    return ((OverallIncomeYear * vehicles_number) - Income) ;
    //return (OverallIncomeYear * vehicles_number);
    //return (OverallIncomeYear * dVehicleNum);
}

int CFleetTechEconomicEng::VehicleOperationalExpensesDay(CVehicle *pVehicle, 
									char* Region,
									// ������� �������� ...
								    int &ExpensesDayOnTrip,	// �� ���� 	
									int &ExpensesDayInPortLoading,	// �� ������� ��� ��������� ����������
									int &ExpensesDayInPortNoLoading, // �� ������� ��� �������� ��������
									int &ExpensesDayNoOperationWinter,	// �� ������
									int &ExpensesDayOnRepair)	// �� �������

{
	CString strTmp;
    int load;
	float cPtp_Proj, cPtp_Proto, N_Nprot;

    FILE *streamError;
    streamError = fopen(m_sLogFNameError, "a");
	
	
	//C������� ������� ���������
	int ExpensesDayOnTripProto;	// �� ���� 	
    int ExpensesDayInPortLoadingProto;	// �� ������� ��� ��������� ����������
    int ExpensesDayInPortNoLoadingProto; // �� ������� ��� �������� ��������
    int ExpensesDayNoOperationWinterProto;	// �� ������
    int ExpensesDayOnRepairProto;	// �� �������


	CString strProtoName = pVehicle->GetPrototypeName();
	CString strRegisterClass = pVehicle->GetRegisterClass();
	load = pVehicle->GetLoad();

	//����� ������ ������ ��������� � �������.
	int flProtoFound = 0;
	int i;

	// �������� ������� ���������:
	if (strRegisterClass.Find("*ACV*") != -1){ //ACV
		//����� ������ ������ ��� ��������� � �������.
		for (i = 1; i < MAX_PROTOTYPES_RECORDS_IN_ACVDB; i++){
			if (m_aACVPrototype[i].name == strProtoName){
				flProtoFound = 1;
				break;
			}
		}
		if (flProtoFound == 0){
			CString strMessage = "�� ������ �������� ��� �����. VehicleOperationalExpensesDay()";
			strMessage += pVehicle->GetTypeName();
			strMessage += "\n";
			//AfxMessageBox(strMessage);
			m_pOutView->TextOut(strMessage);
			fprintf(streamError, strMessage);
			fclose(streamError);
			m_nErrorStatus = 1;
			return 0;
		}
		// �������� ������� ��� ���������:
		ExpensesDayOnTripProto = m_aACVPrototype[i].exp1;
		ExpensesDayInPortLoadingProto = m_aACVPrototype[i].exp2;
		ExpensesDayInPortNoLoadingProto = m_aACVPrototype[i].exp3;
		ExpensesDayNoOperationWinterProto = m_aACVPrototype[i].exp4;
		ExpensesDayOnRepairProto = m_aACVPrototype[i].exp5;

		//����� ���������� (� ������������, ��������� ��� �������������)
		//������������ ����� ��������� ���������	//� ������ ������ ��������� ... � ���������� �.�. ������������
		if (pVehicle->IsExist() == 1){
			ExpensesDayOnTrip = ExpensesDayOnTripProto;	// �� ���� 	
			ExpensesDayInPortLoading = ExpensesDayInPortLoadingProto;	// �� ������� ��� ��������� ����������
			ExpensesDayInPortNoLoading = ExpensesDayInPortNoLoadingProto; // �� ������� ��� �������� ��������
			ExpensesDayNoOperationWinter = ExpensesDayNoOperationWinterProto;	// �� ������
			ExpensesDayOnRepair = ExpensesDayOnRepairProto;	// �� �������
			fclose(streamError);
			return 1;
		}



		// �������� ������� ��� �������:
		//�� ����:
		ACVMainDetails* acv_tmp = new ACVMainDetails;
		ACVPrototypeMainDetails* acv_prototype_tmp = new ACVPrototypeMainDetails;
		acv_prototype_tmp->name = m_aACVPrototype[i].name;
		CFleetTechEconomicEng::Instance()->GetACVPrototypeMainDetails(acv_prototype_tmp);
		acv_tmp->name = pVehicle->GetTypeName();
		acv_tmp->speed = (float)pVehicle->GetSpeed();
		acv_tmp->Dgr = pVehicle->GetLoad();
		acv_tmp->r = pVehicle->GetRange();
		if (acv_tmp->r <= 0){
			CString strMessage = "������ � �������� ��������� ����. VehicleOperationalExpensesDay()";
			strMessage += pVehicle->GetTypeName();
			strMessage += "\n";
			//AfxMessageBox(strMessage);
			m_pOutView->TextOut(strMessage);
			fprintf(streamError, strMessage);
			fclose(streamError);
			m_nErrorStatus = 1;
			return 0;
		}
		acv_tmp->h3 = pVehicle->GetH3();
		CACVDesign::Instance()->GetACVMainDetails(acv_prototype_tmp, acv_tmp);
		if(acv_tmp->N < 7000)
			cPtp_Proj = (float)(0.38 - 0.024*(acv_tmp->N/1000 - 2));
		else 
			cPtp_Proj = (float)(0.25 - 1.43*acv_tmp->N/1000000);
		
		if(m_aACVPrototype[i].N < 7000)
			cPtp_Proto = (float)(0.38 - 0.024*(m_aACVPrototype[i].N/1000 - 2));
		else 
			cPtp_Proto = (float)(0.25 - 1.43*m_aACVPrototype[i].N/1000000);
				
		N_Nprot = (float)acv_tmp->N/m_aACVPrototype[i].N;
		ExpensesDayOnTrip = (int)(ExpensesDayOnTripProto * N_Nprot * cPtp_Proj/cPtp_Proto);
		
		//�� ������� ��� ��������� ����������:
		ExpensesDayInPortLoading = (int)(ExpensesDayInPortLoadingProto*pow(N_Nprot, 0.1f));
		//�� ������� ��� �������� ��������:
		ExpensesDayInPortNoLoading = (int)(ExpensesDayInPortNoLoadingProto*(float)pow(N_Nprot, 0.1f));
		//�� ������:
		ExpensesDayNoOperationWinter = (int)(ExpensesDayNoOperationWinterProto*pow(N_Nprot, 0.1f));
		//�� �������:
		ExpensesDayOnRepair = (int)(ExpensesDayOnRepairProto*pow(N_Nprot, 0.1f));

	}
	else{	//Ship
		//����� ������ ������ ����� ��������� � �������.
		for (i = 1; i < MAX_PROTOTYPES_RECORDS_IN_SHIPDB; i++){
			if (m_aShipPrototype[i]->name == strProtoName){
				flProtoFound = 1;
				break;
			}
		}
		if (flProtoFound == 0){
			CString strMessage = "�� ������ �������� ��� ����� ";
			strMessage += pVehicle->GetTypeName();
			strMessage += ". VehicleOperationalExpensesDay()\n";
			//AfxMessageBox(strMessage);
			m_pOutView->TextOut(strMessage);
			fprintf(streamError, strMessage);
			fclose(streamError);
			m_nErrorStatus = 1;
			return 0;
		}
		// �������� ������� ����� ���������:
		ExpensesDayOnTripProto = m_aShipPrototype[i]->exp1;
		ExpensesDayInPortLoadingProto = m_aShipPrototype[i]->exp2;
		ExpensesDayInPortNoLoadingProto = m_aShipPrototype[i]->exp3;
		ExpensesDayNoOperationWinterProto = m_aShipPrototype[i]->exp4;
		ExpensesDayOnRepairProto = m_aShipPrototype[i]->exp5;

		// �������� ������� ����� �������:
		//�� ����:
		ShipMainDetails* ship_tmp = new ShipMainDetails;
		ShipPrototypeMainDetails* ship_prototype_tmp = new ShipPrototypeMainDetails;
		ship_prototype_tmp->name = m_aShipPrototype[i]->name;
		CFleetTechEconomicEng::Instance()->GetShipPrototypeMainDetails(ship_prototype_tmp);
		ship_tmp->speed = (float)pVehicle->GetSpeed();
		ship_tmp->Pgr = pVehicle->GetLoad();
		ship_tmp->name = pVehicle->GetTypeName();
		ship_tmp->mu = GetCargoSpecificCapacity(pVehicle);
		CShipDesign::Instance()->GetShipMainDetails(ship_prototype_tmp, ship_tmp);
		
		//������������ ������� (9.19), ����: N=V^0.5*speed^2.5/C 
		//����������� ����������� ��������� ������� � ���������
		N_Nprot = (float) pow((float (ship_tmp->D)/m_aShipPrototype[i]->D),0.5f)*((float) pow((ship_tmp->speed/m_aShipPrototype[i]->speed),2.5f));
		//��� ������ ������� ��������� ��������� ������� �������, 
		//�� ���������� ������������ N_Nprot � ������� < 1.
		ExpensesDayOnTrip = (int) (ExpensesDayOnTripProto*pow(N_Nprot, 0.7f));
		
	
		//�� ������� ��� ��������� ����������:
		ExpensesDayInPortLoading = (int)(ExpensesDayInPortLoadingProto*pow(N_Nprot, 0.1f));
		//�� ������� ��� �������� ��������:
		ExpensesDayInPortNoLoading = (int)(ExpensesDayInPortNoLoadingProto*pow(N_Nprot, 0.1f));
		//�� ������:
		ExpensesDayNoOperationWinter = (int)(ExpensesDayNoOperationWinterProto*pow(N_Nprot, 0.1f));
		//�� �������:
		ExpensesDayOnRepair = (int)(ExpensesDayOnRepairProto*pow(N_Nprot, 0.1f));
	
	}
	
	if( (ExpensesDayOnTrip <= 0) || 
		(ExpensesDayInPortLoading <= 0) || 
		(ExpensesDayInPortNoLoading <= 0) || 
		(ExpensesDayNoOperationWinter <= 0) || 
		(ExpensesDayOnRepair <= 0)) 
	{
		CString strMessage = "������ � ���������� ������� ���������������� �������� �����:";
		strMessage += pVehicle->GetTypeName();
		strMessage += ". VehicleOperationalExpensesDay()\n";
		fclose(streamError);
		m_nErrorStatus = 1;
		return 0;	
	}

	fclose(streamError);
    return 1;
}

int CFleetTechEconomicEng::VehicleOperationalExpensesYear(CVehicle *pVehicle, char* Region, int TripDistanceForYear)
{
    int ExpensesYear, load, res;

	FILE *streamError;
    streamError = fopen(m_sLogFNameError, "a");
	
	//������� �������
	int ExpensesDayOnTrip = 0;	// �� ���� 	
    int ExpensesDayInPortLoading = 0;	// �� ������� ��� ��������� ����������
    int ExpensesDayInPortNoLoading = 0; // �� ������� ��� �������� ��������
    int ExpensesDayNoOperationWinter = 0;	// �� ������
    int ExpensesDayOnRepair = 0;	// �� �������
    
	//���������� ���� ...
	//int NumberDaysOnTripYear;	// �� ����
    //int NumberDaysInPortLoadingYear;	//�� ������� ��� ��������� ����������
    //int NumberDaysInPortNoLoadingYear;	//�� ������� ��� �������� ��������
    //int NumberDaysNoOperationWinter;	//�� ������
    //int NumberDaysOnRepair;	//�� �������

	load = pVehicle->GetLoad();

	res = VehicleOperationalExpensesDay(	pVehicle, 
											Region,
											ExpensesDayOnTrip,	
											ExpensesDayInPortLoading,
											ExpensesDayInPortNoLoading, 
											ExpensesDayNoOperationWinter,	
											ExpensesDayOnRepair);

	if (res == 0){
		CString strMessage = "������ � ���������� ���������������� �������� �����:  ";
		strMessage += pVehicle->GetTypeName();
		strMessage += ".  ������������ �����������... VehicleOperationalExpensesYear()\n";
		//AfxMessageBox(strMessage);
		m_pOutView->TextOut(strMessage);
		fprintf(streamError, strMessage);


		//"Stab" ��� ���������/��������:
		ExpensesYear = 1000;	//prev value : 100
		ExpensesYear = (int)(ExpensesYear * load/30 * 0.7);
	}

	///
	int OperationsDaysInYear;
	int NavigationPeriod = 	GetVehicleNavigationPeriod(pVehicle->GetRegisterClass(),Region);
	int WaitingTimeForWether = GetVehicleWaitingYearTimeForWether(pVehicle->GetRegisterClass(),Region);
	int RepairYearTime = GetVehicleRepairYearTime(pVehicle->GetRegisterClass(), Region);

	if ((NavigationPeriod == -1)||(WaitingTimeForWether == -1)||(RepairYearTime == -1)){
		fclose(streamError);
		return 0;
	}
	else {
		if (RepairYearTime < (365 - NavigationPeriod)){ //������ �������������� �� ����� ������� ������
			OperationsDaysInYear = NavigationPeriod - WaitingTimeForWether;
		}else{
			RepairYearTime = RepairYearTime - (365 - NavigationPeriod);
			OperationsDaysInYear = NavigationPeriod - WaitingTimeForWether - RepairYearTime;
		}
	}

	int TripTime = (int) (((float)TripDistanceForYear/pVehicle->GetSpeed())/24);
	int PortLoadNoLoadTime = OperationsDaysInYear - TripTime;
	if (PortLoadNoLoadTime <=0){
		fclose(streamError);
		return 0;
	}

	//������� �� ���
	int ExpensesOnTrip = ExpensesDayOnTrip * TripTime; // �� ���� 	
    int ExpensesInPortLoading = (int)(0.75*ExpensesDayInPortLoading * PortLoadNoLoadTime);	// �� ������� ��� ��������� ����������
    int ExpensesInPortNoLoading = (int)(0.25*ExpensesDayInPortNoLoading * PortLoadNoLoadTime); // �� ������� ��� �������� ��������
    int ExpensesNoOperationWinter = ExpensesDayNoOperationWinter * WaitingTimeForWether;	// �� ������
    int ExpensesOnRepair = ExpensesDayOnRepair * RepairYearTime;	// �� �������

	ExpensesYear =	ExpensesOnTrip+ExpensesInPortLoading+
					ExpensesInPortNoLoading+
					ExpensesNoOperationWinter+ExpensesOnRepair;


	fclose(streamError);
    return ExpensesYear;

}


int CFleetTechEconomicEng::VehicleBuildCoastYearPay(CVehicle *pVehicle)
{
    int VehicleBuildCoastYearPay; // ������� ������� � ���� ��������� ����� �� �������������� �����
    int VehicleBuildCoast; // ������������ ��������� �����
    int Resourse; // ���� ������ ����� (���)
    double Rent; // ��������� ������ ����� 
    
    VehicleBuildCoast = pVehicle->GetCoast();
    Resourse = 20;
    Rent = 0.12;

    VehicleBuildCoastYearPay = (int)(VehicleBuildCoast * Rent * (pow((1+ Rent),Resourse))/(pow((1+ Rent),Resourse) - 1));

    /*
	FILE *stream;   //for debug
    stream = fopen( "E:\\Users\\mv\\Log\\Rent.txt", "w" ); // for debug
    fprintf (stream,"BuildCoast: %d, VehicleBuildCoastYearPay: %d\n",VehicleBuildCoast, VehicleBuildCoastYearPay);
    fclose(stream);
	*/


    return VehicleBuildCoastYearPay;
}

int CFleetTechEconomicEng::GetVehicleNavigationPeriod(char* VehicleClass, char* Region)
{
	int result = -1; //����������������� �������������� ������� � ����
	CString strRegisterClass;
	strRegisterClass = VehicleClass;

	if (strcmp(Region, "Arctic") == 0){//***********�������***********
		//if (strcmp(VehicleClass, "ACV") == 0)
		if (strRegisterClass.Find("*ACV*") != -1)
				return 300; //365
		if (strcmp(VehicleClass, "ULA") == 0)
				return 300;
		if (strcmp(VehicleClass, "L1") == 0)
				return 100; //200
	}else { //************************* ������������� ����� **********
		//if (strcmp(VehicleClass, "ACV") == 0)
		if (strRegisterClass.Find("*ACV*") != -1)
				return 300;
		if (strcmp(VehicleClass, "ULA") == 0)
				return 300;
		if (strcmp(VehicleClass, "L1") == 0)
				return 300;
	}
	return result;
}


int CFleetTechEconomicEng::GetVehicleWaitingYearTimeForWether (char* VehicleClass, char* Region)
{
	CString strRegisterClass;
	strRegisterClass = VehicleClass;
	int result = -1; //����� �������� �� ������������� � ����

	if (strcmp(Region, "Arctic") == 0){//***********�������***********
		//if (strcmp(VehicleClass, "ACV") == 0)
		if (strRegisterClass.Find("*ACV*") != -1)
				return 80;
		if (strcmp(VehicleClass, "ULA") == 0)
				return 30;
		if (strcmp(VehicleClass, "L1") == 0)
				return 10;
	}else { //************************* ������������� ����� **********
		//if (strcmp(VehicleClass, "ACV") == 0)
		if (strRegisterClass.Find("*ACV*") != -1)
				return 30;
		if (strcmp(VehicleClass, "ULA") == 0)
				return 15;
		if (strcmp(VehicleClass, "L1") == 0)
				return 15;
	}
	return result;
}

int CFleetTechEconomicEng::GetVehicleRepairYearTime(char* VehicleClass, char* Region)
{
	CString strRegisterClass;
	strRegisterClass = VehicleClass;
	int result; //����� ������� � ����

	if (strcmp(Region, "Arctic") == 0){//***********�������***********
		//if (strcmp(VehicleClass, "ACV") == 0){
		if (strRegisterClass.Find("*ACV*") != -1){
			result = 60;
		}else{
			result = 70;
		}
	}else { //************************* ������������� ����� **********
		//if (strcmp(VehicleClass, "ACV") == 0){
		if (strRegisterClass.Find("*ACV*") != -1){
			result = 45;
		}else{
			result = 55;
		}
	}
	return result;
}

int CFleetTechEconomicEng::GetVehicleOperationsDaysInYear(char* VehicleClass, char* Region)
{
	int OperationsDaysInYear;

	int NavigationPeriod = 	GetVehicleNavigationPeriod(VehicleClass,Region);
	int WaitingTimeForWether = GetVehicleWaitingYearTimeForWether(VehicleClass,Region);
	int RepairYearTime = GetVehicleRepairYearTime(VehicleClass, Region);
/*
    FILE *stream;   //for debug
    stream = fopen( "E:\\Users\\mv\\Log\\Economic_Validate1.txt", "a" ); // for debug
    fprintf (stream,"VehicleClass: %s, Region: %s, NavigationPeriod: %d,WaitingTimeForWether: %d,RepairYearTime: %d\n",VehicleClass, Region,NavigationPeriod,WaitingTimeForWether,RepairYearTime);
	fclose(stream);
*/

	if ((NavigationPeriod == -1)||(WaitingTimeForWether == -1)||(RepairYearTime == -1)){
		return -1;
	}
	
	if (RepairYearTime < (365 - NavigationPeriod)){ //������ �������������� �� ����� ������� ������
		OperationsDaysInYear = NavigationPeriod - WaitingTimeForWether;
	}else{
		RepairYearTime = RepairYearTime - (365 - NavigationPeriod);
		OperationsDaysInYear = NavigationPeriod - WaitingTimeForWether - RepairYearTime;
	}



	return OperationsDaysInYear;
}

int CFleetTechEconomicEng::VehicleIncomeOnRouterLForYear(CRouterL *pRouter, int &VehicleNum, double &dVehicleNum, int &VehicleCapability)
{
    int OverallIncomeYear, BuildCoastYearPay, OperationalExpensesYear,
		OperationDaysInYear, DistancePortPNB, DistancePortPNBAll, PNBRequiredCargo, PNBRequiredCargoAll;
	CLine *pLine;
	CPort *pBasePort;
	CVehicle *pVehicle;
	int cargo_type, PNB_num, PNB_id, LineCount, vehicle_load, TransportMoment;
	//double dVehicleNumLine;
	int VehicleCapabilityOnLine, RouterOperationsTime;
	float LoadTimeDays, LoadTimeDaysAll;


	pBasePort = pRouter->GetBasePort();
	pLine = pRouter->GetLine();
	pVehicle = pRouter->GetVehicle();
	vehicle_load = pVehicle->GetLoad();
	dVehicleNum = 0;
	VehicleCapability = 0;
	DistancePortPNB = 0;
	DistancePortPNBAll = 0;
	PNBRequiredCargoAll = 0;
	LoadTimeDaysAll = 0;
	TransportMoment = 0;

	int flLineCount = 1;
	while(pLine){
		LineCount = pLine->GetCount();
		if (LineCount > 1)
			flLineCount = 0;
		if (LineCount == 0){
			pLine = pLine->GetNextLine();
			continue;
		}
		PNB_num = pLine->GetPNBnum();
		PNB_id = pBasePort->GetPNB(PNB_num).id;
		cargo_type = pLine->GetCargoType();
		DistancePortPNB = pBasePort->GetDistancePortPNB(PNB_id);
		DistancePortPNBAll += DistancePortPNB * LineCount;
		LoadTimeDays = pBasePort->GetPNBShipLoadingTime(PNB_num, pVehicle, cargo_type);
		LoadTimeDays = LoadTimeDays*LineCount;
		LoadTimeDaysAll += LoadTimeDays;
		PNBRequiredCargo = pBasePort->GetPNBRequiredCargo(PNB_num,cargo_type);
		VehicleCapabilityOnLine = vehicle_load * LineCount;
		if(VehicleCapabilityOnLine > PNBRequiredCargo){
			VehicleCapabilityOnLine = PNBRequiredCargo;
		}
		VehicleCapability += VehicleCapabilityOnLine;
		PNBRequiredCargoAll += PNBRequiredCargo;

	    ///////dVehicleNumLine = ((double)pBasePort->GetPNBRequiredCargoFirst(PNB_num,cargo_type))/VehicleCapabilityOnLine;
	    //dVehicleNumLine = ((double)pBasePort->GetPNBRequiredCargo(PNB_num,cargo_type))/VehicleCapabilityOnLine;
		//dVehicleNum += dVehicleNumLine;

/*		if(VehicleCapabilityOnLine > PNBRequiredCargo){
			TransportMoment += PNBRequiredCargo * DistancePortPNB;
		}
		else {
			TransportMoment += VehicleCapabilityOnLine * DistancePortPNB;
		}
*/
		TransportMoment += VehicleCapabilityOnLine * DistancePortPNB;

		pLine = pLine->GetNextLine();
	}
/*
	VehicleNum = floor(dVehicleNum);
	if ((dVehicleNum - (double)VehicleNum) > 0.5)
		VehicleNum = ceil(dVehicleNum);		
*/
//	if (dVehicleNum >400){
//		int debug_stab = 0;
//	}

	RouterOperationsTime = (int) (((2*(float)DistancePortPNBAll/pVehicle->GetSpeed())/24)+ LoadTimeDaysAll);
	OperationDaysInYear = CFleetTechEconomicEng::Instance()->GetVehicleOperationsDaysInYear(pVehicle->GetRegisterClass(), pBasePort->GetPortRegion());

	//PNBRequiredCargo = pBasePort->GetPNBRequiredCargo(PNB_num,cargo_type);
	//if((flLineCount == 1) && (PNBRequiredCargo > VehicleCapability)){
	//		dVehicleNum = (double)PNBRequiredCargo/VehicleCapability;
	double tmp_dVehicleNum = (double)PNBRequiredCargoAll/VehicleCapability;
	if( tmp_dVehicleNum > 2){
		dVehicleNum = tmp_dVehicleNum;
	}
	else { 
		dVehicleNum = (double)RouterOperationsTime/OperationDaysInYear;
	}


	
	VehicleNum = (int)ceil(dVehicleNum);

    BuildCoastYearPay = VehicleBuildCoastYearPay(pVehicle);
	int VehicleTripForYear = DistancePortPNBAll*2;
    OperationalExpensesYear = VehicleOperationalExpensesYear(pVehicle, pBasePort->GetPortRegion(), VehicleTripForYear);

/*	if (VehicleNum == 1){
		if (VehicleCapability > PNBRequiredCargo){
			VehicleCapability = PNBRequiredCargo;
		}
	}
	else { // VehicleNum > 1
		VehicleCapability = (int)ceil(VehicleCapability * dVehicleNum); 
	}
*/
	if (VehicleNum > 1){
		VehicleCapability = (int)ceil(VehicleCapability * dVehicleNum); 
	}

	//����� {begin}: //(��� ������ �����)
	int Income, tariff;// Distance;
	//Distance = DistancePortPNB; //Distance for year on RouterL
	//load = pVehicle->GetLoad();
	//Income = tariff * load * Distance; 
	tariff = GetVehicleTariff(pVehicle,cargo_type); //$ for t*mile
	//Income = tariff * vehicle_load * DistancePortPNBAll; 
	Income = tariff * TransportMoment; 	//29.04.05
	//����� {end}

    OverallIncomeYear = Income -(BuildCoastYearPay + OperationalExpensesYear);

	//OverallIncomeYear = -(BuildCoastYearPay + OperationalExpensesYear);
	


    return ((int)ceil(OverallIncomeYear * dVehicleNum));

}

int CFleetTechEconomicEng::VehicleRouterPLineValidation(CPort *pBasePort, int PNB_num, int cargo_type, CVehicle *pVehicle)
{

	int RequiredCargo;
  ///////////////////////////////////////////////////////////////
  ////�������� �� ������������������. ����������� ������� ����.
  ///////////////////////////////////////////////////////////////
  
    //���������� ������ ����� �� ���������.  
    int router_number = VehicleRouterNumberForNavigation(pBasePort, PNB_num, cargo_type, pVehicle);
    
    //������������������ �����:
    //���������� ����� ���� cargo_type ������� ����� ����� ��������� �� ���������.
    int cargo_for_navigation = (pVehicle->GetLoad())* router_number;
    
    //���������� ����� ����������� ��� ���������� �������������� �� �����.
	RequiredCargo = pBasePort->GetPNBRequiredCargo(PNB_num,cargo_type);
    double dVehicleNum = ((double)RequiredCargo)/cargo_for_navigation;

    if (dVehicleNum < 0.001)
        return 0;

	if (!(pVehicle->GetCargoCompatibility(cargo_type))){
		return 0;
	}

	// �������� ���������� ������������ �����
	int nVechicleExist = pVehicle->IsExist();
	int nVechicleExistDesign = pVehicle->IsExistDesign();
	if ((nVechicleExist == 1) && (nVechicleExistDesign == 0)){
		int nVehicleExistNum = pVehicle->GetExistNumRemain();
		if (dVehicleNum > nVehicleExistNum)
			return 0;
	}

    return 1;

}



//int CFleetTechEconomicEng::GetVehicleBuildCoast(CString strShipTypeName, CString strRegisterClass, int load, int speed, int L, int B, int T,  CString strPrototypeName)
int CFleetTechEconomicEng::GetVehicleBuildCoast(ShipMainDetails * project, CString strPrototypeName)
{
	int flProtoFound = 0;
	int Ind;
	float Dk_Dkprot, N_Nprot, D, S;

	//Damp Prototypes DB into Table.
	DumpShipProtoDBIntoTable();

	//����� ������ ������ ��������� � �������.
	for (int i = 1; i < MAX_PROTOTYPES_RECORDS_IN_SHIPDB; i++){
		if (m_aShipPrototype[i]->name == strPrototypeName){
			Ind = i;
			flProtoFound = 1;
			break;
		}
	}
	if (flProtoFound == 0){
		CString strMessage = "�� ������ �������� ��� ����� ";
		strMessage += project->name;
		AfxMessageBox(strMessage);
		m_pOutView->TextOut(strMessage);
		//AfxMessageBox("GetVehicleBuildCoast: next");
		m_pOutView->TextOut("   ���������...");
		return 0;
	}
	
	//����� ������ ��� ����������� �������� ��.
	//������������ ���������������� ������� � �������� D � v
	//(����������������� �������������� ����������� Cmn)
	// ��. ���� 9.4, (9.19)
	// ��� ����� � Fr >= 0.3 ������������ ������� ������ (9.17),  
	// � �mn �� �.�. ��������� � �.�. ���������. 
	float Fr = (float)(project->speed/sqrt(9.81*project->L));

	// �������� ������������ ��������� ����� � ��������� �� ������.
	// ������ ������������ �������.
	// S = S�. + S�� +  Sconst,
	// S�. - ��������� �������; S�� - ��������� ��; 
	// Sconst - ������������ ��������� ����� ������������� ���������� ����� ������� � ����������.
	// S�. =  S�.����.* (D�./D�.����).
	// S�� =  S��.����.*(N/N����)^k21;   k21 ~= 1/3. 
	// �������� ������������ �� (9.19) ��� (9.17), ��. ���� 9.4.
	// ����������� ������� k21 ��������� ����������� ��������� �� �� ��������. 
	// �����  ���������� = 1/3 - ���� ����������� ����������� �����������. 
	// ��������� ������ ������ ��� ���������� ������������� ������������.
	// ������������  �������� ��������� ������� � �� � ����� �� ��������� ����� � �����.
	// S�. = S*k1; S�� = S*k2; ����� k1 � k2 ������� �� ���������.
	// �����:
	// S = S����.*k1*(D�./D�.����) + S����.*k2*(N/N����)^k21 + S����.*(1-k1-k2) = 
	//   = S����. * [k1*((D�./D�.����)-1) + k2*((N/N����)^k21 -1) + 1].

	// D�./D�.����  = (p01*D)/(p01����.*D����)		(2.3)  
	//				= (p01/p01����.) * (P/P����) * (nu.����./nu).  (*)
	// ������, �������� ��� �������� ����� �������� � ���������� ����� ���� �������������,
	// ����� ��������������� ��������������� ��������� �� ������� 2.5. ����. 
	// ��������� ������ ������ ������������� ������ � ������� �������������� �����.
	// �.�.:  D�./D�.���� ~= P��/P��.����.
	// ����������� ��� H/T, delta, � �������� ������� ���������:
	// D�./D�.���� ~= P��/P��.���� = (L/L����)^2.5 * (B/B����)		(**)
	// �������� ����� ������������ �������������� ������� ������ (2.16):
	// P�� = q'*delta^1/3*L^5/4*B^3/4*H^1/2.
	// ����������� ������������ ���������� ���� q' (???) � ������������ ����� �������: 
	// D�./D�.���� ~= (L/L����)^5/4 * (B/B����)^3/4 (H/H)^1/2.		(***)
	// ��� �����, �������� ������ (**) � (***) ������ ������������, ������ ������������� (***)

	float Ht = m_aShipPrototype[Ind]->H/m_aShipPrototype[Ind]->T;
	project->H = project->T*Ht;
	// S  = S����. * [k1*((D�./D�.����)-1) + k2*((N/N����)^k21 -1) + 1].
	float tmp1 = (float)pow(project->L/m_aShipPrototype[Ind]->L,(float)5/4);
	float tmp2 = (float)pow(project->B/m_aShipPrototype[Ind]->B,(float)3/4);
	float tmp3 = (float)sqrt(project->H/m_aShipPrototype[Ind]->H);
	
	Dk_Dkprot = (float)((pow((project->L/m_aShipPrototype[Ind]->L),(float)5/4)) * (pow((project->B/m_aShipPrototype[Ind]->B),(float)3/4)) * (sqrt(project->H/m_aShipPrototype[Ind]->H)));
	//������ �������������:
	D = project->Pgr*((float)m_aShipPrototype[Ind]->D/m_aShipPrototype[Ind]->Pgr);
	if (Fr >= 0.3){
		N_Nprot = (float)((sqrt(D/m_aShipPrototype[Ind]->D))*(pow((project->speed/m_aShipPrototype[Ind]->speed),3)));
	}else{
		N_Nprot = (float)((sqrt(D/m_aShipPrototype[Ind]->D))*(pow((project->speed/m_aShipPrototype[Ind]->speed),2.5f)));
	}
	
	S = (float)(m_aShipPrototype[Ind]->priceBuild * ( m_aShipPrototype[Ind]->priceK1*(Dk_Dkprot-1) + m_aShipPrototype[Ind]->priceK2*(sqrt(N_Nprot)-1) + 1));

	return (int)S;
}


void CFleetTechEconomicEng::DumpShipProtoDBIntoTable()
{
	char * ShipProtoDBFile;
	int RecordsNumber;

	//ShipProtoDBFile = "proto_ship_db.fd";
	CString	str = ((CFleetOptimizerApp*)AfxGetApp())->GetProfileString("FleetOptimizer","DbProtoShip");
	ShipProtoDBFile = _strdup(str.GetBuffer(20));
	
	CFile f( ShipProtoDBFile, CFile::modeRead );
	f.SeekToBegin();
	CArchive ar(&f, CArchive::load);

	
	// a)verify records number in DB file.
	CString	strNum, strTmp;
	ULONGLONG positionEnd, position; 
	char *stopstring;
	
	CFile* fp = ar.GetFile();
	fp->SeekToEnd();
	positionEnd = fp->GetPosition();
	fp->SeekToBegin();
	position = fp->GetPosition();
	
	if (positionEnd == 0)
		RecordsNumber = 0;
	else {
		ar >> strNum;
		RecordsNumber  = (int)strtod(strNum,&stopstring);
	}
	int i = 1;

	//Dump DB into str Table.
	while (i <= RecordsNumber){
		ar >> m_aShipPrototype[i]->name;
		ar >> m_aShipPrototype[i]->projectNumber; 
		ar >> m_aShipPrototype[i]->registerClass; 
		ar >> strNum;
		m_aShipPrototype[i]->L  = (float)strtod(strNum,&stopstring);
		ar >> strNum;
		m_aShipPrototype[i]->Lpp  = (float)strtod(strNum,&stopstring);
		ar >> strNum;
		m_aShipPrototype[i]->B  = (float)strtod(strNum,&stopstring);
		ar >> strNum;
		m_aShipPrototype[i]->H  = (float)strtod(strNum,&stopstring);
		ar >> strNum;
		m_aShipPrototype[i]->T  = (float)strtod(strNum,&stopstring);
		ar >> strNum;
		m_aShipPrototype[i]->D  = (int)strtod(strNum,&stopstring);
		ar >> strNum;
		m_aShipPrototype[i]->DW  = (int)strtod(strNum,&stopstring);
		ar >> strNum;
		m_aShipPrototype[i]->Pgr  = (int)strtod(strNum,&stopstring);
		ar >> strNum;
		m_aShipPrototype[i]->speed  = (float)strtod(strNum,&stopstring);
		ar >> strNum;
		m_aShipPrototype[i]->speed_bal  = (float)strtod(strNum,&stopstring);
		ar >> strNum;
		m_aShipPrototype[i]->dist  = (int)strtod(strNum,&stopstring);
		ar >> strNum;
		m_aShipPrototype[i]->Wkip  = (int)strtod(strNum,&stopstring);
		ar >> strNum;
		m_aShipPrototype[i]->Wnas  = (int)strtod(strNum,&stopstring);
		ar >> strNum;
		m_aShipPrototype[i]->luk_num  = (int)strtod(strNum,&stopstring);
		ar >> m_aShipPrototype[i]->load_stub1; 
		ar >> strNum;
		m_aShipPrototype[i]->stab1  = (int)strtod(strNum,&stopstring);
		ar >> m_aShipPrototype[i]->load_stub2; 
		ar >> strNum;
		m_aShipPrototype[i]->stab2  = (int)strtod(strNum,&stopstring);
		ar >> strTmp;  //empty column
		ar >> strNum;
		m_aShipPrototype[i]->priceBuild  = (int)strtod(strNum,&stopstring);
		ar >> strNum;
		m_aShipPrototype[i]->priceK1  = (float)strtod(strNum,&stopstring);
		ar >> strNum;
		m_aShipPrototype[i]->priceK2  = (float)strtod(strNum,&stopstring);
		ar >> str; //empty column
		ar >> strNum;
		m_aShipPrototype[i]->exp1  = (int)strtod(strNum,&stopstring);
		ar >> strNum;
		m_aShipPrototype[i]->exp2  = (int)strtod(strNum,&stopstring);
		ar >> strNum;
		m_aShipPrototype[i]->exp3  = (int)strtod(strNum,&stopstring);
		ar >> strNum;
		m_aShipPrototype[i]->exp4  = (int)strtod(strNum,&stopstring);
		ar >> strNum;
		m_aShipPrototype[i]->exp5  = (int)strtod(strNum,&stopstring);
		ar >> strNum;
		m_aShipPrototype[i]->priceK3  = (float)strtod(strNum,&stopstring);
		ar >> strNum;
		m_aShipPrototype[i]->p01  = (int)strtod(strNum,&stopstring);
		ar >> str; //empty column
		ar >> str; //empty column, end of line
		i++;
	}


	ar.Close();
	f.Close();

}

int CFleetTechEconomicEng::GetShipPrototypeMainDetails(ShipPrototypeMainDetails *prototype)
{
	int flProtoFound = 0;
	int Ind;
	//CString strProtoName, str_bl;
	//str_bl = " ";
	//strProtoName = str_bl + prototype->name;

	DumpShipProtoDBIntoTable();

	//����� ������ ������ ��������� � �������.
	for (int i = 1; i < MAX_PROTOTYPES_RECORDS_IN_SHIPDB; i++){
		if (m_aShipPrototype[i]->name == prototype->name){
		//if (m_aShipPrototype[i]->name == strProtoName){
			Ind = i;
			flProtoFound = 1;
			break;
		}
	}
	if (flProtoFound == 0){
		CString strMessage = "�� ������ �������� ��� ����� (CFleetTechEconomicEng::GetShipPrototypeMainDetails)";
		CString strtmp = prototype->name;
		strMessage += strtmp;
		AfxMessageBox(strMessage);
		//AfxMessageBox("CFleetTechEconomicEng::GetShipPrototypeMainDetails.  next");
		m_pOutView->TextOut(strMessage);
		m_pOutView->TextOut("   ���������...");
		return 0;
	}
	
	SetShipPrototypeMainDetails(prototype, Ind);

	
	return 1;
}

int CFleetTechEconomicEng::SetShipPrototypeMainDetails(ShipPrototypeMainDetails *prototype, int i)
{

	 prototype->name = m_aShipPrototype[i]->name;	//(0,1,"��� ");
	 prototype->projectNumber = m_aShipPrototype[i]->projectNumber;	//(0,2,"N ������� ");
	 prototype->registerClass = m_aShipPrototype[i]->registerClass;		//(0,3,"����� ��������");
	 prototype->L = m_aShipPrototype[i]->L;	//(0,4,"L, �. ����� ����������");
	 prototype->Lpp = m_aShipPrototype[i]->Lpp;	//(0,5,"L �, �. ����� ����� ����������������");
	 prototype->B = m_aShipPrototype[i]->B;		//(0,6,"B, �. ������");
	 prototype->H = m_aShipPrototype[i]->H;		//(0,7,"H, �. ������ �����");
	 prototype->T = m_aShipPrototype[i]->T;		//(0,8,"T, �. ������ �� �������� ���� (T1/T2)");
	 prototype->D = m_aShipPrototype[i]->D;	//(0,9,"D��, �. ������������� � ����� (T1/T2)");
	 prototype->DW = m_aShipPrototype[i]->DW;		//(0,10,"DW, �.  ������� (T1/T2)");
	 prototype->Pgr = m_aShipPrototype[i]->Pgr;	//(0,11,"P��, �. ���������������� (T1/T2)");
	 prototype->speed = m_aShipPrototype[i]->speed;	//(0,12,"V��, ��. �������� ���� � �����");
	 prototype->speed_bal = m_aShipPrototype[i]->speed_bal;	//(0,13,"V���, ��. �������� ���� � ��������");
	 prototype->dist = m_aShipPrototype[i]->dist;	//(0,14,"Dist, ����. ��������� �������� ");
	 prototype->Wkip = m_aShipPrototype[i]->Wkip;	//(0,15,"W���, �3. ���������������� �������");
	 prototype->Wnas = m_aShipPrototype[i]->Wnas;	//(0,16,"W���, �3. ���������������� �������");
	 prototype->luk_num = m_aShipPrototype[i]->luk_num;	//(0,17,"�������� ����, ���.");
	 prototype->load_stub1 = m_aShipPrototype[i]->load_stub1;	//(0,18,"������, ���.*����������������");
	 prototype->stab1 = m_aShipPrototype[i]->stab1;		//(0,19,"������, ����� �� ����, �.");
	 prototype->load_stub2 = m_aShipPrototype[i]->load_stub2;		//(0,20,"�����, ���.*����������������");
	 prototype->stab2 = m_aShipPrototype[i]->stab2;		//(0,21,"�����, ����� �� ����, �.");
	 prototype->priceBuild = m_aShipPrototype[i]->priceBuild;	//(0,23,"������������ ��������� �����");
	 prototype->priceK1 = m_aShipPrototype[i]->priceK1;	//(0,24,"��������� �������/��������� �����");
	 prototype->priceK2 = m_aShipPrototype[i]->priceK2;	//(0,25,"��������� ��/��������� �����");
	 prototype->exp1 = m_aShipPrototype[i]->exp1;		//(0,27,"�����.������� ��������, �� ����");
	 prototype->exp2 = m_aShipPrototype[i]->exp2;		//(0,28,"�����.����.���., �� ������� � ��.����������");
	 prototype->exp3 = m_aShipPrototype[i]->exp3;		//(0,29,"�����.����.���., �� ������� ��� ��.��������");
	 prototype->exp4 = m_aShipPrototype[i]->exp4;		//(0,30,"�����.����.���., �� ������");
	 prototype->exp5 = m_aShipPrototype[i]->exp5;		//(0,31,"�����.����.���., �� �������");
	 prototype->priceK3 = m_aShipPrototype[i]->priceK3;	//(0,32,"��������� �������/�����.����.�� ����");
	 prototype->p01 = m_aShipPrototype[i]->p01;		//(0,33," p01, ������");

	return 1;
}



int CFleetTechEconomicEng::GetACVPrototypeMainDetails(ACVPrototypeMainDetails *prototype)
{
	int flProtoFound = 0;
	int Ind;

	DumpACVProtoDBIntoTable();
	
	//����� ������ ������ ��������� � �������.
	for (int i = 1; i < MAX_PROTOTYPES_RECORDS_IN_ACVDB; i++){
		if (m_aACVPrototype[i].name == prototype->name){
			Ind = i;
			flProtoFound = 1;
			break;
		}
	}
	if (flProtoFound == 0){
		CString strMessage = "�� ������ �������� ��� ����� (CFleetTechEconomicEng::GetACVPrototypeMainDetails) ";
		CString strtmp = prototype->name;
		strMessage += strtmp;
		AfxMessageBox(strMessage);
		//AfxMessageBox("CFleetTechEconomicEng::GetACVPrototypeMainDetails,  exit");
		m_pOutView->TextOut(strMessage);
		m_pOutView->TextOut("   ���������...");

		return 0;
	}
	
	SetACVPrototypeMainDetails(prototype, Ind);

	
	return 1;
}


int CFleetTechEconomicEng::SetACVPrototypeMainDetails(ACVPrototypeMainDetails *prototype, int i)
{
	prototype->speed = m_aACVPrototype[i].speed_lim;		//speed
	prototype->r = m_aACVPrototype[i].r;				// ��������� ��������� ���� �����
	prototype->a1 = m_aACVPrototype[i].a1;				// ����������� �������� ������
	prototype->a2 = m_aACVPrototype[i].a2;				// �����������, ����������� ����������� ������� �������,
														// ������ ��������� ���������� ��� ����������� � �.�.
	prototype->h_3 = m_aACVPrototype[i].h_3;			// h3% ������ �������������� �����
	
	prototype->name = m_aACVPrototype[i].name;			// (0,1,"��� ");
	prototype->projectNumber = m_aACVPrototype[i].projectNumber;	//(0,?,"N ������� ");
	prototype->registerClass = m_aACVPrototype[i].registerClass;	// (0,2,"����� ��������");
	prototype->L = m_aACVPrototype[i].L;				// (0,3,"   L");
	prototype->B = m_aACVPrototype[i].B;				// (0,4,"   B");
	prototype->D = m_aACVPrototype[i].D;				// (0,5,"  D�, �"); 
	prototype->Lp = m_aACVPrototype[i].Lp;				// (0,6,"  L�");  ����� ��������� �������, �
	prototype->Bp = m_aACVPrototype[i].Bp;				// (0,7,"  B�");
	prototype->LpBp = m_aACVPrototype[i].LpBp;			// (0,8,"L�/B�"); ��������� ��
	prototype->Sp = m_aACVPrototype[i].Sp;				// (0,9,"  S�");
	prototype->Hgo = m_aACVPrototype[i].Hgo;			// (0,10,"H ��");
	prototype->HnlHgo = m_aACVPrototype[i].HnlHgo;		// (0,11,"H��/H��, �����.������ �� ��");
	prototype->lp = m_aACVPrototype[i].lp;				// (0,12,"l�=L�/(D/p)^1/3"); �����. �����
	prototype->Perp = m_aACVPrototype[i].Perp;			// (0,13,"  ��, �, �������� ��");
	prototype->Pp = m_aACVPrototype[i].Pp;				// (0,14,"  ��, ���, �������� � ��");
	prototype->Qn = m_aACVPrototype[i].Qn;				// (0,15,"������.������. �3/�");
	prototype->Q = m_aACVPrototype[i].Q;				// (0,16,"Q, �3/�, ������ �������");
	prototype->HgoBp = m_aACVPrototype[i].HgoBp;		// (0,17,"H��/B�, �����.������ ��");
	prototype->Kp = m_aACVPrototype[i].Kp;				// (0,18,"Kp, ����.����.");
	prototype->N = m_aACVPrototype[i].N;				// (0,19,"N, ���, ����.����.����.��");
	prototype->GD = m_aACVPrototype[i].GD;				// (0,20,"���. � ��� ��");
	prototype->NmaxNnom = m_aACVPrototype[i].NmaxNnom;	// (0,21,"����.��, Nmax/N���");
	prototype->Dviz = m_aACVPrototype[i].Dviz;			// (0,22,"���.���� * ���");
	prototype->Ddviz = m_aACVPrototype[i].Ddviz;		// (0,23,"������� ����., �");
	prototype->n_dviz = m_aACVPrototype[i].n_dviz;		// (0,24,"������� ��.����., 1/���");
	prototype->num_type_nagn = m_aACVPrototype[i].num_type_nagn;		// (0,25,"���.������������. * ���");
	prototype->Dnagn = m_aACVPrototype[i].Dnagn;		// (0,26,"������� �� ����.");
	prototype->n_nagn = m_aACVPrototype[i].n_nagn;		// (0,27,"������� ��.����., 1/���");
	prototype->speed_lim = m_aACVPrototype[i].speed_lim;// (0,28,"����.�������� �� ����� ����");
	prototype->FrD = m_aACVPrototype[i].FrD;			// (0,29,"�������.�������� FrD // (����.)");
	prototype->Dgr = m_aACVPrototype[i].Dgr;			// (0,30,"D��, �, ����� ��������� �����");
	prototype->pas_car = m_aACVPrototype[i].pas_car;	// (0,31,"���-��.������./�������.");
	prototype->Dtp = m_aACVPrototype[i].Dtp;			// (0,32,"D��, �, ������ �������");
	prototype->cND = m_aACVPrototype[i].cND;			// (0,33,"N/D��, ���/�, �������� �������������������");
	prototype->cKn = m_aACVPrototype[i].cKn;			// (0,34,"K=G*v/N, ����������� ��������");
	prototype->cQp = m_aACVPrototype[i].cQp;				// (0,35,"Q�/D�, ������.������ ����. ��.");
	prototype->cQps = m_aACVPrototype[i].cQps;			// (0,36,"Q�s*10^2, �������.����.����.����.");
	prototype->cQph = m_aACVPrototype[i].cQph;			// (0,37,"Q�h, �������.����.����.����.");
	prototype->hgo_d = m_aACVPrototype[i].hgo_d;		// (0,38,"�����.������ ��, H��/// (D�/p)^1/3");
	prototype->te = m_aACVPrototype[i].te;				// (0,39,"t�, ����.����� ��� ������� ��,��� �=0.55");
	prototype->teLp = m_aACVPrototype[i].teLp;			// (0,40,"// (t�/L�)10^3, �����.�����");
	prototype->teSp = m_aACVPrototype[i].teSp;			// (0,41,"// (t��/S�)10^2, �����.�����");
	prototype->Gs = m_aACVPrototype[i].Gs;				// (0,42,"Gs*10^2, ����. ��������");
	prototype->Gl = m_aACVPrototype[i].Gl;				// (0,43,"Gl*10^2, ����. ��������");
	prototype->timep = m_aACVPrototype[i].timep;		// (0,44,"t�=// (S�*H��)/Q�, c, ����� ���������� ��");
	prototype->Tz = m_aACVPrototype[i].Tz;				// (0,45,"Tz=6.28// (H��/g)^0.5, c, ������.������ ����. �����");
	prototype->timepTz = m_aACVPrototype[i].timepTz;	// (0,46,"t�/Tz");
	prototype->Dpor = m_aACVPrototype[i].Dpor;			// (0,47,"D���, �, ������������� ��������");
	prototype->cDgrD = m_aACVPrototype[i].cDgrD;		// (0,48,"n��=// (D��+D��)/D�, ����.������.������ ����� �� ����.������-��.");
	prototype->priceBuild = m_aACVPrototype[i].priceBuild;		// (0,51,"������������ ��������� �����");
	prototype->priceK1 = m_aACVPrototype[i].priceK1;	//("��������� �������/��������� �����");
	prototype->priceK2 = m_aACVPrototype[i].priceK2;	//("��������� ��/��������� �����");
	prototype->priceK21 = m_aACVPrototype[i].priceK21;	//("��������� ��/��������� �����");
	prototype->exp1 = m_aACVPrototype[i].exp1;		//("�����.������� ��������, �� ����");
	prototype->exp2 = m_aACVPrototype[i].exp2;		//("�����.����.���., �� ������� � ��.����������");
	prototype->exp3 = m_aACVPrototype[i].exp3;		//("�����.����.���., �� ������� ��� ��.��������");
	prototype->exp4 = m_aACVPrototype[i].exp4;		//("�����.����.���., �� ������");
	prototype->exp5 = m_aACVPrototype[i].exp5;		//("�����.����.���., �� �������");
	prototype->priceK3 = m_aACVPrototype[i].priceK3;	//("��������� �������/�����.����.�� ����");

	return 1;
}


//int CFleetTechEconomicEng::GetACVBuildCoast(CString strACVTypeName, CString strRegisterClass, int load, int speed, int L, int B, int T,  CString strPrototypeName)
int CFleetTechEconomicEng::GetACVBuildCoast(ACVMainDetails * project, CString strPrototypeName)
{
	int flProtoFound = 0;
	int Ind;
	float	S, Dk_proto, Dk, Dk_Dkprot, N_Nprot, Dgo_Dgoprot, 
			nu_tr,nu_pk,nu_dz,k3,k4,Kgk;

//	CString str_bl = " ";
//	strPrototypeName = str_bl + strPrototypeName;


	//Damp Prototypes DB into Table.
	DumpACVProtoDBIntoTable();

	//����� ������ ������ ��������� � �������.
	for (int i = 1; i < MAX_PROTOTYPES_RECORDS_IN_ACVDB; i++){
		if (m_aACVPrototype[i].name == strPrototypeName){
			Ind = i;
			flProtoFound = 1;
			break;
		}
	}
	if (flProtoFound == 0){
		CString strMessage = "�� ������ �������� ��� ����� ";
		strMessage += strPrototypeName;
		AfxMessageBox(strMessage);
		//AfxMessageBox("GetACVBuildCoast. next");
		m_pOutView->TextOut(strMessage);
		m_pOutView->TextOut("    ���������...");

		return 0;
	}
	
	// �������� ������������ ��������� ����� � ��������� �� ������.
	// ������ ������������ �������.
	// S = S�. + S�� + S�� +  Sconst,
	// S�. - ��������� �������; S�� - ��������� ��; S�� - ��������� ��; 
	// Sconst - ������������ ��������� ����� ������������� ���������� ����� ������� � ����������.
	// S�. =  S�.����.* (D�./D�.����).
	// S�� =  S��.����.*(N/N����)^k21;   k21 ~= 1/2. 
	// ����������� ������� k�� ��������� ����������� ��������� �� �� ��������. 
	// �����  ���������� = 1/2 - ���� ����������� ����������� �����������. 
	// ��������� ������ ������ ��� ���������� ������������� ������������.
	// ������������  �������� ��������� ������� � �� � ����� �� ��������� ����� � �����.
	// S�. = S*k1; S�� = S*k2; S�� = S*k3;  ����� k1,k2,k3 ������� �� ���������.
	// �����:
	// S = S����.*k1*(D�./D�.����) + S����.*k2*(N/N����)^k21 + S����.*k3*(D��./D��.����)+ S����.*(1-k1-k2-k3) = 
	//   = S����. * [k1*((D�./D�.����)-1) + k2*((N/N����)^k21 -1) + k3*((D��./D��.����)-1) + 1].

	// ����� ������� ������� � ��������� ������������ ����� ������ ����� ���
	// �� ��������(9.3-9.5), � ����������� �� ������������ �������� ��� 
	// Gs (1.13), ����������� ��������� � ���������� �������������� ���.
	// ��.  �.�.�������, �������������� �����, ����.
	//
	// Gs ����������� �� ���������.
	
	
	/// ******* ������ *******
	if (m_aACVPrototype[Ind].Gs <= 0.0078){
		Dk_proto = (float)(m_aACVPrototype[Ind].D * (0.28/(pow(m_aACVPrototype[Ind].D,0.33f)) + 0.01*(pow((m_aACVPrototype[Ind].D/m_aACVPrototype[Ind].Gs),0.33f)))); //(9.3), �.�.�������
		Dk = (float)(project->D * (0.28/(pow(project->D,0.33f)) + 0.01*(pow((project->D/m_aACVPrototype[Ind].Gs),0.33f)))); //(9.3)
	}else {
		if ((m_aACVPrototype[Ind].Gs > 0.0078) &&  (m_aACVPrototype[Ind].Gs <= 0.0156)){
			Dk_proto = (float)(m_aACVPrototype[Ind].D * (0.24 + 0.00175*(pow((m_aACVPrototype[Ind].D/m_aACVPrototype[Ind].Gs),0.33f)))); //(9.4)
			Dk = (float)(project->D * (0.24 + 0.00175*(pow((project->D/m_aACVPrototype[Ind].Gs),0.33f)))); //(9.4)
		}else {
			Dk_proto = (float)(m_aACVPrototype[Ind].D * (0.21 + 0.002*(pow((m_aACVPrototype[Ind].D/m_aACVPrototype[Ind].Gs),0.33f)))); //(9.5)
			Dk = (float)(project->D * (0.21 + 0.002*(pow((project->D/m_aACVPrototype[Ind].Gs),0.33f)))); //(9.5), �.�.�������
		}
	}
	if (((Dk/project->D) < 0.25) || ((Dk/project->D) > 0.32))	// 0.25<Dk/Dkprot<0.32,  ��. (9.2), ������� �.�.
		Dk = (float)(0.285*project->D);

	if (((Dk_proto/m_aACVPrototype[Ind].D < 0.25) || ((Dk_proto/m_aACVPrototype[Ind].D) > 0.32)))
		Dk_proto = (float)(0.285*m_aACVPrototype[Ind].D);


	Dk_Dkprot = (float)Dk/Dk_proto;


	/// ******* �� *******
	bool bGTD = TRUE; // �� - ���
	if (m_aACVPrototype[Ind].GD.Find("Turbo") == -1)
		bGTD = FALSE;	// �� - ��
	if (bGTD){
		nu_tr = (float)0.94;
		nu_pk = (float)0.42;
		if (m_aACVPrototype[Ind].name.Find("JEFF") != -1)
			nu_pk = (float)0.43;
		k3 = (float)1.08;
	}else{
		nu_tr = (float)0.97;
		nu_pk = (float)0.43;
		k3 = (float)1;
	}
	nu_dz = (float)0.5;
	Kgk = (float)0.2;
	k4 = (float)0.8;

	m_aACVPrototype[Ind].speed = m_aACVPrototype[Ind].speed_lim;
	N_Nprot = (float)((project->D/m_aACVPrototype[Ind].D)*(project->speed/m_aACVPrototype[Ind].speed)*(1+ k3*k4*Kgk*sqrt(project->Hgo)*nu_dz/(nu_pk*project->speed))/(1+ k3*k4*Kgk*sqrt(m_aACVPrototype[Ind].Hgo)*nu_dz/(nu_pk*m_aACVPrototype[Ind].speed)));

	/// ******* �� *******
	// Dgo = g'*Sp*Hgo   (9.50), �.�.�������.
	// ���������� g' �������� ������������� ��� ���������� ����������� ����.
	// g' = (11+-1.5) ��.�3., ��. ������ 9.6, �.�.�������
	
	Dgo_Dgoprot = (project->Hgo/m_aACVPrototype[Ind].Hgo)*(project->Sp/m_aACVPrototype[Ind].Sp);
	
	S = (float)(m_aACVPrototype[Ind].priceBuild * ( m_aACVPrototype[Ind].priceK1*(Dk_Dkprot-1) + m_aACVPrototype[Ind].priceK2*(sqrt(N_Nprot)-1) + m_aACVPrototype[Ind].priceK3*(Dgo_Dgoprot-1) + 1));

	return (int)S;
}



void CFleetTechEconomicEng::DumpACVProtoDBIntoTable()
{
	char * ACVProtoDBFile;
	int RecordsNumber;
	CString str;

	//ACVProtoDBFile = "proto_acv_db.fd";
	str = ((CFleetOptimizerApp*)AfxGetApp())->GetProfileString("FleetOptimizer","DbProtoACV");
	ACVProtoDBFile = _strdup(str.GetBuffer(20));


	CFile f( ACVProtoDBFile, CFile::modeRead );
	f.SeekToBegin();
	CArchive ar(&f, CArchive::load);

	
	// a)verify records number in DB file.
	CString	strNum, strTmp;
	ULONGLONG positionEnd, position; 
	char *stopstring;
	
	CFile* fp = ar.GetFile();
	fp->SeekToEnd();
	positionEnd = fp->GetPosition();
	fp->SeekToBegin();
	position = fp->GetPosition();
	
	if (positionEnd == 0)
		RecordsNumber = 0;
	else {
		ar >> strNum;
		RecordsNumber  = (int)strtod(strNum,&stopstring);
	}
	int i = 1;

	//Dump DB into str Table.
	while (i <= RecordsNumber){
			//ar >> m_aACVPrototype[i].
		ar >> m_aACVPrototype[i].name;								// (0,1,"��� ");
		ar >> m_aACVPrototype[i].projectNumber;						// (0,2,"N ������� ");
		ar >> m_aACVPrototype[i].registerClass;						// (0,3,"����� ��������");
		ar >> strNum;
		m_aACVPrototype[i].L  = (float)strtod(strNum,&stopstring);	// (0,4,"   L");
		ar >> strNum;
		m_aACVPrototype[i].B  = (float)strtod(strNum,&stopstring);	// (0,5,"   B");
		ar >> strNum;
		m_aACVPrototype[i].D  = (float)strtod(strNum,&stopstring);	// (0,6,"  D�, �"); 
		ar >> strNum;
		m_aACVPrototype[i].Lp  = (float)strtod(strNum,&stopstring);	// (0,7,"  L�");  ����� ��������� �������, �
		ar >> strNum;
		m_aACVPrototype[i].Bp  = (float)strtod(strNum,&stopstring);	// (0,8,"  B�");
		ar >> strNum;
		m_aACVPrototype[i].LpBp  = (float)strtod(strNum,&stopstring);	// (0,9,"L�/B�"); ��������� ��
		ar >> strNum;
		m_aACVPrototype[i].Sp  = (float)strtod(strNum,&stopstring);	// (0,10,"  S�");
		ar >> strNum;
		m_aACVPrototype[i].Hgo  = (float)strtod(strNum,&stopstring);	// (0,11,"H ��");
		ar >> strNum;
		m_aACVPrototype[i].HnlHgo  = (float)strtod(strNum,&stopstring);// (0,12,"H��/H��, �����.������ �� ��");
		ar >> strNum;
		m_aACVPrototype[i].lp  = (float)strtod(strNum,&stopstring);	// (0,13,"l�=L�/(D/p)^1/3"); �����. �����
		ar >> strNum;
		m_aACVPrototype[i].Perp  = (float)strtod(strNum,&stopstring);	// (0,14,"  ��, �, �������� ��");
		ar >> strNum;
		m_aACVPrototype[i].Pp  = (float)strtod(strNum,&stopstring);	// (0,15,"  ��, ���, �������� � ��");
		ar >> strNum;
		m_aACVPrototype[i].Qn  = (int)strtod(strNum,&stopstring);	// (0,16,"������.������. �3/�");
		ar >> strNum;
		m_aACVPrototype[i].Q  = (int)strtod(strNum,&stopstring);	// (0,17,"Q, �3/�, ������ �������");
		ar >> strNum;
		m_aACVPrototype[i].HgoBp  = (float)strtod(strNum,&stopstring);		// (0,18,"H��/B�, �����.������ ��");
		ar >> strNum;
		m_aACVPrototype[i].Kp  = (float)strtod(strNum,&stopstring);			// (0,19,"Kp, ����.����.");
		ar >> strNum;
		m_aACVPrototype[i].N  = (int)strtod(strNum,&stopstring);				// (0,20,"N, ���, ����.����.����.��");
		ar >> m_aACVPrototype[i].GD;			// (0,21,"���. � ��� ��");
		ar >> m_aACVPrototype[i].NmaxNnom;	// (0,22,"����.��, Nmax/N���");
		ar >> m_aACVPrototype[i].Dviz;		// (0,23,"���.���� * ���");
		ar >> strNum;
		m_aACVPrototype[i].Ddviz  = (float)strtod(strNum,&stopstring);		// (0,24,"������� ����., �");
		ar >> strNum;
		m_aACVPrototype[i].n_dviz  = (int)strtod(strNum,&stopstring);			// (0,25,"������� ��.����., 1/���");
		ar >> m_aACVPrototype[i].num_type_nagn;	// (0,26,"���.������������. * ���");
		ar >> strNum;
		m_aACVPrototype[i].Dnagn  = (float)strtod(strNum,&stopstring);		// (0,27,"������� �� ����.");
		ar >> strNum;
		m_aACVPrototype[i].n_nagn  = (int)strtod(strNum,&stopstring);			// (0,28,"������� ��.����., 1/���");
		ar >> strNum;
		m_aACVPrototype[i].speed_lim  = (float)strtod(strNum,&stopstring);		// (0,29,"����.�������� �� ����� ����");
		ar >> strNum;
		m_aACVPrototype[i].FrD  = (float)strtod(strNum,&stopstring);			// (0,30,"�������.�������� FrD // (����.)");
		ar >> strNum;
		m_aACVPrototype[i].Dgr  = (int)strtod(strNum,&stopstring);			// (0,31,"D��, �, ����� ��������� �����");
		ar >> m_aACVPrototype[i].pas_car;	// (0,32,"���-��.������./�������.");
		ar >> strNum;
		m_aACVPrototype[i].Dtp  = (float)strtod(strNum,&stopstring);			// (0,33,"D��, �, ������ �������");
		ar >> strNum;
		m_aACVPrototype[i].cND  = (float)strtod(strNum,&stopstring);			// (0,34,"N/D��, ���/�, �������� �������������������");
		ar >> strNum;
		m_aACVPrototype[i].cKn  = (float)strtod(strNum,&stopstring);			// (0,35,"K=G*v/N, ����������� ��������");
		ar >> strNum;
		m_aACVPrototype[i].cQp  = (float)strtod(strNum,&stopstring);			// (0,36,"Q�/D�, ������.������ ����. ��.");
		ar >> strNum;
		m_aACVPrototype[i].cQps  = (float)strtod(strNum,&stopstring);			// (0,37,"Q�s*10^2, �������.����.����.����.");
		ar >> strNum;
		m_aACVPrototype[i].cQph  = (float)strtod(strNum,&stopstring);			// (0,38,"Q�h, �������.����.����.����.");
		ar >> strNum;
		m_aACVPrototype[i].hgo_d  = (float)strtod(strNum,&stopstring);		// (0,39,"�����.������ ��, H��/// (D�/p)^1/3");
		ar >> strNum;
		m_aACVPrototype[i].te  = (float)strtod(strNum,&stopstring);			// (0,40,"t�, ����.����� ��� ������� ��,��� �=0.55");
		ar >> strNum;
		m_aACVPrototype[i].teLp  = (float)strtod(strNum,&stopstring);			// (0,41,"// (t�/L�)10^3, �����.�����");
		ar >> strNum;
		m_aACVPrototype[i].teSp  = (float)strtod(strNum,&stopstring);			// (0,42,"// (t��/S�)10^2, �����.�����");
		ar >> strNum;
		m_aACVPrototype[i].Gs  = (float)strtod(strNum,&stopstring);			// (0,43,"Gs*10^2, ����. ��������");
		ar >> strNum;
		m_aACVPrototype[i].Gl  = (float)strtod(strNum,&stopstring);			// (0,44,"Gl*10^2, ����. ��������");
		ar >> strNum;
		m_aACVPrototype[i].timep  = (float)strtod(strNum,&stopstring);		// (0,45,"t�=// (S�*H��)/Q�, c, ����� ���������� ��");
		ar >> strNum;
		m_aACVPrototype[i].Tz  = (float)strtod(strNum,&stopstring);			// (0,46,"Tz=6.28*// (H��/g)^0.5, c, ������.������ ����. �����");
		ar >> strNum;
		m_aACVPrototype[i].timepTz  = (float)strtod(strNum,&stopstring);		// (0,47,"t�/Tz");
		ar >> strNum;
		m_aACVPrototype[i].Dpor  = (float)strtod(strNum,&stopstring);			// (0,48,"D���, �, ������������� ��������");
		ar >> strNum;
		m_aACVPrototype[i].cDgrD  = (float)strtod(strNum,&stopstring);		// (0,49,"n��=// (D��+D��)/D�, ����.������.������ ����� �� ����.������-��.");
		ar >> strNum;
		m_aACVPrototype[i].r  = (int)strtod(strNum,&stopstring);				// (0.50, "��������� ����, ����")
		ar >> strNum;
		m_aACVPrototype[i].priceBuild  = (int)strtod(strNum,&stopstring);		// (0,51,"������������ ��������� �����");
		ar >> strNum;
		m_aACVPrototype[i].priceK1  = (float)strtod(strNum,&stopstring);		// (0,52,"��������� �������/��������� �����");
		ar >> strNum;
		m_aACVPrototype[i].priceK2  = (float)strtod(strNum,&stopstring);		// (0,53,"��������� ��/��������� �����");
		ar >> strNum;
		m_aACVPrototype[i].priceK21  = (float)strtod(strNum,&stopstring);		// (0,54,"��������� ��/��������� �����");
		ar >> strNum;
		m_aACVPrototype[i].exp1  = (int)strtod(strNum,&stopstring);			// (0,55,"�����.������� ��������, �� ����");
		ar >> strNum;
		m_aACVPrototype[i].exp2  = (int)strtod(strNum,&stopstring);			// (0,56,"�����.����.���., �� ������� � ��.����������");
		ar >> strNum;
		m_aACVPrototype[i].exp3  = (int)strtod(strNum,&stopstring);			// (0,57,"�����.����.���., �� ������� ��� ��.��������");
		ar >> strNum;
		m_aACVPrototype[i].exp4  = (int)strtod(strNum,&stopstring);			// (0,58,"�����.����.���., �� ������");
		ar >> strNum;
		m_aACVPrototype[i].exp5  = (int)strtod(strNum,&stopstring);			// (0,59,"�����.����.���., �� �������");
		ar >> strNum;
		m_aACVPrototype[i].priceK3  = (float)strtod(strNum,&stopstring);		// (0,60,"��������� �������/�����.����.�� ����");


		ar >> str; //end of line
		i++;
	}
}



///////////// for existing ships we need case when cargo was partioally transfered for specific port.
// begin:
int CFleetTechEconomicEng::VehicleIncomeOnLineForYear(CPort *pBasePort, int PNB_num, int cargo_type, CVehicle *pVehicle, int &vehicles_number, double &dVehicleNum, int &BuildCoastYearPay, int &OperationalExpensesYear, int &cargo_for_navigation, int cargo_done)
{
    int OverallIncomeYear;

    //���������� ������ ����� �� ���������.  
    int router_number = VehicleRouterNumberForNavigation(pBasePort, PNB_num, cargo_type, pVehicle);
    
    //���������� ����� ���� cargo_type ������������ ������ �� ���������.
    cargo_for_navigation = (pVehicle->GetLoad())* router_number;
    
    //���������� ����� ���� pVehicle->GetTypeName(), ����������� ��� ���������� �������������� �� �����.
	//dVehicleNum = ((double)pBasePort->GetPNBRequiredCargo(PNB_num,cargo_type))/cargo_for_navigation;
	dVehicleNum = ((double)(pBasePort->GetPNBRequiredCargo(PNB_num,cargo_type) - cargo_done))/cargo_for_navigation;
    vehicles_number = (int)ceil(dVehicleNum);
    /*
    fprintf (stream,"C����: %s\n",pVehicle->GetTypeName()); 
    fprintf (stream,"vehicles_n: %f\n",dVehicleNum); 
    fprintf (stream,"���������������� �����: %d\n",pVehicle->GetLoad()); 
    fprintf (stream,"cargo_for_navigation: %d\nvehicles_number: %d\n",cargo_for_navigation,vehicles_number); //for debug
    fprintf (stream,"AvailableCargo: %d\n",pBasePort->GetAvailableCargo(cargo_type));
    */

    BuildCoastYearPay = VehicleBuildCoastYearPay(pVehicle);

	//����� {begin}:
	int Income, Distance, DistancePortPNB, tariff, load;

	int PNB_id = pBasePort->GetPNB(PNB_num).id;
	DistancePortPNB = pBasePort->GetDistancePortPNB(PNB_id); //one router
	Distance = DistancePortPNB*router_number; //miles for year
	tariff = GetVehicleTariff(pVehicle,cargo_type); //$ for t*mile
	load = pVehicle->GetLoad();
	Income = tariff * load * Distance;
	//����� {end}

   	int VehicleTripForYear = Distance*2;
	OperationalExpensesYear = VehicleOperationalExpensesYear(pVehicle, pBasePort->GetPortRegion(),VehicleTripForYear);

    OverallIncomeYear = Income -(BuildCoastYearPay + OperationalExpensesYear);

    //OverallIncomeYear = -(BuildCoastYearPay + OperationalExpensesYear);


    return ((OverallIncomeYear * vehicles_number) - Income) ;
    //return (OverallIncomeYear * vehicles_number);
    //return (OverallIncomeYear * dVehicleNum);
}





int CFleetTechEconomicEng::VehicleLineValidation(CPort *pBasePort, int PNB_num, int cargo_type, CVehicle *pVehicle, int cargo_done)
{

  ///////////////////////////////////////////////////////////////
  ////�������� �� ������������������. ����������� ������� ����.
  ///////////////////////////////////////////////////////////////
  
    //���������� ������ ����� �� ���������.  
    int router_number = VehicleRouterNumberForNavigation(pBasePort, PNB_num, cargo_type, pVehicle);
    
    //������������������ �����:
    //���������� ����� ���� cargo_type ������� ����� ����� ��������� �� ���������.
    int cargo_for_navigation = (pVehicle->GetLoad())* router_number;
    
    //���������� ����� ����������� ��� ���������� �������������� �� �����.
    //double dVehicleNum = ((double)pBasePort->GetPNBRequiredCargo(PNB_num,cargo_type))/cargo_for_navigation;
    double dVehicleNum = ((double)(pBasePort->GetPNBRequiredCargo(PNB_num,cargo_type) - cargo_done))/cargo_for_navigation;

	/*
    FILE *stream;   //for debug
    stream = fopen( "E:\\Users\\mv\\Log\\Economic_Validate.txt", "w" ); // for debug
    fprintf (stream,"dVehicleNum: %f\n",dVehicleNum);
    fclose(stream);
    */
    if (dVehicleNum < 0.01)
        return 0;

	if (!(pVehicle->GetCargoCompatibility(cargo_type))){
		return 0;
	}

	// �������� ���������� ������������ �����
	int nVechicleExist = pVehicle->IsExist();
	int nVechicleExistDesign = pVehicle->IsExistDesign();
	if ((nVechicleExist == 1) && (nVechicleExistDesign == 0)){
		int nVehicleExistNum = pVehicle->GetExistNumRemain();
		if (dVehicleNum > nVehicleExistNum)
			return 0;
	}

    return 1;
}
// end:
///////////// for existing ships we need case when crgo was partioal transfered for specific port.



int CFleetTechEconomicEng::GetExistVehicleBuildCoast(CShip* pShip, ShipMainDetails* project)
{
	int coast;

	if (!pShip->IsExist())
		return -1;

	if (pShip->IsExistDesign() == 1)
		coast = pShip->GetExistDesignPrice();
	else if ((pShip->IsExistOperation() == 1) && (pShip->IsExistBuild() == 1))
		coast = ((pShip->GetExistNum() * pShip->GetExistPrice()) + (pShip->GetExistBuildNum() * pShip->GetExistBuildPrice()))/(pShip->GetExistNum() + pShip->GetExistBuildNum());
	else if ((pShip->IsExistOperation() == 1) && (pShip->IsExistBuild() == 0))
		coast = pShip->GetExistPrice();
	else if	((pShip->IsExistOperation() == 0) && (pShip->IsExistBuild() == 1))
		coast = pShip->GetExistBuildPrice();		

	
	return coast;
}


int CFleetTechEconomicEng::GetExistACVBuildCoast(CShip* pACV, ACVMainDetails* project)
{
	int coast;

	if (!pACV->IsExist())
		return -1;

	if (pACV->IsExistDesign() == 1)
		coast = pACV->GetExistDesignPrice();
	else if ((pACV->IsExistOperation() == 1) && (pACV->IsExistBuild() == 1))
		coast = ((pACV->GetExistNum() * pACV->GetExistPrice()) + (pACV->GetExistBuildNum() * pACV->GetExistBuildPrice()))/(pACV->GetExistNum() + pACV->GetExistBuildNum());
	else if ((pACV->IsExistOperation() == 1) && (pACV->IsExistBuild() == 0))
		coast = pACV->GetExistPrice();
	else if	((pACV->IsExistOperation() == 0) && (pACV->IsExistBuild() == 1))
		coast = pACV->GetExistBuildPrice();		

	
	return coast;
}

int CFleetTechEconomicEng::VehicleIncomeOnRouterPForYear(CRouterP *pRouter)
{
    int Income, OverallIncomeYear, BuildCoastYearPay, OperationalExpensesYear;
	int cargoInPlan, cargoOnRouter;
	CVehicle *pVehicle;

	///if (pRouter == 0)
	///	return 0;
	

	pVehicle = pRouter->GetVehicle();
	BuildCoastYearPay = VehicleBuildCoastYearPay(pVehicle);

	//����� {begin}: (for RouterP)
	int IncomeOnStep, Distance,DistanceOverall, tariff, load,
		cargo_type, cargo_type_prev,
		PNB_num, PNB_num_prev, PNB_id, PNB_id_prev;

	CPort *pBasePort = pRouter->GetBasePort();
	load = pVehicle->GetLoad();
	CPortPoint* pPPoint = pRouter->GetPPoitsListHead();
	PNB_num = pPPoint->GetPNBnum();
	PNB_id = pBasePort->GetPNB(PNB_num).id;
	Distance = pBasePort->GetDistancePortPNB(PNB_id);
	cargo_type = pPPoint->GetCargoType();
	tariff = GetVehicleTariff(pVehicle,cargo_type);
    //cargoInPlan = pRouter->GetCargoLoadInPlanForPNB(PNB_id,cargo_type);
	cargoOnRouter = 0;
	cargoInPlan = pPPoint->GetCargoLoadInShipPlan();
	cargoOnRouter += cargoInPlan;
    //cargoInPlan = pRouter->GetCargoLoadInPlanForPNB(PNB_num,cargo_type);
	Income = (tariff * cargoInPlan * Distance);		//��� ������� ������ ��������

	//�������� ���������� ����� �� ����� �� ��������:
	int PNB_IDnum, cargo_num; 
	cargoInPlan = 0;
	for (PNB_IDnum = 0; PNB_IDnum < MAXPNBNUMBER; PNB_IDnum++)
		for (cargo_num = 0; cargo_num <= 4; cargo_num++)
			cargoInPlan += pRouter->GetCargoLoadInPlanForPNB(PNB_IDnum,cargo_num);


	pPPoint = pPPoint->GetNextPPoint();
	DistanceOverall = Distance;

	while(pPPoint){
		PNB_num_prev = PNB_num;
		PNB_id_prev = PNB_id;
		cargo_type_prev = cargo_type;
		PNB_num = pPPoint->GetPNBnum();
		PNB_id = pBasePort->GetPNB(PNB_num).id;
		cargo_type = pPPoint->GetCargoType();
		cargoInPlan = pPPoint->GetCargoLoadInShipPlan(); //14.05.05
		cargoOnRouter += cargoInPlan;
		//cargoInPlan = pRouter->GetCargoLoadInPlanForPNB(PNB_id,cargo_type); //01.05.05
		//cargoInPlan = pRouter->GetCargoLoadInPlanForPNB(PNB_num,cargo_type); //14.05.05
		if (PNB_num == -1){
			Distance = pBasePort->GetDistancePortPNB(PNB_id_prev);
			cargoInPlan = 0; // �������� ����; ���������� ...TBD
		}
		else if ((PNB_id_prev == PNB_id)&& (cargo_type_prev == cargo_type )){  
					// ��� �� �����. ����� ������������ �� ������ � ������� ����
			Distance = pBasePort->GetDistancePortPNB(PNB_id);
		}
		else{
			Distance = pBasePort->GetDistancePNBPNB(PNB_id_prev,PNB_id);
		}
		tariff = GetVehicleTariff(pVehicle,cargo_type);
		//IncomeOnStep = (tariff * load * Distance);
		IncomeOnStep = (tariff * cargoInPlan * Distance);
		Income += IncomeOnStep;

		pPPoint = pPPoint->GetNextPPoint();
		DistanceOverall += Distance;
	}
	//����� {end}: (for RouterP)

   	int VehicleTripForYear = DistanceOverall;
    OperationalExpensesYear = VehicleOperationalExpensesYear(pVehicle, pRouter->GetBasePort()->GetPortRegion(), VehicleTripForYear);

    OverallIncomeYear = Income -(BuildCoastYearPay + OperationalExpensesYear);
    //OverallIncomeYear = -(BuildCoastYearPay + OperationalExpensesYear);
	
	return OverallIncomeYear;
}

int CFleetTechEconomicEng::GetVehicleTariff(CVehicle *pVehicle, int cargo_type)
{
	int TariffProtoScale, tariff, proto_load, vehicle_load, res;
	float KScale;

	switch(cargo_type){
    case 1:	//coal
         TariffProtoScale = pVehicle->GetTariffForProtoScaleCoal();
         break;
    case 2:	//general
         TariffProtoScale = pVehicle->GetTariffForProtoScaleGenVF();
         break;
    case 3:	//oil
         TariffProtoScale = pVehicle->GetTariffForProtoScaleOil();
         break;
    case 4:	//vegetables_fruits
         TariffProtoScale = pVehicle->GetTariffForProtoScaleGenVF();
         break;
    }
	
	if (TariffProtoScale == 0)
		return 0;

	return TariffProtoScale; //stab tmp

	CString strProtoName = pVehicle->GetPrototypeName();
	CString strRegisterClass = pVehicle->GetRegisterClass();
	vehicle_load = pVehicle->GetLoad();

	if (strRegisterClass.Find("*ACV*") != -1){ //ACV
		ACVPrototypeMainDetails* acv_prototype_tmp = new ACVPrototypeMainDetails;
		acv_prototype_tmp->name = strProtoName;
		res = GetACVPrototypeMainDetails(acv_prototype_tmp);
		proto_load = acv_prototype_tmp->Dgr;
		KScale = (float)(-0.25*proto_load/vehicle_load + 1.25); 
	}
	else{	//Ship
		ShipPrototypeMainDetails* ship_prototype_tmp = new ShipPrototypeMainDetails;
		ship_prototype_tmp->name = strProtoName;
		res = GetShipPrototypeMainDetails(ship_prototype_tmp);
		proto_load = ship_prototype_tmp->Pgr;
		KScale = (float)(-0.25*proto_load/vehicle_load + 1.25); 
	}


	tariff = (int)(TariffProtoScale * KScale);
	
	return tariff;
}

long CFleetTechEconomicEng::GetInvestmentYearLimit()
{
	return m_InvestmentYearLimit;
}

int CFleetTechEconomicEng::SetInvestmentYearLimit(long InvestmentYearLimit)
{
	m_InvestmentYearLimit = InvestmentYearLimit;

	return 1;
}

int CFleetTechEconomicEng::VehicleInvestCapabilityValidate(CVehicle *pVehicleIn, CVehicle *pVehicleOut)
{
	
	if (m_InvestmentYearLimit == 0) // ���������������� �� ����������
		return 1;

	return 1;
}

long CFleetTechEconomicEng::GetInvestmentYear()
{
	return m_InvestmentYear;
}

int CFleetTechEconomicEng::SetInvestmentYear(long InvestmentYear)
{
	m_InvestmentYear = InvestmentYear;
	return 1;
}

long CFleetTechEconomicEng::InvestmentYearCompute(CRouter *pRouter)
{
    int Investment = 0;
	int VehicleNum, BuildCoastYear;
	CVehicle *pVehicle;

    if (pRouter == NULL){
		MessageBox(NULL,"������ ��������� - ����","InvestmentYearComputeForRouterS",MB_OK);
		m_pOutView->TextOut("������ ��������� - ���� (CFleetTechEconomicEng::CFleetTechEconomicEng())");
        return 0;        
    }
    else{
        while(pRouter){
			VehicleNum = pRouter->GetVehicleNumber();
			pVehicle =	pRouter->GetVehicle();
			BuildCoastYear = CFleetTechEconomicEng::Instance()->VehicleBuildCoastYearPay(pVehicle);
			Investment += (long)(VehicleNum*BuildCoastYear);
            pRouter = pRouter->GetNextRouter();  
        }
    }
	
	return Investment;
}

long CFleetTechEconomicEng::InvestmentYearCompute(CRouterL *pRouter)
{
    int Investment = 0;
	int VehicleNum, BuildCoastYear;
	CVehicle *pVehicle;

    if (pRouter == NULL){
		MessageBox(NULL,"������ ��������� - ����","InvestmentYearComputeForRouterS",MB_OK);
		m_pOutView->TextOut("������ ��������� - ���� (CFleetTechEconomicEng::CFleetTechEconomicEng())");
        return 0;        
    }
    else{
        while(pRouter){
			VehicleNum = pRouter->GetVehicleNumber();
			pVehicle =	pRouter->GetVehicle();
			BuildCoastYear = CFleetTechEconomicEng::Instance()->VehicleBuildCoastYearPay(pVehicle);
			Investment += (long)(VehicleNum*BuildCoastYear);
            pRouter = pRouter->GetNextRouter();  
        }
    }
	
	return Investment;
}

int CFleetTechEconomicEng::GetErrorStatus()
{
	return m_nErrorStatus;
}

float CFleetTechEconomicEng::GetCargoSpecificCapacity(int Coal, int General, int Oil, int VegFruits)
{
	double cargoSpecificCapacity;
	 
	//all - Stub;

	cargoSpecificCapacity = 1.6;

	if (Coal)
		cargoSpecificCapacity = 1.2;
	else if ((Oil) && (!General))
		cargoSpecificCapacity = 1.4;
	else if ((Oil) && (General))
		cargoSpecificCapacity = 1.5;


	return (float)cargoSpecificCapacity;
}

float CFleetTechEconomicEng::GetCargoSpecificCapacity(CVehicle *pVehicle)
{
	double cargoSpecificCapacity;
	int Coal, Oil, General;
	 
	//all - Stub;

	cargoSpecificCapacity = 1.6;

	Coal = pVehicle->GetCargoCompatibility(1);
	Oil = pVehicle->GetCargoCompatibility(3);
	General = pVehicle->GetCargoCompatibility(2);

	if (Coal)
		cargoSpecificCapacity = 1.2;
	else if ((Oil) && (!General))
		cargoSpecificCapacity = 1.4;
	else if ((Oil) && (General))
		cargoSpecificCapacity = 1.5;


	return (float)cargoSpecificCapacity;
}
