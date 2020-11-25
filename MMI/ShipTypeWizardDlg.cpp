// ShipTypeWizardDlg.cpp : implementation file
//

#include "stdafx.h"
//#include "	\ add additional includes here"
#include "ShipTypeWizardDlg.h"
#include "ShipTypePropertySheet.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CShipTypeWizardDlg dialog


CShipTypeWizardDlg::CShipTypeWizardDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CShipTypeWizardDlg::IDD, pParent)
{

	//{{AFX_DATA_INIT(CShipTypeWizardDlg)
	//}}AFX_DATA_INIT
	//m_ctrlCargoCompatiblGeneral.SetState(0);

	//m_pShipsDBFile = "ships_db.fd";
	CString str = ((CFleetOptimizerApp*)AfxGetApp())->GetProfileString("FleetOptimizer","DbShips");
	m_pShipsDBFile = _strdup(str.GetBuffer(20));

	m_nID = CShipTypeWizardDlg::IDD;
	CDialog::Create(m_nID, pParent);

	m_pShipProtoWizDlg = NULL;
	m_pACVProtoWizDlg = NULL;

}


void CShipTypeWizardDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CShipTypeWizardDlg)
	DDX_Control(pDX, IDC_SHIPWIZS_SPEED_TEXT, m_ctrlSpeed);
	DDX_Control(pDX, IDC_CHECK3, m_ctrlCargoCompatiblVegFruits);
	DDX_Control(pDX, IDC_CHECK4, m_ctrlCargoCompatiblCoal);
	DDX_Control(pDX, IDC_CHECK2, m_ctrlCargoCompatiblOil);
	DDX_Control(pDX, IDC_CHECK1, m_ctrlCargoCompatiblGeneral);
	DDX_Control(pDX, IDC_SHIPTYPE_T_TEXT, m_ctrlT);
	DDX_Control(pDX, IDC_SHIPTYPE_L_TEXT, m_ctrlL);
	DDX_Control(pDX, IDC_SHIPTYPE_B_TEXT, m_ctrlB);
	DDX_Control(pDX, IDC_SHIPWIZPROTO_TEXT, m_ctrlPrototypeName);
	DDX_Control(pDX, IDC_SHIPWIZSLOAD_TEXT, m_ctrlLoad);
	DDX_Control(pDX, IDC_SHIPWIZREGISTERCLASS_TEXT, m_ctrlRegisterClass);
	DDX_Control(pDX, IDC_SHIPWIZTYPENAME_TEXT, m_ctrlShipTypeName);
	DDX_Control(pDX, IDC_SHIPTYPEWIZ_LIST, m_ShipGenInfoGrid);
	//}}AFX_DATA_MAP

	//m_ctrlCargoCompatiblGeneral.EnableWindow(FALSE);
	//m_ctrlCargoCompatiblGeneral.SetCheck(1);

	FillGenInfoGrid();
}


BEGIN_MESSAGE_MAP(CShipTypeWizardDlg, CDialog)
	//{{AFX_MSG_MAP(CShipTypeWizardDlg)
	ON_BN_CLICKED(IDC_SHIPTYPEWIZ_EDIT, OnShiptypeWizEdit)
	ON_BN_CLICKED(IDC_SHIPTYPEWIZ_DETAILS, OnShiptypeWizDetails)
	ON_BN_CLICKED(IDC_SHIPTYPEWIZ_NEW, OnShiptypeWizNew)
	ON_BN_CLICKED(IDC_SHIPTYPEWIZ_DELETE, OnShiptypewizDelete)
	ON_BN_CLICKED(IDC_SHIPTYPEWIZ_REFRESH, OnShiptypeWizRefresh)
	ON_BN_CLICKED(IDC_SHIPTYPEWIZ_PROTO_SHIP, OnShipPrototypeWizButton)
	ON_BN_CLICKED(IDC_SHIPTYPEWIZ_PROTO_ACV, OnACVPrototypeWizButton)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CShipTypeWizardDlg message handlers

void CShipTypeWizardDlg::OnShiptypeWizEdit() 
{
	// TODO: Add your control notification handler code here
	CShipTypePropertySheet thePSheet(_T("Edit Ship Type Details"));

	FillSelectedShipInfo();
	thePSheet.m_SelectedShipMainInfo = m_SelectedShipMainInfo;
	thePSheet.m_OpenStat = 2;
	thePSheet.m_CShipTypeMainPPage.m_OpenStat = 2;
	thePSheet.m_CShipTypeOptimizParamPPage.m_OpenStat = 2;
	thePSheet.m_RecordsNumber = m_RecordsNumber;
	thePSheet.m_pShipsDBFile = m_pShipsDBFile;
	m_SelectedShipID = 	m_ShipGenInfoGrid.GetRow();
	thePSheet.m_SelectedShipID = m_SelectedShipID;
	//thePSheet.DoModal();
	if (thePSheet.DoModal() == IDOK){
		OnShiptypeWizRefresh();
	}	
	
}

