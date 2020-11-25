// ScenarioWizardDlg.cpp : implementation file
//


#include "stdafx.h"
//#include "	\ add additional includes here"
#include "ScenarioWizardDlg.h"
#include "PortWizardDlg.h"
#include "ShipTypeWizardDlg.h"
#include "InvestDlg.h"
#include "resource.h"
#include <io.h>
#include <fcntl.h>      
#include <sys/types.h>
#include <sys/stat.h>
#include <shlobj.h>


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CScenarioWizardDlg dialog


CScenarioWizardDlg::CScenarioWizardDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CScenarioWizardDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CScenarioWizardDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pPortWizDlg = NULL;
	m_pShipWizDlg = NULL;
	m_pInvestDlg = NULL;
	m_SelectedPortMainInfo = NULL;
	m_SelectedShipMainInfo = NULL;
	m_SelectedPortLandStructInfo = NULL;
	//m_pShipsDBFile = "ships_db.fd";
	//m_pPortsDBFile = "ports_db.fd";
	//m_pPortDistMatrixFile = "ports_dist.fl";
	ReadFromRegistry();
	m_pScenarioFile = "";
	m_pScenDefaultFile = "default.fsc";
	m_flagEditPort = 0;
	m_flagEditShip = 0;
	m_bAutoLoad = 0;

}


void CScenarioWizardDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CScenarioWizardDlg)
	DDX_Control(pDX, IDC_INVESTLIMIT_EDIT, m_ctrInvestLimitEdit);
	DDX_Control(pDX, IDC_INVESTLIMIT_STATIC, m_ctrInvestLimitGroupBox);
	DDX_Control(pDX, IDC_SCEN_PORTDB_GRID, m_PortDBGrid);
	DDX_Control(pDX, IDC_SCEN_PORTS_GRID, m_ScenPortGrid);
	DDX_Control(pDX, IDC_SCEN_SHIPDB_GRID, m_ShipsDBGrid);
	DDX_Control(pDX, IDC_SCEN_SHIPS_GRID, m_ScenShipsGrid);
	//}}AFX_DATA_MAP

	FillPortDBGrid();
	FillShipDBGrid();
	SetTitlesShipsGrid();
	SetTitlesPortGrids();
	FillInvestInfo();

	//LoadDefaultScenario();
	if (m_bAutoLoad == 1){
		CString str = 	((CFleetOptimizerApp*)AfxGetApp())->GetProfileString("FleetOptimizer","ScenarioFile");	
		m_pScenarioFile = strdup(str.GetBuffer(20));		
		LoadScenarioData();
		str = "Мастер Сценариев - " + str;
		SetWindowText(str);
	}
}


BEGIN_MESSAGE_MAP(CScenarioWizardDlg, CDialog)
    ON_MESSAGE(WM_GOODBYE_PORTWIZARD, OnGoodbyePortWizard)
    ON_MESSAGE(WM_GOODBYE_SHIPWIZARD, OnGoodbyeShipWizard)
	//{{AFX_MSG_MAP(CScenarioWizardDlg)
	ON_BN_CLICKED(IDC_PORTWIZARD_BUTTON, OnPortwizardButton)
	ON_BN_CLICKED(IDC_SCENARIO_PORTVIEW_BUTTON, OnScenarioPortView)
	ON_BN_CLICKED(IDC_SHIPWIZARD_BUTTON, OnShipWizardButton)
	ON_BN_CLICKED(IDC_BUTTON2, OnGetPort)
	ON_BN_CLICKED(IDC_FILE_SAVE_BUTTON, OnScenFileSave)
	ON_BN_CLICKED(IDC_FILE_SAVE_AS_BUTTON, OnScenFileSaveAs)
	ON_BN_CLICKED(IDC_FILE_LOAD_BUTTON, OnScenFileLoad)
	ON_BN_CLICKED(IDC_GETSHIP_BUTTON, OnGetShip)
	ON_BN_CLICKED(IDC_SCENARIO_SHIPVIEW_BUTTON, OnScenarioShipView)
	ON_BN_CLICKED(IDC_SCENARIO_REFRESH_BUTTON, OnScenarioRefreshButton)
	ON_BN_CLICKED(IDC_SCENARIO_SHIPDELETE_BUTTON, OnScenarioShipDeleteButton)
	ON_BN_CLICKED(IDC_SCENARIO_PORTDELETE_BUTTON, OnScenarioPortDeleteButton)
	ON_WM_CONTEXTMENU()
	ON_COMMAND(ID_SCENWIZCONTX_SHIPSDB_VIEW, OnContextMenuShipsDBView)
	ON_COMMAND(ID_SCENWIZCONTX_SHIPSDB_GET, OnContextMenuShipsDBGet)
	ON_COMMAND(ID_SCENWIZCONTX_SHIPSCEN_VIEW, OnContextMenuShipScenView)
	ON_COMMAND(ID_SCENWIZCONTX_SHIPSSCEN_DELETE, OnContextMenuShipsScenDelete)
	ON_COMMAND(ID_SCENWIZCONTX_PORTSDB_VIEW, OnContextMenuPortsDBView)
	ON_COMMAND(ID_SCENWIZCONTX_PORTSDB_GET, OnContextMenuPortsDBGet)
	ON_COMMAND(ID_SCENWIZCONTX_PORTSCEN_VIEW, OnContextMunuPortScenView)
	ON_COMMAND(ID_SCENWIZCONTX_PORTSCEN_DELETE, OnContextMenuPortScenDelete)
	ON_BN_CLICKED(IDC_SCENARIO_PORTEDIT_BUTTON, OnScenarioPortEditButton)
	ON_BN_CLICKED(IDC_SCENARIO_SHIPEDIT_BUTTON, OnScenarioShipEditButton)
	ON_COMMAND(ID_SCENWIZCONTX_SHIPSCEN_EDIT, OnContextMenuShipScenEdit)
	ON_COMMAND(ID_SCENWIZCONTX_PORTSCEN_EDIT, OnContextMenuPortScenEdit)
	ON_BN_CLICKED(IDC_INVEST_BUTTON, OnInvestmentButton)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CScenarioWizardDlg message handlers

void CScenarioWizardDlg::OnPortwizardButton() 
{
	// TODO: Add your control notification handler code here
		//CPortWizardDlg   dlg;
		//dlg.DoModal();
		
		if(!m_pPortWizDlg)
		{
			m_pPortWizDlg  = new CPortWizardDlg();
		}
		else
		{
			m_pPortWizDlg->ShowWindow(SW_RESTORE);
			//m_pPortWizDlg->UpdateData(FALSE);
		}
		m_pPortWizDlg->ShowWindow(SW_SHOW);
}

void CScenarioWizardDlg::OnScenarioPortView() 
{
	m_test = 55555;

	CPortDetailsPSheet thePSheet(_T("Port Details"),this,NULL);
	FillSelectedPortScenInfo();
	thePSheet.SetSelectedPortMainInfo(m_SelectedPortMainInfo);
	thePSheet.m_CPortMainDetailsPPage.m_SelectedPortMainInfo = m_SelectedPortMainInfo;
	thePSheet.m_PortLandStructPPage.m_SelectedPortLandStructInfo = m_SelectedPortLandStructInfo;
	thePSheet.m_CPortDistMatrixPPage.m_ISLoadDetails = TRUE;
	thePSheet.m_CPortMainDetailsPPage.m_ISLoadDetails = TRUE;
	thePSheet.m_PortLandStructPPage.m_ISLoadDetails = TRUE;
	thePSheet.m_OpenStat = 1; //view
	thePSheet.m_CPortMainDetailsPPage.m_OpenStat = 1;
	thePSheet.m_PortLandStructPPage.m_OpenStat = 1;
	thePSheet.m_RecordsNumber = m_PortDBRecordsNum;
	thePSheet.DoModal();

}

void CScenarioWizardDlg::OnShipWizardButton() 
{
	// TODO: Add your control notification handler code here
	//CShipTypeWizardDlg dlg;
	//dlg.DoModal();

	if(!m_pShipWizDlg)
	{
		m_pShipWizDlg  = new CShipTypeWizardDlg();
	}
	else
	{
		m_pShipWizDlg->ShowWindow(SW_RESTORE);
		//m_pShipWizDlg->UpdateData(FALSE);
	}
	m_pShipWizDlg->ShowWindow(SW_SHOW);


}

long CScenarioWizardDlg::OnGoodbyePortWizard(UINT wParam, LONG lParam)
{
    // message received in response to modeless dialog OK
    //  and Cancel buttons
    /*TRACE("CScenarioWizardDlg::OnGoodbye %x, %lx\n", wParam, lParam);
    TRACE("Dialog edit1 contents = %s\n",
         (const char*) m_pDlg->m_strEdit1);
		 */
    m_pPortWizDlg->DestroyWindow();
    return 0L;
}

long CScenarioWizardDlg::OnGoodbyeShipWizard(UINT wParam, LONG lParam)
{
    // message received in response to modeless dialog OK
    //  and Cancel buttons
    /*TRACE("CScenarioWizardDlg::OnGoodbye %x, %lx\n", wParam, lParam);
    TRACE("Dialog edit1 contents = %s\n",
         (const char*) m_pDlg->m_strEdit1);
		 */
    m_pShipWizDlg->DestroyWindow();
    return 0L;
}

void CScenarioWizardDlg::Serialize(CArchive& ar) 
{
	if (ar.IsStoring())
	{	// storing code
	}
	else
	{	// loading code
		CString strPortName, strPortType, strBasePortName, 
				strPortRegion, strTmpMatrixValue, strNum;
		ULONGLONG positionEnd, position; 
		
		CFile* fp = ar.GetFile();
		fp->SeekToEnd();
		positionEnd = fp->GetPosition();
		fp->SeekToBegin();
		position = fp->GetPosition();
		
		//if (dwPositionEnd == 0)
		if (positionEnd < 3)
			m_PortDBRecordsNum = 0;
		else {
				ar >> strNum;
				char *stopstring;
				m_PortDBRecordsNum  = (int)strtod(strNum,&stopstring);
		}

		int i = 1;
		int j;
		char strRowNum[5];
   
		while (i <= m_PortDBRecordsNum){
			ar >> strPortName;
			ar >> strPortType;
			ar >> strBasePortName;
			ar >> strPortRegion;

			position = fp->GetPosition();
			
			_itoa(i, strRowNum, 10);
			m_PortDBGrid.SetTextMatrix(i,0,strRowNum);

			m_PortDBGrid.SetTextMatrix(i,1,strPortName);
			m_PortDBGrid.SetTextMatrix(i,2,strPortType);
			m_PortDBGrid.SetTextMatrix(i,3,strBasePortName);
			m_PortDBGrid.SetTextMatrix(i,4,strPortRegion);
			
			//Cargoes set
			for (j = 5; j <=20; j++){
				ar >> strNum;
				m_PortDBGrid.SetTextMatrix(i,j,strNum);
			}
			//Land Struct
			for (j = 21; j <=37; j++){
				ar >> strNum;
				m_PortDBGrid.SetTextMatrix(i,j,strNum);
			}

			position = fp->GetPosition();
			i++;
		}//end of while (i <= m_RecordsNumber)

	}
}

void CScenarioWizardDlg::FillPortDBGrid()
{
	CFile f( m_pPortsDBFile, CFile::modeRead );
	f.SeekToBegin();
	CArchive ar(&f, CArchive::load);
	Serialize(ar);
}

