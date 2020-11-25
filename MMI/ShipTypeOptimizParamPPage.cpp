// ShipTypeOptimizParamPPage.cpp : implementation file
//

#include "stdafx.h"
//#include "	\ add additional includes here"
#include "ShipTypeOptimizParamPPage.h"
#include "ShipTypePropertySheet.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CShipTypeOptimizParamPPage property page

IMPLEMENT_DYNCREATE(CShipTypeOptimizParamPPage, CPropertyPage)

CShipTypeOptimizParamPPage::CShipTypeOptimizParamPPage() : CPropertyPage(CShipTypeOptimizParamPPage::IDD)
{
	//{{AFX_DATA_INIT(CShipTypeOptimizParamPPage)
	m_bUseShipExistSet = FALSE;
	m_bShipExistForce = FALSE;
	m_bShipExist = FALSE;
	m_bShipExistBuild = FALSE;
	m_bShipExistDesign = FALSE;
	//}}AFX_DATA_INIT
	
	m_bInitialized = FALSE;
}

CShipTypeOptimizParamPPage::~CShipTypeOptimizParamPPage()
{
}

void CShipTypeOptimizParamPPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CShipTypeOptimizParamPPage)
	DDX_Control(pDX, IDC_SHIPTYPE_EXIST_FORCE_CHECK, m_ctrlShipExistForceCheckBox);
	DDX_Control(pDX, IDC_SHIPEXDESIGN_PRICE_EDIT, m_ctrlShipExistDesignPrice);
	DDX_Control(pDX, IDC_SHIPEXDESIGN_ADVPRICE_EDIT, m_ctrlShipExistDesignAdvantPrice);
	DDX_Control(pDX, IDC_SHIPEXBUILD_PRICE_EDIT, m_ctrlShipExistBuildPrice);
	DDX_Control(pDX, IDC_SHIPEXBUILD_ADVPRICE_EDIT, m_ctrlShipExistBuildAdvantPrice);
	DDX_Control(pDX, IDC_SHIPEXIST_PRICE_EDIT, m_ctrlShipExistPrice);
	DDX_Control(pDX, IDC_SHIPEXIST_ADVPRICE_EDIT, m_ctrlShipExistAdvantPrice);
	DDX_Control(pDX, IDC_SHIPEXBUILD_NUM_COMBO, m_ctrlShipExistBuildNumComboBox);
	DDX_Control(pDX, IDC_SHIPEXIST_NUM_COMBO, m_ctrlShipExistNumComboBox);
	DDX_Control(pDX, IDC_SHIPTYPE_EXISTDESIGN_CHECK, m_ctrlShipExistDesignCheckBox);
	DDX_Control(pDX, IDC_SHIPTYPE_EXISTBUILD_CHECK, m_ctrlShipExistBuildCheckBox);
	DDX_Control(pDX, IDC_SHIPTYPE_EXIST_CHECK, m_ctrlShipExistCheckBox);
	DDX_Control(pDX, IDC_SHIPTYPE_EXIST_SET_CHECK, m_ctrlShipExistSetCheckBox);
	DDX_Control(pDX, IDC_SHIPTYPE_PRICE_MIN, m_ctrlShipTariffGenVF);
	DDX_Control(pDX, IDC_SHIPTYPE_PRICE_MAX, m_ctrlShipTariffOil);
	DDX_Control(pDX, IDC_SHIPTYPE_PRICE_DELTA, m_ctrlShipTariffCoal);
	DDX_Control(pDX, IDC_SHIPTYPE_LOAD_MIN, m_ctrlShipLoadMin);
	DDX_Control(pDX, IDC_SHIPTYPE_LOAD_MAX, m_ctrlShipLoadMax);
	DDX_Control(pDX, IDC_SHIPTYPE_LOAD_DELTA, m_ctrlShipLoadDelta);
	DDX_Check(pDX, IDC_SHIPTYPE_EXIST_SET_CHECK, m_bUseShipExistSet);
	DDX_Check(pDX, IDC_SHIPTYPE_EXIST_FORCE_CHECK, m_bShipExistForce);
	DDX_Check(pDX, IDC_SHIPTYPE_EXIST_CHECK, m_bShipExist);
	DDX_Check(pDX, IDC_SHIPTYPE_EXISTBUILD_CHECK, m_bShipExistBuild);
	DDX_Check(pDX, IDC_SHIPTYPE_EXISTDESIGN_CHECK, m_bShipExistDesign);
	//}}AFX_DATA_MAP
	
	m_ctrlShipExistPrice.SetLimitText(7);
	m_ctrlShipExistAdvantPrice.SetLimitText(7);
	m_ctrlShipExistBuildPrice.SetLimitText(7);
	m_ctrlShipExistBuildAdvantPrice.SetLimitText(7);
	m_ctrlShipExistDesignPrice.SetLimitText(7);
	m_ctrlShipExistDesignAdvantPrice.SetLimitText(7);

