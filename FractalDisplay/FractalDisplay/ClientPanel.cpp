#include "stdafx.h"
#include "ClientPanel.h"
#include "MainFrame.h"
#include "resource.h"


CClientPanel::CClientPanel()
{
	m_editCr.m_hWnd = NULL;
	m_editCi.m_hWnd = NULL;
	m_btnClose.m_hWnd = NULL;
	m_btnShow.m_hWnd = NULL;
	m_btnSave.m_hWnd = NULL;
	m_btnSvSpectrum.m_hWnd = NULL;
	m_btnMagnify.m_hWnd = NULL;
	m_pFrameBuffer = NULL;
	m_pSpectrumBuffer = NULL;
	m_fCr = m_fCi = 0.0f;
	m_nR = m_nG = m_nB = 0;
	m_nIndex = 0;
	m_strR.Format(_T("%d"), m_nR);
	m_strG.Format(_T("%d"), m_nG);
	m_strB.Format(_T("%d"), m_nB);
	CRect rc(30, 40, 70, 70);
	m_rcR = rc;
	rc.left = 80;
	rc.right = 120;
	m_rcG = rc;
	rc.left = 130;
	rc.right = 170;
	m_rcB = rc;
	m_rcPostri.left = 220 + 1;
	m_rcPostri.right = m_rcPostri.left + 5;
	m_rcPostri.top = 100 + 3;
	m_rcPostri.bottom = m_rcPostri.top + 5;

	CRect rcImage(200, 130, 200 + IMAGEWIDTH, 130 + IMAGEHEIGHT);
	m_rcPreImage = rcImage;
	m_rcCurImage = rcImage;
	m_fRange = RANGE;
	m_u0 = -1.5f;
	m_v0 = -1.5f;

	m_bMovePos = false;
	m_bSetColor = false;

	m_rcSpectrum.left = 220;
	m_rcSpectrum.top = 20;
	m_rcSpectrum.right = 220 + SPECTRUMWIDTH;
	m_rcSpectrum.bottom = 20 + SPECTRUMHEIGHT;

	m_rcImage.left = 200;
	m_rcImage.top = 130;
	m_rcImage.right = 200 + IMAGEWIDTH;
	m_rcImage.bottom = 130 + IMAGEHEIGHT;
}


CClientPanel::~CClientPanel()
{
	DeleteObject(m_hPen);
	DeleteObject(m_hFont);
	free(m_pFrameBuffer);
	free(m_pSpectrumBuffer);
	free(m_pIndexBuffer);
}
BEGIN_MESSAGE_MAP(CClientPanel, CBasePanel)
	ON_WM_CREATE()
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_CLOSEBTN, OnClickCloseBtn)
	ON_BN_CLICKED(IDC_SHOWBTN, OnClickShowBtn)
	ON_BN_CLICKED(IDC_SAVEBTN, OnClickSaveImage)
	ON_BN_CLICKED(IDC_SVSPECBTN, OnClickSvSpectrum)
	ON_BN_CLICKED(IDC_MAGNIFYBTN, OnClickMagnify)
	ON_MESSAGE(WM_TABDOWN, OnTabDown)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_KEYDOWN()
	ON_WM_CHAR()
	ON_WM_ERASEBKGND()
	ON_WM_KEYUP()
END_MESSAGE_MAP()


