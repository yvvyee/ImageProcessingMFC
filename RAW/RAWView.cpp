
// RAWView.cpp : implementation of the CRAWView class
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "RAW.h"
#endif

#include "RAWDoc.h"
#include "RAWView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CRAWView

IMPLEMENT_DYNCREATE(CRAWView, CView)

BEGIN_MESSAGE_MAP(CRAWView, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CRAWView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
END_MESSAGE_MAP()

// CRAWView construction/destruction

CRAWView::CRAWView() noexcept
{
	// TODO: add construction code here

}

CRAWView::~CRAWView()
{
}

BOOL CRAWView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CRAWView drawing

void CRAWView::OnDraw(CDC* /*pDC*/)
{
	CRAWDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: add draw code for native data here
}


// CRAWView printing


void CRAWView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CRAWView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CRAWView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CRAWView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

void CRAWView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CRAWView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CRAWView diagnostics

#ifdef _DEBUG
void CRAWView::AssertValid() const
{
	CView::AssertValid();
}

void CRAWView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CRAWDoc* CRAWView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CRAWDoc)));
	return (CRAWDoc*)m_pDocument;
}
#endif //_DEBUG


// CRAWView message handlers
