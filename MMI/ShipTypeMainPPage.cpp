// ShipTypeMainPPage.cpp : implementation file
//

#include "stdafx.h"
//#include "	\ add additional includes here"
#include "ShipTypeMainPPage.h"
#include "ShipTypePropertySheet.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CShipTypeMainPPage property page

IMPLEMENT_DYNCREATE(CShipTypeMainPPage, CPropertyPage)

CShipTypeMainPPage::CShipTypeMainPPage() : CPropertyPage(CShipTypeMainPPage::IDD)
{
	//{{AFX_DATA_INIT(CShipTypeMainPPage)
	//}}AFX_DATA_INIT

	m_bInitialized = FALSE;		

}

CShipTypeMainPPage::~CShipTypeMainPPage()
{
}

void CShipTypeMainPPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CShipTypeMainPPage)
	DDX_Control(pDX, IDC_RANGE_EDIT, m_ctrRange);
	DDX_Control(pDX, IDC_H3_EDIT, m_ctr_h3);
	DDX_Control(pDX, IDC_SHIPTYPE_SPEED_TEXT, m_ctrlSpeed);
	DDX_Control(pDX, IDC_SHIPTYPE_T_TEXT2, m_ctrlT);
	DDX_Control(pDX, IDC_SHIPTYPE_L_TEXT2, m_ctrlL);
	DDX_Control(pDX, IDC_SHIPTYPE_B_TEXT2, m_ctrlB);
	DDX_Control(pDX, IDC_SHIPTYPEDW_TEXT, m_ctrDW);
	DDX_Control(pDX, IDC_CHECK3, m_ctrlCargoCompatibleVegFruits);
	DDX_Control(pDX, IDC_CHECK4, m_ctrlCargoCompatiblCoal);
	DDX_Control(pDX, IDC_CHECK2, m_ctrlCargoCompatiblOil);
	DDX_Control(pDX, IDC_CHECK1, m_ctrlCargoCompatiblGeneral);
	DDX_Control(pDX, IDC_SHIPTYPEREGISTERCLASS, m_ctrShipRegisterClass);
	DDX_Control(pDX, IDC_PROTOTYPENAME, m_ctrPrototypeName);
	DDX_Control(pDX, IDC_SHIPTYPENAME_TEXT, m_ctrlShipTypeName);
	//}}AFX_DATA_MAP

	if((!m_bInitialized) && (m_OpenStat != 3))
	{
		FillShipDetails();
		m_bInitialized = TRUE;		
	}
	
	if (m_OpenStat == 1)
		DisableControls();

}


BEGIN_MESSAGE_MAP(CShipTypeMainPPage, CPropertyPage)
	//{{AFX_MSG_MAP(CShipTypeMainPPage)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CShipTypeMainPPage message handlers

void CShipTypeMainPPage::Serialize(CArchive& ar) 
{
	CString str;
	int CheckState;

	if (ar.IsStoring())
	{	// storing code
		m_ctrlShipTypeName.GetWindowText(str);
		ar << str;
		m_ctrShipRegisterClass.GetWindowText(str);
		ar << str;
		m_ctrPrototypeName.GetWindowText(str);
		ar << str;
		m_ctrDW.GetWindowText(str);
		ar << str;
		m_ctrlL.GetWindowText(str);
		ar << str;
		m_ctrlB.GetWindowText(str);
		ar << str;
		m_ctrlT.GetWindowText(str);
		ar << str;
		m_ctrlSpeed.GetWindowText(str);
		ar << str;
		m_ctrRange.GetWindowText(str);
		ar << str;
		m_ctr_h3.GetWindowText(str);
		ar << str;


		CheckState = m_ctrlCargoCompatiblGeneral.GetCheck();
		((CShipTypePropertySheet*)m_pParentPtr)->PutIntToArchive(CheckState,ar);
		CheckState = m_ctrlCargoCompatiblOil.GetCheck();
		((CShipTypePropertySheet*)m_pParentPtr)->PutIntToArchive(CheckState,ar);
		CheckState = m_ctrlCargoCompatiblCoal.GetCheck();
		((CShipTypePropertySheet*)m_pParentPtr)->PutIntToArchive(CheckState,ar);
		CheckState = m_ctrlCargoCompatibleVegFruits.GetCheck();
		((CShipTypePropertySheet*)m_pParentPtr)->PutIntToArchive(CheckState,ar);
	}
	else
	{	// loading code
	}
}

void CShipTypeMainPPage::SetParentPtr(CPropertySheet *p)
{
		m_pParentPtr = (CShipTypePropertySheet*)p;
}

