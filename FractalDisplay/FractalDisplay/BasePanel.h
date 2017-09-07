#pragma once


// CBasePanel

class CBasePanel : public CWnd
{
	DECLARE_DYNAMIC(CBasePanel)

public:
	CBasePanel();
	virtual ~CBasePanel();

protected:
	DECLARE_MESSAGE_MAP()
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

public:
	virtual BOOL Create(LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID);
};


