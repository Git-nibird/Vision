#pragma once


// CAxisState �Ի���

class CAxisState : public CDialog
{
	DECLARE_DYNAMIC(CAxisState)

public:
	CAxisState(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CAxisState();

// �Ի�������
	enum { IDD = IDD_AxisState };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
	virtual void PostNcDestroy();
	virtual void OnCancel();
};
