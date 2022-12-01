// CHistogramDlg.cpp : implementation file
//

#include "pch.h"
#include "BMP.h"
#include "afxdialogex.h"
#include "CHistogramDlg.h"


// CHistogramDlg dialog

IMPLEMENT_DYNAMIC(CHistogramDlg, CDialogEx)

CHistogramDlg::CHistogramDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_HISTOGRAM, pParent)
{

}

CHistogramDlg::~CHistogramDlg()
{
}

void CHistogramDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CHistogramDlg, CDialogEx)
	ON_WM_PAINT()
END_MESSAGE_MAP()


// CHistogramDlg message handlers
void CHistogramDlg::SetImage(CDib& dib)
{
	float hist[256] = { 0.f, };
	
	this->CalcHist(dib);

	if (dib.GetBitCount() == 8) {	// 그레이스케일
		this->NormHist(m_HistogramGray);
	}
	if (dib.GetBitCount() == 24) {	// 컬러
		this->NormHist(m_HistogramRGB[0]);
		this->NormHist(m_HistogramRGB[1]);
		this->NormHist(m_HistogramRGB[2]);
	}
}

void CHistogramDlg::CalcHist(CDib& dib)
{
	register int i, j;

	int w = dib.GetWidth();
	int h = dib.GetHeight();

	float area = (float)w * h;

	if (dib.GetBitCount() == 8)	// gray-scale
	{
		memset(this->m_HistogramGray, 0, sizeof(float) * 256);

		BYTE** ptr = dib.GetPtr();

		for (j = 0; j < h; j++)	// count
		{
			for (i = 0; i < w; i++)
			{
				this->m_HistogramGray[ptr[j][i]]++;
			}
		}

		for (i = 0; i < 256; i++)
		{
			this->m_HistogramGray[i] /= area;
		}
	}
	else if (dib.GetBitCount() == 24) // rgb color
	{
		this->isColor = true;

		memset(this->m_HistogramRGB[0], 0, sizeof(float) * 256); // b
		memset(this->m_HistogramRGB[1], 0, sizeof(float) * 256); // g
		memset(this->m_HistogramRGB[2], 0, sizeof(float) * 256); // r

		RGBBYTE** ptr = dib.GetRGBPtr();

		for (j = 0; j < h; j++)	// count
		{
			for (i = 0; i < w; i++)
			{
				this->m_HistogramRGB[0][ptr[j][i].b]++; // b
				this->m_HistogramRGB[1][ptr[j][i].g]++; // g
				this->m_HistogramRGB[2][ptr[j][i].r]++; // r
			}
		}

		for (i = 0; i < 256; i++)
		{
			this->m_HistogramRGB[0][i] /= area;	// b
			this->m_HistogramRGB[1][i] /= area;	// g
			this->m_HistogramRGB[2][i] /= area;	// r
		}
	}
}

void CHistogramDlg::NormHist(float *src)
{
	register int i;
	float max_value = src[0];
	for (i = 1; i < 256; i++)
	{
		if (src[i] > max_value)
		{
			max_value = src[i];
		}
	}

	for (i = 0; i < 256; i++)
	{
		src[i] = src[i] * 100 / max_value;
	}
}

void CHistogramDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: Add your message handler code here
	// Do not call CDialogEx::OnPaint() for painting messages

	register int i;

	if (this->isColor) // rgb color
	{
		// B 채널 히스토그램
		dc.SelectStockObject(DC_PEN);
		dc.SetDCPenColor(RGB(255, 0, 0));
		for (i = 0; i < 256; i++)
		{
			dc.MoveTo(20 + i, 130);
			dc.LineTo(20 + i, 130 - 
				(int)this->m_HistogramRGB[0][i]);
		}

		for (i = 0; i < 256; i++)
		{
			dc.SelectStockObject(DC_PEN);
			dc.SetDCPenColor(RGB(i, 0, 0));

			dc.MoveTo(20 + i, 140);
			dc.LineTo(20 + i, 155);
		}
		// G 채널 히스토그램
		dc.SelectStockObject(DC_PEN);
		dc.SetDCPenColor(RGB(0, 255, 0));
		for (i = 0; i < 256; i++)
		{
			dc.MoveTo(20 + i, 155 + 130);
			dc.LineTo(20 + i, 155 + 130 - 
				(int)this->m_HistogramRGB[1][i]);
		}

		for (i = 0; i < 256; i++)
		{
			dc.SelectStockObject(DC_PEN);
			dc.SetDCPenColor(RGB(0, i, 0));

			dc.MoveTo(20 + i, 155 + 140);
			dc.LineTo(20 + i, 155 + 155);
		}
		// R 채널 히스토그램
		dc.SelectStockObject(DC_PEN);
		dc.SetDCPenColor(RGB(0, 0, 255));
		for (i = 0; i < 256; i++)
		{
			dc.MoveTo(20 + i, 310 + 130);
			dc.LineTo(20 + i, 310 + 130 - 
				(int)this->m_HistogramRGB[2][i]);
		}

		for (i = 0; i < 256; i++)
		{
			dc.SelectStockObject(DC_PEN);
			dc.SetDCPenColor(RGB(0, 0, i));

			dc.MoveTo(20 + i, 310 + 140);
			dc.LineTo(20 + i, 310 + 155);
		}
	}
	else	// gray-scale
	{
		for (i = 0; i < 256; i++)
		{
			dc.MoveTo(20 + i, 130);
			dc.LineTo(20 + i, 130 - 
				(int)this->m_HistogramGray[i]);
		}

		for (i = 0; i < 256; i++)
		{
			dc.SelectStockObject(DC_PEN);
			dc.SetDCPenColor(RGB(i, i, i));

			dc.MoveTo(20 + i, 140);
			dc.LineTo(20 + i, 155);
		}
	}
}