void CScenarioWizardDlg::FillSelectedPortScenInfo()
{
	char *stopstring;
	CString strNum;
	int row = m_ScenPortGrid.GetRow();

	m_SelectedPortMainInfo = new CPortMainInfo;
	m_SelectedPortLandStructInfo = new CPortLandStructInfo;

	strNum = m_ScenPortGrid.GetTextMatrix(row,0);
	m_SelectedPortMainInfo->m_nPortID = (int)strtod(strNum,&stopstring);

	m_SelectedPortMainInfo->m_strPortName = m_ScenPortGrid.GetTextMatrix(row,1);
	m_SelectedPortMainInfo->m_strPortType = m_ScenPortGrid.GetTextMatrix(row,2);
	m_SelectedPortMainInfo->m_strBasePortName = m_ScenPortGrid.GetTextMatrix(row,3);
	m_SelectedPortMainInfo->m_strPortRegion = m_ScenPortGrid.GetTextMatrix(row,4);
	
	strNum = m_ScenPortGrid.GetTextMatrix(row,5);
	m_SelectedPortMainInfo->required_cargoes.coal = (int)strtod(strNum,&stopstring);
	strNum = m_ScenPortGrid.GetTextMatrix(row,6);
	m_SelectedPortMainInfo->required_cargoes.general = (int)strtod(strNum,&stopstring);
	strNum = m_ScenPortGrid.GetTextMatrix(row,7);
	m_SelectedPortMainInfo->required_cargoes.oil = (int)strtod(strNum,&stopstring);
	strNum = m_ScenPortGrid.GetTextMatrix(row,8);
	m_SelectedPortMainInfo->required_cargoes.vegetables_fruits = (int)strtod(strNum,&stopstring);
	strNum = m_ScenPortGrid.GetTextMatrix(row,9);
	m_SelectedPortMainInfo->available_cargoes.coal = (int)strtod(strNum,&stopstring);
	strNum = m_ScenPortGrid.GetTextMatrix(row,10);
	m_SelectedPortMainInfo->available_cargoes.general = (int)strtod(strNum,&stopstring);
	strNum = m_ScenPortGrid.GetTextMatrix(row,11);
	m_SelectedPortMainInfo->available_cargoes.oil = (int)strtod(strNum,&stopstring);
	strNum = m_ScenPortGrid.GetTextMatrix(row,12);
	m_SelectedPortMainInfo->available_cargoes.vegetables_fruits = (int)strtod(strNum,&stopstring);

	strNum = m_ScenPortGrid.GetTextMatrix(row,13);
	m_SelectedPortMainInfo->ship_loading_productivity.coal = (int)strtod(strNum,&stopstring);
	strNum = m_ScenPortGrid.GetTextMatrix(row,14);
	m_SelectedPortMainInfo->ship_loading_productivity.general = (int)strtod(strNum,&stopstring);
	strNum = m_ScenPortGrid.GetTextMatrix(row,15);
	m_SelectedPortMainInfo->ship_loading_productivity.oil = (int)strtod(strNum,&stopstring);
	strNum = m_ScenPortGrid.GetTextMatrix(row,16);
	m_SelectedPortMainInfo->ship_loading_productivity.vegetables_fruits = (int)strtod(strNum,&stopstring);

	strNum = m_ScenPortGrid.GetTextMatrix(row,17);
	m_SelectedPortMainInfo->acv_loading_productivity.coal = (int)strtod(strNum,&stopstring);
	strNum = m_ScenPortGrid.GetTextMatrix(row,18);
	m_SelectedPortMainInfo->acv_loading_productivity.general = (int)strtod(strNum,&stopstring);
	strNum = m_ScenPortGrid.GetTextMatrix(row,19);
	m_SelectedPortMainInfo->acv_loading_productivity.oil = (int)strtod(strNum,&stopstring);
	strNum = m_ScenPortGrid.GetTextMatrix(row,20);
	m_SelectedPortMainInfo->acv_loading_productivity.vegetables_fruits = (int)strtod(strNum,&stopstring);

	////////////  Land Structures Info  /////////////
	strNum = m_ScenPortGrid.GetTextMatrix(row,21);
	m_SelectedPortLandStructInfo->m_nLandStructCalcActivateCheck = (int)strtod(strNum,&stopstring);
	strNum = m_ScenPortGrid.GetTextMatrix(row,22); 
	m_SelectedPortLandStructInfo->m_nACVCourtAvailableCheck = (int)strtod(strNum,&stopstring);
	strNum = m_ScenPortGrid.GetTextMatrix(row,23); 
	m_SelectedPortLandStructInfo->m_nACVCourtInvestPP = (int)strtod(strNum,&stopstring);
	strNum = m_ScenPortGrid.GetTextMatrix(row,24); 
	m_SelectedPortLandStructInfo->m_nShipCourtAvailableCheck = (int)strtod(strNum,&stopstring);
	strNum = m_ScenPortGrid.GetTextMatrix(row,25); 
	m_SelectedPortLandStructInfo->m_nShipCourtInvestPP = (int)strtod(strNum,&stopstring);

	strNum = m_ScenPortGrid.GetTextMatrix(row,26); 
	m_SelectedPortLandStructInfo->m_nACVSpace = (int)strtod(strNum,&stopstring);
	strNum = m_ScenPortGrid.GetTextMatrix(row,27); 
	m_SelectedPortLandStructInfo->m_nACVSpaceElemPrice = (int)strtod(strNum,&stopstring);
	strNum = m_ScenPortGrid.GetTextMatrix(row,28); 
	m_SelectedPortLandStructInfo->m_nMoorLength = (int)strtod(strNum,&stopstring);
	strNum = m_ScenPortGrid.GetTextMatrix(row,29); 
	m_SelectedPortLandStructInfo->m_nMoorElemPrice = (int)strtod(strNum,&stopstring);

	strNum = m_ScenPortGrid.GetTextMatrix(row,30); 
	m_SelectedPortLandStructInfo->m_structRepairDetailsACV.n = (int)strtod(strNum,&stopstring);
	strNum = m_ScenPortGrid.GetTextMatrix(row,31); 
	m_SelectedPortLandStructInfo->m_structRepairDetailsACV.Pgr = (int)strtod(strNum,&stopstring);
	strNum = m_ScenPortGrid.GetTextMatrix(row,32); 
	m_SelectedPortLandStructInfo->m_structRepairDetailsACV.a = (int)strtod(strNum,&stopstring);
	strNum = m_ScenPortGrid.GetTextMatrix(row,33); 
	m_SelectedPortLandStructInfo->m_structRepairDetailsACV.b = (int)strtod(strNum,&stopstring);

	strNum = m_ScenPortGrid.GetTextMatrix(row,34); 
	m_SelectedPortLandStructInfo->m_structRepairDetailsShip.n = (int)strtod(strNum,&stopstring);
	strNum = m_ScenPortGrid.GetTextMatrix(row,35); 
	m_SelectedPortLandStructInfo->m_structRepairDetailsShip.Pgr = (int)strtod(strNum,&stopstring);
	strNum = m_ScenPortGrid.GetTextMatrix(row,36); 
	m_SelectedPortLandStructInfo->m_structRepairDetailsShip.a = (int)strtod(strNum,&stopstring);
	strNum = m_ScenPortGrid.GetTextMatrix(row,37); 
	m_SelectedPortLandStructInfo->m_structRepairDetailsShip.b = (int)strtod(strNum,&stopstring);



}

void CScenarioWizardDlg::FillSelectedPortDBInfo()
{
	char *stopstring;
	CString strNum;
	int row = m_PortDBGrid.GetRow();

	m_SelectedPortMainInfo = new CPortMainInfo;
	m_SelectedPortLandStructInfo = new CPortLandStructInfo;

	strNum = m_PortDBGrid.GetTextMatrix(row,0);
	m_SelectedPortMainInfo->m_nPortID = (int)strtod(strNum,&stopstring);

	m_SelectedPortMainInfo->m_strPortName = m_PortDBGrid.GetTextMatrix(row,1);
	m_SelectedPortMainInfo->m_strPortType = m_PortDBGrid.GetTextMatrix(row,2);
	m_SelectedPortMainInfo->m_strBasePortName = m_PortDBGrid.GetTextMatrix(row,3);
	m_SelectedPortMainInfo->m_strPortRegion = m_PortDBGrid.GetTextMatrix(row,4);
	
	strNum = m_PortDBGrid.GetTextMatrix(row,5);
	m_SelectedPortMainInfo->required_cargoes.coal = (int)strtod(strNum,&stopstring);
	strNum = m_PortDBGrid.GetTextMatrix(row,6);
	m_SelectedPortMainInfo->required_cargoes.general = (int)strtod(strNum,&stopstring);
	strNum = m_PortDBGrid.GetTextMatrix(row,7);
	m_SelectedPortMainInfo->required_cargoes.oil = (int)strtod(strNum,&stopstring);
	strNum = m_PortDBGrid.GetTextMatrix(row,8);
	m_SelectedPortMainInfo->required_cargoes.vegetables_fruits = (int)strtod(strNum,&stopstring);
	strNum = m_PortDBGrid.GetTextMatrix(row,9);
	m_SelectedPortMainInfo->available_cargoes.coal = (int)strtod(strNum,&stopstring);
	strNum = m_PortDBGrid.GetTextMatrix(row,10);
	m_SelectedPortMainInfo->available_cargoes.general = (int)strtod(strNum,&stopstring);
	strNum = m_PortDBGrid.GetTextMatrix(row,11);
	m_SelectedPortMainInfo->available_cargoes.oil = (int)strtod(strNum,&stopstring);
	strNum = m_PortDBGrid.GetTextMatrix(row,12);
	m_SelectedPortMainInfo->available_cargoes.vegetables_fruits = (int)strtod(strNum,&stopstring);

	strNum = m_PortDBGrid.GetTextMatrix(row,13);
	m_SelectedPortMainInfo->ship_loading_productivity.coal = (int)strtod(strNum,&stopstring);
	strNum = m_PortDBGrid.GetTextMatrix(row,14);
	m_SelectedPortMainInfo->ship_loading_productivity.general = (int)strtod(strNum,&stopstring);
	strNum = m_PortDBGrid.GetTextMatrix(row,15);
	m_SelectedPortMainInfo->ship_loading_productivity.oil = (int)strtod(strNum,&stopstring);
	strNum = m_PortDBGrid.GetTextMatrix(row,16);
	m_SelectedPortMainInfo->ship_loading_productivity.vegetables_fruits = (int)strtod(strNum,&stopstring);

	strNum = m_PortDBGrid.GetTextMatrix(row,17);
	m_SelectedPortMainInfo->acv_loading_productivity.coal = (int)strtod(strNum,&stopstring);
	strNum = m_PortDBGrid.GetTextMatrix(row,18);
	m_SelectedPortMainInfo->acv_loading_productivity.general = (int)strtod(strNum,&stopstring);
	strNum = m_PortDBGrid.GetTextMatrix(row,19);
	m_SelectedPortMainInfo->acv_loading_productivity.oil = (int)strtod(strNum,&stopstring);
	strNum = m_PortDBGrid.GetTextMatrix(row,20);
	m_SelectedPortMainInfo->acv_loading_productivity.vegetables_fruits = (int)strtod(strNum,&stopstring);

	////////////  Land Structures Info  /////////////
	strNum = m_PortDBGrid.GetTextMatrix(row,21); 
	m_SelectedPortLandStructInfo->m_nLandStructCalcActivateCheck = (int)strtod(strNum,&stopstring);
	strNum = m_PortDBGrid.GetTextMatrix(row,22); 
	m_SelectedPortLandStructInfo->m_nACVCourtAvailableCheck = (int)strtod(strNum,&stopstring);
	strNum = m_PortDBGrid.GetTextMatrix(row,23); 
	m_SelectedPortLandStructInfo->m_nACVCourtInvestPP = (int)strtod(strNum,&stopstring);
	strNum = m_PortDBGrid.GetTextMatrix(row,24); 
	m_SelectedPortLandStructInfo->m_nShipCourtAvailableCheck = (int)strtod(strNum,&stopstring);
	strNum = m_PortDBGrid.GetTextMatrix(row,25); 
	m_SelectedPortLandStructInfo->m_nShipCourtInvestPP = (int)strtod(strNum,&stopstring);

	strNum = m_PortDBGrid.GetTextMatrix(row,26); 
	m_SelectedPortLandStructInfo->m_nACVSpace = (int)strtod(strNum,&stopstring);
	strNum = m_PortDBGrid.GetTextMatrix(row,27); 
	m_SelectedPortLandStructInfo->m_nACVSpaceElemPrice = (int)strtod(strNum,&stopstring);
	strNum = m_PortDBGrid.GetTextMatrix(row,28); 
	m_SelectedPortLandStructInfo->m_nMoorLength = (int)strtod(strNum,&stopstring);
	strNum = m_PortDBGrid.GetTextMatrix(row,29); 
	m_SelectedPortLandStructInfo->m_nMoorElemPrice = (int)strtod(strNum,&stopstring);

	strNum = m_PortDBGrid.GetTextMatrix(row,30); 
	m_SelectedPortLandStructInfo->m_structRepairDetailsACV.n = (int)strtod(strNum,&stopstring);
	strNum = m_PortDBGrid.GetTextMatrix(row,31); 
	m_SelectedPortLandStructInfo->m_structRepairDetailsACV.Pgr = (int)strtod(strNum,&stopstring);
	strNum = m_PortDBGrid.GetTextMatrix(row,32); 
	m_SelectedPortLandStructInfo->m_structRepairDetailsACV.a = (int)strtod(strNum,&stopstring);
	strNum = m_PortDBGrid.GetTextMatrix(row,33); 
	m_SelectedPortLandStructInfo->m_structRepairDetailsACV.b = (int)strtod(strNum,&stopstring);

	strNum = m_PortDBGrid.GetTextMatrix(row,34); 
	m_SelectedPortLandStructInfo->m_structRepairDetailsShip.n = (int)strtod(strNum,&stopstring);
	strNum = m_PortDBGrid.GetTextMatrix(row,35); 
	m_SelectedPortLandStructInfo->m_structRepairDetailsShip.Pgr = (int)strtod(strNum,&stopstring);
	strNum = m_PortDBGrid.GetTextMatrix(row,36); 
	m_SelectedPortLandStructInfo->m_structRepairDetailsShip.a = (int)strtod(strNum,&stopstring);
	strNum = m_PortDBGrid.GetTextMatrix(row,37); 
	m_SelectedPortLandStructInfo->m_structRepairDetailsShip.b = (int)strtod(strNum,&stopstring);

}



