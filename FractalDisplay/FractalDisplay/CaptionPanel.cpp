#include "stdafx.h"
#include "CaptionPanel.h"
#include "resource.h"
#include "MainFrame.h"


CCaptionPanel::CCaptionPanel()
{
	m_btnMin.m_hWnd = NULL;
	m_btnCls.m_hWnd = NULL;
}


CCaptionPanel::~CCaptionPanel()
{
	DeleteObject(m_hFont);
}
BEGIN_MESSAGE_MAP(CCaptionPanel, CBasePanel)
	ON_WM_CREATE()
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_BN_CLICKED(IDC_MINBTN, OnClickMinBtn)
	ON_BN_CLICKED(IDC_CLSBTN, OnClickClsBtn)
END_MESSAGE_MAP()


int CCaptionPanel::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CBasePanel::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  在此添加您专用的创建代码
	m_hFont = CreateFont(20, 0, 0, 0, 0, 0, 0, 0, /*GB2312_CHARSET*/ANSI_CHARSET, 0, 0, 0, 0, _T("Microsoft YaHei UI"));

	if (m_imgCaption.IsNull())
	{
		m_imgCaption.LoadFromResource(AfxGetInstanceHandle(), IDB_DARKGRAY);
	}

	CRect rcClient;
	GetClientRect(rcClient);
	if (m_btnMin.m_hWnd == NULL)
	{
		m_btnMin.Create(_T("MinButton"), WS_CHILD | WS_VISIBLE, CRect(rcClient.Width() - 80, 0, rcClient.Width() - 40, 25), this, IDC_MINBTN);
		m_btnMin.SetImage(IDB_MINBTN);
	}

	if (m_btnCls.m_hWnd == NULL)
	{
		m_btnCls.Create(_T("ClsButton"), WS_CHILD | WS_VISIBLE, CRect(rcClient.Width() - 40, 0, rcClient.Width(), 25), this, IDC_CLSBTN);
		m_btnCls.SetImage(IDB_CLSBTN);
	}

	return 0;
}


void CCaptionPanel::OnPaint()
{
	CPaintDC dc(this); // device context for painting
					   // TODO: 在此处添加消息处理程序代码
					   // 不为绘图消息调用 CBasePanel::OnPaint()

	CRect rcClient;
	GetClientRect(rcClient);
	if (!m_imgCaption.IsNull())
	{
		m_imgCaption.Draw(dc.m_hDC, rcClient);
	}

	CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd();
	if (pMainFrame)
	{
		// HICON hIcon = (HICON)GetClassLong(pMainFrame->m_hWnd, GCL_HICON);
		HICON hIcon = (HICON)::LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDI_ICON16));
		if (hIcon)
		{
			DrawIconEx(dc.m_hDC, 10, 5, hIcon, 16, 16, 0, 0, DI_NORMAL);
		}
		CString strCaption;
		pMainFrame->GetWindowText(strCaption);
		if (!strCaption.IsEmpty())
		{
			CRect rcText = rcClient;
			rcText.left = 35;
			HFONT hPreFont = (HFONT)dc.SelectObject(m_hFont);
			int nPreBkMode = dc.SetBkMode(TRANSPARENT);
			COLORREF cPreColor = dc.SetTextColor(RGB(255, 255, 255));
			dc.DrawText(strCaption, rcText, DT_LEFT | DT_VCENTER | DT_SINGLELINE | DT_WORD_ELLIPSIS);
			dc.SelectObject(hPreFont);
			dc.SetBkMode(nPreBkMode);
			dc.SetTextColor(cPreColor);
		}
	}
}


void CCaptionPanel::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd();
	if (pMainFrame)
	{
		pMainFrame->SendMessage(WM_NCLBUTTONDOWN, HTCAPTION);
	}
	CBasePanel::OnLButtonDown(nFlags, point);
}

void CCaptionPanel::OnClickMinBtn()
{
	CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd();
	if (pMainFrame)
	{
		pMainFrame->SendMessage(WM_SYSCOMMAND, SC_MINIMIZE, 0);
	}
}

void CCaptionPanel::OnClickClsBtn()
{
	CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd();
	if (pMainFrame)
	{
		pMainFrame->SendMessage(WM_SYSCOMMAND, SC_CLOSE, 0);
	}
}