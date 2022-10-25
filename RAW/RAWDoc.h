
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
	int m_inH = 0;		// �Է� RAW �̹����� ���� ũ��
	int m_inW = 0;		// �Է� RAW �̹����� ���� ũ��
	int m_inSz = 0;		// �Է� RAW �̹����� ��ü ũ��

	int m_outH = 0;		// ��� RAW �̹����� ���� ũ��
	int m_outW = 0;		// ��� RAW �̹����� ���� ũ��
	int m_outSz = 0;	// ��� RAW �̹����� ��ü ũ��

	int m_histSz = 0;	// ������׷� ũ��

	UCHAR* m_inImg;			// �Է� RAW �̹��� ����
	UCHAR* m_outImg;		// ��� RAW �̹��� ����
	UCHAR* m_inHistImg;		// �Է� �̹����� ������׷� �̹��� ����
	UCHAR* m_outHistImg;	// ��� �̹����� ������׷� �̹��� ����

// Operations
public:
	UCHAR* MakeHistImg(UCHAR* img);	// ������׷� �̹����� �����ϴ� �Լ�

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
	// ���� ȭ�ҿ� ��� ���� ���ϴ� �Լ�
	void OnPixelpointprocessingAdd();
	// ���� ȭ�ҿ��� ��� ���� ���� �Լ�
	void OnPixelpointprocessingSub();
	// ȭ�� ���� �Լ�
	void OnPixelpointprocessingInverse();
	// ȭ�� ����ȭ �Լ�
	void OnPixelpointprocessingBinarization();
	// �α� ��ȯ �Լ�
	void OnPixelpointprocessingLogtransform();
	// ���� ���� �Լ�
	void OnPixelpointprocessingGammacorrection();
	// �����Ͷ������̼� �Լ�
	void OnPixelpointprocessingPosterization();
	// ��Ʈ ��� ���� �Լ�
	void OnPixelpointprocessingBitplaneslicing();
};
