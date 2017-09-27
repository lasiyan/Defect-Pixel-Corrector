// ImageViewer.cpp : implementation file
//

#include "stdafx.h"
#include "IpDefectPixelCorrection.h"
#include "IpDefectPixelCorrectionDlg.h"
#include "ImageViewerDlg.h"


// CImageViewer dialog

IMPLEMENT_DYNAMIC(CImageViewerDlg, CDialog)

CImageViewerDlg::CImageViewerDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CImageViewerDlg::IDD, pParent)
{
}

CImageViewerDlg::CImageViewerDlg(CString _fp, int _w, int _h)
{
	strFilePath = _fp;
	nWidth = _w;
	nHeight = _h;
}

CImageViewerDlg::~CImageViewerDlg()
{
}

void CImageViewerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_VIEW, m_viewCtrl);
}


BEGIN_MESSAGE_MAP(CImageViewerDlg, CDialog)
	ON_WM_PAINT()
	ON_STN_CLICKED(IDC_VIEW, &CImageViewerDlg::OnStnClickedView)
END_MESSAGE_MAP()


// CImageViewer message handlers

BOOL CImageViewerDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here
/*
	HBITMAP hBitmap = NULL;
	hBitmap = (HBITMAP)LoadImage(
		NULL,
		strFilePath,
		IMAGE_BITMAP,
		0,0,LR_LOADFROMFILE
	); 
	m_viewCtrl.SetBitmap(hBitmap);
	if(!hBitmap)::CloseHandle(hBitmap);
*/
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CImageViewerDlg::OnStnClickedView()
{
	// TODO: Add your control notification handler code here
}

void CImageViewerDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: Add your message handler code here
	// Do not call CDialog::OnPaint() for painting messages
	HBITMAP hBitmap = NULL;
	hBitmap = (HBITMAP)LoadImage(
		NULL,
		strFilePath,
		IMAGE_BITMAP,
		0,0,LR_LOADFROMFILE
	); 
	
	BITMAP bmp = { 0 };
	GetObject(hBitmap, sizeof(BITMAP), &bmp);

	HDC hDc;
	hDc = CreateCompatibleDC(dc);
	SelectObject(hDc, hBitmap);

	BitBlt(dc.m_hDC, 0, 0, 320, 320, hDc, 0, 0, SRCCOPY);

	CDialog::OnPaint();
}
