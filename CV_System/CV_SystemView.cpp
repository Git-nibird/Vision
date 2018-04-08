
// CV_SystemView.cpp : CCV_SystemView ���ʵ��
//

#include "stdafx.h"
// SHARED_HANDLERS ������ʵ��Ԥ��������ͼ������ɸѡ�������
// ATL ��Ŀ�н��ж��壬�����������Ŀ�����ĵ����롣
#ifndef SHARED_HANDLERS
#include "CV_System.h"


#include "AxisState.h"
#include "MainFrm.h"
#include "CameraDS.h"

#include "CV_SystemDoc.h"
#include "CV_SystemView.h"
#include "ImageJudge.h"
#endif

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
	ON_BN_CLICKED(IDC_SAVEIMG, &CCV_SystemView::OnClickedSaveimg)
	ON_BN_CLICKED(IDC_CHOOSEREF, &CCV_SystemView::OnClickedChooseref)
	ON_COMMAND(ID_MSE, &CCV_SystemView::OnMse)
	ON_COMMAND(ID_PSNR, &CCV_SystemView::OnPsnr)
	ON_COMMAND(ID_SSIM, &CCV_SystemView::OnSsim)
	ON_COMMAND(ID_CV, &CCV_SystemView::OnCv)
	ON_COMMAND(ID_NR, &CCV_SystemView::OnNr)
	ON_COMMAND(ID_HELP, &CCV_SystemView::OnHelp)
END_MESSAGE_MAP()


// CCV_SystemView ����/����

CCV_SystemView::CCV_SystemView()
	: CFormView(CCV_SystemView::IDD)
{
	// TODO: �ڴ˴���ӹ������
	pStc=NULL;
	pDC=NULL;
	img = NULL;
	cap = NULL;
	pAxisDlg=NULL;
}

CCV_SystemView::~CCV_SystemView()
{
	m_CvvImage.Destroy();
	pAxisDlg = NULL;
}

void CCV_SystemView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO1, m_CBNCamList);
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


	m_nCamCount = CCameraDS::CameraCount();
	if( m_nCamCount < 1 )
	{
		AfxMessageBox(_T("���������1������ͷ��"));
		//return -1;
	}
	// ����Ͽ�CamList���������ͷ���Ƶ��ַ���
	char camera_name[1024];
	char istr[25];
	CString camstr;
	for(int i=0; i < m_nCamCount; i++)
	{  
		int retval = CCameraDS::CameraName(i, camera_name, sizeof(camera_name) );

		sprintf_s(istr, " # %d", i);
		strcat_s( camera_name, istr );  
		camstr = camera_name;
		if(retval >0)
			m_CBNCamList.AddString(camstr);
		else
			AfxMessageBox(_T("���ܻ�ȡ����ͷ������"));
	}
	camstr.ReleaseBuffer();
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

//void CCV_SystemView::OnSize(UINT nType, int cx, int cy)
//{
//	CFormView::OnSize(nType, cx, cy);
//	//MessageBox(_T("MAXIMIZED!"));
//	// TODO: �ڴ˴������Ϣ����������
//	//if (nType==3)
//	//{
//	//	MessageBox(_T("MAXIMIZED!"));
//	//}
//	//if (nType==SIZE_MAXIMIZED)
//	//{
//	//	MessageBox(_T("MAXIMIZED!"));
//	//}
//	//if (::IsZoomed(m_hWnd))
//	//{
//	//	MessageBox(_T("MAXIMIZED!"));
//	//}
//	//ImageShow();
//}


void CCV_SystemView::OnBnClickedStart()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//MessageBox(_T("OK!"));
	ImageShow();
}

void CCV_SystemView::ImageShow()
{
	if (!cap.isOpened())
	{
		cap.open(0);
	}
	if (!cap.isOpened())
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
		cap >> frame;
		img = &IplImage(frame);
		//CFrameWnd * pFwnd = GetParentFrame();
		//pFwnd->GetMenu()->GetSubMenu(0)->EnableMenuItem(1, MF_BYPOSITION | MF_DISABLED | MF_GRAYED);
		theApp.GetMainWnd()->GetMenu()->GetSubMenu(4)->EnableMenuItem(1, MF_BYPOSITION | MF_DISABLED | MF_GRAYED);
		//CvvImage m_CvvImage;
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


void CCV_SystemView::OnClickedSaveimg()
{
	CString strFilter = _T("*.bmp|*.bmp|*.jpg|*.jpg|*.png|*.png||");
	CFileDialog dlg(FALSE, _T("bmp"), _T("save.bmp"), NULL, strFilter);
	if (dlg.DoModal() != IDOK)
		return;

	CString strFileName;
	CString strExtension;
	strFileName = dlg.m_ofn.lpstrFile;
	strExtension = dlg.m_ofn.lpstrFilter;
	USES_CONVERSION;
	std::string ss(W2A(strFileName)); //under UNICODE, use the WideCharToMutilByte
	cv::Mat saveImg = frame;
	cv::imwrite(ss, saveImg);
}


void CCV_SystemView::OnClickedChooseref()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	CString strFilter = _T("*.bmp|*.bmp|*.jpg|*.jpg|*.png|*.png||");
	CFileDialog dlg(TRUE, _T("bmp"), _T("ref.bmp"), NULL, strFilter);
	if (dlg.DoModal() != IDOK)
		return;

	CString strFileName;
	CString strExtension;
	strFileName = dlg.m_ofn.lpstrFile;
	strExtension = dlg.m_ofn.lpstrFilter;
	USES_CONVERSION;
	std::string ss(W2A(strFileName)); //under UNICODE, use the WideCharToMutilByte
	cv::Mat openImg;
	openImg = cv::imread(ss);
	if (openImg.empty())
	{
		MessageBox(_T("����ο�ͼ�����������ѡ��"));
		return;
	}
	ref = openImg;
}


