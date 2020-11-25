// FleetOptimizer.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "FleetOptimizer.h"

#include "MainFrm.h"
#include "FleetOptimizerDoc.h"
#include "FleetOptimizerView.h"
#include "OutputView.h"
#include "..\MMI\PortDialog.h"
#include "..\MMI\PortDialog1.h"
#include "..\MMI\ScenarioWizardDlg.h"
#include "..\MMI\ShipTypePropertySheet.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFleetOptimizerApp

BEGIN_MESSAGE_MAP(CFleetOptimizerApp, CWinApp)
  //  ON_MESSAGE(WM_GOODBYE_FLEET_SHIPWIZARD, OnGoodbyeShipWizard)
	//{{AFX_MSG_MAP(CFleetOptimizerApp)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
	ON_COMMAND(ID_RUN_OPTIMIZER, OnRunOptimizer)
	ON_COMMAND(ID_CREATE_INITIAL_BASE_PLAN, OnCreateInitialBasePlan)
	ON_COMMAND(ID_OPTIMIZATION_ST1LP, OnOptimizationSt1Lp)
	ON_COMMAND(ID_OPTIMIZATION_ST2LPDP, OnOptimizationSt2LpDp)
	ON_COMMAND(ID_OPTIMIZATION_ST3DP, OnOptimizationSt3Dp)
	ON_COMMAND(ID_BUILD_PORT_SHOW, OnBuildPortShowList)
	ON_COMMAND(ID_BUILD_SCENARIO_SCENARIOWIZARD, OnScenarioWizard)
	ON_COMMAND(ID_BUILD_SCENARIOWIZARD_SCENARIODLOAD, OnScenarioWizardLoad)
	ON_COMMAND(BUILD_SHIP_PROTO_ACV, OnShipProtoAcv)
	ON_COMMAND(BUILD_SHIP_PROTO_SHIP, OnShipProtoShip)
	ON_COMMAND(ID_BUILD_SHIPTYPE_NEW, OnBuildShipTypeNew)
	ON_COMMAND(ID_HELP_FINDER, OnHelpFinder)
	ON_COMMAND(ID_BUILD_SHIPWIZARD, OnBuildShipWizard)
	ON_COMMAND(ID_BUILD_PORTWIZARD, OnBuildPortWizard)
	ON_COMMAND(ID_RUN_OPTIMIZER_SETTINGS, OnOptimizerRunSettings)
	ON_COMMAND(ID_RUN_OPTIMIZER_SELECTED, OnRunOptimizerSelected)
	ON_COMMAND(ID_FILE_NEW_SCENARIO, OnFileNewScenario)
	ON_COMMAND(ID_FILE_OPEN, OnFileOpen)
	ON_COMMAND(ID_FILE_MRU_FILE1, OnFileMruFile1)
	ON_COMMAND(ID_FILE_MRU_FILE2, OnFileMruFile2)
	ON_COMMAND(ID_FILE_MRU_FILE3, OnFileMruFile3)
	ON_COMMAND(ID_FILE_MRU_FILE4, OnFileMruFile4)
	ON_COMMAND(ID_VIEW_LOGS_DLG, OnViewLogsDlg)
	ON_COMMAND(ID_VIEW_LOGS_1OPTIMSTEP, OnViewLogsOpenFileOptimizStep1Plan)
	ON_COMMAND(ID_VIEW_LOGS_2OPTIMSTEP, OnViewLogsOpenFileOptimizStep2Plan)
	ON_COMMAND(ID_VIEW_LOGS_3OPTIMSTEP, OnViewLogsOpenFileOptimizStep3Plan)
	ON_COMMAND(ID_VIEW_LOGS_INITBASEPLAN, OnViewLogsOpenFileInitialBasePlan)
	ON_COMMAND(ID_VIEW_LOGS_ERRORS, OnViewLogsOpenFileErrorsLog)
	ON_COMMAND(ID_VIEW_LOGS_INITDATA, OnViewLogsOpenFileInitDBMVehiclesAll)
	ON_COMMAND(ID_VIEW_LOGS_SHIPS, OnViewLogsOpenFileVehiclesList)
	ON_COMMAND(ID_VIEW_EASYSTART_DLG, OnViewEasyStartDlg)
	//}}AFX_MSG_MAP
	// Standard file based document commands
	ON_COMMAND(ID_FILE_NEW, CWinApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, CWinApp::OnFileOpen)
	// Standard print setup command
	//ON_COMMAND(ID_FILE_PRINT_SETUP, CWinApp::OnFilePrintSetup)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFleetOptimizerApp construction

