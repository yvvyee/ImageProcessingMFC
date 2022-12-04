#pragma once
#include "afxdialogex.h"


// CFilterDlg dialog

class CFilterDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CFilterDlg)

public:
	CFilterDlg(CWnd* pParent = nullptr);   // standard constructor
	virtual ~CFilterDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_FILTER };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CSpinButtonCtrl m_csKsize;
	CSpinButtonCtrl m_csSigma;
	CComboBox m_ccFilter;
	CEdit m_ceKsize;
	CEdit m_ceSigma;
	afx_msg void OnDeltaposSpinKsize(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDeltaposSpinSigma(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnCbnSelchangeComboFilter();

	UCHAR m_fType = 0;
	UCHAR m_fSize = 3;
	DOUBLE m_sigma = 1.;
	virtual BOOL OnInitDialog();
};
