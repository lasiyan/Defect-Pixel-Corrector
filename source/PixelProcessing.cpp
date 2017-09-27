#include "StdAfx.h"
#include "PixelProcessing.h"

CPixelProcessing::CPixelProcessing(CMyImage* pFile, int _sel, BYTE _hv, BYTE _dv)
{
	nWidth = pFile->GetWidth();
	nHeight = pFile->GetHeight();
	nBitDepth = pFile->GetBitCount();
	nOutput = _sel;
	nHotVal = _hv;
	nDeadVal = _dv;

	g_cor.clear();

	// Make Result Image
	pResult = new CMyImage(
		pFile->GetFileName(),
		nWidth,
		nHeight,
		nOutput
	);
}
CMyImage* CPixelProcessing::start(BYTE** szOrigin)
{
	szResult = pResult->GetPixels2D();

	if(nBitDepth == 8)
	{
		_8bitImageCorrection(szOrigin);
	}
	else if(nBitDepth == 24 && nOutput == 8)
	{
		BYTE** szGray = new BYTE*[nHeight];
		szGray[0] = new BYTE[nWidth * nHeight];
		for(int i = 1; i < nHeight; i++) szGray[i] = szGray[i-1] + nWidth;
		ZeroMemory(szGray[0], nWidth * nHeight);
		RGBto8BitGray(szGray, szOrigin, nWidth, nHeight);
		
		_8bitImageCorrection(szGray);

		delete[] szGray[0];
		delete[] szGray;
	}
	else if(nBitDepth == 24 && nOutput == 24)
	{
		_24bitImageCorrection(szOrigin);
	}	
	
	return pResult;
}

void CPixelProcessing::_8bitImageCorrection(BYTE** szOrigin)
{
	// Start Median Filter
	int nRow, nCol;
	bool isDefect;
	vector<BYTE> vecMask(MASK_SIZE * MASK_SIZE, 0);

	for(nRow = 1; nRow < nHeight - 1; nRow++)
	{
		for(nCol = 1; nCol < nWidth - 1; nCol++)
		{
			isDefect = GetPixelState(nRow, nCol, szOrigin[nRow][nCol]);
			if(isDefect == true)
			{
				for(int y = -1; y < MASK_SIZE - 1; y++)
				{
					for(int x = -1; x < MASK_SIZE - 1; x++)
					{
						vecMask[(y + 1) * (MASK_SIZE - 1) + (x + 1)] = szOrigin[nRow + y][nCol + x];
					}
				}

				szResult[nRow][nCol] = GetMedian(vecMask);
			}
			else
			{
				szResult[nRow][nCol] = szOrigin[nRow][nCol];
			}
		}
	}
}

void CPixelProcessing::_24bitImageCorrection(BYTE** szOrigin)
{
	int nRow, nCol;
	bool isDefect;
	vector<BYTE> vecBMask(MASK_SIZE * MASK_SIZE, 0);
	vector<BYTE> vecGMask(MASK_SIZE * MASK_SIZE, 0);
	vector<BYTE> vecRMask(MASK_SIZE * MASK_SIZE, 0);

	// Make 24bit Image
	Pixel** szRGB = new Pixel*[nHeight];
	szRGB[0] = new Pixel[nWidth * nHeight];
	for(int i = 1; i < nHeight; i++)
	{
		szRGB[i] = szRGB[i - 1] + nWidth;
	}

	int k;
	for(int j = 0; j < nHeight; j++)
	{
		for(int i = 0; i < nWidth; i++)
		{
			k = i * 3;
			szRGB[j][i].Set(
				szOrigin[j][k],
				szOrigin[j][k + 1],
				szOrigin[j][k + 2]
			);		
		}
	}


	for(nRow = 1; nRow < nHeight - 1; nRow++)
	{
		for(nCol = 1; nCol < nWidth - 1; nCol++)
		{
			if(nRow == 42 && nCol >= 116)
				TRACE(_T("a\n"));
			isDefect = GetPixelState(nRow, nCol, szRGB[nRow][nCol]);


			if(isDefect == true)
			{
				for(int y = -1; y < MASK_SIZE - 1; y++)
				{
					for(int x = -1; x < MASK_SIZE - 1; x++)
					{
						vecBMask[(y + 1) * (MASK_SIZE - 1) + (x + 1)] = szRGB[nRow + y][nCol + x].GetBlue();
						vecGMask[(y + 1) * (MASK_SIZE - 1) + (x + 1)] = szRGB[nRow + y][nCol + x].GetGreen();
						vecRMask[(y + 1) * (MASK_SIZE - 1) + (x + 1)] = szRGB[nRow + y][nCol + x].GetRed();
					}
				}

				szResult[nRow][3 * nCol] = GetMedian(vecBMask);
				szResult[nRow][3 * nCol + 1] = GetMedian(vecGMask);
				szResult[nRow][3 * nCol + 2] = GetMedian(vecRMask);
			}
			else
			{
				szResult[nRow][3 * nCol] = szRGB[nRow][nCol].GetBlue();
				szResult[nRow][3 * nCol + 1] = szRGB[nRow][nCol].GetGreen();
				szResult[nRow][3 * nCol + 2] = szRGB[nRow][nCol].GetRed();
			}
		}
	}

 	delete[] szRGB[0];
	delete[] szRGB;
}




// Bubble Sort
int CPixelProcessing::GetMedian(vector<BYTE> vec)
{
	int nSize = MASK_SIZE * MASK_SIZE;
	int nMedian = MASK_SIZE * MASK_SIZE / 2;

	sort(vec.begin(), vec.end());	// Mask[0][0] ~ [2][2]
	
	return vec[nMedian];
}


bool CPixelProcessing::GetPixelState(int _row, int _col, int curPix)
{
	_row = (nHeight - 1) - _row;
	if(curPix >= 0 && curPix <= nDeadVal)
	{
		g_cor.push_back(_Point(_row, _col, DEAD_PIXEL));
		return true;
	}
	if(curPix <= 255 && curPix >= nHotVal)
	{
		g_cor.push_back(_Point(_row, _col, HOT_PIXEL));
		return true;
	}
	return false;
}

bool CPixelProcessing::GetPixelState(int _row, int _col, Pixel curPix)
{
	BYTE b = curPix.GetBlue();
	BYTE g = curPix.GetGreen();
	BYTE r = curPix.GetRed();
	_row = (nHeight - 1) - _row;
	if( (b >= 0 && b <= nDeadVal) &&
		(g >= 0 && g <= nDeadVal) &&
		(r >= 0 && r <= nDeadVal)
	)
	{
		g_cor.push_back(_Point(_col, _row, DEAD_PIXEL));
 		return true;
	}
	if( (b >= nHotVal && b <= 255) &&
		(g >= nHotVal && g <= 255) &&
		(r >= nHotVal && r <= 255)
	)
	{
		g_cor.push_back(_Point(_col, _row, HOT_PIXEL));
		return true;
	}
	return false;
}