CFleetOptimizerApp::CFleetOptimizerApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
	m_pACVProtoWizDlg = NULL;
	m_pShipProtoWizDlg = NULL;
	m_pShipWizDlg = NULL;
	m_pPortWizDlg = NULL;
	m_pLogsDlg = NULL;
	m_pCurrFleetOptimizerDoc = NULL;
	pDocTemplate = NULL;
	m_bScenLoaded = 0;
	m_bShowEasyStartDlg = 0;
	
	m_bUseOptAll = TRUE;	// Optimization  settings begin
	m_bUseOptSpecific = FALSE;
	m_bUseOptStep1 = FALSE;
	m_bUseOptStep2 = FALSE;
	m_bUseOptStep3 = FALSE;
	m_bUseSensitivityCheck = FALSE;// Optimization  settings end

	m_pShipsDBFile = "..\\DB\\ships_db.fd";
	//CString str = ((CFleetOptimizerApp*)AfxGetApp())->GetProfileString("FleetOptimizer","DbShips");
	//m_pShipsDBFile = _strdup(str.GetBuffer(20));
	//m_sLogFNameError = "E:\\Users\\mv\\Log\\Errors_log.txt";

	m_bOptimizationInProcess = FALSE;
	m_hCreateInitialBasePlanThread = NULL;
	m_nCreateInitialBasePlanThreadId = 0;
	m_hOptimizationSt1LpThread = NULL;
	m_nOptimizationSt1LpThreadId = 0;
	m_hOptimizationSt2LpDpThread = NULL;
	m_nOptimizationSt2LpDpThreadId = 0;
	m_hOptimizationSt3DpThread = NULL;
	m_nOptimizationSt3DpThreadId = 0;
	m_hRunFullOptimizationThread = NULL;
	m_nRunFullOptimizationThreadId = 0;
	m_hRunOptimizerSelectedThread = NULL;
	m_nRunOptimizerSelectedThreadId = 0;

	
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CFleetOptimizerApp object

CFleetOptimizerApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CFleetOptimizerApp initialization

BOOL CFleetOptimizerApp::InitInstance()
{
	AfxEnableControlContainer();

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.

#if _MSC_VER <= 1200 // MFC 6.0 or earlier
#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif
#endif


	// Change the registry key under which our settings are stored.
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization.
	//SetRegistryKey(_T("Local AppWizard-Generated Applications"));
	SetRegistryKey(_T("FleetOptimizer"));


	LoadStdProfileSettings(4);  // Load standard INI file options (including MRU)

	// Register the application's document templates.  Document templates
	//  serve as the connection between documents, frame windows and views.

	//CSingleDocTemplate* pDocTemplate;
	pDocTemplate = new CSingleDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(CFleetOptimizerDoc),
		RUNTIME_CLASS(CMainFrame),       // main SDI frame window
		RUNTIME_CLASS(CFleetOptimizerView));
		//RUNTIME_CLASS(COutputView));
	AddDocTemplate(pDocTemplate);

	// Parse command line for standard shell commands, DDE, file open
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);

	// Dispatch commands specified on the command line
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;

	// The one and only window has been initialized, so show and update it.
	m_pMainWnd->ShowWindow(SW_SHOW);
	m_pMainWnd->UpdateWindow();

	m_pCurrFleetOptimizerDoc = 	(CFleetOptimizerDoc*)pDocTemplate->OpenDocumentFile(NULL);
	m_pCurrFleetOptimizerDoc->SetTitle("");

	ReadDataFromRegistry();
    COptimizer::Instance();
	if(m_bShowEasyStartDlg)
		RunEasyStartDlg();

	return TRUE;
}


/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
		// No message handlers
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
	
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

// App command to run the dialog
void CFleetOptimizerApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

/////////////////////////////////////////////////////////////////////////////
// CFleetOptimizerApp message handlers







