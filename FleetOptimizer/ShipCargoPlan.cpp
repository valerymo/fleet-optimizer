// ShipCargoPlan.cpp: implementation of the CShipCargoPlan class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "fleetoptimizer.h"
#include "ShipCargoPlan.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CShipCargoPlan::CShipCargoPlan()
{
	m_pList = 0;
}

CShipCargoPlan::~CShipCargoPlan()
{

}

void CShipCargoPlan::append(CShipCargoPlanNode *pCargoNode)
{
    if(CShipCargoPlan::m_pList == 0)
        m_pList = pCargoNode;
    else
        m_at_end->next = pCargoNode;
            
    m_at_end = pCargoNode;
}

CShipCargoPlanNode* CShipCargoPlan::GetShipCargoPlanHead()
{
	return m_pList;
}
