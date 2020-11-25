// OptimizationSt3Dp.h: interface for the COptimizationSt3Dp class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_OPTIMIZATIONST3DP_H__968E00B7_BBEE_4F0D_8FA6_2DA821606305__INCLUDED_)
#define AFX_OPTIMIZATIONST3DP_H__968E00B7_BBEE_4F0D_8FA6_2DA821606305__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class COutputView;

class COptimizationSt3Dp  
{
public:
	int print();
	static COptimizationSt3Dp* Instance();
	int OptimisationDynProgMethod();
	
protected:
	COptimizationSt3Dp();
	virtual ~COptimizationSt3Dp();

private:
	void ResetExistVehicleNum(void);
	int ShipLoadingPlanCreate(CRouterP *);
	int RemoveDynamicRoutersListNode(CRouterP* pt);
	int UpdatePNBsCargoRequirments(CRouterP*);
	int CheckPNBsCargoRequirments(CRouterP*);
	int normRouter(CRouterP*&);
	int GetRouterPMarkStub(CRouterP*);
	int GetRouterPMark(CRouterP*);
	int UpdateDynamicRoutersListPrev();
	int CheckUpdateDynamicRoutersListOnStep(int step);
	void appendDynRList(CRouterP *pRouter);
	void appendDynRListPrev(CRouterP *pRouter);
	void RemoveDynamicRoutersList();
	void RemoveDynamicRoutersListPrev();
	int FindDynamicRouterBest();
	int FindDynamicRouterBest1();
	int CreateDynamicRoutersNext();
	int RouterCreate(CRouterP *&);
	int CreateDynamicRoutersFirst(CRouterP * pRouterP);

	static COptimizationSt3Dp* _instance;
	CRouter * m_RoutersList;
	CRouterP * m_DynamicRoutersList;
	CRouterP * m_DynamicRoutersListPrev;
	CRouterP * m_at_endDynRList;
	CRouterP * m_at_endDynRListPrev;
	CRouterP * m_RouterBest;
	CRouterP * m_RouterBestOnStep;
	CRouterP * m_RouterBestOnStepPrev;

	COutputView* m_pOutView;


};

#endif // !defined(AFX_OPTIMIZATIONST3DP_H__968E00B7_BBEE_4F0D_8FA6_2DA821606305__INCLUDED_)
