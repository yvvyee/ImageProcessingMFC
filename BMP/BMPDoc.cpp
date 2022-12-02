
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
	return m_Dib.Save(lpszPathName);
}


CDib *CBMPDoc::OnColormodelRgb2yuv(CDib dib)
{
	// TODO: Add your implementation code here.
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
