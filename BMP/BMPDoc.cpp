
// BMPDoc.cpp : implementation of the CBMPDoc class
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "BMP.h"
#endif

#include "BMPDoc.h"

#include <propkey.h>

#include "CIP.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CBMPDoc

IMPLEMENT_DYNCREATE(CBMPDoc, CDocument)

BEGIN_MESSAGE_MAP(CBMPDoc, CDocument)
END_MESSAGE_MAP()


// CBMPDoc construction/destruction

CBMPDoc::CBMPDoc() noexcept
{
	// TODO: add one-time construction code here

}

CBMPDoc::~CBMPDoc()
{
}

BOOL CBMPDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	BOOL bSuccess = TRUE;
	if (theApp.m_pNewDib != NULL) {
		m_Dib.Copy(theApp.m_pNewDib);
		theApp.m_pNewDib = NULL;
	}
	else {
		bSuccess = FALSE;
	}
	return bSuccess;
}




// CBMPDoc serialization

void CBMPDoc::Serialize(CArchive& ar)
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
void CBMPDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
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
void CBMPDoc::InitializeSearchContent()
{
	CString strSearchContent;
	// Set search contents from document's data.
	// The content parts should be separated by ";"

	// For example:  strSearchContent = _T("point;rectangle;circle;ole object;");
	SetSearchContent(strSearchContent);
}

void CBMPDoc::SetSearchContent(const CString& value)
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

// CBMPDoc diagnostics

#ifdef _DEBUG
void CBMPDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CBMPDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CBMPDoc commands


BOOL CBMPDoc::OnOpenDocument(LPCTSTR lpszPathName)
{
	if (!CDocument::OnOpenDocument(lpszPathName))
		return FALSE;

	// TODO:  Add your specialized creation code here
	m_Dib.Load(lpszPathName); // 추가

	return TRUE;
}


BOOL CBMPDoc::OnSaveDocument(LPCTSTR lpszPathName)
{
	// TODO: Add your specialized code here and/or call the base class

	// return CDocument::OnSaveDocument(lpszPathName); // 기존 코드 주석
	CFileDialog SaveDlg(FALSE, NULL, lpszPathName,
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, _T("Bitmap Image (*.BMP) |*.BMP|"));
	if (SaveDlg.DoModal() == IDOK)
	{
		lpszPathName = SaveDlg.GetPathName();
		return m_Dib.Save(lpszPathName);
	}
	return FALSE;
}


CDib *CBMPDoc::OnColormodelRgb2yuv()
{
	CDib& dib = this->m_Dib;

	if (dib.GetBitCount() == 24)
	{
		int w = dib.GetWidth();
		int h = dib.GetHeight();

		CDib* temp = new CDib[3];

		temp[0].Copy(&dib);
		temp[1].Copy(&dib);
		temp[2].Copy(&dib);

		DOUBLE*** YUV = RGB2YUV(dib);

		RGBBYTE** yPtr = temp[0].GetRGBPtr();
		RGBBYTE** uPtr = temp[1].GetRGBPtr();
		RGBBYTE** vPtr = temp[2].GetRGBPtr();

		for (register int j = 0; j < h; j++)
		{
			for (register int i = 0; i < w; i++)
			{
				yPtr[j][i].r = (BYTE)limit(YUV[0][j][i] * 255.);
				yPtr[j][i].g = (BYTE)limit(YUV[0][j][i] * 255.);
				yPtr[j][i].b = (BYTE)limit(YUV[0][j][i] * 255.);

				uPtr[j][i].r = (BYTE)limit(YUV[1][j][i] * 255.);
				uPtr[j][i].g = (BYTE)limit(YUV[1][j][i] * 255.);
				uPtr[j][i].b = (BYTE)limit(YUV[1][j][i] * 255.);

				vPtr[j][i].r = (BYTE)limit(YUV[2][j][i] * 255.);
				vPtr[j][i].g = (BYTE)limit(YUV[2][j][i] * 255.);
				vPtr[j][i].b = (BYTE)limit(YUV[2][j][i] * 255.);
			}
		}
		delete[] YUV;
		return temp;
	}
	else if (dib.GetBitCount() == 8)
	{
		AfxMessageBox(L"Cannot apply to gray-scale image!");
		return nullptr;
	}
	else
	{
		AfxMessageBox(L"Wrong Image Format!");
		return nullptr;
	}
}


