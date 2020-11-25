// PortMainDetailsPPage.cpp : implementation file
//

#include "stdafx.h"
//#include "	\ add additional includes here"
#include "PortMainDetailsPPage.h"
#include "PortDetailsPSheet.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPortMainDetailsPPage property page

IMPLEMENT_DYNCREATE(CPortMainDetailsPPage, CPropertyPage)

CPortMainDetailsPPage::CPortMainDetailsPPage() : CPropertyPage(CPortMainDetailsPPage::IDD)
{
	//{{AFX_DATA_INIT(CPortMainDetailsPPage)
	m_strPortRegion = _T("");
	m_strPortName = _T("");
	m_strPortType = _T("");
	m_strBasePortName = _T("");
	m_strPortID = _T("");
	//}}AFX_DATA_INIT

	m_ISModifiable = FALSE;
	m_ISLoadDetails = FALSE;
	m_bInitialized = FALSE;
	m_SelectedPortMainInfo = NULL;

}

CPortMainDetailsPPage::~CPortMainDetailsPPage()
{
}

void CPortMainDetailsPPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPortMainDetailsPPage)
	DDX_Control(pDX, IDC_PORTID, m_ctrlPortID);
	DDX_Control(pDX, IDC_PORTNAME_TEXT, m_ctrlPortName);
	DDX_Control(pDX, IDC_COMBO2, m_ctrlPortRegion);
	DDX_Control(pDX, IDC_COMBO1, m_ctrlPortType);
	DDX_Control(pDX, IDC_BASEPORTNAME_TEXT, m_ctrlBasePortName);
	DDX_CBString(pDX, IDC_COMBO2, m_strPortRegion);
	DDX_Text(pDX, IDC_PORTNAME_TEXT, m_strPortName);
	DDX_CBString(pDX, IDC_COMBO1, m_strPortType);
	DDX_Text(pDX, IDC_BASEPORTNAME_TEXT, m_strBasePortName);
	DDX_Control(pDX, IDC_MSFLEXGRID_CARGOINFO, m_CargoInfoGrid);
	DDX_Text(pDX, IDC_PORTID, m_strPortID);
	//}}AFX_DATA_MAP
	//Test1();

/*	if (m_OpenStat == 1){
		FillPortDetails();
		//DisableControls();
	}else if (m_OpenStat == 2)
		FillPortDetails();
*/	
	if((!m_bInitialized) && (m_OpenStat != 3))
	{
		FillPortDetails();
		m_bInitialized = TRUE;		
	}
	if (m_OpenStat == 1)
		DisableControls();
}


BEGIN_MESSAGE_MAP(CPortMainDetailsPPage, CPropertyPage)
	//{{AFX_MSG_MAP(CPortMainDetailsPPage)
	ON_EN_CHANGE(IDC_PORTNAME_TEXT, OnChangePortnameText)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPortMainDetailsPPage message handlers

//DEL CEdit& CPortMainDetailsPPage::Edit1()
//DEL {
//DEL 	return *(CEdit*)GetDlgItem(IDC_PORTNAME_TEXT);
//DEL }

void CPortMainDetailsPPage::Test1()
{
	if (m_OpenStat != 1)
		return;
		
	m_strPortRegion = _T(m_SelectedPortMainInfo->m_strPortRegion);
	m_strPortName = _T(m_SelectedPortMainInfo->m_strPortName);
	m_strPortType = _T(m_SelectedPortMainInfo->m_strPortType);
	m_strBasePortName = _T(m_SelectedPortMainInfo->m_strBasePortName);

	m_strPortRegion = _T("KUKU");
	


/*		char strRowNum[5];
		
		CScenarioWizardDlg* pScenDlg;

		pScenDlg = (CScenarioWizardDlg*)((CPortDetailsPSheet*)m_pParentPtr)->GetParentWndPoint();

		int testNum = pScenDlg->m_test; //stab
		_itoa(testNum, strRowNum, 10);


		//CString strMatrixValue = "777";



		m_CargoInfoGrid.SetTextMatrix(1,1,strRowNum);	
		m_CargoInfoGrid.SetTextMatrix(2,1,strRowNum);
		m_CargoInfoGrid.SetTextMatrix(3,1,strRowNum);
		m_CargoInfoGrid.SetTextMatrix(4,1,strRowNum);	

		m_strPortName = _T("testNum");
		m_strPortType = _T("testNum");
		m_strBasePortName = _T("testNum");
		m_strPortRegion = _T("testNum");
*/		
}