void CScenarioWizardDlg::OnGetPort() 
{
	CString str;
	int col;
	int row_db, // row in DB table
		row;	//row in scenatio table
		row_db = m_PortDBGrid.GetRow();
	
	int rows_num = m_ScenPortGrid.GetRows();
	
	//char strTmp[5];
	//_itoa(rows_num, strTmp, 10);
	//AfxMessageBox(strTmp);

	//Проверка существования пункта в сценарии
	int flDublicate = 0;
	CString str_in;
	str_in = m_PortDBGrid.GetTextMatrix(row_db,1);
	//AfxMessageBox(str_in);
	for (row = 1; row <= rows_num; row ++){
		str = m_ScenPortGrid.GetTextMatrix(row,1);
		//AfxMessageBox(str);
		if (strcmp(str, "") == 0)
			break;
		if (strcmp(str, str_in) == 0){
			flDublicate = 1;
			AfxMessageBox("Порт уже существует в сценарии.\nВыберите другой порт.");
			break;
		}
	}

    if (!flDublicate){
		//Нахождение номера строки для добавления записи пункта
		for (row = 1; row <= rows_num; row ++){
			str = m_ScenPortGrid.GetTextMatrix(row,2);
			//AfxMessageBox(str);
			if (strcmp(str, "") == 0)
				break;
		}

		for (col = 0; col <= 20; col++){
			str = m_PortDBGrid.GetTextMatrix(row_db,col);
			m_ScenPortGrid.SetTextMatrix(row,col,str);

		}
		for (col = 21; col <= 37; col++){ // Land Struct
			str = m_PortDBGrid.GetTextMatrix(row_db,col);
			m_ScenPortGrid.SetTextMatrix(row,col,str);

		}

	}
}

void CScenarioWizardDlg::SaveScenToFile()
{
	CFile f;
	CFileException e;
	CString str;

	//m_pScenarioFile = "ScenTest.fsc";
	
	f.Open( m_pScenarioFile, CFile::modeCreate | CFile::modeWrite, &e);
	CArchive arScen(&f, CArchive::store);
	
	str = "SHIPS DATA";
	arScen << str;
	SaveScenShipDataToFile(arScen);
	str = "PORTS DATA";
	arScen << str;
	SaveScenPortDataToFile(arScen);
	//str = "END OF SCENARIO FILE";
	str = "INVEST DATA";
	arScen << str;
	SaveScenInvestDataToFile(arScen);
	str = "END OF SCENARIO FILE";
	arScen << str;

	arScen.Close();
	//f.Close();

}

void CScenarioWizardDlg::SaveScenShipDataToFile(CArchive& ar)
{
	CString str;

	for (int i = 1; i <= m_ShipsDBRecordsNum; i++){
		str = m_ScenShipsGrid.GetTextMatrix(i,1);
		if (str == "")
			continue;
		for (int j = 1; j <= 33; j++){
			str = m_ScenShipsGrid.GetTextMatrix(i,j);
			ar << str;
		}
	}
}

void CScenarioWizardDlg::SaveScenPortDataToFile(CArchive &ar)
{
	CString str;

	for (int i = 1; i <= m_PortDBRecordsNum; i++){
		str = m_ScenPortGrid.GetTextMatrix(i,1);
		if (str == "")
			continue;
		for (int j = 0; j <= 20; j++){
			str = m_ScenPortGrid.GetTextMatrix(i,j);
			ar << str;
		}
		for (int j = 21; j <= 37; j++){ // Land Struct
			str = m_ScenPortGrid.GetTextMatrix(i,j);
			ar << str;
		}
	}
}

void CScenarioWizardDlg::OnScenFileSave() 
{
	//SaveScenToFile();

	if (m_pScenarioFile == "")
		OnScenFileSaveAs();
	else
		SaveScenToFile();
	
}


void CScenarioWizardDlg::OnScenFileSaveAs() 
{
	//CString CurrFileName =  m_pScenarioFile;
	CString CurrFileName = "";

	if(CurrFileName.Find(".fsc")== -1)
		CurrFileName  =  CurrFileName +".fsc";
	CFileDialog fileDialog(FALSE,NULL,CurrFileName,NULL,
							"Fleet Optimiser Scenario Files (*.fsc)|*.fsc|");

	if(fileDialog.DoModal()==IDOK)
	{
		CString Filename = fileDialog.GetPathName();
		if((Filename.Find(CurrFileName) == -1)&&( _access(Filename,00)!= -1 ))
		{
			CString TMPFilename = Filename;
	    
			CString messText;
			
			messText = TMPFilename;
			messText = messText + " уже существует.\nЗаменить файл?";
			int pushed;
	
			pushed = AfxMessageBox(messText,MB_ICONQUESTION|MB_YESNOCANCEL);
			if(pushed == IDCANCEL)
				return;
			else if(pushed == IDNO)
				{
					OnScenFileSaveAs();
					return;
				}
		}	


		////////////
		if(Filename.Find(".fsc")== -1)
			Filename  =  Filename +".fsc";

		//strcpy(m_pScenarioFile, Filename);
		//m_pScenarioFile = Filename.GetBuffer( 20 );
		m_pScenarioFile = strdup(Filename);
		SaveScenToFile();
	}

	
}

void CScenarioWizardDlg::OnScenFileLoad() 
{
	CFileDialog fileDialog(TRUE,NULL,NULL,NULL,
						"Fleet Optimiser Scenario Files (*.fsc)|*.fsc|All Files (*.*)|*.*|");											 
	
	if(fileDialog.DoModal()==IDOK)
	{		
		CString extension = fileDialog.GetFileExt();
		
		if(extension != "fsc")
			AfxMessageBox("Unknown file type\nPlease select *.fcs file");
		else
		{
			CString filename = fileDialog.GetPathName();
			//m_pScenarioFile = filename.GetBuffer( 20 );
			m_pScenarioFile = strdup(filename.GetBuffer(20)); 
		
			LoadScenarioData();
		}
	}
}

void CScenarioWizardDlg::LoadScenPortData(CArchive& ar)
{
		CString strPortID, strPortName, strPortType, strBasePortName, 
				strPortRegion, strTmpMatrixValue, strNum;


		int i = 1;
		int j, count;
//		char strRowNum[5];

		count = 0;
		while (count <= m_PortDBRecordsNum){

			ar >> strPortID;
			//if (strPortID == "END OF SCENARIO FILE")
			if ((strPortID == "INVEST DATA") || (strPortID == "END OF SCENARIO FILE")  )
				break;
			ar >> strPortName;
			ar >> strPortType;
			ar >> strBasePortName;
			ar >> strPortRegion;

		//	_itoa(i, strRowNum, 10);
		//	m_ScenPortGrid.SetTextMatrix(i,0,strRowNum);


			m_ScenPortGrid.SetTextMatrix(i,0,strPortID);
			m_ScenPortGrid.SetTextMatrix(i,1,strPortName);
			m_ScenPortGrid.SetTextMatrix(i,2,strPortType);
			m_ScenPortGrid.SetTextMatrix(i,3,strBasePortName);
			m_ScenPortGrid.SetTextMatrix(i,4,strPortRegion);
			
			//Cargoes set
			for (j = 5; j <= 20; j++){
				ar >> strNum;
				m_ScenPortGrid.SetTextMatrix(i,j,strNum);
			}
			//Land Struct
			for (j = 21; j <= 37; j++){
				ar >> strNum;
				m_ScenPortGrid.SetTextMatrix(i,j,strNum);
			}


			i++;
			count++;
		}//end of while (i <= m_RecordsNumber)

}



void CScenarioWizardDlg::LoadScenShipData(CArchive& ar)
{
	CString str;
	char strRowNum[5];
	int count = 0;
	int j, i = 1;

	ar >> str;

	while(count < 200){
		ar >> str;
		if (str == "PORTS DATA")
			break;
		else{
			_itoa(i, strRowNum, 10);
			m_ScenShipsGrid.SetTextMatrix(i,0,strRowNum);
			m_ScenShipsGrid.SetTextMatrix(i,1,str);
			for (j = 2; j <= 33; j++){
				ar >> str;
				m_ScenShipsGrid.SetTextMatrix(i,j,str);
			}
			count++;
			i++;
		}
	}

}

void CScenarioWizardDlg::LoadScenarioData()
{
	CFile f( m_pScenarioFile, CFile::modeRead );
	f.SeekToBegin();
	CArchive ar(&f, CArchive::load);

	ClearScenShipsGrid();
	ClearScenPortsGrid();

	LoadScenShipData(ar);
	LoadScenPortData(ar);
	LoadScenInvestData(ar);

	ar.Close();
	f.Close();
}

