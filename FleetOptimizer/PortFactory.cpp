// PortFactory.cpp: implementation of the CPortFactory class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "FleetOptimizer.h"
#include "PortFactory.h"
#include "Port.h"
#include "PortList.h"
//#include <iostream.h>
#include <io.h>
#include "OutputView.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


CPortFactory* CPortFactory::_instance = 0;

CPortFactory* CPortFactory::Instance(){
    if (_instance == 0){
        _instance = new CPortFactory;
    }
    return _instance;
}
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPortFactory::CPortFactory()
{
    //MessageBox(NULL,"CPortFactory()","CPortFactory",MB_OK);

	m_pScenarioFile = "default.fsc";
	//m_pPortDistMatrixFile = "ports_dist.fl";
	CString	str = ((CFleetOptimizerApp*)AfxGetApp())->GetProfileString("FleetOptimizer","DbPortsDistMatrix");
	m_pPortDistMatrixFile = _strdup(str.GetBuffer(20));

	POSITION pos = ((CFleetOptimizerApp*)AfxGetApp())->GetCurrFleetOptimizerDoc()->GetFirstViewPosition();
	m_pOutView = (COutputView*) ((CFleetOptimizerApp*)AfxGetApp())->GetCurrFleetOptimizerDoc()->GetNextView(pos);
	m_pOutView = (COutputView*) ((CFleetOptimizerApp*)AfxGetApp())->GetCurrFleetOptimizerDoc()->GetNextView(pos);
	m_pOutView->TextOut("CPortFactory()");


/*
    CreatePortsInstances();

    //MessageBox(NULL,"CPortFactory(): Ports created","CPortFactory",MB_OK);
	//m_pOutView->TextOut("    Ports created");
	if (SaveToFile())
		//MessageBox(NULL,"CPortFactory(): Ports Saved to File SUCSSEDED","CPortFactory",MB_OK);
		m_pOutView->TextOut("     Ports Saved to File SUCSSEDED");
	else{ 
		//MessageBox(NULL,"CPortFactory(): Ports Saved to File FAILED","CPortFactory",MB_OK);
		m_pOutView->TextOut("     Ports Saved to File FAILED");
	}
*/
}

/*CPortFactory::~CPortFactory()
{

}*/