void CBMPDoc::OnFrameprocessingMotionestimation()
{
	// ref 는 현재 열린 프레임
	CDib& ref = this->m_Dib;

	register int i, j, k, l, m, n;
	int w = ref.GetWidth();
	int h = ref.GetHeight();

	// cur 는 2번째로 여는 프레임
	if (ref.GetBitCount() == 24)
	{
		CDib cur;
		CFileDialog OpenDlg(TRUE);
		if (OpenDlg.DoModal() == IDOK)
		{
			cur.Load(OpenDlg.GetPathName());
			if (cur.GetBitCount()	!= 24 ||
				cur.GetWidth()		!= w  ||
				cur.GetHeight()		!= h) {
				AfxMessageBox(L"Images not matched");
				return;
			}
		}
		AfxNewImage(cur);

		int mb_sz = 16;										// 매크로블록(MB) 의 크기는 16 (정사각형)
		int margin = 7;										// margin = 7, MB 의 상하좌우로 커지는 크기
		int sr_sz = margin * 2 + mb_sz;						// 탐색영역(SR) 의 크기는 30 (정사각형)
		DOUBLE** macro_block	= Alloc2DMem(mb_sz, mb_sz);	// MB 버퍼 동적할당
		DOUBLE** search_range	= Alloc2DMem(sr_sz, sr_sz);	// SR 버퍼 동적할당
		DOUBLE** residual		= Alloc2DMem(h, w);			// residual 버퍼 동적할당
		
		// 2개의 프레임을 YUV 포맷으로 변환
		DOUBLE*** refYUV = RGB2YUV(ref);
		DOUBLE*** curYUV = RGB2YUV(cur);
		
		// 프레임의 크기 (w * h) 가 상하좌우 margin 만큼 큰 버퍼를 동적할당
		DOUBLE** refLumaWithMargin = Alloc2DMem(h + margin * 2, w + margin * 2);

		// margin 크기만큼 키운 버퍼에 ref 의 Y 채널 값을 복사 (가운데 위치해야함)
		for (i = 0; i < h; i++) {
			for (j = 0; j < w; j++) {
				refLumaWithMargin[i + margin][j + margin] = refYUV[0][i][j];
			}
		}
			
		// Motion Estimation 시작: 프레임의 크기 (h, w) 만큼 반복문 순회 
		// (MB 가 서로 겹치지 않게 주의)
		// (MB 크기를 고려하여 반복문 종료 시점 설정해야 함)
		for (i = 0; i < h - mb_sz; i += mb_sz)
		{
			for (j = 0; j < w - mb_sz; j += mb_sz)
			{
				// margin 이 포함된 ref 의 Y 버퍼에서 SR 을 복사
				for (k = 0; k < sr_sz; k++) {
					for (l = 0; l < sr_sz; l++) {
						search_range[k][l] = refLumaWithMargin[i + k][j + l];
					}
				}

				// cur 의 Y 버퍼에서 MB 을 복사
				for (k = 0; k < mb_sz; k++) {
					for (l = 0; l < mb_sz; l++) {
						macro_block[k][l] = curYUV[0][i + k][j + l];
					}
				}
					
				// SR에서 오차가 가장 적은 위치의 가로, 세로 좌표를 기억할 변수 선언
				int best_k = 0;
				int best_l = 0;

				// 최적의 SAD 값을 저장할 변수 선언, 초기값은 충분히 큰 값이어야 함
				DOUBLE best_sad = DBL_MAX;


				DOUBLE rv = 0.;
				DOUBLE cv = 0.;

				// SR 크기 만큼 반복문 순회
				for (k = 0; k < sr_sz - mb_sz; k++) {
					for (l = 0; l < sr_sz - mb_sz; l++) {

						DOUBLE sad = 0.;

						// MB 크기 만큼 반복문 순회
						for (m = 0; m < mb_sz; m++)	{
							for (n = 0; n < mb_sz; n++) {

								// SAD: Sum of Absolute Differece 계산
								rv = search_range[k + m][l + n];
								cv = macro_block[m][n];
							}
						}

						// SAD 가 최소인지 확인하고 최적 SAD 값 갱신
						if (best_sad > sad) {
							best_sad = sad;

							// 최적 SAD 값을 갖는 SR 의 위치 좌표 갱신
							best_k = k;
							best_l = l;
						}
					}
				}
				
				// MB 크기만큼 반복문 순회
				for (m = 0; m < mb_sz; m++)  {
					for (n = 0; n < mb_sz; n++) {
						
						// SAD 최적인 SR 과 MB 간에 Absolute Difference 를 구해 residual 에 저장
						rv = search_range[best_k + m][best_l + n];
						cv = macro_block[m][n];
						residual[i + m][j + n] = abs(rv - cv);
					}
				}
			}
		}
		// Motion Estimation 종료
		
		// 실수 범위의 residual 을 정수 범위로 스케일링하고 ref 의 RGB 버퍼에 복사
		CDib cur_res;
		cur_res.Copy(&cur);
		RGBBYTE** ptr = cur_res.GetRGBPtr();
		for (i = 0; i < h; i++) {
			for (j = 0; j < w; j++) {
				ptr[i][j].r = (BYTE)(residual[i][j] * 255.);
				ptr[i][j].g = (BYTE)(residual[i][j] * 255.);
				ptr[i][j].b = (BYTE)(residual[i][j] * 255.);
			}
		}

		// 화면에 표시
		AfxNewImage(cur_res);

		// 동적할당한 모든 버퍼를 해제
		delete[] macro_block;
		delete[] search_range;
		delete[] residual;
		delete[] refYUV;
		delete[] curYUV;
		delete[] refLumaWithMargin;
	}
	else if (ref.GetBitCount() == 8)
	{
		AfxMessageBox(L"Cannot apply to gray-scale image!");
		return;
	}
	else
	{
		AfxMessageBox(L"Wrong Image Format!");
		return;
	}
}


