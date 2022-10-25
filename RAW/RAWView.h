
// RAWView.h : interface of the CRAWView class
//

#pragma once


class CRAWView : public CView
{
protected: // create from serialization only
	CRAWView() noexcept;
	DECLARE_DYNCREATE(CRAWView)

// Attributes
public:
	CRAWDoc* GetDocument() const;

// Operations
public:
	void ShowRAWImg(CDC& pDC, CRAWDoc* pDoc, bool isOut);

// Overrides
public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// Implementation
public:
	virtual ~CRAWView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in RAWView.cpp
inline CRAWDoc* CRAWView::GetDocument() const
   { return reinterpret_cast<CRAWDoc*>(m_pDocument); }
#endif