void CPortFactory::CreatePortsInstances(void)
{

	POSITION pos = ((CFleetOptimizerApp*)AfxGetApp())->GetCurrFleetOptimizerDoc()->GetFirstViewPosition();
	m_pOutView = (COutputView*) ((CFleetOptimizerApp*)AfxGetApp())->GetCurrFleetOptimizerDoc()->GetNextView(pos);
	m_pOutView = (COutputView*) ((CFleetOptimizerApp*)AfxGetApp())->GetCurrFleetOptimizerDoc()->GetNextView(pos);
	m_pOutView->TextOut("Port objects creating by CPortFactory");

	m_pScenarioFile = ((CFleetOptimizerApp*)AfxGetApp())->m_pScenarioFile;
	CreatePortsInstancesFromScenario();

    //MessageBox(NULL,"CPortFactory(): Ports created","CPortFactory",MB_OK);
	//m_pOutView->TextOut("    Ports created");
	if (SaveToFile()){
		//MessageBox(NULL,"CPortFactory(): Ports Saved to File SUCSSEDED","CPortFactory",MB_OK);
		//m_pOutView->TextOut("Порты сохранены в Базе Данных.  CPortFactory::CreatePortsInstances()");
	}
	else{ 
		//MessageBox(NULL,"CPortFactory(): Ports Saved to File FAILED","CPortFactory",MB_OK);
		m_pOutView->TextOut("Error saving ports in DataBase. CPortFactory::CreatePortsInstances()");
	}


/*
//////////////////////  PortA. "BASA 1"  ///////////////////////////
// PortA. creation

    CPort* pPortA = new CPort;
    sprintf(pPortA->name, "BASA 1");
    sprintf(pPortA->Region, "Arctic");
    pPortA->available_cargoes.coal = 2500;
    pPortA->available_cargoes.general = 2000;
    pPortA->available_cargoes.oil = 3000;
    pPortA->available_cargoes.vegetables_fruits = 1000;

//PNB creation and add to PNB list
//PNB KUKU
    sprintf(pPortA->PNBList[0].name,"KUKU");
    pPortA->PNBList[0].id = 1;
    pPortA->PNBList[0].required_cargoes.coal = 20000;
    pPortA->PNBList[0].required_cargoes.general = 7000;
    pPortA->PNBList[0].required_cargoes.oil = 1800;
    pPortA->PNBList[0].required_cargoes.vegetables_fruits = 1700;
    pPortA->PNBList[0].required_cargoes_0.coal = 20000;
    pPortA->PNBList[0].required_cargoes_0.general = 7000;
    pPortA->PNBList[0].required_cargoes_0.oil = 1800;
    pPortA->PNBList[0].required_cargoes_0.vegetables_fruits = 1700;

//	pPortA->PNBList[0].acv_loading_productivity.coal_loading = 10; // тонн.час
//	pPortA->PNBList[0].acv_loading_productivity.general_loading = 10;
//	pPortA->PNBList[0].acv_loading_productivity.oil_loading = 10;
//	pPortA->PNBList[0].acv_loading_productivity.vegetables_fruits_loading = 10;
//
//	pPortA->PNBList[0].ship_loading_productivity.coal_loading = 10; // тонн.час
//	pPortA->PNBList[0].ship_loading_productivity.general_loading = 10;
//	pPortA->PNBList[0].ship_loading_productivity.oil_loading = 10;
//	pPortA->PNBList[0].ship_loading_productivity.vegetables_fruits_loading = 10;

	pPortA->PNBList[0].acv_loading_productivity[1] = 10; // тонн.час
	pPortA->PNBList[0].acv_loading_productivity[2] = 10;
	pPortA->PNBList[0].acv_loading_productivity[3] = 10;
	pPortA->PNBList[0].acv_loading_productivity[4] = 10;

	pPortA->PNBList[0].ship_loading_productivity[1] = 10; // тонн.час
	pPortA->PNBList[0].ship_loading_productivity[2] = 10;
	pPortA->PNBList[0].ship_loading_productivity[3] = 10;
	pPortA->PNBList[0].ship_loading_productivity[4] = 10;

//PNB MUKU
    sprintf(pPortA->PNBList[1].name,"MUKU");
    pPortA->PNBList[1].id = 2;
    pPortA->PNBList[1].required_cargoes.coal = 7000;
    pPortA->PNBList[1].required_cargoes.general = 1550;
    pPortA->PNBList[1].required_cargoes.oil = 2000;
    pPortA->PNBList[1].required_cargoes.vegetables_fruits = 1700;
    pPortA->PNBList[1].required_cargoes_0.coal = 7000;
    pPortA->PNBList[1].required_cargoes_0.general = 1550;
    pPortA->PNBList[1].required_cargoes_0.oil = 2000;
    pPortA->PNBList[1].required_cargoes_0.vegetables_fruits = 1700;


//	pPortA->PNBList[1].acv_loading_productivity.coal_loading = 10; // тонн.час
//	pPortA->PNBList[1].acv_loading_productivity.general_loading = 10;
//	pPortA->PNBList[1].acv_loading_productivity.oil_loading = 10;
//	pPortA->PNBList[1].acv_loading_productivity.vegetables_fruits_loading = 10;
//
//	pPortA->PNBList[1].ship_loading_productivity.coal_loading = 10; // тонн.час
//	pPortA->PNBList[1].ship_loading_productivity.general_loading = 10;
//	pPortA->PNBList[1].ship_loading_productivity.oil_loading = 10;
//	pPortA->PNBList[1].ship_loading_productivity.vegetables_fruits_loading = 10;

	pPortA->PNBList[1].acv_loading_productivity[1] = 10; // coal, тонн.час
	pPortA->PNBList[1].acv_loading_productivity[2] = 10; //general_loading
	pPortA->PNBList[1].acv_loading_productivity[3] = 10; //oil_loading
	pPortA->PNBList[1].acv_loading_productivity[4] = 10; //vegetables_fruits_loading

	pPortA->PNBList[1].ship_loading_productivity[1] = 10; // тонн.час
	pPortA->PNBList[1].ship_loading_productivity[2] = 10;
	pPortA->PNBList[1].ship_loading_productivity[3] = 10;
	pPortA->PNBList[1].ship_loading_productivity[4] = 10;
//	//PNB MUKU
//    sprintf(pPortA->PNBList[1].name,"MUKU");
//    pPortA->PNBList[1].id = 2;
//    pPortA->PNBList[1].required_cargoes.coal = 7000;
//    pPortA->PNBList[1].required_cargoes.general = 155000;
//    pPortA->PNBList[1].required_cargoes.oil = 98000;
//    pPortA->PNBList[1].required_cargoes.vegetables_fruits = 1000;
//    pPortA->PNBList[1].required_cargoes_0.coal = 7000;
//    pPortA->PNBList[1].required_cargoes_0.general = 155000;
//    pPortA->PNBList[1].required_cargoes_0.oil = 98000;
//    pPortA->PNBList[1].required_cargoes_0.vegetables_fruits = 1000;


//DistanceMatrix
    int DistanceMatrixPortA[11][11] = {
        { 0,400,300,200,100, 1, 1, 1, 1, 1, 1},
        {400, 0, 200, 400, 50, 1, 1, 1, 1, 1, 1},
        {300, 200, 0, 30, 600, 1, 1, 1, 1, 1, 1},
        {200, 400, 30, 0, 70, 1, 1, 1, 1, 1, 1},
        {100, 50, 600, 70, 0, 1, 1, 1, 1, 1, 1},
        { 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1},
        { 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1},
        { 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1},
        { 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1},
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1},
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0}
    };  
    int i,j;
    for (i=0; i<11; i++){
        for (j=0;j<11;j++){
            pPortA->DistanceMatrix[i][j]= DistanceMatrixPortA[i][j];
        }
    }
	
	pPortA->LinesNumber = 8;
//Add to Ports list 
    CPortList::_instance->append(pPortA);

//LOG into E:\\Users\\mv\\Log\\fleet.out
    FILE *stream;
    stream = fopen( "E:\\Users\\mv\\Log\\fleet.txt", "w" );
    fprintf( stream, "Base Port: %s\n", pPortA->name);
    fprintf( stream, "Base Port, coal : %d\n", pPortA->available_cargoes.coal);
    fprintf( stream, "PNB %s:\n", pPortA->PNBList[0].name);
    fprintf( stream, "\tid: %d\n",pPortA->PNBList[0].id);
    fprintf( stream, "\tid!!: %d\n",pPortA->PNBList[1].id);
    fprintf( stream, "\tDistance from Base: %d\n",pPortA->DistanceMatrix[0][pPortA->PNBList[0].id]);
    fprintf( stream, "\trequired_cargoes:\n\t\tcoal: %d\n",pPortA->PNBList[0].required_cargoes.coal);
    fprintf( stream, "\t\tgeneral: %d\n",pPortA->PNBList[0].required_cargoes.general);
    fprintf( stream, "\t\toil: %d\n\n",pPortA->PNBList[0].required_cargoes.oil);
    fprintf( stream, "DistanceMatrix:\n");
    for (i=0; i<11; i++){
        for (j=0;j<11;j++){
            fprintf( stream, "%d ",pPortA->DistanceMatrix[i][j]);
        }
       fprintf( stream, "\n");
    }
//LOG end
*/
}


