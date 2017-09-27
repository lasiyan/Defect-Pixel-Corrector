#pragma once
#include "afxwin.h"


// CImageViewer dialog

class CImageViewerDlg : public CDialog
{
	DECLARE_DYNAMIC(CImageViewerDlg)

public:
	CImageViewerDlg(CWnd* pParent = NULL);   // standard constructor
	CImageViewerDlg(CString, int, int);
	virtual ~CImageViewerDlg();
	
// Dialog Data
	enum { IDD = IDD_VIEW_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
	virtual BOOL OnInitDialog();
	afx_msg void OnStnClickedView();
	CStatic m_viewCtrl;
	CStatic *pViewCrtl;

private:
	CString strFilePath;
	int nWidth;
	int nHeight;


public:
};