int CClientPanel::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CBasePanel::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  Add your specialized creation code here
	if (m_imgClient.IsNull())
	{
		m_imgClient.LoadFromResource(AfxGetInstanceHandle(), IDB_DARKGRAY);
	}

	if (m_imgPostri.IsNull())
	{
		m_imgPostri.LoadFromResource(AfxGetInstanceHandle(), IDB_POSTRI);
	}

	if (m_editCr.m_hWnd == NULL)
	{
		m_editCr.Create(WS_CHILD | WS_VISIBLE | ES_LEFT, CRect(50, 155, 170, 185), this, IDC_CREDIT);
		m_editCr.SetMargins(5, 0);
	}

	if (m_editCi.m_hWnd == NULL)
	{
		m_editCi.Create(WS_CHILD | WS_VISIBLE | ES_LEFT, CRect(50, 215, 170, 245), this, IDC_CIEDIT);
		m_editCi.SetMargins(5, 0);
	}

	if (m_btnShow.m_hWnd == NULL)
	{
		m_btnShow.Create(_T("ShowButton"), WS_CHILD | WS_VISIBLE, CRect(50, 575, 150, 605), this, IDC_SHOWBTN);
		m_btnShow.SetImage(IDB_SHOWBTN);
	}

	if (m_btnSave.m_hWnd == NULL)
	{
		m_btnSave.Create(_T("SaveButton"), WS_CHILD | WS_VISIBLE, CRect(50, 645, 150, 675), this, IDC_SAVEBTN);
		m_btnSave.SetImage(IDB_SAVEBTN);
	}

	if (m_btnMagnify.m_hWnd == NULL)
	{
		m_btnMagnify.Create(_T("MagnifyButton"), WS_CHILD | WS_VISIBLE, CRect(50, 715, 150, 745), this, IDC_MAGNIFYBTN);
		m_btnMagnify.SetImage(IDB_MAGNIFYBTN);
	}

	if (m_btnClose.m_hWnd == NULL)
	{
		m_btnClose.Create(_T("CloseButton"), WS_CHILD | WS_VISIBLE, CRect(50, 785, 150, 815), this, IDC_CLOSEBTN);
		m_btnClose.SetImage(IDB_CLOSEBTN);
	}

	if (m_btnSvSpectrum.m_hWnd == NULL)
	{
		m_btnSvSpectrum.Create(_T("SaveSpectrumButton"), WS_CHILD | WS_VISIBLE, CRect(50, 80, 150, 105), this, IDC_SVSPECBTN);
		m_btnSvSpectrum.SetImage(IDB_SVSPECBTN);
	}

	m_hPen = CreatePen(PS_SOLID, 1, RGB(100, 100, 100));
	m_hFont = CreateFont(20, 0, 0, 0, 0, 0, 0, 0, ANSI_CHARSET, 0, 0, 0, 0, _T("Microsoft YaHei UI"));

	m_editCr.SetFocus();

	m_pFrameBuffer = (unsigned char*)malloc(IMAGEWIDTH * IMAGEHEIGHT * 3);
	m_pIndexBuffer = (unsigned char*)malloc(IMAGEWIDTH * IMAGEHEIGHT);
	memset(m_pIndexBuffer, 0, IMAGEWIDTH * IMAGEHEIGHT);
	m_pSpectrumBuffer = (unsigned char*)malloc(SPECTRUMWIDTH * SPECTRUMHEIGHT * 3);

	CString strCr, strCi;
	strCr.Format(_T("%f"), m_fCr);
	strCi.Format(_T("%f"), m_fCi);
	GetDlgItem(IDC_CREDIT)->SetWindowTextW(strCr);
	GetDlgItem(IDC_CIEDIT)->SetWindowTextW(strCi);
	m_editCr.SetSel(0, -1);

	CreateSpectrum();
	m_nIndex = 0;
	m_nR = m_rValue[m_nIndex];
	m_nG = m_gValue[m_nIndex];
	m_nB = m_bValue[m_nIndex];
	m_strR.Format(_T("%d"), m_nR);
	m_strG.Format(_T("%d"), m_nG);
	m_strB.Format(_T("%d"), m_nB);

	return 0;
}


