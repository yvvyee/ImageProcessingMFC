
// BMP.h : main header file for the BMP application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'pch.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols


// CBMPApp:
// See BMP.cpp for the implementation of this class
//
class CDib; // 추가
class CBMPApp : public CWinAppEx
{
public:
	CBMPApp() noexcept;
	CDib* m_pNewDib; // 추가

// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// Implementation
	UINT  m_nAppLook;
	BOOL  m_bHiColorIcons;

	virtual void PreLoadState();
	virtual void LoadCustomState();
	virtual void SaveCustomState();

	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CBMPApp theApp;

void AfxNewImage(CDib& dib); // 추가