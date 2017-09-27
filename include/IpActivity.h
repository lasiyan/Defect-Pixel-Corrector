#pragma once
#include "MyImage.h"
#include "PixelProcessing.h"

class CIpActivity
{

public:
	CIpActivity(int _sel, BYTE _hVal, BYTE _dVal)
	{
		if(_sel == 0)	nOutputType = 8;
		if(_sel == 1)	nOutputType = 24;
		nHotValue = _hVal;
		nDeadValue = _dVal;
	}
	~CIpActivity(void);

private:
	int nOutputType;
	BYTE nHotValue;
	BYTE nDeadValue;

	BOOL LoadFile(CMyImage*);
	BOOL SaveFile(CMyImage*);
	CString MakeFileName(CString);

	CMyImage* pResult;
	BYTE** szGrayImage;

public:

	BOOL Execute(CMyImage*);
};
