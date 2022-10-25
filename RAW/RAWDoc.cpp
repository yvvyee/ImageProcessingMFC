
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

UCHAR* CRAWDoc::MakeHistImg(UCHAR* img, bool isOut)
{
	int histSz = 256;
	if (isOut) {
		m_outHistSz = histSz;
	}
	else {
		m_inHistSz = histSz;
	}
	
	// 빈도 조사
	int value;
	int HIST[256] = { 0, };
	int img_size = m_inH * m_inW;
	for (int i = 0; i < img_size; i++)
	{
		value = (int)img[i];
		HIST[value]++;
	}

	// 최소-최대 정규화
	double MAX = HIST[0];
	double MIN = HIST[0];
	for (int i = 0; i < histSz; i++)
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
	for (int i = 0; i < histSz; i++)
	{
		HIST[i] = (unsigned char)
			((HIST[i] - MIN) / DIF * 255);
	}
	// 히스토그램 이미지화
	int histImgSz = (histSz * histSz)
		+ (histSz * 20);
	UCHAR* histoImg = new UCHAR[histImgSz];

	// 흰색으로 초기화
	for (int i = 0; i < histImgSz; i++)
	{
		histoImg[i] = 255;
	}

	// 히스토그램 막대부분만 검은색 칠함
	for (int i = 0; i < histSz; i++)
	{
		for (int j = 0; j < HIST[i]; j++)
		{
			histoImg[histSz * (histSz - j - 1) + i] = 0;
		}
	}
	// 히스토그램 하단 막대 표시
	for (int i = histSz + 5; i < histSz + 20; i++)
	{
		for (int j = 0; j < histSz; j++)
		{
			histoImg[histSz * i + j] = j;
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

	m_inHistImg = MakeHistImg(m_inImg, false);
	return TRUE;
}


// 기존 화소에 상수 값을 더하는 함수
void CRAWDoc::OnPixelpointprocessingAdd()
{
	// TODO: Add your implementation code here.
	m_outH = m_inH;
	m_outW = m_inW;
	m_outSz = m_outH * m_outW;
	m_outImg = new UCHAR[m_outSz];
	CInputDialog dlg;
	if (dlg.DoModal() == IDOK) {
		int num = (int)dlg.GetNum();
		for (int i = 0; i < m_inSz; i++) {
			int val = m_inImg[i] + num;
			if (val > 255) {
				// wrapping 방식
				val = val % 256;
			}
			m_outImg[i] = val;
		}
		m_outHistImg = MakeHistImg(m_outImg);
	}
}


// 기존 화소에서 상수 값을 빼는 함수
void CRAWDoc::OnPixelpointprocessingSub()
{
	// TODO: Add your implementation code here.
	CInputDialog dlg;
	if (dlg.DoModal() == IDOK) 
	{
		m_outH = m_inH;
		m_outW = m_inW;
		m_outSz = m_outH * m_outW;
		m_outImg = new UCHAR[m_outSz];

		int num = (int)dlg.GetNum();
		for (int i = 0; i < m_inSz; i++) {
			int val = m_inImg[i] - num;
			if (val < 0) {
				// Clamping 방식
				val = 0;
			}
			m_outImg[i] = val;
		}
		m_outHistImg = MakeHistImg(m_outImg);
	}
}


// 화소 반전 함수
void CRAWDoc::OnPixelpointprocessingInverse()
{
	// TODO: Add your implementation code here.
	m_outH = m_inH;
	m_outW = m_inW;
	m_outSz = m_outH * m_outW;
	m_outImg = new UCHAR[m_outSz];

	for (int i = 0; i < m_inSz; i++) {
		m_outImg[i] = 255 - m_inImg[i];
	}
	m_outHistImg = MakeHistImg(m_outImg);

}


// 화소 이진화 함수
void CRAWDoc::OnPixelpointprocessingBinarization()
{
	// TODO: Add your implementation code here.
	CInputDialog dlg;
	dlg.SetStr(L"Threshold 값 :");
	if (dlg.DoModal() == IDOK) {
		m_outH = m_inH;
		m_outW = m_inW;
		m_outSz = m_outH * m_outW;
		m_outImg = new UCHAR[m_outSz];

		int threshold = (int)dlg.GetNum();
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


// 로그 변환 함수
void CRAWDoc::OnPixelpointprocessingLogtransform()
{
	// TODO: Add your implementation code here.
	CInputDialog dlg;
	dlg.SetStr(L"상수 값 :");
	if (dlg.DoModal() == IDOK) {
		m_outH = m_inH;
		m_outW = m_inW;
		m_outSz = m_outH * m_outW;
		m_outImg = new UCHAR[m_outSz];

		double cons = dlg.GetNum();
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


// 감마 보정 함수
void CRAWDoc::OnPixelpointprocessingGammacorrection()
{
	// TODO: Add your implementation code here.
	CInputDialog dlg;
	dlg.SetStr(L"감마 값 :");
	if (dlg.DoModal() == IDOK) {
		m_outH = m_inH;
		m_outW = m_inW;
		m_outSz = m_outH * m_outW;
		m_outImg = new UCHAR[m_outSz];

		double gamma = dlg.GetNum();
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


// 포스터라이제이션 함수
void CRAWDoc::OnPixelpointprocessingPosterization()
{
	// TODO: Add your implementation code here.
	CInputDialog dlg;
	dlg.SetStr(L"양자화 수 :");
	if (dlg.DoModal() == IDOK) {
		m_outH = m_inH;
		m_outW = m_inW;
		m_outSz = m_outH * m_outW;
		m_outImg = new UCHAR[m_outSz];

		int div = 256 / (int)dlg.GetNum();
		for (int i = 0; i < m_inSz; i++) {
			m_outImg[i] = m_inImg[i] / div * div + div / 2;
		}
		m_outHistImg = MakeHistImg(m_outImg);
	}
}


// 비트 평면 분할 함수
void CRAWDoc::OnPixelpointprocessingBitplaneslicing()
{
	// TODO: Add your implementation code here.
	CInputDialog dlg;
	dlg.SetStr(L"비트번호(1~8) :");
	if (dlg.DoModal() == IDOK) {
		m_outH = m_inH;
		m_outW = m_inW;
		m_outSz = m_outH * m_outW;
		m_outImg = new UCHAR[m_outSz];

		int pos = (int)dlg.GetNum() - 1;
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


// 비선형 명암 대비 변환 함수
void CRAWDoc::OnPixelpointprocessingContraststretching()
{
	// TODO: Add your implementation code here.
	CInputDialog dlg;
	double r1 = 0;
	double r2 = 0;
	double s1 = 0;
	double s2 = 0;

	dlg.SetStr(L"r1:");
	if (dlg.DoModal() == IDOK) { 
		r1 = dlg.GetNum(); 
	}
	else {
		return;
	}
	dlg.SetStr(L"r2:");
	if (dlg.DoModal() == IDOK) {
		r2 = dlg.GetNum();
	}
	else {
		return;
	}
	dlg.SetStr(L"s1:");
	if (dlg.DoModal() == IDOK) {
		s1 = dlg.GetNum();
	}
	else {
		return;
	}
	dlg.SetStr(L"s2:");
	if (dlg.DoModal() == IDOK) {
		s2 = dlg.GetNum();
	}
	else {
		return;
	}

	double alpha = s1 / r1;
	double beta = (s2 - s1) / (r2 - r1);
	double gamma = (255 - s2) / (255 - r2);

	m_outH = m_inH;
	m_outW = m_inW;
	m_outSz = m_outH * m_outW;
	m_outImg = new UCHAR[m_outSz];

	for (int i = 0; i < m_inSz; i++) {
		UCHAR r = m_inImg[i];
		if (0 <= r && r < r1)
		{
			m_outImg[i] = alpha * r;
		}
		if (r1 <= r && r < r2)
		{
			m_outImg[i] = beta * (r - r1) + s1;
		}
		if (r2 <= r && r < 256)
		{
			m_outImg[i] = gamma * (r - r2) + s2;
		}
	}
	m_outHistImg = MakeHistImg(m_outImg);
}
