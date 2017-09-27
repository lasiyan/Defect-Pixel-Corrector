
// IpDefectPixelCorrectionDlg.cpp : implementation file
//

#include "stdafx.h"
#include "IpDefectPixelCorrection.h"
#include "IpDefectPixelCorrectionDlg.h"
#include "MyImage.h"
#include "IpActivity.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CIpDefectPixelCorrectionDlg dialog




CIpDefectPixelCorrectionDlg::CIpDefectPixelCorrectionDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CIpDefectPixelCorrectionDlg::IDD, pParent)
	, m_edFilePath(_T(""))
	, m_edInputType(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	pDlg = NULL;
}

void CIpDefectPixelCorrectionDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_FILEPATH, m_edFilePath);
	DDX_Text(pDX, IDC_INPUTBIT, m_edInputType);
	DDX_Control(pDX, IDC_OUTPUTLIST, m_outputList);
	DDX_Control(pDX, IDC_HOTSLIDER, m_ctrlHotSlider);
	DDX_Control(pDX, IDC_DEADSLIDER, m_crtlDeadSlider);
}

BEGIN_MESSAGE_MAP(CIpDefectPixelCorrectionDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_FILEOPEN, &CIpDefectPixelCorrectionDlg::OnBnClickedFileopen)
	ON_BN_CLICKED(IDC_CORRECTION, &CIpDefectPixelCorrectionDlg::OnBnClickedCorrection)
	ON_CBN_SELCHANGE(IDC_OUTPUTLIST, &CIpDefectPixelCorrectionDlg::OnCbnSelchangeOutputlist)
	ON_WM_HSCROLL()
	ON_EN_CHANGE(IDC_HOTVALUE, &CIpDefectPixelCorrectionDlg::OnEnChangeHotvalue)
	ON_EN_CHANGE(IDC_DEADVALUE, &CIpDefectPixelCorrectionDlg::OnEnChangeDeadvalue)
END_MESSAGE_MAP()


// CIpDefectPixelCorrectionDlg message handlers

BOOL CIpDefectPixelCorrectionDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
	screenX = GetSystemMetrics(SM_CXSCREEN);
	screenY = GetSystemMetrics(SM_CYSCREEN);
	screenX = screenX / 2 + 50;
	screenY = screenY / 2 - 50;

	m_ctrlHotSlider.SetRange(0, 255);
	m_crtlDeadSlider.SetRange(0, 255);
	m_ctrlHotSlider.SetPos(255);
	SetDlgItemInt(IDC_HOTVALUE, 255);
	SetDlgItemInt(IDC_DEADVALUE, 0);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CIpDefectPixelCorrectionDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CIpDefectPixelCorrectionDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CIpDefectPixelCorrectionDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}
void CIpDefectPixelCorrectionDlg::OnCbnSelchangeOutputlist(){}

void CIpDefectPixelCorrectionDlg::OnBnClickedFileopen()
{
	// Open File Dialog
	TCHAR szFilter[] = _T("Bmp Files(*.bmp)|*.bmp|");
	CFileDialog dlg(TRUE, NULL, NULL, OFN_HIDEREADONLY, szFilter);

	if(dlg.DoModal() == IDOK)
	{
		m_edFilePath = dlg.GetPathName();		
		UpdateData(FALSE);
		// get picture-control dc
		if((hBitmap = (HBITMAP)LoadImage(NULL, m_edFilePath, IMAGE_BITMAP, 0, 0,
			LR_LOADFROMFILE | LR_CREATEDIBSECTION | LR_DEFAULTSIZE)) == NULL)
		{
			AfxMessageBox(_T("This File is Broken BMP"));
			return;
		}
		else
		{
			pBitmap = CBitmap::FromHandle(hBitmap);
			pBitmap->GetBitmap(&Bitmap);

			m_width = Bitmap.bmWidth;
			m_height = Bitmap.bmHeight;
			m_bitCount = Bitmap.bmBitsPixel;

			MakeImageOutputList(m_bitCount);
		}
	}
}
void CIpDefectPixelCorrectionDlg::OnBnClickedCorrection()
{
	if(m_ctrlHotSlider.GetPos() < 200)
	{
		if(MessageBox(
			_T("If you set Hot Pixel value less 200, the Correction will take a long time\r\nContinue?"),
			_T("Warning !"), MB_YESNO) == IDNO)	return;
	}
	else if(m_crtlDeadSlider.GetPos() > 50)
	{
		if(MessageBox(
			_T("If you set Dead Pixel value over 50, the Correction will take a long time\r\nContinue?"),
			_T("Warning !"), MB_YESNO) == IDNO) return;
	}

	CMyImage* pFile = new CMyImage(m_edFilePath, m_width, m_height, m_bitCount);
	CIpActivity activity(
		m_outputList.GetCurSel(),
		GetDlgItemInt(IDC_HOTVALUE),
		GetDlgItemInt(IDC_DEADVALUE)
	);

	if(!activity.Execute(pFile))
	{
		AfxMessageBox(_T("Detection Fail"));
		return;
	}
	else
	{
		ResultDialogShow();
	}
}

void CIpDefectPixelCorrectionDlg::ResultDialogShow()
{
	pDlg = new CResultDlg(this);  

	if ( pDlg )  
	{  
		int height = ((g_cor.size() * 23) > 100) ? (g_cor.size() * 23) : 100;
		pDlg->Create( IDD_RESULT_DIALOG );
		pDlg->ShowWindow( SW_SHOW );
		pDlg->MoveWindow(screenX,screenY,256,height);
	}
}

void CIpDefectPixelCorrectionDlg::MakeImageOutputList(int _bc)
{
	m_edInputType = "";
	m_outputList.ResetContent();
	
	switch(_bc)
	{
	case 8:
		m_edInputType = _T("8 Bit Grayscale Image");
	
		m_outputList.AddString(_T("8 Bit Grayscale Image"));
		m_outputList.SetCurSel(0);
		break;
	case 24:
		m_edInputType = _T("24 Bit RGB Image");

		m_outputList.InsertString(0, _T("8 Bit Grayscale Image"));
		m_outputList.InsertString(1, _T("24 Bit RGB Image"));
		m_outputList.SetCurSel(0);
		break;
	}

	UpdateData(FALSE);
}

void CIpDefectPixelCorrectionDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	if(pScrollBar != NULL && pScrollBar->m_hWnd == m_ctrlHotSlider.m_hWnd)
	{
		SetDlgItemInt(IDC_HOTVALUE, m_ctrlHotSlider.GetPos());
	}

	CDialog::OnHScroll(nSBCode, nPos, pScrollBar);

	if(pScrollBar != NULL && pScrollBar->m_hWnd == m_crtlDeadSlider.m_hWnd)
	{		
		SetDlgItemInt(IDC_DEADVALUE, m_crtlDeadSlider.GetPos());
	}

	CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
}

void CIpDefectPixelCorrectionDlg::OnEnChangeHotvalue()
{
	int val = GetDlgItemInt(IDC_HOTVALUE);
	if(val > 255)
	{
		AfxMessageBox(_T("Max Value is 255"));
		SetDlgItemInt(IDC_HOTVALUE, 255);
		val = 255;
	}
	
	m_ctrlHotSlider.SetPos(val);
}

void CIpDefectPixelCorrectionDlg::OnEnChangeDeadvalue()
{
	int val = GetDlgItemInt(IDC_DEADVALUE);
	if(val > 255)
	{
		AfxMessageBox(_T("Max Value is 255"));
		SetDlgItemInt(IDC_DEADVALUE, 255);
		val = 255;
	}

	m_crtlDeadSlider.SetPos(val);
}
