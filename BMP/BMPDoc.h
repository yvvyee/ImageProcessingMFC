
// BMPDoc.h : interface of the CBMPDoc class
//


#pragma once
#include "CDib.h"

class CBMPDoc : public CDocument
{
protected: // create from serialization only
	CBMPDoc() noexcept;
	DECLARE_DYNCREATE(CBMPDoc)

// Attributes
public:
	CDib m_Dib; // 추가

// Operations
public:

// Overrides
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
#ifdef SHARED_HANDLERS
	virtual void InitializeSearchContent();
	virtual void OnDrawThumbnail(CDC& dc, LPRECT lprcBounds);
#endif // SHARED_HANDLERS

// Implementation
public:
	virtual ~CBMPDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()

#ifdef SHARED_HANDLERS
	// Helper function that sets search content for a Search Handler
	void SetSearchContent(const CString& value);
#endif // SHARED_HANDLERS
public:
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
	virtual BOOL OnSaveDocument(LPCTSTR lpszPathName);
	CDib *OnColormodelRgb2yuv();
	void OnFrameprocessingMotionestimation();
	void OnFrameprocessingMotioncompensation();
	void OnFrameprocessingSimpledifference();
};
