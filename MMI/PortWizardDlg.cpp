// PortWizardDlg.cpp : implementation file
//

#include "stdafx.h"
#include <io.h>


//#include "	\ add additional includes here"
#include "PortWizardDlg.h"
#include "PortDetailsPSheet.h"
#include <stdlib.h>


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

BOOL ClickedOK = FALSE;

/////////////////////////////////////////////////////////////////////////////
// CPortWizardDlg dialog


CPortWizardDlg::CPortWizardDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CPortWizardDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPortWizardDlg)
	//}}AFX_DATA_INIT

	//m_pPortsDBFile = "ports_db.fd";
	//m_pPortDistMatrixFile = "ports_dist.fl";
	CString str = ((CFleetOptimizerApp*)AfxGetApp())->GetProfileString("FleetOptimizer","DbPorts");
	m_pPortsDBFile = _strdup(str.GetBuffer(20));
	str = ((CFleetOptimizerApp*)AfxGetApp())->GetProfileString("FleetOptimizer","DbPortsDistMatrix");
	m_pPortDistMatrixFile = _strdup(str.GetBuffer(20));

	//m_pScenarioWizardDlg = NULL;
	m_nID = CPortWizardDlg::IDD;
	CDialog::Create(m_nID, pParent);

}

//CPortWizardDlg::CPortWizardDlg(CScenarioWizardDlg *pScenarioWizardDlg)
//{
//	m_pScenarioWizardDlg = pScenarioWizardDlg;
//
//	m_pPortsDBFile = "ports_db.fd";
//	m_pPortDistMatrixFile = "ports_dist.fl";
//}

void CPortWizardDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPortWizardDlg)
	DDX_Control(pDX, IDC_PORTWIZ_LIST, m_PortGenInfoGrid);
	DDX_Control(pDX, IDC_PORTWIZ_CARGOINFO, m_PortCargoInfoGrid);
	DDX_Control(pDX, IDC_MSFLEXGRID2, m_PortsDistMatrixInfoGrid);
	//}}AFX_DATA_MAP

	FillGenInfoGrid();
	/*CFile f( m_pPortsDBFile, CFile::modeRead );
	f.SeekToBegin();
	CArchive ar(&f, CArchive::load);
	Serialize(ar);*/
	if (m_ActionFlag == 2){
		//AfxMessageBox("EDIT!!!");
		m_ActionFlag = 0;
	}
}


BEGIN_MESSAGE_MAP(CPortWizardDlg, CDialog)
	//{{AFX_MSG_MAP(CPortWizardDlg)
	ON_BN_CLICKED(IDC_PORTWIZ_DETAILS, OnPortWizDetails)
	ON_BN_CLICKED(IDC_PORTWIZ_NEW, OnPortWizNew)
	ON_BN_CLICKED(IDC_PORTWIZ_EDIT, OnPortWizEdit)
	ON_BN_CLICKED(IDC_PORTWIZ_DELETE, OnPortwizDelete)
	ON_BN_CLICKED(IDC_PORTWIZ_REFRESH, OnPortWizRefresh)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPortWizardDlg message handlers

void CPortWizardDlg::OnPortWizDetails() 
{
	// TODO: Add your control notification handler code here
	CPortDetailsPSheet thePSheet(_T("Характеристики порта"));
	FillSelectedPortInfo();
//	thePSheet.m_CPortDistMatrixPPage.m_ISModifiable = TRUE;
//	thePSheet.m_CPortMainDetailsPPage.m_ISModifiable = TRUE;
	thePSheet.m_CPortMainDetailsPPage.m_SelectedPortMainInfo = m_SelectedPortMainInfo;
	thePSheet.m_PortLandStructPPage.m_SelectedPortLandStructInfo = m_SelectedPortLandStructInfo;
	thePSheet.m_CPortDistMatrixPPage.m_ISLoadDetails = TRUE;
	thePSheet.m_CPortMainDetailsPPage.m_ISLoadDetails = TRUE;
	thePSheet.m_PortLandStructPPage.m_ISLoadDetails = TRUE;
	thePSheet.m_OpenStat = 1;
	thePSheet.m_CPortMainDetailsPPage.m_OpenStat = 1;
	thePSheet.m_PortLandStructPPage.m_OpenStat = 1;
	thePSheet.m_RecordsNumber = m_RecordsNumber;
	thePSheet.DoModal();

}