void CBMPDoc::OnFrameprocessingMotioncompensation()
{
	// TODO: Add your implementation code here.
}


void CBMPDoc::OnFrameprocessingSimpledifference()
{
	CDib& ref = this->m_Dib;

	register int i, j, k, l, m, n;
	int w = ref.GetWidth();
	int h = ref.GetHeight();

	if (ref.GetBitCount() == 24)
	{
		CDib cur;
		CFileDialog OpenDlg(TRUE);
		if (OpenDlg.DoModal() == IDOK)
		{
			cur.Load(OpenDlg.GetPathName());
			if (cur.GetBitCount() != 24 ||
				cur.GetWidth() != w ||
				cur.GetHeight() != h) {
				AfxMessageBox(L"Images not matched");
				return;
			}
		}
		AfxNewImage(cur);

		DOUBLE** residual = Alloc2DMem(h, w);
		DOUBLE*** refYUV = RGB2YUV(ref);
		DOUBLE*** curYUV = RGB2YUV(cur);

		for (i = 0; i < h; i++)
		{
			for (j = 0; j < w; j++)
			{
				DOUBLE rv = refYUV[0][i][j];
				DOUBLE cv = curYUV[0][i][j];
				residual[i][j] = abs(rv - cv);
			}
		}

		CDib cur_res;
		cur_res.Copy(&cur);

		RGBBYTE** ptr = cur_res.GetRGBPtr();
		for (i = 0; i < h; i++)
		{
			for (j = 0; j < w; j++)
			{
				ptr[i][j].r = (BYTE)(residual[i][j] * 255.);
				ptr[i][j].g = (BYTE)(residual[i][j] * 255.);
				ptr[i][j].b = (BYTE)(residual[i][j] * 255.);
			}
		}
		AfxNewImage(cur_res);

		delete[] residual;
		delete[] refYUV;
		delete[] curYUV;
	}
	else if (ref.GetBitCount() == 8)
	{
		AfxMessageBox(L"Cannot apply to gray-scale image!");
		return;
	}
	else
	{
		AfxMessageBox(L"Wrong Image Format!");
		return;
	}
}