void CScenarioWizardDlg::FillShipDBGrid()
{
		CString	strNum,	strShipTypeName, strRegisterClass, strProtoType,
			strDW, strL, strB, strT, strSpeed, strRange, strh3, strCargoCompatiblGeneral,
			strCargoCompatiblOil, strCargoCompatiblCoal, strCargoCompatiblVegFruits,
			strShipLoadMin, strShipLoadMax, strShipLoadDelta,
			strTariffGenVF, strTariffOil, strTariffCoal,
			strShipExistDesignCheckBox,	strShipExistBuildCheckBox, strShipExistCheckBox,strShipExistSetCheckBox,strShipExistForceCheckBox,
			strShipExistNumberComboBox, strShipExistAdvantPrice, strShipExistPrice, 
			strShipExistInBuildNumberComboBox, strShipExistBuildAdvantPrice, strShipExistBuildPrice,
			strShipExistDesignPrice, strShipExistDesignAdvantPrice;

		ULONGLONG positionEnd, dwPosition;
		
		CFile f( m_pShipsDBFile, CFile::modeRead );
		CArchive ar(&f, CArchive::load);

		f.SeekToEnd();
		positionEnd = f.GetPosition();
		f.SeekToBegin();
		dwPosition = f.GetPosition();
		
		if (positionEnd == 0)
			m_ShipsDBRecordsNum = 0;
		else {
				ar >> strNum;
				char *stopstring;
				m_ShipsDBRecordsNum  = (int)strtod(strNum,&stopstring);
		}

		int i = 1;
//		int j;
		char strRowNum[5];
   
		while (i <= m_ShipsDBRecordsNum){
			ar >> strShipTypeName;
			ar >> strRegisterClass;
			ar >> strProtoType;
			ar >> strDW;
			ar >> strL;
			ar >> strB;
			ar >> strT;
			ar >> strSpeed;
			ar >> strRange;
			ar >> strh3;

			ar >> strCargoCompatiblGeneral;
			ar >> strCargoCompatiblOil;
			ar >> strCargoCompatiblCoal;
			ar >> strCargoCompatiblVegFruits;

			ar >> strShipLoadMin;
			ar >> strShipLoadMax;
			ar >> strShipLoadDelta;
			ar >> strTariffGenVF;
			ar >> strTariffOil;
			ar >> strTariffCoal;

			ar >> strShipExistSetCheckBox;
			ar >> strShipExistForceCheckBox;
			ar >> strShipExistCheckBox;
			ar >> strShipExistBuildCheckBox;
			ar >> strShipExistDesignCheckBox;
			ar >> strShipExistNumberComboBox;
			ar >> strShipExistAdvantPrice;
			ar >> strShipExistPrice;
			ar >> strShipExistInBuildNumberComboBox;
			ar >> strShipExistBuildAdvantPrice;
			ar >> strShipExistBuildPrice;
			ar >> strShipExistDesignAdvantPrice;
			ar >> strShipExistDesignPrice;




			_itoa(i, strRowNum, 10);
			m_ShipsDBGrid.SetTextMatrix(i,0,strRowNum);

			m_ShipsDBGrid.SetTextMatrix(i,1,strShipTypeName);
			m_ShipsDBGrid.SetTextMatrix(i,2,strRegisterClass);
			m_ShipsDBGrid.SetTextMatrix(i,3,strProtoType);
			m_ShipsDBGrid.SetTextMatrix(i,4,strDW);
			m_ShipsDBGrid.SetTextMatrix(i,5,strL);
			m_ShipsDBGrid.SetTextMatrix(i,6,strB);
			m_ShipsDBGrid.SetTextMatrix(i,7,strT);
			m_ShipsDBGrid.SetTextMatrix(i,8,strCargoCompatiblGeneral);
			m_ShipsDBGrid.SetTextMatrix(i,9,strCargoCompatiblOil);
			m_ShipsDBGrid.SetTextMatrix(i,10,strCargoCompatiblCoal);
			m_ShipsDBGrid.SetTextMatrix(i,11,strCargoCompatiblVegFruits);
			m_ShipsDBGrid.SetTextMatrix(i,12,strShipLoadMin);
			m_ShipsDBGrid.SetTextMatrix(i,13,strShipLoadMax);
			m_ShipsDBGrid.SetTextMatrix(i,14,strShipLoadDelta);
			m_ShipsDBGrid.SetTextMatrix(i,15,strTariffGenVF);
			m_ShipsDBGrid.SetTextMatrix(i,16,strTariffOil);
			m_ShipsDBGrid.SetTextMatrix(i,17,strTariffCoal);
			m_ShipsDBGrid.SetTextMatrix(i,18,strSpeed);
			m_ShipsDBGrid.SetTextMatrix(i,19,strRange);
			m_ShipsDBGrid.SetTextMatrix(i,20,strh3);
			//begin:  ship exist 
			m_ShipsDBGrid.SetTextMatrix(i,21,strShipExistSetCheckBox);
			m_ShipsDBGrid.SetTextMatrix(i,22,strShipExistForceCheckBox);
			m_ShipsDBGrid.SetTextMatrix(i,23,strShipExistCheckBox);
			m_ShipsDBGrid.SetTextMatrix(i,24,strShipExistBuildCheckBox);
			m_ShipsDBGrid.SetTextMatrix(i,25,strShipExistDesignCheckBox);
			m_ShipsDBGrid.SetTextMatrix(i,26,strShipExistNumberComboBox);
			m_ShipsDBGrid.SetTextMatrix(i,27,strShipExistAdvantPrice);
			m_ShipsDBGrid.SetTextMatrix(i,28,strShipExistPrice);
			m_ShipsDBGrid.SetTextMatrix(i,29,strShipExistInBuildNumberComboBox);
			m_ShipsDBGrid.SetTextMatrix(i,30,strShipExistBuildAdvantPrice);
			m_ShipsDBGrid.SetTextMatrix(i,31,strShipExistBuildPrice);
			m_ShipsDBGrid.SetTextMatrix(i,32,strShipExistDesignAdvantPrice);
			m_ShipsDBGrid.SetTextMatrix(i,33,strShipExistDesignPrice);
			//end:  ship exist
			
			i++;
		}//end of while (i <= m_RecordsNumber)
	ar.Close();
	f.Close();
}

void CScenarioWizardDlg::OnGetShip() 
{
	CString str;
	int col;
	int row_db, // row in DB table
		row;	//row in scenatio table
		row_db = m_ShipsDBGrid.GetRow();
	
	int rows_num = m_ScenShipsGrid.GetRows();
	

	//Проверка существования судна в сценарии
	int flDublicate = 0;
	CString str_in;
	str_in = m_ShipsDBGrid.GetTextMatrix(row_db,1);
	//AfxMessageBox(str_in);
	for (row = 1; row <= rows_num; row ++){
		str = m_ScenShipsGrid.GetTextMatrix(row,1);
		//AfxMessageBox(str);
		if (strcmp(str, "") == 0)
			break;
		if (strcmp(str, str_in) == 0){
			flDublicate = 1;
			AfxMessageBox("Судно уже существует в сценарии.\nВыберите другое судно.");
			break;
		}
	}

    if (!flDublicate){
		//Нахождение номера строки для добавления записи
		for (row = 1; row <= rows_num; row ++){
			str = m_ScenShipsGrid.GetTextMatrix(row,2);
			//AfxMessageBox(str);
			if (strcmp(str, "") == 0)
				break;
		}

		for (col = 1; col <= 33; col++){
			str = m_ShipsDBGrid.GetTextMatrix(row_db,col);
			m_ScenShipsGrid.SetTextMatrix(row,col,str);

		}
	}
	
}

void CScenarioWizardDlg::OnScenarioShipView() 
{

	CShipTypePropertySheet thePSheet(_T("Ship Details"),this,NULL);
	FillSelectedShipScenInfo();
	thePSheet.m_SelectedShipMainInfo = m_SelectedShipMainInfo;
	thePSheet.m_OpenStat = 1;
	thePSheet.m_CShipTypeMainPPage.m_OpenStat = 1;
	thePSheet.m_CShipTypeOptimizParamPPage.m_OpenStat = 1;
	thePSheet.DoModal();	
}

void CScenarioWizardDlg::FillSelectedShipScenInfo()
{
	char *stopstring;
	CString strNum;
	int row = m_ScenShipsGrid.GetRow();

	m_SelectedShipMainInfo = new CShipMainInfo;

	m_SelectedShipMainInfo->m_strlShipTypeName = m_ScenShipsGrid.GetTextMatrix(row,1);
	m_SelectedShipMainInfo->m_strShipRegisterClass = m_ScenShipsGrid.GetTextMatrix(row,2);
	m_SelectedShipMainInfo->m_strPrototypeName = m_ScenShipsGrid.GetTextMatrix(row,3);
	
	strNum = m_ScenShipsGrid.GetTextMatrix(row,4);
	m_SelectedShipMainInfo->m_nDW = (int)strtod(strNum,&stopstring);

	strNum = m_ScenShipsGrid.GetTextMatrix(row,5);
	m_SelectedShipMainInfo->m_nL = (int)strtod(strNum,&stopstring);

	strNum = m_ScenShipsGrid.GetTextMatrix(row,6);
	m_SelectedShipMainInfo->m_nB = (int)strtod(strNum,&stopstring);

	strNum = m_ScenShipsGrid.GetTextMatrix(row,7);
	m_SelectedShipMainInfo->m_nT = (int)strtod(strNum,&stopstring);

	strNum = m_ScenShipsGrid.GetTextMatrix(row,8);
	m_SelectedShipMainInfo->m_nCargoCompatiblGeneral = (int)strtod(strNum,&stopstring);

	strNum = m_ScenShipsGrid.GetTextMatrix(row,9);
	m_SelectedShipMainInfo->m_nCargoCompatiblOil = (int)strtod(strNum,&stopstring);

	strNum = m_ScenShipsGrid.GetTextMatrix(row,10);
	m_SelectedShipMainInfo->m_nCargoCompatiblCoal = (int)strtod(strNum,&stopstring);

	strNum = m_ScenShipsGrid.GetTextMatrix(row,11);
	m_SelectedShipMainInfo->m_nCargoCompatibleVegFruits = (int)strtod(strNum,&stopstring);

	strNum = m_ScenShipsGrid.GetTextMatrix(row,12);
	m_SelectedShipMainInfo->m_nLoadMin = (int)strtod(strNum,&stopstring);

	strNum = m_ScenShipsGrid.GetTextMatrix(row,13);
	m_SelectedShipMainInfo->m_nLoadMax = (int)strtod(strNum,&stopstring);

	strNum = m_ScenShipsGrid.GetTextMatrix(row,14);
	m_SelectedShipMainInfo->m_nLoadDelta = (int)strtod(strNum,&stopstring);

	strNum = m_ScenShipsGrid.GetTextMatrix(row,15);
	m_SelectedShipMainInfo->m_nTariffGenVF = (int)strtod(strNum,&stopstring);

	strNum = m_ScenShipsGrid.GetTextMatrix(row,16);
	m_SelectedShipMainInfo->m_nTariffOil = (int)strtod(strNum,&stopstring);

	strNum = m_ScenShipsGrid.GetTextMatrix(row,17);
	m_SelectedShipMainInfo->m_nTariffCoal = (int)strtod(strNum,&stopstring);

	strNum = m_ScenShipsGrid.GetTextMatrix(row,18);
	m_SelectedShipMainInfo->m_nSpeed = (int)strtod(strNum,&stopstring);

	strNum = m_ScenShipsGrid.GetTextMatrix(row,19);
	m_SelectedShipMainInfo->m_nRange = (int)strtod(strNum,&stopstring);

	strNum = m_ScenShipsGrid.GetTextMatrix(row,20);
	m_SelectedShipMainInfo->m_nh3 = (float)strtod(strNum,&stopstring);

	//begin: ship exist
	strNum = m_ScenShipsGrid.GetTextMatrix(row,21);
	m_SelectedShipMainInfo->m_nShipExistSetCheck = (int)strtod(strNum,&stopstring);

	strNum = m_ScenShipsGrid.GetTextMatrix(row,22);
	m_SelectedShipMainInfo->m_nShipExistForceCheck = (int)strtod(strNum,&stopstring);

	strNum = m_ScenShipsGrid.GetTextMatrix(row,23);
	m_SelectedShipMainInfo->m_nShipExistCheck = (int)strtod(strNum,&stopstring);

	strNum = m_ScenShipsGrid.GetTextMatrix(row,24);
	m_SelectedShipMainInfo->m_nShipExistBuildCheck = (int)strtod(strNum,&stopstring);

	strNum = m_ScenShipsGrid.GetTextMatrix(row,25);
	m_SelectedShipMainInfo->m_nShipExistDesignCheck = (int)strtod(strNum,&stopstring);

	strNum = m_ScenShipsGrid.GetTextMatrix(row,26);
	m_SelectedShipMainInfo->m_nShipExistNum = (int)strtod(strNum,&stopstring);
	strNum = m_ScenShipsGrid.GetTextMatrix(row,27);
	m_SelectedShipMainInfo->m_nShipExistAdvantPrice = (int)strtod(strNum,&stopstring);
	strNum = m_ScenShipsGrid.GetTextMatrix(row,28);
	m_SelectedShipMainInfo->m_nShipExistPrice = (int)strtod(strNum,&stopstring);

	strNum = m_ScenShipsGrid.GetTextMatrix(row,29);
	m_SelectedShipMainInfo->m_nShipExistBuildNum = (int)strtod(strNum,&stopstring);
	strNum = m_ScenShipsGrid.GetTextMatrix(row,30);
	m_SelectedShipMainInfo->m_nShipExistBuildAdvantPrice = (int)strtod(strNum,&stopstring);
	strNum = m_ScenShipsGrid.GetTextMatrix(row,31);
	m_SelectedShipMainInfo->m_nShipExistBuildPrice = (int)strtod(strNum,&stopstring);

	strNum = m_ScenShipsGrid.GetTextMatrix(row,32);
	m_SelectedShipMainInfo->m_nShipExistDesignAdvantPrice = (int)strtod(strNum,&stopstring);
	strNum = m_ScenShipsGrid.GetTextMatrix(row,33);
	m_SelectedShipMainInfo->m_nShipExistDesignPrice = (int)strtod(strNum,&stopstring);

	//end: ship exit
}