void CClientPanel::OnPaint()
{
	CPaintDC dc(this); // device context for painting
					   // TODO: Add your message handler code here
					   // Do not call CBasePanel::OnPaint() for painting messages
	CRect rcClient;
	GetClientRect(rcClient);
	if (!m_imgClient.IsNull())
	{
		m_imgClient.Draw(dc.m_hDC, rcClient);
	}

	if (!m_imgPostri.IsNull())
	{
		m_imgPostri.Draw(dc.m_hDC, m_rcPostri);
	}

	HPEN hPrePen = (HPEN)dc.SelectObject(m_hPen);

	dc.MoveTo(10, 5);
	dc.LineTo(940, 5);
	dc.LineTo(940, 120);
	dc.LineTo(10, 120);
	dc.LineTo(10, 5);

	dc.MoveTo(10, 130);
	dc.LineTo(190, 130);
	dc.LineTo(190, 530);
	dc.LineTo(10, 530);
	dc.LineTo(10, 130);

	dc.MoveTo(10, 540);
	dc.LineTo(190, 540);
	dc.LineTo(190, rcClient.bottom - 10);
	dc.LineTo(10, rcClient.bottom - 10);
	dc.LineTo(10, 540);

	HFONT hPreFont = (HFONT)dc.SelectObject(m_hFont);
	int nPreBkMode = dc.SetBkMode(TRANSPARENT);
	COLORREF cPreColor = dc.SetTextColor(RGB(255, 255, 255));
	
	dc.DrawText(_T("R"), CRect(30, 10, 70, 40), DT_VCENTER | DT_CENTER | DT_SINGLELINE);
	dc.DrawText(_T("G"), CRect(80, 10, 120, 40), DT_VCENTER | DT_CENTER | DT_SINGLELINE);
	dc.DrawText(_T("B"), CRect(130, 10, 170, 40), DT_VCENTER | DT_CENTER | DT_SINGLELINE);
	dc.DrawText(m_strR, m_rcR, DT_VCENTER | DT_CENTER | DT_SINGLELINE);
	dc.DrawText(m_strG, m_rcG, DT_VCENTER | DT_CENTER | DT_SINGLELINE);
	dc.DrawText(m_strB, m_rcB, DT_VCENTER | DT_CENTER | DT_SINGLELINE);
	dc.DrawText(_T("Cr"), CRect(10, 160, 40, 180), DT_VCENTER | DT_RIGHT | DT_SINGLELINE);
	dc.DrawText(_T("Ci"), CRect(10, 220, 40, 240), DT_VCENTER | DT_RIGHT | DT_SINGLELINE);

	dc.SelectObject(hPreFont);
	dc.SetBkMode(nPreBkMode);
	dc.SetTextColor(cPreColor);
	dc.SelectObject(hPrePen);


	DrawFrameBuffer(&dc);
	DrawSpectrumBuffer(&dc);

	HPEN framePen = CreatePen(PS_SOLID, 1, RGB(255, 0, 0));
	hPrePen = (HPEN)dc.SelectObject(framePen);
	dc.MoveTo(m_rcCurImage.left, m_rcCurImage.top);
	dc.LineTo(m_rcCurImage.right, m_rcCurImage.top);
	dc.LineTo(m_rcCurImage.right, m_rcCurImage.bottom);
	dc.LineTo(m_rcCurImage.left, m_rcCurImage.bottom);
	dc.LineTo(m_rcCurImage.left, m_rcCurImage.top);
	dc.SelectObject(hPreFont);
}


void CClientPanel::OnSize(UINT nType, int cx, int cy)
{
	CBasePanel::OnSize(nType, cx, cy);

	// TODO: Add your message handler code here
}


void CClientPanel::OnClickCloseBtn()
{
	CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd();
	if (pMainFrame)
	{
		pMainFrame->SendMessage(WM_SYSCOMMAND, SC_CLOSE, 0);
	}
}

void CClientPanel::OnClickShowBtn()
{
	JuliaSetPicture();
	Invalidate();
	m_bSetColor = false;
}

void CClientPanel::OnClickSaveImage()
{
	SaveBitmapToFile(m_pFrameBuffer, IMAGEWIDTH, IMAGEHEIGHT);
	m_bSetColor = false;
}

void CClientPanel::OnClickSvSpectrum()
{

}

void CClientPanel::OnClickMagnify()
{
	float preInc = m_fRange / (float)IMAGEWIDTH;
	m_u0 = m_u0 + (m_rcCurImage.left - m_rcPreImage.left) * preInc;
	m_v0 = m_v0 + (m_rcCurImage.top - m_rcPreImage.top) * preInc;
	float newRange = (float)m_rcCurImage.Width() / (float)m_rcPreImage.Width() * m_fRange;
	m_fRange = newRange;

	JuliaSetPicture();
	Invalidate();
	m_bSetColor = false;
}