/*	if((!m_bInitialized) && (m_OpenStat != 3))
	{
		FillShipDetails();
		m_bInitialized = TRUE;		
		EnableShipExistingSettings(2, FALSE);
	}
*/
	if(!m_bInitialized){
		if (m_OpenStat != 3)
			FillShipDetails();
	
		if (m_OpenStat == 2)
			EnableShipExistingSettings(2, TRUE);
		else
			EnableShipExistingSettings(2, FALSE);
		m_bInitialized = TRUE;		
	}
	
	if (m_OpenStat == 1)
		DisableControls();

	//GetUpdatePPageDataStruct();
}


BEGIN_MESSAGE_MAP(CShipTypeOptimizParamPPage, CPropertyPage)
	//{{AFX_MSG_MAP(CShipTypeOptimizParamPPage)
	ON_BN_CLICKED(IDC_SHIPTYPE_EXIST_SET_CHECK, OnUseShipExistSettings)
	ON_BN_CLICKED(IDC_SHIPTYPE_EXIST_CHECK, OnShipExist)
	ON_BN_CLICKED(IDC_SHIPTYPE_EXISTBUILD_CHECK, OnShipExistBuild)
	ON_BN_CLICKED(IDC_SHIPTYPE_EXISTDESIGN_CHECK, OnShipExistDesign)
	ON_BN_CLICKED(IDC_SHIPTYPE_EXIST_FORCE_CHECK, OnShipExistForce)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CShipTypeOptimizParamPPage message handlers