void CScenarioWizardDlg::OnScenarioRefreshButton() 
{
	CleanShipDBGrid();
	CleanPortDBGrid();
	FillPortDBGrid();
	FillShipDBGrid();
	FillInvestInfo();

	if (m_flagEditPort == 1){
		CString str;
		for (int i = 0; i <= 20; i++){
			str = m_PortDBGrid.GetTextMatrix(m_nEditPortID, i); 
			m_ScenPortGrid.SetTextMatrix(m_nEditPortNum,i, str); 
		}
		for (int i = 21; i <= 37; i++){ //Land Struct
			str = m_PortDBGrid.GetTextMatrix(m_nEditPortID, i); 
			m_ScenPortGrid.SetTextMatrix(m_nEditPortNum,i, str); 
		}
		m_flagEditPort = 0;
	}
	if (m_flagEditShip == 1){
		CString str;
		for (int col = 1; col <= 33; col++){
			str = m_ShipsDBGrid.GetTextMatrix(m_nEditShipID, col); 
			m_ScenShipsGrid.SetTextMatrix(m_nEditShipNum,col, str); 
		}		
		m_flagEditShip = 0;
	}

}

void CScenarioWizardDlg::CleanPortDBGrid()
{

}

void CScenarioWizardDlg::CleanShipDBGrid()
{
	int i, j, max;

	max = m_ShipsDBRecordsNum + 1;
	for (i = 1; i <= max; i++)
		for (j = 0; j <= 33; j++)
			m_ShipsDBGrid.SetTextMatrix(i,j,"");

}

void CScenarioWizardDlg::OnOK()
{
	if (m_pScenarioFile != "")
		CopyScenarioFileToDefault();

	((CFleetOptimizerApp*)AfxGetApp())->WriteProfileInt("FleetOptimizer","PortsNumberInDB",m_PortDBRecordsNum);	


	CDialog::OnOK();
}

void CScenarioWizardDlg::CopyScenarioFileToDefault()
{

	int fh_read, fh_write;
	unsigned int fileSize, nbytes = 1, bytesread, byteswritten;
	unsigned int read_tmp, write_tmp;
	char buffer[1];
	bytesread = 0;
	byteswritten = 0;


	fh_read = open( m_pScenarioFile, O_RDONLY|O_BINARY );
	if (fh_read == -1){
		AfxMessageBox("Ошибка открытия файла сценария для чтения.\nCScenarioWizardDlg::CopyScenarioFileToDefault()");
		return;
	}

	fileSize = lseek(fh_read,0,SEEK_END);
	lseek(fh_read,0,SEEK_SET);

	chmod(m_pScenDefaultFile,S_IWRITE|S_IREAD);
	remove(m_pScenDefaultFile);
	fh_write = open( m_pScenDefaultFile,O_CREAT|O_BINARY|O_WRONLY |S_IWRITE );

	if (fh_write == -1){
		AfxMessageBox("Ошибка открытия файла сценария (default) для записи.\nCScenarioWizardDlg::CopyScenarioFileToDefault()");
		return;
	}

	while(1) {
		if (bytesread + nbytes > fileSize)
			break;

		read_tmp = read(fh_read,buffer,nbytes);
		write_tmp = write( fh_write, buffer, sizeof( buffer ));
		bytesread += read_tmp;
		byteswritten += write_tmp;
	}
	close( fh_read );
	close( fh_write );

}

void CScenarioWizardDlg::LoadDefaultScenario()
{
// The default scenario: latest opened scenario if OK button was choosed to close dialog.
// If Cancell was pressed to close the dialog - previouse scenario remains as default.

	CFile f( m_pScenDefaultFile, CFile::modeRead );
	f.SeekToBegin();
	CArchive ar(&f, CArchive::load);

	LoadScenShipData(ar);
	LoadScenPortData(ar);

	ar.Close();
	f.Close();
}

void CScenarioWizardDlg::SetTitlesShipsGrid()
{
			//// Scenario Grid /////
			m_ScenShipsGrid.SetTextMatrix(0,1,"Ship/ACV Type");
			m_ScenShipsGrid.SetTextMatrix(0,2,"Classification");
			m_ScenShipsGrid.SetTextMatrix(0,3,"Prototype");
			m_ScenShipsGrid.SetTextMatrix(0,4,"   DW");
			m_ScenShipsGrid.SetTextMatrix(0,5,"Length");
			m_ScenShipsGrid.SetTextMatrix(0,6,"Beam/Width");
			m_ScenShipsGrid.SetTextMatrix(0,7,"Draft");
			m_ScenShipsGrid.SetTextMatrix(0,8,"General cargo - compartibility");
			m_ScenShipsGrid.SetTextMatrix(0,9,"Oil - compartibility");
			m_ScenShipsGrid.SetTextMatrix(0,10,"Coal - compartibility");
			m_ScenShipsGrid.SetTextMatrix(0,11,"Veg-froots. - compartibility");
			m_ScenShipsGrid.SetTextMatrix(0,12,"Payload min");
			m_ScenShipsGrid.SetTextMatrix(0,13,"Payload max");
			m_ScenShipsGrid.SetTextMatrix(0,14,"Payload step");
			m_ScenShipsGrid.SetTextMatrix(0,15,"Rates - General cargo and Veg-froots");
			m_ScenShipsGrid.SetTextMatrix(0,16,"Rates - Oil");
			m_ScenShipsGrid.SetTextMatrix(0,17,"Rates - Coal");
			m_ScenShipsGrid.SetTextMatrix(0,18,"Speed, kn");
			m_ScenShipsGrid.SetTextMatrix(0,19,"Range, miles");
			m_ScenShipsGrid.SetTextMatrix(0,20,"h 3%");
			//begin:  ship exist (set titles)
			m_ScenShipsGrid.SetTextMatrix(0,21,"Ship exists (0,1)");
			m_ScenShipsGrid.SetTextMatrix(0,22,"Mandatory inclusion in the plan(0,1)");
			m_ScenShipsGrid.SetTextMatrix(0,23,"In Operation (0,1)");
			m_ScenShipsGrid.SetTextMatrix(0,24,"In Build (0,1)");
			m_ScenShipsGrid.SetTextMatrix(0,25,"In Design (0,1)");
			m_ScenShipsGrid.SetTextMatrix(0,26,"Number of ships in operation");
			m_ScenShipsGrid.SetTextMatrix(0,27,"Advantage (USD) of the ship in operation");
			m_ScenShipsGrid.SetTextMatrix(0,28,"Cost (USD) of the ship in operation");
			m_ScenShipsGrid.SetTextMatrix(0,29,"Number of ships in Build");
			m_ScenShipsGrid.SetTextMatrix(0,30,"Advantage (USD) of the ship in Build");
			m_ScenShipsGrid.SetTextMatrix(0,31,"Cost (USD) of the ship in Build");
			m_ScenShipsGrid.SetTextMatrix(0,32,"Advantage (USD) of the ship in Design");
			m_ScenShipsGrid.SetTextMatrix(0,33,"Cost (USD) of the ship in Design");
			//end:  ship exist (set titles)
			
			int rowsCount = m_ScenShipsGrid.GetRows();
			char strRowNum[5];
			for (int i = 1; i < rowsCount; i++){
				_itoa(i, strRowNum, 10);
				m_ScenShipsGrid.SetTextMatrix(i,0,strRowNum);
			}


			////////  DB Grid //////////
			m_ShipsDBGrid.SetTextMatrix(0,1,"Ship Type");
			m_ShipsDBGrid.SetTextMatrix(0,2,"Classification");
			m_ShipsDBGrid.SetTextMatrix(0,3,"Prototype");
			m_ShipsDBGrid.SetTextMatrix(0,4,"   DW");
			m_ShipsDBGrid.SetTextMatrix(0,5,"Length");
			m_ShipsDBGrid.SetTextMatrix(0,6,"Beam/Width");
			m_ShipsDBGrid.SetTextMatrix(0,7,"Draft");
			m_ShipsDBGrid.SetTextMatrix(0,8,"General cargo - compartibility");
			m_ShipsDBGrid.SetTextMatrix(0,9,"Oil - compartibility");
			m_ShipsDBGrid.SetTextMatrix(0,10,"Coal - compartibility");
			m_ShipsDBGrid.SetTextMatrix(0,11,"Veg-froots. - compartibility");
			m_ShipsDBGrid.SetTextMatrix(0,12,"Payload min");
			m_ShipsDBGrid.SetTextMatrix(0,13,"Payload max");
			m_ShipsDBGrid.SetTextMatrix(0,14,"Payload step");
			m_ShipsDBGrid.SetTextMatrix(0,15,"Rates - General cargo and Veg-froots");
			m_ShipsDBGrid.SetTextMatrix(0,16,"Rates - Oil");
			m_ShipsDBGrid.SetTextMatrix(0,17,"Rates - Coal");
			m_ShipsDBGrid.SetTextMatrix(0,18,"Speed, kn");
			m_ShipsDBGrid.SetTextMatrix(0,19,"Range, miles");
			m_ShipsDBGrid.SetTextMatrix(0,20,"h 3%");
			//begin:  ship exist (set titles)
			m_ShipsDBGrid.SetTextMatrix(0,21,"Ship exists (0,1)");
			m_ShipsDBGrid.SetTextMatrix(0,22,"Mandatory inclusion in the plan (0,1)");
			m_ShipsDBGrid.SetTextMatrix(0,23,"In Operation (0,1)");
			m_ShipsDBGrid.SetTextMatrix(0,24,"In Build (0,1)");
			m_ShipsDBGrid.SetTextMatrix(0,25,"In Design (0,1)");
			m_ShipsDBGrid.SetTextMatrix(0,26,"Number of ships in operation");
			m_ShipsDBGrid.SetTextMatrix(0,27,"Advantage (USD) of the ship in operation");
			m_ShipsDBGrid.SetTextMatrix(0,28,"Cost (USD) of the ship in operation");
			m_ShipsDBGrid.SetTextMatrix(0,29,"Number of ships in Build");
			m_ShipsDBGrid.SetTextMatrix(0,30,"Advantage (USD) of the ship in Build");
			m_ShipsDBGrid.SetTextMatrix(0,31,"Cost (USD) of the ship in Build");
			m_ShipsDBGrid.SetTextMatrix(0,32,"Advantage (USD) of the ship in Design");
			m_ShipsDBGrid.SetTextMatrix(0,33,"Cost (USD) of the ship in Design");
			//end:  ship exist (set titles)
			rowsCount = m_ShipsDBGrid.GetRows();
			for (int i = 1; i < rowsCount; i++){
				_itoa(i, strRowNum, 10);
				m_ShipsDBGrid.SetTextMatrix(i,0,strRowNum);
			}

}

