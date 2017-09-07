// EditControl.cpp : 实现文件
//

#include "stdafx.h"
#include "FractalDisplay.h"
#include "EditControl.h"

const int EDITBORDERWIDTH = 1;

// CEditControl

IMPLEMENT_DYNAMIC(CEditControl, CEdit)

CEditControl::CEditControl()
{
	// Border properties
	m_rcNCBottom.SetRectEmpty();
	m_rcNCTop.SetRectEmpty();
	m_rcNCLeft.SetRectEmpty();
	m_rcNCRight.SetRectEmpty();
	m_rcClientTop.SetRectEmpty();
	m_clrBorder = RGB(100, 100, 100);
	m_pBrushBorder = NULL;
	m_pBrushClient = NULL;
	// TextRect properties
	m_clrText = RGB(255, 255, 255);
	m_clrClient = RGB(70, 70, 70);
	m_nFontHeight = 20;
	m_strFont = _T("Microsoft YaHei UI");
	m_pFont = NULL;
	// State properties
	m_bIsHover = FALSE;
	m_bIsFocus = FALSE;
}

CEditControl::~CEditControl()
{
	if (m_pBrushBorder == NULL) delete m_pBrushBorder;
	if (m_pBrushClient == NULL) delete m_pBrushClient;
	if (m_pFont == NULL) delete m_pFont;
	DeleteObject(m_brClient);
}


BEGIN_MESSAGE_MAP(CEditControl, CEdit)
	ON_WM_NCCALCSIZE()
	ON_WM_NCPAINT()
	ON_WM_CTLCOLOR_REFLECT()
	ON_WM_KILLFOCUS()
	ON_WM_SETFOCUS()
	ON_WM_CREATE()
	ON_WM_MOUSEMOVE()
	ON_MESSAGE(WM_MOUSEHOVER, OnMouseHover)
	ON_MESSAGE(WM_MOUSELEAVE, OnMouseLeave)
	ON_WM_KEYDOWN()
	ON_WM_CHAR()
END_MESSAGE_MAP()



// CEditControl 消息处理程序




void CEditControl::OnNcCalcSize(BOOL bCalcValidRects, NCCALCSIZE_PARAMS* lpncsp)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CRect rcWnd, rcClient;
	GetWindowRect(rcWnd);
	rcClient.left = 0;
	rcClient.top = 0;
	rcClient.right = rcWnd.right - rcWnd.left;
	rcClient.bottom = rcWnd.bottom - rcWnd.top;

	lpncsp->rgrc[0].left += EDITBORDERWIDTH;
	lpncsp->rgrc[0].right -= EDITBORDERWIDTH;
	lpncsp->rgrc[0].bottom -= EDITBORDERWIDTH;
	lpncsp->rgrc[0].top += (rcClient.bottom - EDITBORDERWIDTH * 2 - m_nFontHeight) / 2 + 1;

	m_rcNCTop = rcClient;
	m_rcNCTop.bottom = EDITBORDERWIDTH;

	m_rcNCBottom = rcClient;
	m_rcNCBottom.top = rcClient.Height() - EDITBORDERWIDTH;

	m_rcNCLeft = rcClient;
	m_rcNCLeft.right = EDITBORDERWIDTH;

	m_rcNCRight = rcClient;
	m_rcNCRight.left = rcClient.Width() - EDITBORDERWIDTH;

	m_rcClientTop = rcClient;
	m_rcClientTop.top = EDITBORDERWIDTH;
	m_rcClientTop.left = EDITBORDERWIDTH;
	m_rcClientTop.right = rcClient.Width() - 1;
	m_rcClientTop.bottom = lpncsp->rgrc[0].top;

	// CEdit::OnNcCalcSize(bCalcValidRects, lpncsp);
}