void CFleetOptimizerApp::OnRunOptimizer() 
{
	// TODO: Add your command handler code here
    //COptimizer::Instance();
	//CShipFactory::Instance()->CreateShipsInstances();

	if (m_bScenLoaded == 0){
		CString strMessage = "Scenario is not loaded...\nPlease load an existing scenario, or create a new scenario and upload it.";
		MessageBox(NULL,strMessage,"Scenario is not loaded",MB_ICONEXCLAMATION);
		return;

	}

	//OnCreateInitialBasePlan(); //23.10.04
	//OnOptimizationSt1Lp();
	//OnOptimizationSt2LpDp();
	//OnOptimizationSt3Dp();

	if (m_bOptimizationInProcess == FALSE){	//23.10.04
		m_hRunFullOptimizationThread = ::CreateThread(
								(LPSECURITY_ATTRIBUTES)NULL,// no security
								(DWORD)0,                   // same stack size
								(LPTHREAD_START_ROUTINE)RunFullOptimizationFunc, // thread procedure
								(PVOID)this,            // parameter pointer
								(DWORD)0,               // start immediately
								&m_nRunFullOptimizationThreadId  // thread id
								);
		
		if (!m_hRunFullOptimizationThread){
			MessageBox(NULL,"Error: CreateThread","3rd stage of Optimization: CFleetOptimizerApp::OnRunOptimizer()",MB_OK);
			m_bOptimizationInProcess = FALSE;
			return;
		}	
		m_bOptimizationInProcess = TRUE;
	}
	else {
		MessageBox(NULL,"Optimization is already running. Can't start new process.","3-этап оптимизации: CFleetOptimizerApp::OnRunOptimizer()",MB_OK);
	}
}



void CFleetOptimizerApp::OnCreateInitialBasePlan() 
{
	if (m_bScenLoaded == 0){
		CString strMessage = "Scenario is not loaded...\nPlease load an existing scenario, or create a new scenario and upload it.";
		MessageBox(NULL,strMessage,"Scenario is not loaded",MB_ICONEXCLAMATION);
		return;

	}
    
	//CInitialBasePlan::CreateInitialBasePlan();
	if (m_bOptimizationInProcess == FALSE){	//23.10.04
		m_hCreateInitialBasePlanThread = ::CreateThread(
								(LPSECURITY_ATTRIBUTES)NULL,// no security
								(DWORD)0,                   // same stack size
								(LPTHREAD_START_ROUTINE)CreateInitialBasePlanFunc, // thread procedure
								(PVOID)this,            // parameter pointer
								(DWORD)0,               // start immediately
								&m_nCreateInitialBasePlanThreadId  // thread id
								);
		
		if (!m_hCreateInitialBasePlanThread){
			MessageBox(NULL,"Error: CreateThread","3rd stage of Optimization: CFleetOptimizerApp::OnCreateInitialBasePlan()",MB_OK);
			m_bOptimizationInProcess = FALSE;
			return;
		}	
		m_bOptimizationInProcess = TRUE;
	}
	else {
		MessageBox(NULL,"Optimization is already running. Can't start new process.","3-этап оптимизации: CFleetOptimizerApp::OnCreateInitialBasePlan()",MB_OK);
	}

	
}


void CFleetOptimizerApp::OnOptimizationSt1Lp() 
{
	if (m_bScenLoaded == 0){
		CString strMessage = "Scenario is not loaded...\nPlease load an existing scenario, or create a new scenario and upload it.";
		MessageBox(NULL,strMessage,"Scenario is not loaded",MB_ICONEXCLAMATION);
		return;

	}
	//COptimizationSt1LP::Instance()->SimplexMethod(); //23.10.04 
	if (m_bOptimizationInProcess == FALSE){	//23.10.04

		m_hOptimizationSt1LpThread = ::CreateThread(
								(LPSECURITY_ATTRIBUTES)NULL,// no security
								(DWORD)0,                   // same stack size
								(LPTHREAD_START_ROUTINE)OptimizationSt1LpFunc, // thread procedure
								(PVOID)this,            // parameter pointer
								(DWORD)0,               // start immediately
								&m_nOptimizationSt1LpThreadId  // thread id
								);
		
		if (!m_hOptimizationSt1LpThread){
			MessageBox(NULL,"Error: CreateThread","3rd stage of Optimization: CFleetOptimizerApp::OptimizationSt1Lp()",MB_OK);
			m_bOptimizationInProcess = FALSE;
			return;
		}	
		m_bOptimizationInProcess = TRUE;
	}
	else {
		MessageBox(NULL,"Optimization is already running. Can't start new process.","3-этап оптимизации: CFleetOptimizerApp::OptimizationSt1Lp()",MB_OK);
	}

}

