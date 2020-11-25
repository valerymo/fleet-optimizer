// FleetTechEconomicEng.h: interface for the CFleetTechEconomicEng class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FLEETTECHECONOMICENG_H__F4408052_62D2_4C18_9B04_DF6BCA3F94F8__INCLUDED_)
#define AFX_FLEETTECHECONOMICENG_H__F4408052_62D2_4C18_9B04_DF6BCA3F94F8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "..\MMI\PrototypeWizardShipDlg.h"
#include "..\MMI\PrototypeWizardACVDlg.h"
#include "..\ShipDesignModule\ShipDesign.h"
#include "..\ShipDesignModule\ACVDesign.h"
#include "Ship.h"

class COutputView;

class CFleetTechEconomicEng  
{
friend class CShipFactory;  
public:
	float GetCargoSpecificCapacity(int, int, int, int);
	int GetErrorStatus();
	long InvestmentYearCompute(CRouter*); 
	long InvestmentYearCompute(CRouterL*); 
	//long InvestmentYearComputeForRouterP(CRouterP*); 
	int SetInvestmentYear(long InvestmentYear);
	long GetInvestmentYear();
	int VehicleInvestCapabilityValidate(CVehicle *pVehicleIn,  CVehicle *pVehicleOut);
	long GetInvestmentYearLimit();
	int GetVehicleTariff(CVehicle * pVehicle, int cargo_type);
	int VehicleIncomeOnRouterPForYear(CRouterP *);
	int GetShipPrototypeMainDetails(ShipPrototypeMainDetails *prototype);
	int GetACVPrototypeMainDetails(ACVPrototypeMainDetails *prototype);
	//int GetACVBuildCoast(CString strShipTypeName, CString strRegisterClass, int load, int speed, int L, int B, int T, CString strPrototypeName);
	int GetACVBuildCoast(ACVMainDetails * project, CString strPrototypeName);
	//int GetVehicleBuildCoast(CString strShipTypeName, CString strRegisterClass, int load, int speed, int L, int B, int T, CString strPrototypeName);
	int GetVehicleBuildCoast(ShipMainDetails * project, CString strPrototypeName);
	int GetExistACVBuildCoast(CShip* pACV, ACVMainDetails* project);
	int GetExistVehicleBuildCoast(CShip* pShip, ShipMainDetails* project);
	int VehicleRouterPLineValidation(CPort *pBasePort, int PNB_num, int cargo_type, CVehicle *pVehicle);
	int VehicleIncomeOnRouterLForYear(CRouterL*, int&, double&, int& Capability);
	int GetVehicleOperationsDaysInYear(char* VehicleClass, char* PortRegion);
	int GetVehicleRepairYearTime(char* VehicleClass, char* PortRegion);
	int GetVehicleWaitingYearTimeForWether(char* VehicleClass, char* PortRegion);
	int GetVehicleNavigationPeriod(char* VehicleClass, char* PortRegion);
	int VehicleBuildCoastYearPay(CVehicle *pVehicle);
	int VehicleOperationalExpensesYear(CVehicle *pVehicle, char* Region, int TripForYear);
	int VehicleOperationalExpensesDay(CVehicle *pVehicle, char* Region, int &ExpensesDayOnTrip,int &ExpensesDayInPortLoading,int &ExpensesDayInPortNoLoading,int &ExpensesDayNoOperationWinter,int &ExpensesDayOnRepair);
    int VehicleIncomeOnLineForYear(CPort *pBasePort, int PNB_num, int cargo_type, CVehicle *pVehicle,int &vehicle_number, double &dVehicleNum, int &BuildCoastYearPay, int &OperationalExpensesYear, int &vehicleCargoForNavigation);
	int VehicleIncomeOnLineForYear(CPort *pBasePort, int PNB_num, int cargo_type, CVehicle *pVehicle,int &vehicle_number, double &dVehicleNum, int &BuildCoastYearPay, int &OperationalExpensesYear, int &vehicleCargoForNavigation, int cardo_done);
	int VehicleLineValidation(CPort *pBasePort, int PNB_num, int cargo_type,CVehicle *pVehicle);
	int VehicleLineValidation(CPort *pBasePort, int PNB_num, int cargo_type,CVehicle *pVehicle, int cargo_done);
	int VehicleRouterNumberForNavigation(CPort *pBasePort, int PNB_num, int cargo_type,CVehicle *pVehicle);
    static CFleetTechEconomicEng* Instance();
protected:
    CFleetTechEconomicEng();
	virtual ~CFleetTechEconomicEng();
private:
	float GetCargoSpecificCapacity(CVehicle *pVehicle);
	int m_nErrorStatus;
	CString m_sLogFNameError;
	int SetInvestmentYearLimit(long InvestmentYearLimit);
	long m_InvestmentYearLimit;
	long m_InvestmentYear;
	int SetShipPrototypeMainDetails(ShipPrototypeMainDetails * prototype, int i);
	int SetACVPrototypeMainDetails(ACVPrototypeMainDetails * prototype, int i);
	void DumpShipProtoDBIntoTable(void);
	void DumpACVProtoDBIntoTable(void);
	//ShipPrototypeMainDetails m_aShipPrototype [MAX_PROTOTYPES_RECORDS_IN_SHIPDB];
	ShipPrototypeMainDetails * m_aShipPrototype [MAX_PROTOTYPES_RECORDS_IN_SHIPDB];
	ACVPrototypeMainDetails m_aACVPrototype [MAX_PROTOTYPES_RECORDS_IN_ACVDB];
	//ACVPrototypeMainDetails (*m_aACVPrototype) [MAX_PROTOTYPES_RECORDS_IN_ACVDB];
	COutputView* m_pOutView;
    
	static CFleetTechEconomicEng* _instance;

};

#endif // !defined(AFX_FLEETTECHECONOMICENG_H__F4408052_62D2_4C18_9B04_DF6BCA3F94F8__INCLUDED_)
