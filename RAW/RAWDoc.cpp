
// RAWDoc.cpp : implementation of the CRAWDoc class
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "RAW.h"
#endif

#include "RAWDoc.h"

#include <propkey.h>

#include "CInputDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CRAWDoc

IMPLEMENT_DYNCREATE(CRAWDoc, CDocument)

BEGIN_MESSAGE_MAP(CRAWDoc, CDocument)
END_MESSAGE_MAP()


// CRAWDoc construction/destruction

CRAWDoc::CRAWDoc() noexcept
{
	// TODO: add one-time construction code here

}

CRAWDoc::~CRAWDoc()
{
}

UCHAR* CRAWDoc::MakeHistImg(UCHAR* img)
{
	m_histSz = 256;
	
	// �� ����
	int value;
	int HIST[256] = { 0, };
	int img_size = m_inH * m_inW;
	for (int i = 0; i < img_size; i++)
	{
		value = (int)img[i];
		HIST[value]++;
	}

	// �ּ�-�ִ� ����ȭ
	double MAX = HIST[0];
	double MIN = HIST[0];
	for (int i = 0; i < m_histSz; i++)
	{
		if (MIN > HIST[i])
		{
			MIN = HIST[i];
		}
		if (MAX < HIST[i])
		{
			MAX = HIST[i];
		}
	}
	double DIF = MAX - MIN;
	for (int i = 0; i < m_histSz; i++)
	{
		HIST[i] = (unsigned char)
			((HIST[i] - MIN) / DIF * 255);
	}
	// ������׷� �̹���ȭ
	int histImgSz = (m_histSz * m_histSz)
		+ (m_histSz * 20);
	UCHAR* histoImg = new UCHAR[histImgSz];

	// ������� �ʱ�ȭ
	for (int i = 0; i < histImgSz; i++)
	{
		histoImg[i] = 255;
	}

	// ������׷� ����κи� ������ ĥ��
	for (int i = 0; i < m_histSz; i++)
	{
		for (int j = 0; j < HIST[i]; j++)
		{
			histoImg[m_histSz * (m_histSz - j - 1) + i] = 0;
		}
	}
	// ������׷� �ϴ� ���� ǥ��
	for (int i = m_histSz + 5; i < m_histSz + 20; i++)
	{
		for (int j = 0; j < m_histSz; j++)
		{
			histoImg[m_histSz * i + j] = j;
		}
	}
	return histoImg;
}

BOOL CRAWDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}




// CRAWDoc serialization

void CRAWDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}

#ifdef SHARED_HANDLERS

// Support for thumbnails
void CRAWDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
	// Modify this code to draw the document's data
	dc.FillSolidRect(lprcBounds, RGB(255, 255, 255));

	CString strText = _T("TODO: implement thumbnail drawing here");
	LOGFONT lf;

	CFont* pDefaultGUIFont = CFont::FromHandle((HFONT) GetStockObject(DEFAULT_GUI_FONT));
	pDefaultGUIFont->GetLogFont(&lf);
	lf.lfHeight = 36;

	CFont fontDraw;
	fontDraw.CreateFontIndirect(&lf);

	CFont* pOldFont = dc.SelectObject(&fontDraw);
	dc.DrawText(strText, lprcBounds, DT_CENTER | DT_WORDBREAK);
	dc.SelectObject(pOldFont);
}

// Support for Search Handlers
void CRAWDoc::InitializeSearchContent()
{
	CString strSearchContent;
	// Set search contents from document's data.
	// The content parts should be separated by ";"

	// For example:  strSearchContent = _T("point;rectangle;circle;ole object;");
	SetSearchContent(strSearchContent);
}

void CRAWDoc::SetSearchContent(const CString& value)
{
	if (value.IsEmpty())
	{
		RemoveChunk(PKEY_Search_Contents.fmtid, PKEY_Search_Contents.pid);
	}
	else
	{
		CMFCFilterChunkValueImpl *pChunk = nullptr;
		ATLTRY(pChunk = new CMFCFilterChunkValueImpl);
		if (pChunk != nullptr)
		{
			pChunk->SetTextValue(PKEY_Search_Contents, value, CHUNK_TEXT);
			SetChunkValue(pChunk);
		}
	}
}

