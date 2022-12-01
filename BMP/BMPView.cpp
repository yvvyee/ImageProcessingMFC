
// BMPView.cpp : implementation of the CBMPView class
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "BMP.h"
#endif

#include "BMPDoc.h"
#include "BMPView.h"

#include "CHistogramDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CBMPView

IMPLEMENT_DYNCREATE(CBMPView, CScrollView)

BEGIN_MESSAGE_MAP(CBMPView, CScrollView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CBMPView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_COMMAND(ID_HISTOGRAM, &CBMPView::OnHistogram)
END_MESSAGE_MAP()

// CBMPView construction/destruction

CBMPView::CBMPView() noexcept
{
	// TODO: add construction code here

}

CBMPView::~CBMPView()
{
}

BOOL CBMPView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CBMPView drawing

void CBMPView::OnDraw(CDC* pDC)
{
	CBMPDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: add draw code for native data here
	if (pDoc->m_Dib.IsValid())
	{
		pDoc->m_Dib.Draw(pDC->m_hDC);
	}
}


// CBMPView printing


void CBMPView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CBMPView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CBMPView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CBMPView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

void CBMPView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CBMPView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CBMPView diagnostics

#ifdef _DEBUG
void CBMPView::AssertValid() const
{
	CView::AssertValid();
}

void CBMPView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CBMPDoc* CBMPView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CBMPDoc)));
	return (CBMPDoc*)m_pDocument;
}
#endif //_DEBUG


// CBMPView message handlers


void CBMPView::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();

	// TODO: Add your specialized code here and/or call the base class
	CSize sizeTotal;
	CBMPDoc* pDoc = GetDocument();
	if (pDoc->m_Dib.IsValid()) {
		sizeTotal.cx = pDoc->m_Dib.GetWidth();
		sizeTotal.cy = pDoc->m_Dib.GetHeight();
	}
	else {
		sizeTotal.cx = sizeTotal.cy = 100;
	}
	SetScrollSizes(MM_TEXT, sizeTotal);
	ResizeParentToFit(TRUE);
}


void CBMPView::OnHistogram()
{
	// TODO: Add your command handler code here
	CBMPDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	CHistogramDlg dlg;
	dlg.SetImage(pDoc->m_Dib);
	dlg.DoModal();
}
