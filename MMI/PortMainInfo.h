// PortMainInfo.h: interface for the CPortMainInfo class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PORTMAININFO_H__A6F823BE_E228_401D_B057_4A2A8FB3D4C7__INCLUDED_)
#define AFX_PORTMAININFO_H__A6F823BE_E228_401D_B057_4A2A8FB3D4C7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000



class CPortMainInfo : public CObject  
{
public:
	CPortMainInfo();
	virtual ~CPortMainInfo();

	void operator=(const CPortMainInfo &portInfo);

	CString	m_strPortRegion;
	CString	m_strPortName;
	CString	m_strPortType;
	CString	m_strBasePortName;
	int m_nPortID;
    struct {
        int coal; //ugol
        int general; //generalni
        int oil; //neft
        int vegetables_fruits; //ovoschi i frukti  
        }required_cargoes, available_cargoes;

    struct LoadingProductivity {
        int coal; //ugol
        int general; //generalni
        int oil; //neft
        int vegetables_fruits; //ovoschi i frukti  
        }ship_loading_productivity, acv_loading_productivity;
};

#endif // !defined(AFX_PORTMAININFO_H__A6F823BE_E228_401D_B057_4A2A8FB3D4C7__INCLUDED_)