void CShipTypeOptimizParamPPage::Serialize(CArchive& ar) 
{
	CString str;
	int  CheckStateShipExistSet, CheckStateShipExistForce,  CheckStateShipExist, //, CheckState
					CheckStateShipExistBuild, CheckStateShipExistDesign;

	if (ar.IsStoring())
	{	// storing code
		m_ctrlShipLoadMin.GetWindowText(str);
		ar << str;
		m_ctrlShipLoadMax.GetWindowText(str);
		ar << str;
		m_ctrlShipLoadDelta.GetWindowText(str);
		ar << str;
		m_ctrlShipTariffGenVF.GetWindowText(str);
		ar << str;
		m_ctrlShipTariffOil.GetWindowText(str);
		ar << str;
		m_ctrlShipTariffCoal.GetWindowText(str);
		ar << str;

		// storing existing ship data {begin
		//1. Check if the ship exists.
		CheckStateShipExistSet = m_ctrlShipExistSetCheckBox.GetCheck();
		((CShipTypePropertySheet*)m_pParentPtr)->PutIntToArchive(CheckStateShipExistSet,ar);
		CheckStateShipExistForce = m_ctrlShipExistForceCheckBox.GetCheck();
		((CShipTypePropertySheet*)m_pParentPtr)->PutIntToArchive(CheckStateShipExistForce,ar);
		CheckStateShipExist = m_ctrlShipExistCheckBox.GetCheck(); 
		((CShipTypePropertySheet*)m_pParentPtr)->PutIntToArchive(CheckStateShipExist,ar);
		CheckStateShipExistBuild = m_ctrlShipExistBuildCheckBox.GetCheck();
		((CShipTypePropertySheet*)m_pParentPtr)->PutIntToArchive(CheckStateShipExistBuild,ar);
		CheckStateShipExistDesign = m_ctrlShipExistDesignCheckBox.GetCheck();
		((CShipTypePropertySheet*)m_pParentPtr)->PutIntToArchive(CheckStateShipExistDesign,ar);

		if (CheckStateShipExistSet == 0){ //ship doesn't exist
			str = "";
			//((CShipTypePropertySheet*)m_pParentPtr)->PutIntToArchive(0,ar);
			ar << str;
			ar << str;
			ar << str;
			//((CShipTypePropertySheet*)m_pParentPtr)->PutIntToArchive(0,ar);
			ar << str;
			ar << str;
			ar << str;
			//((CShipTypePropertySheet*)m_pParentPtr)->PutIntToArchive(0,ar);
			ar << str;
			ar << str;
		}
		else { // Ship - exists (in operation, build or design)
			//2. Check if the ship is in operation:
			if(CheckStateShipExist == 0){  //ship - not in operation
				str = "";
				ar << str;
				ar << str;
				ar << str;				
			}
			else{ //ship is in operation
				GetDlgItem(IDC_SHIPEXIST_NUM_COMBO)->GetWindowText(str);
				ar << str;
				GetDlgItem(IDC_SHIPEXIST_ADVPRICE_EDIT)->GetWindowText(str);
				ar << str;
				GetDlgItem(IDC_SHIPEXIST_PRICE_EDIT)->GetWindowText(str);
				ar << str;
			} //end of (2): ship is in operation.
			//3. Check if the ship is in Build:
			if(CheckStateShipExistBuild == 0){ //ship is not in Build
				str = "";
				ar << str;
				ar << str;
				ar << str;				
			}
			else{ //ship is in Build
				GetDlgItem(IDC_SHIPEXBUILD_NUM_COMBO)->GetWindowText(str);
				ar << str;
				GetDlgItem(IDC_SHIPEXBUILD_ADVPRICE_EDIT)->GetWindowText(str);
				ar << str;
				GetDlgItem(IDC_SHIPEXBUILD_PRICE_EDIT)->GetWindowText(str);
				ar << str;
			}//end of (3): ship is in Build.
			//4. Check if the ship is in Design:
			if(CheckStateShipExistDesign == 0){
				str = "";
				ar << str;
				ar << str;
			}
			else{ //ship is in Design
				GetDlgItem(IDC_SHIPEXDESIGN_ADVPRICE_EDIT)->GetWindowText(str);
				ar << str;
				GetDlgItem(IDC_SHIPEXDESIGN_PRICE_EDIT)->GetWindowText(str);
				ar << str;
			}//end of (4): ship is in Design
		}

	}
	else
	{	// loading code
	}
}

void CShipTypeOptimizParamPPage::SetParentPtr(CPropertySheet *p)
{
		m_pParentPtr = (CShipTypePropertySheet*)p;
}

