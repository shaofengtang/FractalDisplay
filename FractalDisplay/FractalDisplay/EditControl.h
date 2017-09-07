#pragma once


// CEditControl

class CEditControl : public CEdit
{
	DECLARE_DYNAMIC(CEditControl)

public:
	CEditControl();
	virtual ~CEditControl();

private:
	// Border properties
	CRect m_rcNCBottom;
	CRect m_rcNCTop;
	CRect m_rcNCLeft;
	CRect m_rcNCRight;
	CRect m_rcClientTop;
	COLORREF m_clrBorder;
	CBrush* m_pBrushBorder;
	CBrush* m_pBrushClient;
	// TextRect properties
	COLORREF m_clrText;
	COLORREF m_clrClient;
	HBRUSH m_brClient;
	int m_nFontHeight;
	CString m_strFont;
	CFont* m_pFont;
	// State properties
	BOOL m_bIsHover;
	BOOL m_bIsFocus;

protected:
	DECLARE_MESSAGE_MAP()
	afx_msg void OnNcCalcSize(BOOL bCalcValidRects, NCCALCSIZE_PARAMS* lpncsp);
	afx_msg void OnNcPaint();
	afx_msg HBRUSH CtlColor(CDC* /*pDC*/, UINT /*nCtlColor*/);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg LRESULT OnMouseHover(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnMouseLeave(WPARAM wParam, LPARAM lParam);
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
};