void CEditControl::OnNcPaint()
{
	// TODO: 在此处添加消息处理程序代码
	CWindowDC dc(this);

	if (m_pBrushBorder) delete m_pBrushBorder;
	m_pBrushBorder = new CBrush(m_clrBorder);

	if (m_pBrushClient) delete m_pBrushClient;
	m_pBrushClient = new CBrush(m_clrClient);

	dc.FillRect(m_rcClientTop, m_pBrushClient);
	dc.FillRect(m_rcNCTop, m_pBrushBorder);
	dc.FillRect(m_rcNCLeft, m_pBrushBorder);
	dc.FillRect(m_rcNCRight, m_pBrushBorder);
	dc.FillRect(m_rcNCBottom, m_pBrushBorder);
	// 不为绘图消息调用 CEdit::OnNcPaint()
}


HBRUSH CEditControl::CtlColor(CDC* pDC, UINT nCtlColor)
{
	// TODO:  在此更改 DC 的任何特性
	pDC->SetTextColor(m_clrText);
	pDC->SetBkColor(m_clrClient);
	return m_brClient;
	// TODO:  如果不应调用父级的处理程序，则返回非 null 画笔
	// return NULL;
}


void CEditControl::OnKillFocus(CWnd* pNewWnd)
{
	CEdit::OnKillFocus(pNewWnd);

	// TODO: 在此处添加消息处理程序代码
	m_bIsFocus = FALSE;
	m_clrBorder = RGB(100, 100, 100);
	::SendMessage(this->m_hWnd, WM_NCPAINT, 0, 0);
}


void CEditControl::OnSetFocus(CWnd* pOldWnd)
{
	CEdit::OnSetFocus(pOldWnd);

	// TODO: 在此处添加消息处理程序代码
	m_bIsFocus = TRUE;
	m_clrBorder = RGB(0, 130, 210);
	::SendMessage(this->m_hWnd, WM_NCPAINT, 0, 0);
}


int CEditControl::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CEdit::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  在此添加您专用的创建代码
	m_brClient = CreateSolidBrush(m_clrClient);

	m_pFont = new CFont;    //生成字体对象
	m_pFont->CreateFont(m_nFontHeight, 0, 0, 0, 0, 0, 0, 0, GB2312_CHARSET, 0, 0, 0, 0, m_strFont);
	SetFont(m_pFont);
	SetLimitText(15);

	return 0;
}


void CEditControl::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	TRACKMOUSEEVENT tme;
	tme.cbSize = sizeof(TRACKMOUSEEVENT);
	tme.hwndTrack = m_hWnd;
	tme.dwFlags = TME_HOVER | TME_LEAVE;
	tme.dwHoverTime = 1;
	TrackMouseEvent(&tme);
	CEdit::OnMouseMove(nFlags, point);
}

LRESULT CEditControl::OnMouseHover(WPARAM wParam, LPARAM lParam)
{
	if (!m_bIsHover)
	{
		m_bIsHover = TRUE;
		m_clrBorder = RGB(0, 130, 210);
		::SendMessage(this->m_hWnd, WM_NCPAINT, 0, 0);
		Invalidate();
	}
	return TRUE;
}

LRESULT CEditControl::OnMouseLeave(WPARAM wParam, LPARAM lParam)
{
	if (m_bIsHover)
	{
		m_bIsHover = FALSE;
		if (!m_bIsFocus)
		{
			m_clrBorder = RGB(100, 100, 100);
			::SendMessage(this->m_hWnd, WM_NCPAINT, 0, 0);
			Invalidate();
		}
	}
	return TRUE;
}

void CEditControl::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (nChar == VK_TAB)
	{
		CWnd* wndParent = GetParent();
			wndParent->SendMessage(WM_TABDOWN, this->GetDlgCtrlID(), 0);
	}

	CEdit::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CEditControl::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (nChar <= '9' && nChar >= '0')//数字
		CEdit::OnChar(nChar, nRepCnt, nFlags);
	else if (nChar == '.' || nChar == 0x08 || nChar == '-')
		CEdit::OnChar(nChar, nRepCnt, nFlags);
	//CEdit::OnChar(nChar, nRepCnt, nFlags);
}
