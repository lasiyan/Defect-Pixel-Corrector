// ResultDlg.cpp : implementation file
//

#include "stdafx.h"
#include "IpDefectPixelCorrection.h"
#include "ResultDlg.h"


// CResultDlg dialog

IMPLEMENT_DYNAMIC(CResultDlg, CDialog)

CResultDlg::CResultDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CResultDlg::IDD, pParent)
{
}

CResultDlg::~CResultDlg()
{
}

void CResultDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_RESULTLIST, m_listControl);
}


BEGIN_MESSAGE_MAP(CResultDlg, CDialog)
	ON_WM_SIZE()
END_MESSAGE_MAP()


// CResultDlg message handlers

BOOL CResultDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	CRect rect;
	this->GetClientRect(rect);
	this->MoveWindow(rect);	

	// TODO:  Add extra initialization here
	// if no defect pixel
	if(g_cor.size() == 0)
	{
		m_listControl.InsertColumn(0, _T("Result"), LVCFMT_CENTER, 240);
		m_listControl.InsertItem(0, _T("There are no defect pixels"));
	}
	else
	{
		CString strMsg, strPixel;

		m_listControl.SetExtendedStyle(LVS_EX_FULLROWSELECT);

		m_listControl.InsertColumn(0, _T("X pos"), LVCFMT_CENTER, 60);
		m_listControl.InsertColumn(1, _T("Y pos"), LVCFMT_CENTER, 60);
		m_listControl.InsertColumn(2, _T("Pixel Type"), LVCFMT_CENTER, 120);

		for(int i = 0; i < g_cor.size(); i++)
		{
			if(g_cor[i].state == HOT_PIXEL)  strPixel.Format(_T("Hot Pixel(%d)"), g_cor[i].val);
			if(g_cor[i].state == DEAD_PIXEL) strPixel.Format(_T("Dead Pixel(%d)"), g_cor[i].val);

			strMsg.Format(_T("%d"),g_cor[i].x);
			m_listControl.InsertItem(i, strMsg);
			strMsg.Format(_T("%d"),g_cor[i].y);
			m_listControl.SetItemText(i, 1, strMsg);
			m_listControl.SetItemText(i, 2, strPixel);
		}
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CResultDlg::PostNcDestroy()
{
	// TODO: Add your specialized code here and/or call the base class
	delete(this);
	CDialog::PostNcDestroy();
}

void CResultDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);
	if(m_listControl.GetSafeHwnd()) {
		m_listControl.SetWindowPos(NULL,0,0,cx,cy,SWP_NOZORDER);
	}
	// TODO: Add your message handler code here
}
