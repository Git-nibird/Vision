
// CV_SystemView.cpp : CCV_SystemView ���ʵ��
//

#include "stdafx.h"
// SHARED_HANDLERS ������ʵ��Ԥ��������ͼ������ɸѡ�������
// ATL ��Ŀ�н��ж��壬�����������Ŀ�����ĵ����롣
#ifndef SHARED_HANDLERS
#include "CV_System.h"
#endif

#include "AxisState.h"
#include "MainFrm.h"

#include "CV_SystemDoc.h"
#include "CV_SystemView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CCV_SystemView

IMPLEMENT_DYNCREATE(CCV_SystemView, CFormView)

BEGIN_MESSAGE_MAP(CCV_SystemView, CFormView)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()

	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_START, &CCV_SystemView::OnBnClickedStart)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_IMWAIT, &CCV_SystemView::OnBnClickedImwait)
	ON_WM_CLOSE()
	ON_COMMAND(IDM_AxisState, &CCV_SystemView::OnAxisstate)
END_MESSAGE_MAP()


// CCV_SystemView ����/����

CCV_SystemView::CCV_SystemView()
	: CFormView(CCV_SystemView::IDD)
{
	// TODO: �ڴ˴���ӹ������
	pStc=NULL;
	pDC=NULL;
	capture=NULL;
	pAxisDlg=NULL;
}

CCV_SystemView::~CCV_SystemView()
{
}

void CCV_SystemView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
}

BOOL CCV_SystemView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: �ڴ˴�ͨ���޸�
	//  CREATESTRUCT cs ���޸Ĵ��������ʽ

	return CFormView::PreCreateWindow(cs);
}

void CCV_SystemView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();
	GetParentFrame()->RecalcLayout();
	//m_nMapMode = -1;
	ResizeParentToFit();
	pStc=(CStatic *)GetDlgItem(IDC_IMSHOW);//IDC_VIEWΪPicture�ؼ�ID
	pStc->GetClientRect(rect);//��CWind��ͻ���������㴫������
	pDC=pStc->GetDC(); //�õ�Picture�ؼ��豸������
	hDC=pDC->GetSafeHdc(); //�õ��ؼ��豸�����ĵľ�� 

}

void CCV_SystemView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CCV_SystemView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CCV_SystemView ���

#ifdef _DEBUG
void CCV_SystemView::AssertValid() const
{
	CFormView::AssertValid();
}

void CCV_SystemView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}

CCV_SystemDoc* CCV_SystemView::GetDocument() const // �ǵ��԰汾��������
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CCV_SystemDoc)));
	return (CCV_SystemDoc*)m_pDocument;
}
#endif //_DEBUG


// CCV_SystemView ��Ϣ�������

void CCV_SystemView::OnSize(UINT nType, int cx, int cy)
{
	CFormView::OnSize(nType, cx, cy);
	//MessageBox(_T("MAXIMIZED!"));
	// TODO: �ڴ˴������Ϣ����������
	//if (nType==3)
	//{
	//	MessageBox(_T("MAXIMIZED!"));
	//}
	//if (nType==SIZE_MAXIMIZED)
	//{
	//	MessageBox(_T("MAXIMIZED!"));
	//}
	//if (::IsZoomed(m_hWnd))
	//{
	//	MessageBox(_T("MAXIMIZED!"));
	//}
	//ImageShow();
}


void CCV_SystemView::OnBnClickedStart()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//MessageBox(_T("OK!"));
	ImageShow();
}

void CCV_SystemView::ImageShow()
{
	if (!capture)
	{
		capture=cvCaptureFromCAM(0);
	}
	if (!capture)
	{
		AfxMessageBox(_T("�޷��������ͷ!"));
		return;
	}
	SetTimer(1, 25, NULL);
}

void CCV_SystemView::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

	if (1==nIDEvent)
	{
		IplImage* img =0;
		img = cvQueryFrame(capture);

		CvvImage m_CvvImage;
		m_CvvImage.CopyOf(img, 1);
		m_CvvImage.DrawToHDC(hDC, &rect);
	}

	CFormView::OnTimer(nIDEvent);
}


void CCV_SystemView::OnBnClickedImwait()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	KillTimer(1);
}


void CCV_SystemView::OnClose()
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	CFormView::OnClose();
}


void CCV_SystemView::OnAxisstate()
{
	// TODO: �ڴ���������������
	pAxisDlg = new CAxisState();
	pAxisDlg->Create(IDD_AxisState, this);
	pAxisDlg->ShowWindow(SW_SHOW);
}
