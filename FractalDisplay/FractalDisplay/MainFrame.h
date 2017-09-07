#pragma once
#include <atlimage.h>
#include "CaptionPanel.h"
#include "ClientPanel.h"


// CMainFrame

class CMainFrame : public CWnd
{
	DECLARE_DYNAMIC(CMainFrame)

public:
	CMainFrame();
	virtual ~CMainFrame();

private:
	CImage m_imgBorder;
	CCaptionPanel m_captionPanel;
	CClientPanel m_clientPanel;

protected:
	DECLARE_MESSAGE_MAP()
	afx_msg void OnNcCalcSize(BOOL bCalcValidRects, NCCALCSIZE_PARAMS* lpncsp);
	afx_msg void OnNcPaint();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void PostNcDestroy();
};


