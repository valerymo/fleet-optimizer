// Cargo.h: interface for the CCargo class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CARGO_H__2C70A446_B60F_450D_89DC_0B20F577DAF0__INCLUDED_)
#define AFX_CARGO_H__2C70A446_B60F_450D_89DC_0B20F577DAF0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class  CCargo  
{
public:
    int coal; //ugol
    int general; //generalni
    int oil; //neft
    int vegetables_fruits; //ovoschi i frukti  
	CCargo();
	virtual ~CCargo();
    enum CargoType {Coal=1,General,Oil,Vegetables_fruits};

};

#endif // !defined(AFX_CARGO_H__2C70A446_B60F_450D_89DC_0B20F577DAF0__INCLUDED_)