void CClientPanel::DrawFrameBuffer(CDC *pDC)
{
	if (m_pFrameBuffer == NULL || m_pIndexBuffer == NULL)
	{
		return;
	}

	// fill out framebuffer
	for (int i = 0; i < IMAGEHEIGHT; ++i)
	{
		for (int j = 0; j < IMAGEWIDTH; ++j)
		{
			m_pFrameBuffer[(i * IMAGEWIDTH + j) * 3 + 0] = m_bValue[m_pIndexBuffer[(i * IMAGEWIDTH + j)]]; //b
			m_pFrameBuffer[(i * IMAGEWIDTH + j) * 3 + 1] = m_gValue[m_pIndexBuffer[(i * IMAGEWIDTH + j)]]; //g
			m_pFrameBuffer[(i * IMAGEWIDTH + j) * 3 + 2] = m_rValue[m_pIndexBuffer[(i * IMAGEWIDTH + j)]]; //r
		}
	}
	

	HDC hdc;
	hdc = ::CreateCompatibleDC(pDC->m_hDC);
	HBITMAP m_bitmap;

	// Display the current image
	char buffer[sizeof(BITMAPINFO)];
	BITMAPINFO* binfo = (BITMAPINFO*)buffer;
	memset(binfo, 0, sizeof(BITMAPINFOHEADER));
	binfo->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);

	// Create the bitmap
	BITMAPINFOHEADER* bih = &binfo->bmiHeader;
	bih->biBitCount = 3 * 8;
	bih->biWidth = IMAGEWIDTH; // m_pApplication->m_nWidth;
	bih->biHeight = IMAGEHEIGHT; // m_pApplication->m_nHeight;
	bih->biPlanes = 1;
	bih->biCompression = BI_RGB;
	bih->biSizeImage = 0;

	m_bitmap = CreateDIBSection(hdc, binfo, 0, 0, 0, DIB_RGB_COLORS);

	int colors = DIB_RGB_COLORS;

	::SelectObject(hdc, m_bitmap);
	binfo->bmiHeader.biBitCount = 0;
	GetDIBits(hdc, m_bitmap, 0, 0, 0, binfo, colors);
	binfo->bmiHeader.biBitCount = 24;
	binfo->bmiHeader.biHeight = -abs(binfo->bmiHeader.biHeight);
	SetDIBits(hdc, m_bitmap, 0, IMAGEHEIGHT/*m_pApplication->m_nHeight*/, m_pFrameBuffer, binfo, colors);

	::SetStretchBltMode(pDC->m_hDC, COLORONCOLOR);
	//CRect client;
	//GetClientRect(&client);
	::BitBlt(pDC->m_hDC, 200, 130, IMAGEWIDTH/*m_pApplication->m_nWidth*/, IMAGEHEIGHT/*m_pApplication->m_nHeight*/,
		hdc, 0, 0, SRCCOPY);
	::DeleteDC(hdc);
	DeleteObject(m_bitmap);
}

