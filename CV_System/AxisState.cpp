// AxisState.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "CV_System.h"
#include "AxisState.h"
#include "afxdialogex.h"


// CAxisState �Ի���

IMPLEMENT_DYNAMIC(CAxisState, CDialog)

CAxisState::CAxisState(CWnd* pParent /*=NULL*/)
	: CDialog(CAxisState::IDD, pParent)
	, m_axisnum(_T(""))
{

}

CAxisState::~CAxisState()
{
}

void CAxisState::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_CBString(pDX, IDC_COMBO1, m_axisnum);
}


BEGIN_MESSAGE_MAP(CAxisState, CDialog)
END_MESSAGE_MAP()


// CAxisState ��Ϣ�������


void CAxisState::PostNcDestroy()
{
	// TODO: �ڴ����ר�ô����/����û���

	CDialog::PostNcDestroy();
	delete this;
}


void CAxisState::OnCancel()
{
	// TODO: �ڴ����ר�ô����/����û���

	//CDialog::OnCancel();
	DestroyWindow();
}


BOOL CAxisState::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	((CComboBox*)GetDlgItem(IDC_COMBO1))->AddString(_T("1"));
	((CComboBox*)GetDlgItem(IDC_COMBO1))->AddString(_T("2"));
	((CComboBox*)GetDlgItem(IDC_COMBO1))->AddString(_T("3"));
	((CComboBox*)GetDlgItem(IDC_COMBO1))->AddString(_T("4"));

	((CComboBox*)GetDlgItem(IDC_COMBO1))->SetCurSel(0);
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}
