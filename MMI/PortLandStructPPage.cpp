// PortLandStructPPage.cpp : implementation file
//

#include "stdafx.h"
#include "PortLandStructPPage.h"
#include "PortDetailsPSheet.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPortLandStructPPage property page

IMPLEMENT_DYNCREATE(CPortLandStructPPage, CPropertyPage)

CPortLandStructPPage::CPortLandStructPPage() : CPropertyPage(CPortLandStructPPage::IDD)
{
	//{{AFX_DATA_INIT(CPortLandStructPPage)
	m_bACVCourtAvailable = FALSE;
	m_bShipCourtAvailable = FALSE;
	m_bLandStructCalcActivate = FALSE;
	//}}AFX_DATA_INIT


	m_ISModifiable = FALSE;
	m_ISLoadDetails = FALSE;
	m_bInitialized = FALSE;
	m_SelectedPortLandStructInfo = NULL;
}

CPortLandStructPPage::~CPortLandStructPPage()
{
}

void CPortLandStructPPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPortLandStructPPage)
	DDX_Control(pDX, IDC_LANDSTRUCT_ACTIVATE_CHECK, m_ctrlLandStructCalcActivate);
	DDX_Control(pDX, IDC_LAND_POINT_SHIPCOURT_CHECK, m_ctrlShipCourtAvailable);
	DDX_Control(pDX, IDC_LAND_POINT_ACVCOURT_CHECK, m_ctrlACVCourtAvailable);
	DDX_Control(pDX, IDC_SHIPCOURTINVEST_EDIT, m_ctrlShipCourtInvestPP);
	DDX_Control(pDX, IDC_LANDBASE_MOORLENGTHPRICE_EDIT, m_ctrlMoorElemPrice);
	DDX_Control(pDX, IDC_LANDBASE_MOORLENGTH_EDIT, m_ctrlMoorLength);
	DDX_Control(pDX, IDC_LANDBASE_ACVSPACEPRICE_EDIT, m_ctrlACVSpaceElemPrice);
	DDX_Control(pDX, IDC_LANDBASE_ACVSPACE_EDIT, m_ctrlACVSpace);
	DDX_Control(pDX, IDC_ACVCOURTINVEST_EDIT, m_ctrlACVCourtInvestPP);
	DDX_Control(pDX, IDC_MSFLEXGRID_LAND_REPAIR, m_ctrlRepairDetailsGrid);
	DDX_Check(pDX, IDC_LAND_POINT_ACVCOURT_CHECK, m_bACVCourtAvailable);
	DDX_Check(pDX, IDC_LAND_POINT_SHIPCOURT_CHECK, m_bShipCourtAvailable);
	DDX_Check(pDX, IDC_LANDSTRUCT_ACTIVATE_CHECK, m_bLandStructCalcActivate);
	//}}AFX_DATA_MAP

	if(!m_bInitialized){
		if (m_OpenStat != 3)
			FillPortLandDetails();
	
		if (m_OpenStat == 2)
			EnableLandStructSettings(2, TRUE);
		else
			EnableLandStructSettings(2, FALSE);
		m_bInitialized = TRUE;		
	}
	
	if (m_OpenStat == 1)
		DisableControls();

}


BEGIN_MESSAGE_MAP(CPortLandStructPPage, CPropertyPage)
	//{{AFX_MSG_MAP(CPortLandStructPPage)
	ON_BN_CLICKED(IDC_LANDSTRUCT_ACTIVATE_CHECK, OnLandStructActivateCheck)
	ON_BN_CLICKED(IDC_LAND_POINT_ACVCOURT_CHECK, OnLandStructPPACVCourtCheck)
	ON_BN_CLICKED(IDC_LAND_POINT_SHIPCOURT_CHECK, OnLandStructPPShipCourtCheck)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPortLandStructPPage message handlers