void CClientPanel::DrawSpectrumBuffer(CDC *pDC)
{
	if (m_pSpectrumBuffer == NULL)
	{
		return;
	}

	HDC hdc;
	hdc = ::CreateCompatibleDC(pDC->m_hDC);
	HBITMAP m_bitmap;

	// Display the current image
	char buffer[sizeof(BITMAPINFO)];
	BITMAPINFO* binfo = (BITMAPINFO*)buffer;
	memset(binfo, 0, sizeof(BITMAPINFOHEADER));
	binfo->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);

	// Create the bitmap
	BITMAPINFOHEADER* bih = &binfo->bmiHeader;
	bih->biBitCount = 3 * 8;
	bih->biWidth = SPECTRUMWIDTH; // m_pApplication->m_nWidth;
	bih->biHeight = SPECTRUMHEIGHT; // m_pApplication->m_nHeight;
	bih->biPlanes = 1;
	bih->biCompression = BI_RGB;
	bih->biSizeImage = 0;

	m_bitmap = CreateDIBSection(hdc, binfo, 0, 0, 0, DIB_RGB_COLORS);

	int colors = DIB_RGB_COLORS;

	::SelectObject(hdc, m_bitmap);
	binfo->bmiHeader.biBitCount = 0;
	GetDIBits(hdc, m_bitmap, 0, 0, 0, binfo, colors);
	binfo->bmiHeader.biBitCount = 24;
	binfo->bmiHeader.biHeight = -abs(binfo->bmiHeader.biHeight);
	SetDIBits(hdc, m_bitmap, 0, SPECTRUMHEIGHT/*m_pApplication->m_nHeight*/, m_pSpectrumBuffer, binfo, colors);

	::SetStretchBltMode(pDC->m_hDC, COLORONCOLOR);
	//CRect client;
	//GetClientRect(&client);
	::BitBlt(pDC->m_hDC, 220, 20, SPECTRUMWIDTH/*m_pApplication->m_nWidth*/, SPECTRUMHEIGHT/*m_pApplication->m_nHeight*/,
		hdc, 0, 0, SRCCOPY);
	::DeleteDC(hdc);
	DeleteObject(m_bitmap);
}

void CClientPanel::JuliaSetPicture()
{
	CString strCr, strCi;
	GetDlgItem(IDC_CREDIT)->GetWindowText(strCr);
	GetDlgItem(IDC_CIEDIT)->GetWindowText(strCi);
	m_fCr = _ttof(strCr);
	m_fCi = _ttof(strCi);

	float uu, vv, u, v;
	int k;
	// set color reference
	/*char rValue[100];
	char gValue[100];
	char bValue[100];

	for (int i = 0; i < 20; ++i)
	{
		rValue[i] = i * 8;
		gValue[i] = i * 12;
		bValue[i] = 0;
	}
	for (int i = 20; i < 100; ++i)
	{
		rValue[i] = (2 * i + 1) % 256;
		gValue[i] = (i * i - 5) % 256;
		bValue[i] = (i - 19) % 256;
	}*/

#if _DEBUG
	int count[COLORNUM];
	for (int i = 0; i < COLORNUM; ++i) count[i] = 0;
#endif

	float inc = m_fRange / IMAGEWIDTH;
	for (int i = 0; i < IMAGEHEIGHT; ++i)
	{
		for (int j = 0; j < IMAGEWIDTH; ++j)
		{
			//u = (float)(j - IMAGEWIDTH / 2.0f) / (float)(IMAGEWIDTH / 3.0f);
			//v = (float)(i - IMAGEHEIGHT / 2.0f) / (float)(IMAGEHEIGHT / 3.0f);
			u = m_u0 + inc * j;
			v = m_v0 + inc * i;
			for (k = 0; k < COLORNUM - 1; ++k)
			{
				uu = u * u;
				vv = v * v;
				if (sqrt(uu + vv) > 2.0f) break;
				v = 2.0f * u * v + m_fCi;
				u = uu - vv + m_fCr;
			}
#if _DEBUG
			++count[k];
#endif
			// 
			// m_pFrameBuffer[(i * IMAGEWIDTH + j) * 3 + 0] = m_bValue[k]; //b
			// m_pFrameBuffer[(i * IMAGEWIDTH + j) * 3 + 1] = m_gValue[k]; //g
			// m_pFrameBuffer[(i * IMAGEWIDTH + j) * 3 + 2] = m_rValue[k]; //r
			// index
			m_pIndexBuffer[(i * IMAGEWIDTH + j)] = k;
		}
	}

#if _DEBUG
	FILE *pout;
	fopen_s(&pout, "Counts.txt", "wb");
	if (pout)
	{
		for (int i = 0; i < COLORNUM; ++i)
			fprintf(pout, "k = %d, count = %d\n", i, count[i]);
	}
	else
	{
		AfxMessageBox(_T("Faided to open file!"));
	}
	fclose(pout);
#endif
}