void CShipTypeWizardDlg::OnShiptypeWizDetails() 
{
	// TODO: Add your control notification handler code here
	CShipTypePropertySheet thePSheet(_T("Характеристики Типа Судна"));
	FillSelectedShipInfo();
	thePSheet.m_SelectedShipMainInfo = m_SelectedShipMainInfo;
	thePSheet.m_OpenStat = 1;
	thePSheet.m_CShipTypeMainPPage.m_OpenStat = 1;
	thePSheet.m_CShipTypeOptimizParamPPage.m_OpenStat = 1;

	thePSheet.DoModal();	
}

void CShipTypeWizardDlg::OnShiptypeWizNew() 
{
	// TODO: Add your control notification handler code here
	CShipTypePropertySheet thePSheet(_T("New Ship Type Creation"));
	thePSheet.m_CShipTypeMainPPage.m_ISModifiable = TRUE;
	thePSheet.m_CShipTypeOptimizParamPPage.m_ISModifiable = TRUE;
	thePSheet.m_OpenStat = 3;
	thePSheet.m_CShipTypeMainPPage.m_OpenStat = 3;
	thePSheet.m_CShipTypeOptimizParamPPage.m_OpenStat = 3;
	thePSheet.m_RecordsNumber = m_RecordsNumber;
	thePSheet.m_pShipsDBFile = m_pShipsDBFile;

	if (thePSheet.DoModal() == IDOK){
		OnShiptypeWizRefresh();
	}
	
}

void CShipTypeWizardDlg::FillGenInfoGrid()
{
	CFile f( m_pShipsDBFile, CFile::modeRead );
	f.SeekToBegin();
	CArchive ar(&f, CArchive::load);
	Serialize(ar);
	ar.Close();
	f.Close();
}