int CPortFactory::SaveToFile()
{
    CPort *pBasePort = CPortList::Instance()->GetPortListHead();
	int i;

	if (pBasePort == 0){
		m_pOutView->TextOut("Ошибка: Список портов - пуст! CPortFactory::SaveToFile()");
		return 0;
	}


	char* pFileName = "port_log.fl";
	CFile f( pFileName, CFile::modeCreate | CFile::modeWrite );

	CArchive archive(&f, CArchive::store);

	int BasePortNum = 0;
	while(pBasePort){
		BasePortNum++;
		pBasePort= pBasePort->GetNextPort();	
	}
	
	archive << BasePortNum;
	pBasePort = CPortList::Instance()->GetPortListHead();
	while(pBasePort){
		CString PortName(pBasePort->GetPortName()); //Port Name
		archive << PortName;
		for (i = 0; i < MAXPNBNUMBER; i++){
			PortName = pBasePort->GetPNB(i).name; //PNB Name
			archive << PortName;
			archive << pBasePort->GetPNB(i).required_cargoes.coal;
			archive << pBasePort->GetPNB(i).required_cargoes.general;
			archive << pBasePort->GetPNB(i).required_cargoes.oil;
			archive << pBasePort->GetPNB(i).required_cargoes.vegetables_fruits;
		}

		pBasePort= pBasePort->GetNextPort();	
	}

	archive.Close();
	f.Close();

	return 1;
}