#endif // SHARED_HANDLERS

// CRAWDoc diagnostics

#ifdef _DEBUG
void CRAWDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CRAWDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CRAWDoc commands


BOOL CRAWDoc::OnOpenDocument(LPCTSTR lpszPathName)
{
	if (!CDocument::OnOpenDocument(lpszPathName))
		return FALSE;

	// TODO:  Add your specialized creation code here
	CFile File;
	File.Open(lpszPathName, CFile::modeRead | CFile::typeBinary);

	if (File.GetLength() == 32 * 32) {
		m_inH = 32;
		m_inW = 32;
	}
	else if (File.GetLength() == 64 * 64) {
		m_inH = 64;
		m_inW = 64;
	}
	else if (File.GetLength() == 128 * 128) {
		m_inH = 128;
		m_inW = 128;
	}
	else if (File.GetLength() == 256 * 256) {
		m_inH = 256;
		m_inW = 256;
	}
	else if (File.GetLength() == 512 * 512) {
		m_inH = 512;
		m_inW = 512;
	}
	else if (File.GetLength() == 640 * 480) {
		m_inH = 480;
		m_inW = 640;
	}
	else if (File.GetLength() == 176 * 144) {
		m_inH = 144;
		m_inW = 176;
	}
	else if (File.GetLength() == 176 * 216) {
		m_inH = 216;
		m_inW = 176;
	}
	else {
		AfxMessageBox(L"Not Support Size", MB_OK | MB_ICONEXCLAMATION);
		return 0;
	}
	m_inSz = m_inW * m_inH;
	m_inImg = new UCHAR[m_inSz];

	for (int i = 0; i < m_inSz; i++) {
		m_inImg[i] = 255;
	}
	File.Read(m_inImg, m_inSz);
	File.Close();

	m_inHistImg = MakeHistImg(m_inImg);
	return TRUE;
}


// ���� ȭ�ҿ� ��� ���� ���ϴ� �Լ�
void CRAWDoc::OnPixelpointprocessingAdd()
{
	// TODO: Add your implementation code here.
	m_outH = m_inH;
	m_outW = m_inW;
	m_outSz = m_outH * m_outW;
	m_outImg = new unsigned char[m_outSz];
	CInputDialog dlg;
	if (dlg.DoModal() == IDOK) {
		int num = (int)dlg.number;
		for (int i = 0; i < m_inSz; i++) {
			int val = m_inImg[i] + num;
			if (val > 255) {
				// wrapping ���
				val = val % 256;
			}
			m_outImg[i] = val;
		}
		m_outHistImg = MakeHistImg(m_outImg);
	}
}


// ���� ȭ�ҿ��� ��� ���� ���� �Լ�
void CRAWDoc::OnPixelpointprocessingSub()
{
	// TODO: Add your implementation code here.
	m_outH = m_inH;
	m_outW = m_inW;
	m_outSz = m_outH * m_outW;
	m_outImg = new unsigned char[m_outSz];
	CInputDialog dlg;
	if (dlg.DoModal() == IDOK) {
		int num = (int)dlg.number;
		for (int i = 0; i < m_inSz; i++) {
			int val = m_inImg[i] - num;
			if (val < 0) {
				// Clamping ���
				val = 0;
			}
			m_outImg[i] = val;
		}
		m_outHistImg = MakeHistImg(m_outImg);
	}
}


// ȭ�� ���� �Լ�
void CRAWDoc::OnPixelpointprocessingInverse()
{
	// TODO: Add your implementation code here.
	m_outH = m_inH;
	m_outW = m_inW;
	m_outSz = m_outH * m_outW;
	m_outImg = new unsigned char[m_outSz];

	for (int i = 0; i < m_inSz; i++) {
		m_outImg[i] = 255 - m_inImg[i];
	}
	m_outHistImg = MakeHistImg(m_outImg);

}


