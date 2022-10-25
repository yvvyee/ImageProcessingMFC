// CInputDialog.cpp : implementation file
//

#include "pch.h"
#include "RAW.h"
#include "afxdialogex.h"
#include "CInputDialog.h"


// CInputDialog dialog

IMPLEMENT_DYNAMIC(CInputDialog, CDialogEx)

CInputDialog::CInputDialog(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_INPUTDLG, pParent)
	, m_num(0)
{

}

CInputDialog::~CInputDialog()
{
}

void CInputDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_num);
	DDX_Control(pDX, IDC_CAPTION, m_cap);
	m_cap.SetWindowTextW(m_str);
}


BEGIN_MESSAGE_MAP(CInputDialog, CDialogEx)
END_MESSAGE_MAP()


// CInputDialog message handlers


BOOL CInputDialog::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  Add extra initialization here
	GetDlgItem(IDC_EDIT1)->SetWindowTextW(L"");
	GetDlgItem(IDC_EDIT1)->SetFocus();
	return FALSE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}
