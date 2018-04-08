
// MainFrm.cpp : CMainFrame ���ʵ��
//

#include "stdafx.h"
#include "CV_System.h"

#include "MainFrm.h"
#include "CV_SystemDoc.h"
#include "CV_SystemView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWndEx)

const int  iMaxUserToolbars = 10;
const UINT uiFirstUserToolBarId = AFX_IDW_CONTROLBAR_FIRST + 40;
const UINT uiLastUserToolBarId = uiFirstUserToolBarId + iMaxUserToolbars - 1;

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWndEx)
	ON_WM_CREATE()
//	ON_COMMAND(ID_VIEW_CUSTOMIZE, &CMainFrame::OnViewCustomize)
//	ON_REGISTERED_MESSAGE(AFX_WM_CREATETOOLBAR, &CMainFrame::OnToolbarCreateNew)
//	ON_COMMAND_RANGE(ID_VIEW_APPLOOK_WIN_2000, ID_VIEW_APPLOOK_WINDOWS_7, &CMainFrame::OnApplicationLook)
//	ON_UPDATE_COMMAND_UI_RANGE(ID_VIEW_APPLOOK_WIN_2000, ID_VIEW_APPLOOK_WINDOWS_7, &CMainFrame::OnUpdateApplicationLook)
	ON_WM_CLOSE()
//	ON_COMMAND(IDCANCEL, &CMainFrame::OnIdcancel)
//	ON_WM_SIZE()
//ON_BN_CLICKED(IDC_START, &CMainFrame::OnBnClickedStart)
//ON_WM_TIMER()
ON_COMMAND(ID_32791, &CMainFrame::OnExit)
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // ״̬��ָʾ��
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

// CMainFrame ����/����

CMainFrame::CMainFrame()
{
	// TODO: �ڴ���ӳ�Ա��ʼ������
	//theApp.m_nAppLook = theApp.GetInt(_T("ApplicationLook"), ID_VIEW_APPLOOK_OFF_2007_BLUE);
}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWndEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	//�Զ��崰��ͼ�ꡢ�˵�����ʽ
	m_hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDI_ICON1));
	SetClassLongPtr(m_hWnd, GCLP_HICON, (LONG)m_hIcon);

	CMenu menu;
	menu.LoadMenu(IDR_MAINFRAME);
	SetMenu(&menu);
	menu.Detach();
	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWndEx::PreCreateWindow(cs) )
		return FALSE;
	// TODO: �ڴ˴�ͨ���޸�
	//  CREATESTRUCT cs ���޸Ĵ��������ʽ
	//cs.style=WS_OVERLAPPED | WS_SYSMENU/* | WS_THICKFRAME*/;
	//cs.cx = 782; cs.cy = 665;
	cs.style&=~FWS_ADDTOTITLE;
	cs.lpszName = _T("VisualSystem");
	return TRUE;
}

// CMainFrame ���

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWndEx::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWndEx::Dump(dc);
}
#endif //_DEBUG

void CMainFrame::OnClose()
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	//CRect wSize;
	//GetWindowRect(&wSize);
	//theApp.WriteInt(_T("WinSize_X"), wSize.left);
	//theApp.WriteInt(_T("WinSize_Y"), wSize.top);
	//theApp.WriteInt(_T("WinSize_H"), wSize.Height());
	//theApp.WriteInt(_T("WinSize_W"), wSize.Width());
	CCV_SystemView* pView=(CCV_SystemView*)GetActiveView();
	//if (pView->capture != NULL)
	//	cvReleaseCapture(&pView->capture);
	if (pView->cap.isOpened())
	{
		pView->cap.release();
		pView->cap = NULL;
	}
	CFrameWndEx::OnClose();
}


//void CMainFrame::OnIdcancel()
//{
//	// TODO: �ڴ���������������
//}


//void CMainFrame::OnSize(UINT nType, int cx, int cy)
//{
//	CFrameWndEx::OnSize(nType, cx, cy);
//
//	// TODO: �ڴ˴������Ϣ����������
//	if(nType==SIZE_MINIMIZED)
//		return;
//	if (nType==SIZE_MAXIMIZED)
//	{
//		char msg[2048];
//		CWnd *pWnd;
//		CRect winSize;
//		GetWindowRect(&winSize);
//		ScreenToClient(&winSize);
//		sprintf_s(msg, "OK!%d", winSize.bottom);
//		CStringW Msg;
//		Msg= msg;
//		//pWnd=GetDlgItem(IDC_IMSHOW);
//		//pWnd->MoveWindow(winSize.left, winSize.top, winSize.Height()+cx, winSize.Width()+cy);
//		AfxMessageBox(Msg);
//	}
//
//	//ChangeSize(pWnd, winSize, cx, cy);
//	//pWnd=GetDlgItem(IDC_CONTROL);
//	//ChangeSize(pWnd, winSize, cx, cy);
//	//pWnd=GetDlgItem(IDC_STATIC);
//	//ChangeSize(pWnd, winSize, cx, cy);
//	//GetClientRect(&winSize);
//	//MessageBox(_T("gotcha!"));
//}

//void CMainFrame::ChangeSize(CWnd *pWnd, CRect m_rect, int cx, int cy)
//{
//	if (pWnd)
//	{
//		CRect rect;
//		pWnd->GetWindowRect(&rect);
//		ScreenToClient(&rect);
//		rect.left=rect.left*cx/m_rect.Width();
//		rect.right=rect.right*cy/m_rect.Width();
//		rect.top=rect.top*cy/m_rect.Height();
//		rect.bottom=rect.bottom*cy/m_rect.Height();
//		pWnd->MoveWindow(&rect);
//	}
//}


void CMainFrame::OnExit()
{
	// TODO:  �ڴ���������������
	OnClose();
}