void CShipTypeWizardDlg::Serialize(CArchive& ar) 
{
	if (ar.IsStoring())
	{	// storing code
	}
	else
	{	// loading code

		CString	strNum,	strShipTypeName, strRegisterClass, strCargoCompatiblGeneral,
			strCargoCompatiblOil, strCargoCompatiblCoal, strCargoCompatiblVegFruits,
			strShipLoadMin, strShipLoadMax, strShipLoadDelta,
			strShipTariffGenVF, strShipTariffOil, strShipTariffCoal,
			strProtoType, strDW, strL, strB, strT, strSpeed, strRange, strh3,
			strShipExistDesignCheckBox,	strShipExistBuildCheckBox, strShipExistCheckBox,strShipExistSetCheckBox,strShipExistForceCheckBox,
			strShipExistNumberComboBox, strShipExistAdvantPrice, strShipExistPrice, 
			strShipExistInBuildNumberComboBox, strShipExistBuildAdvantPrice, strShipExistBuildPrice,
			strShipExistDesignPrice, strShipExistDesignAdvantPrice;


		ULONGLONG positionEnd, position; 
		int nState;
		char *stopstring;

		
		CFile* fp = ar.GetFile();
		fp->SeekToEnd();
		positionEnd = fp->GetPosition();
		fp->SeekToBegin();
		position = fp->GetPosition();
		
		if (positionEnd == 0)
			m_RecordsNumber = 0;
		else {
				ar >> strNum;
				char *stopstring;
				m_RecordsNumber  = (int)strtod(strNum,&stopstring);
		}
		//m_RecordsNumber = 3;
		int i = 1;
		char strRowNum[5];
   
		while (i <= m_RecordsNumber){
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
			nState = (int)strtod(strCargoCompatiblGeneral,&stopstring);
			m_ctrlCargoCompatiblGeneral.SetCheck(nState);

			ar >> strCargoCompatiblOil;
			nState = (int)strtod(strCargoCompatiblOil,&stopstring);
			m_ctrlCargoCompatiblOil.SetCheck(nState);
			
			ar >> strCargoCompatiblCoal;
			nState = (int)strtod(strCargoCompatiblCoal,&stopstring);
			m_ctrlCargoCompatiblCoal.SetCheck(nState);
			
			ar >> strCargoCompatiblVegFruits;
			nState = (int)strtod(strCargoCompatiblVegFruits,&stopstring);
			m_ctrlCargoCompatiblVegFruits.SetCheck(nState);


			ar >> strShipLoadMin;
			ar >> strShipLoadMax;
			ar >> strShipLoadDelta;
			ar >> strShipTariffGenVF;
			ar >> strShipTariffOil;
			ar >> strShipTariffCoal;

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

			m_ShipGenInfoGrid.SetTextMatrix(0,1,"Ship Type");
			m_ShipGenInfoGrid.SetTextMatrix(0,2,"Classification");
			m_ShipGenInfoGrid.SetTextMatrix(0,3,"Prototype");
			m_ShipGenInfoGrid.SetTextMatrix(0,4,"   DW");
			m_ShipGenInfoGrid.SetTextMatrix(0,5,"Length");
			m_ShipGenInfoGrid.SetTextMatrix(0,6,"Beam");
			m_ShipGenInfoGrid.SetTextMatrix(0,7,"Draft");
			m_ShipGenInfoGrid.SetTextMatrix(0,8,"General cargo - compartibility");
			m_ShipGenInfoGrid.SetTextMatrix(0,9,"Oil - compartibility");
			m_ShipGenInfoGrid.SetTextMatrix(0,10,"Coal - compartibility");
			m_ShipGenInfoGrid.SetTextMatrix(0,11,"Veg-froots. - compartibility");
			m_ShipGenInfoGrid.SetTextMatrix(0,12,"Payload min");
			m_ShipGenInfoGrid.SetTextMatrix(0,13,"Payload max");
			m_ShipGenInfoGrid.SetTextMatrix(0,14,"Payload step");
			m_ShipGenInfoGrid.SetTextMatrix(0,15,"Rates - General cargo and Veg-froots");
			m_ShipGenInfoGrid.SetTextMatrix(0,16,"Rates - Oil");
			m_ShipGenInfoGrid.SetTextMatrix(0,17,"Rates - Coal");
			m_ShipGenInfoGrid.SetTextMatrix(0,18,"Speed, kn");
			m_ShipGenInfoGrid.SetTextMatrix(0,19,"Range, miles");
			m_ShipGenInfoGrid.SetTextMatrix(0,20,"h 3%");
			//begin:  ship exist (set titles)
			m_ShipGenInfoGrid.SetTextMatrix(0,21,"Ship exists (0,1)");
			m_ShipGenInfoGrid.SetTextMatrix(0,22,"Mandatory inclusion in the plan (0,1)");
			m_ShipGenInfoGrid.SetTextMatrix(0,23,"In Operation (0,1)");
			m_ShipGenInfoGrid.SetTextMatrix(0,24,"In Build (0,1)");
			m_ShipGenInfoGrid.SetTextMatrix(0,25,"In Design (0,1)");
			m_ShipGenInfoGrid.SetTextMatrix(0,26,"Number of ships in operation");
			m_ShipGenInfoGrid.SetTextMatrix(0,27,"Advantage (USD) of the ship in operation");
			m_ShipGenInfoGrid.SetTextMatrix(0,28,"Cost (USD) of the ship in operation");
			m_ShipGenInfoGrid.SetTextMatrix(0,29,"Number of ships in Build");
			m_ShipGenInfoGrid.SetTextMatrix(0,30,"Advantage (USD) of the ship in Build");
			m_ShipGenInfoGrid.SetTextMatrix(0,31,"Cost (USD) of the ship in Build");
			m_ShipGenInfoGrid.SetTextMatrix(0,32,"Advantage (USD) of the ship in Design");
			m_ShipGenInfoGrid.SetTextMatrix(0,33,"Cost (USD) of the ship in Design");
			//end:  ship exist (set titles)

			_itoa(i, strRowNum, 10);
			m_ShipGenInfoGrid.SetTextMatrix(i,0,strRowNum);

			m_ShipGenInfoGrid.SetTextMatrix(i,1,strShipTypeName);
			m_ShipGenInfoGrid.SetTextMatrix(i,2,strRegisterClass);
			m_ShipGenInfoGrid.SetTextMatrix(i,3,strProtoType);
			m_ShipGenInfoGrid.SetTextMatrix(i,4,strDW);
			m_ShipGenInfoGrid.SetTextMatrix(i,5,strL);
			m_ShipGenInfoGrid.SetTextMatrix(i,6,strB);
			m_ShipGenInfoGrid.SetTextMatrix(i,7,strT);
			m_ShipGenInfoGrid.SetTextMatrix(i,8,strCargoCompatiblGeneral);
			m_ShipGenInfoGrid.SetTextMatrix(i,9,strCargoCompatiblOil);
			m_ShipGenInfoGrid.SetTextMatrix(i,10,strCargoCompatiblCoal);
			m_ShipGenInfoGrid.SetTextMatrix(i,11,strCargoCompatiblVegFruits);
			m_ShipGenInfoGrid.SetTextMatrix(i,12,strShipLoadMin);
			m_ShipGenInfoGrid.SetTextMatrix(i,13,strShipLoadMax);
			m_ShipGenInfoGrid.SetTextMatrix(i,14,strShipLoadDelta);
			m_ShipGenInfoGrid.SetTextMatrix(i,15,strShipTariffGenVF);
			m_ShipGenInfoGrid.SetTextMatrix(i,16,strShipTariffOil);
			m_ShipGenInfoGrid.SetTextMatrix(i,17,strShipTariffCoal);
			m_ShipGenInfoGrid.SetTextMatrix(i,18,strSpeed);
			m_ShipGenInfoGrid.SetTextMatrix(i,19,strRange);
			m_ShipGenInfoGrid.SetTextMatrix(i,20,strh3);
			//begin:  ship exist 
			m_ShipGenInfoGrid.SetTextMatrix(i,21,strShipExistSetCheckBox);
			m_ShipGenInfoGrid.SetTextMatrix(i,22,strShipExistForceCheckBox);
			m_ShipGenInfoGrid.SetTextMatrix(i,23,strShipExistCheckBox);
			m_ShipGenInfoGrid.SetTextMatrix(i,24,strShipExistBuildCheckBox);
			m_ShipGenInfoGrid.SetTextMatrix(i,25,strShipExistDesignCheckBox);
			m_ShipGenInfoGrid.SetTextMatrix(i,26,strShipExistNumberComboBox);
			m_ShipGenInfoGrid.SetTextMatrix(i,27,strShipExistAdvantPrice);
			m_ShipGenInfoGrid.SetTextMatrix(i,28,strShipExistPrice);
			m_ShipGenInfoGrid.SetTextMatrix(i,29,strShipExistInBuildNumberComboBox);
			m_ShipGenInfoGrid.SetTextMatrix(i,30,strShipExistBuildAdvantPrice);
			m_ShipGenInfoGrid.SetTextMatrix(i,31,strShipExistBuildPrice);
			m_ShipGenInfoGrid.SetTextMatrix(i,32,strShipExistDesignAdvantPrice);
			m_ShipGenInfoGrid.SetTextMatrix(i,33,strShipExistDesignPrice);
			//end:  ship exist

			
			i++;
		}//end of while (i <= m_RecordsNumber)

	}//end of else //loading code
}

