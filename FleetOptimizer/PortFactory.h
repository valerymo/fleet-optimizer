// PortFactory.h: interface for the CPortFactory class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PORTFACTORY_H__54893D9B_D30F_464A_A9ED_C7291DFFA2D1__INCLUDED_)
#define AFX_PORTFACTORY_H__54893D9B_D30F_464A_A9ED_C7291DFFA2D1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class COutputView;

class CPortFactory  
{
public:
	int SaveToFile();
	void CreatePortsInstances();
    static CPortFactory* Instance();
protected:
	CPortFactory();
	//virtual ~CPortFactory();
private:
	void CreatePortsInstancesFromScenario(void);
    static CPortFactory* _instance;
	char* m_pScenarioFile;
	char* m_pPortDistMatrixFile;
	int GetIntFromArchive(CArchive& ar);
	COutputView* m_pOutView;

};

#endif // !defined(AFX_PORTFACTORY_H__54893D9B_D30F_464A_A9ED_C7291DFFA2D1__INCLUDED_)