void CPortLandStructPPage::Serialize(CArchive& ar) 
{
	CString str;
	int CheckStateLandStructCalcActivate, CheckStateShipCourtAvailable, CheckStateACVCourtAvailable;

	if (ar.IsStoring())
	{	// storing code
		CheckStateLandStructCalcActivate = m_ctrlLandStructCalcActivate.GetCheck();
		((CPortDetailsPSheet*)m_pParentPtr)->PutIntToArchive(CheckStateLandStructCalcActivate,ar);
		CheckStateACVCourtAvailable = m_ctrlACVCourtAvailable.GetCheck();
		((CPortDetailsPSheet*)m_pParentPtr)->PutIntToArchive(CheckStateACVCourtAvailable,ar);
		m_ctrlACVCourtInvestPP.GetWindowText(str);
		if (str == "")
			str = "0";
		ar << str;
		CheckStateShipCourtAvailable = m_ctrlShipCourtAvailable.GetCheck();
		((CPortDetailsPSheet*)m_pParentPtr)->PutIntToArchive(CheckStateShipCourtAvailable,ar);
		m_ctrlShipCourtInvestPP.GetWindowText(str);
		if (str == "")
			str = "0";
		ar << str;
		
		m_ctrlACVSpace.GetWindowText(str);
		if (str == "")
			str = "0";
		ar << str;
		m_ctrlACVSpaceElemPrice.GetWindowText(str);
		if (str == "")
			str = "0";
		ar << str;
		m_ctrlMoorLength.GetWindowText(str);
		if (str == "")
			str = "0";
		ar << str;
		m_ctrlMoorElemPrice.GetWindowText(str);
		if (str == "")
			str = "0";
		ar << str;
/*		ar << m_ctrlRepairDetailsGrid.GetTextMatrix(1,1);	// СВП. Кол-во СВП, которое может принять существующая ремонтная база
		ar << m_ctrlRepairDetailsGrid.GetTextMatrix(1,2);	// СВП. Грузоподъемность СВП, для которого приводятся данные (кол-во и коэффициенты)
		ar << m_ctrlRepairDetailsGrid.GetTextMatrix(1,3);	// СВП. Стоимость достройки ремонтной базы для одного СВП (указанной грузоподъемности)
		ar << m_ctrlRepairDetailsGrid.GetTextMatrix(1,4);	// СВП. Стоимость достройки не зависяшая от количества СВП
		ar << m_ctrlRepairDetailsGrid.GetTextMatrix(2,1);	// Судно. Кол-во судов, которое может принять существующая ремонтная база
		ar << m_ctrlRepairDetailsGrid.GetTextMatrix(2,2);	// Судно. Грузоподъемность судна, для которого приводятся данные (кол-во и коэффициенты)
		ar << m_ctrlRepairDetailsGrid.GetTextMatrix(2,3);	// Судно. Стоимость достройки ремонтной базы для одного судна (указанной грузоподъемности)
		ar << m_ctrlRepairDetailsGrid.GetTextMatrix(2,4);	// Судно. Стоимость достройки не зависяшая от количества судов
*/		int i, j;
		for (i = 1; i <=2; i++){ //i - row: СВП, Судно
			for (j = 1; j <=4; j++){	//j = column
				str = m_ctrlRepairDetailsGrid.GetTextMatrix(i,j);
				if (str == "")
					str = "0";
				ar << str;
			}
		}		

	}
	else
	{	// loading code
	}
}


void CPortLandStructPPage::SetParentPtr(CPropertySheet *p)
{
			m_pParentPtr = (CPortDetailsPSheet*)p;
}

BEGIN_EVENTSINK_MAP(CPortLandStructPPage, CPropertyPage)
    //{{AFX_EVENTSINK_MAP(CPortLandStructPPage)
	ON_EVENT(CPortLandStructPPage, IDC_MSFLEXGRID_LAND_REPAIR, -600 /* Click */, OnClickMsflexGridLandRepair, VTS_NONE)
	ON_EVENT(CPortLandStructPPage, IDC_MSFLEXGRID_LAND_REPAIR, -601 /* DblClick */, OnDblClickMsflexgridLandRepair, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

void CPortLandStructPPage::OnClickMsflexGridLandRepair() 
{
	// TODO: Add your control notification handler code here
	
}

void CPortLandStructPPage::FillGridsTitles()
{
	m_ctrlRepairDetailsGrid.SetTextMatrix(1,0,_T("ACV"));
	m_ctrlRepairDetailsGrid.SetTextMatrix(2,0,_T("Vessel"));

}

BOOL CPortLandStructPPage::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	FillGridsTitles();
	return TRUE;
}



