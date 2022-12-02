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
}


BEGIN_MESSAGE_MAP(CFilterDlg, CDialogEx)
END_MESSAGE_MAP()


// CFilterDlg message handlers
