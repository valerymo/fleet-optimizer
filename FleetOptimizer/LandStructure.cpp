// LandStructure.cpp: implementation of the CLandStructure class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "fleetoptimizer.h"
#include "LandStructure.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CLandStructure::CLandStructure()
{
	next = NULL;
}

CLandStructure::~CLandStructure()
{

}

int CLandStructure::GetType()
{
	return m_nType;
}

int CLandStructure::GetPrice()
{
	return m_nPrice; 
}


bool CLandStructure::IsServiceProvided(CVehicle * pVehicle, int serviceType){
							// serviceType:
							// 1 - причал
							// 2 - прощадка для СВП
							// 3 - ремонтная база для судов
							// 4 - ремонтная база для СВП

	if (serviceType != m_nType)
		return 0;

	CString strRegisterClass = pVehicle->GetRegisterClass();
	int MaxLoad;

	if (strRegisterClass.Find("*ACV*") != -1) //ACV
		MaxLoad = m_nMaxACVSizeService;
	else
		MaxLoad = m_nMaxShipSizeService;	// Ship

	if (pVehicle->GetLoad() > MaxLoad)
		return 0;

	return 1;
}

int CLandStructure::GetMaxACVSizeService()
{
	return m_nMaxACVSizeService;
}

int CLandStructure::GetMaxShipSizeService()
{
	return m_nMaxShipSizeService;

}
