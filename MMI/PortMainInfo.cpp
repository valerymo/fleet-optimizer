// PortMainInfo.cpp: implementation of the CPortMainInfo class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "PortMainInfo.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPortMainInfo::CPortMainInfo()
{

}

CPortMainInfo::~CPortMainInfo()
{

}

void CPortMainInfo::operator=(const CPortMainInfo &portInfo){

	if (this == &portInfo)
			return;
	
	m_nPortID = portInfo.m_nPortID;
	m_strPortRegion = portInfo.m_strPortRegion;
	m_strPortName = portInfo.m_strPortName;
	m_strPortType = portInfo.m_strPortType;
	m_strBasePortName = portInfo.m_strBasePortName;

	required_cargoes.coal = portInfo.required_cargoes.coal;
	required_cargoes.general = portInfo.required_cargoes.general;
	required_cargoes.oil = portInfo.required_cargoes.oil;
	required_cargoes.vegetables_fruits = portInfo.required_cargoes.vegetables_fruits;

	available_cargoes.coal = portInfo.available_cargoes.coal;
	available_cargoes.general = portInfo.available_cargoes.general;
	available_cargoes.oil = portInfo.available_cargoes.oil;
	available_cargoes.vegetables_fruits = portInfo.available_cargoes.vegetables_fruits;

	ship_loading_productivity.coal = portInfo.ship_loading_productivity.coal;
	ship_loading_productivity.general = portInfo.ship_loading_productivity.general;
	ship_loading_productivity.oil = portInfo.ship_loading_productivity.oil;
	ship_loading_productivity.vegetables_fruits = portInfo.ship_loading_productivity.vegetables_fruits;

	acv_loading_productivity.coal = portInfo.acv_loading_productivity.coal;
	acv_loading_productivity.general = portInfo.acv_loading_productivity.general;
	acv_loading_productivity.oil = portInfo.acv_loading_productivity.oil;
	acv_loading_productivity.vegetables_fruits = portInfo.acv_loading_productivity.vegetables_fruits;


}