int CShipTypeWizardDlg::GetIntFromArchive(CArchive& ar)
{
	char *stopstring;
	CString strData;
	int intData;

	ar >> strData;
	intData = (int)strtod(strData,&stopstring);

	return intData;
}

void CShipTypeWizardDlg::PutIntToArchive(int intData, CArchive& ar1)
{
	char strData[5];
	CString strText;

	_itoa(intData, strData, 10);
	strText = strData;
	strText.Format("%s", strText);   
	ar1 << strText;
}

BEGIN_EVENTSINK_MAP(CShipTypeWizardDlg, CDialog)
    //{{AFX_EVENTSINK_MAP(CShipTypeWizardDlg)
	ON_EVENT(CShipTypeWizardDlg, IDC_SHIPTYPEWIZ_LIST, -600 /* Click */, OnClickShiptypewizList, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

void CShipTypeWizardDlg::OnClickShiptypewizList() 
{
	CString str;
	char *stopstring;
	int nState;

	int row = m_ShipGenInfoGrid.GetRow();
	
	str = m_ShipGenInfoGrid.GetTextMatrix(row,1);
	m_ctrlShipTypeName.SetWindowText(str);
	//m_ctrlShipTypeName.EnableWindow(FALSE);
	
	str = m_ShipGenInfoGrid.GetTextMatrix(row,2);
	m_ctrlRegisterClass.SetWindowText(str);
	//m_ctrlRegisterClass.EnableWindow(FALSE);

	str = m_ShipGenInfoGrid.GetTextMatrix(row,3);
	m_ctrlPrototypeName.SetWindowText(str);

	str = m_ShipGenInfoGrid.GetTextMatrix(row,4);
	m_ctrlLoad.SetWindowText(str);

	str = m_ShipGenInfoGrid.GetTextMatrix(row,5);
	m_ctrlL.SetWindowText(str);

	str = m_ShipGenInfoGrid.GetTextMatrix(row,6);
	m_ctrlB.SetWindowText(str);

	str = m_ShipGenInfoGrid.GetTextMatrix(row,7);
	m_ctrlT.SetWindowText(str);

	str = m_ShipGenInfoGrid.GetTextMatrix(row,18);
	m_ctrlSpeed.SetWindowText(str);


	str = m_ShipGenInfoGrid.GetTextMatrix(row,8);
	nState = (int)strtod(str,&stopstring);
	m_ctrlCargoCompatiblGeneral.SetCheck(nState);
	m_ctrlCargoCompatiblGeneral.EnableWindow(FALSE);
	//m_ctrlCargoCompatiblGeneral.SetState(1);

	str = m_ShipGenInfoGrid.GetTextMatrix(row,9);
	nState = (int)strtod(str,&stopstring);
	m_ctrlCargoCompatiblOil.SetCheck(nState);
	m_ctrlCargoCompatiblOil.EnableWindow(FALSE);
	//m_ctrlCargoCompatiblOil.SetState(1);

	str = m_ShipGenInfoGrid.GetTextMatrix(row,10);
	nState = (int)strtod(str,&stopstring);
	m_ctrlCargoCompatiblCoal.SetCheck(nState);
	m_ctrlCargoCompatiblCoal.EnableWindow(FALSE);
	//m_ctrlCargoCompatiblCoal.SetState(1);
	

	
	str = m_ShipGenInfoGrid.GetTextMatrix(row,11);
	nState = (int)strtod(str,&stopstring);
	m_ctrlCargoCompatiblVegFruits.SetCheck(nState);
	m_ctrlCargoCompatiblVegFruits.EnableWindow(FALSE);
	//m_ctrlCargoCompatiblVegFruits.SetState(1);
	
	//SetModified(FALSE);



}

void CShipTypeWizardDlg::FillSelectedShipInfo()
{
	char *stopstring;
	CString strNum;
	int row = m_ShipGenInfoGrid.GetRow();

	m_SelectedShipMainInfo = new CShipMainInfo;

	m_SelectedShipMainInfo->m_strlShipTypeName = m_ShipGenInfoGrid.GetTextMatrix(row,1);
	m_SelectedShipMainInfo->m_strShipRegisterClass = m_ShipGenInfoGrid.GetTextMatrix(row,2);
	m_SelectedShipMainInfo->m_strPrototypeName = m_ShipGenInfoGrid.GetTextMatrix(row,3);
	
	strNum = m_ShipGenInfoGrid.GetTextMatrix(row,4);
	m_SelectedShipMainInfo->m_nDW = (int)strtod(strNum,&stopstring);

	strNum = m_ShipGenInfoGrid.GetTextMatrix(row,5);
	m_SelectedShipMainInfo->m_nL = (int)strtod(strNum,&stopstring);

	strNum = m_ShipGenInfoGrid.GetTextMatrix(row,6);
	m_SelectedShipMainInfo->m_nB = (int)strtod(strNum,&stopstring);

	strNum = m_ShipGenInfoGrid.GetTextMatrix(row,7);
	m_SelectedShipMainInfo->m_nT = (int)strtod(strNum,&stopstring);

	strNum = m_ShipGenInfoGrid.GetTextMatrix(row,8);
	m_SelectedShipMainInfo->m_nCargoCompatiblGeneral = (int)strtod(strNum,&stopstring);

	strNum = m_ShipGenInfoGrid.GetTextMatrix(row,9);
	m_SelectedShipMainInfo->m_nCargoCompatiblOil = (int)strtod(strNum,&stopstring);

	strNum = m_ShipGenInfoGrid.GetTextMatrix(row,10);
	m_SelectedShipMainInfo->m_nCargoCompatiblCoal = (int)strtod(strNum,&stopstring);

	strNum = m_ShipGenInfoGrid.GetTextMatrix(row,11);
	m_SelectedShipMainInfo->m_nCargoCompatibleVegFruits = (int)strtod(strNum,&stopstring);

	strNum = m_ShipGenInfoGrid.GetTextMatrix(row,12);
	m_SelectedShipMainInfo->m_nLoadMin = (int)strtod(strNum,&stopstring);

	strNum = m_ShipGenInfoGrid.GetTextMatrix(row,13);
	m_SelectedShipMainInfo->m_nLoadMax = (int)strtod(strNum,&stopstring);

	strNum = m_ShipGenInfoGrid.GetTextMatrix(row,14);
	m_SelectedShipMainInfo->m_nLoadDelta = (int)strtod(strNum,&stopstring);

	strNum = m_ShipGenInfoGrid.GetTextMatrix(row,15);
	m_SelectedShipMainInfo->m_nTariffGenVF = (int)strtod(strNum,&stopstring);

	strNum = m_ShipGenInfoGrid.GetTextMatrix(row,16);
	m_SelectedShipMainInfo->m_nTariffOil = (int)strtod(strNum,&stopstring);

	strNum = m_ShipGenInfoGrid.GetTextMatrix(row,17);
	m_SelectedShipMainInfo->m_nTariffCoal = (int)strtod(strNum,&stopstring);

	strNum = m_ShipGenInfoGrid.GetTextMatrix(row,18);
	m_SelectedShipMainInfo->m_nSpeed = (int)strtod(strNum,&stopstring);

	strNum = m_ShipGenInfoGrid.GetTextMatrix(row,19);
	m_SelectedShipMainInfo->m_nRange = (int)strtod(strNum,&stopstring);

	m_SelectedShipMainInfo->m_strh3 = m_ShipGenInfoGrid.GetTextMatrix(row,20);
	strNum = m_ShipGenInfoGrid.GetTextMatrix(row,20);
	m_SelectedShipMainInfo->m_nh3 = (float)atof(strNum);


	//begin: ship exist
	strNum = m_ShipGenInfoGrid.GetTextMatrix(row,21);
	m_SelectedShipMainInfo->m_nShipExistSetCheck = (int)strtod(strNum,&stopstring);

	strNum = m_ShipGenInfoGrid.GetTextMatrix(row,22);
	m_SelectedShipMainInfo->m_nShipExistForceCheck = (int)strtod(strNum,&stopstring);

	strNum = m_ShipGenInfoGrid.GetTextMatrix(row,23);
	m_SelectedShipMainInfo->m_nShipExistCheck = (int)strtod(strNum,&stopstring);
	
	strNum = m_ShipGenInfoGrid.GetTextMatrix(row,24);
	m_SelectedShipMainInfo->m_nShipExistBuildCheck = (int)strtod(strNum,&stopstring);
	
	strNum = m_ShipGenInfoGrid.GetTextMatrix(row,25);
	m_SelectedShipMainInfo->m_nShipExistDesignCheck = (int)strtod(strNum,&stopstring);

	strNum = m_ShipGenInfoGrid.GetTextMatrix(row,26);
	m_SelectedShipMainInfo->m_nShipExistNum = (int)strtod(strNum,&stopstring);
	strNum = m_ShipGenInfoGrid.GetTextMatrix(row,27);
	m_SelectedShipMainInfo->m_nShipExistAdvantPrice = (int)strtod(strNum,&stopstring);
	strNum = m_ShipGenInfoGrid.GetTextMatrix(row,28);
	m_SelectedShipMainInfo->m_nShipExistPrice = (int)strtod(strNum,&stopstring);

	strNum = m_ShipGenInfoGrid.GetTextMatrix(row,29);
	m_SelectedShipMainInfo->m_nShipExistBuildNum = (int)strtod(strNum,&stopstring);
	strNum = m_ShipGenInfoGrid.GetTextMatrix(row,30);
	m_SelectedShipMainInfo->m_nShipExistBuildAdvantPrice = (int)strtod(strNum,&stopstring);
	strNum = m_ShipGenInfoGrid.GetTextMatrix(row,31);
	m_SelectedShipMainInfo->m_nShipExistBuildPrice = (int)strtod(strNum,&stopstring);

	strNum = m_ShipGenInfoGrid.GetTextMatrix(row,32);
	m_SelectedShipMainInfo->m_nShipExistDesignAdvantPrice = (int)strtod(strNum,&stopstring);
	strNum = m_ShipGenInfoGrid.GetTextMatrix(row,33);
	m_SelectedShipMainInfo->m_nShipExistDesignPrice = (int)strtod(strNum,&stopstring);
	//end: ship exist

}

int CShipTypeWizardDlg::GetSelectedShipID()
{
	m_SelectedShipID = 	m_ShipGenInfoGrid.GetRow();

	return m_SelectedShipID;
}

void CShipTypeWizardDlg::OnShiptypewizDelete() 
{
	CString strNum;
//	char *stopstring;
	CString strText;
	CFile f;
	CFileException e;

	//SetUpdatedShipInfo();
	m_SelectedShipID = 	m_ShipGenInfoGrid.GetRow();
	
	CShipMainInfo ShipInfo[MAX_SHIP_RECORDS_IN_DB];
	if (m_RecordsNumber >= MAX_SHIP_RECORDS_IN_DB)
		AfxMessageBox("The number of vessels in the Database has exceeded the program settings.\nValue MAX_SHIP_RECORDS_IN_DB should be increased in code.");
	
	f.Open( m_pShipsDBFile, CFile::modeRead, &e);
	CArchive ar(&f, CArchive::load);

	int RecNum = GetIntFromArchive(ar);

	for (int i = 1; i <= RecNum; i++){
	
		ar >> ShipInfo[i].m_strlShipTypeName;
		ar >> ShipInfo[i].m_strShipRegisterClass;
		ar >> ShipInfo[i].m_strPrototypeName;

		ShipInfo[i].m_nDW = GetIntFromArchive(ar);
		ShipInfo[i].m_nL = GetIntFromArchive(ar);
		ShipInfo[i].m_nB = GetIntFromArchive(ar);
		ShipInfo[i].m_nT = GetIntFromArchive(ar);
		ShipInfo[i].m_nSpeed = GetIntFromArchive(ar);
		ShipInfo[i].m_nRange = GetIntFromArchive(ar);
		ar >> ShipInfo[i].m_strh3;
		ShipInfo[i].m_nCargoCompatiblGeneral = GetIntFromArchive(ar);
		ShipInfo[i].m_nCargoCompatiblOil = GetIntFromArchive(ar);
		ShipInfo[i].m_nCargoCompatiblCoal = GetIntFromArchive(ar);
		ShipInfo[i].m_nCargoCompatibleVegFruits = GetIntFromArchive(ar);
		ShipInfo[i].m_nLoadMin = GetIntFromArchive(ar);
		ShipInfo[i].m_nLoadMax = GetIntFromArchive(ar);
		ShipInfo[i].m_nLoadDelta = GetIntFromArchive(ar);
		ShipInfo[i].m_nTariffGenVF = GetIntFromArchive(ar);
		ShipInfo[i].m_nTariffOil = GetIntFromArchive(ar);
		ShipInfo[i].m_nTariffCoal = GetIntFromArchive(ar);

//begin: ship exist
		ShipInfo[i].m_nShipExistSetCheck = GetIntFromArchive(ar);
		ShipInfo[i].m_nShipExistForceCheck = GetIntFromArchive(ar);
		ShipInfo[i].m_nShipExistCheck = GetIntFromArchive(ar);
		ShipInfo[i].m_nShipExistBuildCheck = GetIntFromArchive(ar);
		ShipInfo[i].m_nShipExistDesignCheck = GetIntFromArchive(ar);
		ShipInfo[i].m_nShipExistNum = GetIntFromArchive(ar);
		ShipInfo[i].m_nShipExistAdvantPrice = GetIntFromArchive(ar); 
		ShipInfo[i].m_nShipExistPrice = GetIntFromArchive(ar);
		ShipInfo[i].m_nShipExistBuildNum = GetIntFromArchive(ar);
		ShipInfo[i].m_nShipExistBuildAdvantPrice = GetIntFromArchive(ar);
		ShipInfo[i].m_nShipExistBuildPrice = GetIntFromArchive(ar);
		ShipInfo[i].m_nShipExistDesignAdvantPrice = GetIntFromArchive(ar);
		ShipInfo[i].m_nShipExistDesignPrice = GetIntFromArchive(ar);
//end: ship exist
	}

	ar.Close();
	f.Close();
	
	/////////////// Rewriting (Updating) Archive file ///////////////
	int numTmp = 0;

	CFile f1( m_pShipsDBFile, CFile::modeCreate | CFile::modeWrite);
	CArchive ar1(&f1, CArchive::store);

	int newRecNum = m_RecordsNumber - 1;
	PutIntToArchive(newRecNum, ar1);

	for (int j = 1; j <= m_RecordsNumber; j++){
		if (j == m_SelectedShipID)
			continue;

		ar1 << ShipInfo[j].m_strlShipTypeName;
		ar1 << ShipInfo[j].m_strShipRegisterClass;
		ar1 << ShipInfo[j].m_strPrototypeName;

		PutIntToArchive(ShipInfo[j].m_nDW, ar1);
		PutIntToArchive(ShipInfo[j].m_nL, ar1);
		PutIntToArchive(ShipInfo[j].m_nB, ar1);
		PutIntToArchive(ShipInfo[j].m_nT, ar1);
		PutIntToArchive(ShipInfo[j].m_nSpeed, ar1);
		PutIntToArchive(ShipInfo[j].m_nRange, ar1);
		ar1 << ShipInfo[j].m_strh3;
		PutIntToArchive(ShipInfo[j].m_nCargoCompatiblGeneral, ar1);
		PutIntToArchive(ShipInfo[j].m_nCargoCompatiblOil, ar1);
		PutIntToArchive(ShipInfo[j].m_nCargoCompatiblCoal, ar1);
		PutIntToArchive(ShipInfo[j].m_nCargoCompatibleVegFruits, ar1);
		PutIntToArchive(ShipInfo[j].m_nLoadMin, ar1);
		PutIntToArchive(ShipInfo[j].m_nLoadMax, ar1);
		PutIntToArchive(ShipInfo[j].m_nLoadDelta, ar1);
		PutIntToArchive(ShipInfo[j].m_nTariffGenVF, ar1);
		PutIntToArchive(ShipInfo[j].m_nTariffOil, ar1);
		PutIntToArchive(ShipInfo[j].m_nTariffCoal, ar1);
//begin: ship exist
		PutIntToArchive(ShipInfo[j].m_nShipExistSetCheck, ar1);
		PutIntToArchive(ShipInfo[j].m_nShipExistForceCheck, ar1);
		PutIntToArchive(ShipInfo[j].m_nShipExistCheck, ar1);
		PutIntToArchive(ShipInfo[j].m_nShipExistBuildCheck, ar1);
		PutIntToArchive(ShipInfo[j].m_nShipExistDesignCheck, ar1);
		PutIntToArchive(ShipInfo[j].m_nShipExistNum, ar1);
		PutIntToArchive(ShipInfo[j].m_nShipExistAdvantPrice, ar1);
		PutIntToArchive(ShipInfo[j].m_nShipExistPrice, ar1);
		PutIntToArchive(ShipInfo[j].m_nShipExistBuildNum, ar1);
		PutIntToArchive(ShipInfo[j].m_nShipExistBuildAdvantPrice, ar1);
		PutIntToArchive(ShipInfo[j].m_nShipExistBuildPrice, ar1);
		PutIntToArchive(ShipInfo[j].m_nShipExistDesignAdvantPrice, ar1);
		PutIntToArchive(ShipInfo[j].m_nShipExistDesignPrice, ar1);
//end: ship exist


	}

	ar1.Close();
	f1.Close();

	m_RecordsNumber--; 

	CleanGenInfoGrid();
	FillGenInfoGrid();
	OnClickShiptypewizList();
	
}

void CShipTypeWizardDlg::OnShiptypeWizRefresh() 
{
	CleanGenInfoGrid();
	FillGenInfoGrid();
	OnClickShiptypewizList();
}

void CShipTypeWizardDlg::CleanGenInfoGrid()
{
	int i,j,max;
	
	max = m_RecordsNumber+1;

	for (i = 1; i <= max; i++)
		for (j = 0; j <= 33; j++)
			m_ShipGenInfoGrid.SetTextMatrix(i,j,"");

}


//DEL void CShipTypeWizardDlg::OnShiptypeWizNewACV() 
//DEL {
//DEL 	// TODO: Add your control notification handler code here
//DEL 	CShipTypePropertySheet thePSheet(_T("New Ship Type Creation"));
//DEL 	thePSheet.m_CShipTypeMainPPage.m_ISModifiable = TRUE;
//DEL 	thePSheet.m_CShipTypeOptimizParamPPage.m_ISModifiable = TRUE;
//DEL 	thePSheet.m_OpenStat = 3;
//DEL 	thePSheet.m_CShipTypeMainPPage.m_OpenStat = 3;
//DEL 	thePSheet.m_CShipTypeOptimizParamPPage.m_OpenStat = 3;
//DEL 	thePSheet.m_RecordsNumber = m_RecordsNumber;
//DEL 	thePSheet.m_pShipsDBFile = m_pShipsDBFile;
//DEL 	thePSheet.DoModal();	
//DEL }

void CShipTypeWizardDlg::OnShipPrototypeWizButton() 
{
	// TODO: Add your control notification handler code here
	if(!m_pShipProtoWizDlg)
	{
		m_pShipProtoWizDlg  = new CPrototypeWizardShipDlg();
	}
	else
	{
		m_pShipProtoWizDlg->ShowWindow(SW_RESTORE);
	}
	m_pShipProtoWizDlg->ShowWindow(SW_SHOW);

}

void CShipTypeWizardDlg::OnACVPrototypeWizButton() 
{
	// TODO: Add your control notification handler code here
	if(!m_pACVProtoWizDlg)
	{
		m_pACVProtoWizDlg  = new CPrototypeWizardACVDlg();
	}
	else
	{
		m_pACVProtoWizDlg->ShowWindow(SW_RESTORE);
	}
	m_pACVProtoWizDlg->ShowWindow(SW_SHOW);	
}
