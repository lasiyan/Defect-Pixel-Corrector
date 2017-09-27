#include "StdAfx.h"
#include "IpActivity.h"

CIpActivity::~CIpActivity(void){}

BOOL CIpActivity::Execute(CMyImage* pFile)
{
	// 1. Load File
	if(!LoadFile(pFile))
	{
		AfxMessageBox(_T("Load File Error"));
		return FALSE;
	}
	
	// 2. Start Defect Pixel Correction
	CPixelProcessing Processing(pFile, nOutputType, nHotValue, nDeadValue);
	pResult = Processing.start(pFile->GetPixels2D());
	delete(pFile);
	
	// 3. Save File
	if(!SaveFile(pResult))
	{
		AfxMessageBox(_T("Save File Error"));
		return FALSE;
	}
	delete(pResult);

	return TRUE;
}

BOOL CIpActivity::LoadFile(CMyImage *pFile)
{
	FILE* fp;

	CT2A ascii(pFile->GetFileName(), CP_UTF8);	// CString to const char*
	if((fopen_s(&fp, ascii.m_psz, "rb")) != 0)
	{
		AfxMessageBox(_T("fopen_s() Error"));
		return FALSE;
	}

	if(pFile->GetBitCount() == 8)
	{
		fseek(fp, sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + sizeof(RGBQUAD) * 256, SEEK_SET);
		fread(pFile->GetPixels(), 1, pFile->GetImageSize(), fp);
	}
	if(pFile->GetBitCount() == 24)
	{
		fseek(fp, sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER), SEEK_SET);
		fread(pFile->GetPixels(), 3, pFile->GetImageSize(), fp);
	}

	fclose(fp);
	return TRUE;
}

BOOL CIpActivity::SaveFile(CMyImage *pResult)
{
	FILE* fp2;

	CString strNewFileName = MakeFileName(pResult->GetFileName());
	CT2A ascii(strNewFileName, CP_UTF8);	// CString to const char*
	if((fopen_s(&fp2, ascii.m_psz, "wb")) != 0)
	{
		AfxMessageBox(_T("fopen_s() Error"));
		return FALSE;
	}

	pResult->MakeBitmap();

	// Write Bitmap Headers
	fwrite(&pResult->bfh, sizeof(BITMAPFILEHEADER), 1, fp2);	
	fwrite(&pResult->bih, sizeof(BITMAPINFOHEADER), 1, fp2);

	switch(pResult->GetBitCount())
	{
	case 8:
		fwrite(&pResult->rgb, sizeof(RGBQUAD), 256, fp2);
		fwrite(pResult->GetPixels(), 1, pResult->GetImageSize(), fp2);
		break;
	case 24:
		fwrite(pResult->GetPixels(), 3, pResult->GetImageSize(), fp2);
		break;
	}
	fclose(fp2);

	return TRUE;
}

CString CIpActivity::MakeFileName(CString _strFileName)
{
	int nPos;
	CString originFileName = _strFileName;
	CString strNewPath;
	nPos = originFileName.ReverseFind('.');

	strNewPath = originFileName.Left(nPos) + _T("_result.bmp");

	return strNewPath;
}