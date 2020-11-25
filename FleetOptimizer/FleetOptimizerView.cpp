// FleetOptimizerView.cpp : implementation of the CFleetOptimizerView class
//

#include "stdafx.h"
#include "FleetOptimizer.h"

#include "FleetOptimizerDoc.h"
#include "FleetOptimizerView.h"
#include "InitialBasePlan.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFleetOptimizerView

IMPLEMENT_DYNCREATE(CFleetOptimizerView, CView)

BEGIN_MESSAGE_MAP(CFleetOptimizerView, CView)
	//{{AFX_MSG_MAP(CFleetOptimizerView)
	//}}AFX_MSG_MAP
	// Standard printing commands
//	ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
//	ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
//	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFleetOptimizerView construction/destruction

CFleetOptimizerView::CFleetOptimizerView()
{
	// TODO: add construction code here

}

CFleetOptimizerView::~CFleetOptimizerView()
{
}

BOOL CFleetOptimizerView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CFleetOptimizerView drawing

void CFleetOptimizerView::OnDraw(CDC* pDC)
{
	CFleetOptimizerDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	// TODO: add draw code for native data here
}

/////////////////////////////////////////////////////////////////////////////
// CFleetOptimizerView printing

BOOL CFleetOptimizerView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CFleetOptimizerView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CFleetOptimizerView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

/////////////////////////////////////////////////////////////////////////////
// CFleetOptimizerView diagnostics

#ifdef _DEBUG
void CFleetOptimizerView::AssertValid() const
{
	CView::AssertValid();
}

void CFleetOptimizerView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CFleetOptimizerDoc* CFleetOptimizerView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CFleetOptimizerDoc)));
	return (CFleetOptimizerDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CFleetOptimizerView message handlers






