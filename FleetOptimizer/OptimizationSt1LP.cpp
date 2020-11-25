// OptimizationSt1LP.cpp: implementation of the COptimizationSt1LP class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "FleetOptimizer.h"
#include "OptimizationSt1LP.h"
#include "InitialBasePlan.h"
#include "OutputView.h"
#include "MarineTechEconomicEng.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

COptimizationSt1LP* COptimizationSt1LP::_instance = 0;

COptimizationSt1LP* COptimizationSt1LP::Instance(){
    if (_instance == 0){
        _instance = new COptimizationSt1LP;
    }
    return _instance;
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

COptimizationSt1LP::COptimizationSt1LP()
{
    RoutersList = 0;
	//MessageBox(NULL,"COptimizationSt1LP()","COptimizationSt1LP",MB_OK);

	POSITION pos = ((CFleetOptimizerApp*)AfxGetApp())->GetCurrFleetOptimizerDoc()->GetFirstViewPosition();
	m_pOutView = (COutputView*) ((CFleetOptimizerApp*)AfxGetApp())->GetCurrFleetOptimizerDoc()->GetNextView(pos);
	m_pOutView = (COutputView*) ((CFleetOptimizerApp*)AfxGetApp())->GetCurrFleetOptimizerDoc()->GetNextView(pos);
	m_pOutView->TextOut("COptimizationSt1LP()");

	m_LandStructExpenciesYear = 0;
	m_bStepDone = 0;

}

COptimizationSt1LP::~COptimizationSt1LP()
{

}

int COptimizationSt1LP::DualMarksCompute()
{
	return 1;
}

int COptimizationSt1LP::GoalFunctionCompute()
{
    int Function = 0;
	CRouter *pRouter = RoutersList;

    if (pRouter == NULL){
		MessageBox(NULL,"������ ��������� - ����","1-���� �����������:GoalFunctionCompute()",MB_OK);
		m_pOutView->TextOut("������ ��������� - ���� (1-���� �����������:GoalFunctionCompute())");
        return 0;        
    }
    else{
        while(pRouter){
			//Function += (int)(pRouter->GetVehicleIncome() * pRouter->GetDVehicleNumber());
			Function += (int)pRouter->GetVehicleIncome();
            pRouter = pRouter->next;  
        }
    }
	
	//Land struct:
	BOOL bIncludeLandStructCalc = FALSE;
	bIncludeLandStructCalc = ((CFleetOptimizerApp*)AfxGetApp())->GetProfileInt("FleetOptimizer","IncludeLandStructCalc",0);	
	if (bIncludeLandStructCalc){
		m_LandStructExpenciesYear = CMarineTechEconomicEng::Instance()->GetLandStructExpenciesYearAllSt1();
		Function -= m_LandStructExpenciesYear;
	} 
	// end of Land struct
	
	GoalFunction = Function;


	return Function;
}

int COptimizationSt1LP::GoalFunctionCorrect()
{
	return 1;
}

int COptimizationSt1LP::VectorsMarksCompute(CRouter *&RouterImprov, CVehicle *&VehicleBest, int &VehicleBestNum, double &dVehicleNumBest, double &VectorsMarkBest, int &VehicleBestIncome)
{
	double DualMark;		// Y - ������������ ������
	double VectorsMark; //VectorsMarkBest;	// Z - ������ ��������
	int vehicleIncomeBase, vehicleIncome, /*vehicleIncomeAll,*/ vehicleNumber;
	//int vehicleCargoForNavigation, VehicleCargoForNavigationBest; // VehicleBestNum;
	CPort *pBasePort;		//������� ����� �������� �������� �����
	CVehicle *pVehicleBP;	//����� �������� �������� �����
	CVehicle *pVehicleBest;
	int PNB_num, cargo_type;
    //char VehicleBPTypeName[20], VehicleTypeName[20];
    int flagRouterFound = 0;
	CRouter *pRouterImprov;
	int VehicleNum = 0;
	double	dVehicleNum; // dVehicleNumBest;
    int BuildCoastYearPay, OperationalExpensesYear;
	int cargo_for_navigation;
	double vehicleCapability;
	int nVehicleExist, nVehicleExistForce, flagGetVehicle, AdvantPrice, 
		nVehicleBestExist, nVehicleBestExistForce, VehicleBestAdvantPrice;
	CString VehicleBPTypeName,VehicleTypeName;

	nVehicleBestExistForce = 0;
	
	CRouter *pRouter = CBasePlanSt1LP::Instance()->GetRoutersListHead();
	
	//FILE *stream;
    //stream = fopen( "E:\\Users\\mv\\Log\\OptimizationSl1LP.txt", "a" );

    if (pRouter == NULL){
        //fprintf (stream,"������ ��������� ����\n");
		//fclose(stream);
        return 0;
    }
	VectorsMarkBest = -1000000;
	nVehicleBestExist = 0;

	long lInvestmentYearLimit = CFleetTechEconomicEng::Instance()->GetInvestmentYearLimit();
	long lInvestmentYear = CFleetTechEconomicEng::Instance()->GetInvestmentYear();
	int BuildCoastYearVehicleRout=0;//BuildCoastYearVehicleIn;
	int InvestDelta = 0;
	int InvestDeltaBest = 0;

	//���������������� � ��������� ����������
	BOOL bIncludeLandStructCalc = FALSE;
	int LandInvestDelta = 0;
	int LandInvestDeltaBest = 0;
	bIncludeLandStructCalc = ((CFleetOptimizerApp*)AfxGetApp())->GetProfileInt("FleetOptimizer","IncludeLandStructCalc",0);	

	

    while(pRouter){
		vehicleIncomeBase = pRouter->GetVehicleIncome();
		vehicleNumber = pRouter->GetVehicleNumber();
		//vehicleCargoForNavigation = pRouter->GetVehicleCargoForNavigation();
		pVehicleBP =	pRouter->GetVehicle();
		VehicleBPTypeName = pVehicleBP->GetTypeName();
		pBasePort =		pRouter->GetBasePort();
		PNB_num =		pRouter->GetLine()->GetPNBnum();
		cargo_type =	pRouter->GetCargoType();

		BuildCoastYearVehicleRout = CFleetTechEconomicEng::Instance()->VehicleBuildCoastYearPay(pVehicleBP);

		//DualMark = (double)(-vehicleIncome)* vehicleNumber / vehicleCargoForNavigation;
		DualMark = (double)(-vehicleIncomeBase)/ (pBasePort->GetPNBRequiredCargo(PNB_num,cargo_type));

		
		//fprintf (stream,"\n\n***********************************************\n");
        //fprintf (stream,"���: %s\n",pRouter->GetBasePort()->GetPNB(PNB_num).name);
        //fprintf (stream,"����: %d\n", cargo_type);
		//fprintf (stream,"�����: %d\t ���������� �����: %d\t ��.������: %f\n", vehicleIncomeBase,vehicleNumber,DualMark);
        //fprintf (stream,"�����: %s\n",pRouter->GetVehicle()->GetTypeName());
        //fprintf (stream,"���������������� �����: %d\n\n",pRouter->GetVehicle()->GetLoad());
        ///old/fprintf (stream,"���������� ����� �������: %f\n", pRouter->dVehicleNum);
		

        CVehicle *pVehicle = CVehiclesList::Instance()->GetVehicleListHead();
        while(pVehicle){  
			AdvantPrice = 0;
            //�������� ������������� ����� � �����:
            if(CFleetTechEconomicEng::Instance()->VehicleLineValidation(pBasePort,PNB_num,cargo_type,pVehicle)==0){
                pVehicle = pVehicle->GetNextVehicle();               
                continue;
            }
            
			//�������� �� ��������� �� ����� � ������ �������� ��������
			VehicleTypeName =  pVehicle->GetTypeName();
			if ((!(strcmp(VehicleBPTypeName,VehicleTypeName))) && (pVehicleBP->GetLoad() == pVehicle->GetLoad())){
				pVehicle = pVehicle->GetNextVehicle();
				continue;
			}

            //����������� ������� ������ ����� �� ����� �� ���������.
            vehicleIncome = CFleetTechEconomicEng::Instance()->VehicleIncomeOnLineForYear(pBasePort,PNB_num,cargo_type,pVehicle,VehicleNum,dVehicleNum, BuildCoastYearPay, OperationalExpensesYear, cargo_for_navigation);
			if ((vehicleIncome >0) && (VehicleNum >1))	//test
				VehicleNum -= 1;						//test

			
			//�������� ����������� �������� (������) ����� 
			//� ����� ������ ����������������:
			if(lInvestmentYearLimit > 0){// lInvestmentYearLimit ���� == 0, �� ��� �����������
				//Land:
				if (bIncludeLandStructCalc){
					LandInvestDelta = CMarineTechEconomicEng::Instance()->GetLandStructExpenciesYearDeltaSt1(pBasePort, PNB_num, pVehicle, VehicleNum, pVehicleBP, vehicleNumber, 1);
				}
				//end of Land

				//���� ��������� ����� ������������ ������ ���������
				//����� �� ��������, �� ��������� ��������
				InvestDelta = (BuildCoastYearPay*VehicleNum - BuildCoastYearVehicleRout*vehicleNumber);
				if (InvestDelta > 0){
					// �������� ����������� ����������������:
					if((lInvestmentYear+InvestDelta+LandInvestDelta) > lInvestmentYearLimit){
						pVehicle = pVehicle->GetNextVehicle();               
						continue;
					}
				}
			}
			//			
			
			
			
			
			//vehicleCapability = ((double)pBasePort->GetPNBRequiredCargo(PNB_num,cargo_type))/VehicleNum;
			vehicleCapability = ((double)pBasePort->GetPNBRequiredCargo(PNB_num,cargo_type)); // ��� ���� ����� �����

			// Z = A*Y-C :
			VectorsMark = vehicleCapability * DualMark + vehicleIncome;
/*
            fprintf (stream,"�����: %s\n",pVehicle->GetTypeName());
            fprintf (stream,"���������������� �����: %d\n",pVehicle->GetLoad());
            fprintf (stream,"���������� �����: %d\n", VehicleNum);
            fprintf (stream,"���������� ����� �������: %f\n", dVehicleNum);
			fprintf (stream,"Z mark: %f\n\n\n", VectorsMark);
*/

			flagGetVehicle = 0;
			nVehicleExistForce = 0;
			nVehicleExist = pVehicle->IsExist();
			nVehicleExistForce = pVehicle->IsExistForce();
			if (nVehicleExist)
				AdvantPrice = pVehicle->GetExistGenAdvantPrice();
			
			if (VectorsMark > 0.0000001){ // 19 April 2002
			//if (VectorsMark >0){
				//if (flagRouterFound == 0) {
				//	flagRouterFound = 1;
				//}
/*            fprintf (stream,"�����: %s\n",pVehicle->GetTypeName());
            fprintf (stream,"���������������� �����: %d\n",pVehicle->GetLoad());
            fprintf (stream,"���������� �����: %d\n", VehicleNum);
            fprintf (stream,"���������� ����� �������: %f\n", dVehicleNum);
			fprintf (stream,"Z mark: %f\n\n\n", VectorsMark);
*/

				if (VectorsMark > VectorsMarkBest){
					if (nVehicleExist){//Ship exists
						if (!nVehicleBestExistForce || nVehicleExistForce)
							flagGetVehicle = 1;
					}
					else {//Ship doesn't exist
						if (nVehicleBestExist == 0){ //previouse(best) vehicle - doesn't exist
							flagGetVehicle = 1;					
						} 
						else { //previouse(best) vehicle - exists
							//If Current Vehicle MuchBetter ?..:
							if(!nVehicleBestExistForce || (VehicleBestAdvantPrice != -1)){
								if (VectorsMark > (VehicleBestAdvantPrice + VectorsMarkBest))
									flagGetVehicle = 1;
							}
						}//end: previouse(best) vehicle - exists
					}//end: Ship doesn't exist
				}//end: if (VectorsMark > VectorsMarkBest)
				else { // VectorsMark <= VectorsMarkBest
					if (nVehicleExist && (!nVehicleBestExistForce)) { //Ship Exist; � ���� ���� ������������ ...
						if (nVehicleExistForce || (AdvantPrice == -1) // ������������ ����� ����� ����������� ������������
							|| ((VectorsMark + VehicleBestAdvantPrice) > VectorsMarkBest)){
									flagGetVehicle = 1;  
						}
					}
				}
				
				//if ((flagGetVehicle == 1) && (pRouter->GetVehicle()->IsExistForce() != 1)){
				if (flagGetVehicle == 1){
					if ((pRouter->GetVehicle()->IsExistForce() != 1) || (pVehicle->IsExistForce())){
						// ���� ����� ����������� �������� �� ����� �������� ������������� ��������� � ����
						// ��� �������� ����� ����� ���� ������� 
						// (�.�. ��� ������������� ����� ����� �������� ������������� ��������� � ����,
						// ����� ����������� ����. ��������:
						flagRouterFound = 1;
						if (nVehicleExist && (pVehicle->IsExistDesign() == 0)){
							pVehicle->UpdateExistNumRemainDecrem(VehicleNum);
						}
						if (nVehicleBestExist && (pVehicleBest->IsExistDesign() == 0)) { //��������� ����� - ������������
							pVehicleBest->UpdateExistNumRemainIncrem(VehicleNum);
						}
						VectorsMarkBest = VectorsMark;
						VehicleBestIncome = vehicleIncome;
						pVehicleBest = pVehicle;
						VehicleBestNum = VehicleNum;
						dVehicleNumBest = dVehicleNum;
						//VehicleCargoForNavigationBest = cargo_for_navigation;
						pRouterImprov = pRouter;
						nVehicleBestExist = nVehicleExist;
						nVehicleBestExistForce = nVehicleExistForce;
						VehicleBestAdvantPrice = AdvantPrice;
						LandInvestDeltaBest = LandInvestDelta;
						InvestDeltaBest = InvestDelta;

						//fprintf (stream,"����� ���������� �� �������� � �����: %s\n",pVehicleBest->GetTypeName());
						//fprintf (stream,"���������������� �����: %d\n",pVehicleBest->GetLoad());
						//fprintf (stream,"���������� �����: %d\n", VehicleBestNum);
						//fprintf (stream,"���������� ����� �������: %f\n", dVehicleNumBest);
						//fprintf (stream,"Z mark: %f\n\n\n", VectorsMarkBest);
					}

				}

			}
			pVehicle = pVehicle->GetNextVehicle();        
		}
		pRouter = pRouter->GetNextRouter();

    }

	if (flagRouterFound){
		//fprintf (stream,"\n\n*******************************************************\n");
		//fprintf (stream,"����� �������� � �����: %s\n",pVehicleBest->GetTypeName());
		//fprintf (stream,"���������������� �����: %d\n",pVehicleBest->GetLoad());
		//fprintf (stream,"���������� �����: %d\n", VehicleBestNum);
		//fprintf (stream,"���������� ����� �������: %f\n", dVehicleNumBest);
		//fprintf (stream,"Z mark: %f\n\n\n", VectorsMarkBest);
		//int num_tmp = pRouterImprov->GetLine()->GetPNBnum();
		//int cargo_type_tmp =	pRouterImprov->GetCargoType();
		//fprintf (stream,"���������� �������:");
		//fprintf (stream,"   ���: %s", pRouterImprov->GetBasePort()->GetPNB(num_tmp).name);
		//fprintf (stream,"   ����: %d\n", cargo_type_tmp);
		//fprintf (stream,"   �����: %s\n", pRouterImprov->GetVehicle()->GetTypeName() /*VehicleBPTypeName*/);
		//fprintf (stream,"       ����������������: %d\n", pRouterImprov->GetVehicle()->GetLoad()  /*pVehicleBP->GetLoad()*/);
		//fprintf (stream,"       ����������: %d\n", pRouterImprov->GetVehicleNumber());
	
		while((RouterImprov->GetNextRouter() != NULL) && (RouterImprov->GetVehicle()->IsExistForce())) {	//04 Mar.
			RouterImprov = RouterImprov->GetNextRouter();
		}

		RouterImprov = pRouterImprov;
		VehicleBest = pVehicleBest;

		if(lInvestmentYearLimit > 0){
			lInvestmentYear += InvestDeltaBest;
			CFleetTechEconomicEng::Instance()->SetInvestmentYear(lInvestmentYear);
		}

		//MessageBox(NULL,"Z > 0 found","VectorsMarksCompute",MB_OK);
		m_pOutView->TextOut("1st stage of Optimization - Route with better mark found, continue ...");		
		//fclose(stream);
		return 1;
	} else {
		//MessageBox(NULL,"Z > 0 NOT FOUND","VectorsMarksCompute",MB_OK);
		//fclose(stream);
		return 10;
	}
}

int COptimizationSt1LP::SimplexMethod()
{
	
	int VehicleNumBest, VehicleBestIncome, result;
	double dVehicleNumBest, VectorsMarkBest;
	//int Function;			//	�������� ������� ������� �������� ����
	//int	FunctionCorrect;	//	������������������ �������� ������� ������� �������� ����,
							//	� ������ ���������� ����� � �����
    
	//MessageBox(NULL,"COptimizationSt1LP::SimplexMethod()","SimplexMethod",MB_OK);
	m_pOutView->TextOut("1st stage of Optimization: SimplexMethod()");

	result = CBasePlanSt1LP::Instance()->CreateBasePlanInit();
	if (result == 0){
		MessageBox(NULL,"Routes list is empty.\nMissing Initial Base Plan. ", " 1st stage of Optimization: SimplexMethod()",MB_ICONEXCLAMATION);
		m_pOutView->TextOut("Routes list is empty. Missing Initial Base Plan. (1st stage of Optimization: SimplexMethod())");
        return 0;        
    }


	CRouter *pRoutetTmp = CBasePlanSt1LP::Instance()->GetRoutersListHead();
	CRouter *&pRouterImprov = *&pRoutetTmp;
	RoutersList = CBasePlanSt1LP::Instance()->GetRoutersListHead();

    CVehicle *pVehicleTmp = CVehiclesList::Instance()->GetVehicleListHead();
	CVehicle *&pVehicleBest = *&pVehicleTmp;
	 


	FILE *stream;
	//stream = fopen( "E:\\Users\\mv\\Log\\OptimizationProgressStep1.txt", "w" );
	CString LogFile = ((CFleetOptimizerApp*)AfxGetApp())->GetProfileString("FleetOptimizer","LogDir");
	LogFile += "OptimizationProgressStep1.txt";
    stream = fopen(LogFile, "w" );

	
	//Function = GoalFunctionCompute();
	//FunctionCorrect = GoalFunctionCorrect();
	if (GoalFunctionCompute() == 0){
		m_pOutView->TextOut("Error in calculating the objective function [1]. Exiting ... (1st stage of Optimization: SimplexMethod)");
		MessageBox(NULL,"Error in calculating the objective function [1]. Exiting ... ","1st stage of Optimization: SimplexMethod",MB_OK);
		fclose(stream);
		return 0;	
	}
	fprintf (stream,"Objective function: %d\n\n\n", this->GoalFunction);

	long lInvestmentYearLimit = CFleetTechEconomicEng::Instance()->GetInvestmentYearLimit();
	if (lInvestmentYearLimit > 0){
		long Investment = CFleetTechEconomicEng::Instance()->InvestmentYearCompute(RoutersList);
		CFleetTechEconomicEng::Instance()->SetInvestmentYear(Investment);
	}
	BOOL bIncludeLandStructCalc = FALSE;
	bIncludeLandStructCalc = ((CFleetOptimizerApp*)AfxGetApp())->GetProfileInt("FleetOptimizer","IncludeLandStructCalc",0);	

	//DualMarksCompute();

/*********************************************/
	//debug begin:
	int count = 0;
	char strNum[5];
	CString strDebugMessage;
	//debug end.

	int optimumFlag = 0;
	while(optimumFlag != 1)
	{
		if (count == 29){
			int test1 = 1;
		}
		result = VectorsMarksCompute(pRouterImprov, pVehicleBest, VehicleNumBest, dVehicleNumBest, VectorsMarkBest, VehicleBestIncome);
		if (result == 10) {
			optimumFlag = 1;
			//debug begin
			_itoa(count, strNum, 10);
			strDebugMessage = "Best route found in ";
			strDebugMessage += strNum;
			strDebugMessage += " iterations";
			//MessageBox(NULL, strDebugMessage,"VectorsMarksCompute",MB_OK);
			strDebugMessage += " (COptimizationSt1LP::SimplexMethod)";
			m_pOutView->TextOut(strDebugMessage);
			//debug end
			break;
		}
		count++;

		fprintf (stream,"\n\n*******************************************************\n");
		fprintf (stream,"����� �������� � �����: %s\n",pVehicleBest->GetTypeName());
		fprintf (stream,"���������������� �����: %d\n",pVehicleBest->GetLoad());
		fprintf (stream,"���������� �����: %d\n", VehicleNumBest);
		fprintf (stream,"���������� ����� �������: %f\n", dVehicleNumBest);
		fprintf (stream,"Z ������ ��������: %f\n", VectorsMarkBest);


	/*********************************************\
		���������� ������ ����������� ��������� ���������
		���	������� (1-��) ����� �����������, � ������������ ����������,
		������ �������� �������� � ������ ����� �� ��������. 
		����� ��������� - ������������ � ���������.
	/*********************************************/
						
		pRouterImprov->SetVehicle(pVehicleBest);
		pRouterImprov->vehicleNumber = VehicleNumBest;
		pRouterImprov->dVehicleNumber = dVehicleNumBest;
		pRouterImprov->vehicleIncome = VehicleBestIncome;

		if ((lInvestmentYearLimit > 0) && (bIncludeLandStructCalc))
			CMarineTechEconomicEng::Instance()->UpdateLandStructVirtualDataSt1();

		//GoalFunctionCompute();
		if (GoalFunctionCompute() == 0){
			m_pOutView->TextOut("Error in calculating the objective function [2]. Exiting... (1st stage of Optimization: SimplexMethod )");
			MessageBox(NULL,"Error in calculating the objective function [2]. Exiting... ","1st stage of Optimization: SimplexMethod",MB_OK);
			fclose(stream);
			return 0;	
		}
		fprintf (stream,"Objective function: %d\n\n\n", this->GoalFunction);

	}
		



/*********************************************/
	this->print();
	//CBasePlanSt1LP::Instance()->print(); //identical results in this point for bothe function (this and previouse).
	fclose(stream);
	//MessageBox(NULL,"���������","1-���� �����������: SimplexMethod()",MB_OK);
	m_pOutView->TextOut("Completed 1st stage of Optimization: SimplexMethod, Routes - shuttle");
	m_bStepDone = 1;
	return 1;
}





///////////////////////////
void COptimizationSt1LP::print()
{   
    CRouter *pRouter = RoutersList;
    
	CString sCargoType = "";
    FILE *stream;
    //stream = fopen( "E:\\Users\\mv\\Log\\Step1Plan.txt", "w" );
	CString LogFile = ((CFleetOptimizerApp*)AfxGetApp())->GetProfileString("FleetOptimizer","LogFileOptimizStep1Plan");	
    stream = fopen(LogFile, "w" );

    if (pRouter == NULL){
        fprintf (stream,"Routes list is empty\n");        
    }
    else{
        while(pRouter){

            fprintf (stream,"������� ����: %s\n",pRouter->GetBasePort()->GetPortName());
            int PNBnum = pRouter->GetLine()->GetPNBnum();
            fprintf (stream,"���: %s\n",pRouter->GetBasePort()->GetPNB(PNBnum).name);
            int cargo_type = pRouter->GetCargoType();
			switch(cargo_type){
			 case 1:
				 sCargoType = "�����";
				 break;
			 case 2:
				 sCargoType = "�����������";
				 break;
			 case 3:
				 sCargoType = "���";
				 break;
			 case 4:
				 sCargoType = "�����-������";
				 break;
			}
            //fprintf (stream,"����: %d\n", cargo_type);
            fprintf (stream,"����: %s\n", sCargoType.GetString());
            fprintf (stream,"���������� ����� ����������� ���: %d �\n", pRouter->GetBasePort()->GetPNBRequiredCargo(PNBnum,cargo_type));
            fprintf (stream,"�����: %s\n",pRouter->GetVehicle()->GetTypeName());
            fprintf (stream,"���������������� �����: %d �\n",pRouter->GetVehicle()->GetLoad());
            //fprintf (stream,"������������������ �����: %d\n",pRouter->GetVehicleCargoForNavigation());
            fprintf (stream,"���������� �����: %f\n", pRouter->GetDVehicleNumber());
            fprintf (stream,"���������� ����� �����: %d\n", pRouter->GetVehicleNumber());
            //fprintf (stream,"����� ����� �� ��������: %d\n\n", ((pRouter->GetVehicleIncome())*(pRouter->GetVehicleNumber())));
            fprintf (stream,"����� ����� �� ��������: %d\n\n", pRouter->GetVehicleIncome());
      
            pRouter = pRouter->next;  
        }
    }
	fclose(stream);
}

CRouter * COptimizationSt1LP::GetRouterListPointer()
{
	return RoutersList;
}

int COptimizationSt1LP::GetLandStructExpenciesYear()
{
	return m_LandStructExpenciesYear;
}

int COptimizationSt1LP::IsStepDone()
{
	return m_bStepDone;
}
