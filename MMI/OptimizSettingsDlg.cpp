// OptimizSettingsDlg.cpp : implementation file
//

#include "stdafx.h"
//#include "mmi.h"
#include "OptimizSettingsDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// COptimizSettingsDlg dialog
class CFleetOptimizerApp;

COptimizSettingsDlg::COptimizSettingsDlg(CWnd* pParent /*=NULL*/)
	: CDialog(COptimizSettingsDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(COptimizSettingsDlg)
	m_bUseOptAll = TRUE;
	m_bUseOptStep1 = FALSE;
	m_bUseOptStep2 = FALSE;
	m_bUseOptStep3 = FALSE;
	m_bUseSensitivityCheck = FALSE;
	//}}AFX_DATA_INIT
}


void COptimizSettingsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(COptimizSettingsDlg)
	DDX_Control(pDX, IDC_SENSE_EDIT1, m_ctrlSenceCheckRate);
	DDX_Control(pDX, IDC_SENSE_CHECK1, m_ctrlUseSensitivityCheck);
	DDX_Control(pDX, IDC_SPECIFIC_RADIO, m_ctrlUseOptSpecific);
	DDX_Control(pDX, IDC_ALL_RADIO, m_ctrlUseOptAll);
	DDX_Control(pDX, IDC_STEP3_CHECK, m_ctrOptStep3Check);
	DDX_Control(pDX, IDC_STEP2_CHECK, m_ctrOptStep2Check);
	DDX_Control(pDX, IDC_STEP1_CHECK, m_ctrOptStep1Check);
	DDX_Radio(pDX, IDC_ALL_RADIO, m_bUseOptAll);
	DDX_Check(pDX, IDC_STEP1_CHECK, m_bUseOptStep1);
	DDX_Check(pDX, IDC_STEP2_CHECK, m_bUseOptStep2);
	DDX_Check(pDX, IDC_STEP3_CHECK, m_bUseOptStep3);
	DDX_Check(pDX, IDC_SENSE_CHECK1, m_bUseSensitivityCheck);
	//}}AFX_DATA_MAP

	LoadSettings();

}


BEGIN_MESSAGE_MAP(COptimizSettingsDlg, CDialog)
	//{{AFX_MSG_MAP(COptimizSettingsDlg)
	ON_BN_CLICKED(IDC_ALL_RADIO, OnUseAllOptimizRadio)
	ON_BN_CLICKED(IDC_SPECIFIC_RADIO, OnUseSpecificOptimizRadio)
	ON_BN_CLICKED(IDC_SENSE_CHECK1, OnSensitivityCheck)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COptimizSettingsDlg message handlers

void COptimizSettingsDlg::OnUseAllOptimizRadio() 
{
	UpdateData(TRUE);
	EnableOptimizSettings(TRUE);	
}

void COptimizSettingsDlg::OnUseSpecificOptimizRadio() 
{
	UpdateData(TRUE);
	EnableOptimizSettings(FALSE);	
}

void COptimizSettingsDlg::EnableOptimizSettings(BOOL Flag)
{
	if (Flag == 1){
		m_bUseOptAll = TRUE;
		m_ctrlUseOptAll.SetCheck(1);
		m_ctrlUseOptSpecific.SetCheck(0);
		GetDlgItem(IDC_STEP1_CHECK)->EnableWindow(FALSE);
		GetDlgItem(IDC_STEP2_CHECK)->EnableWindow(FALSE);
		GetDlgItem(IDC_STEP3_CHECK)->EnableWindow(FALSE);
	}
	else{
		m_bUseOptAll = FALSE;
		m_ctrlUseOptAll.SetCheck(0);
		m_ctrlUseOptSpecific.SetCheck(1);
		GetDlgItem(IDC_STEP1_CHECK)->EnableWindow(TRUE);
		GetDlgItem(IDC_STEP2_CHECK)->EnableWindow(TRUE);
		GetDlgItem(IDC_STEP3_CHECK)->EnableWindow(TRUE);
	}
}

void COptimizSettingsDlg::SaveSelectedOptions()
{
	BOOL bStep1 = 0;
	BOOL bStep2 = 0;
	BOOL bStep3 = 0;
	BOOL bSenseCheck = 0;

	if (m_ctrOptStep1Check.GetCheck() == 1)
		bStep1 = 1;
	if (m_ctrOptStep2Check.GetCheck() == 1)
		bStep2 = 1;
	if (m_ctrOptStep3Check.GetCheck() == 1)
		bStep3 = 1;
	if (m_ctrlUseSensitivityCheck.GetCheck() == 1)
		bSenseCheck = 1;


	((CFleetOptimizerApp*)AfxGetApp())->WriteProfileInt("FleetOptimizer","UseOptAll",m_bUseOptAll);	
	((CFleetOptimizerApp*)AfxGetApp())->WriteProfileInt("FleetOptimizer","UseOptStep1",bStep1);	
	((CFleetOptimizerApp*)AfxGetApp())->WriteProfileInt("FleetOptimizer","UseOptStep2",bStep2);	
	((CFleetOptimizerApp*)AfxGetApp())->WriteProfileInt("FleetOptimizer","UseOptStep3",bStep3);	
	((CFleetOptimizerApp*)AfxGetApp())->WriteProfileInt("FleetOptimizer","UseSenseCheck",bSenseCheck);	
	((CFleetOptimizerApp*)AfxGetApp())->WriteProfileInt("FleetOptimizer","SenseCheckRate",m_nSenceCheckRate);	

}

void COptimizSettingsDlg::LoadSettings()
{
	if (m_bUseOptAll == TRUE){
		EnableOptimizSettings(TRUE);
		m_ctrlUseOptAll.SetCheck(1);
	}
	else
		EnableOptimizSettings(FALSE);

	char strNum[10];
	_itoa(m_nSenceCheckRate, strNum, 10);
	m_ctrlSenceCheckRate.SetWindowText(strNum);	
	if (m_bUseSensitivityCheck == FALSE){
		GetDlgItem(IDC_SENSE_EDIT1)->EnableWindow(FALSE);
	}
}

void COptimizSettingsDlg::OnOK() 
{
	// TODO: Add extra validation here
	CString str;
	char *stopstring;
	m_ctrlSenceCheckRate.GetWindowText(str);
	m_nSenceCheckRate = (int)strtod(str,&stopstring);

	if (((CFleetOptimizerApp*)AfxGetApp())->m_bOptimizationInProcess == TRUE){
		MessageBox("Оптимизация в процессе выполнения. Изменения не будут сохранены","CCOptimizSettingsDlg::OnOK()",MB_OK);
	}else{
		SaveSelectedOptions();	
	}

	CDialog::OnOK();
}

void COptimizSettingsDlg::OnSensitivityCheck() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	if (m_bUseSensitivityCheck == FALSE){
		GetDlgItem(IDC_SENSE_EDIT1)->EnableWindow(FALSE);
	}
	else
		GetDlgItem(IDC_SENSE_EDIT1)->EnableWindow(TRUE);

}
