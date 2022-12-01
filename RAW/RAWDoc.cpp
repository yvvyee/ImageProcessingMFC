
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

	if (File.GetLength() == 128 * 128) {
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
	else if (File.GetLength() == 320 * 240) {
		m_inH = 240;
		m_inW = 320;
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


// 히스토그램 스트레칭
void CRAWDoc::OnHistogramprocessingStretching()
{
	// TODO: Add your implementation code here.
	m_outH = m_inH;
	m_outW = m_inW;
	m_outSz = m_outH * m_outW;
	m_outImg = new UCHAR[m_outSz];

	UCHAR LOW = 0, HIGH = 255;
	UCHAR MIN = m_inImg[0], MAX = m_inImg[0];
	for (int i = 0; i < m_inSz; i++) {
		if (MIN > m_inImg[i])
			MIN = m_inImg[i];
		if (MAX < m_inImg[i])
			MAX = m_inImg[i];
	}
	for (int i = 0; i < m_outSz; i++) {
		m_outImg[i] = (UCHAR)((m_inImg[i] - MIN) * HIGH / (MAX - MIN));
	}
	m_outHistImg = MakeHistImg(m_outImg);

}


// 엔드 인 탐색
void CRAWDoc::OnHistogramprocessingEndinsearch()
{
	// TODO: Add your implementation code here.
	UCHAR LOW = 0, HIGH = 255;
	double MIN = m_inImg[0], MAX = m_inImg[0];

	CInputDialog dlg;
	dlg.SetStr(L"최소값 :");
	if (dlg.DoModal() == IDOK) {
		MIN = dlg.GetNum();
	}
	else {
		return;
	}
	dlg.SetStr(L"최대값 :");
	if (dlg.DoModal() == IDOK) {
		MAX = dlg.GetNum();
	}
	else {
		return;
	}

	m_outH = m_inH;
	m_outW = m_inW;
	m_outSz = m_outH * m_outW;
	m_outImg = new UCHAR[m_outSz];
	for (int i = 0; i < m_outSz; i++) {
		m_outImg[i] = (UCHAR)((m_inImg[i] - MIN) * HIGH / (MAX - MIN));
	}
	m_outHistImg = MakeHistImg(m_outImg);
}


// 히스토그램 평활화
void CRAWDoc::OnHistogramprocessingEqualization()
{
	// TODO: Add your implementation code here.
	m_outH = m_inH;
	m_outW = m_inW;
	m_outSz = m_outH * m_outW;
	m_outImg = new UCHAR[m_outSz];
	double sum = 0., hist[256] = { 0., };
	for (int i = 0, value = 0; i < m_inSz; i++) {
		value = (int)m_inImg[i];
		hist[value]++;
	}
	for (int i = 0; i < 256; i++) {
		sum += hist[i];
		hist[i] = sum;
	}
	for (int i = 0; i < m_outSz; i++) {
		UCHAR temp = m_inImg[i];
		m_outImg[i] = (UCHAR)(hist[temp] / m_outSz * 255);
	}
	m_outHistImg = MakeHistImg(m_outImg);
}


// 히스토그램 명세화
void CRAWDoc::OnHistogramprocessingSpecification()
{
	// TODO: Add your implementation code here.
	// 첫 번째 이미지에 대해 equalization 수행
	this->OnHistogramprocessingEqualization();

	// 두 번째 이미지 열기
	CFile File;
	CFileDialog OpenDlg(TRUE);
	UCHAR* tempImg = new UCHAR[m_inSz];
	if (OpenDlg.DoModal() == IDOK) {
		File.Open(OpenDlg.GetPathName(), CFile::modeRead);
		if (File.GetLength() == (unsigned)m_inSz) {
			File.Read(tempImg, m_inSz);
			File.Close();
		}
		else {
			AfxMessageBox(L"Image size not matched");
			return;
		}
	}
	// 두번째 이미지 히스토그램 계산
	int hist[256] = { 0, };
	for (int i = 0, val = 0; i < m_inSz; i++) {
		val = (int)tempImg[i];
		hist[val]++;
	}
	// 누적히스토그램 계산
	double sum = 0.;
	for (int i = 0; i < 256; i++) {
		sum += hist[i];
		hist[i] = sum;
	}
	// 누적히스토그램 정규화
	double min = hist[0], max = hist[255];
	for (int i = 0; i < 256; i++) {
		sum = hist[i];
		hist[i] = (UCHAR)((sum - min) / (max - min) * 255);
	}
	// 룩업테이블 생성
	int top = 255;
	int bottom = top - 1;
	UCHAR LUT[256] = { 0, };
	while (TRUE)
	{
		for (int i = hist[bottom]; i < hist[top]; i++)
		{
			LUT[i] = top;
		}
		top = bottom;
		bottom = bottom - 1;
		if (bottom <= -1)
		{
			break;
		}
	}
	// 첫번째 이미지의 inverse equalization 수행
	for (int i = 0; i < m_outSz; i++) {
		int val = (int)m_outImg[i];
		m_outImg[i] = LUT[val];
	}
	m_outHistImg = MakeHistImg(m_outImg);
}


// 3x3 크기의 컨볼루션 마스크를 사용한 엠보싱 함수
void CRAWDoc::OnRegionprocessingConvolution()
{
	// TODO: Add your implementation code here.
	
	// 3가지 종류의 마스크 생성
	double mask[3][3][3] = {
		{{ 0., 0., 0. },	// 1. 가운데 값만 1
		 { 0., 1., 0. },
		 { 0., 0., 0. }},

		{{ -1., 0., 0. },	// 2. 마스크의 합이 0
		 {  0., 0., 0. },
		 {  0., 0., 1. }},

		{{ 1.,  1., 1. },	// 3. 마스크의 합이 0
		 { 1., -8., 1. },
		 { 1.,  1., 1. }}
	};

	// 마스크 선택
	int idx = 0;
	CInputDialog dlg;
	dlg.SetStr(L"마스크 번호(1~3) :");
	if (dlg.DoModal() == IDOK) {
		idx = (int)dlg.GetNum() - 1;
		if (idx < 0 || idx > 3) {
			AfxMessageBox(L"Incorrect Mask Number");
			return;
		}
	}
	else {
		return;
	}

	// 컨볼루션 연산 수행
	double** inImgTemp = ConvolutionProcess(m_inImg, mask[idx]);

	// 원본 영상의 평균값을 계산
	int SUM = 0;
	for (int i = 0; i < m_inSz; i++) {
		SUM += (int)m_inImg[i];
	}
	int AVG = (int)(SUM / m_inSz);

	// 결과 이미지 버퍼에 저장
	m_outH = m_inH;
	m_outW = m_inW;
	m_outSz = m_outH * m_outW;
	m_outImg = new UCHAR[m_outSz];

	for (int i = 0; i < m_outH; i++) {
		for (int j = 0; j < m_outW; j++) {
			int val = inImgTemp[i][j] + AVG;
			if (val > 255) {
				m_outImg[i * m_outW + j] = 255;
			}
			else if (val < 0) {
				m_outImg[i * m_outW + j] = 0;
			}
			else {
				m_outImg[i * m_outW + j] = val;
			}
		}
	} // 컨볼루션 후 평균 값을 더한 결과가 0 ~ 255 범위 내 값이 되도록 조정 (clipping)
	m_outHistImg = MakeHistImg(m_outImg);
}

// 3x3 크기의 컨볼루션 연산을 수행하는 함수
double** CRAWDoc::ConvolutionProcess(UCHAR* img, double mask[3][3])
{
	// 2차원 이미지 버퍼 선언
	double** inTemp = ImageBuffer2D(m_inH + 2, m_inW + 2);
	double** outTemp = ImageBuffer2D(m_inH, m_inW);

	// 입력 이미지를 2차원 버퍼에 저장
	for (int i = 0; i < m_inH; i++) {
		for (int j = 0; j < m_inW; j++) {
			inTemp[i][j] = (double)m_inImg[i * m_inW + j];
		}
	}

	// 컨볼루션 연산
	double SUM = 0.;
	// 전체 이미지에 대해 슬라이딩 윈도 방식으로 수행
	for (int i = 0; i < m_inH; i++) {
		for (int j = 0; j < m_inW; j++) {
			// 마스크의 크기만큼 요소별 곱의 전체 합을 계산
			for (int n = 0; n < 3; n++) {
				for (int m = 0; m < 3; m++) {
					SUM += mask[n][m] * inTemp[i + n][j + m];
				}
			}
			outTemp[i][j] = SUM;	// 전체합을 출력 이미지의 화소 위치에 저장
			SUM = 0.;				// 변수 초기화
		}
	}
	delete[] inTemp; // 입력 이미지 버퍼는 해제
	return outTemp;
}

// 2차원 이미지 배열을 할당 후 반환
double** CRAWDoc::ImageBuffer2D(int height, int width)
{
	// 이미지의 세로 길이만큼 동적 할당
	double** temp = new double* [height];
	for (int i = 0; i < height; i++) {
		// 이미지의 가로 길이만큼 동적 할당
		temp[i] = new double[width];
	}
	// 할당된 버퍼를 초기화
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			temp[i][j] = 0.;
		}
	}
	return temp;
}

// 두 이미지를 합성하는 함수
void CRAWDoc::OnPixelpointprocessingDissolve() {
	// 두 번째 이미지 열기
	CFile File;
	CFileDialog OpenDlg(TRUE);
	UCHAR* tempImg = new UCHAR[m_inSz];
	if (OpenDlg.DoModal() == IDOK) {
		File.Open(OpenDlg.GetPathName(), CFile::modeRead);
		if (File.GetLength() == (unsigned)m_inSz) {
			File.Read(tempImg, m_inSz);
			File.Close();
		}
		else {
			AfxMessageBox(L"Image size not matched");
			return;
		}
	}

	// 알파 값 입력
	CInputDialog dlg;
	if (dlg.DoModal() == IDOK) {
		m_outH = m_inH;
		m_outW = m_inW;
		m_outSz = m_outH * m_outW;
		m_outImg = new UCHAR[m_outSz];

		double alpha = dlg.GetNum();
		for (int i = 0; i < m_inSz; i++) {
			UCHAR val = (UCHAR)(alpha * m_inImg[i] 
				+ (1 - alpha) * tempImg[i]);
			m_outImg[i] = val;
		}
		m_outHistImg = MakeHistImg(m_outImg);
	}
}
