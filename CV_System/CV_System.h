
// CV_System.h : CV_System Ӧ�ó������ͷ�ļ�
//
#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"       // ������

#include "CvvImage.h"
// CCV_SystemApp:
// �йش����ʵ�֣������ CV_System.cpp
//

class CCV_SystemApp : public CWinAppEx
{
public:
	CCV_SystemApp();


// ��д
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// ʵ��
	UINT  m_nAppLook;
	BOOL  m_bHiColorIcons;
	UINT  m_nX;
	UINT  m_nY;
	UINT  m_nHeight;
	UINT  m_nWideth;
	virtual void PreLoadState();
	virtual void LoadCustomState();
	virtual void SaveCustomState();

//	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()

	afx_msg void OnAbout();
};

extern CCV_SystemApp theApp;
