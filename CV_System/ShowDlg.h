#pragma once


// CShowDlg �Ի���
#include "CvvImage.h"
class CShowDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CShowDlg)

public:
	CShowDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CShowDlg();

// �Ի�������
	enum { IDD = IDD_CV_SYSTEM_FORM };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()

private:       
	CRect rect;
	CStatic* pStc; //��ʶͼ����ʾ��Picture�ؼ�
	CDC* pDC; //��Ƶ��ʾ�ؼ��豸������
	HDC hDC; //��Ƶ��ʾ�ؼ��豸���
	CvCapture* capture; //��Ƶ��ȡ�ṹ
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedStart();
};
