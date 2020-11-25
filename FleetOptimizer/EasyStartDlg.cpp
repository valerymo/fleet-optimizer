// EasyStartDlg.cpp : implementation file
//

#include "stdafx.h"
#include "fleetoptimizer.h"
#include "EasyStartDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CEasyStartDlg dialog


CEasyStartDlg::CEasyStartDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CEasyStartDlg::IDD, pParent)
{
	EnableAutomation();

	//{{AFX_DATA_INIT(CEasyStartDlg)
	m_bShowDlg = TRUE;
	//}}AFX_DATA_INIT
	
	m_nID = CEasyStartDlg::IDD;
	CDialog::Create(m_nID, pParent);

	ReadFromRegistry();
	m_ctrlShowDlgCheck.SetCheck(m_bShowDlg);
}


void CEasyStartDlg::OnFinalRelease()
{
	// When the last reference for an automation object is released
	// OnFinalRelease is called.  The base class will automatically
	// deletes the object.  Add additional cleanup required for your
	// object before calling the base class.

	CDialog::OnFinalRelease();
}

void CEasyStartDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CEasyStartDlg)
	DDX_Control(pDX, IDC_CHECK1, m_ctrlShowDlgCheck);
	DDX_Check(pDX, IDC_CHECK1, m_bShowDlg);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CEasyStartDlg, CDialog)
	//{{AFX_MSG_MAP(CEasyStartDlg)
	ON_BN_CLICKED(IDC_ES_CREATE_SCENARIO_BUTTON, OnCreateScenarioButton)
	ON_BN_CLICKED(IDC_ES_LOAD_SCENARIO_BUTTON, OnLoadScenarioButton)
	ON_BN_CLICKED(IDC_ES_RUN_OPTIMIZ_BUTTON, OnRunOptimizButton)
	ON_BN_CLICKED(IDC_ES_SHOW_RESULTS_BUTTON, OnShowResultsButton)
	ON_BN_CLICKED(IDC_ES_HELP_BUTTON, OnHelpButton)
	ON_BN_CLICKED(IDCLOSE, OnClose)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BEGIN_DISPATCH_MAP(CEasyStartDlg, CDialog)
	//{{AFX_DISPATCH_MAP(CEasyStartDlg)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_DISPATCH_MAP
END_DISPATCH_MAP()

// Note: we add support for IID_IEasyStartDlg to support typesafe binding
//  from VBA.  This IID must match the GUID that is attached to the 
//  dispinterface in the .ODL file.

// {73D37E83-0993-49F7-9EEB-B918787A2D1C}
static const IID IID_IEasyStartDlg =
{ 0x73d37e83, 0x993, 0x49f7, { 0x9e, 0xeb, 0xb9, 0x18, 0x78, 0x7a, 0x2d, 0x1c } };

BEGIN_INTERFACE_MAP(CEasyStartDlg, CDialog)
	INTERFACE_PART(CEasyStartDlg, IID_IEasyStartDlg, Dispatch)
END_INTERFACE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEasyStartDlg message handlers

void CEasyStartDlg::OnCreateScenarioButton() 
{
	((CFleetOptimizerApp*)AfxGetApp())->OnScenarioWizard();
}

void CEasyStartDlg::OnLoadScenarioButton() 
{
	((CFleetOptimizerApp*)AfxGetApp())->OnScenarioWizardLoad();
}

void CEasyStartDlg::OnRunOptimizButton() 
{
	((CFleetOptimizerApp*)AfxGetApp())->OnRunOptimizer();
}

void CEasyStartDlg::OnShowResultsButton() 
{
	((CFleetOptimizerApp*)AfxGetApp())->OnViewLogsDlg();
}

void CEasyStartDlg::OnHelpButton() 
{
	// TODO: Add your control notification handler code here
    ((CFleetOptimizerApp*)AfxGetApp())->WinHelp(0L, HELP_FINDER);
}

void CEasyStartDlg::OnClose() 
{
	m_bShowDlg = m_ctrlShowDlgCheck.GetCheck();
	SaveToRegistry();

	CDialog::OnOK();	
}

void CEasyStartDlg::ReadFromRegistry()
{
	BOOL bHideEasyStartDlg = 	((CFleetOptimizerApp*)AfxGetApp())->GetProfileInt("FleetOptimizer","HideEasyStartDlg",0);	
	if(!bHideEasyStartDlg)
		m_bShowDlg = 1;
	else
		m_bShowDlg = 0;
}

void CEasyStartDlg::SaveToRegistry()
{
	BOOL bHideEasyStartDlg = 1;

	if(m_bShowDlg == TRUE)
		bHideEasyStartDlg = 0;
	((CFleetOptimizerApp*)AfxGetApp())->WriteProfileInt("FleetOptimizer","HideEasyStartDlg",bHideEasyStartDlg);	
}
