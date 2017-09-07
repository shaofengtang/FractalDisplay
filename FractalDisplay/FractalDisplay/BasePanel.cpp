// BasePanel.cpp : 实现文件
//

#include "stdafx.h"
#include "FractalDisplay.h"
#include "BasePanel.h"


// CBasePanel

IMPLEMENT_DYNAMIC(CBasePanel, CWnd)

CBasePanel::CBasePanel()
{

}

CBasePanel::~CBasePanel()
{
}


BEGIN_MESSAGE_MAP(CBasePanel, CWnd)
END_MESSAGE_MAP()



// CBasePanel 消息处理程序


BOOL CBasePanel::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此添加专用代码和/或调用基类
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


BOOL CBasePanel::Create(LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID)
{
	// TODO: 在此添加专用代码和/或调用基类

	return CWnd::Create(_T("BASEPANELCLASS"), lpszWindowName, dwStyle, rect, pParentWnd, nID);
}
