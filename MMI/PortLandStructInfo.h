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
		int n; // ���-�� ���, ������� ����� ������� ������������ ��������� ����
		int Pgr; // ���������������� ���, ��� �������� ���������� ������ (���-�� � ������������)
		int a; // ��������� ��������� ��������� ���� ��� ������ ��� (��������� ����������������)
		int b; // ��������� ��������� �� ��������� �� ���������� ���
    }m_structRepairDetailsACV, m_structRepairDetailsShip;

};

#endif // !defined(AFX_PORTLANDSTRUCTINFO_H__E1693FAE_449A_49D0_B1D1_E624CE035A5E__INCLUDED_)