// ȭ�� ����ȭ �Լ�
void CRAWDoc::OnPixelpointprocessingBinarization()
{
	// TODO: Add your implementation code here.
	m_outH = m_inH;
	m_outW = m_inW;
	m_outSz = m_outH * m_outW;
	m_outImg = new unsigned char[m_outSz];
	CInputDialog dlg;
	if (dlg.DoModal() == IDOK) {
		int threshold = (int)dlg.number;
		for (int i = 0; i < m_inSz; i++) {
			int val = m_inImg[i];
			if (val < threshold) {
				m_outImg[i] = 0;
			}
			else {
				m_outImg[i] = 255;
			}
		}
		m_outHistImg = MakeHistImg(m_outImg);
	}
}


// �α� ��ȯ �Լ�
void CRAWDoc::OnPixelpointprocessingLogtransform()
{
	// TODO: Add your implementation code here.
	m_outH = m_inH;
	m_outW = m_inW;
	m_outSz = m_outH * m_outW;
	m_outImg = new unsigned char[m_outSz];

	CInputDialog dlg;
	if (dlg.DoModal() == IDOK) {
		double cons = dlg.number;
		for (int i = 0; i < m_inSz; i++) {
			double scaled_val = m_inImg[i] / 255.;
			int val = cons * log(1 + scaled_val) * (255 / log(256));
			if (val < 0) { m_outImg[i] = 0; }
			else if (val > 255) { m_outImg[i] = 255; }
			else { m_outImg[i] = val; }
		}
		m_outHistImg = MakeHistImg(m_outImg);
	}
}


// ���� ���� �Լ�
void CRAWDoc::OnPixelpointprocessingGammacorrection()
{
	// TODO: Add your implementation code here.
	m_outH = m_inH;
	m_outW = m_inW;
	m_outSz = m_outH * m_outW;
	m_outImg = new unsigned char[m_outSz];

	CInputDialog dlg;
	if (dlg.DoModal() == IDOK) {
		double gamma = dlg.number;
		for (int i = 0; i < m_inSz; i++) {
			double scaled_val = m_inImg[i] / 255.;
			int val = pow(scaled_val, 1. / gamma) * 255;
			if (val < 0) { m_outImg[i] = 0; }
			else if (val > 255) { m_outImg[i] = 255; }
			else { m_outImg[i] = val; }
		}
		m_outHistImg = MakeHistImg(m_outImg);
	}

}


// �����Ͷ������̼� �Լ�
void CRAWDoc::OnPixelpointprocessingPosterization()
{
	// TODO: Add your implementation code here.
	m_outH = m_inH;
	m_outW = m_inW;
	m_outSz = m_outH * m_outW;
	m_outImg = new unsigned char[m_outSz];

	CInputDialog dlg;
	if (dlg.DoModal() == IDOK) {
		int div = 256 / (int)dlg.number;
		for (int i = 0; i < m_inSz; i++) {
			m_outImg[i] = m_inImg[i] / div * div + div / 2;
		}
		m_outHistImg = MakeHistImg(m_outImg);
	}
}


// ��Ʈ ��� ���� �Լ�
void CRAWDoc::OnPixelpointprocessingBitplaneslicing()
{
	// TODO: Add your implementation code here.
	m_outH = m_inH;
	m_outW = m_inW;
	m_outSz = m_outH * m_outW;
	m_outImg = new unsigned char[m_outSz];

	CInputDialog dlg;
	if (dlg.DoModal() == IDOK) {
		int pos = (int)dlg.number - 1;
		UCHAR mask = 0x01 << pos;
		for (int i = 0; i < m_inSz; i++) {
			UCHAR val = m_inImg[i];
			if ((val & mask) == pow(2, pos)) {
				m_outImg[i] = 255;
			}
			else { m_outImg[i] = 0; }
		}
		m_outHistImg = MakeHistImg(m_outImg);
	}
}
