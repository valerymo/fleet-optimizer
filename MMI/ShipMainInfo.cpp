// ShipMainInfo.cpp: implementation of the CShipMainInfo class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ShipMainInfo.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CShipMainInfo::CShipMainInfo()
{

}

CShipMainInfo::~CShipMainInfo()
{

}


void CShipMainInfo::operator=(const CShipMainInfo &shipInfo){

	if (this == &shipInfo)
		return;

	m_strlShipTypeName = shipInfo.m_strlShipTypeName;
	m_strShipRegisterClass = shipInfo.m_strShipRegisterClass;
	m_strPrototypeName = shipInfo.m_strPrototypeName;
	m_nDW = shipInfo.m_nDW;
	m_nL = shipInfo.m_nL;
	m_nB = shipInfo.m_nB;
	m_nT = shipInfo.m_nT;
	m_nSpeed = shipInfo.m_nSpeed;
	m_nRange = shipInfo.m_nRange;
	m_strh3 = shipInfo.m_strh3;
	m_nCargoCompatiblOil = shipInfo.m_nCargoCompatiblOil;
	m_nCargoCompatiblGeneral = shipInfo.m_nCargoCompatiblGeneral;
	m_nCargoCompatiblCoal = shipInfo.m_nCargoCompatiblCoal;
	m_nCargoCompatibleVegFruits = shipInfo.m_nCargoCompatibleVegFruits;
	m_nLoadMin = shipInfo.m_nLoadMin;
	m_nLoadMax = shipInfo.m_nLoadMax;
	m_nLoadDelta = shipInfo.m_nLoadDelta;
	m_nTariffGenVF = shipInfo.m_nTariffGenVF;
	m_nTariffOil = shipInfo.m_nTariffOil;
	m_nTariffCoal = shipInfo.m_nTariffCoal;

	// begin: ship exist
	m_nShipExistSetCheck = shipInfo.m_nShipExistSetCheck;
	m_nShipExistForceCheck = shipInfo.m_nShipExistForceCheck;
	m_nShipExistCheck = shipInfo.m_nShipExistCheck;
	m_nShipExistBuildCheck = shipInfo.m_nShipExistBuildCheck;
	m_nShipExistDesignCheck = shipInfo.m_nShipExistDesignCheck;
	m_nShipExistNum = shipInfo.m_nShipExistNum;
	m_nShipExistAdvantPrice = shipInfo.m_nShipExistAdvantPrice;
	m_nShipExistPrice = shipInfo.m_nShipExistPrice;
	m_nShipExistBuildNum = shipInfo.m_nShipExistBuildNum;
	m_nShipExistBuildAdvantPrice = shipInfo.m_nShipExistBuildAdvantPrice;
	m_nShipExistBuildPrice = shipInfo.m_nShipExistBuildPrice;
	m_nShipExistDesignAdvantPrice = shipInfo.m_nShipExistDesignAdvantPrice;
	m_nShipExistDesignPrice = shipInfo.m_nShipExistDesignPrice;
	//end: ship exist


}
