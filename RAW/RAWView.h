
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
public:
	afx_msg void OnPixelpointprocessingAdd();
	afx_msg void OnPixelpointprocessingSub();
	afx_msg void OnPixelpointprocessingInverse();
	afx_msg void OnPixelpointprocessingBinarization();
	afx_msg void OnPixelpointprocessingLogtransform();
	afx_msg void OnPixelpointprocessingGammacorrection();
	afx_msg void OnPixelpointprocessingPosterization();
	afx_msg void OnPixelpointprocessingBitplaneslicing();
	afx_msg void OnPixelpointprocessingContraststretching();
	afx_msg void OnHistogramprocessingStretching();
	afx_msg void OnHistogramprocessingEndinsearch();
	afx_msg void OnHistogramprocessingEqualization();
	afx_msg void OnHistogramprocessingSpecification();
	afx_msg void OnRegionprocessingConvolution();
	afx_msg void OnPixelpointprocessingDissolve();
};

#ifndef _DEBUG  // debug version in RAWView.cpp
inline CRAWDoc* CRAWView::GetDocument() const
   { return reinterpret_cast<CRAWDoc*>(m_pDocument); }
#endif

