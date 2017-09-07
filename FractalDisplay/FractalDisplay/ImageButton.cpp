// ImageButton.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "FractalDisplay.h"
#include "ImageButton.h"


// CImageButton

IMPLEMENT_DYNAMIC(CImageButton, CWnd)

CImageButton::CImageButton()
{
	m_bIsHover = FALSE;
	m_bIsDown = FALSE;
}

CImageButton::~CImageButton()
{
}


BEGIN_MESSAGE_MAP(CImageButton, CWnd)
	ON_WM_PAINT()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_MESSAGE(WM_MOUSEHOVER, OnMouseHover)
	ON_MESSAGE(WM_MOUSELEAVE, OnMouseLeave)
END_MESSAGE_MAP()



// CImageButton ��Ϣ�������




void CImageButton::OnPaint()
{
	CPaintDC dc(this); // device context for painting
					   // TODO: �ڴ˴������Ϣ����������
					   // ��Ϊ��ͼ��Ϣ���� CWnd::OnPaint()
	CRect rcClient;
	GetClientRect(rcClient);
	if (!m_imgButton.IsNull())
	{
		BOOL bIsEnabled = IsWindowEnabled();
		if (!bIsEnabled)
		{
			m_imgButton.Draw(dc.m_hDC, rcClient, CRect(m_imgButton.GetWidth() * 3 / 4, 0, m_imgButton.GetWidth(), m_imgButton.GetHeight()));
			return;
		}
		if (m_bIsHover)
		{
			if (m_bIsDown)
			{
				m_imgButton.Draw(dc.m_hDC, rcClient, CRect(m_imgButton.GetWidth() * 2 / 4, 0, m_imgButton.GetWidth() * 3 / 4, m_imgButton.GetHeight()));
			}
			else
			{
				m_imgButton.Draw(dc.m_hDC, rcClient, CRect(m_imgButton.GetWidth() * 1 / 4, 0, m_imgButton.GetWidth() * 2 / 4, m_imgButton.GetHeight()));
			}
		}
		else
		{
			m_imgButton.Draw(dc.m_hDC, rcClient, CRect(0, 0, m_imgButton.GetWidth() * 1 / 4, m_imgButton.GetHeight()));
		}
	}
}


void CImageButton::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	// OnMouseMove is responsible for calling TrackMouseEvent()
	// to track mouse state. It sends WM_MOUSEHOVER and WM_MOUSELEAVE
	// which are uncommon message, and the program should use ON_MESSAGE(messange, messagefun) to handle these messages
	TRACKMOUSEEVENT tme;
	tme.cbSize = sizeof(TRACKMOUSEEVENT);
	tme.hwndTrack = m_hWnd;
	tme.dwFlags = TME_HOVER | TME_LEAVE;
	tme.dwHoverTime = 1;
	TrackMouseEvent(&tme);
	CWnd::OnMouseMove(nFlags, point);
}


void CImageButton::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	if (!m_bIsDown)
	{
		m_bIsDown = TRUE;
		Invalidate();
	}
	CWnd::OnLButtonDown(nFlags, point);
}


void CImageButton::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

	CWnd::OnLButtonUp(nFlags, point);

	if (m_bIsDown)
	{
		m_bIsDown = FALSE;
		Invalidate();

		CWnd* pWndParent = GetParent();
		if (pWndParent)
		{
			WORD wID = GetDlgCtrlID();
			pWndParent->SendMessage(WM_COMMAND, MAKEWPARAM(wID, BN_CLICKED), (LPARAM)m_hWnd);
		}
	}
}


BOOL CImageButton::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: �ڴ����ר�ô����/����û���
	HINSTANCE hInstance = (HINSTANCE)AfxGetInstanceHandle();
	ASSERT(hInstance);

	WNDCLASSEX wcex;
	wcex.cbSize = sizeof(WNDCLASSEX);
	// check if the wndclass has been registered
	BOOL bRet = GetClassInfoEx(hInstance, cs.lpszClass, &wcex);
	if (bRet) return TRUE;

	wcex.style = CS_VREDRAW | CS_HREDRAW;
	wcex.lpfnWndProc = AfxWndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = (HICON)::LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON32));
	wcex.hIconSm = (HICON)::LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON16));
	// wcex.hIcon = (HICON)::LoadImage(NULL, _T("res\\2DParticleKinematic\\ICON.ico"), IMAGE_ICON, 0, 0, LR_DEFAULTSIZE | LR_LOADFROMFILE);
	wcex.hCursor = ::LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = cs.lpszClass;

	// Register the wndclass
	bRet = RegisterClassEx(&wcex);

	return bRet;
}

BOOL CImageButton::Create(LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID)
{
	return CWnd::Create(_T("IMAGEBUTTONCLASS"), lpszWindowName, dwStyle, rect, pParentWnd, nID);
}

LRESULT CImageButton::OnMouseHover(WPARAM wParam, LPARAM lParam)
{
	if (!m_bIsHover)
	{
		m_bIsHover = TRUE;
		Invalidate();
	}
	return TRUE;
}

LRESULT CImageButton::OnMouseLeave(WPARAM wParam, LPARAM lParam)
{
	if (m_bIsHover)
	{
		m_bIsHover = FALSE;
		m_bIsDown = FALSE;
		Invalidate();
	}
	return TRUE;
}

void CImageButton::SetImage(UINT uIDResource)
{
	bool bIsNull = m_imgButton.IsNull();
	if (!bIsNull) m_imgButton.Destroy();
	m_imgButton.LoadFromResource(AfxGetInstanceHandle(), uIDResource);
}