void CPortFactory::CreatePortsInstancesFromScenario()
{
	CString	str, strPortID, strPortName, strPortType, strBasePortName, strPortRegion;
	int cargoReqCoal,cargoReqGen,cargoReqOil,cargoReqVegFr,
		cargoAvCoal,cargoAvGen,cargoAvOil,cargoAvVegFr,
		shipLoadProdCoal, shipLoadProdGen, shipLoadProdOil, shipLoadProdVegFr,
		acvLoadProdCoal, acvLoadProdGen, acvLoadProdOil, acvLoadProdVegFr;
	CPort *pBasePort;
	CString strPortNameTmp, strBasePortNameTmp;
	int nPNBNum, flBasePortExist, nPortID;
	char *stopstring;
	ULONGLONG positionEnd;
	/*LandStruct: */ int nLandStructCalcActivate, nShipCourtAvailable, nACVCourtAvailable,
	nShipCourtInvestPP, nMoorElemPrice, nMoorLength, nACVSpaceElemPrice,
	nACVSpace, nACVCourtInvestPP;
	int RepairDetails[2][4];
	int i, j;

	//Проверка чувствительности решения задачи к изменению объемов перевозок.
	//Если bSensCheck == TRUE -  значения требуемых портам/пунктам грузов увеличиваются на nSensCheckValue процентов.
	//Если bSensCheck == FALSE, то решается задача с заданными объемами,
	//int bSensCheck = TRUE; //
	//int nSensCheckValue = 15; // % - проценты

	int bSensCheck = ((CFleetOptimizerApp*)AfxGetApp())->GetProfileInt("FleetOptimizer","UseSenseCheck",0);
	int nSensCheckValue = ((CFleetOptimizerApp*)AfxGetApp())->GetProfileInt("FleetOptimizer","SenseCheckRate",0);

	//end

	str = ((CFleetOptimizerApp*)AfxGetApp())->GetProfileString("FleetOptimizer","AppDir");	
	SetCurrentDirectory(str);

	if (_access(m_pScenarioFile,00) == -1)
	{
		AfxMessageBox("Ошибка: Файл сценария не существует! CPortFactory::CreatePortsInstancesFromScenario()");
		m_pOutView->TextOut("Ошибка: Файл сценария не существует! CPortFactory::CreatePortsInstancesFromScenario()");
		return;
	}
	if (_access(m_pPortDistMatrixFile,00) == -1)
	{
		AfxMessageBox("Ошибка: Файл матрицы расстояний не существует! CPortFactory:CreatePortsInstancesFromScenario()");
		m_pOutView->TextOut("Ошибка: Файл матрицы расстояний не существует! CPortFactory:CreatePortsInstancesFromScenario()");
		return;
	}
	
	CFile f( m_pScenarioFile, CFile::modeRead );
	f.SeekToEnd();
	positionEnd = f.GetPosition();
	if (positionEnd < 3){ //Scenario File - empty
		AfxMessageBox("Файл сценария - пуст! CPortFactory::CreatePortsInstancesFromScenario()");
		m_pOutView->TextOut("Файл сценария - пуст! CPortFactory::CreatePortsInstancesFromScenario()");
		return;		
	}
	f.SeekToBegin();
	CArchive ar(&f, CArchive::load);
	positionEnd = f.GetPosition();




	int flPortsDataFound = 0;
	int count = 0;
	//int i = 1;

	while(!flPortsDataFound && (count < 1000)){
			ar >> str; 
			if (str == "PORTS DATA")
				flPortsDataFound = 1;
			count++;
	}
	
	count = 0;
	while(count < 100){
		ar >> str; 
		//if (str == "END OF SCENARIO FILE")
		if ((str.Find("INVEST DATA") != -1) ||(str.Find("END OF SCENARIO FILE") != -1))
		//if ((str == "INVEST DATA") ||(str == "END OF SCENARIO FILE")) 
			break;

		strPortID = str;
		nPortID = (int)strtod(strPortID,&stopstring);
		ar >> strPortName;
		ar >> strPortType;
		ar >> strBasePortName;
		ar >> strPortRegion;
		
		cargoReqCoal	= GetIntFromArchive(ar);
		cargoReqGen		= GetIntFromArchive(ar);
		cargoReqOil		= GetIntFromArchive(ar);
		cargoReqVegFr	= GetIntFromArchive(ar);
		cargoAvCoal		= GetIntFromArchive(ar);
		cargoAvGen		= GetIntFromArchive(ar);
		cargoAvOil		= GetIntFromArchive(ar);
		cargoAvVegFr	= GetIntFromArchive(ar);

		if (bSensCheck == TRUE){ //Проверка чувствительности решения задачи к изменению объемов перевозок
			cargoReqCoal	+= (int)((float)(cargoReqCoal*nSensCheckValue)/100);
			cargoReqGen		+= (int)((float)(cargoReqGen*nSensCheckValue)/100);;
			cargoReqOil		+= (int)((float)(cargoReqOil*nSensCheckValue)/100);;
			cargoReqVegFr	+= (int)((float)(cargoReqVegFr*nSensCheckValue)/100);;
			cargoAvCoal		+= (int)((float)(cargoAvCoal*nSensCheckValue)/100);;
			cargoAvGen		+= (int)((float)(cargoAvGen*nSensCheckValue)/100);;
			cargoAvOil		+= (int)((float)(cargoAvOil*nSensCheckValue)/100);;
			cargoAvVegFr	+= (int)((float)(cargoAvVegFr*nSensCheckValue)/100);;
		}

		shipLoadProdCoal	= GetIntFromArchive(ar);
		shipLoadProdGen		= GetIntFromArchive(ar);
		shipLoadProdOil		= GetIntFromArchive(ar);
		shipLoadProdVegFr	= GetIntFromArchive(ar);
		acvLoadProdCoal		= GetIntFromArchive(ar);
		acvLoadProdGen		= GetIntFromArchive(ar);
		acvLoadProdOil		= GetIntFromArchive(ar);
		acvLoadProdVegFr	= GetIntFromArchive(ar);
		
		//Land Struct loading from archive begin
		nLandStructCalcActivate = GetIntFromArchive(ar);
		nACVCourtAvailable = GetIntFromArchive(ar);
		nACVCourtInvestPP = GetIntFromArchive(ar);
		nShipCourtAvailable = GetIntFromArchive(ar);
		nShipCourtInvestPP = GetIntFromArchive(ar);
		nACVSpace = GetIntFromArchive(ar);
		nACVSpaceElemPrice = GetIntFromArchive(ar);
		nMoorLength = GetIntFromArchive(ar);
		nMoorElemPrice = GetIntFromArchive(ar);
		for (i=0; i <=1; i++)
			for (j=0; j <=3; j++)
				RepairDetails[i][j] = GetIntFromArchive(ar);;
		//Land Struct loading from archive end

		if (strPortType == "Base Port"){ // Базовый порт
			//Проверить существует ли уже данный порт в списке.
			//Если существует - значит он был создан во время загрузки PNB.
			//В любом случае порт не содержит информации о себе: ID, грузы, ...
			// т.е. эта информация д.б. добавлена.
			pBasePort = CPortList::Instance()->GetPortListHead();
			flBasePortExist = 0;
			while(pBasePort){
				strBasePortNameTmp = pBasePort->GetPortName(); //Port Name
				if (strPortName == strBasePortNameTmp){
					flBasePortExist = 1;
					break;
				}
				else
					pBasePort= pBasePort->GetNextPort();	
			}
			if (flBasePortExist == 0){
				CPort *pPort = new CPort;
				sprintf(pPort->name, strPortName);
				sprintf(pPort->Region, strPortRegion);
				CPortList::_instance->append(pPort);
				pBasePort = pPort;
			}
			pBasePort->id = nPortID;
			pBasePort->available_cargoes.coal = cargoAvCoal;
			pBasePort->available_cargoes.general = cargoAvGen;
			pBasePort->available_cargoes.oil = cargoAvOil;
			pBasePort->available_cargoes.vegetables_fruits = cargoAvVegFr; 

			pBasePort->ship_loading_productivity[1] = shipLoadProdCoal;
			pBasePort->ship_loading_productivity[2] = shipLoadProdGen;
			pBasePort->ship_loading_productivity[3] = shipLoadProdOil;
			pBasePort->ship_loading_productivity[4] = shipLoadProdVegFr;

			pBasePort->acv_loading_productivity[1] = acvLoadProdCoal;
			pBasePort->acv_loading_productivity[2] = acvLoadProdGen;
			pBasePort->acv_loading_productivity[3] = acvLoadProdOil;
			pBasePort->acv_loading_productivity[4] = acvLoadProdVegFr;

			//Land Structs:
			pBasePort->m_LandStructData.nACVSpace = nACVSpace;
			pBasePort->m_LandStructData.nACVSpaceElemPrice = nACVSpaceElemPrice;
			pBasePort->m_LandStructData.nMoorLength = nMoorLength;
			pBasePort->m_LandStructData.nMoorElemPrice = nMoorElemPrice;
			pBasePort->m_LandStructData.RepairDetailsACV.nCapability = RepairDetails[0][0];
			pBasePort->m_LandStructData.RepairDetailsACV.nVhehicleLoad = RepairDetails[0][1];
			pBasePort->m_LandStructData.RepairDetailsACV.a = RepairDetails[0][2];
			pBasePort->m_LandStructData.RepairDetailsACV.b = RepairDetails[0][3];
			pBasePort->m_LandStructData.RepairDetailsShip.nCapability = RepairDetails[1][0];
			pBasePort->m_LandStructData.RepairDetailsShip.nVhehicleLoad = RepairDetails[1][1];
			pBasePort->m_LandStructData.RepairDetailsShip.a = RepairDetails[1][2];
			pBasePort->m_LandStructData.RepairDetailsShip.b = RepairDetails[1][3];
		

		}

		else if (strPortType == "PNB"){ // ПНБ
			//Проверить, существует ли базовый порт; если нет - определить его
			//подробная информация о базовом порте будет добавлена 
			//при загрузке базового порта из архива
			pBasePort = CPortList::Instance()->GetPortListHead();
			flBasePortExist = 0;
			while(pBasePort){
				strBasePortNameTmp = pBasePort->GetPortName(); //Port Name
				if (strBasePortName == strBasePortNameTmp){
					flBasePortExist = 1;
					break;
				}
				else
					pBasePort= pBasePort->GetNextPort();	
			}
			if (flBasePortExist == 0){
				CPort *pPort = new CPort;
				sprintf(pPort->name, strBasePortName);
				sprintf(pPort->Region, strPortRegion);
				CPortList::_instance->append(pPort);
				pBasePort = pPort;
			}
			nPNBNum = pBasePort->GetLinesNumber()/4;
			//nPNBNum++;
			//pBasePort->PNBList[nPNBNum].id = nPNBNum;
			pBasePort->PNBList[nPNBNum].id = nPortID;
		    sprintf(pBasePort->PNBList[nPNBNum].name,strPortName);

			pBasePort->PNBList[nPNBNum].required_cargoes.coal = cargoReqCoal;
			pBasePort->PNBList[nPNBNum].required_cargoes.general = cargoReqGen;
			pBasePort->PNBList[nPNBNum].required_cargoes.oil = cargoReqOil;
			pBasePort->PNBList[nPNBNum].required_cargoes.vegetables_fruits = cargoReqVegFr;

			pBasePort->PNBList[nPNBNum].required_cargoes_0.coal = cargoReqCoal;
			pBasePort->PNBList[nPNBNum].required_cargoes_0.general = cargoReqGen;
			pBasePort->PNBList[nPNBNum].required_cargoes_0.oil = cargoReqOil;
			pBasePort->PNBList[nPNBNum].required_cargoes_0.vegetables_fruits = cargoReqVegFr;

			pBasePort->PNBList[nPNBNum].ship_loading_productivity[1] = shipLoadProdCoal;
			pBasePort->PNBList[nPNBNum].ship_loading_productivity[2] = shipLoadProdGen;
			pBasePort->PNBList[nPNBNum].ship_loading_productivity[3] = shipLoadProdOil;
			pBasePort->PNBList[nPNBNum].ship_loading_productivity[4] = shipLoadProdVegFr;

			pBasePort->PNBList[nPNBNum].acv_loading_productivity[1] = acvLoadProdCoal;
			pBasePort->PNBList[nPNBNum].acv_loading_productivity[2] = acvLoadProdGen;
			pBasePort->PNBList[nPNBNum].acv_loading_productivity[3] = acvLoadProdOil;
			pBasePort->PNBList[nPNBNum].acv_loading_productivity[4] = acvLoadProdVegFr;

			pBasePort->LinesNumber += 4;

			//Land Structs:
			pBasePort->PNBList[nPNBNum].LandStructDataPP.bACVCourtAvailable = nACVCourtAvailable;
			pBasePort->PNBList[nPNBNum].LandStructDataPP.bShipCourtAvailable = nShipCourtAvailable;
			pBasePort->PNBList[nPNBNum].LandStructDataPP.nACVCourtInvestRequere = nACVCourtInvestPP;
			pBasePort->PNBList[nPNBNum].LandStructDataPP.nShipCourtInvestRequere = nShipCourtInvestPP; 

		}
		else {}

		count++;
	}//end of: while(count < 100){

	ar.Close();
	f.Close();

	//Fill Distance matrix details for each Base Port	
	CFile f1( m_pPortDistMatrixFile, CFile::modeRead );
	f1.SeekToBegin();
	CArchive arPortDist(&f1, CArchive::load);
	pBasePort = CPortList::Instance()->GetPortListHead();
//	int i,j;
	count = ((CFleetOptimizerApp*)AfxGetApp())->GetProfileInt("FleetOptimizer","PortsNumberInDB",0);	

	while(pBasePort){
		for (i = 0; i < MAX_PORT_RECORDS_IN_DB; i++)
			for (j = 0; j < MAX_PORT_RECORDS_IN_DB; j++){
				pBasePort->DistanceMatrix[i][j] = 1;
			}		
		for (i = 1; i <= count; i++)
			for (j = 1; j <= i; j++){
				pBasePort->DistanceMatrix[i][j] = GetIntFromArchive(arPortDist);
			}
		for (i = 1; i <= count; i++)
			for (j = i+1; j <= count; j++){
				pBasePort->DistanceMatrix[i][j] = pBasePort->DistanceMatrix[j][i];
			}
		pBasePort= pBasePort->GetNextPort();	
	}
	arPortDist.Close();
	f1.Close();

/*
/////////// test ////////
	//DistanceMatrix
    int DistanceMatrixPortA[11][11] = {
        { 0,400,300,200,100, 1, 1, 1, 1, 1, 1},
        {400, 0, 200, 400, 50, 1, 1, 1, 1, 1, 1},
        {300, 200, 0, 30, 600, 1, 1, 1, 1, 1, 1},
        {200, 400, 30, 0, 70, 1, 1, 1, 1, 1, 1},
        {100, 50, 600, 70, 0, 1, 1, 1, 1, 1, 1},
        { 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1},
        { 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1},
        { 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1},
        { 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1},
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1},
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0}
    };  
    int i,j;
	pBasePort = CPortList::Instance()->GetPortListHead();
	while(pBasePort){
		for (i=0; i<11; i++){
			for (j=0;j<11;j++){
				pBasePort->DistanceMatrix[i][j]= DistanceMatrixPortA[i][j];
			}
		}
		pBasePort= pBasePort->GetNextPort();
	}
*/
}





int CPortFactory::GetIntFromArchive(CArchive &ar)
{
	char *stopstring;
	CString strData;
	int intData;

	ar >> strData;
	intData = (int)strtod(strData,&stopstring);

	return intData;
}