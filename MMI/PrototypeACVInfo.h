// PrototypeACVInfo.h: interface for the CPrototypeACVInfo class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PROTOTYPEACVINFO_H__04844D01_32AB_406A_AB77_6B363DD92F1A__INCLUDED_)
#define AFX_PROTOTYPEACVINFO_H__04844D01_32AB_406A_AB77_6B363DD92F1A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define MAX_CHARACTER_NUM_ACV	50

class CPrototypeACVInfo : public Object  
{
public:
	CPrototypeACVInfo();
	virtual ~CPrototypeACVInfo();

private:
	CString strInfo[MAX_CHARACTER_NUM_ACV];

};

#endif // !defined(AFX_PROTOTYPEACVINFO_H__04844D01_32AB_406A_AB77_6B363DD92F1A__INCLUDED_)