void CShipTypeOptimizParamPPage::FillShipDetails()
{
	char strNum[5];
	int num;

	
	num = ((CShipTypePropertySheet*)m_pParentPtr)->m_SelectedShipMainInfo->m_nLoadMin;
	_itoa(num, strNum, 10);
	m_ctrlShipLoadMin.SetWindowText(strNum);

	num = ((CShipTypePropertySheet*)m_pParentPtr)->m_SelectedShipMainInfo->m_nLoadMax;
	_itoa(num, strNum, 10);
	m_ctrlShipLoadMax.SetWindowText(strNum);

	num = ((CShipTypePropertySheet*)m_pParentPtr)->m_SelectedShipMainInfo->m_nLoadDelta;
	_itoa(num, strNum, 10);
	m_ctrlShipLoadDelta.SetWindowText(strNum);

	num = ((CShipTypePropertySheet*)m_pParentPtr)->m_SelectedShipMainInfo->m_nTariffGenVF;
	_itoa(num, strNum, 10);
	m_ctrlShipTariffGenVF.SetWindowText(strNum);

	num = ((CShipTypePropertySheet*)m_pParentPtr)->m_SelectedShipMainInfo->m_nTariffOil;
	_itoa(num, strNum, 10);
	m_ctrlShipTariffOil.SetWindowText(strNum);

	num = ((CShipTypePropertySheet*)m_pParentPtr)->m_SelectedShipMainInfo->m_nTariffCoal;
	_itoa(num, strNum, 10);
	m_ctrlShipTariffCoal.SetWindowText(strNum);

	//begin:  ship exist
	num = ((CShipTypePropertySheet*)m_pParentPtr)->m_SelectedShipMainInfo->m_nShipExistSetCheck;
	m_ctrlShipExistSetCheckBox.SetCheck(num);

	num = ((CShipTypePropertySheet*)m_pParentPtr)->m_SelectedShipMainInfo->m_nShipExistForceCheck;
	m_ctrlShipExistForceCheckBox.SetCheck(num);

	num = ((CShipTypePropertySheet*)m_pParentPtr)->m_SelectedShipMainInfo->m_nShipExistCheck;
	m_ctrlShipExistCheckBox.SetCheck(num);

	num = ((CShipTypePropertySheet*)m_pParentPtr)->m_SelectedShipMainInfo->m_nShipExistBuildCheck;
	m_ctrlShipExistBuildCheckBox.SetCheck(num);

	num = ((CShipTypePropertySheet*)m_pParentPtr)->m_SelectedShipMainInfo->m_nShipExistDesignCheck;
	m_ctrlShipExistDesignCheckBox.SetCheck(num);	

	num = ((CShipTypePropertySheet*)m_pParentPtr)->m_SelectedShipMainInfo->m_nShipExistNum;
	_itoa(num, strNum, 10);
	//m_ctrlShipExistNumComboBox.SetWindowText(strNum);
	m_ctrlShipExistNumComboBox.SelectString(-1,strNum);

	num = ((CShipTypePropertySheet*)m_pParentPtr)->m_SelectedShipMainInfo->m_nShipExistAdvantPrice;
	_itoa(num, strNum, 10);
	m_ctrlShipExistAdvantPrice.SetWindowText(strNum);

	num = ((CShipTypePropertySheet*)m_pParentPtr)->m_SelectedShipMainInfo->m_nShipExistPrice;
	_itoa(num, strNum, 10);
	m_ctrlShipExistPrice.SetWindowText(strNum);

	num = ((CShipTypePropertySheet*)m_pParentPtr)->m_SelectedShipMainInfo->m_nShipExistBuildNum;
	_itoa(num, strNum, 10);
	//m_ctrlShipExistBuildNumComboBox.SetWindowText(strNum);
	m_ctrlShipExistBuildNumComboBox.SelectString(-1,strNum);

	num = ((CShipTypePropertySheet*)m_pParentPtr)->m_SelectedShipMainInfo->m_nShipExistBuildAdvantPrice;
	_itoa(num, strNum, 10);
	m_ctrlShipExistBuildAdvantPrice.SetWindowText(strNum);

	num = ((CShipTypePropertySheet*)m_pParentPtr)->m_SelectedShipMainInfo->m_nShipExistBuildPrice;
	_itoa(num, strNum, 10);
	m_ctrlShipExistBuildPrice.SetWindowText(strNum);

	num = ((CShipTypePropertySheet*)m_pParentPtr)->m_SelectedShipMainInfo->m_nShipExistDesignAdvantPrice;
	_itoa(num, strNum, 10);
	m_ctrlShipExistDesignAdvantPrice.SetWindowText(strNum);

	num = ((CShipTypePropertySheet*)m_pParentPtr)->m_SelectedShipMainInfo->m_nShipExistDesignPrice;
	_itoa(num, strNum, 10);
	m_ctrlShipExistDesignPrice.SetWindowText(strNum);
	//end: ship exist

}

