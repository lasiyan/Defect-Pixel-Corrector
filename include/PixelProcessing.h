#pragma once

#include "MyImage.h"
#define MASK_SIZE 3
#define THRESHOLD 7

class Pixel {
private:
	BYTE blue;
	BYTE green;
	BYTE red;

public:
	Pixel(){
		blue;
		green;
		red;
	}
	~Pixel(){}

	BYTE GetBlue()	{ return blue;	}
	BYTE GetGreen() { return green;	}
	BYTE GetRed()	{ return red;	}
	BYTE GetVal()	{ 
		int avg, gavg, bavg;
		avg = (blue + green + red) / 3;
		return avg;
	}
	void Set(BYTE _b, BYTE _g, BYTE _r)
	{
		blue = _b;
		green = _g;
		red = _r;
	}
	
	void GetNearAvg(Pixel top, Pixel bottom, Pixel left, Pixel right)
	{
		this->Set(
			(top.blue + bottom.blue + left.blue + right.blue) / 4,
			(top.green + bottom.green + left.green + right.green) / 4,
			(top.red + bottom.red + left.red + right.red) / 4
		);
	}

	Pixel operator-(const Pixel& right)
	{
		Pixel result;
		result.Set(
			abs(this->blue - right.blue),
			abs(this->green - right.green),
			abs(this->red - right.red)
		);
		return result;
	}
	
	bool DefectCheck(BYTE _min, BYTE _max)
	{
		if(	(this->blue >= _min && this->blue <= _max) &&
			(this->green >= _min && this->green <= _max) &&
			(this->red >= _min && this->red <= _max) )
		{
			return true;
		}
		else false;
	}

};

class CPixelProcessing
{
public:
	CPixelProcessing(CMyImage*, int, BYTE, BYTE);
	~CPixelProcessing(void){}

	CMyImage*& start(BYTE**);

private:
	int nWidth;
	int nHeight;
	int nBitDepth;
	int nOutput;
	BYTE nHotVal;
	BYTE nDeadVal;

	CMyImage *pResult;

	BYTE** szResult;
	BYTE *temp;

	void CorrectionGray(BYTE**);
	void CorrectionRGB(BYTE**);

	int GetMedian(vector<BYTE>);

	bool isDefect(int cx, int cy, BYTE cur, BYTE near);
	bool isDefect(int cx, int cy, Pixel cur, Pixel near);
};

