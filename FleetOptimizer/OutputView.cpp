// FleetOptimizerView.cpp : implementation of the COutputView class
//

#include "stdafx.h"
#include "FleetOptimizer.h"

#include "FleetOptimizerDoc.h"
#include "OutputView.h"
#include "InitialBasePlan.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// COutputView

IMPLEMENT_DYNCREATE(COutputView, CScrollView)

BEGIN_MESSAGE_MAP(COutputView, CScrollView)
	//{{AFX_MSG_MAP(COutputView)
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CScrollView::OnFilePrint)
//	ON_COMMAND(ID_FILE_PRINT_DIRECT, CScrollView::OnFilePrint)
//	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CScrollView::OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COutputView construction/destruction

COutputView::COutputView()
{
	// TODO: add construction code here

}

COutputView::~COutputView()
{
}

BOOL COutputView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CScrollView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// COutputView drawing

void COutputView::OnDraw(CDC* pDC)
{
	CFleetOptimizerDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	// TODO: add draw code for native data here
    TEXTMETRIC tm;
    CFont font;
    int i,nHeight;

/*
	font.CreateFont(-300, 0, 0, 0, 400, FALSE, FALSE, 0, ANSI_CHARSET,
              OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
              DEFAULT_PITCH | FF_SWISS, "Arial");
*/
	font.CreateFont(-300, 0, 0, 0, 400, FALSE, FALSE, 0, ANSI_CHARSET,
              OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
              DEFAULT_PITCH | FF_SWISS, "Courier New CYR");

    CFont* pOldFont = pDC->SelectObject(&font);
    pDC->GetTextMetrics(&tm);
    nHeight = tm.tmHeight + tm.tmExternalLeading;

/*    for (i = 0; i < 30; i++) {

      pDC->TextOut(50, -i * nHeight - 720, "KUKU");
    }
    */

	i = 0;
	CString str;
	POSITION pos = 	m_ListOfOutputStrings.GetHeadPosition();
	while (pos != NULL)
	{
		str = m_ListOfOutputStrings.GetNext(pos);
		pDC->TextOut(50, -i * nHeight - 720, str);
		i++;
	}

	pDC->SelectObject(pOldFont);	

}

/////////////////////////////////////////////////////////////////////////////
// COutputView printing

BOOL COutputView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void COutputView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void COutputView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

/////////////////////////////////////////////////////////////////////////////
// COutputView diagnostics

#ifdef _DEBUG
void COutputView::AssertValid() const
{
	CScrollView::AssertValid();
}

void COutputView::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}

CFleetOptimizerDoc* COutputView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CFleetOptimizerDoc)));
	return (CFleetOptimizerDoc*)m_pDocument;
}
#endif //_DEBUG



void COutputView::OnInitialUpdate() 
{
	CScrollView::OnInitialUpdate();
	
	// TODO: Add your specialized code here and/or call the base class
    //CSize sizeTotal(11520, 15120);  // 8 by 10.5 inches
    CSize sizeTotal(11520, 60000);  
	CSize sizePage(sizeTotal.cx / 2,
                   sizeTotal.cy / 2);   // page scroll
    CSize sizeLine(sizeTotal.cx / 100,
                   sizeTotal.cy / 100); // line scroll



    SetScrollSizes(MM_TWIPS, sizeTotal, sizePage, sizeLine);
}



void COutputView::TextOut(CString text)
{
	m_ListOfOutputStrings.AddTail(text);
	Invalidate(NULL);
}