LRESULT CClientPanel::OnTabDown(WPARAM wParam, LPARAM lParam)
{
	switch (wParam)
	{
	case IDC_CREDIT:
		m_editCi.SetFocus();
		m_editCi.SetSel(0, -1);
		break;

	case IDC_CIEDIT:
		m_editCr.SetFocus();
		m_editCr.SetSel(0, -1);
		break;

	
	default:
		break;
	}
	m_bSetColor = false;

	return TRUE;
}

void CClientPanel::SaveBitmapToFile(unsigned char *frameBuffer, int width, int height)
{
	BITMAPINFOHEADER bmpInfoHeader = { 0 };
	// set the size
	bmpInfoHeader.biSize = sizeof(BITMAPINFOHEADER);
	// bit count
	bmpInfoHeader.biBitCount = 24;
	// use all colors
	bmpInfoHeader.biClrImportant = 0;
	// use as many colors according to bits per pixel
	bmpInfoHeader.biClrUsed = 0;
	// store as uncompressed
	bmpInfoHeader.biCompression = BI_RGB;
	// set the height in pixels
	bmpInfoHeader.biHeight = height;
	// set the width in pixels
	bmpInfoHeader.biWidth = width;
	// default number of planes
	bmpInfoHeader.biPlanes = 1;
	// calculate th eimage size in bytes
	bmpInfoHeader.biSizeImage = width * height * 3;

	BITMAPFILEHEADER bfh = { 0 };
	// 0x4D42 = MB
	bfh.bfType = 0x4D42;
	// offset to the RGBQUAD
	bfh.bfOffBits = sizeof(BITMAPINFOHEADER) + sizeof(BITMAPFILEHEADER);
	// total size of image including size of headers
	bfh.bfSize = bfh.bfOffBits + bmpInfoHeader.biSizeImage;
	// create the file
	HANDLE hFile = CreateFile(_T("FractalImage.bmp"), GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if (!hFile)
	{
		AfxMessageBox(_T("Failed to create saving file!"));
		return;
	}

	DWORD dwWritten = 0;
	// write the file header
	WriteFile(hFile, &bfh, sizeof(bfh), &dwWritten, NULL);
	// write the bitmap info header
	WriteFile(hFile, &bmpInfoHeader, sizeof(bmpInfoHeader), &dwWritten, NULL);
	// write the RGB DATA
	char *buffer = (char*)malloc(IMAGEWIDTH * IMAGEHEIGHT * 3);
	for (int j = 0; j < IMAGEHEIGHT; ++j)
	{
		for (int i = 0; i < IMAGEWIDTH; ++i)
		{
			buffer[(j * IMAGEWIDTH + i) * 3 + 0] = frameBuffer[((IMAGEHEIGHT - j - 1) * IMAGEWIDTH + i) * 3 + 0];
			buffer[(j * IMAGEWIDTH + i) * 3 + 1] = frameBuffer[((IMAGEHEIGHT - j - 1) * IMAGEWIDTH + i) * 3 + 1];
			buffer[(j * IMAGEWIDTH + i) * 3 + 2] = frameBuffer[((IMAGEHEIGHT - j - 1) * IMAGEWIDTH + i) * 3 + 2];
		}
	}
	WriteFile(hFile, buffer, bmpInfoHeader.biSizeImage, &dwWritten, NULL);
	free(buffer);
	// close the file handle
	CloseHandle(hFile);
}

void CClientPanel::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	CRect rc(200, 130, 200 + IMAGEWIDTH, 130 + IMAGEHEIGHT);
	if (rc.PtInRect(point))
	{
		m_rcCurImage.left = point.x;
		m_rcCurImage.top = point.y;
	}
	else if (m_rcPostri.PtInRect(point))
	{
		//AfxMessageBox(_T("HIT"));
		m_bMovePos = true;
		m_bSetColor = true;
		this->SetFocus();
	}

	CBasePanel::OnLButtonDown(nFlags, point);
}


