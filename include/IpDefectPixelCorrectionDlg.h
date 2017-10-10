
// IpDefectPixelCorrectionDlg.h : header file
//

#pragma once
#include "afxwin.h"
#include "afxcmn.h"

// CIpDefectPixelCorrectionDlg dialog
class CIpDefectPixelCorrectionDlg : public CDialog
{
// Construction
public:
	CIpDefectPixelCorrectionDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_IPDEFECTPIXELCORRECTION_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

public:
	CString m_edFilePath;	
	CString m_edInputType;	// 8bit Gray or 24bit RGB
	CComboBox m_outputList;	// 8bit Gray or 24bit RGB
	CSliderCtrl m_ctrlHotSlider;
	CSliderCtrl m_crtlDeadSlider;
	afx_msg void OnBnClickedFileopen();
	afx_msg void OnBnClickedCorrection();
	afx_msg void OnCbnSelchangeOutputlist();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnEnChangeHotvalue();
	afx_msg void OnEnChangeDeadvalue();

private:
	int m_width;
	int m_height;
	int m_bitCount;
	
	void MakeImageOutputList(int);
	void ResultDialogShow();
};
