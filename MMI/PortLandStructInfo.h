// PortLandStructInfo.h: interface for the CPortLandStructInfo class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PORTLANDSTRUCTINFO_H__E1693FAE_449A_49D0_B1D1_E624CE035A5E__INCLUDED_)
#define AFX_PORTLANDSTRUCTINFO_H__E1693FAE_449A_49D0_B1D1_E624CE035A5E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CPortLandStructInfo : public CObject  
{
public:
	CPortLandStructInfo();
	virtual ~CPortLandStructInfo();

	void operator=(const CPortLandStructInfo &portLandStructInfo);

	int	m_nLandStructCalcActivateCheck;
	int	m_nShipCourtAvailableCheck;
	int	m_nACVCourtAvailableCheck;
	int	m_nACVCourtInvestPP;
	int	m_nShipCourtInvestPP;
	int	m_nACVSpace;
	int	m_nACVSpaceElemPrice;
	int	m_nMoorLength;
	int	m_nMoorElemPrice;

	struct {
		int n; // Кол-во СВП, которое может принять существующая ремонтная база
		int Pgr; // Грузоподъемность СВП, для которого приводятся данные (кол-во и коэффициенты)
		int a; // Стоимость достройки ремонтной базы для одного СВП (указанной грузоподъемности)
		int b; // Стоимость достройки не зависяшая от количества СВП
    }m_structRepairDetailsACV, m_structRepairDetailsShip;

};

#endif // !defined(AFX_PORTLANDSTRUCTINFO_H__E1693FAE_449A_49D0_B1D1_E624CE035A5E__INCLUDED_)
