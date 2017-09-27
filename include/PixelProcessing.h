#pragma once

#include "MyImage.h"
#define MASK_SIZE 5

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
	void Set(int _b, int _g, int _r)
	{
		blue = _b;
		green = _g;
		red = _r;
	}
};

class CPixelProcessing
{
public:
	CPixelProcessing(CMyImage*, int, BYTE, BYTE);
	~CPixelProcessing(void){}

	CMyImage* start(BYTE**);

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

	void _8bitImageCorrection(BYTE**);
	void _24bitImageCorrection(BYTE**);

	int GetMedian(vector<BYTE>);
	bool GetPixelState(int, int, int);
	bool GetPixelState(int, int, Pixel);
};
