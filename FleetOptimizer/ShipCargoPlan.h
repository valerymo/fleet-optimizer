// ShipCargoPlan.h: interface for the CShipCargoPlan class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SHIPCARGOPLAN_H__2FA32520_E619_4E82_BF58_3453A4A09BC9__INCLUDED_)
#define AFX_SHIPCARGOPLAN_H__2FA32520_E619_4E82_BF58_3453A4A09BC9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ShipCargoPlanNode.h"

class CShipCargoPlan : public CObject  
{
public:
	CShipCargoPlanNode* GetShipCargoPlanHead();
	void append(CShipCargoPlanNode*);
	CShipCargoPlan();
	virtual ~CShipCargoPlan();

private:
	CShipCargoPlanNode* m_pList;
	CShipCargoPlanNode* m_at_end;

};

#endif // !defined(AFX_SHIPCARGOPLAN_H__2FA32520_E619_4E82_BF58_3453A4A09BC9__INCLUDED_)