void CClientPanel::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	if (m_bMovePos)
	{
		if (point.x >= 220 && point.x <= 220 + SPECTRUMWIDTH)
		{
			int newPos = (point.x - 220) / 7;
			m_rcPostri.left = 220 + newPos * 7 + 1;
			m_rcPostri.right = m_rcPostri.left + 5;
			m_nIndex = newPos;
			m_nR = m_rValue[m_nIndex];
			m_nG = m_gValue[m_nIndex];
			m_nB = m_bValue[m_nIndex];
			m_strR.Format(_T("%d"), m_nR);
			m_strG.Format(_T("%d"), m_nG);
			m_strB.Format(_T("%d"), m_nB);
			InvalidateRect(m_rcR);
			InvalidateRect(m_rcG);
			InvalidateRect(m_rcB);
			CRect rcPostriLine = m_rcPostri;
			rcPostriLine.left = m_rcSpectrum.left;
			rcPostriLine.right = m_rcSpectrum.right;
			InvalidateRect(rcPostriLine);
		}
		m_bMovePos = false;
	}
	else
	{
		CRect rc(200, 130, 200 + IMAGEWIDTH, 130 + IMAGEHEIGHT);
		if (rc.PtInRect(point))
		{
			m_rcCurImage.right = point.x;
			m_rcCurImage.bottom = m_rcCurImage.top + m_rcCurImage.Width();
			//InvalidateRect(m_rcCurImage);
			Invalidate();
		}
		else
		{
			m_rcCurImage.left = m_rcPreImage.left;
			m_rcCurImage.top = m_rcPreImage.top;
		}
	}
	
	CBasePanel::OnLButtonUp(nFlags, point);
}

void CClientPanel::CreateSpectrum()
{
	// generate spectrum
	/*for (int i = 0; i < 20; ++i)
	{
		m_rValue[i] = i * 8;
		m_gValue[i] = i * 12;
		m_bValue[i] = 0;
	}
	for (int i = 20; i < 100; ++i)
	{
		m_rValue[i] = (2 * i + 1) % 256;
		m_gValue[i] = (i * i - 5) % 256;
		m_bValue[i] = (i - 19) % 256;
	}*/
	for (int i = 0; i < 30; ++i)
	{
		m_rValue[i] = i * 8;
		m_gValue[i] = i * 8;
		m_bValue[i] = i * 8;
	}
	for (int i = 30; i < COLORNUM; ++i)
	{
		m_rValue[i] = 0;
		m_gValue[i] = 255 - (i - 30) * 3;
		m_bValue[i] = 255 - (i - 30) * 3;
	}
	/*for (int i = 10; i < 45; ++i)
	{
		m_rValue[i] = 120; // 255 - (i - 10) * 5;
		m_gValue[i] = 120;
		m_bValue[i] = 80 + (i - 10) * 5;
	}
	for (int i = 45; i < 60; ++i)
	{
		m_rValue[i] = 250;
		m_gValue[i] = 0;
		m_bValue[i] = (i - 40) * 10;
	}*/
	/*for (int i = 10; i < 70; ++i)
	{
		m_rValue[i] = 255 - i * 2;
		m_gValue[i] = 160 - i * 2;
		m_bValue[i] = 0;
	}*/
	/*for (int i = 60; i < COLORNUM; ++i)
	{
		m_rValue[i] = (i - 60) * 5;
		m_gValue[i] = (i - 60) * 5;
		m_bValue[i] = (i - 60) * 5;
	}*/
	/*for (int i = 50; i < 70; ++i)
	{
		m_rValue[i] = 255 - (i - 50) * 2;
		m_gValue[i] = 128 - (i - 50);
		m_bValue[i] = 255;
	}
	for (int i = 70; i < COLORNUM; ++i)
	{
		m_rValue[i] = 255 - (i - 70);
		m_gValue[i] = 255 - (i - 70) * 2;
		m_bValue[i] = 0;
	}*/
	// generate spectrum framebuffer
	for (int j = 0; j < SPECTRUMHEIGHT; ++j)
	{
		for (int i = 0; i < COLORNUM; ++i)
		{
			for (int k = 0; k < SPECTRUMWIDTH / COLORNUM; ++k)
			{
				m_pSpectrumBuffer[((j * COLORNUM + i) * 7 + k) * 3 + R] = m_bValue[i];
				m_pSpectrumBuffer[((j * COLORNUM + i) * 7 + k) * 3 + G] = m_gValue[i];
				m_pSpectrumBuffer[((j * COLORNUM + i) * 7 + k) * 3 + B] = m_rValue[i];
			}
		}
	}
}


