
// RAWDoc.h : interface of the CRAWDoc class
//


#pragma once


class CRAWDoc : public CDocument
{
protected: // create from serialization only
	CRAWDoc() noexcept;
	DECLARE_DYNCREATE(CRAWDoc)

// Attributes
public:
	int m_inH = 0;		// 입력 RAW 이미지의 세로 크기
	int m_inW = 0;		// 입력 RAW 이미지의 가로 크기
	int m_inSz = 0;		// 입력 RAW 이미지의 전체 크기

	int m_outH = 0;		// 결과 RAW 이미지의 세로 크기
	int m_outW = 0;		// 결과 RAW 이미지의 가로 크기
	int m_outSz = 0;	// 결과 RAW 이미지의 전체 크기

	int m_histSz = 0;	// 히스토그램 크기

	UCHAR* m_inImg;			// 입력 RAW 이미지 버퍼
	UCHAR* m_outImg;		// 결과 RAW 이미지 버퍼
	UCHAR* m_inHistImg;		// 입력 이미지의 히스토그램 이미지 버퍼
	UCHAR* m_outHistImg;	// 결과 이미지의 히스토그램 이미지 버퍼

// Operations
public:
	UCHAR* MakeHistImg(UCHAR* img);	// 히스토그램 이미지를 생성하는 함수

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
	virtual ~CRAWDoc();
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
};