//DEL BOOL CPortMainDetailsPPage::OnApply() 
//DEL {
//DEL 	// TODO: Add your specialized code here and/or call the base class
//DEL 	UpdateData();
//DEL 	
//DEL //	Test1();
//DEL /*
//DEL 	char * pProjectFile1 = "port_log1.fl";	
//DEL 	CFile f( pProjectFile1, CFile::modeCreate | CFile::modeWrite );
//DEL 	CArchive ar(&f, CArchive::store);
//DEL 	Serialize(ar);
//DEL */
//DEL 
//DEL 	return CPropertyPage::OnApply();
//DEL }

void CPortMainDetailsPPage::OnChangePortnameText() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CPropertyPage::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	SetModified(TRUE);
//		SetCargoTableTitles();

	
}

void CPortMainDetailsPPage::SetCargoTableTitles()
{
	//GetDlgItem(IDC_MSFLEXGRID_CARGOINFO)->
	m_CargoInfoGrid.SetTextMatrix(1,0,_T("Уголь"));
	m_CargoInfoGrid.SetTextMatrix(2,0,_T("Генгрузы"));
	m_CargoInfoGrid.SetTextMatrix(3,0,_T("Нефть"));
	m_CargoInfoGrid.SetTextMatrix(4,0,_T("Овощи-фрукты"));
}

