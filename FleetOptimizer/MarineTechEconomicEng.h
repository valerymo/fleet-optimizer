// MarineTechEconomicEng.h: interface for the CMarineTechEconomicEng class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MARINETECHECONOMICENG_H__482BD4CA_B713_41CD_923D_15209994C383__INCLUDED_)
#define AFX_MARINETECHECONOMICENG_H__482BD4CA_B713_41CD_923D_15209994C383__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class COutputView;

class CMarineTechEconomicEng : public CObject  
{
public:
	//int initLandStructurePPVirt();
	int UpdateLandStructVirtualDataSt0();
	int UpdateLandStructVirtualDataSt1();
	int UpdateLandStructVirtualDataSt2();
	//int UpdateLandStructVirtualDataSt1(CRouter *pRouter, CVehicle *pVehicleIn, int  nVehicleInNumber);
	int GetLandStructExpenciesYearAllSt1();
	int GetLandStructExpenciesYearAllSt2();
	int GetLandStructExpenciesYearAllSt3();
	int GetLandStructExpenciesYearDeltaSt3BasePort(CVehicle *pVehicleIn);
	int GetLandStructExpenciesYearDeltaSt3PPs(CRouterP* pRouter);
	int GetLandStructExpenciesYearDeltaSt2(CRouterL *pRouter, CVehicle *pVehicleIn, int VehicleInNum);
	int GetLandStructExpenciesYearDeltaSt1(CPort *pBasePort, int PNB_num, CVehicle *pVehicleIn, int VehicleNum, CVehicle *pVehicleBP, int VehicleBPNum,  int BasePlanNum);
	int GetLandStructExpenciesYearDeltaSt0(CPort *pBasePort, int PNB_num, CVehicle *pVehicleIn, int VehicleNum, int BasePlanNum);

	CMarineTechEconomicEng();
	virtual ~CMarineTechEconomicEng();
    static CMarineTechEconomicEng* Instance();

private:
	int ExpenciesYear(int Expencies);
	int CalculateShipCourtPricePP();
	int CalculateACVCourtPricePP();
	int InitLandStructurePPVirt();
	void doInitLandStructurePPTmp();
	//void doUpdateLandStructurePPVirt(CRouter *pRouter);
	int GetExpenciesUnicOrBiggestVehicle(CPort *pBasePort, int PNB_num, CVehicle *pVehicleIn, int VehicleNum, CVehicle *pVehicleBP, int VehicleBPNum, int res);
	int GetExpenciesShipCourt(CPort *pBasePort, int PNB_num, CVehicle *pVehicleIn);
	int GetExpenciesShipRepair(CPort *pBasePort, int VehicleInLoad, int VehicleNum);
	int GetExpenciesShipMoor(CPort *pBasePort, CVehicle *pVehicle);
	int GetExpenciesACVRepair(CPort *pBasePort, int VehicleInLoad, int VehicleNum);
	int GetExpenciesACVSpace(CPort *pBasePort, CVehicle *pVehicleIn, int VehicleNum);
	int GetExpenciesACVSpace(CPort *pBasePort, CVehicle *pVehicleIn, int VehicleNum, CVehicle *pVehicleBP, int VehicleBPNum);
	int IsBiggestVehicleOut(CVehicle *pVehicle);
	int IsBiggestVehicleIn(CVehicle *pVehicle);
	int IsUnicTypeVehicleOut(CVehicle *pVehicle);
	int IsUnicTypeVehicleIn(CVehicle* pVehicle);
	int GetBiggestShipLoad();
	int IsExistInPlanAmguema();
	int m_StepOpt;
	CRouter *m_pRouter;
	CRouterL *m_pRouterL;
	CRouterP *m_pRouterP;
	int GetMoorLength(CVehicle *pVehicle);
	int GetMoorLength();
	int GetSpaceForACVs();
	int GetSpaceForACVs(CVehicle *pVehicleIn, int VehicleNum);
	int GetSpaceForACVs(CRouterS *pRouter);

	static CMarineTechEconomicEng* _instance;

	COutputView* m_pOutView;


};

#endif // !defined(AFX_MARINETECHECONOMICENG_H__482BD4CA_B713_41CD_923D_15209994C383__INCLUDED_)