void CScenarioWizardDlg::OnScenarioShipDeleteButton() 
{
	int SelectedRow = 	m_ScenShipsGrid.GetRow();
	int RowsNum = 198; //m_ScenShipsGrid.GetRows();
	int ColNum = 32; // m_ScenShipsGrid.GetCol();
	CString strArray[199][33];
	int row, col;

	
	if(SelectedRow == RowsNum)
	{
		for (col =1; col < ColNum; col++){
			m_ScenShipsGrid.SetTextMatrix(RowsNum,col,"");
		}
	}
	else
	{

		for (row = 1; row < RowsNum; row ++){
			for (col =1; col < ColNum; col++)
				strArray[row][col] = m_ScenShipsGrid.GetTextMatrix(row,col);	
		}
		//m_ScenShipsGrid.Clear();

		for (row = 1; row < RowsNum; row ++){
			if (row == SelectedRow)
				break;
			for (col =1; col < ColNum; col++){
				m_ScenShipsGrid.SetTextMatrix(row,col,strArray[row][col]);
			}	
		}
		for (row = SelectedRow; row < RowsNum; row ++){
			for (col =1; col < ColNum; col++)
				m_ScenShipsGrid.SetTextMatrix(row,col,strArray[row+1][col]);
		}
		for (col =1; col < ColNum; col++){
				m_ScenShipsGrid.SetTextMatrix(RowsNum,col,"");
		}
	}

}

void CScenarioWizardDlg::SetTitlesPortGrids()
{
	int rowsCount, i;
	char strRowNum[5];

/*	rowsCount = m_ScenPortGrid.GetRows();
	for (i = 1; i < rowsCount; i++){
		_itoa(i, strRowNum, 10);
		m_ScenPortGrid.SetTextMatrix(i,0,strRowNum);
	}
*/

	rowsCount = m_PortDBGrid.GetRows();
	for (i = 1; i < rowsCount; i++){
		_itoa(i, strRowNum, 10);
		m_PortDBGrid.SetTextMatrix(i,0,strRowNum);
	}
/*
	m_PortDBGrid.SetTextMatrix(0,1,"Port Name");
	m_PortDBGrid.SetTextMatrix(0,2,"Port Type");
	m_PortDBGrid.SetTextMatrix(0,3,"BasePort Name");
	m_PortDBGrid.SetTextMatrix(0,4,"Port Region");
	
	m_ScenPortGrid.SetTextMatrix(0,1,"Port Name");
	m_ScenPortGrid.SetTextMatrix(0,2,"Port Type");
	m_ScenPortGrid.SetTextMatrix(0,3,"BasePort Name");
	m_ScenPortGrid.SetTextMatrix(0,4,"Port Region");
*/


	m_PortDBGrid.SetTextMatrix(0,1,_T("Name"));
	m_PortDBGrid.SetTextMatrix(0,2,_T("Type"));
	m_PortDBGrid.SetTextMatrix(0,3,_T("Base Port"));
	m_PortDBGrid.SetTextMatrix(0,4,_T("Region"));
	m_PortDBGrid.SetTextMatrix(0,5,_T("Coal-required, t"));
	m_PortDBGrid.SetTextMatrix(0,6,_T("General-required, t"));
	m_PortDBGrid.SetTextMatrix(0,7,_T("Oil-required, t"));
	m_PortDBGrid.SetTextMatrix(0,8,_T("Veg-froots-required, t"));
	m_PortDBGrid.SetTextMatrix(0,9,_T("Coal-available, t"));
	m_PortDBGrid.SetTextMatrix(0,10,_T("General-available, t"));
	m_PortDBGrid.SetTextMatrix(0,11,_T("Oil-available, t"));
	m_PortDBGrid.SetTextMatrix(0,12,_T("Veg-froots-available, t"));
	m_PortDBGrid.SetTextMatrix(0,13,_T("Coal. Ship Load/Unload t/h"));
	m_PortDBGrid.SetTextMatrix(0,14,_T("General. Ship Load/Unload t/h"));
	m_PortDBGrid.SetTextMatrix(0,15,_T("Oil. Ship Load/Unload t/h"));
	m_PortDBGrid.SetTextMatrix(0,16,_T("Veg-froots. Ship Load/Unload t/h"));
	m_PortDBGrid.SetTextMatrix(0,17,_T("Coal. ACV Load/Unload t/h"));
	m_PortDBGrid.SetTextMatrix(0,18,_T("General. ACV Load/Unload t/h"));
	m_PortDBGrid.SetTextMatrix(0,19,_T("Oil. ACV Load/Unload t/h"));
	m_PortDBGrid.SetTextMatrix(0,20,_T("Veg-froots. ACV Load/Unload t/h"));
	//Land Struct begin
	m_PortDBGrid.SetTextMatrix(0,21,_T("Consider OnShorel infrastructure(0,1)"));
	m_PortDBGrid.SetTextMatrix(0,22,_T("PortPoint Onshore area for ACV exists(0,1)"));
	m_PortDBGrid.SetTextMatrix(0,23,_T("PortPoint Onshore area for ACV: inbestments required"));
	m_PortDBGrid.SetTextMatrix(0,24,_T("PortPoint Onshore area for Ships with ramp exists0,1)"));
	m_PortDBGrid.SetTextMatrix(0,25,_T("PortPoint Onshore area for Ships with ramp: inbestments required"));
	m_PortDBGrid.SetTextMatrix(0,26,_T("Size of ACV Onshore area in Base Port"));
	m_PortDBGrid.SetTextMatrix(0,27,_T("Cost of 100 m2 of ACV Onshore area in Base Port"));
	m_PortDBGrid.SetTextMatrix(0,28,_T("Berth length (existing), m"));
	m_PortDBGrid.SetTextMatrix(0,29,_T("Cost of 10 m of Berth"));
	m_PortDBGrid.SetTextMatrix(0,30,_T("ACV Repair Base n"));
	m_PortDBGrid.SetTextMatrix(0,31,_T("ACV Repair Base Pgr"));
	m_PortDBGrid.SetTextMatrix(0,32,_T("ACV Repair Base a"));
	m_PortDBGrid.SetTextMatrix(0,33,_T("ACV Repair Base b"));
	m_PortDBGrid.SetTextMatrix(0,34,_T("Ship Repair Base n"));
	m_PortDBGrid.SetTextMatrix(0,35,_T("Ship Repair Base Pgr"));
	m_PortDBGrid.SetTextMatrix(0,36,_T("Ship Repair Base a"));
	m_PortDBGrid.SetTextMatrix(0,37,_T("Ship Repair Base b"));
	//Land Struct end

	m_ScenPortGrid.SetTextMatrix(0, 1, _T("Name"));
	m_ScenPortGrid.SetTextMatrix(0, 2, _T("Type"));
	m_ScenPortGrid.SetTextMatrix(0, 3, _T("Base Port"));
	m_ScenPortGrid.SetTextMatrix(0, 4, _T("Region"));
	m_ScenPortGrid.SetTextMatrix(0, 5, _T("Coal-required, t"));
	m_ScenPortGrid.SetTextMatrix(0, 6, _T("General-required, t"));
	m_ScenPortGrid.SetTextMatrix(0, 7, _T("Oil-required, t"));
	m_ScenPortGrid.SetTextMatrix(0, 8, _T("Veg-froots-required, t"));
	m_ScenPortGrid.SetTextMatrix(0, 9, _T("Coal-available, t"));
	m_ScenPortGrid.SetTextMatrix(0, 10, _T("General-available, t"));
	m_ScenPortGrid.SetTextMatrix(0, 11, _T("Oil-available, t"));
	m_ScenPortGrid.SetTextMatrix(0, 12, _T("Veg-froots-available, t"));
	m_ScenPortGrid.SetTextMatrix(0, 13, _T("Coal. Ship Load/Unload t/h"));
	m_ScenPortGrid.SetTextMatrix(0, 14, _T("General. Ship Load/Unload t/h"));
	m_ScenPortGrid.SetTextMatrix(0, 15, _T("Oil. Ship Load/Unload t/h"));
	m_ScenPortGrid.SetTextMatrix(0, 16, _T("Veg-froots. Ship Load/Unload t/h"));
	m_ScenPortGrid.SetTextMatrix(0, 17, _T("Coal. ACV Load/Unload t/h"));
	m_ScenPortGrid.SetTextMatrix(0, 18, _T("General. ACV Load/Unload t/h"));
	m_ScenPortGrid.SetTextMatrix(0, 19, _T("Oil. ACV Load/Unload t/h"));
	m_ScenPortGrid.SetTextMatrix(0, 20, _T("Veg-froots. ACV Load/Unload t/h"));
	//Land Struct begin
	m_ScenPortGrid.SetTextMatrix(0, 21, _T("Consider OnShorel infrastructure(0,1)"));
	m_ScenPortGrid.SetTextMatrix(0, 22, _T("PortPoint Onshore area for ACV exists(0,1)"));
	m_ScenPortGrid.SetTextMatrix(0, 23, _T("PortPoint Onshore area for ACV: inbestments required"));
	m_ScenPortGrid.SetTextMatrix(0, 24, _T("PortPoint Onshore area for Ships with ramp exists0,1)"));
	m_ScenPortGrid.SetTextMatrix(0, 25, _T("PortPoint Onshore area for Ships with ramp: inbestments required"));
	m_ScenPortGrid.SetTextMatrix(0, 26, _T("Size of ACV Onshore area in Base Port"));
	m_ScenPortGrid.SetTextMatrix(0, 27, _T("Cost of 100 m2 of ACV Onshore area in Base Port"));
	m_ScenPortGrid.SetTextMatrix(0, 28, _T("Berth length (existing), m"));
	m_ScenPortGrid.SetTextMatrix(0, 29, _T("Cost of 10 m of Berth"));
	m_ScenPortGrid.SetTextMatrix(0, 30, _T("ACV Repair Base n"));
	m_ScenPortGrid.SetTextMatrix(0, 31, _T("ACV Repair Base Pgr"));
	m_ScenPortGrid.SetTextMatrix(0, 32, _T("ACV Repair Base a"));
	m_ScenPortGrid.SetTextMatrix(0, 33, _T("ACV Repair Base b"));
	m_ScenPortGrid.SetTextMatrix(0, 34, _T("Ship Repair Base n"));
	m_ScenPortGrid.SetTextMatrix(0, 35, _T("Ship Repair Base Pgr"));
	m_ScenPortGrid.SetTextMatrix(0, 36, _T("Ship Repair Base a"));
	m_ScenPortGrid.SetTextMatrix(0, 37, _T("Ship Repair Base b"));
	//Land Struct end




}

