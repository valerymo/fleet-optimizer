// ShipMainInfo.h: interface for the CShipMainInfo class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SHIPMAININFO_H__1C8D3DC0_2C96_4EED_A39C_1C43CB95A34A__INCLUDED_)
#define AFX_SHIPMAININFO_H__1C8D3DC0_2C96_4EED_A39C_1C43CB95A34A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CShipMainInfo : public CObject  
{
public:
	CShipMainInfo();
	virtual ~CShipMainInfo();

	void operator=(const CShipMainInfo &shipInfo);

public:
	CString	m_strlShipTypeName;
	CString	m_strShipRegisterClass;
	CString	m_strPrototypeName;
	int		m_nDW;
	int		m_nL;
	int		m_nB;
	int		m_nT;
	int		m_nSpeed;
	int		m_nRange;
	float	m_nh3;
	CString m_strh3;
	int		m_nCargoCompatiblOil;
	int		m_nCargoCompatiblGeneral;
	int		m_nCargoCompatiblCoal;
	int		m_nCargoCompatibleVegFruits;
	int		m_nLoadMin;
	int		m_nLoadMax;
	int		m_nLoadDelta;
	int		m_nTariffGenVF;
	int		m_nTariffOil;
	int		m_nTariffCoal;
	// {existing ship details begin
	int		m_nShipExistSetCheck;
	int		m_nShipExistForceCheck;
	int		m_nShipExistCheck;
	int		m_nShipExistBuildCheck;
	int		m_nShipExistDesignCheck;
	int		m_nShipExistNum;
	int		m_nShipExistAdvantPrice;
	int		m_nShipExistPrice;
	int		m_nShipExistBuildNum;
	int		m_nShipExistBuildAdvantPrice;
	int		m_nShipExistBuildPrice;
	int		m_nShipExistDesignAdvantPrice;
	int		m_nShipExistDesignPrice;
	//}existing ship details end

};

#endif // !defined(AFX_SHIPMAININFO_H__1C8D3DC0_2C96_4EED_A39C_1C43CB95A34A__INCLUDED_)
