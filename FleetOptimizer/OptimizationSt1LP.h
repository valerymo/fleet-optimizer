// OptimizationSt1LP.h: interface for the COptimizationSt1LP class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_OPTIMIZATIONST1LP_H__F778D7EC_E777_4F11_ABD8_5137C9A634D2__INCLUDED_)
#define AFX_OPTIMIZATIONST1LP_H__F778D7EC_E777_4F11_ABD8_5137C9A634D2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class COutputView;

class COptimizationSt1LP  
{
public:
	int IsStepDone();
	int GetLandStructExpenciesYear();
	CRouter * GetRouterListPointer(void);
	int SimplexMethod(void);
	static COptimizationSt1LP* Instance();
	void print();
protected:
	COptimizationSt1LP();
	virtual ~COptimizationSt1LP();
private:
	int m_LandStructExpenciesYear;
	CRouter * RoutersList;
	int GoalFunctionPrevStep;
	int GoalFunction;
	int VectorsMarksCompute(CRouter *&pRouterImprov, CVehicle *&pVehicleBest, int &VehicleBestNum, double &dVehicleNumBest, double &VectorsMarkBest, int &VehicleBestIncome);
	int GoalFunctionCorrect(void);
	int GoalFunctionCompute(void);
	int DualMarksCompute(void);
	COutputView* m_pOutView;
	int m_bStepDone;
	
	static COptimizationSt1LP* _instance;

};

#endif // !defined(AFX_OPTIMIZATIONST1LP_H__F778D7EC_E777_4F11_ABD8_5137C9A634D2__INCLUDED_)
