// ShipFactory.cpp: implementation of the CShipFactory class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "FleetOptimizer.h"
#include "ShipFactory.h"
#include "Ship.h"
#include "VehiclesList.h"
#include "FleetTechEconomicEng.h"
#include <io.h>
#include "OutputView.h"

#define MMI_INT

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

CShipFactory* CShipFactory::_instance = 0;

CShipFactory* CShipFactory::Instance(){
    if (_instance == 0){
        _instance = new CShipFactory;
    }
    return _instance;
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CShipFactory::CShipFactory()
{
    //MessageBox(NULL,"CShipFactory()","CShipFactory",MB_OK);
	m_pScenarioFile = "default.fsc";

    //MessageBox(NULL,"CShipFactory()","CShipFactory",MB_OK);
	POSITION pos = ((CFleetOptimizerApp*)AfxGetApp())->GetCurrFleetOptimizerDoc()->GetFirstViewPosition();
	COutputView* pView = (COutputView*) ((CFleetOptimizerApp*)AfxGetApp())->GetCurrFleetOptimizerDoc()->GetNextView(pos);
	m_pOutView = (COutputView*) ((CFleetOptimizerApp*)AfxGetApp())->GetCurrFleetOptimizerDoc()->GetNextView(pos);
	//m_pOutView->TextOut("CShipFactory(). Создание объектов судов ...");
	m_pOutView->TextOut("CShipFactory()");

	m_sLogFNameError = ((CFleetOptimizerApp*)AfxGetApp())->GetLogFNameError();
	m_nErrorStatus = 0;

    //CreateShipsInstances();
	//m_pOutView->TextOut("     done");


}

CShipFactory::~CShipFactory()
{

}



void CShipFactory::CreateShipsInstances()
{
	POSITION pos = ((CFleetOptimizerApp*)AfxGetApp())->GetCurrFleetOptimizerDoc()->GetFirstViewPosition();
	COutputView* pView = (COutputView*) ((CFleetOptimizerApp*)AfxGetApp())->GetCurrFleetOptimizerDoc()->GetNextView(pos);
	m_pOutView = (COutputView*) ((CFleetOptimizerApp*)AfxGetApp())->GetCurrFleetOptimizerDoc()->GetNextView(pos);
	m_pOutView->TextOut("Ships objects creation by CShipFactory");
	
	m_pScenarioFile = ((CFleetOptimizerApp*)AfxGetApp())->m_pScenarioFile;
	GetInvestmentInfoFromScenario();
	CreateShipsInstancesFromScenario();

/*
    int count, load, coast;

    for (coast = 20000, load = 3000, count = 0; count < 150; count++){
        CShip* pShipA = new CShip("Pioner", "L1");
        //CShip* pShipA = new CShip("Pioner");
		//CShip* pShipA = new CShip;
		//sprintf(pShipA->TypeName, "Pioner");
		//sprintf(pShipA->RegisterClass, "L1");
        pShipA->Coast = coast;
        pShipA->Load = load;
        pShipA->Speed = 15;
		pShipA->CargoCompatibility[1]=1;
		pShipA->CargoCompatibility[2]=1;
		pShipA->CargoCompatibility[3]=1;
		pShipA->CargoCompatibility[4]=1;
        CVehiclesList::_instance->append(pShipA);
        load = load+50;
        coast += 100;
    }

	//

    CShip* pShipB = new CShip("Amguema", "ULA");
	//CShip* pShipB = new CShip; //
	//sprintf(pShipB->TypeName, "Amguema");//
	//sprintf(pShipB->RegisterClass, "ULA");//
    pShipB->Coast = 500000;
    pShipB->Load = 50000;
    pShipB->Speed = 15;
	pShipB->CargoCompatibility[1]=1;
	pShipB->CargoCompatibility[2]=1;
	pShipB->CargoCompatibility[3]=1;
	pShipB->CargoCompatibility[4]=1;


    CShip* pShipC = new CShip("LC15", "ULA");
	//CShip* pShipC = new CShip; //
	sprintf(pShipC->TypeName, "LC15");//
	//sprintf(pShipC->RegisterClass, "ULA");//
    pShipC->Coast = 750000;
    pShipC->Load = 70000;
    pShipC->Speed = 18;
	pShipC->CargoCompatibility[1]=1;
	pShipC->CargoCompatibility[2]=1;
	pShipC->CargoCompatibility[3]=1;
	pShipC->CargoCompatibility[4]=1;
	

      
    CVehiclesList::_instance->append(pShipB);
    CVehiclesList::_instance->append(pShipC);
    
    for (coast = 50000, load = 20; load < 300; load += 5, coast += 400){
        CShip* pShipD = new CShip("Hover", "ACV");
		//CShip* pShipD = new CShip; //
		//sprintf(pShipD->TypeName, "Hover");//
		//sprintf(pShipD->RegisterClass, "ACV");//
        pShipD->Coast = coast;
        pShipD->Load = load;
        pShipD->Speed = 28;
		pShipD->CargoCompatibility[1]=0;
		pShipD->CargoCompatibility[2]=1;
		pShipD->CargoCompatibility[3]=1;
		pShipD->CargoCompatibility[4]=1;

        CVehiclesList::_instance->append(pShipD);
    }

    for (coast = 10000, load = 400; load < 1000; load += 20, coast += 200){
        CShip* pShipE = new CShip("FiderVessel", "L1");
        pShipE->Coast = coast;
        pShipE->Load = load;
        pShipE->Speed = 15;
		pShipE->CargoCompatibility[1]=1;
		pShipE->CargoCompatibility[2]=1;
		pShipE->CargoCompatibility[3]=1;
		pShipE->CargoCompatibility[4]=1;

        CVehiclesList::_instance->append(pShipE);
    }
*/
    CVehiclesList::_instance->print();

}

void CShipFactory::CreateShipsInstancesFromScenario()
{
	CString	str, strNum, strShipTypeName, strRegisterClass, strPrototypeName, strh3;
	int	nDW, nL, nB, nT, nSpeed, nRange, nCargoCompatiblGeneral,
			nCargoCompatiblOil, nCargoCompatiblCoal, nCargoCompatiblVegFruits,
			nLoadMin, nLoadMax, nLoadDelta,
			nTariffGenVF, nTariffOil, nTariffCoal,
			nShipExistSetCheck, nShipExistForceCheck, nShipExistCheck, nShipExistBuildCheck, nShipExistDesignCheck,
			nShipExistNum, nShipExistAdvantPrice, nShipExistPrice, nShipExistBuildNum,
			nShipExistBuildAdvantPrice, nShipExistBuildPrice, nShipExistDesignAdvantPrice, nShipExistDesignPrice;

	float h3;
	
	int i, count, load;
	//int coast;
	int fl_proto = 1;
	int fl_acv_proto = 0;
	int res;
	char *stopstring;
	long lInvestmentYearLimit;
	ULONGLONG positionEnd;

	char strData[5];

	FILE *streamError;
    streamError = fopen(m_sLogFNameError, "a");
	
	CACVDesign::Instance()->m_bPrintIsOn = 1;


	ShipMainDetails* ship_tmp = new ShipMainDetails;
	ShipPrototypeMainDetails* ship_prototype_tmp = new ShipPrototypeMainDetails;

	ACVMainDetails* acv_tmp = new ACVMainDetails;
	ACVPrototypeMainDetails* acv_prototype_tmp = new ACVPrototypeMainDetails;

	if (_access(m_pScenarioFile,00) == -1)
	{
		AfxMessageBox("Ошибка: Файл сценария не существует! CShipFactory::CreateShipsInstancesFromScenario()");
		m_pOutView->TextOut("Ошибка: Файл сценария не существует! CShipFactory::CreateShipsInstancesFromScenario()");
		fprintf(streamError, "Ошибка: Файл сценария не существует! CShipFactory::CreateShipsInstancesFromScenario()");

		return;
	}
	
	CFile f( m_pScenarioFile, CFile::modeRead );
	f.SeekToEnd();
	positionEnd = f.GetPosition();
	if (positionEnd < 3){ //Scenario File - empty
		//AfxMessageBox("CreateShipsInstancesFromScenario(): Scenario File - empty");
		m_pOutView->TextOut("     Файл сценария - пуст! CShipFactory::CreateShipsInstancesFromScenario()");
		fprintf(streamError,"Файл сценария - пуст! CShipFactory::CreateShipsInstancesFromScenario()");
		return;		
	}
	f.SeekToBegin();
	CArchive ar(&f, CArchive::load);

	count = 0;
	i = 1;

	ar >> str;
	while(count < 10){
		ar >> str; //Load first field of record
		if (str == "INVEST DATA"){
			ar >> strNum;	
			lInvestmentYearLimit = (long)strtod(strNum,&stopstring);
			CFleetTechEconomicEng::Instance()->SetInvestmentYearLimit(lInvestmentYearLimit);
		}else if (str == "PORTS DATA"){
			break;
		}
		else{ //Load all other fields of record
			strShipTypeName = str;
			ar >> strRegisterClass;
			ar >> strPrototypeName;

			nDW = GetIntFromArchive(ar);
			nL = GetIntFromArchive(ar);
			nB = GetIntFromArchive(ar);
			nT = GetIntFromArchive(ar);
			nCargoCompatiblGeneral = GetIntFromArchive(ar);
			nCargoCompatiblOil = GetIntFromArchive(ar);
			nCargoCompatiblCoal = GetIntFromArchive(ar);
			nCargoCompatiblVegFruits = GetIntFromArchive(ar);
			nLoadMin = GetIntFromArchive(ar);
			nLoadMax = GetIntFromArchive(ar);
			nLoadDelta = GetIntFromArchive(ar);
			nTariffGenVF = GetIntFromArchive(ar);
			nTariffOil = GetIntFromArchive(ar);
			nTariffCoal = GetIntFromArchive(ar);
			nSpeed = GetIntFromArchive(ar);
			nRange = GetIntFromArchive(ar);
			ar >> strNum;
			h3 = (float)strtod(strNum,&stopstring);
			//begin: ship exist
			nShipExistSetCheck = GetIntFromArchive(ar);
			nShipExistForceCheck = GetIntFromArchive(ar);
			nShipExistCheck = GetIntFromArchive(ar);
			nShipExistBuildCheck = GetIntFromArchive(ar);
			nShipExistDesignCheck = GetIntFromArchive(ar);
			nShipExistNum = GetIntFromArchive(ar);
			nShipExistAdvantPrice = GetIntFromArchive(ar); 
			nShipExistPrice = GetIntFromArchive(ar);
			nShipExistBuildNum = GetIntFromArchive(ar);
			nShipExistBuildAdvantPrice = GetIntFromArchive(ar);
			nShipExistBuildPrice = GetIntFromArchive(ar);
			nShipExistDesignAdvantPrice = GetIntFromArchive(ar);
			nShipExistDesignPrice = GetIntFromArchive(ar);
			//end: ship exist
			
			if ((nLoadMax <= nLoadMin) && (nShipExistSetCheck == 0))
				continue;
			
			if (nShipExistSetCheck == 0){ //ship dosen't exist
			//Create Ship/ACV instances according to Record's data
				//coast = nPriceMin; //nPriceMin replaced by nTariffGenVF;
				for ( load = nLoadMin; load <= nLoadMax; load += nLoadDelta){
					CShip* pShip = new CShip(strShipTypeName.GetBuffer(20), strRegisterClass.GetBuffer(10));

					if (fl_proto == 1){  //tmp flag for prototypes integration
										// need to be deleted later.
						if (strRegisterClass.Find("*ACV*") != -1){ //ACV
							acv_prototype_tmp->name = strPrototypeName;
							res = CFleetTechEconomicEng::Instance()->GetACVPrototypeMainDetails(acv_prototype_tmp);
							acv_tmp->name = strShipTypeName;
							acv_tmp->speed = (float)nSpeed;
							acv_tmp->Dgr = load;
							acv_tmp->r = nRange;
							acv_tmp->h3 = h3;
							res = CACVDesign::Instance()->GetACVMainDetails(acv_prototype_tmp, acv_tmp);
							//nLp = acv_tmp->Lp;
							//nBp = ship_tmp->Bp;
							//nT = ship_tmp->T;
							//pShip->Coast = CFleetTechEconomicEng::_instance->GetACVBuildCoast(strShipTypeName,strRegisterClass,load,nSpeed,nLp,nBp,nT,strPrototypeName);  
							if (res == -1)
								pShip->Coast = -1;
							else
								pShip->Coast = CFleetTechEconomicEng::_instance->GetACVBuildCoast(acv_tmp,strPrototypeName);
							
							pShip->Range = nRange;
							pShip->h3 = h3;
						
						}else { //ship
							ship_prototype_tmp->name = strPrototypeName;
							res = CFleetTechEconomicEng::Instance()->GetShipPrototypeMainDetails(ship_prototype_tmp);
							ship_tmp->speed = (float)nSpeed;
							ship_tmp->Pgr = load;
							ship_tmp->name = strShipTypeName;
							ship_tmp->mu = CFleetTechEconomicEng::Instance()->GetCargoSpecificCapacity(nCargoCompatiblCoal, nCargoCompatiblGeneral, nCargoCompatiblOil, nCargoCompatiblVegFruits);
							res = CShipDesign::Instance()->GetShipMainDetails(ship_prototype_tmp, ship_tmp);
							//nL = ship_tmp->L;
							//nB = ship_tmp->B;
							//nT = ship_tmp->T;
							//pShip->Coast = CFleetTechEconomicEng::_instance->GetVehicleBuildCoast(strShipTypeName,strRegisterClass,load,nSpeed,nL,nB,nT,strPrototypeName);  
							pShip->Coast = CFleetTechEconomicEng::_instance->GetVehicleBuildCoast(ship_tmp, strPrototypeName);  
							pShip->Range = 0;
							pShip->h3 = 0;

						}// end if (strRegisterClass.Find("*ACV*") != -1) //ACV
					} //end if (fl_proto == 1)
					else{	//(fl_proto == 0)
					//		pShip->Coast = coast;
					}

					pShip->Load = load;
					pShip->Speed = nSpeed;
					pShip->CargoCompatibility[1]= nCargoCompatiblCoal;
					pShip->CargoCompatibility[2]= nCargoCompatiblGeneral;
					pShip->CargoCompatibility[3]= nCargoCompatiblOil;
					pShip->CargoCompatibility[4]= nCargoCompatiblVegFruits;
					pShip->TariffGenVF = nTariffGenVF;
					pShip->TariffOil = nTariffOil;
					pShip->TariffCoal = nTariffCoal;
					//pShip->PrototypeName = strPrototypeName.;
					strcpy(pShip->PrototypeName, strPrototypeName);
					
					if (pShip->Coast > 0)
						CVehiclesList::_instance->append(pShip);
					else{
						CString strMessage = "Ошибка: проблема создания расчетного судна (строительная стоимость...): ";
						strMessage += pShip->GetTypeName();
						strMessage += ", Грузоподъемность:  ";
						_itoa(pShip->Load, strData, 10);
						strMessage += strData;
						strMessage += ". CShipFactory()\n";
						fprintf(streamError, strMessage);
						m_nErrorStatus = 1;
					}
					//coast += nPriceDelta; //nPriceDelta replaced by nTariffCoal

					//if (nShipExistSetCheck == 1)
					//	break;
					
				} // end: 	for ( load = nLoadMin; load <= nLoadMax; load += nLoadDelta)
			}//end: if (nShipExistSetCheck == 0){ 
			else if (nShipExistSetCheck == 1){//ship exists
					CShip* pShip = new CShip(strShipTypeName.GetBuffer(20), strRegisterClass.GetBuffer(10));

					pShip->Exist = nShipExistSetCheck;
					pShip->ExistForce = nShipExistForceCheck;
					pShip->ExistOperation = nShipExistCheck;
					pShip->ExistBuild = nShipExistBuildCheck;
					pShip->ExistDesign = nShipExistDesignCheck;
					pShip->ExistNum = nShipExistNum ;
					pShip->ExistNumRemain = pShip->ExistNum;
					pShip->ExistAdvantPrice = nShipExistAdvantPrice ; 
					pShip->ExistPrice = nShipExistPrice ;
					pShip->ExistBuildNum = nShipExistBuildNum ;
					pShip->ExistBuildNumRemain = pShip->ExistBuildNum;
					pShip->ExistBuildAdvantPrice = nShipExistBuildAdvantPrice ;
					pShip->ExistBuildPrice = nShipExistBuildPrice ;
					pShip->ExistDesignAdvantPrice = nShipExistDesignAdvantPrice ;
					pShip->ExistDesignPrice = nShipExistDesignPrice ;

					if (strRegisterClass.Find("*ACV*") != -1){ //ACV
						acv_prototype_tmp->name = strPrototypeName;
						res = CFleetTechEconomicEng::Instance()->GetACVPrototypeMainDetails(acv_prototype_tmp);
						res = CACVDesign::Instance()->GetExistACVMainDetails(acv_prototype_tmp, acv_tmp);
						pShip->Load = acv_tmp->Dgr;
						pShip->Speed = (int)acv_tmp->speed;
						pShip->Range = acv_tmp->r;
						pShip->h3 = acv_tmp->h3;
						if (res == -1)
							pShip->Coast = -1;
						else
							pShip->Coast = CFleetTechEconomicEng::_instance->GetExistACVBuildCoast(pShip, acv_tmp);
					
					}else { //ship
						ship_prototype_tmp->name = strPrototypeName;
						res = CFleetTechEconomicEng::Instance()->GetShipPrototypeMainDetails(ship_prototype_tmp);
						ship_tmp->mu = CFleetTechEconomicEng::Instance()->GetCargoSpecificCapacity(nCargoCompatiblCoal, nCargoCompatiblGeneral, nCargoCompatiblOil, nCargoCompatiblVegFruits);
						res = CShipDesign::Instance()->GetExistShipMainDetails(ship_prototype_tmp, ship_tmp);
						pShip->Load = ship_tmp->Pgr;
						pShip->Speed = (int)ship_tmp->speed;						
						pShip->Coast = CFleetTechEconomicEng::_instance->GetExistVehicleBuildCoast(pShip, ship_tmp);  
					}// end: if (strRegisterClass.Find("*ACV*") != -1) //ACV
					pShip->CargoCompatibility[1]= nCargoCompatiblCoal;
					pShip->CargoCompatibility[2]= nCargoCompatiblGeneral;
					pShip->CargoCompatibility[3]= nCargoCompatiblOil;
					pShip->CargoCompatibility[4]= nCargoCompatiblVegFruits;

					pShip->TariffGenVF = nTariffGenVF;
					pShip->TariffOil = nTariffOil;
					pShip->TariffCoal = nTariffCoal;
					strcpy(pShip->PrototypeName, strPrototypeName);

					if (pShip->Coast > 0)
						CVehiclesList::_instance->append(pShip);
					else{
						CString strMessage = "Ошибка: проблема создания расчетного судна (строительная стоимость...): ";
						strMessage += pShip->GetTypeName();
						strMessage += ", Грузоподъемность:  ";
						_itoa(pShip->Load, strData, 10);
						strMessage += strData;
						strMessage += ". CShipFactory()\n";
						fprintf(streamError, strMessage);
						m_nErrorStatus = 1;
					}

			}//end: else if (nShipExistSetCheck == 1)//ship exists
			else {}


			count++;
			i++;
		}
	}
	
	CACVDesign::Instance()->m_bPrintIsOn = 0;

}

int CShipFactory::GetIntFromArchive(CArchive &ar)
{
	char *stopstring;
	CString strData;
	int intData;

	ar >> strData;
	intData = (int)strtod(strData,&stopstring);

	return intData;
}

void CShipFactory::PutIntToArchive(int intData, CArchive &ar)
{
	char strData[5];
	CString strText;

	_itoa(intData, strData, 10);
	strText = strData;
	strText.Format("%s", strText);
	strText.TrimLeft();
	ar << strText;
}

void CShipFactory::GetInvestmentInfoFromScenario()
{
	CString	str, strNum;
	long lInvestmentYearLimit;
	char *stopstring;
	ULONGLONG positionEnd;
	int nIncludeLandStruct;

	if (_access(m_pScenarioFile,00) == -1)
	{
		AfxMessageBox("Ошибка: Файл сценария не существует! CShipFactory::GetInvestmentInfoFromScenario()");
		m_pOutView->TextOut("Ошибка: Файл сценария не существует! CShipFactory::GetInvestmentInfoFromScenario()");
		return;
	}
	
	CFile f( m_pScenarioFile, CFile::modeRead );
	f.SeekToEnd();
	positionEnd = f.GetPosition();
	if (positionEnd < 3){ //Scenario File - empty
		//AfxMessageBox("GetInvestmentInfoFromScenario: Scenario File - empty");
		m_pOutView->TextOut("Файл сценария - пуст! CShipFactory::GetInvestmentInfoFromScenario()");
		return;		
	}
	f.SeekToBegin();
	CArchive ar(&f, CArchive::load);

	int count = 0;

	ar >> str;
	while(count < 1000){
		ar >> str; //Load first field of record
		if (str == "INVEST DATA"){
			ar >> strNum;	//invest Limit
			lInvestmentYearLimit = (long)strtod(strNum,&stopstring);
			CFleetTechEconomicEng::Instance()->SetInvestmentYearLimit(lInvestmentYearLimit);

			ar >> strNum;	//Include Land Struct in calculation in scenario
			nIncludeLandStruct = (int)strtod(strNum,&stopstring);
			if ((nIncludeLandStruct != 0)&& (nIncludeLandStruct != 1)){
				AfxMessageBox("Ошибка загрузки IncludeLandStruct-инфо. из файла сценария.\nCShipFactory::GetInvestmentInfoFromScenario()");
				m_pOutView->TextOut("Ошибка загрузки значения \"IncludeLandStruct\" из файла сценария. CShipFactory::GetInvestmentInfoFromScenario()");
				return;
			}

			break;
		}else if (str == "END OF SCENARIO FILE"){
			break;
		}
		count++;
	}

	ar.Close();
	f.Close();

}

int CShipFactory::GetErrorStatus()
{
	return m_nErrorStatus;
}
