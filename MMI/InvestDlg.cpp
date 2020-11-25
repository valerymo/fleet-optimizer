// InvestDlg.cpp : implementation file
//

#include "stdafx.h"
//#include "	\ add additional includes here"
#include "InvestDlg.h"
#include "..\FleetOptimizer\FleetOptimizer.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CInvestDlg dialog


CInvestDlg::CInvestDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CInvestDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CInvestDlg)
	m_InvestmentSize = 0;
	m_bIncludeLandStructCalc = FALSE;
	//}}AFX_DATA_INIT
	m_nID = CInvestDlg::IDD;
	CDialog::Create(m_nID, pParent);

	ReadFromRegistry();
	FillDialogDetails();
}


void CInvestDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CInvestDlg)
	DDX_Control(pDX, IDC_CHECK_LANDSTRUCTINCLUDE, m_ctrlIncludeLandStructCalcInScen);
	DDX_Control(pDX, IDC_EDIT3, m_ctrInvestmentSize);
	DDX_Text(pDX, IDC_EDIT3, m_InvestmentSize);
	DDV_MinMaxLong(pDX, m_InvestmentSize, 0, 1000000000);
	DDX_Check(pDX, IDC_CHECK_LANDSTRUCTINCLUDE, m_bIncludeLandStructCalc);
	//}}AFX_DATA_MAP

}


BEGIN_MESSAGE_MAP(CInvestDlg, CDialog)
	//{{AFX_MSG_MAP(CInvestDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CInvestDlg message handlers




///////
void CInvestDlg::SaveToRegistry() {

	((CFleetOptimizerApp*)AfxGetApp())->WriteProfileInt("FleetOptimizer","Investment",m_InvestmentSize);	
	((CFleetOptimizerApp*)AfxGetApp())->WriteProfileInt("FleetOptimizer","IncludeLandStructCalc",m_bIncludeLandStructCalc);	

}

void CInvestDlg::OnOK() 
{
	CString str;
	char *stopstring;
	m_ctrInvestmentSize.GetWindowText(str);
	m_InvestmentSize = (int)strtod(str,&stopstring);

	m_bIncludeLandStructCalc = m_ctrlIncludeLandStructCalcInScen.GetCheck();
	
	SaveToRegistry();

	
	CDialog::OnOK();
}

void CInvestDlg::ReadFromRegistry()
{
	m_InvestmentSize = ((CFleetOptimizerApp*)AfxGetApp())->GetProfileInt("FleetOptimizer","Investment",0);	
	m_bIncludeLandStructCalc = ((CFleetOptimizerApp*)AfxGetApp())->GetProfileInt("FleetOptimizer","IncludeLandStructCalc",0);	
}



void CInvestDlg::FillDialogDetails()
{
	char strNum[10];

	_itoa(m_InvestmentSize, strNum, 10);
	m_ctrInvestmentSize.SetWindowText(strNum);
	m_ctrlIncludeLandStructCalcInScen.SetCheck(m_bIncludeLandStructCalc);
}
