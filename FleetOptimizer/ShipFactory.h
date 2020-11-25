// ShipFactory.h: interface for the CShipFactory class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SHIPFACTORY_H__8E3B1EBD_F0DC_4F3D_925E_B47B32C0A72F__INCLUDED_)
#define AFX_SHIPFACTORY_H__8E3B1EBD_F0DC_4F3D_925E_B47B32C0A72F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "..\ShipDesignModule\ShipDesign.h"
#include "..\FleetOptimizer\FleetTechEconomicEng.h"

//COutputView* m_pOutView;
#include "OutputView.h"


class CShipFactory  
{
public:
	int GetErrorStatus();
	void CreateShipsInstances();
    static CShipFactory* Instance();
protected:
	CShipFactory();
	virtual ~CShipFactory();
private:
	int m_nErrorStatus;
	CString m_sLogFNameError;
	void GetInvestmentInfoFromScenario();
	void PutIntToArchive(int intData, CArchive& ar);
	int GetIntFromArchive(CArchive& ar);
	void CreateShipsInstancesFromScenario();
	char* m_pScenarioFile;
	COutputView* m_pOutView;

    static CShipFactory* _instance;
};

#endif // !defined(AFX_SHIPFACTORY_H__8E3B1EBD_F0DC_4F3D_925E_B47B32C0A72F__INCLUDED_)