void CFleetOptimizerApp::OnOptimizationSt2LpDp() 
{
	if (m_bScenLoaded == 0){
		CString strMessage = "Scenario is not loaded...\nPlease load an existing scenario, or create a new scenario and upload it.";
		MessageBox(NULL,strMessage,"Scenario is not loaded",MB_ICONEXCLAMATION);
		return;

	}
	//COptimizationSt2LpDp::Instance()->SimplexMethod(); //23.10.04
	if (m_bOptimizationInProcess == FALSE){	//23.10.04
		m_hOptimizationSt2LpDpThread = ::CreateThread(
								(LPSECURITY_ATTRIBUTES)NULL,// no security
								(DWORD)0,                   // same stack size
								(LPTHREAD_START_ROUTINE)OptimizationSt2LpDpFunc, // thread procedure
								(PVOID)this,            // parameter pointer
								(DWORD)0,               // start immediately
								&m_nOptimizationSt2LpDpThreadId  // thread id
								);
		
		if (!m_hOptimizationSt2LpDpThread){
			MessageBox(NULL,"Error: CreateThread","3rd stage of Optimization: CFleetOptimizerApp::OnOptimizationSt2LpDp()",MB_OK);
			m_bOptimizationInProcess = FALSE;
			return;
		}	
		m_bOptimizationInProcess = TRUE;
	}
	else {
		MessageBox(NULL,"Optimization is already running. Can't start new process.","3-этап оптимизации: CFleetOptimizerApp::OnOptimizationSt2LpDp()",MB_OK);
	}

}

void CFleetOptimizerApp::OnOptimizationSt3Dp() 
{
	if (m_bScenLoaded == 0){
		CString strMessage = "Scenario is not loaded...\nPlease load an existing scenario, or create a new scenario and upload it.";
		MessageBox(NULL,strMessage,"Scenario is not loaded",MB_ICONEXCLAMATION);
		return;
	}
	//COptimizationSt3Dp::Instance()->OptimisationDynProgMethod();
	if (m_bOptimizationInProcess == FALSE){	//23.10.04

		m_hOptimizationSt3DpThread = ::CreateThread(
								(LPSECURITY_ATTRIBUTES)NULL,// no security
								(DWORD)0,                   // same stack size
								(LPTHREAD_START_ROUTINE)OptimizationSt3DpFunc, // thread procedure
								(PVOID)this,            // parameter pointer
								(DWORD)0,               // start immediately
								&m_nOptimizationSt3DpThreadId  // thread id
								);
		
		if (!m_hOptimizationSt3DpThread){
			MessageBox(NULL,"Error: CreateThread","3rd stage of Optimization: CFleetOptimizerApp::OnOptimizationSt3Dp()",MB_OK);
			m_bOptimizationInProcess = FALSE;
			return;
		}	
		m_bOptimizationInProcess = TRUE;
	}
	else {
		MessageBox(NULL,"Optimization is already running. Can't start new process.","3rd stage of Optimization: CFleetOptimizerApp::OnOptimizationSt3Dp()",MB_OK);
	}
}

void CFleetOptimizerApp::OnBuildPortShowList() 
{
	CPortDialog1 dlg;
    int ret = dlg.DoModal();
}

void CFleetOptimizerApp::OnScenarioWizard() 
{
	CScenarioWizardDlg dlg;
	CString str; 
	CString TitleString = "";
    //int ret = dlg.DoModal();
	if (dlg.DoModal() == IDOK){
		m_pScenarioFile = _strdup(dlg.m_pScenarioFile);
		str = m_pScenarioFile;
		((CMainFrame*)AfxGetMainWnd())->UpdateScenarioFile(str);
		if (str != ""){
			LoadScenario();
			TitleString.Format(" Scenario: %s",m_pScenarioFile);
			m_pCurrFleetOptimizerDoc->SetTitle(TitleString);
			m_bScenLoaded = 1;
		}
		else{
			m_pCurrFleetOptimizerDoc->SetTitle("");
			m_bScenLoaded = 0;
		}
	}

}

