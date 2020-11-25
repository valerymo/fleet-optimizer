// LandStructList.cpp: implementation of the CLandStructList class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "fleetoptimizer.h"
#include "LandStructList.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CLandStructList::CLandStructList()
{
	m_pLandStructList = NULL;
}

CLandStructList::~CLandStructList()
{

}

void CLandStructList::append(CLandStructure* pLandStruct)
{
    if(CLandStructList::m_pLandStructList == 0)
        m_pLandStructList = pLandStruct;
    else
        at_end->next = pLandStruct;

	at_end = pLandStruct;
}
