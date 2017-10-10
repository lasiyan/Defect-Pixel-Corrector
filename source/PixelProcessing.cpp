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
	
	// Make Result Image
	pResult = new CMyImage(
		pFile->GetFileName(),
		nWidth,
		nHeight,
		nOutput
	);
}

// PixelProcessing Class's main
CMyImage*& CPixelProcessing::start(BYTE** szOrigin)
{
	szResult = pResult->GetPixels2D();

	if(nBitDepth == 8)
	{
		CorrectionGray(szOrigin);
	}
	else if(nBitDepth == 24 && nOutput == 8)
	{
		BYTE** szGray;
		RGB_to_Gray(szGray, szOrigin, nWidth, nHeight);
		CorrectionGray(szGray);
		DestroyImage(szGray);
	}
	else if(nBitDepth == 24 && nOutput == 24)
	{
		CorrectionRGB(szOrigin);
	}

	return pResult;
}

void CPixelProcessing::CorrectionGray(BYTE** szOrigin)
{
	ReverseImage(szOrigin, nWidth, nHeight);

	for(int i = 0; i < nWidth; i++)
	{
		szResult[0][i] = szOrigin[0][i];			
		szResult[nHeight-1][i] = szOrigin[nHeight-1][i];			
	}
	for(int i = 0; i < nHeight; i++)
	{
		szResult[i][0] = szOrigin[i][0];			
		szResult[i][nWidth-1] = szOrigin[i][nWidth-1];	
	}

	// Start Median Filter
	int idx, nearSum;
	BYTE curVal, nearAvg;	
	vector<BYTE> vecMask(MASK_SIZE * MASK_SIZE, 0);

	for(int y = 1; y < nHeight - 1; y++)
	{
		for(int x = 1; x < nWidth - 1; x++)
		{
			if(x == 1591 && y == 825)
				TRACE("val:	%d\n", szOrigin[y][x]);
			curVal = szOrigin[y][x];

			// Avg (top, bottom, left, right) value
			nearSum = szOrigin[y-1][x] + szOrigin[y+1][x] + szOrigin[y][x-1] + szOrigin[y][x+1];
			nearAvg = nearSum / 4;
			
			if(isDefect(y, x, curVal, nearAvg))
			{
				idx = 0;
				for(int j = -1; j < MASK_SIZE - 1; j++)
				{
					for(int i = -1; i < MASK_SIZE - 1; i++)
					{
						vecMask[idx++] = szOrigin[y + j][x + i];						
					}
				}
				szResult[y][x] = GetMedian(vecMask);
			}
			else
			{
				szResult[y][x] = szOrigin[y][x];
			}
		}
	}
	
	ReverseImage(szResult, nWidth, nHeight);
}

void CPixelProcessing::CorrectionRGB(BYTE** szOrigin)
{
		
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
			szRGB[j][i].Set(szOrigin[j][k], szOrigin[j][k + 1], szOrigin[j][k + 2]);		
			szResult[j][k] = szRGB[j][i].GetBlue();
			szResult[j][k + 1] = szRGB[j][i].GetGreen();
			szResult[j][k + 2] = szRGB[j][i].GetRed();
		}
	}

	ReverseImage(szRGB, nWidth, nHeight);

	// Start
	Pixel curVal, nearAvg;
	BYTE filterIdx;
	vector<BYTE> vecBMask(MASK_SIZE * MASK_SIZE, 0);
	vector<BYTE> vecGMask(MASK_SIZE * MASK_SIZE, 0);
	vector<BYTE> vecRMask(MASK_SIZE * MASK_SIZE, 0);	
	for(int y = 1; y < nHeight - 1; y++)
	{
		for(int x = 1; x < nWidth - 1; x++)
		{
			if(y == 611 && x == 616)
				TRACE(_T("r: %d g: %d b: %d\n"), szRGB[y][x].GetRed(),szRGB[y][x].GetGreen(),szRGB[y][x].GetBlue());
			curVal = szRGB[y][x];
			if(curVal.GetVal() > nDeadVal && curVal.GetVal() < nHotVal)
			{
				continue;
			}
			else
			{

				nearAvg.GetNearAvg(szRGB[y-1][x], szRGB[y+1][x], szRGB[y][x-1], szRGB[y][x+1]);

				filterIdx = 0;
				if(isDefect(y, x, curVal, nearAvg))
				{
					for(int j = -1; j < MASK_SIZE - 1; j++)
					{
						for(int i = -1; i < MASK_SIZE - 1; i++)
						{
							vecBMask[filterIdx] = szRGB[y + j][x + i].GetBlue();
							vecGMask[filterIdx] = szRGB[y + j][x + i].GetGreen();
							vecRMask[filterIdx++] = szRGB[y + j][x + i].GetRed();
						}
					}

					szResult[y][3 * x] = GetMedian(vecBMask);
					szResult[y][3 * x + 1] = GetMedian(vecGMask);
					szResult[y][3 * x + 2] = GetMedian(vecRMask);
				}
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

// Check Current pixel is Defect pixel
bool CPixelProcessing::isDefect(int cx, int cy, BYTE _cur, BYTE _near)
{
	// If Current Pixel and near Pixel's difference are less THRESHOLD(5)
	// the Pixel doesn't count
	int differ = abs(_cur - _near);

	if(	differ < THRESHOLD )
	{
		return false;
	}

	if(_cur >= 0 && _cur <= nDeadVal)
	{
		g_cor.push_back(_Point(cy, cx, DEAD_PIXEL, _cur));
		return true;
	}
	if(_cur <= 255 && _cur >= nHotVal)
	{
		g_cor.push_back(_Point(cy, cx, HOT_PIXEL, _cur));
		return true;
	}
	return false;
}
bool CPixelProcessing::isDefect(int cx, int cy, Pixel _cur, Pixel _near)
{
	// If Current Pixel and Pre Pixel's difference are less <THRESHOLD(5)>
	// the Pixel doesn't count
	Pixel sub = _cur - _near;
//	if( sub.GetBlue() < THRESHOLD || sub.GetRed() < THRESHOLD || sub.GetGreen() < THRESHOLD)
	if( sub.GetVal() < THRESHOLD)
	{
		return false;
	}

	if( _cur.DefectCheck(0, nDeadVal) )
	{
		g_cor.push_back(_Point(cx, cy, DEAD_PIXEL, _cur.GetVal()));
		return true;
	}
	if( _cur.DefectCheck(nHotVal, 255) )
	{
		g_cor.push_back(_Point(cx, cy, HOT_PIXEL, _cur.GetVal()));
		return true;
	}
	return false;
}