void CPortWizardDlg::OnPortWizNew() 
{
	CPortDetailsPSheet thePSheet(_T("New Port Details Adding to Fleet DataBase"));
	thePSheet.m_CPortDistMatrixPPage.m_ISModifiable = TRUE;
	thePSheet.m_CPortMainDetailsPPage.m_ISModifiable = TRUE;
	thePSheet.m_PortLandStructPPage.m_ISModifiable = TRUE;
	thePSheet.m_OpenStat = 3;
	thePSheet.m_CPortMainDetailsPPage.m_OpenStat = 3;
	thePSheet.m_PortLandStructPPage.m_OpenStat = 3;
	thePSheet.m_RecordsNumber = m_RecordsNumber;
	thePSheet.m_CPortMainDetailsPPage.m_RecordsNumber = m_RecordsNumber;

	if (thePSheet.DoModal() == IDOK){
		OnPortWizRefresh();
	}
}

void CPortWizardDlg::OnPortWizEdit() 
{
	CPortDetailsPSheet thePSheet(_T("Edit Port Details"));
	FillSelectedPortInfo();
	thePSheet.m_CPortDistMatrixPPage.m_ISModifiable = TRUE;
	thePSheet.m_CPortMainDetailsPPage.m_ISModifiable = TRUE;
	thePSheet.m_PortLandStructPPage.m_ISModifiable = TRUE;
	thePSheet.m_CPortMainDetailsPPage.m_SelectedPortMainInfo = m_SelectedPortMainInfo;
	thePSheet.m_PortLandStructPPage.m_SelectedPortLandStructInfo = m_SelectedPortLandStructInfo;
	thePSheet.m_CPortDistMatrixPPage.m_ISLoadDetails = TRUE;
	thePSheet.m_CPortMainDetailsPPage.m_ISLoadDetails = TRUE;
	thePSheet.m_PortLandStructPPage.m_ISLoadDetails = TRUE;
	thePSheet.m_OpenStat = 2;
	m_SelectedPortID = 	m_PortGenInfoGrid.GetRow();
	thePSheet.m_SelectedPortID = m_SelectedPortID;
	m_ActionFlag = thePSheet.m_OpenStat;
	thePSheet.m_CPortMainDetailsPPage.m_OpenStat = 2;
	thePSheet.m_PortLandStructPPage.m_OpenStat = 2;
	thePSheet.m_RecordsNumber = m_RecordsNumber;

	if (thePSheet.DoModal() == IDOK){
		OnPortWizRefresh();
	}
	
}



BOOL CPortWizardDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
//	CPortDetailsPSheet thePSheet(_T(""));
//	m_pMainWnd = &thePSheet;

	FillGridsTitles();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CPortWizardDlg::Serialize(CArchive& ar) 
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
			m_RecordsNumber = 0;
		else {
				ar >> strNum;
				char *stopstring;
				m_RecordsNumber  = (int)strtod(strNum,&stopstring);
		}
//m_RecordsNumber = 3;
		int i = 1;
		int j;
		char strRowNum[5];
		int flTmpLandStructInt = 1;
   
		while (i <= m_RecordsNumber){
			ar >> strPortName;
			ar >> strPortType;
			ar >> strBasePortName;
			ar >> strPortRegion;

			position = fp->GetPosition();
			
			_itoa(i, strRowNum, 10);
			m_PortGenInfoGrid.SetTextMatrix(i,0,strRowNum);

			m_PortGenInfoGrid.SetTextMatrix(i,1,strPortName);
			m_PortGenInfoGrid.SetTextMatrix(i,2,strPortType);
			m_PortGenInfoGrid.SetTextMatrix(i,3,strBasePortName);
			m_PortGenInfoGrid.SetTextMatrix(i,4,strPortRegion);
			
			//Cargoes set: 5...20; 
			for (j = 5; j <=20; j++){
//				strNum = "";
				ar >> strNum;
				m_PortGenInfoGrid.SetTextMatrix(i,j,strNum);
			}
			//end of Cargoes set
			if (flTmpLandStructInt == 1){
				//Land Struct: 21...37
				for (j = 21; j <=37; j++){
					ar >> strNum;
					m_PortGenInfoGrid.SetTextMatrix(i,j,strNum);
				}
			}

/*			
			int max = 13 + i;//m_RecordsNumber;
			for (j = 13; j < max; j++){
				ar >> strTmpMatrixValue;
				m_PortGenInfoGrid.SetTextMatrix(i,j,strTmpMatrixValue);
			}
*/			

			position = fp->GetPosition();
			i++;
		}//end of while (i <= m_RecordsNumber)

		// Read DistMatrix file and fill m_PortsDistMatrixInfoGrid
	//	DWORD dwPositionEnd, dwPosition;

		
		CFile f;
		CFileException e;
		f.Open( m_pPortDistMatrixFile, CFile::modeRead, &e);
		f.SeekToEnd();
		positionEnd = f.GetPosition();
		f.SeekToBegin();
		position = f.GetPosition();
		if (positionEnd != 0){
			CArchive arPortDist(&f, CArchive::load);
			CString strMatrixValue;
			int flEx = 0;
			for (i = 1; i <= m_RecordsNumber; i++){
			//for (i = 1; i < 1; i++)
				for (int j = 1; j <= i; j++){
					strMatrixValue = "";
					try
					{
						arPortDist >> strMatrixValue;
						m_PortsDistMatrixInfoGrid.SetTextMatrix(i,j,strMatrixValue);
					}
					catch (CArchiveException* pArchEx)
					{
						int cause = pArchEx->m_cause;
						if (cause == 3){
							AfxMessageBox("Матрица расстояний выглядит незавершенной!");
							flEx = 1;
							break;
						}
					}
				}
				if (flEx)
					break;
			}
			//if (i < m_RecordsNumber)
			//		AfxMessageBox("Distance Matrix seems not completed!");
	

			arPortDist.Close();
		}
		f.Close();
		//End of Read DistMatrix file and fill m_PortsDistMatrixInfoGrid

	}
}