BEGIN_EVENTSINK_MAP(CPortMainDetailsPPage, CPropertyPage)
    //{{AFX_EVENTSINK_MAP(CPortMainDetailsPPage)
	ON_EVENT(CPortMainDetailsPPage, IDC_MSFLEXGRID_CARGOINFO, -601 /* DblClick */, OnDblClickCargoinfoGrid, VTS_NONE)
	ON_EVENT(CPortMainDetailsPPage, IDC_MSFLEXGRID_CARGOINFO, 73 /* Scroll */, OnScrollCargoinfoGrid, VTS_NONE)
	ON_EVENT(CPortMainDetailsPPage, IDC_MSFLEXGRID_CARGOINFO, -600 /* Click */, OnClickCargoinfoGrid, VTS_NONE)
	ON_EVENT(CPortMainDetailsPPage, IDC_MSFLEXGRID_CARGOINFO, 69 /* SelChange */, OnSelChangeCargoinfoGrid, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()



void CPortMainDetailsPPage::OnDblClickCargoinfoGrid() 
{
	// TODO: Add your control notification handler code here

	//there is CEditGrid::OnKeyPressGrid(short FAR* ascii) handler,
	//but we cancel it.Instead of catch "OnDblClick" event in CEditGrid,
	//we use it here.

	if (m_ISModifiable == TRUE){
		short i = 13;
		m_CargoInfoGrid.OnKeyPressGrid(&i);// Simulate a return.
	}
}

void CPortMainDetailsPPage::OnScrollCargoinfoGrid() 
{
	// TODO: Add your control notification handler code here
	m_CargoInfoGrid.OnUpdateGrid();
	
}

void CPortMainDetailsPPage::OnClickCargoinfoGrid() 
{
	// TODO: Add your control notification handler code here
	
}

void CPortMainDetailsPPage::OnSelChangeCargoinfoGrid() 
{
	// TODO: Add your control notification handler code here
	
}

void CPortMainDetailsPPage::Serialize(CArchive& ar) 
{
	CString strMatrixValue, str;
	int i, j;

	if (ar.IsStoring())
	{	// storing code

/*		ar << m_strPortName;
		ar << m_strPortType;
		ar << m_strBasePortName;
		ar << m_strPortRegion;
*/
		m_ctrlPortName.GetWindowText(str);
		ar << str;
		m_ctrlPortType.GetWindowText(str);
		ar << str;
		m_ctrlBasePortName.GetWindowText(str);
		ar << str;
		m_ctrlPortRegion.GetWindowText(str);
		ar << str;

/*
		ar << m_CargoInfoGrid.GetTextMatrix(1,1);	//required_cargoes.coal
		ar << m_CargoInfoGrid.GetTextMatrix(2,1);	//required_cargoes.general
		ar << m_CargoInfoGrid.GetTextMatrix(3,1);	//required_cargoes.oil
		ar << m_CargoInfoGrid.GetTextMatrix(4,1);	//required_cargoes.vegetables_fruits
		ar << m_CargoInfoGrid.GetTextMatrix(1,2);	//available_cargoes.coal
		ar << m_CargoInfoGrid.GetTextMatrix(2,2);	//available_cargoes.general
		ar << m_CargoInfoGrid.GetTextMatrix(3,2);	//available_cargoes.oil
		ar << m_CargoInfoGrid.GetTextMatrix(4,2);	//available_cargoes.vegetables_fruits

		ar << m_CargoInfoGrid.GetTextMatrix(1,3);	//Ship load t/h. coal
		ar << m_CargoInfoGrid.GetTextMatrix(2,3);	//Ship load t/h. general
		ar << m_CargoInfoGrid.GetTextMatrix(3,3);	//Ship load t/h. oil
		ar << m_CargoInfoGrid.GetTextMatrix(4,3);	//Ship load t/h. vegetables_fruits

		ar << m_CargoInfoGrid.GetTextMatrix(1,4);	//ACV load t/h. coal
		ar << m_CargoInfoGrid.GetTextMatrix(2,4);	//ACV load t/h. general
		ar << m_CargoInfoGrid.GetTextMatrix(3,4);	//ACV load t/h. oil
		ar << m_CargoInfoGrid.GetTextMatrix(4,4);	//ACV load t/h. vegetables_fruits
*/
		for (j = 1; j <=4; j++){ //j - column
			for (i = 1; i <=4; i++){	//i = row
				strMatrixValue = m_CargoInfoGrid.GetTextMatrix(i,j);
				if (strMatrixValue == "")
					strMatrixValue = "0";
				ar << strMatrixValue;
			}
		}
	}
	else
	{	// loading code
/*
		ar >> m_strPortName;
		ar >> m_strPortType;
		ar >> m_strBasePortName;
		ar >> m_strPortRegion;
		
		ar >> strMatrixValue;
		m_CargoInfoGrid.SetTextMatrix(1,1,strMatrixValue);	//required_cargoes.coal
		strMatrixValue = "";
		ar >> strMatrixValue;
		m_CargoInfoGrid.SetTextMatrix(2,1,strMatrixValue);	//required_cargoes.general
		strMatrixValue = "";
		ar >> strMatrixValue;
		m_CargoInfoGrid.SetTextMatrix(3,1,strMatrixValue);	//required_cargoes.oil
		strMatrixValue = "";
		ar >> strMatrixValue;
		m_CargoInfoGrid.SetTextMatrix(4,1,strMatrixValue);	//required_cargoes.vegetables_fruits
		strMatrixValue = "";
		ar >> strMatrixValue;
		m_CargoInfoGrid.SetTextMatrix(1,2,strMatrixValue);	//available_cargoes.coal
		strMatrixValue = "";
		ar >> strMatrixValue;
		m_CargoInfoGrid.SetTextMatrix(2,2,strMatrixValue);	//available_cargoes.general
		strMatrixValue = "";
		ar >> strMatrixValue;
		m_CargoInfoGrid.SetTextMatrix(3,2,strMatrixValue);	//available_cargoes.oil
		strMatrixValue = "";
		ar >> strMatrixValue;
		m_CargoInfoGrid.SetTextMatrix(4,2,strMatrixValue);	//available_cargoes.vegetables_fruits
*/		
	}
}



//DEL BOOL CPortMainDetailsPPage::OnInitDialog()
//DEL {
//DEL 		m_strPortName = _T("NUNU");
//DEL 		m_strPortType = _T("PNB");
//DEL 		m_strBasePortName = _T("Prov");
//DEL 		m_strPortRegion = _T("Arctic");
//DEL 		
//DEL 
//DEL 		return TRUE;
//DEL 
//DEL }


void CPortMainDetailsPPage::SetParentPtr(CPropertySheet *p)
{
			m_pParentPtr = (CPortDetailsPSheet*)p;
}

BOOL CPortMainDetailsPPage::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	FillGridsTitles();
	//FillPortDetails(); //?
	return TRUE;
}

