// OptimizationSt2LpDp.h: interface for the COptimizationSt2LpDp class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_OPTIMIZATIONST2LPDP_H__C6784434_B96F_4D18_B460_6638F08F8064__INCLUDED_)
#define AFX_OPTIMIZATIONST2LPDP_H__C6784434_B96F_4D18_B460_6638F08F8064__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class COutputView;

class COptimizationSt2LpDp  
{
public:
	int IsStepDone();
	int SimplexMethod(void);
	static COptimizationSt2LpDp* Instance();
	int print();

protected:
	COptimizationSt2LpDp();
	virtual ~COptimizationSt2LpDp();

private:
	void ResetExistVehicleNum(void);
	int cleanDummyRouters();
	int VehicleSCapabilityOnRouterCompute(CRouterL*); //capability of one vehicle on router
	int normRouter(CRouterL *&pt);
	void appendDynRListPrev(CRouterL*);
	int UpdateDynamicRoutersListPrev();
	int GoalFunctionPrevStep;
	int GoalFunction;
	int GoalFunctionCompute();
	int FindDynamicRouterBest();
	int CreateDynamicRoutersFirst(CRouterL *);
	int CreateDynamicRoutersNext();
	static COptimizationSt2LpDp* _instance;
	int RouterCreate(CRouterL *&);
	int VectorsMarksCompute(CRouterL *&pRouterOut, CRouterL *&pRouterIn, double &VectorsMark);
	CRouter*  m_RoutersList;
	CRouterL* m_DynamicRoutersList;
	CRouterL* m_DynamicRoutersListPrev;
	CRouterL* m_at_endDynRList;
	CRouterL* m_at_endDynRListPrev;
	CRouterL* m_RouterBestOnStep;
	CRouterL* m_RouterBestOnStepPrev;
	int m_bStepDone;

	void appendDynRList(CRouterL *);
//	int CreateDuplicateDynRList(void);
	//int RemoveDynamicRoutersList(CRouterL *);
	void RemoveDynamicRoutersList();
	void RemoveDynamicRoutersListPrev();
	COutputView* m_pOutView;

    CCargo::CargoType cargoType;      //Тип груза на маршруте






};

#endif // !defined(AFX_OPTIMIZATIONST2LPDP_H__C6784434_B96F_4D18_B460_6638F08F8064__INCLUDED_)
