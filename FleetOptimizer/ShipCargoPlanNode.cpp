// ShipCargoPlanNode.cpp: implementation of the CShipCargoPlanNode class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "fleetoptimizer.h"
#include "ShipCargoPlanNode.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CShipCargoPlanNode::CShipCargoPlanNode()
{
	next = NULL;
}

CShipCargoPlanNode::~CShipCargoPlanNode()
{

}