void CScenarioWizardDlg::OnScenarioPortDeleteButton() 
{
	int SelectedRow = 	m_ScenPortGrid.GetRow();
	int RowsNum = 98; //m_ScenPortGrid.GetRows();
	int ColNum = 23; // m_ScenPortGrid.GetCol();
	CString strArray[99][25];
	int row, col;

	
	if(SelectedRow == RowsNum)
	{
		for (col =1; col < ColNum; col++){
			m_ScenPortGrid.SetTextMatrix(RowsNum,col,"");
		}
	}
	else
	{

		for (row = 1; row < RowsNum; row ++){
			for (col =0; col < ColNum; col++)
				strArray[row][col] = m_ScenPortGrid.GetTextMatrix(row,col);	
		}

		for (row = 1; row < RowsNum; row ++){
			if (row == SelectedRow)
				break;
			for (col =0; col < ColNum; col++){
				m_ScenPortGrid.SetTextMatrix(row,col,strArray[row][col]);
			}	
		}
		for (row = SelectedRow; row < RowsNum; row ++){
			for (col =0; col < ColNum; col++)
				m_ScenPortGrid.SetTextMatrix(row,col,strArray[row+1][col]);
		}
		for (col =0; col < ColNum; col++){
				m_ScenPortGrid.SetTextMatrix(RowsNum,col,"");
		}
	}
	
}

void CScenarioWizardDlg::ClearScenShipsGrid()
{
	int RowsNum = m_ScenShipsGrid.GetRows();
	int ColNum = m_ScenShipsGrid.GetCols();

	int row, col;
	for (row = 1; row < RowsNum; row ++)		
		for (col =1; col < ColNum; col++)
			m_ScenShipsGrid.SetTextMatrix(row,col,"");

	m_ScenShipsGrid.SetRow(1);
	
}

void CScenarioWizardDlg::ClearScenPortsGrid()
{
	int RowsNum = m_ScenPortGrid.GetRows();
	int ColNum = m_ScenPortGrid.GetCols();

	int row, col;
	for (row = 1; row < RowsNum; row ++)		
		for (col =1; col < ColNum; col++)
			m_ScenPortGrid.SetTextMatrix(row,col,"");

	m_ScenPortGrid.SetRow(1);
}

void CScenarioWizardDlg::FillSelectedShipDBInfo()
{
	char *stopstring;
	CString strNum;
	int row = m_ShipsDBGrid.GetRow();

	m_SelectedShipMainInfo = new CShipMainInfo;

	m_SelectedShipMainInfo->m_strlShipTypeName = m_ShipsDBGrid.GetTextMatrix(row,1);
	m_SelectedShipMainInfo->m_strShipRegisterClass = m_ShipsDBGrid.GetTextMatrix(row,2);
	m_SelectedShipMainInfo->m_strPrototypeName = m_ShipsDBGrid.GetTextMatrix(row,3);
	
	strNum = m_ShipsDBGrid.GetTextMatrix(row,4);
	m_SelectedShipMainInfo->m_nDW = (int)strtod(strNum,&stopstring);

	strNum = m_ShipsDBGrid.GetTextMatrix(row,5);
	m_SelectedShipMainInfo->m_nL = (int)strtod(strNum,&stopstring);

	strNum = m_ShipsDBGrid.GetTextMatrix(row,6);
	m_SelectedShipMainInfo->m_nB = (int)strtod(strNum,&stopstring);

	strNum = m_ShipsDBGrid.GetTextMatrix(row,7);
	m_SelectedShipMainInfo->m_nT = (int)strtod(strNum,&stopstring);

	strNum = m_ShipsDBGrid.GetTextMatrix(row,8);
	m_SelectedShipMainInfo->m_nCargoCompatiblGeneral = (int)strtod(strNum,&stopstring);

	strNum = m_ShipsDBGrid.GetTextMatrix(row,9);
	m_SelectedShipMainInfo->m_nCargoCompatiblOil = (int)strtod(strNum,&stopstring);

	strNum = m_ShipsDBGrid.GetTextMatrix(row,10);
	m_SelectedShipMainInfo->m_nCargoCompatiblCoal = (int)strtod(strNum,&stopstring);

	strNum = m_ShipsDBGrid.GetTextMatrix(row,11);
	m_SelectedShipMainInfo->m_nCargoCompatibleVegFruits = (int)strtod(strNum,&stopstring);

	strNum = m_ShipsDBGrid.GetTextMatrix(row,12);
	m_SelectedShipMainInfo->m_nLoadMin = (int)strtod(strNum,&stopstring);

	strNum = m_ShipsDBGrid.GetTextMatrix(row,13);
	m_SelectedShipMainInfo->m_nLoadMax = (int)strtod(strNum,&stopstring);

	strNum = m_ShipsDBGrid.GetTextMatrix(row,14);
	m_SelectedShipMainInfo->m_nLoadDelta = (int)strtod(strNum,&stopstring);

	strNum = m_ShipsDBGrid.GetTextMatrix(row,15);
	m_SelectedShipMainInfo->m_nTariffGenVF = (int)strtod(strNum,&stopstring);

	strNum = m_ShipsDBGrid.GetTextMatrix(row,16);
	m_SelectedShipMainInfo->m_nTariffOil = (int)strtod(strNum,&stopstring);

	strNum = m_ShipsDBGrid.GetTextMatrix(row,17);
	m_SelectedShipMainInfo->m_nTariffCoal = (int)strtod(strNum,&stopstring);

	strNum = m_ShipsDBGrid.GetTextMatrix(row,18);
	m_SelectedShipMainInfo->m_nSpeed = (int)strtod(strNum,&stopstring);

	strNum = m_ShipsDBGrid.GetTextMatrix(row,19);
	m_SelectedShipMainInfo->m_nRange = (int)strtod(strNum,&stopstring);

	strNum = m_ShipsDBGrid.GetTextMatrix(row,20);
	m_SelectedShipMainInfo->m_nh3 = (float)strtod(strNum,&stopstring);

	//begin: ship exist
	strNum = m_ShipsDBGrid.GetTextMatrix(row,21);
	m_SelectedShipMainInfo->m_nShipExistSetCheck = (int)strtod(strNum,&stopstring);

	strNum = m_ShipsDBGrid.GetTextMatrix(row,22);
	m_SelectedShipMainInfo->m_nShipExistForceCheck = (int)strtod(strNum,&stopstring);

	strNum = m_ShipsDBGrid.GetTextMatrix(row,23);
	m_SelectedShipMainInfo->m_nShipExistCheck = (int)strtod(strNum,&stopstring);

	strNum = m_ShipsDBGrid.GetTextMatrix(row,24);
	m_SelectedShipMainInfo->m_nShipExistBuildCheck = (int)strtod(strNum,&stopstring);

	strNum = m_ShipsDBGrid.GetTextMatrix(row,25);
	m_SelectedShipMainInfo->m_nShipExistDesignCheck = (int)strtod(strNum,&stopstring);

	strNum = m_ShipsDBGrid.GetTextMatrix(row,26);
	m_SelectedShipMainInfo->m_nShipExistNum = (int)strtod(strNum,&stopstring);
	strNum = m_ShipsDBGrid.GetTextMatrix(row,27);
	m_SelectedShipMainInfo->m_nShipExistAdvantPrice = (int)strtod(strNum,&stopstring);
	strNum = m_ShipsDBGrid.GetTextMatrix(row,28);
	m_SelectedShipMainInfo->m_nShipExistPrice = (int)strtod(strNum,&stopstring);

	strNum = m_ShipsDBGrid.GetTextMatrix(row,29);
	m_SelectedShipMainInfo->m_nShipExistBuildNum = (int)strtod(strNum,&stopstring);
	strNum = m_ShipsDBGrid.GetTextMatrix(row,30);
	m_SelectedShipMainInfo->m_nShipExistBuildAdvantPrice = (int)strtod(strNum,&stopstring);
	strNum = m_ShipsDBGrid.GetTextMatrix(row,31);
	m_SelectedShipMainInfo->m_nShipExistBuildPrice = (int)strtod(strNum,&stopstring);

	strNum = m_ShipsDBGrid.GetTextMatrix(row,32);
	m_SelectedShipMainInfo->m_nShipExistDesignAdvantPrice = (int)strtod(strNum,&stopstring);
	strNum = m_ShipsDBGrid.GetTextMatrix(row,33);
	m_SelectedShipMainInfo->m_nShipExistDesignPrice = (int)strtod(strNum,&stopstring);

	//end: ship exit
}