void CShipTypeOptimizParamPPage::DisableControls()
{
	m_ctrlShipTariffGenVF.SetReadOnly(TRUE);
	m_ctrlShipTariffOil.SetReadOnly(TRUE);
	m_ctrlShipTariffCoal.SetReadOnly(TRUE);
	m_ctrlShipLoadMin.SetReadOnly(TRUE);
	m_ctrlShipLoadMax.SetReadOnly(TRUE);
	m_ctrlShipLoadDelta.SetReadOnly(TRUE);

	EnableShipExistingSettings(1, FALSE);
}

void CShipTypeOptimizParamPPage::GetUpdatePPageDataStruct()
{
	CString str;
	str = "";
	
	m_ctrlShipLoadMin.GetWindowText(str);
	m_ppage_data.strShipLoadMin = str;
	m_ctrlShipLoadMax.GetWindowText(str);
	m_ppage_data.strShipLoadMax = str;
	m_ctrlShipLoadDelta.GetWindowText(str);
	m_ppage_data.strShipLoadDelta = str;
	m_ctrlShipTariffGenVF.GetWindowText(str);
	m_ppage_data.strShipTariffGenVF = str;
	m_ctrlShipTariffOil.GetWindowText(str);
	m_ppage_data.strShipTariffOil = str;
	m_ctrlShipTariffCoal.GetWindowText(str);
	m_ppage_data.strShipTariffCoal = str;
}

void CShipTypeOptimizParamPPage::OnChangeShiptypeLoadMin() 
{
//	UpdateData(TRUE);
	SetModified(TRUE);
}

void CShipTypeOptimizParamPPage::OnUseShipExistSettings() 
{
	UpdateData(TRUE);
	EnableShipExistingSettings(3,m_bUseShipExistSet);	
	SetModified(TRUE);	
}

void CShipTypeOptimizParamPPage::OnShipExist() 
{
	UpdateData(TRUE);
	EnableShipExistingSettings(4,m_bShipExist);	
	SetModified(TRUE);		
}

void CShipTypeOptimizParamPPage::OnShipExistBuild() 
{
	UpdateData(TRUE);
	EnableShipExistingSettings(5,m_bShipExistBuild);	
	SetModified(TRUE);	
}

void CShipTypeOptimizParamPPage::OnShipExistDesign() 
{
	UpdateData(TRUE);
	EnableShipExistingSettings(6,m_bShipExistDesign);	
	SetModified(TRUE);	
}

