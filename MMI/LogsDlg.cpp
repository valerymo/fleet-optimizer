// LogsDlg.cpp : implementation file
//

#include "stdafx.h"
//#include "mmi.h"
#include "LogsDlg.h"
#include "..\FleetOptimizer\FleetOptimizer.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CLogsDlg dialog


CLogsDlg::CLogsDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CLogsDlg::IDD, pParent)
{
	EnableAutomation();

	//{{AFX_DATA_INIT(CLogsDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_nID = CLogsDlg::IDD;
	CDialog::Create(m_nID, pParent);
	
	ReadDataFromRegistry();
}


void CLogsDlg::OnFinalRelease()
{
	// When the last reference for an automation object is released
	// OnFinalRelease is called.  The base class will automatically
	// deletes the object.  Add additional cleanup required for your
	// object before calling the base class.

	CDialog::OnFinalRelease();
}

void CLogsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CLogsDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CLogsDlg, CDialog)
	//{{AFX_MSG_MAP(CLogsDlg)
	ON_BN_CLICKED(IDCLOSE, OnClose)
	ON_BN_CLICKED(IDC_BUTTON1, OnOpenFileVehiclesList)
	ON_BN_CLICKED(IDC_BUTTON6, OnOpenFileInitDBMVehiclesAll)
	ON_BN_CLICKED(IDC_BUTTON7, OnOpenFileErrorsLog)
	ON_BN_CLICKED(IDC_BUTTON2, OnOpenFileInitialBasePlan)
	ON_BN_CLICKED(IDC_BUTTON3, OnOpenFileOptimizStep1Plan)
	ON_BN_CLICKED(IDC_BUTTON4, OnOpenFileOptimizStep2Plan)
	ON_BN_CLICKED(IDC_BUTTON5, OnOpenFileOptimizStep3Plan)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BEGIN_DISPATCH_MAP(CLogsDlg, CDialog)
	//{{AFX_DISPATCH_MAP(CLogsDlg)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_DISPATCH_MAP
END_DISPATCH_MAP()

// Note: we add support for IID_ILogsDlg to support typesafe binding
//  from VBA.  This IID must match the GUID that is attached to the 
//  dispinterface in the .ODL file.

// {2C0DA68A-92DE-45E3-AE3E-5DFEEA1BA71E}
static const IID IID_ILogsDlg =
{ 0x2c0da68a, 0x92de, 0x45e3, { 0xae, 0x3e, 0x5d, 0xfe, 0xea, 0x1b, 0xa7, 0x1e } };

BEGIN_INTERFACE_MAP(CLogsDlg, CDialog)
	INTERFACE_PART(CLogsDlg, IID_ILogsDlg, Dispatch)
END_INTERFACE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLogsDlg message handlers

void CLogsDlg::OnClose() 
{
	OnOK();	
}

void CLogsDlg::OnOpenFileVehiclesList() 
{
	CString command = "notepad ";
	command += m_pLogFileVehiclesList;
	WinExec(command , SW_SHOW);
}

void CLogsDlg::OnOpenFileInitDBMVehiclesAll() 
{
	CString command = "notepad ";
	command += m_pLogFileInitDBMVehiclesAll;
	WinExec(command , SW_SHOW);	
}

void CLogsDlg::OnOpenFileErrorsLog() 
{
	CString command = "notepad ";
	command += m_pLogFileErrors;
	WinExec(command , SW_SHOW);		
}


void CLogsDlg::OnOpenFileInitialBasePlan() 
{
	CString command = "notepad ";
	command += m_pLogFileInitialBasePlan;
	WinExec(command , SW_SHOW);			
}

void CLogsDlg::OnOpenFileOptimizStep1Plan() 
{
	CString command = "notepad ";
	command += m_pLogFileOptimizStep1Plan;
	WinExec(command , SW_SHOW);			
}

void CLogsDlg::OnOpenFileOptimizStep2Plan() 
{
	CString command = "notepad ";
	command += m_pLogFileOptimizStep2Plan;
	WinExec(command , SW_SHOW);			
}

void CLogsDlg::OnOpenFileOptimizStep3Plan() 
{
	CString command = "notepad ";
	command += m_pLogFileOptimizStep3Plan;
	WinExec(command , SW_SHOW);			
}

void CLogsDlg::ReadDataFromRegistry()
{
	CString str;

	str = 	((CFleetOptimizerApp*)AfxGetApp())->GetProfileString("FleetOptimizer","LogFileVehiclesList");	
	m_pLogFileVehiclesList = _strdup(str.GetBuffer(20));

	str = 	((CFleetOptimizerApp*)AfxGetApp())->GetProfileString("FleetOptimizer","LogFileInitDBMVehiclesAll");	
	m_pLogFileInitDBMVehiclesAll = _strdup(str.GetBuffer(20));

	str = 	((CFleetOptimizerApp*)AfxGetApp())->GetProfileString("FleetOptimizer","LogFileErrors");	
	m_pLogFileErrors = _strdup(str.GetBuffer(20));

	str = 	((CFleetOptimizerApp*)AfxGetApp())->GetProfileString("FleetOptimizer","LogFileInitialBasePlan");	
	m_pLogFileInitialBasePlan = _strdup(str.GetBuffer(20));

	str = 	((CFleetOptimizerApp*)AfxGetApp())->GetProfileString("FleetOptimizer","LogFileOptimizStep1Plan");	
	m_pLogFileOptimizStep1Plan = _strdup(str.GetBuffer(20));

	str = 	((CFleetOptimizerApp*)AfxGetApp())->GetProfileString("FleetOptimizer","LogFileOptimizStep2Plan");	
	m_pLogFileOptimizStep2Plan = _strdup(str.GetBuffer(20));

	str = 	((CFleetOptimizerApp*)AfxGetApp())->GetProfileString("FleetOptimizer","LogFileOptimizStep3Plan");	
	m_pLogFileOptimizStep3Plan = _strdup(str.GetBuffer(20));

}