void CFleetOptimizerApp::OnShipProtoAcv() 
{
	// TODO: Add your command handler code here
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

void CFleetOptimizerApp::OnShipProtoShip() 
{
	// TODO: Add your command handler code here
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

void CFleetOptimizerApp::OnBuildShipTypeNew() 
{

	// clearing records number in Ships DB 
	ULONGLONG positionEnd, position;
	CString	strNum;
//	char *stopstring;
	int RecordsNumber;
	
	CFile f( m_pShipsDBFile, CFile::modeRead );
	f.SeekToBegin();
	CArchive ar(&f, CArchive::load);
	
	CFile* fp = ar.GetFile();
	fp->SeekToEnd();
	positionEnd = fp->GetPosition();
	fp->SeekToBegin();
	position = fp->GetPosition();

	if (positionEnd == 0)
		RecordsNumber = 0;
	else {
			ar >> strNum;
			char *stopstring;
			RecordsNumber  = (int)strtod(strNum,&stopstring);
	}
	ar.Close();
	f.Close();

	//Open "New Ship Type Creation" window:
	CShipTypePropertySheet thePSheet(_T("New Ship Type Creation"));
	thePSheet.m_CShipTypeMainPPage.m_ISModifiable = TRUE;
	thePSheet.m_CShipTypeOptimizParamPPage.m_ISModifiable = TRUE;
	thePSheet.m_OpenStat = 3;
	thePSheet.m_CShipTypeMainPPage.m_OpenStat = 3;
	thePSheet.m_CShipTypeOptimizParamPPage.m_OpenStat = 3;
	thePSheet.m_RecordsNumber = RecordsNumber;
	thePSheet.m_pShipsDBFile = m_pShipsDBFile;
	thePSheet.DoModal();		
}

void CFleetOptimizerApp::OnHelpFinder() 
{
    AfxGetApp()->WinHelp(0L, HELP_FINDER);
}

CFleetOptimizerDoc* CFleetOptimizerApp::GetCurrFleetOptimizerDoc()
{
	return m_pCurrFleetOptimizerDoc;
}

void CFleetOptimizerApp::OnBuildShipWizard() 
{
	if(!m_pShipWizDlg)
	{
		m_pShipWizDlg  = new CShipTypeWizardDlg();
	}
	else
	{
		m_pShipWizDlg->ShowWindow(SW_RESTORE);
	}
	m_pShipWizDlg->ShowWindow(SW_SHOW);
}

void CFleetOptimizerApp::OnBuildPortWizard() 
{
	if(!m_pPortWizDlg)
	{
		m_pPortWizDlg  = new CPortWizardDlg();
	}
	else
	{
		m_pPortWizDlg->ShowWindow(SW_RESTORE);
	}
	m_pPortWizDlg->ShowWindow(SW_SHOW);
	
}

long CFleetOptimizerApp::OnGoodbyeShipWizard(UINT wParam, LONG lParam)
{
    // message received in response to modeless dialog OK
    //  and Cancel buttons
    /*TRACE("CFleetOptimizerApp::OnGoodbye %x, %lx\n", wParam, lParam);
    TRACE("Dialog edit1 contents = %s\n",
         (const char*) m_pDlg->m_strEdit1);
		 */
    m_pShipWizDlg->DestroyWindow();
    return 0L;
}

CString CFleetOptimizerApp::GetLogFNameError()
{
	//return m_sLogFNameError;
	CString str = m_pLogFileErrors;
	return str;
}

void CFleetOptimizerApp::OnOptimizerRunSettings() 
{
	GetOptimizSelectedOptions();
	COptimizSettingsDlg OptimizerSettingsDlg;
	OptimizerSettingsDlg.m_bUseOptAll = m_bUseOptAll;
	OptimizerSettingsDlg.m_bUseOptStep1 = m_bUseOptStep1;
	OptimizerSettingsDlg.m_bUseOptStep2 = m_bUseOptStep2;
	OptimizerSettingsDlg.m_bUseOptStep3 = m_bUseOptStep3;
	OptimizerSettingsDlg.m_bUseSensitivityCheck = m_bUseSensitivityCheck;
	OptimizerSettingsDlg.m_nSenceCheckRate = m_nSenceCheckRate;
	OptimizerSettingsDlg.DoModal();
	GetOptimizSelectedOptions();
}

void CFleetOptimizerApp::OnRunOptimizerSelected() 
{
	if (m_bScenLoaded == 0){
		CString strMessage = "Scenario is not loaded...\nPlease load an existing scenario, or create a new scenario and upload it.";
		MessageBox(NULL,strMessage,"Scenario is not loaded",MB_ICONEXCLAMATION);
		return;

	}
	
	GetOptimizSelectedOptions();
	if (m_bUseOptAll){
		OnRunOptimizer();	// Do All
	}
	else {
		//OnCreateInitialBasePlan(); 	//23.10.04
		//if (m_bUseOptStep1)
		//	OnOptimizationSt1Lp();
		//if (m_bUseOptStep2)
		//	OnOptimizationSt2LpDp();
		//if (m_bUseOptStep3)
		//	OnOptimizationSt3Dp();

		if (m_bOptimizationInProcess == FALSE){	//23.10.04
			m_hRunOptimizerSelectedThread = ::CreateThread(
									(LPSECURITY_ATTRIBUTES)NULL,// no security
									(DWORD)0,                   // same stack size
									(LPTHREAD_START_ROUTINE)RunOptimizerSelectedFunc, // thread procedure
									(PVOID)this,            // parameter pointer
									(DWORD)0,               // start immediately
									&m_nRunOptimizerSelectedThreadId  // thread id
									);
			
			if (!m_hRunOptimizerSelectedThread){
				MessageBox(NULL,"Error: CreateThread","3rd stage of Optimization: CFleetOptimizerApp::OnRunOptimizerSelected()",MB_OK);
				m_bOptimizationInProcess = FALSE;
				return;
			}	
			m_bOptimizationInProcess = TRUE;
		}
		else {
			MessageBox(NULL,"Optimization is already running. Can't start new process.","3rd stage of Optimization: CFleetOptimizerApp::OnRunOptimizerSelected()",MB_OK);
		}


	}
}

void CFleetOptimizerApp::GetOptimizSelectedOptions()
{
	m_bUseOptAll = 	((CFleetOptimizerApp*)AfxGetApp())->GetProfileInt("FleetOptimizer","UseOptAll",0);	
	m_bUseOptStep1 = ((CFleetOptimizerApp*)AfxGetApp())->GetProfileInt("FleetOptimizer","UseOptStep1",0);	
	m_bUseOptStep2 = ((CFleetOptimizerApp*)AfxGetApp())->GetProfileInt("FleetOptimizer","UseOptStep2",0);	
	m_bUseOptStep3 = ((CFleetOptimizerApp*)AfxGetApp())->GetProfileInt("FleetOptimizer","UseOptStep3",0);
	m_bUseSensitivityCheck = ((CFleetOptimizerApp*)AfxGetApp())->GetProfileInt("FleetOptimizer","UseSenseCheck",0);
	m_nSenceCheckRate = ((CFleetOptimizerApp*)AfxGetApp())->GetProfileInt("FleetOptimizer","SenseCheckRate",0);
}

void CFleetOptimizerApp::LoadScenario()
{
	CShipFactory::Instance()->CreateShipsInstances();
	CPortFactory::Instance()->CreatePortsInstances();

    CMainFrame* pFrame = (CMainFrame*) AfxGetApp()->m_pMainWnd;
    CStatusBar* pStatus = &pFrame->m_wndStatusBar;
    if (pStatus) {
      pStatus->SetPaneText(0, "Scenario loaded");
    }
	CString str = m_pScenarioFile;
	((CMainFrame*)AfxGetMainWnd())->UpdateScenarioFile(str);

}


void CFleetOptimizerApp::OnScenarioWizardLoad() 
{
	CScenarioWizardDlg dlg;
	CString str; 
	CString TitleString = "";
	dlg.m_bAutoLoad = 1;
	
	CFileDialog fileDialog(TRUE,NULL,NULL,NULL,
						"Fleet Optimiser Scenario Files (*.fsc)|*.fsc|All Files (*.*)|*.*|");											 
	if(fileDialog.DoModal()==IDOK)
	{		
		CString extension = fileDialog.GetFileExt();
		if(extension != "fsc")
			AfxMessageBox("Unknown file format\nPlease select *.fcs file.");
		else
		{
			CString filename = fileDialog.GetPathName();
			m_pScenarioFile = strdup(filename.GetBuffer(20)); 
		}

		((CFleetOptimizerApp*)AfxGetApp())->WriteProfileString("FleetOptimizer","ScenarioFile", m_pScenarioFile);	

		if (dlg.DoModal() == IDOK){
			m_pScenarioFile = _strdup(dlg.m_pScenarioFile);
			str = m_pScenarioFile;
			((CMainFrame*)AfxGetMainWnd())->UpdateScenarioFile(str);
			if (str != ""){
				LoadScenario();
				TitleString.Format(" Scenario: %s",m_pScenarioFile);
				m_pCurrFleetOptimizerDoc->SetTitle(TitleString);
				AddToRecentFileList(str);
				m_bScenLoaded = 1;
			}
			else{
				m_pCurrFleetOptimizerDoc->SetTitle("");
				m_bScenLoaded = 0;
			}
		}
	}
}

void CFleetOptimizerApp::OnFileNewScenario() 
{
	// TODO: Add your command handler code here
	OnScenarioWizard();	
}

void CFleetOptimizerApp::OnFileOpen() 
{
	OnScenarioWizardLoad();
}

void CFleetOptimizerApp::AddToRecentFileList(LPCTSTR lpszPathName) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	CWinApp::AddToRecentFileList(lpszPathName);
}

void CFleetOptimizerApp::ScenarioWizardLoad()
{
	CScenarioWizardDlg dlg;
	CString str; 
	CString TitleString = "";
	dlg.m_bAutoLoad = 1;

	if (dlg.DoModal() == IDOK){
		m_pScenarioFile = _strdup(dlg.m_pScenarioFile);
		str = m_pScenarioFile;
		((CMainFrame*)AfxGetMainWnd())->UpdateScenarioFile(str);
		if (str != ""){
			LoadScenario();
			TitleString.Format(" Scenario: %s",m_pScenarioFile);
			m_pCurrFleetOptimizerDoc->SetTitle(TitleString);
			AddToRecentFileList(str);
			m_bScenLoaded = 1;
		}
		else{
			m_pCurrFleetOptimizerDoc->SetTitle("");
			m_bScenLoaded = 0;
		}
	}


}

void CFleetOptimizerApp::OnFileMruFile1() 
{
	CString str = 	((CFleetOptimizerApp*)AfxGetApp())->GetProfileString("Recent File List","File1");
	m_pScenarioFile = strdup(str.GetBuffer(20));
	((CFleetOptimizerApp*)AfxGetApp())->WriteProfileString("FleetOptimizer","ScenarioFile", m_pScenarioFile);

	ScenarioWizardLoad();
}

void CFleetOptimizerApp::OnFileMruFile2() 
{
	CString str = 	((CFleetOptimizerApp*)AfxGetApp())->GetProfileString("Recent File List","File2");
	m_pScenarioFile = strdup(str.GetBuffer(20));
    ((CFleetOptimizerApp*)AfxGetApp())->WriteProfileString("FleetOptimizer","ScenarioFile", m_pScenarioFile);	

	ScenarioWizardLoad();
}

void CFleetOptimizerApp::OnFileMruFile3() 
{
	CString str = 	((CFleetOptimizerApp*)AfxGetApp())->GetProfileString("Recent File List","File3");
	m_pScenarioFile = strdup(str.GetBuffer(20));
    ((CFleetOptimizerApp*)AfxGetApp())->WriteProfileString("FleetOptimizer","ScenarioFile", m_pScenarioFile);	

	ScenarioWizardLoad();
}

void CFleetOptimizerApp::OnFileMruFile4() 
{
	CString str = 	((CFleetOptimizerApp*)AfxGetApp())->GetProfileString("Recent File List","File4");
	m_pScenarioFile = strdup(str.GetBuffer(20));
    ((CFleetOptimizerApp*)AfxGetApp())->WriteProfileString("FleetOptimizer","ScenarioFile", m_pScenarioFile);	

	ScenarioWizardLoad();
}

void CFleetOptimizerApp::OnViewLogsDlg() 
{
	// TODO: Add your command handler code here
	if(!m_pLogsDlg)
	{
		m_pLogsDlg  = new CLogsDlg();
	}
	else
	{
		m_pLogsDlg->ShowWindow(SW_RESTORE);
	}
	m_pLogsDlg->ShowWindow(SW_SHOW);		
}

void CFleetOptimizerApp::OnViewLogsOpenFileVehiclesList() 
{
	CString command = "notepad ";
	command += m_pLogFileVehiclesList;
	WinExec(command , SW_SHOW);	
}

void CFleetOptimizerApp::OnViewLogsOpenFileInitDBMVehiclesAll() 
{
	CString command = "notepad ";
	command += m_pLogFileInitDBMVehiclesAll;
	WinExec(command , SW_SHOW);	
}

void CFleetOptimizerApp::OnViewLogsOpenFileErrorsLog() 
{
	CString command = "notepad ";
	command += m_pLogFileErrors;
	WinExec(command , SW_SHOW);		
}

void CFleetOptimizerApp::OnViewLogsOpenFileInitialBasePlan() 
{
	CString command = "notepad ";
	command += m_pLogFileInitialBasePlan;
	WinExec(command , SW_SHOW);			
}

void CFleetOptimizerApp::OnViewLogsOpenFileOptimizStep1Plan() 
{
	CString command = "notepad ";
	command += m_pLogFileOptimizStep1Plan;
	WinExec(command , SW_SHOW);			
}

void CFleetOptimizerApp::OnViewLogsOpenFileOptimizStep2Plan() 
{
	CString command = "notepad ";
	command += m_pLogFileOptimizStep2Plan;
	WinExec(command , SW_SHOW);			
}

void CFleetOptimizerApp::OnViewLogsOpenFileOptimizStep3Plan() 
{
	CString command = "notepad ";
	command += m_pLogFileOptimizStep3Plan;
	WinExec(command , SW_SHOW);			
}


void CFleetOptimizerApp::ReadDataFromRegistry()
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

	str = 	((CFleetOptimizerApp*)AfxGetApp())->GetProfileString("FleetOptimizer","LogFileVehiclesDetails");	
	m_pLogFileVehiclesDetails = _strdup(str.GetBuffer(20));

	BOOL bHideEasyStartDlg = 	((CFleetOptimizerApp*)AfxGetApp())->GetProfileInt("FleetOptimizer","HideEasyStartDlg",0);	
	if(!bHideEasyStartDlg)
		m_bShowEasyStartDlg = 1;
	else
		m_bShowEasyStartDlg = 0;

}

