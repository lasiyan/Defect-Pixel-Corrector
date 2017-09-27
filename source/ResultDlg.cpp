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
END_MESSAGE_MAP()


// CResultDlg message handlers

BOOL CResultDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	
	// TODO:  Add extra initialization here
	CString strMsg, strPixel;

	m_listControl.SetExtendedStyle(LVS_EX_FULLROWSELECT);

	m_listControl.InsertColumn(0, _T("X pos"), LVCFMT_CENTER, 60);
	m_listControl.InsertColumn(1, _T("Y pos"), LVCFMT_CENTER, 60);
	m_listControl.InsertColumn(2, _T("Pixel Type"), LVCFMT_CENTER, 120);

	for(int i = 0; i < g_cor.size(); i++)
	{
		if(g_cor[i].state == HOT_PIXEL)  strPixel = _T("Hot Pixel");
		if(g_cor[i].state == DEAD_PIXEL) strPixel = _T("Dead Pixel");

		strMsg.Format(_T("%d"),g_cor[i].x);
		m_listControl.InsertItem(i, strMsg);
		strMsg.Format(_T("%d"),g_cor[i].y);
		m_listControl.SetItemText(i, 1, strMsg);
		m_listControl.SetItemText(i, 2, strPixel);
	}
	
	// if no defect pixel
	if(g_cor.size() == 0)
	{
		while(m_listControl.GetHeaderCtrl()->GetItemCount() > 0) m_listControl.DeleteColumn(0);
		m_listControl.InsertColumn(0, _T("Result"), LVCFMT_CENTER, 240);
		m_listControl.InsertItem(0, _T("There are no defect pixels"));
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
