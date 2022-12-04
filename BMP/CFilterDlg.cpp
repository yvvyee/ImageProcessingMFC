// CFilterDlg.cpp : implementation file
//

#include "pch.h"
#include "BMP.h"
#include "afxdialogex.h"
#include "CFilterDlg.h"


// CFilterDlg dialog

IMPLEMENT_DYNAMIC(CFilterDlg, CDialogEx)

CFilterDlg::CFilterDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_FILTER, pParent)
{

}

CFilterDlg::~CFilterDlg()
{
}

void CFilterDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SPIN_KSIZE, m_csKsize);
	DDX_Control(pDX, IDC_SPIN_SIGMA, m_csSigma);
	DDX_Control(pDX, IDC_COMBO_FILTER, m_ccFilter);
	DDX_Control(pDX, IDC_EDIT_KSIZE, m_ceKsize);
	DDX_Control(pDX, IDC_EDIT_SIGMA, m_ceSigma);
}


BEGIN_MESSAGE_MAP(CFilterDlg, CDialogEx)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_KSIZE, &CFilterDlg::OnDeltaposSpinKsize)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_SIGMA, &CFilterDlg::OnDeltaposSpinSigma)
	ON_CBN_SELCHANGE(IDC_COMBO_FILTER, &CFilterDlg::OnCbnSelchangeComboFilter)
END_MESSAGE_MAP()


// CFilterDlg message handlers


void CFilterDlg::OnDeltaposSpinKsize(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: Add your control notification handler code here
	int iVal = pNMUpDown->iPos + pNMUpDown->iDelta;
	iVal = iVal <= 0 ? 0 : iVal;

	m_fSize = 3 + iVal * 2;

	CString sVal;
	sVal.Format(_T("%d\n"), m_fSize);
	m_ceKsize.SetWindowTextW(sVal);

	*pResult = 0;
}


void CFilterDlg::OnDeltaposSpinSigma(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: Add your control notification handler code here
	CString sVal;
	sVal.Format(_T("%d\n"), pNMUpDown->iPos);
	m_ceSigma.SetWindowTextW(sVal);

	if (m_fType == 1)
	{
		m_sigma = (FLOAT)pNMUpDown->iPos;
	}

	*pResult = 0;
}


void CFilterDlg::OnCbnSelchangeComboFilter()
{
	// TODO: Add your control notification handler code here
	m_fType = m_ccFilter.GetCurSel();
}


BOOL CFilterDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  Add extra initialization here
	m_ccFilter.AddString(_T("Blurring (Average Filter)"));
	m_ccFilter.AddString(_T("Gaussian Smoothing"));
	m_ccFilter.AddString(_T("Sharpening"));
	m_ccFilter.AddString(_T("High Frequency Band Pass Filter"));

	m_ccFilter.SetCurSel(0);

	m_ceKsize.SetWindowTextW(_T("3"));
	m_csKsize.SetRange(0, 3);
	m_csKsize.SetPos(0);

	m_ceSigma.SetWindowTextW(_T("1"));
	m_csSigma.SetRange(1, 10);
	m_csSigma.SetPos(0);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}