void CFleetOptimizerApp::RunEasyStartDlg()
{
	if (!m_pEasyStartDlg)
	{
		m_pEasyStartDlg  = new CEasyStartDlg();
	}
	else
	{
		m_pEasyStartDlg->ShowWindow(SW_RESTORE);
	}
	m_pEasyStartDlg->ShowWindow(SW_SHOW);
}

void CFleetOptimizerApp::OnViewEasyStartDlg() 
{
	RunEasyStartDlg();
}



DWORD WINAPI CFleetOptimizerApp::CreateInitialBasePlanFunc(void* thisPointer)
{
	if (!thisPointer)
		return 1;
	int res = CInitialBasePlan::CreateInitialBasePlan();
	((CFleetOptimizerApp*)thisPointer)->m_bOptimizationInProcess = FALSE;
	if (res ==1)
		MessageBox(NULL,"Done","Initial Base plan of Optimization. CreateInitialBasePlanFunc()",MB_OK);
	return 1;
}

DWORD WINAPI CFleetOptimizerApp::OptimizationSt1LpFunc(void* thisPointer)
{
	if (!thisPointer)
		return 1;
	int res = COptimizationSt1LP::Instance()->SimplexMethod();
	((CFleetOptimizerApp*)thisPointer)->m_bOptimizationInProcess = FALSE;	
	MessageBox(NULL,"Done","1st stage of Optimization. OptimizationSt1LpFunc()",MB_OK);
	return res;
}