BEGIN_EVENTSINK_MAP(CPortWizardDlg, CDialog)
    //{{AFX_EVENTSINK_MAP(CPortWizardDlg)
	ON_EVENT(CPortWizardDlg, IDC_PORTWIZ_LIST, -600 /* Click */, OnClickPortwizList, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

void CPortWizardDlg::OnClickPortwizList() 
{
	// TODO: Add your control notification handler code here
	int row = m_PortGenInfoGrid.GetRow();
	if (row == 0)
		return;

	int i, j;
	CString strTmpMatrixValue;
	for (i = 1, j= 5; i <=4; i ++,j++){
		strTmpMatrixValue = m_PortGenInfoGrid.GetTextMatrix(row,j);
		m_PortCargoInfoGrid.SetTextMatrix(i,1,strTmpMatrixValue);
		strTmpMatrixValue = m_PortGenInfoGrid.GetTextMatrix(row,j+4);
		m_PortCargoInfoGrid.SetTextMatrix(i,2,strTmpMatrixValue);
		strTmpMatrixValue = m_PortGenInfoGrid.GetTextMatrix(row,j+8);
		m_PortCargoInfoGrid.SetTextMatrix(i,3,strTmpMatrixValue);
		strTmpMatrixValue = m_PortGenInfoGrid.GetTextMatrix(row,j+12);
		m_PortCargoInfoGrid.SetTextMatrix(i,4,strTmpMatrixValue);

	}
}

void CPortWizardDlg::FillGenInfoGrid()
{
	CFile f( m_pPortsDBFile, CFile::modeRead );
	f.SeekToBegin();
	CArchive ar(&f, CArchive::load);
	Serialize(ar);
}

void CPortWizardDlg::FillGridsTitles()
{
	m_PortGenInfoGrid.SetTextMatrix(0,1,_T("Name"));
	m_PortGenInfoGrid.SetTextMatrix(0,2,_T("Type"));
	m_PortGenInfoGrid.SetTextMatrix(0,3,_T("Base Port"));
	m_PortGenInfoGrid.SetTextMatrix(0,4,_T("Region"));
	m_PortGenInfoGrid.SetTextMatrix(0,5,_T("Coal-required, t"));
	m_PortGenInfoGrid.SetTextMatrix(0,6,_T("General-required, t"));
	m_PortGenInfoGrid.SetTextMatrix(0,7,_T("Oil-required, t"));
	m_PortGenInfoGrid.SetTextMatrix(0,8,_T("Veg-froots-required, t"));
	m_PortGenInfoGrid.SetTextMatrix(0,9,_T("Coal-available, t"));
	m_PortGenInfoGrid.SetTextMatrix(0,10,_T("General-available, t"));
	m_PortGenInfoGrid.SetTextMatrix(0,11,_T("Oil-available, t"));
	m_PortGenInfoGrid.SetTextMatrix(0,12,_T("Veg-froots-available, t"));

	m_PortGenInfoGrid.SetTextMatrix(0,13,_T("Coal. Ship Load/Unload t/h"));
	m_PortGenInfoGrid.SetTextMatrix(0,14,_T("General. Ship Load/Unload t/h"));
	m_PortGenInfoGrid.SetTextMatrix(0,15,_T("Oil. Ship Load/Unload t/h"));
	m_PortGenInfoGrid.SetTextMatrix(0,16,_T("Veg-froots. Ship Load/Unload t/h"));

	m_PortGenInfoGrid.SetTextMatrix(0,17,_T("Coal. ACV Load/Unload t/h"));
	m_PortGenInfoGrid.SetTextMatrix(0,18,_T("General. ACV Load/Unload t/h"));
	m_PortGenInfoGrid.SetTextMatrix(0,19,_T("Oil. ACV Load/Unload t/h"));
	m_PortGenInfoGrid.SetTextMatrix(0,20,_T("Veg-froots. ACV Load/Unload t/h"));

	//Land Struct begin
	m_PortGenInfoGrid.SetTextMatrix(0,21,_T("Consider OnShorel infrastructure(0,1)"));
	m_PortGenInfoGrid.SetTextMatrix(0,22,_T("PortPoint Onshore area for ACV exists(0,1)"));
	m_PortGenInfoGrid.SetTextMatrix(0,23,_T("PortPoint Onshore area for ACV: inbestments required"));
	m_PortGenInfoGrid.SetTextMatrix(0,24,_T("PortPoint Onshore area for Ships with ramp exists(0,1)"));
	m_PortGenInfoGrid.SetTextMatrix(0,25,_T("PortPoint Onshore area for Ships with ramp: inbestments required"));
	m_PortGenInfoGrid.SetTextMatrix(0,26,_T("Size of ACV Onshore area in Base Port"));
	m_PortGenInfoGrid.SetTextMatrix(0,27,_T("Cost of 100 m2 of ACV Onshore area in Base Port"));
	m_PortGenInfoGrid.SetTextMatrix(0,28,_T("Berth length (existing), m"));
	m_PortGenInfoGrid.SetTextMatrix(0,29,_T("Cost of 10 m of Berth"));
	m_PortGenInfoGrid.SetTextMatrix(0,30,_T("ACV Repair Base n"));
	m_PortGenInfoGrid.SetTextMatrix(0,31,_T("ACV Repair Base Pgr"));
	m_PortGenInfoGrid.SetTextMatrix(0,32,_T("ACV Repair Base a"));
	m_PortGenInfoGrid.SetTextMatrix(0,33,_T("ACV Repair Base b"));
	m_PortGenInfoGrid.SetTextMatrix(0,34,_T("Ship Repair Base n"));
	m_PortGenInfoGrid.SetTextMatrix(0,35,_T("Ship Repair Base Pgr"));
	m_PortGenInfoGrid.SetTextMatrix(0,36,_T("Ship Repair Base a"));
	m_PortGenInfoGrid.SetTextMatrix(0,37,_T("Ship Repair Base b"));
	//Land Struct end

	//m_PortCargoInfoGrid.SetTextMatrix(0,0,_T("Cargo Type"));
	//m_PortCargoInfoGrid.SetTextMatrix(0,1,_T("Available, t"));
	//m_PortCargoInfoGrid.SetTextMatrix(0,2,_T("Requered, t"));
	//m_PortCargoInfoGrid.SetTextMatrix(0,3,_T("Ship load, t/\h"));
	//m_PortCargoInfoGrid.SetTextMatrix(0,4,_T("ACV load, t/\h"));
	m_PortCargoInfoGrid.SetTextMatrix(1,0,_T("Уголь"));
	m_PortCargoInfoGrid.SetTextMatrix(2,0,_T("Генгрузы"));
	m_PortCargoInfoGrid.SetTextMatrix(3,0,_T("Нефть"));
	m_PortCargoInfoGrid.SetTextMatrix(4,0,_T("Овощи-фрукты"));

	char strNum[5];
	for (int i=1; i < 100; i++){
		_itoa(i, strNum, 10);
		m_PortsDistMatrixInfoGrid.SetColWidth(i, 400);
		m_PortsDistMatrixInfoGrid.SetTextMatrix(i,0,strNum);
		m_PortsDistMatrixInfoGrid.SetTextMatrix(0,i,strNum);
	}


}


//DEL BOOL CPortWizardDlg::Create()
//DEL {
//DEL     return CDialog::Create(CPortWizardDlg::IDD);
//DEL }


void CPortWizardDlg::OnCancel() // not really a message handler
{
	UpdateData(FALSE);
	ShowWindow(SW_HIDE);
}

void CPortWizardDlg::OnOK() // not really a message handler
{
	ClickedOK = TRUE;
	if(!UpdateData(TRUE))
	{
		ClickedOK = FALSE;
		return;
	}

	ClickedOK = FALSE;
	CDialog::OnOK();

}

void CPortWizardDlg::FillSelectedPortInfo()
{
	char *stopstring;
	CString strNum;
	int row = m_PortGenInfoGrid.GetRow();

	m_SelectedPortMainInfo = new CPortMainInfo;
	m_SelectedPortLandStructInfo = new CPortLandStructInfo;


	strNum = m_PortGenInfoGrid.GetTextMatrix(row,0);
	m_SelectedPortMainInfo->m_nPortID = (int)strtod(strNum,&stopstring);

	m_SelectedPortMainInfo->m_strPortName = m_PortGenInfoGrid.GetTextMatrix(row,1);
	m_SelectedPortMainInfo->m_strPortType = m_PortGenInfoGrid.GetTextMatrix(row,2);
	m_SelectedPortMainInfo->m_strBasePortName = m_PortGenInfoGrid.GetTextMatrix(row,3);
	m_SelectedPortMainInfo->m_strPortRegion = m_PortGenInfoGrid.GetTextMatrix(row,4);
	
	strNum = m_PortGenInfoGrid.GetTextMatrix(row,5);
	m_SelectedPortMainInfo->required_cargoes.coal = (int)strtod(strNum,&stopstring);
	strNum = m_PortGenInfoGrid.GetTextMatrix(row,6);
	m_SelectedPortMainInfo->required_cargoes.general = (int)strtod(strNum,&stopstring);
	strNum = m_PortGenInfoGrid.GetTextMatrix(row,7);
	m_SelectedPortMainInfo->required_cargoes.oil = (int)strtod(strNum,&stopstring);
	strNum = m_PortGenInfoGrid.GetTextMatrix(row,8);
	m_SelectedPortMainInfo->required_cargoes.vegetables_fruits = (int)strtod(strNum,&stopstring);
	strNum = m_PortGenInfoGrid.GetTextMatrix(row,9);
	m_SelectedPortMainInfo->available_cargoes.coal = (int)strtod(strNum,&stopstring);
	strNum = m_PortGenInfoGrid.GetTextMatrix(row,10);
	m_SelectedPortMainInfo->available_cargoes.general = (int)strtod(strNum,&stopstring);
	strNum = m_PortGenInfoGrid.GetTextMatrix(row,11);
	m_SelectedPortMainInfo->available_cargoes.oil = (int)strtod(strNum,&stopstring);
	strNum = m_PortGenInfoGrid.GetTextMatrix(row,12);
	m_SelectedPortMainInfo->available_cargoes.vegetables_fruits = (int)strtod(strNum,&stopstring);

	strNum = m_PortGenInfoGrid.GetTextMatrix(row,13);
	m_SelectedPortMainInfo->ship_loading_productivity.coal = (int)strtod(strNum,&stopstring);
	strNum = m_PortGenInfoGrid.GetTextMatrix(row,14);
	m_SelectedPortMainInfo->ship_loading_productivity.general = (int)strtod(strNum,&stopstring);
	strNum = m_PortGenInfoGrid.GetTextMatrix(row,15);
	m_SelectedPortMainInfo->ship_loading_productivity.oil = (int)strtod(strNum,&stopstring);
	strNum = m_PortGenInfoGrid.GetTextMatrix(row,16);
	m_SelectedPortMainInfo->ship_loading_productivity.vegetables_fruits = (int)strtod(strNum,&stopstring);

	strNum = m_PortGenInfoGrid.GetTextMatrix(row,17);
	m_SelectedPortMainInfo->acv_loading_productivity.coal = (int)strtod(strNum,&stopstring);
	strNum = m_PortGenInfoGrid.GetTextMatrix(row,18);
	m_SelectedPortMainInfo->acv_loading_productivity.general = (int)strtod(strNum,&stopstring);
	strNum = m_PortGenInfoGrid.GetTextMatrix(row,19);
	m_SelectedPortMainInfo->acv_loading_productivity.oil = (int)strtod(strNum,&stopstring);
	strNum = m_PortGenInfoGrid.GetTextMatrix(row,20);
	m_SelectedPortMainInfo->acv_loading_productivity.vegetables_fruits = (int)strtod(strNum,&stopstring);

	////////////  Land Structures Info  /////////////
	strNum = m_PortGenInfoGrid.GetTextMatrix(row,21); 
	m_SelectedPortLandStructInfo->m_nLandStructCalcActivateCheck = (int)strtod(strNum,&stopstring);
	strNum = m_PortGenInfoGrid.GetTextMatrix(row,22); 
	m_SelectedPortLandStructInfo->m_nACVCourtAvailableCheck = (int)strtod(strNum,&stopstring);
	strNum = m_PortGenInfoGrid.GetTextMatrix(row,23); 
	m_SelectedPortLandStructInfo->m_nACVCourtInvestPP = (int)strtod(strNum,&stopstring);
	strNum = m_PortGenInfoGrid.GetTextMatrix(row,24); 
	m_SelectedPortLandStructInfo->m_nShipCourtAvailableCheck = (int)strtod(strNum,&stopstring);
	strNum = m_PortGenInfoGrid.GetTextMatrix(row,25); 
	m_SelectedPortLandStructInfo->m_nShipCourtInvestPP = (int)strtod(strNum,&stopstring);

	strNum = m_PortGenInfoGrid.GetTextMatrix(row,26); 
	m_SelectedPortLandStructInfo->m_nACVSpace = (int)strtod(strNum,&stopstring);
	strNum = m_PortGenInfoGrid.GetTextMatrix(row,27); 
	m_SelectedPortLandStructInfo->m_nACVSpaceElemPrice = (int)strtod(strNum,&stopstring);
	strNum = m_PortGenInfoGrid.GetTextMatrix(row,28); 
	m_SelectedPortLandStructInfo->m_nMoorLength = (int)strtod(strNum,&stopstring);
	strNum = m_PortGenInfoGrid.GetTextMatrix(row,29); 
	m_SelectedPortLandStructInfo->m_nMoorElemPrice = (int)strtod(strNum,&stopstring);

	strNum = m_PortGenInfoGrid.GetTextMatrix(row,30); 
	m_SelectedPortLandStructInfo->m_structRepairDetailsACV.n = (int)strtod(strNum,&stopstring);
	strNum = m_PortGenInfoGrid.GetTextMatrix(row,31); 
	m_SelectedPortLandStructInfo->m_structRepairDetailsACV.Pgr = (int)strtod(strNum,&stopstring);
	strNum = m_PortGenInfoGrid.GetTextMatrix(row,32); 
	m_SelectedPortLandStructInfo->m_structRepairDetailsACV.a = (int)strtod(strNum,&stopstring);
	strNum = m_PortGenInfoGrid.GetTextMatrix(row,33); 
	m_SelectedPortLandStructInfo->m_structRepairDetailsACV.b = (int)strtod(strNum,&stopstring);

	strNum = m_PortGenInfoGrid.GetTextMatrix(row,34); 
	m_SelectedPortLandStructInfo->m_structRepairDetailsShip.n = (int)strtod(strNum,&stopstring);
	strNum = m_PortGenInfoGrid.GetTextMatrix(row,35); 
	m_SelectedPortLandStructInfo->m_structRepairDetailsShip.Pgr = (int)strtod(strNum,&stopstring);
	strNum = m_PortGenInfoGrid.GetTextMatrix(row,36); 
	m_SelectedPortLandStructInfo->m_structRepairDetailsShip.a = (int)strtod(strNum,&stopstring);
	strNum = m_PortGenInfoGrid.GetTextMatrix(row,37); 
	m_SelectedPortLandStructInfo->m_structRepairDetailsShip.b = (int)strtod(strNum,&stopstring);

}

int CPortWizardDlg::GetSelectedPortID()
{
	m_SelectedPortID = 	m_PortGenInfoGrid.GetRow();

	return m_SelectedPortID;
}

void CPortWizardDlg::OnPortwizDelete() 
{
//	char *stopstring;
	CString strNum;
	CString strText;

	if (m_RecordsNumber >= MAX_PORT_RECORDS_IN_DB)
		AfxMessageBox("Количество портов в Базе Данных превысило установки программы.\n значение MAX_PORT_RECORDS_IN_DB должно быть увеличено в программе.");

	CPortMainInfo PortInfo[MAX_PORT_RECORDS_IN_DB];
	m_SelectedPortID = 	m_PortGenInfoGrid.GetRow();
	CPortLandStructInfo PortLandInfo[MAX_PORT_RECORDS_IN_DB];


	CFile f( m_pPortsDBFile, CFile::modeRead );
	CArchive ar(&f, CArchive::load);

	GetIntFromArchive(ar); //RecNum == m_RecordsNumber

	for (int i = 1; i <= m_RecordsNumber; i++){

		//if (i == m_SelectedPortID)
		//	continue;
	
		ar >> PortInfo[i].m_strPortName;
		ar >> PortInfo[i].m_strPortType;
		ar >> PortInfo[i].m_strBasePortName;
		ar >> PortInfo[i].m_strPortRegion;

		PortInfo[i].required_cargoes.coal = GetIntFromArchive(ar);
		PortInfo[i].required_cargoes.general = GetIntFromArchive(ar);
		PortInfo[i].required_cargoes.oil = GetIntFromArchive(ar);
		PortInfo[i].required_cargoes.vegetables_fruits = GetIntFromArchive(ar);

		PortInfo[i].available_cargoes.coal = GetIntFromArchive(ar);
		PortInfo[i].available_cargoes.general = GetIntFromArchive(ar);
		PortInfo[i].available_cargoes.oil = GetIntFromArchive(ar);
		PortInfo[i].available_cargoes.vegetables_fruits = GetIntFromArchive(ar);

		PortInfo[i].ship_loading_productivity.coal = GetIntFromArchive(ar);
		PortInfo[i].ship_loading_productivity.general = GetIntFromArchive(ar);
		PortInfo[i].ship_loading_productivity.oil = GetIntFromArchive(ar);
		PortInfo[i].ship_loading_productivity.vegetables_fruits = GetIntFromArchive(ar);

		PortInfo[i].acv_loading_productivity.coal = GetIntFromArchive(ar);
		PortInfo[i].acv_loading_productivity.general = GetIntFromArchive(ar);
		PortInfo[i].acv_loading_productivity.oil = GetIntFromArchive(ar);
		PortInfo[i].acv_loading_productivity.vegetables_fruits = GetIntFromArchive(ar);

		//Land begin
		PortLandInfo[i].m_nLandStructCalcActivateCheck = GetIntFromArchive(ar);
		PortLandInfo[i].m_nACVCourtAvailableCheck = GetIntFromArchive(ar);
		PortLandInfo[i].m_nACVCourtInvestPP = GetIntFromArchive(ar);
		PortLandInfo[i].m_nShipCourtAvailableCheck = GetIntFromArchive(ar);
		PortLandInfo[i].m_nShipCourtInvestPP = GetIntFromArchive(ar);
		PortLandInfo[i].m_nACVSpace = GetIntFromArchive(ar);
		PortLandInfo[i].m_nACVSpaceElemPrice = GetIntFromArchive(ar);
		PortLandInfo[i].m_nMoorLength = GetIntFromArchive(ar);
		PortLandInfo[i].m_nMoorElemPrice = GetIntFromArchive(ar);
		PortLandInfo[i].m_structRepairDetailsACV.n = GetIntFromArchive(ar);
		PortLandInfo[i].m_structRepairDetailsACV.Pgr = GetIntFromArchive(ar);
		PortLandInfo[i].m_structRepairDetailsACV.a = GetIntFromArchive(ar);
		PortLandInfo[i].m_structRepairDetailsACV.b = GetIntFromArchive(ar);
		PortLandInfo[i].m_structRepairDetailsShip.n = GetIntFromArchive(ar);
		PortLandInfo[i].m_structRepairDetailsShip.Pgr = GetIntFromArchive(ar);
		PortLandInfo[i].m_structRepairDetailsShip.a = GetIntFromArchive(ar);
		PortLandInfo[i].m_structRepairDetailsShip.b = GetIntFromArchive(ar);
		//Land end

	}

	ar.Close();
	f.Close();
	
	/////////////// Rewriting (Updating) Archive file ///////////////
//	char strTmp[5];
	int numTmp = 0;

	CFile f1( m_pPortsDBFile, CFile::modeCreate | CFile::modeWrite);
	CArchive ar1(&f1, CArchive::store);

	int newRecNum = m_RecordsNumber - 1;
	PutIntToArchive(newRecNum, ar1);

	for (int j = 1; j <= m_RecordsNumber; j++){
		if (j == m_SelectedPortID)
			continue;
		ar1 << PortInfo[j].m_strPortName;
		ar1 << PortInfo[j].m_strPortType;
		ar1 << PortInfo[j].m_strBasePortName;
		ar1 << PortInfo[j].m_strPortRegion;

		PutIntToArchive(PortInfo[j].required_cargoes.coal, ar1);
		PutIntToArchive(PortInfo[j].required_cargoes.general, ar1);
		PutIntToArchive(PortInfo[j].required_cargoes.oil, ar1);
		PutIntToArchive(PortInfo[j].required_cargoes.vegetables_fruits, ar1);
		PutIntToArchive(PortInfo[j].available_cargoes.coal, ar1);
		PutIntToArchive(PortInfo[j].available_cargoes.general, ar1);
		PutIntToArchive(PortInfo[j].available_cargoes.oil, ar1);
		PutIntToArchive(PortInfo[j].available_cargoes.vegetables_fruits, ar1);

		PutIntToArchive(PortInfo[j].ship_loading_productivity.coal, ar1);
		PutIntToArchive(PortInfo[j].ship_loading_productivity.general, ar1);
		PutIntToArchive(PortInfo[j].ship_loading_productivity.oil, ar1);
		PutIntToArchive(PortInfo[j].ship_loading_productivity.vegetables_fruits, ar1);

		PutIntToArchive(PortInfo[j].acv_loading_productivity.coal, ar1);
		PutIntToArchive(PortInfo[j].acv_loading_productivity.general, ar1);
		PutIntToArchive(PortInfo[j].acv_loading_productivity.oil, ar1);
		PutIntToArchive(PortInfo[j].acv_loading_productivity.vegetables_fruits, ar1);

		//Land begin
		PutIntToArchive(PortLandInfo[j].m_nLandStructCalcActivateCheck, ar1);
		PutIntToArchive(PortLandInfo[j].m_nACVCourtAvailableCheck, ar1);
		PutIntToArchive(PortLandInfo[j].m_nACVCourtInvestPP, ar1);
		PutIntToArchive(PortLandInfo[j].m_nShipCourtAvailableCheck, ar1);
		PutIntToArchive(PortLandInfo[j].m_nShipCourtInvestPP, ar1);
		PutIntToArchive(PortLandInfo[j].m_nACVSpace, ar1);
		PutIntToArchive(PortLandInfo[j].m_nACVSpaceElemPrice, ar1);
		PutIntToArchive(PortLandInfo[j].m_nMoorLength, ar1);
		PutIntToArchive(PortLandInfo[j].m_nMoorElemPrice, ar1);
		PutIntToArchive(PortLandInfo[j].m_structRepairDetailsACV.n, ar1);
		PutIntToArchive(PortLandInfo[j].m_structRepairDetailsACV.Pgr, ar1);
		PutIntToArchive(PortLandInfo[j].m_structRepairDetailsACV.a, ar1);
		PutIntToArchive(PortLandInfo[j].m_structRepairDetailsACV.b, ar1);
		PutIntToArchive(PortLandInfo[j].m_structRepairDetailsShip.n, ar1);
		PutIntToArchive(PortLandInfo[j].m_structRepairDetailsShip.Pgr, ar1);
		PutIntToArchive(PortLandInfo[j].m_structRepairDetailsShip.a, ar1);
		PutIntToArchive(PortLandInfo[j].m_structRepairDetailsShip.b, ar1);
		//Land end
		
	}

	ar1.Close();
	f1.Close();

	UpdateEditPortDistMatrixInDB();

	m_RecordsNumber--; 
	
	OnPortWizRefresh();
	
}


int CPortWizardDlg::GetIntFromArchive(CArchive& ar)
{
	char *stopstring;
	CString strData;
	int intData;

	ar >> strData;
	intData = (int)strtod(strData,&stopstring);

	return intData;
}

void CPortWizardDlg::PutIntToArchive(int intData, CArchive& ar1)
{
	char strData[5];
	CString strText;

	_itoa(intData, strData, 10);
	strText = strData;
	strText.Format("%s", strText);   
	ar1 << strText;
}




void CPortWizardDlg::UpdateEditPortDistMatrixInDB()
{

	CFile f;
	CFileException e;
	CString strMatrixValue;
	int i, j;

	int PortDistMatrix[MAX_PORT_RECORDS_IN_DB][MAX_PORT_RECORDS_IN_DB];

	int NewRecordsNumber = m_RecordsNumber - 1; // Один порт удаляется


    //////// Запись архива в массив  PortDistMatrix[i,j] /////////
	f.Open( m_pPortDistMatrixFile, CFile::modeRead, &e);
	CArchive arPortDist(&f, CArchive::load);

	for (i = 1; i <= m_RecordsNumber; i++)
		for (j = 1; j <= i; j++)
			PortDistMatrix[i][j] = GetIntFromArchive(arPortDist);

	arPortDist.Close();
	f.Close();


	//////// Корректировка массива PortDistMatrix[i,j] /////////

	for (i = 1; i <= NewRecordsNumber ; i++){
		if (i < m_SelectedPortID)
			continue;
		for (j = 1; j <= i; j++){
			if (j < m_SelectedPortID)
				PortDistMatrix[i][j] = PortDistMatrix[i+1][j];
			else
				PortDistMatrix[i][j] = PortDistMatrix[i+1][j+1];
		}
	}


	//////// Перезапись архива /////////
	
	f.Open( m_pPortDistMatrixFile, CFile::modeCreate | CFile::modeWrite, &e);
	CArchive arPortDist1(&f, CArchive::store);
	
	for (i = 1; i <= NewRecordsNumber ; i++)
		for (j = 1; j <= i; j++)
			PutIntToArchive(PortDistMatrix[i][j], arPortDist1);
	
	arPortDist1.Close();
	f.Close();

}



void CPortWizardDlg::OnPortWizRefresh() 
{
	CleanGenInfoGrid();
	FillGenInfoGrid();
	OnClickPortwizList();	
}

void CPortWizardDlg::CleanGenInfoGrid()
{
	int i,j,max;
	
	max = m_RecordsNumber+1;

	for (i = 1; i <= max; i++)
		for (j = 0; j <= 37; j++)	//Cargoes set: 5...20; Land Struct: 21...37
			m_PortGenInfoGrid.SetTextMatrix(i,j,"");

	// Матрица расстояний:
	for (i = 1; i <= max; i++)
		for (j = 1; j <= i; j++)
			m_PortsDistMatrixInfoGrid.SetTextMatrix(i,j,"");

}
