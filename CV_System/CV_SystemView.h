
// CV_SystemView.h : CCV_SystemView ��Ľӿ�
//

#pragma once

#include "resource.h"

#include "AxisState.h"

class CCV_SystemView : public CFormView
{
protected: // �������л�����
	CCV_SystemView();
	DECLARE_DYNCREATE(CCV_SystemView)

public:
	enum{ IDD = IDD_CV_SYSTEM_FORM };

// ����
public:
	CCV_SystemDoc* GetDocument() const;

// ����
public:

// ��д
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	virtual void OnInitialUpdate(); // ������һ�ε���

// ʵ��
public:
	virtual ~CCV_SystemView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ���ɵ���Ϣӳ�亯��
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnBnClickedStart();
private:
	CRect rect;
	CStatic* pStc; //��ʶͼ����ʾ��Picture�ؼ�
	CDC* pDC; //��Ƶ��ʾ�ؼ��豸������
	HDC hDC; //��Ƶ��ʾ�ؼ��豸���
public:
	CvCapture* capture; //��Ƶ��ȡ�ṹ
	CAxisState* pAxisDlg;
public:
	void ImageShow();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedImwait();
	afx_msg void OnClose();
	afx_msg void OnAxisstate();
};

#ifndef _DEBUG  // CV_SystemView.cpp �еĵ��԰汾
inline CCV_SystemDoc* CCV_SystemView::GetDocument() const
   { return reinterpret_cast<CCV_SystemDoc*>(m_pDocument); }
#endif