void CPortLandStructPPage::FillPortLandDetails()
{
	char strNum[10];
	int num;

	num = m_SelectedPortLandStructInfo->m_nLandStructCalcActivateCheck;
	m_ctrlLandStructCalcActivate.SetCheck(num);

	num = m_SelectedPortLandStructInfo->m_nACVCourtAvailableCheck;
	m_ctrlACVCourtAvailable.SetCheck(num);
	num = m_SelectedPortLandStructInfo->m_nACVCourtInvestPP;
	_itoa(num, strNum, 10);
	m_ctrlACVCourtInvestPP.SetWindowText(strNum);

	num = m_SelectedPortLandStructInfo->m_nShipCourtAvailableCheck;
	m_ctrlShipCourtAvailable.SetCheck(num);
	num = m_SelectedPortLandStructInfo->m_nShipCourtInvestPP;
	_itoa(num, strNum, 10);
	m_ctrlShipCourtInvestPP.SetWindowText(strNum);

	num = m_SelectedPortLandStructInfo->m_nACVSpace;
	_itoa(num, strNum, 10);
	m_ctrlACVSpace.SetWindowText(strNum);
	num = m_SelectedPortLandStructInfo->m_nACVSpaceElemPrice;
	_itoa(num, strNum, 10);
	m_ctrlACVSpaceElemPrice.SetWindowText(strNum);

	num = m_SelectedPortLandStructInfo->m_nMoorLength;
	_itoa(num, strNum, 10);
	m_ctrlMoorLength.SetWindowText(strNum);
	num = m_SelectedPortLandStructInfo->m_nMoorElemPrice;
	_itoa(num, strNum, 10);
	m_ctrlMoorElemPrice.SetWindowText(strNum);

	num = m_SelectedPortLandStructInfo->m_structRepairDetailsACV.n;
	_itoa(num, strNum, 10);
	m_ctrlRepairDetailsGrid.SetTextMatrix(1,1,strNum);
	num = m_SelectedPortLandStructInfo->m_structRepairDetailsACV.Pgr;
	_itoa(num, strNum, 10);
	m_ctrlRepairDetailsGrid.SetTextMatrix(1,2,strNum);
	num = m_SelectedPortLandStructInfo->m_structRepairDetailsACV.a;
	_itoa(num, strNum, 10);
	m_ctrlRepairDetailsGrid.SetTextMatrix(1,3,strNum);
	num = m_SelectedPortLandStructInfo->m_structRepairDetailsACV.b;
	_itoa(num, strNum, 10);
	m_ctrlRepairDetailsGrid.SetTextMatrix(1,4,strNum);

	num = m_SelectedPortLandStructInfo->m_structRepairDetailsShip.n;
	_itoa(num, strNum, 10);
	m_ctrlRepairDetailsGrid.SetTextMatrix(2,1,strNum);
	num = m_SelectedPortLandStructInfo->m_structRepairDetailsShip.Pgr;
	_itoa(num, strNum, 10);
	m_ctrlRepairDetailsGrid.SetTextMatrix(2,2,strNum);
	num = m_SelectedPortLandStructInfo->m_structRepairDetailsShip.a;
	_itoa(num, strNum, 10);
	m_ctrlRepairDetailsGrid.SetTextMatrix(2,3,strNum);
	num = m_SelectedPortLandStructInfo->m_structRepairDetailsShip.b;
	_itoa(num, strNum, 10);
	m_ctrlRepairDetailsGrid.SetTextMatrix(2,4,strNum);

}



void CPortLandStructPPage::DisableControls()
{
	m_ctrlLandStructCalcActivate.EnableWindow(FALSE);;
	m_ctrlShipCourtAvailable.EnableWindow(FALSE);;
	m_ctrlACVCourtAvailable.EnableWindow(FALSE);;
	m_ctrlShipCourtInvestPP.SetReadOnly(TRUE);
	m_ctrlMoorElemPrice.SetReadOnly(TRUE);
	m_ctrlMoorLength.SetReadOnly(TRUE);
	m_ctrlACVSpaceElemPrice.SetReadOnly(TRUE);
	m_ctrlACVSpace.SetReadOnly(TRUE);
	m_ctrlACVCourtInvestPP.SetReadOnly(TRUE);
//	m_ctrlRepairDetailsGrid.SetReadOnly(TRUE);

	EnableLandStructSettings(1,FALSE);
	
}

void CPortLandStructPPage::OnDblClickMsflexgridLandRepair() 
{
	//there is CEditGrid::OnKeyPressGrid(short FAR* ascii) handler,
	//but we cancel it.Instead of catch "OnDblClick" event in CEditGrid,
	//we use it here.

	if (m_ISModifiable == TRUE){
		short i = 13;
		m_ctrlRepairDetailsGrid.OnKeyPressGrid(&i);// Simulate a return.
	}	
}


void CPortLandStructPPage::OnLandStructActivateCheck() 
{
	UpdateData(TRUE);
	EnableLandStructSettings(3,m_bLandStructCalcActivate);	
	SetModified(TRUE);		
}

