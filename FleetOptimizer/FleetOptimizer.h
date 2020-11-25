// FleetOptimizer.h : main header file for the FLEETOPTIMIZER application
//

#if !defined(AFX_FLEETOPTIMIZER_H__6AF9FBA0_DD03_4E07_9615_49FD0E5E9458__INCLUDED_)
#define AFX_FLEETOPTIMIZER_H__6AF9FBA0_DD03_4E07_9615_49FD0E5E9458__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols
#include "Optimizer.h"
#include "FleetOptimizerDoc.h"
#include "..\MMI\PrototypeWizardShipDlg.h"
#include "..\MMI\PrototypeWizardACVDlg.h"
#include "..\MMI\ShipTypeWizardDlg.h"
#include "..\MMI\PortWizardDlg.h"
#include "..\MMI\OptimizSettingsDlg.h"
#include "..\MMI\LogsDlg.h"
#include "EasyStartDlg.h"

#define WM_GOODBYE_FLEET_SHIPWIZARD WM_USER + 7

/////////////////////////////////////////////////////////////////////////////
// CFleetOptimizerApp:
// See FleetOptimizer.cpp for the implementation of this class
//

class CFleetOptimizerApp : public CWinApp
{
public:
	CString GetLogFNameError();
	CFleetOptimizerApp();
	CFleetOptimizerDoc* GetCurrFleetOptimizerDoc();

	CSingleDocTemplate* pDocTemplate;
	char* m_pScenarioFile;


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFleetOptimizerApp)
	public:
	virtual BOOL InitInstance();
	virtual void AddToRecentFileList(LPCTSTR lpszPathName);
	//}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(CFleetOptimizerApp)
	afx_msg void OnAppAbout();
	afx_msg void OnRunOptimizer();
	afx_msg void OnCreateInitialBasePlan();
	afx_msg void OnOptimizationSt1Lp();
	afx_msg void OnOptimizationSt2LpDp();
	afx_msg void OnOptimizationSt3Dp();
	afx_msg void OnBuildPortShowList();
	afx_msg void OnScenarioWizard();
	afx_msg void OnScenarioWizardLoad();
	afx_msg void OnShipProtoAcv();
	afx_msg void OnShipProtoShip();
	afx_msg void OnBuildShipTypeNew();
	afx_msg void OnHelpFinder();
	afx_msg void OnBuildShipWizard();
	afx_msg void OnBuildPortWizard();
	afx_msg void OnOptimizerRunSettings();
	afx_msg void OnRunOptimizerSelected();
	afx_msg void OnFileNew();
	afx_msg void OnFileNewScenario();
	afx_msg void OnFileOpen();
	afx_msg void OnFileMruFile1();
	afx_msg void OnFileMruFile2();
	afx_msg void OnFileMruFile3();
	afx_msg void OnFileMruFile4();
	afx_msg void OnViewLogsDlg();
	afx_msg void OnViewLogsOpenFileOptimizStep1Plan();
	afx_msg void OnViewLogsOpenFileOptimizStep2Plan();
	afx_msg void OnViewLogsOpenFileOptimizStep3Plan();
	afx_msg void OnViewLogsOpenFileInitialBasePlan();
	afx_msg void OnViewLogsOpenFileErrorsLog();
	afx_msg void OnViewLogsOpenFileInitDBMVehiclesAll();
	afx_msg void OnViewLogsOpenFileVehiclesList();
	afx_msg void OnViewEasyStartDlg();
	//}}AFX_MSG
	afx_msg long CFleetOptimizerApp::OnGoodbyeShipWizard(UINT wParam, LONG lParam);
	DECLARE_MESSAGE_MAP()

private:
	void RunEasyStartDlg();
	void ReadDataFromRegistry();
	void ScenarioWizardLoad();
	CPrototypeWizardShipDlg* m_pShipProtoWizDlg;
	CPrototypeWizardACVDlg* m_pACVProtoWizDlg;
	CShipTypeWizardDlg* m_pShipWizDlg;
	CPortWizardDlg* m_pPortWizDlg;
	CLogsDlg* m_pLogsDlg;
	CEasyStartDlg* m_pEasyStartDlg;
	char* m_pShipsDBFile;

	CFleetOptimizerDoc* m_pCurrFleetOptimizerDoc;

	BOOL m_bUseOptAll;
	BOOL m_bUseOptSpecific;
	BOOL m_bUseOptStep1;
	BOOL m_bUseOptStep2;
	BOOL m_bUseOptStep3;
	BOOL m_bUseSensitivityCheck;
	int m_nSenceCheckRate;
	void GetOptimizSelectedOptions();
	void LoadScenario();
	BOOL m_bScenLoaded;
	BOOL m_bShowEasyStartDlg;

	char* m_pLogFileVehiclesList; 
	char* m_pLogFileInitDBMVehiclesAll;
	char* m_pLogFileErrors;
	char* m_pLogFileInitialBasePlan;
	char* m_pLogFileOptimizStep1Plan;
	char* m_pLogFileOptimizStep2Plan;
	char* m_pLogFileOptimizStep3Plan;
	char* m_pLogFileVehiclesDetails;
	//CString m_sLogFNameError;

public:
	BOOL m_bOptimizationInProcess;
	HANDLE	m_hCreateInitialBasePlanThread;
	DWORD	m_nCreateInitialBasePlanThreadId;
	HANDLE	m_hOptimizationSt1LpThread;
	DWORD	m_nOptimizationSt1LpThreadId;
	HANDLE	m_hOptimizationSt2LpDpThread;
	DWORD	m_nOptimizationSt2LpDpThreadId;
	HANDLE	m_hOptimizationSt3DpThread;
	DWORD	m_nOptimizationSt3DpThreadId;
	HANDLE	m_hRunFullOptimizationThread;
	DWORD	m_nRunFullOptimizationThreadId;
	HANDLE	m_hRunOptimizerSelectedThread;
	DWORD	m_nRunOptimizerSelectedThreadId;
	static DWORD WINAPI CreateInitialBasePlanFunc(void* thisPointer);
	static DWORD WINAPI OptimizationSt1LpFunc(void* thisPointer);
	static DWORD WINAPI OptimizationSt2LpDpFunc(void* thisPointer);
	static DWORD WINAPI OptimizationSt3DpFunc(void* thisPointer);
	static DWORD WINAPI RunFullOptimizationFunc(void* thisPointer);
	static DWORD WINAPI RunOptimizerSelectedFunc(void* thisPointer);

};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FLEETOPTIMIZER_H__6AF9FBA0_DD03_4E07_9615_49FD0E5E9458__INCLUDED_)
