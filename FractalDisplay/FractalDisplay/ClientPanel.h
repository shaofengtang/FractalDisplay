#pragma once
#include "BasePanel.h"
#include "EditControl.h"
#include "ImageButton.h"

class CClientPanel :
	public CBasePanel
{
public:
	CClientPanel();
	~CClientPanel();

private:
	CImage m_imgClient;
	HPEN m_hPen;
	HFONT m_hFont;

	// parameter area
	float m_fCr, m_fCi;
	CEditControl m_editCr, m_editCi;
	
	// color spectrum area
	int m_nR, m_nG, m_nB, m_nIndex;
	unsigned char* m_pSpectrumBuffer;
	unsigned char m_rValue[COLORNUM];
	unsigned char m_gValue[COLORNUM];
	unsigned char m_bValue[COLORNUM];
	CString m_strR, m_strG, m_strB;
	CRect m_rcR, m_rcG, m_rcB;
	CRect m_rcSpectrum;
	CImageButton m_btnSvSpectrum;
	CImage m_imgPostri;
	CRect m_rcPostri;
	bool m_bMovePos;
	bool m_bSetColor;

	// button area
	CImageButton m_btnClose;
	CImageButton m_btnShow;
	CImageButton m_btnSave;
	CImageButton m_btnMagnify;

	// display area
	unsigned char* m_pFrameBuffer;
	unsigned char* m_pIndexBuffer;
	CRect m_rcImage;
	CRect m_rcPreImage;
	CRect m_rcCurImage;
	float m_fRange;
	float m_u0, m_v0;

public:
	void DrawFrameBuffer(CDC *pDC);
	void DrawSpectrumBuffer(CDC *pDC);
	void JuliaSetPicture();
	void SaveBitmapToFile(unsigned char *frameBuffer, int width, int height);
	void CreateSpectrum();

protected:
	DECLARE_MESSAGE_MAP()
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnClickCloseBtn();
	afx_msg void OnClickShowBtn();
	afx_msg void OnClickSaveImage();
	afx_msg void OnClickSvSpectrum();
	afx_msg void OnClickMagnify();
	afx_msg LRESULT OnTabDown(WPARAM wParam, LPARAM lParam);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
public:
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
};