DWORD WINAPI CFleetOptimizerApp::OptimizationSt2LpDpFunc(void* thisPointer)
{
	if (!thisPointer)
		return 1;
	int res = COptimizationSt2LpDp::Instance()->SimplexMethod();
	((CFleetOptimizerApp*)thisPointer)->m_bOptimizationInProcess = FALSE;	
	MessageBox(NULL,"Done","2nd stage of Optimization. OptimizationSt2LpDpFunc()",MB_OK);
	return res;
}


DWORD WINAPI CFleetOptimizerApp::OptimizationSt3DpFunc(void* thisPointer)
{
	if (!thisPointer)
		return 1;
	int res = COptimizationSt3Dp::Instance()->OptimisationDynProgMethod();
	((CFleetOptimizerApp*)thisPointer)->m_bOptimizationInProcess = FALSE;	
	MessageBox(NULL,"Done","3rd stage of Optimization. OptimizationSt3DpFunc()",MB_OK);
	return res;
}

DWORD WINAPI CFleetOptimizerApp::RunFullOptimizationFunc(void* thisPointer)
{
	if (!thisPointer)
		return 1;

	CInitialBasePlan::CreateInitialBasePlan();
	int res = COptimizationSt1LP::Instance()->SimplexMethod();
	res = COptimizationSt2LpDp::Instance()->SimplexMethod();
	res = COptimizationSt3Dp::Instance()->OptimisationDynProgMethod();

	((CFleetOptimizerApp*)thisPointer)->m_bOptimizationInProcess = FALSE;	

	MessageBox(NULL,"Done","Full Optimization (All 3 stages). RunFullOptimizationFunc()",MB_OK);

	return res;
}

DWORD WINAPI CFleetOptimizerApp::RunOptimizerSelectedFunc(void* thisPointer)
{
	int res = 0;
	if (!thisPointer)
		return 1;

	CInitialBasePlan::CreateInitialBasePlan();
	if (((CFleetOptimizerApp*)thisPointer)->m_bUseOptStep1)
		res = COptimizationSt1LP::Instance()->SimplexMethod();
	if (((CFleetOptimizerApp*)thisPointer)->m_bUseOptStep2)
		res = COptimizationSt2LpDp::Instance()->SimplexMethod();
	if (((CFleetOptimizerApp*)thisPointer)->m_bUseOptStep3)
		res = COptimizationSt3Dp::Instance()->OptimisationDynProgMethod();

	((CFleetOptimizerApp*)thisPointer)->m_bOptimizationInProcess = FALSE;	

	MessageBox(NULL,"Done","Selected stages of Optimization. RunFullOptimizationFunc()",MB_OK);

	return res;
}