void CShipTypeMainPPage::FillShipDetails()
{
	char strNum[5];
	int num;

	m_ctrlShipTypeName.SetWindowText(((CShipTypePropertySheet*)m_pParentPtr)->m_SelectedShipMainInfo->m_strlShipTypeName);
	m_ctrShipRegisterClass.SetWindowText(((CShipTypePropertySheet*)m_pParentPtr)->m_SelectedShipMainInfo->m_strShipRegisterClass);
	m_ctrPrototypeName.SetWindowText(((CShipTypePropertySheet*)m_pParentPtr)->m_SelectedShipMainInfo->m_strPrototypeName);
	
	num = ((CShipTypePropertySheet*)m_pParentPtr)->m_SelectedShipMainInfo->m_nDW;
	_itoa(num, strNum, 10);
	m_ctrDW.SetWindowText(strNum);

	num = ((CShipTypePropertySheet*)m_pParentPtr)->m_SelectedShipMainInfo->m_nL;
	_itoa(num, strNum, 10);
	m_ctrlL.SetWindowText(strNum);

	num = ((CShipTypePropertySheet*)m_pParentPtr)->m_SelectedShipMainInfo->m_nB;
	_itoa(num, strNum, 10);
	m_ctrlB.SetWindowText(strNum);

	num = ((CShipTypePropertySheet*)m_pParentPtr)->m_SelectedShipMainInfo->m_nT;
	_itoa(num, strNum, 10);
	m_ctrlT.SetWindowText(strNum);

	num = ((CShipTypePropertySheet*)m_pParentPtr)->m_SelectedShipMainInfo->m_nSpeed;
	_itoa(num, strNum, 10);
	m_ctrlSpeed.SetWindowText(strNum);

	num = ((CShipTypePropertySheet*)m_pParentPtr)->m_SelectedShipMainInfo->m_nRange;
	_itoa(num, strNum, 10);
	m_ctrRange.SetWindowText(strNum);

	float source = ((CShipTypePropertySheet*)m_pParentPtr)->m_SelectedShipMainInfo->m_nh3;
    char    buffer[10];
	//int     decimal,   sign;
    int     precision = 2;
	//buffer = _ecvt( source, precision, &decimal, &sign );
	_gcvt ( source,3, buffer );
	//strcpy(strNum, buffer);
  	//_itoa(num, strNum, 10);
	m_ctr_h3.SetWindowText(buffer);

	//m_ctr_h3.SetWindowText(((CShipTypePropertySheet*)m_pParentPtr)->m_SelectedShipMainInfo->m_strh3);

	num = ((CShipTypePropertySheet*)m_pParentPtr)->m_SelectedShipMainInfo->m_nCargoCompatiblGeneral;
	m_ctrlCargoCompatiblGeneral.SetCheck(num);

	num = ((CShipTypePropertySheet*)m_pParentPtr)->m_SelectedShipMainInfo->m_nCargoCompatiblOil;
	m_ctrlCargoCompatiblOil.SetCheck(num);

	num = ((CShipTypePropertySheet*)m_pParentPtr)->m_SelectedShipMainInfo->m_nCargoCompatiblCoal;
	m_ctrlCargoCompatiblCoal.SetCheck(num);

	num = ((CShipTypePropertySheet*)m_pParentPtr)->m_SelectedShipMainInfo->m_nCargoCompatibleVegFruits;
	m_ctrlCargoCompatibleVegFruits.SetCheck(num);

}

void CShipTypeMainPPage::DisableControls()
{
	m_ctrlL.SetReadOnly(TRUE);
	m_ctrlB.SetReadOnly(TRUE);
	m_ctrlT.SetReadOnly(TRUE);
	m_ctrDW.SetReadOnly(TRUE);
	m_ctrlSpeed.SetReadOnly(TRUE);
	m_ctrRange.SetReadOnly(TRUE);
	m_ctr_h3.SetReadOnly(TRUE);
	m_ctrlCargoCompatibleVegFruits.EnableWindow(FALSE);
	m_ctrlCargoCompatiblCoal.EnableWindow(FALSE);
	m_ctrlCargoCompatiblOil.EnableWindow(FALSE);
	m_ctrlCargoCompatiblGeneral.EnableWindow(FALSE);
	m_ctrShipRegisterClass.SetReadOnly(TRUE);
	m_ctrPrototypeName.SetReadOnly(TRUE);
	m_ctrlShipTypeName.SetReadOnly(TRUE);
}

