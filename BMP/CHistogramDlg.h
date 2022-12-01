#pragma once
#include "afxdialogex.h"
#include "CDib.h"

// CHistogramDlg dialog

class CHistogramDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CHistogramDlg)

public:
	CHistogramDlg(CWnd* pParent = nullptr);   // standard constructor
	virtual ~CHistogramDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_HISTOGRAM };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

public:
	bool isColor = false;
	float m_HistogramGray[256];
	float m_HistogramRGB[3][256];

	void SetImage(CDib& dib);
	void CalcHist(CDib& dib);
	void NormHist(float *src);

	afx_msg void OnPaint();
};