void CCV_SystemView::OnMse()
{
	// TODO:  �ڴ���������������
	if (ref.empty())
	{
		MessageBox(_T("����ѡ��ο�ͼ��"));
		return;
	}
	if (frame.empty())
	{
		MessageBox(_T("����ͼ����Ϊ�գ�"));
		return;
	}
	if (frame.size() != ref.size())
	{
		MessageBox(_T("�ο�ͼ������Դ�СҪһ��"));
		return;
	}
	cv::Mat inputRef, inputFrame;
	cv::cvtColor(frame, inputFrame, CV_BGR2GRAY);
	cv::cvtColor(ref, inputRef, CV_BGR2GRAY);
	double Mse = getMSE(inputFrame, inputRef);
	CString strResult;
	strResult.Format(_T("%f"), Mse);
	MessageBox(_T("�������Ϊ��"+strResult));
}


void CCV_SystemView::OnPsnr()
{
	// TODO:  �ڴ���������������
	if (ref.empty())
	{
		MessageBox(_T("����ѡ��ο�ͼ��"));
		return;
	}
	if (frame.empty())
	{
		MessageBox(_T("����ͼ����Ϊ�գ�"));
		return;
	}
	if (frame.size() != ref.size())
	{
		MessageBox(_T("�ο�ͼ������Դ�СҪһ��"));
		return;
	}
	cv::Mat inputRef, inputFrame;
	cv::cvtColor(frame, inputFrame, CV_BGR2GRAY);
	cv::cvtColor(ref, inputRef, CV_BGR2GRAY);
	double psnr = getPSNR(inputFrame, inputRef);
	CString strResult;
	strResult.Format(_T("%f"), psnr);
	MessageBox(_T("��ֵ�����Ϊ��" + strResult));
}


void CCV_SystemView::OnSsim()
{
	// TODO:  �ڴ���������������
	if (ref.empty())
	{
		MessageBox(_T("����ѡ��ο�ͼ��"));
		return;
	}
	if (frame.empty())
	{
		MessageBox(_T("����ͼ����Ϊ�գ�"));
		return;
	}
	if (frame.size() != ref.size())
	{
		MessageBox(_T("�ο�ͼ������Դ�СҪһ��"));
		return;
	}
	cv::Mat inputRef, inputFrame;
	cv::cvtColor(frame, inputFrame, CV_BGR2GRAY);
	cv::cvtColor(ref, inputRef, CV_BGR2GRAY);
	double ssim = getSSIM(inputFrame, inputRef);
	CString strResult;
	strResult.Format(_T("%f"), ssim);
	MessageBox(_T("�ṹ���ƶ�Ϊ��" + strResult));
}


void CCV_SystemView::OnCv()
{
	// TODO:  �ڴ���������������
	if (frame.empty())
	{
		MessageBox(_T("����ͼ����Ϊ�գ�"));
		return;
	}
	cv::Mat inputFrame;
	cv::cvtColor(frame, inputFrame, CV_BGR2GRAY);
	double CV = getCV(inputFrame);
	CString strResult;
	strResult.Format(_T("%f"), CV);
	MessageBox(_T("��Եǿ��Ϊ��" + strResult));
}


void CCV_SystemView::OnNr()
{
	// TODO:  �ڴ���������������
	if (frame.empty())
	{
		MessageBox(_T("����ͼ����Ϊ�գ�"));
		return;
	}
	cv::Mat inputFrame;
	cv::cvtColor(frame, inputFrame, CV_BGR2GRAY);
	double nr = getNR(inputFrame);
	CString strResult;
	strResult.Format(_T("%f"), nr);
	MessageBox(_T("������Ϊ��" + strResult));
}


void CCV_SystemView::OnHelp()
{
	// TODO:  �ڴ���������������
	ShellExecute(NULL, TEXT("OPEN"),_T("MCT2008Help.CHM"), NULL, NULL, SW_SHOWNORMAL);
}