void CClientPanel::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: Add your message handler code here and/or call default
	/*if (nChar == 'q')
	{
		AfxMessageBox(_T("Q DOWN!"));
	}*/
	CBasePanel::OnKeyDown(nChar, nRepCnt, nFlags);
}


void CClientPanel::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: Add your message handler code here and/or call default
	if (nChar == 'q' && m_bSetColor)
	{
		m_rValue[m_nIndex] += 1;
		if (m_rValue[m_nIndex] > 255) m_rValue[m_nIndex] = 255;
		else
		{
			m_strR.Format(_T("%d"), m_rValue[m_nIndex]);
			InvalidateRect(m_rcR);
		}
	}
	else if (nChar == 'a' && m_bSetColor)
	{
		m_rValue[m_nIndex] -= 1;
		if (m_rValue[m_nIndex] < 0) m_rValue[m_nIndex] = 0;
		else 
		{
			m_strR.Format(_T("%d"), m_rValue[m_nIndex]);
			InvalidateRect(m_rcR);
		}
	}
	else if (nChar == 'w' && m_bSetColor)
	{
		m_gValue[m_nIndex] += 1;
		if (m_gValue[m_nIndex] > 255) m_gValue[m_nIndex] = 255;
		else
		{
			m_strG.Format(_T("%d"), m_gValue[m_nIndex]);
			InvalidateRect(m_rcG);
		}
	}
	else if (nChar == 's' && m_bSetColor)
	{
		m_gValue[m_nIndex] -= 1;
		if (m_gValue[m_nIndex] < 0) m_gValue[m_nIndex] = 0;
		else
		{
			m_strG.Format(_T("%d"), m_gValue[m_nIndex]);
			InvalidateRect(m_rcG);
		}
	}
	else if (nChar == 'e' && m_bSetColor)
	{
		m_bValue[m_nIndex] += 1;
		if (m_bValue[m_nIndex] > 255) m_bValue[m_nIndex] = 255;
		else
		{
			m_strB.Format(_T("%d"), m_bValue[m_nIndex]);
			InvalidateRect(m_rcB);
		}
	}
	else if (nChar == 'd' && m_bSetColor)
	{
		m_bValue[m_nIndex] -= 1;
		if (m_bValue[m_nIndex] < 0) m_bValue[m_nIndex] = 0;
		else
		{
			m_strB.Format(_T("%d"), m_bValue[m_nIndex]);
			InvalidateRect(m_rcB);
		}
	}
	if (m_bSetColor)
	{

		//InvalidateRect(m_rcSpectrum);
	}

	CBasePanel::OnChar(nChar, nRepCnt, nFlags);
}


BOOL CClientPanel::OnEraseBkgnd(CDC* pDC)
{
	// TODO: Add your message handler code here and/or call default
	return true;
	//return CBasePanel::OnEraseBkgnd(pDC);
}


void CClientPanel::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: Add your message handler code here and/or call default
	if (nChar == 'Q' || nChar == 'A' || nChar == 'W' || nChar == 'S' || nChar == 'E' || nChar == 'D')
	{
		//AfxMessageBox(_T("Key Up."));
		for (int i = 0; i < SPECTRUMHEIGHT; ++i)
		{
			for (int k = 0; k < 7; ++k)
			{
				m_pSpectrumBuffer[((i * COLORNUM + m_nIndex) * 7 + k) * 3 + R] = m_bValue[m_nIndex];
				m_pSpectrumBuffer[((i * COLORNUM + m_nIndex) * 7 + k) * 3 + G] = m_gValue[m_nIndex];
				m_pSpectrumBuffer[((i * COLORNUM + m_nIndex) * 7 + k) * 3 + B] = m_rValue[m_nIndex];
			}
		}
		
		InvalidateRect(m_rcSpectrum);
		InvalidateRect(m_rcImage);
	}
	
	this->SetFocus();
	CBasePanel::OnKeyUp(nChar, nRepCnt, nFlags);
}
