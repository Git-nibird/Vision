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
{

}

CAxisState::~CAxisState()
{
}

void CAxisState::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
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
