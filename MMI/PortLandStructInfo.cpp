// PortLandStructInfo.cpp: implementation of the CPortLandStructInfo class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "PortLandStructInfo.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPortLandStructInfo::CPortLandStructInfo()
{

}

CPortLandStructInfo::~CPortLandStructInfo()
{

}


void CPortLandStructInfo::operator=(const CPortLandStructInfo &portLandStructInfo){

	if (this == &portLandStructInfo)
			return;

	m_nLandStructCalcActivateCheck = portLandStructInfo.m_nLandStructCalcActivateCheck;

	m_nShipCourtAvailableCheck = portLandStructInfo.m_nShipCourtAvailableCheck;
	m_nACVCourtAvailableCheck = portLandStructInfo.m_nACVCourtAvailableCheck;
	m_nACVCourtInvestPP = portLandStructInfo.m_nACVCourtInvestPP;
	m_nShipCourtInvestPP = portLandStructInfo.m_nShipCourtInvestPP;

	m_nACVSpace = portLandStructInfo.m_nACVSpace;
	m_nACVSpaceElemPrice = portLandStructInfo.m_nACVSpaceElemPrice;
	m_nMoorLength = portLandStructInfo.m_nMoorLength;
	m_nMoorElemPrice = portLandStructInfo.m_nMoorElemPrice;

	m_structRepairDetailsACV.n = portLandStructInfo.m_structRepairDetailsACV.n;
	m_structRepairDetailsACV.Pgr = portLandStructInfo.m_structRepairDetailsACV.Pgr;
	m_structRepairDetailsACV.a = portLandStructInfo.m_structRepairDetailsACV.a;
	m_structRepairDetailsACV.b = portLandStructInfo.m_structRepairDetailsACV.b;

	m_structRepairDetailsShip.n = portLandStructInfo.m_structRepairDetailsShip.n;
	m_structRepairDetailsShip.Pgr = portLandStructInfo.m_structRepairDetailsShip.Pgr;
	m_structRepairDetailsShip.a = portLandStructInfo.m_structRepairDetailsShip.a;
	m_structRepairDetailsShip.b = portLandStructInfo.m_structRepairDetailsShip.b;

}
