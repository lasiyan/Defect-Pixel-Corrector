#pragma once
#include "afxwin.h"
#include "afxcmn.h"


// CResultDlg dialog

class CResultDlg : public CDialog
{
	DECLARE_DYNAMIC(CResultDlg)

public:
	CResultDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CResultDlg();

// Dialog Data
	enum { IDD = IDD_RESULT_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	CListCtrl m_listControl;
protected:
	virtual void PostNcDestroy();
};
