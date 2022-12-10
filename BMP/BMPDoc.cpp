
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
	CDib& ref = this->m_Dib;

	register int i, j, k, l, m, n;
	int w = ref.GetWidth();
	int h = ref.GetHeight();

	// 1. 첫번째 프레임은 열려있는 상태, 두번째 프레임을 열고 화면에 출력한다.
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

		// 2. MacroBlock (MB), SearchRange (SR), Residual 버퍼를 생성한다.
		int mb_sz = 16;
		int margin = 7;
		int sr_sz = margin * 2 + mb_sz;
		DOUBLE** macro_block	= Alloc2DMem(mb_sz, mb_sz);
		DOUBLE** search_range	= Alloc2DMem(sr_sz, sr_sz);
		DOUBLE** residual		= Alloc2DMem(h, w);
		
		// 3. RGB 포맷의 두 프레임을 YUV 포맷으로 변환한다. 
		DOUBLE*** refYUV = RGB2YUV(ref);
		DOUBLE*** curYUV = RGB2YUV(cur);
		
		// ReferenceFrame 의 Y 값을 마진 버퍼에 복사
		DOUBLE** refLumaWithMargin = Alloc2DMem(h + margin * 2, w + margin * 2);
		for (i = 0; i < h; i++)
		{
			for (j = 0; j < w; j++)
			{
				refLumaWithMargin[i + margin][j + margin] = refYUV[0][i][j];
			}
		}
			
		// 4. Motion Estimation 처리 시작
		for (i = 0; i < h - mb_sz; i += mb_sz) // 프레임의 해상도 h, w 에 대응하는 2중 루프부터 시작한다.
		{
			for (j = 0; j < w - mb_sz; j += mb_sz)
			{
				for (k = 0; k < sr_sz; k++) // SR 의 크기에 대응하는 2중 루프
				{
					for (l = 0; l < sr_sz; l++)
					{
						// 첫번째 프레임 = 참조 프레임 = ReferenceFrame 에서 탐색 영역 = SearchRange 값을 복사한다.
						search_range[k][l] = refLumaWithMargin[i + k][j + l];
					}
				}

				for (k = 0; k < mb_sz; k++) // MB 의 크기에 대응하는 2중 루프
				{
					for (l = 0; l < mb_sz; l++)
					{
						// 두번째 프레임 = 현재 프레임 = CurrentFrame 에서 매크로 블록 = MacroBlock 값을 복사한다.
						macro_block[k][l] = curYUV[0][i + k][j + l];
					}
				}
					
				// 먼저, 가장 작은 sad 값, 위치 좌표를 기억할 변수들 선언
				int best_k = 0;
				int best_l = 0;
				DOUBLE best_sad = DBL_MAX;
				DOUBLE rv = 0.;
				DOUBLE cv = 0.;
				for (k = 0; k < sr_sz - mb_sz; k++) // SR 크기에 대응하는 2중 루프
				{
					for (l = 0; l < sr_sz - mb_sz; l++)
					{
						DOUBLE sad = 0.;
						for (m = 0; m < mb_sz; m++) // MB 크기에 대응하는 2중 루프
						{
							for (n = 0; n < mb_sz; n++)
							{
								rv = search_range[k + m][l + n];
								cv = macro_block[m][n];
								sad += abs(rv - cv); // SAD: Sum of Absolute Differece
							}
						}
						// SAD 가 최소인지 확인하고 정보를 저장한다.
						if (best_sad > sad)
						{
							best_sad = sad;
							best_k = k;
							best_l = l;
						}
					}
				}
					
				for (m = 0; m < mb_sz; m++) // MB 크기에 대응하는 2중 루프
				{
					for (n = 0; n < mb_sz; n++)
					{
						// SAD 가 최소가 되는 최적 위치와 매크로 블록 사이의 잔차 = Residual (Absolute Difference) 을 구한다.
						rv = search_range[best_k + m][best_l + n];
						cv = macro_block[m][n];
						residual[i + m][j + n] = abs(rv - cv);
					}
				}
			}
		}
		// Motion Estimation 종료
		
		// 5. 실수 범위의 잔차 = Residual 을 정수 범위로 스케일링하고 ReferenceFrame 의 RGB 버퍼에 복사한다.
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

		// 동적할당한 모든 버퍼를 해제한다.
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