BEGIN_EVENTSINK_MAP(CScenarioWizardDlg, CDialog)
    //{{AFX_EVENTSINK_MAP(CScenarioWizardDlg)
	ON_EVENT(CScenarioWizardDlg, IDC_SCEN_SHIPDB_GRID, -601 /* DblClick */, OnDblClickScenShipDBGrid, VTS_NONE)
	ON_EVENT(CScenarioWizardDlg, IDC_SCEN_SHIPS_GRID, -601 /* DblClick */, OnDblClickScenShipScenGrid, VTS_NONE)
	ON_EVENT(CScenarioWizardDlg, IDC_SCEN_PORTDB_GRID, -601 /* DblClick */, OnDblClickScenPortDBGrid, VTS_NONE)
	ON_EVENT(CScenarioWizardDlg, IDC_SCEN_PORTS_GRID, -601 /* DblClick */, OnDblClickScenPortScenGrid, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

void CScenarioWizardDlg::OnDblClickScenShipDBGrid() 
{
	CShipTypePropertySheet thePSheet(_T("Ship Details (Ships DataBase)"),this,NULL);
	FillSelectedShipDBInfo();
	thePSheet.m_SelectedShipMainInfo = m_SelectedShipMainInfo;
	thePSheet.m_OpenStat = 1;
	thePSheet.m_CShipTypeMainPPage.m_OpenStat = 1;
	thePSheet.m_CShipTypeOptimizParamPPage.m_OpenStat = 1;
	thePSheet.DoModal();	
}

void CScenarioWizardDlg::OnDblClickScenShipScenGrid() 
{
	CShipTypePropertySheet thePSheet(_T("Ship Details (Scenario)"),this,NULL);
	FillSelectedShipScenInfo();
	thePSheet.m_SelectedShipMainInfo = m_SelectedShipMainInfo;
	thePSheet.m_OpenStat = 1;
	thePSheet.m_CShipTypeMainPPage.m_OpenStat = 1;
	thePSheet.m_CShipTypeOptimizParamPPage.m_OpenStat = 1;
	thePSheet.DoModal();	
}

void CScenarioWizardDlg::OnDblClickScenPortDBGrid() 
{
	m_test = 55555;

	CPortDetailsPSheet thePSheet(_T("Port Details (Ports DataBase)"),this,NULL);
	FillSelectedPortDBInfo();
	thePSheet.SetSelectedPortMainInfo(m_SelectedPortMainInfo);
	thePSheet.m_CPortMainDetailsPPage.m_SelectedPortMainInfo = m_SelectedPortMainInfo;
	thePSheet.m_PortLandStructPPage.m_SelectedPortLandStructInfo = m_SelectedPortLandStructInfo;
	thePSheet.m_CPortDistMatrixPPage.m_ISLoadDetails = TRUE;
	thePSheet.m_CPortMainDetailsPPage.m_ISLoadDetails = TRUE;
	thePSheet.m_PortLandStructPPage.m_ISLoadDetails = TRUE;
	thePSheet.m_OpenStat = 1; //view
	thePSheet.m_CPortMainDetailsPPage.m_OpenStat = 1;
	thePSheet.m_PortLandStructPPage.m_OpenStat = 1;
	thePSheet.m_RecordsNumber = m_PortDBRecordsNum;
	thePSheet.DoModal();	
}

void CScenarioWizardDlg::OnDblClickScenPortScenGrid() 
{
	m_test = 55555;

	CPortDetailsPSheet thePSheet(_T("Port Details (Scenario)"),this,NULL);
	FillSelectedPortScenInfo();
	thePSheet.SetSelectedPortMainInfo(m_SelectedPortMainInfo);
	thePSheet.m_CPortMainDetailsPPage.m_SelectedPortMainInfo = m_SelectedPortMainInfo;
	thePSheet.m_PortLandStructPPage.m_SelectedPortLandStructInfo = m_SelectedPortLandStructInfo;
	thePSheet.m_CPortDistMatrixPPage.m_ISLoadDetails = TRUE;
	thePSheet.m_CPortMainDetailsPPage.m_ISLoadDetails = TRUE;
	thePSheet.m_PortLandStructPPage.m_ISLoadDetails = TRUE;
	thePSheet.m_OpenStat = 1; //view
	thePSheet.m_CPortMainDetailsPPage.m_OpenStat = 1;
	thePSheet.m_PortLandStructPPage.m_OpenStat = 1;
	thePSheet.m_RecordsNumber = m_PortDBRecordsNum;
	thePSheet.DoModal();	
}



void CScenarioWizardDlg::OnContextMenu(CWnd* pWnd, CPoint point) 
{
 	CMenu menu;
	menu.LoadMenu(IDR_SCENARIOWIZ_CONTEXT);
	CMenu* pPopUp;

	if (this->GetFocus()->GetDlgCtrlID() == IDC_SCEN_SHIPDB_GRID)
		pPopUp = menu.GetSubMenu(0);
	else if (this->GetFocus()->GetDlgCtrlID() == IDC_SCEN_SHIPS_GRID)
		pPopUp = menu.GetSubMenu(1);
	else if (this->GetFocus()->GetDlgCtrlID() == IDC_SCEN_PORTDB_GRID)
		pPopUp = menu.GetSubMenu(2);
	else if (this->GetFocus()->GetDlgCtrlID() == IDC_SCEN_PORTS_GRID)
		pPopUp = menu.GetSubMenu(3);
	else 
		pPopUp = NULL;

	if (pPopUp)
		pPopUp->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON,
 							point.x, point.y, this);

}

void CScenarioWizardDlg::OnContextMenuShipsDBView() 
{
	OnDblClickScenShipDBGrid();	
}

void CScenarioWizardDlg::OnContextMenuShipsDBGet() 
{
	OnGetShip();
}

void CScenarioWizardDlg::OnContextMenuShipScenView() 
{
	OnDblClickScenShipScenGrid();	
}

void CScenarioWizardDlg::OnContextMenuShipsScenDelete() 
{
	OnScenarioShipDeleteButton();
}

void CScenarioWizardDlg::OnContextMenuPortsDBView() 
{
	OnDblClickScenPortDBGrid();	
}

void CScenarioWizardDlg::OnContextMenuPortsDBGet() 
{
	OnGetPort();	
}

void CScenarioWizardDlg::OnContextMunuPortScenView() 
{
	OnDblClickScenPortScenGrid();
	
}

void CScenarioWizardDlg::OnContextMenuPortScenDelete() 
{
	OnScenarioPortDeleteButton();	
}


void CScenarioWizardDlg::OnScenarioPortEditButton() 
{
	char *stopstring;
	CString strNum;
	m_nEditPortNum = m_ScenPortGrid.GetRow();
	strNum = m_ScenPortGrid.GetTextMatrix(m_nEditPortNum,0);
	m_nEditPortID  = (int)strtod(strNum,&stopstring);

	CPortDetailsPSheet thePSheet(_T("Edit Port Details"));
	FillSelectedPortScenInfo();
	thePSheet.SetSelectedPortMainInfo(m_SelectedPortMainInfo);
	thePSheet.m_CPortDistMatrixPPage.m_ISModifiable = TRUE;
	thePSheet.m_CPortMainDetailsPPage.m_ISModifiable = TRUE;
	thePSheet.m_PortLandStructPPage.m_ISModifiable = TRUE;
	thePSheet.m_CPortMainDetailsPPage.m_SelectedPortMainInfo = m_SelectedPortMainInfo;
	thePSheet.m_PortLandStructPPage.m_SelectedPortLandStructInfo = m_SelectedPortLandStructInfo;
	thePSheet.m_OpenStat = 2;
	thePSheet.m_CPortMainDetailsPPage.m_OpenStat = 2;
	thePSheet.m_PortLandStructPPage.m_OpenStat = 2;
	thePSheet.m_SelectedPortID = m_nEditPortID;
	thePSheet.m_RecordsNumber = m_PortDBRecordsNum;

	m_flagEditPort = 1;
	if (thePSheet.DoModal() == IDOK){
		OnScenarioRefreshButton();
	}

}

void CScenarioWizardDlg::OnScenarioShipEditButton() 
{

	
	CShipTypePropertySheet thePSheet(_T("Edit Ship Type Details"),this,NULL);
	FillSelectedShipScenInfo();
	thePSheet.m_SelectedShipMainInfo = m_SelectedShipMainInfo;
	thePSheet.m_OpenStat = 2;
	thePSheet.m_CShipTypeMainPPage.m_OpenStat = 2;
	thePSheet.m_CShipTypeOptimizParamPPage.m_OpenStat = 2;
	thePSheet.m_RecordsNumber = m_ShipsDBRecordsNum;
	thePSheet.m_pShipsDBFile = m_pShipsDBFile;

	//search ShipID in DB table, according to selection in Scenario Ship Table.
	char *stopstring;
	CString strNum, ShipNameInScen, ShipNameInDB;
	int ShipID = 0;
	m_nEditShipNum = m_ScenShipsGrid.GetRow();
	ShipNameInScen = m_ScenShipsGrid.GetTextMatrix(m_nEditShipNum,1);
	for(int i = 1; i <= m_ShipsDBRecordsNum; i++){
		ShipNameInDB = m_ShipsDBGrid.GetTextMatrix(i,1);
		if (ShipNameInDB == ShipNameInScen){
			strNum = m_ShipsDBGrid.GetTextMatrix(i,0);
			m_nEditShipID  = (int)strtod(strNum,&stopstring);
			break;
		}
	}
	thePSheet.m_SelectedShipID = m_nEditShipID;
	//thePSheet.DoModal();	

	m_flagEditShip = 1;
	if (thePSheet.DoModal() == IDOK){
		OnScenarioRefreshButton();
	}


}

void CScenarioWizardDlg::OnContextMenuShipScenEdit() 
{
	OnScenarioShipEditButton();
}

void CScenarioWizardDlg::OnContextMenuPortScenEdit() 
{
	OnScenarioPortEditButton();	
}

void CScenarioWizardDlg::OnInvestmentButton() 
{
	if(!m_pInvestDlg)
	{
		m_pInvestDlg  = new CInvestDlg();
	}
	else
	{
		m_pInvestDlg->ShowWindow(SW_RESTORE);
	}
	char strData[10];
	CString strText;
	_itoa(m_InvestmentSize, strData, 10);
	strText = strData;
	strText.Format("%s", strText);
	strText.TrimLeft();
	m_pInvestDlg->m_ctrInvestmentSize.SetWindowText(strText);
	m_pInvestDlg->m_ctrlIncludeLandStructCalcInScen.SetCheck(m_bIncludeLandStructCalc);

	m_pInvestDlg->ShowWindow(SW_SHOW);


}

void CScenarioWizardDlg::FillInvestInfo()
{
	int flScenLoaded = 1;
	char strNum[15];

	m_ctrInvestLimitGroupBox.ShowWindow(SW_HIDE);
	m_ctrInvestLimitEdit.ShowWindow(SW_HIDE);

	if (flScenLoaded){
		m_ctrInvestLimitGroupBox.ShowWindow(SW_SHOW);
		m_ctrInvestLimitEdit.ShowWindow(SW_SHOW);
		ReadFromRegistry();
		if(m_InvestmentSize == 0){
			strcpy(strNum, "none");			
		}
		else {
				_itoa(m_InvestmentSize, strNum, 10);
		}
		m_ctrInvestLimitEdit.SetWindowText(strNum);
		
		if(m_pInvestDlg) {
			m_bIncludeLandStructCalc = 	m_pInvestDlg->m_ctrlIncludeLandStructCalcInScen.GetCheck();
		}
	}

}



void CScenarioWizardDlg::ReadFromRegistry()
{
	m_InvestmentSize = ((CFleetOptimizerApp*)AfxGetApp())->GetProfileInt("FleetOptimizer","Investment",0);	
	m_bIncludeLandStructCalc = ((CFleetOptimizerApp*)AfxGetApp())->GetProfileInt("FleetOptimizer","IncludeLandStructCalc",0);	

	CString str;
	str = ((CFleetOptimizerApp*)AfxGetApp())->GetProfileString("FleetOptimizer","DbPorts");
	m_pPortsDBFile = _strdup(str.GetBuffer(20));

	str = ((CFleetOptimizerApp*)AfxGetApp())->GetProfileString("FleetOptimizer","DbShips");
	m_pShipsDBFile = _strdup(str.GetBuffer(20));

	str = ((CFleetOptimizerApp*)AfxGetApp())->GetProfileString("FleetOptimizer","DbPortsDistMatrix");
	m_pPortDistMatrixFile = _strdup(str.GetBuffer(20));
}

void CScenarioWizardDlg::SaveToRegistry() {

	((CFleetOptimizerApp*)AfxGetApp())->WriteProfileInt("FleetOptimizer","Investment",m_InvestmentSize);	
	((CFleetOptimizerApp*)AfxGetApp())->WriteProfileInt("FleetOptimizer","IncludeLandStructCalc",m_bIncludeLandStructCalc);	

}


void CScenarioWizardDlg::SaveScenInvestDataToFile(CArchive &ar)
{
	CString str;

	m_ctrInvestLimitEdit.GetWindowText(str);
	ar << str;

//  put m_bIncludeLandStructCalc into archive:
	char strData[5];
	CString strText;
	_itoa(m_bIncludeLandStructCalc, strData, 10);
	strText = strData;
	strText.Format("%s", strText);
	strText.TrimLeft();
	ar << strText;
}


void CScenarioWizardDlg::LoadScenInvestData(CArchive& ar)
{
	CString str;
	char *stopstring;

	ar >> str;	//Invest Limit
	if (str == "END OF SCENARIO FILE")
		return;

	m_InvestmentSize = (int)strtod(str,&stopstring);
	
	if (str == "0")
		str = "none";
	m_ctrInvestLimitEdit.SetWindowText(str);

	ar >> str;	//Land Struct include
	if (str == "END OF SCENARIO FILE")
		return;
	m_bIncludeLandStructCalc = (int)strtod(str,&stopstring);	
	if ((m_bIncludeLandStructCalc != 0)&&(m_bIncludeLandStructCalc != 1)){
		AfxMessageBox("Ошибка загрузки значения \"IncludeLandStruct\" из файла сценария\nCScenarioWizardDlg::LoadScenInvestData()");
		return;
	}

	SaveToRegistry();

}