void CPortMainDetailsPPage::FillPortDetails()
{
	char strNum[5];
	int num;


	if (m_OpenStat == 3){ // показать ID нового порта
		// найти номер последней записи,  +1
		m_RecordsNumber++;
		num = m_RecordsNumber;	
		_itoa(num, strNum, 10);
		m_ctrlPortID.SetWindowText(strNum);		
	}
	else
	{
		num = m_SelectedPortMainInfo->m_nPortID;
		_itoa(num, strNum, 10);
		m_ctrlPortID.SetWindowText(strNum);

		//m_strPortRegion = _T(m_SelectedPortMainInfo->m_strPortRegion);
		//m_strPortName = _T(m_SelectedPortMainInfo->m_strPortName);
		//m_strPortType = _T(m_SelectedPortMainInfo->m_strPortType);
		//m_strBasePortName = _T(m_SelectedPortMainInfo->m_strBasePortName);
		m_ctrlPortRegion.SetWindowText(m_SelectedPortMainInfo->m_strPortRegion);
		m_ctrlPortName.SetWindowText(m_SelectedPortMainInfo->m_strPortName);
		m_ctrlPortType.SetWindowText(m_SelectedPortMainInfo->m_strPortType);
		m_ctrlBasePortName.SetWindowText(m_SelectedPortMainInfo->m_strBasePortName);
		
		num = m_SelectedPortMainInfo->required_cargoes.coal;
		_itoa(num, strNum, 10);
		m_CargoInfoGrid.SetTextMatrix(1,1,strNum);	

		num = m_SelectedPortMainInfo->required_cargoes.general;
		_itoa(num, strNum, 10);
		m_CargoInfoGrid.SetTextMatrix(2,1,strNum);

		num = m_SelectedPortMainInfo->required_cargoes.oil;
		_itoa(num, strNum, 10);
		m_CargoInfoGrid.SetTextMatrix(3,1,strNum);

		num = m_SelectedPortMainInfo->required_cargoes.vegetables_fruits;
		_itoa(num, strNum, 10);
		m_CargoInfoGrid.SetTextMatrix(4,1,strNum);

		num = m_SelectedPortMainInfo->available_cargoes.coal;
		_itoa(num, strNum, 10);
		m_CargoInfoGrid.SetTextMatrix(1,2,strNum);	

		num = m_SelectedPortMainInfo->available_cargoes.general;
		_itoa(num, strNum, 10);
		m_CargoInfoGrid.SetTextMatrix(2,2,strNum);

		num = m_SelectedPortMainInfo->available_cargoes.oil;
		_itoa(num, strNum, 10);
		m_CargoInfoGrid.SetTextMatrix(3,2,strNum);

		num = m_SelectedPortMainInfo->available_cargoes.vegetables_fruits;
		_itoa(num, strNum, 10);
		m_CargoInfoGrid.SetTextMatrix(4,2,strNum);
		////
		num = m_SelectedPortMainInfo->ship_loading_productivity.coal;
		_itoa(num, strNum, 10);
		m_CargoInfoGrid.SetTextMatrix(1,3,strNum);	

		num = m_SelectedPortMainInfo->ship_loading_productivity.general;
		_itoa(num, strNum, 10);
		m_CargoInfoGrid.SetTextMatrix(2,3,strNum);

		num = m_SelectedPortMainInfo->ship_loading_productivity.oil;
		_itoa(num, strNum, 10);
		m_CargoInfoGrid.SetTextMatrix(3,3,strNum);

		num = m_SelectedPortMainInfo->ship_loading_productivity.vegetables_fruits;
		_itoa(num, strNum, 10);
		m_CargoInfoGrid.SetTextMatrix(4,3,strNum);
		////
		num = m_SelectedPortMainInfo->acv_loading_productivity.coal;
		_itoa(num, strNum, 10);
		m_CargoInfoGrid.SetTextMatrix(1,4,strNum);	

		num = m_SelectedPortMainInfo->acv_loading_productivity.general;
		_itoa(num, strNum, 10);
		m_CargoInfoGrid.SetTextMatrix(2,4,strNum);

		num = m_SelectedPortMainInfo->acv_loading_productivity.oil;
		_itoa(num, strNum, 10);
		m_CargoInfoGrid.SetTextMatrix(3,4,strNum);

		num = m_SelectedPortMainInfo->acv_loading_productivity.vegetables_fruits;
		_itoa(num, strNum, 10);
		m_CargoInfoGrid.SetTextMatrix(4,4,strNum);


	}

}

	

void CPortMainDetailsPPage::DisableControls()
{
	if (m_OpenStat == 1){
		m_ctrlBasePortName.EnableWindow(FALSE);
		m_ctrlPortName.EnableWindow(FALSE);
		m_ctrlPortType.EnableWindow(FALSE);
		m_ctrlPortRegion.EnableWindow(FALSE);
	}
	m_ctrlPortID.EnableWindow(FALSE);

}

void CPortMainDetailsPPage::FillGridsTitles()
{
	m_CargoInfoGrid.SetTextMatrix(1,0,_T("Уголь"));
	m_CargoInfoGrid.SetTextMatrix(2,0,_T("Генгрузы"));
	m_CargoInfoGrid.SetTextMatrix(3,0,_T("Нефть"));
	m_CargoInfoGrid.SetTextMatrix(4,0,_T("Овощи-фрукты"));

}
