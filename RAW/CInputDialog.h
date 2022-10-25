#pragma once
#include "afxdialogex.h"


// CInputDialog dialog

class CInputDialog : public CDialogEx
{
	DECLARE_DYNAMIC(CInputDialog)

public:
	CInputDialog(CWnd* pParent = nullptr);   // standard constructor
	virtual ~CInputDialog();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_INPUTDLG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	double GetNum() {
		return m_num;
	}
	void SetStr(CString str) {
		m_str = str;
	}
private:
	double m_num; // 다이얼로그에서 입력받은 숫자
	CStatic m_cap; // 캡션
	CString m_str;
public:
	virtual BOOL OnInitDialog();
};
