#pragma once

// Using Chk Memory Leak !
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
// _CrtSetBreakAlloc(number); // in program's First part and check CALL STACK


class CMyImage
{
public:
    CMyImage::CMyImage() : nWidth(0), nHeight(0), szPixelData(NULL)	{ }
	CMyImage::CMyImage(CString fn, int w, int h, int bc)
	{
		// Data Setting
		strFileName = fn;
		nWidth = w;
		nHeight = h;
		nBitCount = bc;
		int bcByte = nBitCount / 8;
		nWidthByte = nWidth * bcByte;
		nImageSize = nWidth * nHeight;
		nFileSize = nImageSize * bcByte;

		// Make Pixel Memory
		szPixelData = new BYTE*[nHeight];
		szPixelData[0] = new BYTE[nFileSize];
		for (int i = 1; i < nHeight; i++)
		{
			szPixelData[i] = szPixelData[i - 1] + nWidthByte;
		}
		memset(szPixelData[0], 0, nFileSize); // Init Pixels
	}
	CMyImage::~CMyImage()
	{
		if (szPixelData != NULL) {
			delete[] szPixelData[0];
			delete[] szPixelData;
		}
	}

protected:	// Class Member Variable
    // input
	CString strFileName;
	int     nWidth;
	int     nWidthByte;
	int     nHeight;
	int     nImageSize;	// width * height
    int     nFileSize;  // width * height * bitcount
    int     nBitCount;  // One Pixel's bit count
    
    // Pixel Data
    BYTE*   szHeader;
    BYTE**	szPixelData;


public:
	// For Using Bitmap
    BITMAPFILEHEADER bfh;
	BITMAPINFOHEADER bih;
    RGBQUAD rgb[256];
    
    // operator '=' re-define	( float image1 = int image2 )
	CMyImage& CMyImage::operator=(const CMyImage& img)
	{
		if (this == &img) return *this;

		CreateImage(img.nWidth, img.nHeight, img.nBitCount);
		memcpy(szPixelData[0], img.szPixelData[0], nFileSize);

		return *this;
	}
	// Make or Delete new image for = operator
	void CMyImage::CreateImage(int _width, int _height, int _bitcount)
	{
		DestroyImage();

		int width  = _width * (_bitcount / 8);
		int height = _height;

		szPixelData = new BYTE*[height];
		szPixelData[0] = new BYTE[width * height];

		for (int i = 1; i < height; i++)
		{
			szPixelData[i] = szPixelData[i - 1] + width;
		}
		memset(szPixelData[0], 0, width * height); // Init Pixels
	}
	void CMyImage::DestroyImage()
	{
		if (szPixelData != NULL) {
			delete[] szPixelData[0];
			delete[] szPixelData;
			szPixelData = NULL;
		}
		nWidth = nHeight = 0;
	}

    // Get Pixels 1D or 2D
	BYTE*	GetPixels() const {
		if (szPixelData) return szPixelData[0];
		else return NULL;
	}
	BYTE**	GetPixels2D() const { return szPixelData;   } 
	
	// private Setting
    CString     GetFileName()   { return strFileName;   }
    int         GetWidth()      { return nWidth;        }
    int         GetWidthByte()  { return nWidthByte;    }
    int         GetHeight()     { return nHeight;       }
    int         GetImageSize()  { return nImageSize;    }
    int         GetFileSize()   { return nFileSize;     }
    int         GetBitCount()   { return nBitCount;     }
    BYTE*       GetHeader()     { return szHeader;      }

    void    SetFileName (CString _fn)   { strFileName = _fn; }
    void    setWidth    (int _w)        { nWidth = _w;       }
    void    setWidthByte(int _wb)       { nWidthByte = _wb;  }
    void    setHeight   (int _h)        { nHeight = _h;      }
    void    setImageSize(int _is)       { nImageSize = _is;  }
    void    setFileSize (int _fs)       { nFileSize = _fs;   }
    void    setBitCount (int _bc)       { nBitCount = _bc;   }
    void    setHeader   (BYTE* _sz)     { szHeader = _sz;    }
    void    setPixelData(BYTE** _sz)    { szPixelData = _sz; }

    bool    IsValid()     const { return (szPixelData != NULL); }

	void CMyImage::MakeBitmap()
	{
		// Fill BITMAP struct
		memset(&bfh, 0, sizeof(BITMAPFILEHEADER));
		memset(&bih, 0, sizeof(BITMAPINFOHEADER));
		memset(&rgb, 0, sizeof(RGBQUAD) * 256);
		bfh.bfType = 19778;		// Ascii 'B' and 'M'
		bih.biBitCount = nBitCount;	// Need bit to draw per Pixel(1, 4, 8, 16, 24, 32)


		if( nBitCount == 8 )
		{	
			bfh.bfSize = (nWidth * nHeight) + sizeof(RGBQUAD) * 256 + sizeof(BITMAPINFOHEADER) + sizeof(BITMAPFILEHEADER);
			bfh.bfOffBits = 14 + 40 + 4 * 256;	// size of bf, bi, rgbquad*4 // 24bit not use color table, so 54 val;
		}
		if( nBitCount == 24 )
		{
			bfh.bfSize = (nWidth * nHeight * 3) + sizeof(BITMAPINFOHEADER) + sizeof(BITMAPFILEHEADER);
			bfh.bfOffBits = 14 + 40;	// size of bf, bi, rgbquad*4 // 24bit not use color table, so 54 val;
		}

		bih.biSize = 40;	// Size of BITMAPINFOHEADER
		bih.biWidth = nWidth;	// Bitmap Width size(px)
		bih.biHeight = nHeight;	// Bitmap Height size(px)
		bih.biPlanes = 1;	// Always 1
		bih.biSizeImage = (((nWidth * 24 + 31) & ~31) >> 3) * nHeight;
		bih.biXPelsPerMeter = 0;
		bih.biYPelsPerMeter = 0;

		for(int i = 0; i < 256; i++)
		{
			rgb[i].rgbBlue = i;
			rgb[i].rgbGreen = i;
			rgb[i].rgbRed = i;
			rgb[i].rgbReserved = 0;
		}
	}
};

// Useful Method Implement
inline BYTE Limit(const BYTE& value)
{
	return ((value > 255) ? 255 : ((value < 0) ? 0 : value));
}

inline void RGBto8BitGray(BYTE** sz8Bit, BYTE** sz24Bit, int nWidth, int nHeight)
{
	int k;
	int gray;
	for(int j = 0; j < nHeight; j++)
	{
		for(int i = 0; i < nWidth; i++)
		{
			k = i * 3;
			gray = (
			sz24Bit[j][k] +	// B
			sz24Bit[j][k+1] +	// G
			sz24Bit[j][k+2]	// R
			) / 3;
			
			sz8Bit[j][i] = (BYTE)gray;
		}
	}
}
