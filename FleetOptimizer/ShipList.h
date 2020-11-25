// ShipList.h: interface for the CShipList class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SHIPLIST_H__A5122B89_074A_44D5_AA2B_77DE6A38EA49__INCLUDED_)
#define AFX_SHIPLIST_H__A5122B89_074A_44D5_AA2B_77DE6A38EA49__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CShipList : public CObject  
{
friend class CShipFactory;  
public:
    static CShipList* Instance();
    void append (CShip*);

protected:
	CShipList();
	virtual ~CShipList();

private:
    static CShipList* _instance;
    CShip *CShipList;
   	CShip *at_end;

};



#endif // !defined(AFX_SHIPLIST_H__A5122B89_074A_44D5_AA2B_77DE6A38EA49__INCLUDED_)
