// LandStructure.h: interface for the CLandStructure class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LANDSTRUCTURE_H__FCDE0262_A3CE_40CA_A623_82D4B2F50BE1__INCLUDED_)
#define AFX_LANDSTRUCTURE_H__FCDE0262_A3CE_40CA_A623_82D4B2F50BE1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CLandStructure : public CObject  
{
friend class CLandStructList;
public:
	int GetMaxShipSizeService();
	int GetMaxACVSizeService();
	bool IsServiceProvided(CVehicle * pVehicle, int serviceType); 
							// serviceType:
							// 1 - причал
							// 2 - прощадка для СВП
							// 3 - ремонтная база для судов
							// 4 - ремонтная база для СВП
	int GetPrice();
	int GetType();
	CLandStructure();
	virtual ~CLandStructure();

private:
	//int m_nShipNumServCapability;
	int m_nMaxShipSizeService;
	int m_nMaxACVSizeService;
	int m_nPrice;
	int m_nType;	// 1 - причал
					// 2 - прощадка для СВП
					// 3 - площадка для судов с аппарелью в ПНБ
					// 4 - ремонтная база для судов
					// 5 - ремонтная база для СВП
	CLandStructure* next;

};

#endif // !defined(AFX_LANDSTRUCTURE_H__FCDE0262_A3CE_40CA_A623_82D4B2F50BE1__INCLUDED_)
