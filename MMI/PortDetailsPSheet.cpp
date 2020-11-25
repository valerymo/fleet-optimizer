// PortDetailsPSheet.cpp : implementation file
//

#include "stdafx.h"
//#include "	\ add additional includes here"
#include "PortDetailsPSheet.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CPortDetailsPSheet

IMPLEMENT_DYNAMIC(CPortDetailsPSheet, CPropertySheet)

CPortDetailsPSheet::CPortDetailsPSheet(UINT nIDCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(nIDCaption, pParentWnd, iSelectPage)
//	:CPropertySheet(nIDCaption, NULL, 0)
{
	AddControlPages();

	m_pParentWnd = pParentWnd;
	m_CPortDistMatrixPPage.SetParentPtr(this);
	m_CPortMainDetailsPPage.SetParentPtr(this);
	m_PortLandStructPPage.SetParentPtr(this);
	m_SelectedPortMainInfo = NULL;
	m_SelectedPortLandStructInfo = NULL;
	//m_UpdatedPortInfo = NULL;

	m_OpenStat = 0;
	m_ISOK = FALSE;
	m_pProjectFile1 = "port_log1.fl";
	//m_pPortsDBFile = "ports_db.fd";
	//m_pPortDistMatrixFile = "ports_dist.fl";
	CString str = ((CFleetOptimizerApp*)AfxGetApp())->GetProfileString("FleetOptimizer","DbPorts");
	m_pPortsDBFile = _strdup(str.GetBuffer(20));
	str = ((CFleetOptimizerApp*)AfxGetApp())->GetProfileString("FleetOptimizer","DbPortsDistMatrix");
	m_pPortDistMatrixFile = _strdup(str.GetBuffer(20));
	
}


CPortDetailsPSheet::CPortDetailsPSheet(LPCTSTR pszCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(pszCaption, pParentWnd, iSelectPage)
//	:CPropertySheet(pszCaption, NULL, 0)
{
	AddControlPages();

	m_pParentWnd = pParentWnd;
	m_CPortDistMatrixPPage.SetParentPtr(this);
	m_CPortMainDetailsPPage.SetParentPtr(this);
	m_PortLandStructPPage.SetParentPtr(this);
	m_SelectedPortMainInfo = NULL;
	m_SelectedPortLandStructInfo = NULL;
	//m_UpdatedPortInfo = NULL;
	m_OpenStat = 0;
	m_pProjectFile1 = "port_log1.fl";
	//m_pPortsDBFile = "ports_db.fd";
	//m_pPortDistMatrixFile = "ports_dist.fl";
	CString str = ((CFleetOptimizerApp*)AfxGetApp())->GetProfileString("FleetOptimizer","DbPorts");
	m_pPortsDBFile = _strdup(str.GetBuffer(20));
	str = ((CFleetOptimizerApp*)AfxGetApp())->GetProfileString("FleetOptimizer","DbPortsDistMatrix");
	m_pPortDistMatrixFile = _strdup(str.GetBuffer(20));
	
}

CPortDetailsPSheet::~CPortDetailsPSheet()
{
}


BEGIN_MESSAGE_MAP(CPortDetailsPSheet, CPropertySheet)
	//{{AFX_MSG_MAP(CPortDetailsPSheet)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDOK, OnOK)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPortDetailsPSheet message handlers

void CPortDetailsPSheet::AddControlPages()
{

	AddPage(&m_CPortMainDetailsPPage);
	AddPage(&m_CPortDistMatrixPPage);
	AddPage(&m_PortLandStructPPage);

}


BOOL CPortDetailsPSheet::OnInitDialog() 
{
	BOOL bResult = CPropertySheet::OnInitDialog();
	
/*	if (m_OpenStat == 1){ //view details
		CFile f( m_pPortsDBFile, CFile::modeRead );
		CArchive ar(&f, CArchive::load);
		Serialize(ar);

		m_OpenStat = 0;
	}
*/

	
	//Test1();
	
	return bResult;
}

void CPortDetailsPSheet::DoDataExchange(CDataExchange* pDX) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	CPropertySheet::DoDataExchange(pDX);
/*	
	if (m_ISOK == TRUE){
		if (m_OpenStat == 3){ //New Port
			CFile f( m_pPortsDBFile, CFile::modeWrite );
			f.SeekToBegin();
			CArchive ar(&f, CArchive::store);
			Serialize(ar);
			m_OpenStat = 0;
		}
		m_ISOK = FALSE;
	}
	
*/
//	Test1();
}



void CPortDetailsPSheet::Serialize(CArchive& ar) 
{
	CPropertySheet::Serialize(ar);
	m_CPortMainDetailsPPage.Serialize(ar);
	SetActivePage(2);
	m_PortLandStructPPage.Serialize(ar);
	SetActivePage(0);

	if (m_OpenStat == 3){
		if (m_CPortDistMatrixPPage.m_bInitialized){
			//SetActivePage(1);
			CFile f;
			CFileException e;
			f.Open( m_pPortDistMatrixFile, CFile::modeWrite, &e);
			f.SeekToBegin();
			CArchive arPortDist(&f, CArchive::store);
			m_CPortDistMatrixPPage.Serialize(arPortDist);
			arPortDist.Close();
			f.Close();
			//SetActivePage(0);
		}
	}


	if (ar.IsStoring())
	{	// storing code
	}
	else
	{	// loading code
	}
}

void CPortDetailsPSheet::OnOK()
{
	CFile f;
	CFileException e;
	
	//SetActivePage(1);
	//SetActivePage(2);
	//SetActivePage(0);	
	CheckPPagesData();

/*	if (m_CPortMainDetailsPPage.m_ISModifiable == TRUE){
		CFile f( m_pProjectFile1, CFile::modeCreate | CFile::modeWrite );
		CArchive ar(&f, CArchive::store);
		Serialize(ar);
	}
*/
	if (m_OpenStat == 3){ //New Port
		UpdatePortInDB();
		//if (m_CPortDistMatrixPPage.m_bInitialized)
		//	UpdateEditPortDistMatrixInDB();

		//RecordsNumIncrement();
		m_RecordsNumber++;
		f.Open( m_pPortsDBFile, CFile::modeWrite, &e);
		f.SeekToEnd();
		CArchive ar(&f, CArchive::store);
		Serialize(ar);
		ar.Close();
		m_OpenStat = 0;
		f.Close();
	}

	if (m_OpenStat == 2) { //Edit Port
		UpdatePortInDB();
		
		if (m_CPortDistMatrixPPage.m_bInitialized)
			UpdateEditPortDistMatrixInDB();
	}
		
	//	if (!DoApply())
	//      return;
	// Close property sheet and return IDOK
	//m_ISOK = TRUE;
	//f.Close();
	EndDialog(IDOK);
	
}

void CPortDetailsPSheet::Test1()
{
		
		CString strMatrixValue;
		strMatrixValue = "12345";

/*		
		m_CPortMainDetailsPPage.m_strBasePortName = _T("Providenia");
		m_CPortMainDetailsPPage.m_strPortType = "BBB";
		m_CPortMainDetailsPPage.m_strPortRegion = "Arctic";
		m_CPortMainDetailsPPage.m_strPortName = "Uelen";
*/		

		m_CPortMainDetailsPPage.m_CargoInfoGrid.SetTextMatrix(1,1,strMatrixValue);	//required_cargoes.coal
		m_CPortMainDetailsPPage.m_CargoInfoGrid.SetTextMatrix(2,1,strMatrixValue);	//required_cargoes.general
		m_CPortMainDetailsPPage.m_CargoInfoGrid.SetTextMatrix(3,1,strMatrixValue);	//required_cargoes.oil
		m_CPortMainDetailsPPage.m_CargoInfoGrid.SetTextMatrix(4,1,strMatrixValue);	//required_cargoes.vegetables_fruits
		m_CPortMainDetailsPPage.m_CargoInfoGrid.SetTextMatrix(1,2,strMatrixValue);	//available_cargoes.coal
		m_CPortMainDetailsPPage.m_CargoInfoGrid.SetTextMatrix(2,2,strMatrixValue);	//available_cargoes.general
		m_CPortMainDetailsPPage.m_CargoInfoGrid.SetTextMatrix(3,2,strMatrixValue);	//available_cargoes.oil
		m_CPortMainDetailsPPage.m_CargoInfoGrid.SetTextMatrix(4,2,strMatrixValue);	//available_cargoes.vegetables_fruits
/*
	if (m_OpenStat == 1){ //Details
		m_CPortMainDetailsPPage.m_strPortRegion = _T(m_CPortMainDetailsPPage.m_SelectedPortMainInfo->m_strPortRegion);
		m_CPortMainDetailsPPage.m_strPortName = _T(m_CPortMainDetailsPPage.m_SelectedPortMainInfo->m_strPortName);
		m_CPortMainDetailsPPage.m_strPortType = _T(m_CPortMainDetailsPPage.m_SelectedPortMainInfo->m_strPortType);
		m_CPortMainDetailsPPage.m_strBasePortName = _T(m_CPortMainDetailsPPage.m_SelectedPortMainInfo->m_strBasePortName);
	}
*/

}

void CPortDetailsPSheet::RecordsNumIncrement(void){
	CFile f;
	CFileException e;
	char strBuf[3];

	f.Open( m_pPortsDBFile, CFile::modeWrite, &e);
	f.SeekToBegin();
	CArchive ar(&f, CArchive::store);
	m_RecordsNumber++;
	_itoa( m_RecordsNumber, strBuf, 10 );
	CString strNum = strBuf;
	ar << strNum;
	ar.Close();
	f.Close();

}


int CPortDetailsPSheet::GetRecordsNumber()
{
	return m_RecordsNumber;
}

CWnd* CPortDetailsPSheet::GetParentWndPoint()
{
	return m_pParentWnd;
}

void CPortDetailsPSheet::SetSelectedPortMainInfo(CPortMainInfo *portInfo)
{
	m_SelectedPortMainInfo = portInfo;
}

void CPortDetailsPSheet::SetSelectedPortLandStructInfo(CPortLandStructInfo *portLandStructInfo)
{
	m_SelectedPortLandStructInfo = portLandStructInfo;
}

void CPortDetailsPSheet::UpdatePortInDB()
{
	CString strNum;
	CString strText;
	int RecNum;

	SetUpdatedPortInfo();
	
	CPortMainInfo PortInfo[MAX_PORT_RECORDS_IN_DB];
	CPortLandStructInfo PortLandInfo[MAX_PORT_RECORDS_IN_DB];

	if (m_RecordsNumber >= MAX_PORT_RECORDS_IN_DB)
		AfxMessageBox("Количество портов в Базе Данных превысило установки программы.\n значение MAX_PORT_RECORDS_IN_DB должно быть увеличено в программе");
	
	CFile f( m_pPortsDBFile, CFile::modeRead );
	CArchive ar(&f, CArchive::load);

	if ((m_OpenStat == 3)&&(m_RecordsNumber == 0))
		RecNum = 0;
	else
		RecNum = GetIntFromArchive(ar);
	
	for (int i = 1; i <= RecNum; i++){
	
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

		if ((m_OpenStat != 3) && (i == m_SelectedPortID)){ 
		//only for Edit; NOT for New Port
		//if (i == m_SelectedPortID){
			PortInfo[i] = m_UpdatedPortInfo ;
			PortLandInfo[i] = m_UpdatedPortLandInfo;
		}

	}

	ar.Close();
	f.Close();

	/////////////// Rewriting (Updating) Archive file ///////////////
	int numTmp = 0;

	CFile f1( m_pPortsDBFile, CFile::modeCreate | CFile::modeWrite);
	CArchive ar1(&f1, CArchive::store);

	if (m_OpenStat == 3){//New port add
		RecNum = m_RecordsNumber+1;
	}

	PutIntToArchive(RecNum, ar1);

	for (int j = 1; j <= m_RecordsNumber; j++){
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

	//CPropertySheet::Serialize(ar1);
	//m_CPortDistMatrixPPage.Serialize(ar1);

	ar1.Close();
	f1.Close();

}

void CPortDetailsPSheet::SetUpdatedPortInfo()
{
	CString strNum;
	CString strText;
	char *stopstring;

	
	m_CPortMainDetailsPPage.m_ctrlPortName.GetWindowText(strText);
	m_UpdatedPortInfo.m_strPortName = strText;	
	m_CPortMainDetailsPPage.m_ctrlPortType.GetWindowText(strText);
	m_UpdatedPortInfo.m_strPortType = strText;
	m_CPortMainDetailsPPage.m_ctrlBasePortName.GetWindowText(strText);
	m_UpdatedPortInfo.m_strBasePortName = strText;
	m_CPortMainDetailsPPage.m_ctrlPortRegion.GetWindowText(strText);
	m_UpdatedPortInfo.m_strPortRegion = strText;

	strNum = m_CPortMainDetailsPPage.m_CargoInfoGrid.GetTextMatrix(1,1);	//required_cargoes.coal
	m_UpdatedPortInfo.required_cargoes.coal =  (int)strtod(strNum,&stopstring);
	strNum = m_CPortMainDetailsPPage.m_CargoInfoGrid.GetTextMatrix(2,1);	//required_cargoes.general
	m_UpdatedPortInfo.required_cargoes.general = (int)strtod(strNum,&stopstring);
	strNum = m_CPortMainDetailsPPage.m_CargoInfoGrid.GetTextMatrix(3,1);	//required_cargoes.oil
	m_UpdatedPortInfo.required_cargoes.oil =  (int)strtod(strNum,&stopstring);
	strNum = m_CPortMainDetailsPPage.m_CargoInfoGrid.GetTextMatrix(4,1);	//required_cargoes.vegetables_fruits
	m_UpdatedPortInfo.required_cargoes.vegetables_fruits = (int)strtod(strNum,&stopstring);
	strNum = m_CPortMainDetailsPPage.m_CargoInfoGrid.GetTextMatrix(1,2);	//available_cargoes.coal
	m_UpdatedPortInfo.available_cargoes.coal = (int)strtod(strNum,&stopstring);
	strNum = m_CPortMainDetailsPPage.m_CargoInfoGrid.GetTextMatrix(2,2);	//available_cargoes.general
	m_UpdatedPortInfo.available_cargoes.general = (int)strtod(strNum,&stopstring);
	strNum = m_CPortMainDetailsPPage.m_CargoInfoGrid.GetTextMatrix(3,2);	//available_cargoes.oil
	m_UpdatedPortInfo.available_cargoes.oil = (int)strtod(strNum,&stopstring);
	strNum = m_CPortMainDetailsPPage.m_CargoInfoGrid.GetTextMatrix(4,2);	//available_cargoes.vegetables_fruits
	m_UpdatedPortInfo.available_cargoes.vegetables_fruits = (int)strtod(strNum,&stopstring);

	strNum = m_CPortMainDetailsPPage.m_CargoInfoGrid.GetTextMatrix(1,3);	//ship_loading_productivity.coal
	m_UpdatedPortInfo.ship_loading_productivity.coal = (int)strtod(strNum,&stopstring);
	strNum = m_CPortMainDetailsPPage.m_CargoInfoGrid.GetTextMatrix(2,3);	//ship_loading_productivity.general
	m_UpdatedPortInfo.ship_loading_productivity.general = (int)strtod(strNum,&stopstring);
	strNum = m_CPortMainDetailsPPage.m_CargoInfoGrid.GetTextMatrix(3,3);	//ship_loading_productivity.oil
	m_UpdatedPortInfo.ship_loading_productivity.oil = (int)strtod(strNum,&stopstring);
	strNum = m_CPortMainDetailsPPage.m_CargoInfoGrid.GetTextMatrix(4,3);	//ship_loading_productivity.vegetables_fruits
	m_UpdatedPortInfo.ship_loading_productivity.vegetables_fruits = (int)strtod(strNum,&stopstring);

	strNum = m_CPortMainDetailsPPage.m_CargoInfoGrid.GetTextMatrix(1,4);	//acv_loading_productivity.coal
	m_UpdatedPortInfo.acv_loading_productivity.coal = (int)strtod(strNum,&stopstring);
	strNum = m_CPortMainDetailsPPage.m_CargoInfoGrid.GetTextMatrix(2,4);	//acv_loading_productivity.general
	m_UpdatedPortInfo.acv_loading_productivity.general = (int)strtod(strNum,&stopstring);
	strNum = m_CPortMainDetailsPPage.m_CargoInfoGrid.GetTextMatrix(3,4);	//acv_loading_productivity.oil
	m_UpdatedPortInfo.acv_loading_productivity.oil = (int)strtod(strNum,&stopstring);
	strNum = m_CPortMainDetailsPPage.m_CargoInfoGrid.GetTextMatrix(4,4);	//acv_loading_productivity.vegetables_fruits
	m_UpdatedPortInfo.acv_loading_productivity.vegetables_fruits = (int)strtod(strNum,&stopstring);

	//Land Struct begin
	SetActivePage(2);

	m_UpdatedPortLandInfo.m_nLandStructCalcActivateCheck = m_PortLandStructPPage.m_ctrlLandStructCalcActivate.GetCheck();
	m_UpdatedPortLandInfo.m_nACVCourtAvailableCheck = m_PortLandStructPPage.m_ctrlACVCourtAvailable.GetCheck();

	m_PortLandStructPPage.m_ctrlACVCourtInvestPP.GetWindowText(strNum);
	m_UpdatedPortLandInfo.m_nACVCourtInvestPP = (int)strtod(strNum,&stopstring);

	m_UpdatedPortLandInfo.m_nShipCourtAvailableCheck = m_PortLandStructPPage.m_ctrlShipCourtAvailable.GetCheck();

	m_PortLandStructPPage.m_ctrlShipCourtInvestPP.GetWindowText(strNum);
	m_UpdatedPortLandInfo.m_nShipCourtInvestPP = (int)strtod(strNum,&stopstring);

	m_PortLandStructPPage.m_ctrlACVSpace.GetWindowText(strNum);
	m_UpdatedPortLandInfo.m_nACVSpace = (int)strtod(strNum,&stopstring);

	m_PortLandStructPPage.m_ctrlACVSpaceElemPrice.GetWindowText(strNum);
	m_UpdatedPortLandInfo.m_nACVSpaceElemPrice = (int)strtod(strNum,&stopstring);

	m_PortLandStructPPage.m_ctrlMoorLength.GetWindowText(strNum);
	m_UpdatedPortLandInfo.m_nMoorLength = (int)strtod(strNum,&stopstring);

	m_PortLandStructPPage.m_ctrlMoorElemPrice.GetWindowText(strNum);
	m_UpdatedPortLandInfo.m_nMoorElemPrice = (int)strtod(strNum,&stopstring);

	
	strNum = m_PortLandStructPPage.m_ctrlRepairDetailsGrid.GetTextMatrix(1,1);	// СВП. Кол-во СВП, которое может принять существующая ремонтная база
	m_UpdatedPortLandInfo.m_structRepairDetailsACV.n = (int)strtod(strNum,&stopstring);
	
	strNum = m_PortLandStructPPage.m_ctrlRepairDetailsGrid.GetTextMatrix(1,2);	// СВП. Грузоподъемность СВП, для которого приводятся данные (кол-во и коэффициенты)
	m_UpdatedPortLandInfo.m_structRepairDetailsACV.Pgr = (int)strtod(strNum,&stopstring);

	strNum = m_PortLandStructPPage.m_ctrlRepairDetailsGrid.GetTextMatrix(1,3);	// СВП. Стоимость достройки ремонтной базы для одного СВП (указанной грузоподъемности)
	m_UpdatedPortLandInfo.m_structRepairDetailsACV.a = (int)strtod(strNum,&stopstring);


	strNum = m_PortLandStructPPage.m_ctrlRepairDetailsGrid.GetTextMatrix(1,4);	// СВП. Стоимость достройки не зависяшая от количества СВП
	m_UpdatedPortLandInfo.m_structRepairDetailsACV.b = (int)strtod(strNum,&stopstring);
	
	strNum = m_PortLandStructPPage.m_ctrlRepairDetailsGrid.GetTextMatrix(2,1);	// Судно. Кол-во судов, которое может принять существующая ремонтная база
	m_UpdatedPortLandInfo.m_structRepairDetailsShip.n = (int)strtod(strNum,&stopstring);

	strNum = m_PortLandStructPPage.m_ctrlRepairDetailsGrid.GetTextMatrix(2,2);	// Судно. Грузоподъемность судна, для которого приводятся данные (кол-во и коэффициенты)
	m_UpdatedPortLandInfo.m_structRepairDetailsShip.Pgr = (int)strtod(strNum,&stopstring);

	strNum = m_PortLandStructPPage.m_ctrlRepairDetailsGrid.GetTextMatrix(2,3);	// Судно. Стоимость достройки ремонтной базы для одного судна (указанной грузоподъемности)
	m_UpdatedPortLandInfo.m_structRepairDetailsShip.a = (int)strtod(strNum,&stopstring);

	strNum = m_PortLandStructPPage.m_ctrlRepairDetailsGrid.GetTextMatrix(2,4);	// Судно. Стоимость достройки не зависяшая от количества судов
	m_UpdatedPortLandInfo.m_structRepairDetailsShip.b = (int)strtod(strNum,&stopstring);

	SetActivePage(0);
	//Land end
	

}

int CPortDetailsPSheet::GetIntFromArchive(CArchive& ar)
{
	char *stopstring;
	CString strData;
	int intData;

	ar >> strData;
	intData = (int)strtod(strData,&stopstring);

	return intData;
}

void CPortDetailsPSheet::PutIntToArchive(int intData, CArchive& ar1)
{
	char strData[5];
	CString strText;

	_itoa(intData, strData, 10);
	strText = strData;
	strText.Format("%s", strText);
	strText.TrimLeft();
	ar1 << strText;
}



void CPortDetailsPSheet::UpdateEditPortDistMatrixInDB()
{

	CFile f;
	CFileException e;
	CString strTmp;

	SetActivePage(1);

	f.Open( m_pPortDistMatrixFile, CFile::modeWrite, &e);
	f.SeekToBegin();
	CArchive arPortDist(&f, CArchive::store);

//	m_CPortDistMatrixPPage.Serialize(arPortDist1);
	m_CPortDistMatrixPPage.m_bInitialized = TRUE;
	for (int i = 1; i <= m_RecordsNumber; i++)
		for (int j = 1; j <= i; j++){
			strTmp = m_CPortDistMatrixPPage.m_PortDistMatrixGrid.GetTextMatrix(i,j);
			arPortDist << strTmp;
		}
		
	arPortDist.Close();
	f.Close();

	SetActivePage(0);

}

/*


void CPortDetailsPSheet::UpdateEditPortDistMatrixInDB()
{

	CFile f;
	CFileException e;
	CString strMatrixValue;
	int i, j;

	int PortDistMatrix[MAX_PORT_RECORDS_IN_DB][MAX_PORT_RECORDS_IN_DB];

	int PortsNumber = m_RecordsNumber - 1; // Один порт удаляется


    //////// Запись архива в массив  PortDistMatrix[i,j] /////////
	f.Open( m_pPortDistMatrixFile, CFile::modeRead, &e);
	CArchive arPortDist(&f, CArchive::load);

	for (i = 1; i < m_RecordsNumber; i++)
		for (j = 1; j <= i; j++)
			PortDistMatrix[i][j] = GetIntFromArchive(arPortDist);

	arPortDist.Close();
	f.Close();


	//////// Корректировка массива PortDistMatrix[i,j] /////////

	for (i = 1; i < PortsNumber ; i++){
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
	
	f.Open( m_pPortDistMatrixFile, CFile::modeWrite, &e);
	CArchive arPortDist1(&f, CArchive::store);
	
	for (i = 1; i < PortsNumber ; i++)
		for (j = 1; j <= i; j++)
			PutIntToArchive(PortDistMatrix[i][j], arPortDist1);
	
	arPortDist1.Close();
	f.Close();

}


*/

int CPortDetailsPSheet::CheckPPagesData(void){

/*	if (m_OpenStat == 3){	//New port
		SetActivePage(1);
		SetActivePage(2);
		SetActivePage(0);
	}
*/
	return 1;
}