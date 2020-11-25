// ShipTypePropertySheet.cpp : implementation file
//

#include "stdafx.h"
//#include "	\ add additional includes here"
#include "ShipTypePropertySheet.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CShipTypePropertySheet

IMPLEMENT_DYNAMIC(CShipTypePropertySheet, CPropertySheet)

CShipTypePropertySheet::CShipTypePropertySheet(UINT nIDCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(nIDCaption, pParentWnd, iSelectPage)
{
	AddControlPages();
	m_pParentWnd = pParentWnd;
	m_CShipTypeMainPPage.SetParentPtr(this);
	m_CShipTypeOptimizParamPPage.SetParentPtr(this);
	m_SelectedShipMainInfo = NULL;
	m_OpenStat = 0;
	//m_pShipsDBFile = "ships_db.fd";

}

CShipTypePropertySheet::CShipTypePropertySheet(LPCTSTR pszCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(pszCaption, pParentWnd, iSelectPage)
{
	AddControlPages();
	m_pParentWnd = pParentWnd;
	m_CShipTypeMainPPage.SetParentPtr(this);
	m_CShipTypeOptimizParamPPage.SetParentPtr(this);
	m_SelectedShipMainInfo = NULL;

	m_OpenStat = 0;
	//m_pShipsDBFile = "ships_db.fd";
}

CShipTypePropertySheet::~CShipTypePropertySheet()
{
}


BEGIN_MESSAGE_MAP(CShipTypePropertySheet, CPropertySheet)
	//{{AFX_MSG_MAP(CShipTypePropertySheet)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDOK, OnOK)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CShipTypePropertySheet message handlers

BOOL CShipTypePropertySheet::OnInitDialog() 
{
	BOOL bResult = CPropertySheet::OnInitDialog();
	
	// TODO: Add your specialized code here
	
	return bResult;
}

void CShipTypePropertySheet::DoDataExchange(CDataExchange* pDX) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	CPropertySheet::DoDataExchange(pDX);
}

void CShipTypePropertySheet::AddControlPages()
{
	AddPage(&m_CShipTypeMainPPage);
	AddPage(&m_CShipTypeOptimizParamPPage);

}

void CShipTypePropertySheet::Serialize(CArchive& ar) 
{
	CPropertySheet::Serialize(ar);
	m_CShipTypeMainPPage.Serialize(ar);
	m_CShipTypeOptimizParamPPage.Serialize(ar);

	if (ar.IsStoring())
	{	// storing code
	}
	else
	{	// loading code
	}
}

void CShipTypePropertySheet::OnOK()
{
	CFile f;
	CFileException e;

	if (!CheckPPagesData())
		return;

	if (m_OpenStat == 3){ //New Ship
		UpdateShipInDB(); // re-writing DB with changing RecordsNumber
		//RecordsNumIncrement();
		m_RecordsNumber++;
		f.Open( m_pShipsDBFile, CFile::modeWrite, &e);
		f.SeekToEnd();
		CArchive ar(&f, CArchive::store);
		Serialize(ar);
		ar.Close();
		m_OpenStat = 0;
		f.Close();
	}

	if (m_OpenStat == 2) { //Edit Ship
		//m_CShipTypeOptimizParamPPage.GetUpdatePPageDataStruct();
		UpdateShipInDB();
	}
		
	EndDialog(IDOK);
	
}

void CShipTypePropertySheet::RecordsNumIncrement(void){

	CFile f;
	CFileException e;
	char strBuf[3];

	f.Open( m_pShipsDBFile, CFile::modeWrite, &e);
	f.SeekToBegin();
	CArchive ar(&f, CArchive::store);
	m_RecordsNumber++;
	_itoa( m_RecordsNumber, strBuf, 10 );
	CString strNum = strBuf;
	ar << strNum;
	ar.Close();
	f.Close();

}


int CShipTypePropertySheet::GetIntFromArchive(CArchive& ar)
{
	char *stopstring;
	CString strData;
	int intData;

	ar >> strData;
	intData = (int)strtod(strData,&stopstring);

	return intData;
}

void CShipTypePropertySheet::PutIntToArchive(int intData, CArchive& ar1)
{
	char strData[5];
	CString strText;

	_itoa(intData, strData, 10);
	strText = strData;
	strText.Format("%s", strText);
	strText.TrimLeft();
	ar1 << strText;
}

void CShipTypePropertySheet::UpdateShipInDB()
{
	CString strNum;
	CString strText;
	CFile f;
	CFileException e;
	int RecNum;

	SetUpdatedShipInfo();
	
	CShipMainInfo ShipInfo[MAX_SHIP_RECORDS_IN_DB];
	if (m_RecordsNumber >= MAX_SHIP_RECORDS_IN_DB)
		AfxMessageBox("Количество судов в Базе Данных превысило установки программы.\nзначение MAX_SHIP_RECORDS_IN_DB должно быть увеличено в программе.");
	
	f.Open( m_pShipsDBFile, CFile::modeRead, &e);
	CArchive ar(&f, CArchive::load);

	if ((m_OpenStat == 3)&&(m_RecordsNumber == 0))
		RecNum = 0;
	else
		RecNum = GetIntFromArchive(ar);

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

		if ((m_OpenStat != 3) && (i == m_SelectedShipID)){ 
			//only for Edit; NOT for New Ship
			ShipInfo[i] = m_UpdatedShipInfo ;
		}

	}

	ar.Close();
	f.Close();

	/////////////// Rewriting (Updating) Archive file ///////////////
	int numTmp = 0;

	CFile f1( m_pShipsDBFile, CFile::modeCreate | CFile::modeWrite);
	CArchive ar1(&f1, CArchive::store);

	if (m_OpenStat == 3){//New ship add
		RecNum = m_RecordsNumber+1;
	}

	PutIntToArchive(RecNum, ar1);

	for (int j = 1; j <= m_RecordsNumber; j++){
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

}

void CShipTypePropertySheet::SetUpdatedShipInfo()
{
	CString strNum;
	CString strText;
	char *stopstring;

	m_CShipTypeMainPPage.m_ctrlShipTypeName.GetWindowText(strText);
	m_UpdatedShipInfo.m_strlShipTypeName = strText;
	
	m_CShipTypeMainPPage.m_ctrShipRegisterClass.GetWindowText(strText);
	m_UpdatedShipInfo.m_strShipRegisterClass = strText;
	
	m_CShipTypeMainPPage.m_ctrPrototypeName.GetWindowText(strText);
	m_UpdatedShipInfo.m_strPrototypeName = strText;

	m_CShipTypeMainPPage.m_ctrDW.GetWindowText(strNum);
	m_UpdatedShipInfo.m_nDW = (int)strtod(strNum,&stopstring);

	m_CShipTypeMainPPage.m_ctrlL.GetWindowText(strNum);
	m_UpdatedShipInfo.m_nL = (int)strtod(strNum,&stopstring);

	m_CShipTypeMainPPage.m_ctrlB.GetWindowText(strNum);
	m_UpdatedShipInfo.m_nB = (int)strtod(strNum,&stopstring);

	m_CShipTypeMainPPage.m_ctrlT.GetWindowText(strNum);
	m_UpdatedShipInfo.m_nT = (int)strtod(strNum,&stopstring);

	m_CShipTypeMainPPage.m_ctrlSpeed.GetWindowText(strNum);
	m_UpdatedShipInfo.m_nSpeed = (int)strtod(strNum,&stopstring);

	m_CShipTypeMainPPage.m_ctrRange.GetWindowText(strNum);
	m_UpdatedShipInfo.m_nRange = (int)strtod(strNum,&stopstring);
	
	m_CShipTypeMainPPage.m_ctr_h3.GetWindowText(strText);
	m_UpdatedShipInfo.m_strh3 = strText;
	
	m_UpdatedShipInfo.m_nCargoCompatiblGeneral = m_CShipTypeMainPPage.m_ctrlCargoCompatiblGeneral.GetCheck();
	m_UpdatedShipInfo.m_nCargoCompatiblOil = m_CShipTypeMainPPage.m_ctrlCargoCompatiblOil.GetCheck();
	m_UpdatedShipInfo.m_nCargoCompatiblCoal = m_CShipTypeMainPPage.m_ctrlCargoCompatiblCoal.GetCheck();
	m_UpdatedShipInfo.m_nCargoCompatibleVegFruits = m_CShipTypeMainPPage.m_ctrlCargoCompatibleVegFruits.GetCheck();

	SetActivePage(1);

	m_CShipTypeOptimizParamPPage.m_ctrlShipLoadMin.GetWindowText(strNum);
	//strNum = m_CShipTypeOptimizParamPPage.m_ppage_data.strShipLoadMin;
	m_UpdatedShipInfo.m_nLoadMin = (int)strtod(strNum,&stopstring);

	m_CShipTypeOptimizParamPPage.m_ctrlShipLoadMax.GetWindowText(strNum);
	//strNum = m_CShipTypeOptimizParamPPage.m_ppage_data.strShipLoadMax;
	m_UpdatedShipInfo.m_nLoadMax = (int)strtod(strNum,&stopstring);

	m_CShipTypeOptimizParamPPage.m_ctrlShipLoadDelta.GetWindowText(strNum);
	//strNum = m_CShipTypeOptimizParamPPage.m_ppage_data.strShipLoadDelta;
	m_UpdatedShipInfo.m_nLoadDelta = (int)strtod(strNum,&stopstring);

	m_CShipTypeOptimizParamPPage.m_ctrlShipTariffGenVF.GetWindowText(strNum);
	//strNum = m_CShipTypeOptimizParamPPage.m_ppage_data.strShipPriceMin;
	m_UpdatedShipInfo.m_nTariffGenVF = (int)strtod(strNum,&stopstring);

	m_CShipTypeOptimizParamPPage.m_ctrlShipTariffOil.GetWindowText(strNum);
	//strNum = m_CShipTypeOptimizParamPPage.m_ppage_data.strShipPriceMax;
	m_UpdatedShipInfo.m_nTariffOil = (int)strtod(strNum,&stopstring);

	m_CShipTypeOptimizParamPPage.m_ctrlShipTariffCoal.GetWindowText(strNum);
	//strNum = m_CShipTypeOptimizParamPPage.m_ppage_data.strShipPriceDelta;
	m_UpdatedShipInfo.m_nTariffCoal = (int)strtod(strNum,&stopstring);

	//begin: ship exist
	m_UpdatedShipInfo.m_nShipExistSetCheck = m_CShipTypeOptimizParamPPage.m_ctrlShipExistSetCheckBox.GetCheck();
	m_UpdatedShipInfo.m_nShipExistForceCheck = m_CShipTypeOptimizParamPPage.m_ctrlShipExistForceCheckBox.GetCheck();
	m_UpdatedShipInfo.m_nShipExistCheck = m_CShipTypeOptimizParamPPage.m_ctrlShipExistCheckBox.GetCheck();
	m_UpdatedShipInfo.m_nShipExistBuildCheck = m_CShipTypeOptimizParamPPage.m_ctrlShipExistBuildCheckBox.GetCheck();
	m_UpdatedShipInfo.m_nShipExistDesignCheck = m_CShipTypeOptimizParamPPage.m_ctrlShipExistDesignCheckBox.GetCheck();


	m_CShipTypeOptimizParamPPage.m_ctrlShipExistNumComboBox.GetWindowText(strNum);
	m_UpdatedShipInfo.m_nShipExistNum = (int)strtod(strNum,&stopstring);

	m_CShipTypeOptimizParamPPage.m_ctrlShipExistAdvantPrice.GetWindowText(strNum);
	m_UpdatedShipInfo.m_nShipExistAdvantPrice = (int)strtod(strNum,&stopstring);

	m_CShipTypeOptimizParamPPage.m_ctrlShipExistPrice.GetWindowText(strNum);
	m_UpdatedShipInfo.m_nShipExistPrice = (int)strtod(strNum,&stopstring);
	///
	m_CShipTypeOptimizParamPPage.m_ctrlShipExistBuildNumComboBox.GetWindowText(strNum);
	m_UpdatedShipInfo.m_nShipExistBuildNum = (int)strtod(strNum,&stopstring);

	m_CShipTypeOptimizParamPPage.m_ctrlShipExistBuildAdvantPrice.GetWindowText(strNum);
	m_UpdatedShipInfo.m_nShipExistBuildAdvantPrice = (int)strtod(strNum,&stopstring);

	m_CShipTypeOptimizParamPPage.m_ctrlShipExistBuildPrice.GetWindowText(strNum);
	m_UpdatedShipInfo.m_nShipExistBuildPrice = (int)strtod(strNum,&stopstring);
	///
	m_CShipTypeOptimizParamPPage.m_ctrlShipExistDesignAdvantPrice.GetWindowText(strNum);
	m_UpdatedShipInfo.m_nShipExistDesignAdvantPrice = (int)strtod(strNum,&stopstring);

	m_CShipTypeOptimizParamPPage.m_ctrlShipExistDesignPrice.GetWindowText(strNum);
	m_UpdatedShipInfo.m_nShipExistDesignPrice = (int)strtod(strNum,&stopstring);
	//end: ship exist

	SetActivePage(0);
}

int CShipTypePropertySheet::CheckPPagesData()
{
	int speed, flShipExist;
	CString strNum;
	char *stopstring;

//1. Проверка скорости
	m_CShipTypeMainPPage.m_ctrlSpeed.GetWindowText(strNum);
	speed = (int)strtod(strNum,&stopstring);

	SetActivePage(1);
	flShipExist = m_CShipTypeOptimizParamPPage.m_ctrlShipExistSetCheckBox.GetCheck();
	SetActivePage(0);

	if ((speed < 1)&& (flShipExist != 1)){
		AfxMessageBox("Некорректное значение скорости судна");
		return 0;
	}
// end of 1.

	return 1;
}