void CShipTypeOptimizParamPPage::EnableShipExistingSettings(int Num, BOOL Flag)
{
	int CheckStateE, CheckStateB, CheckStateD;

	CheckStateE = 0;
	CheckStateB = 0;
	CheckStateD = 0;

	if (m_ctrlShipExistSetCheckBox.GetCheck()){
		CheckStateE = m_ctrlShipExistCheckBox.GetCheck();
		CheckStateB = m_ctrlShipExistBuildCheckBox.GetCheck();
		CheckStateD = m_ctrlShipExistDesignCheckBox.GetCheck();
	}


	if(Num ==1) // Read Only: All disable
	{
		GetDlgItem(IDC_SHIPEXDESIGN_PRICE_EDIT)->EnableWindow(Flag);
		GetDlgItem(IDC_SHIPEXDESIGN_ADVPRICE_EDIT)->EnableWindow(Flag);
		GetDlgItem(IDC_SHIPEXBUILD_PRICE_EDIT)->EnableWindow(Flag);
		GetDlgItem(IDC_SHIPEXBUILD_ADVPRICE_EDIT)->EnableWindow(Flag);
		GetDlgItem(IDC_SHIPEXIST_PRICE_EDIT)->EnableWindow(Flag);
		GetDlgItem(IDC_SHIPEXIST_ADVPRICE_EDIT)->EnableWindow(Flag);
		GetDlgItem(IDC_SHIPEXBUILD_NUM_COMBO)->EnableWindow(Flag);
		GetDlgItem(IDC_SHIPEXIST_NUM_COMBO)->EnableWindow(Flag);
		GetDlgItem(IDC_SHIPTYPE_EXISTDESIGN_CHECK)->EnableWindow(Flag);
		GetDlgItem(IDC_SHIPTYPE_EXISTBUILD_CHECK)->EnableWindow(Flag);
		GetDlgItem(IDC_SHIPTYPE_EXIST_CHECK)->EnableWindow(Flag);
		GetDlgItem(IDC_SHIPTYPE_EXIST_SET_CHECK)->EnableWindow(Flag);
		GetDlgItem(IDC_SHIPTYPE_EXIST_FORCE_CHECK)->EnableWindow(Flag);
	}
	else if(Num == 2)
	{
		GetDlgItem(IDC_SHIPEXDESIGN_PRICE_EDIT)->EnableWindow(FALSE);
		GetDlgItem(IDC_SHIPEXDESIGN_ADVPRICE_EDIT)->EnableWindow(FALSE);
		GetDlgItem(IDC_SHIPEXBUILD_PRICE_EDIT)->EnableWindow(FALSE);
		GetDlgItem(IDC_SHIPEXBUILD_ADVPRICE_EDIT)->EnableWindow(FALSE);
		GetDlgItem(IDC_SHIPEXIST_PRICE_EDIT)->EnableWindow(FALSE);
		GetDlgItem(IDC_SHIPEXIST_ADVPRICE_EDIT)->EnableWindow(FALSE);
		GetDlgItem(IDC_SHIPEXBUILD_NUM_COMBO)->EnableWindow(FALSE);
		GetDlgItem(IDC_SHIPEXIST_NUM_COMBO)->EnableWindow(FALSE);
		GetDlgItem(IDC_SHIPTYPE_EXISTDESIGN_CHECK)->EnableWindow(FALSE);
		GetDlgItem(IDC_SHIPTYPE_EXISTBUILD_CHECK)->EnableWindow(FALSE);
		GetDlgItem(IDC_SHIPTYPE_EXIST_CHECK)->EnableWindow(FALSE);
		GetDlgItem(IDC_SHIPTYPE_EXIST_SET_CHECK)->EnableWindow(TRUE);
		GetDlgItem(IDC_SHIPTYPE_EXIST_FORCE_CHECK)->EnableWindow(FALSE);
		
		if(Flag)
		{
			//GetDlgItem(IDC_SHIPTYPE_EXIST_FORCE_CHECK)->EnableWindow(TRUE);
			//EnableShipExistingSettings(4,m_bShipExist);
			//EnableShipExistingSettings(4,Flag);
			if (CheckStateE == 1){
				EnableShipExistingSettings(4, TRUE);
				//m_ctrlShipExistDesignCheckBox.EnableWindow(FALSE);
			}
			if (CheckStateB == 1){
				EnableShipExistingSettings(5, TRUE);
				//m_ctrlShipExistDesignCheckBox.EnableWindow(FALSE);
			}
			if (CheckStateD == 1){
				EnableShipExistingSettings(6, TRUE);
				//m_ctrlShipExistCheckBox.EnableWindow(FALSE);
				//m_ctrlShipExistBuildCheckBox.EnableWindow(FALSE);
			}
		}
	}
	else if(Num == 3)
	{
		GetDlgItem(IDC_SHIPTYPE_EXISTDESIGN_CHECK)->EnableWindow(Flag);
		GetDlgItem(IDC_SHIPTYPE_EXISTBUILD_CHECK)->EnableWindow(Flag);
		GetDlgItem(IDC_SHIPTYPE_EXIST_CHECK)->EnableWindow(Flag);
		GetDlgItem(IDC_SHIPTYPE_EXIST_FORCE_CHECK)->EnableWindow(Flag);
		GetDlgItem(IDC_SHIPTYPE_EXIST_SET_CHECK)->EnableWindow(TRUE);


		if(Flag){
			if (CheckStateE == 1){
				EnableShipExistingSettings(4, TRUE);
				m_ctrlShipExistDesignCheckBox.EnableWindow(FALSE);
			}
			if (CheckStateB == 1){
				EnableShipExistingSettings(5, TRUE);
				m_ctrlShipExistDesignCheckBox.EnableWindow(FALSE);
			}
			if (CheckStateD == 1){
				EnableShipExistingSettings(6, TRUE);
				m_ctrlShipExistCheckBox.EnableWindow(FALSE);
				m_ctrlShipExistBuildCheckBox.EnableWindow(FALSE);
			}

		}
	}
	else if(Num == 4)
	{
		m_ctrlShipExistCheckBox.EnableWindow(TRUE);
		m_ctrlShipExistBuildCheckBox.EnableWindow(TRUE);
		m_ctrlShipExistDesignCheckBox.SetCheck(0);
		m_ctrlShipExistDesignCheckBox.EnableWindow(FALSE);
		
		GetDlgItem(IDC_SHIPEXIST_NUM_COMBO)->EnableWindow(Flag);
		GetDlgItem(IDC_SHIPEXIST_PRICE_EDIT)->EnableWindow(Flag);
		GetDlgItem(IDC_SHIPEXIST_ADVPRICE_EDIT)->EnableWindow(Flag);

		if (CheckStateB == 1){
			GetDlgItem(IDC_SHIPEXBUILD_NUM_COMBO)->EnableWindow(TRUE);
			GetDlgItem(IDC_SHIPEXBUILD_PRICE_EDIT)->EnableWindow(TRUE);
			GetDlgItem(IDC_SHIPEXBUILD_ADVPRICE_EDIT)->EnableWindow(TRUE);				
		}
		
		if (!Flag){
			if((CheckStateE == 0) && (CheckStateB == 0)){
				m_ctrlShipExistDesignCheckBox.EnableWindow(TRUE);
				if (CheckStateD == 1){
					GetDlgItem(IDC_SHIPEXDESIGN_PRICE_EDIT)->EnableWindow(TRUE);
					GetDlgItem(IDC_SHIPEXDESIGN_ADVPRICE_EDIT)->EnableWindow(TRUE);
				}
			}
		}

	}
		else if(Num == 5)
	{
		m_ctrlShipExistCheckBox.EnableWindow(TRUE);
		m_ctrlShipExistBuildCheckBox.EnableWindow(TRUE);
		m_ctrlShipExistDesignCheckBox.SetCheck(0);
		m_ctrlShipExistDesignCheckBox.EnableWindow(FALSE);

		GetDlgItem(IDC_SHIPEXBUILD_NUM_COMBO)->EnableWindow(Flag);
		GetDlgItem(IDC_SHIPEXBUILD_PRICE_EDIT)->EnableWindow(Flag);
		GetDlgItem(IDC_SHIPEXBUILD_ADVPRICE_EDIT)->EnableWindow(Flag);


		if (CheckStateE == 1){
			GetDlgItem(IDC_SHIPEXIST_NUM_COMBO)->EnableWindow(TRUE);
			GetDlgItem(IDC_SHIPEXIST_PRICE_EDIT)->EnableWindow(TRUE);
			GetDlgItem(IDC_SHIPEXIST_ADVPRICE_EDIT)->EnableWindow(TRUE);
		}
		
		if (!Flag){
			if((CheckStateE == 0) && (CheckStateB == 0)){
				m_ctrlShipExistDesignCheckBox.EnableWindow(TRUE);
			}
		}


	}
	else if(Num == 6)
	{
		GetDlgItem(IDC_SHIPTYPE_EXISTDESIGN_CHECK)->EnableWindow(TRUE);
		GetDlgItem(IDC_SHIPEXDESIGN_PRICE_EDIT)->EnableWindow(Flag);
		GetDlgItem(IDC_SHIPEXDESIGN_ADVPRICE_EDIT)->EnableWindow(Flag);
		
		m_ctrlShipExistCheckBox.SetCheck(0);
		m_ctrlShipExistBuildCheckBox.SetCheck(0);
		m_ctrlShipExistCheckBox.EnableWindow(FALSE);
		m_ctrlShipExistBuildCheckBox.EnableWindow(FALSE);

		if (!Flag){
			m_ctrlShipExistCheckBox.EnableWindow(TRUE);
			m_ctrlShipExistBuildCheckBox.EnableWindow(TRUE);
		}
	}
}

void CShipTypeOptimizParamPPage::OnShipExistForce() 
{
	// TODO: Add your control notification handler code here
	
}
