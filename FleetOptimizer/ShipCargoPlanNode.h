// ShipCargoPlanNode.h: interface for the CShipCargoPlanNode class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SHIPCARGOPLANNODE_H__9CB08287_D36B_4BA2_A726_F5DC255E2BF1__INCLUDED_)
#define AFX_SHIPCARGOPLANNODE_H__9CB08287_D36B_4BA2_A726_F5DC255E2BF1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CShipCargoPlanNode : public CObject  
{
friend class CShipCargoPlan;
public:
	CShipCargoPlanNode();
	virtual ~CShipCargoPlanNode();

private:
	CShipCargoPlanNode* next;

    int coal; //�����
    int general; //����������� ����
    int oil; //�����
    int vegetables_fruits; // �����-������
};

#endif // !defined(AFX_SHIPCARGOPLANNODE_H__9CB08287_D36B_4BA2_A726_F5DC255E2BF1__INCLUDED_)