void CPortLandStructPPage::OnLandStructPPACVCourtCheck() 
{
	UpdateData(TRUE);
	EnableLandStructSettings(4,m_bACVCourtAvailable);	
	SetModified(TRUE);	
}

void CPortLandStructPPage::OnLandStructPPShipCourtCheck() 
{
	UpdateData(TRUE);
	EnableLandStructSettings(5,m_bShipCourtAvailable);	
	SetModified(TRUE);
}




void CPortLandStructPPage::EnableLandStructSettings(int Num, BOOL Flag)
{
	int CheckStateACVCourt, CheckStateShipCourt;

	CheckStateACVCourt = 0;
	CheckStateShipCourt = 0;

	if (m_ctrlLandStructCalcActivate.GetCheck()){
		CheckStateACVCourt = m_ctrlACVCourtAvailable.GetCheck();
		CheckStateShipCourt = m_ctrlShipCourtAvailable.GetCheck();
	}

	if(Num ==1) // Read Only: All disable
	{
		GetDlgItem(IDC_LAND_POINT_ACVCOURT_CHECK)->EnableWindow(Flag);
		GetDlgItem(IDC_LAND_POINT_SHIPCOURT_CHECK)->EnableWindow(Flag);
		GetDlgItem(IDC_ACVCOURTINVEST_EDIT)->EnableWindow(Flag);
		GetDlgItem(IDC_SHIPCOURTINVEST_EDIT)->EnableWindow(Flag);
		GetDlgItem(IDC_LANDBASE_ACVSPACE_EDIT)->EnableWindow(Flag);
		GetDlgItem(IDC_LANDBASE_ACVSPACEPRICE_EDIT)->EnableWindow(Flag);
		GetDlgItem(IDC_LANDBASE_MOORLENGTH_EDIT)->EnableWindow(Flag);
		GetDlgItem(IDC_LANDBASE_MOORLENGTHPRICE_EDIT)->EnableWindow(Flag);
		GetDlgItem(IDC_MSFLEXGRID_LAND_REPAIR)->EnableWindow(Flag);
	}
	else if(Num == 2)
	{
		/*GetDlgItem(IDC_LAND_POINT_ACVCOURT_CHECK)->EnableWindow(FALSE);
		GetDlgItem(IDC_LAND_POINT_SHIPCOURT_CHECK)->EnableWindow(FALSE);
		GetDlgItem(IDC_ACVCOURTINVEST_EDIT)->EnableWindow(FALSE);
		GetDlgItem(IDC_SHIPCOURTINVEST_EDIT)->EnableWindow(FALSE);
		GetDlgItem(IDC_LANDBASE_ACVSPACE_EDIT)->EnableWindow(FALSE);
		GetDlgItem(IDC_LANDBASE_ACVSPACEPRICE_EDIT)->EnableWindow(FALSE);
		GetDlgItem(IDC_LANDBASE_MOORLENGTH_EDIT)->EnableWindow(FALSE);
		GetDlgItem(IDC_LANDBASE_MOORLENGTHPRICE_EDIT)->EnableWindow(FALSE);
		GetDlgItem(IDC_MSFLEXGRID_LAND_REPAIR)->EnableWindow(FALSE);
		GetDlgItem(IDC_LANDSTRUCT_ACTIVATE_CHECK)->EnableWindow(TRUE);
		*/
		if (m_ctrlLandStructCalcActivate.GetCheck())
			EnableLandStructSettings(3,TRUE);
		else
			EnableLandStructSettings(3,FALSE);
	}
	else if(Num == 3)
	{
		GetDlgItem(IDC_LAND_POINT_ACVCOURT_CHECK)->EnableWindow(Flag);
		GetDlgItem(IDC_LAND_POINT_SHIPCOURT_CHECK)->EnableWindow(Flag);
		GetDlgItem(IDC_ACVCOURTINVEST_EDIT)->EnableWindow(Flag);
		GetDlgItem(IDC_SHIPCOURTINVEST_EDIT)->EnableWindow(Flag);
		GetDlgItem(IDC_LANDBASE_ACVSPACE_EDIT)->EnableWindow(Flag);
		GetDlgItem(IDC_LANDBASE_ACVSPACEPRICE_EDIT)->EnableWindow(Flag);
		GetDlgItem(IDC_LANDBASE_MOORLENGTH_EDIT)->EnableWindow(Flag);
		GetDlgItem(IDC_LANDBASE_MOORLENGTHPRICE_EDIT)->EnableWindow(Flag);
		GetDlgItem(IDC_MSFLEXGRID_LAND_REPAIR)->EnableWindow(Flag);
	}
}
