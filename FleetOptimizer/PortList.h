// PortList.h: interface for the CPortList class.
//
//////////////////////////////////////////////////////////////////////
#include "Port.h"

#if !defined(AFX_PORTLIST_H__76084509_6A2D_442E_9AFB_D15120DC5742__INCLUDED_)
#define AFX_PORTLIST_H__76084509_6A2D_442E_9AFB_D15120DC5742__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CPortList  
{
friend class CPortFactory;  
public:
	CPort * GetPortListHead();
	int is_empty();
    static CPortList* Instance();
    void append (CPort*);

protected:
    CPortList();
	virtual ~CPortList();

private:
    static CPortList* _instance;
    CPort *PortList;
   	CPort *at_end;
};


#endif // !defined(AFX_PORTLIST_H__76084509_6A2D_442E_9AFB_D15120DC5742__INCLUDED_)
