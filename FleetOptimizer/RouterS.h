// RouterS.h: interface for the CRouterS class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ROUTERS_H__30BF9B6B_8A70_47DB_93A7_F67E9D0E03CC__INCLUDED_)
#define AFX_ROUTERS_H__30BF9B6B_8A70_47DB_93A7_F67E9D0E03CC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Router.h"
#include "Line.h"


class CRouterS : public CRouter  
{
public:
	void appendLine(CLine *pLine);
	CLine * GetLine();
	void GetRouter();
	CRouterS(CPort *,int, int);
	virtual ~CRouterS();
private:
    CLine *Line;
};

#endif // !defined(AFX_ROUTERS_H__30BF9B6B_8A70_47DB_93A7_F67E9D0E03CC__INCLUDED_)
