#include "pch.h"
#include "ImgHelper.h"


#ifndef UNDER_CE
#include <atlimage.h>
#endif

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

namespace MFC {
	namespace UI {


#ifdef _WIN32_WCE
#ifdef SUPPORT_IMAGING
#include <imaging.h>
#include <initguid.h>
#include <imgguids.h>
/// 
/// \brief <pre>
/// �̹��� ������ �ε��Ѵ�. CE 5.0�̻��� ���������� ����Ǹ� �����翡�� 
/// ���̺귯���� �����ϴ��� ���θ� Ȯ���ؾ��Ѵ�.
/// </pre>
/// \param   hWnd			- Window Handle. NULL�̸� �ȵȴ�.
/// \param   a_pstrFileName	- �ε��� ���� PNG, JPG, ...
/// \return  HBITMAP		- Load Image Bitmapt Handle
/// 
HBITMAP	CImgHelper::LoadImageFile(HWND a_hWnd, TCHAR* a_pstrFileName)
{
	IImagingFactory*	pImgFactory = NULL;
    IImage*				pImage = NULL;
	ImageInfo			pImgInfo;
	HBITMAP				hBmpImg;
    CoInitializeEx(NULL, COINIT_MULTITHREADED);
	HDC hDC = GetDC(a_hWnd);
    if( SUCCEEDED(CoCreateInstance (CLSID_ImagingFactory, NULL, CLSCTX_INPROC_SERVER, IID_IImagingFactory, (void **)&pImgFactory)) ) {
        if (SUCCEEDED(pImgFactory->CreateImageFromFile(a_pstrFileName, &pImage))) {	
			pImage->GetImageInfo(&pImgInfo);
			RECT rc = { 0, 0, pImgInfo.Width, pImgInfo.Height };
			HDC	hMemDC = CreateCompatibleDC(hDC);
			hBmpImg = CreateCompatibleBitmap(hDC, pImgInfo.Width, pImgInfo.Height);
			HBITMAP hOldBmp = (HBITMAP)SelectObject(hMemDC, hBmpImg);
			pImage->Draw(hMemDC, &rc, NULL);
            pImage->Release();
			pImage = NULL;
			SelectObject(hMemDC, hOldBmp);
			DeleteDC(hMemDC);
        }
        pImgFactory->Release();pImgFactory = NULL;
    }
	ReleaseDC(a_hWnd, hDC);
    CoUninitialize();
	return hBmpImg;
}
#endif
/// 
/// \brief <pre>
/// BMP�̹����� �ε��Ѵ�.
/// </pre>
/// \param   a_pstrFileName	- Bitmap Full Path File Name
/// \return  HBITMAP	- Load Bitmap Handle, must release by DeleteObject
/// 
HBITMAP	CImgHelper::LoadBMPFile(TCHAR* a_pstrFileName)
{
	HANDLE hBitmap = (HBITMAP)SHLoadDIBitmap(a_pstrFileName); 
	return hBitmap ? (HBITMAP)hBitmap : NULL;
}
#else
/// 
/// \brief <pre>
/// Load the image file.
/// </pre>
/// \param   a_pstrFileName - Load File Nmae BMP (bitmap), JPEG, WMF (metafile), ICO (icon), or GIF format
/// \return  HBITMAP - Load Bitmap Handle, 
/// 
HBITMAP	CImgHelper::LoadImageFile(TCHAR* a_pstrFileName)
{
	HANDLE hFile = CreateFile(a_pstrFileName, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if( hFile == INVALID_HANDLE_VALUE ) return NULL;

	DWORD	dwRead = 0;
	DWORD	dwSize = GetFileSize(hFile, NULL);
	HGLOBAL hGlobal = GlobalAlloc(GMEM_MOVEABLE | GMEM_NODISCARD,dwSize);
	LPVOID	pData = GlobalLock(hGlobal);
	ReadFile(hFile, pData, dwSize, &dwRead, NULL);
	GlobalUnlock(hGlobal);
	CloseHandle(hFile);

	IStream* pStream = NULL;
    CreateStreamOnHGlobal(hGlobal, FALSE, &pStream);

	HBITMAP		hBmpImg;
	IPicture*	pPic = NULL;
	OleLoadPicture(pStream, dwSize, FALSE, IID_IPicture, (LPVOID *)&pPic);	
	GlobalFree(hGlobal);
	if ( pPic == NULL ) return NULL;
	pPic->get_Handle((unsigned int*)&hBmpImg);
	return hBmpImg;	
}
/// 
/// \brief <pre>
/// Load an image of the resource.
/// </pre>
/// \param   a_hWnd		- It should not be NULL.
/// \param   a_nResID	- Resource ID
/// \param   a_pstrType	- Resource type "GIF", "WAV",..
/// \return  HBITMAP	CImgHelper::LoadImageRes(HWND 
/// 
HBITMAP	CImgHelper::LoadImageRes(HWND a_hWnd, UINT a_nResID, TCHAR* a_pstrType)
{
	HINSTANCE hInstance;
#ifdef _WIN32_WCE
	hInstance = AfxGetResourceHandle();
#else
	hInstance = (HINSTANCE)GetWindowLong(a_hWnd, GWL_HINSTANCE);
#endif
    HRSRC hSrc = FindResource(hInstance, MAKEINTRESOURCE(a_nResID), a_pstrType);
    HGLOBAL hResData = LoadResource(hInstance, hSrc);
    int dwSize = SizeofResource(hInstance, hSrc);

	HGLOBAL hGlobal = GlobalAlloc(GMEM_MOVEABLE | GMEM_NODISCARD,dwSize);

	char *pDest = reinterpret_cast<char *> (GlobalLock(hGlobal));
    char *pSrc = reinterpret_cast<char *> (LockResource(hResData));

	CopyMemory(pDest,pSrc,dwSize);
	GlobalUnlock(hGlobal);

	IStream* pStream = NULL;
    CreateStreamOnHGlobal(hGlobal, FALSE, &pStream);

	HBITMAP		hBmpImg;
	CString		pFSize;
	IPicture*	pPic = NULL;
	OleLoadPicture(pStream, dwSize, FALSE, IID_IPicture, (LPVOID *)&pPic);
	UnlockResource(hResData);
	FreeResource(hResData);
	if ( pPic == NULL ) return NULL;
	pPic->get_Handle((unsigned int*)&hBmpImg);
	return hBmpImg;
}
/// 
/// \brief <pre>
/// Load the PNG file. Use ATLImage.
/// </pre>
/// \param   a_hWnd	- Window Handle. must not NULL
/// \param   a_pstrFileName	- Full Path File Name
/// \return  HBITMAP	- Load Bitmap Handle 
/// 
HBITMAP	CImgHelper::LoadPNGFile(HWND a_hWnd, TCHAR* a_pstrFileName)
{
	CImage	ImgFile;
	HRESULT hr = ImgFile.Load(a_pstrFileName);
	if( FAILED(hr) ) return NULL;

	HDC hDC = GetDC(a_hWnd);
	HDC	hMemDC = CreateCompatibleDC(hDC);
	HBITMAP hBmpImg = CreateCompatibleBitmap(hDC, ImgFile.GetWidth(), ImgFile.GetHeight());
	HBITMAP hOldBmp = (HBITMAP)SelectObject(hMemDC, hBmpImg);
	ImgFile.Draw(hMemDC, 0, 0, ImgFile.GetWidth(), ImgFile.GetHeight(), 0, 0, ImgFile.GetWidth(), ImgFile.GetHeight());
	SelectObject(hMemDC, hOldBmp);
	DeleteDC(hMemDC);
	if( !ImgFile.IsNull() ) ImgFile.Destroy();
	return hBmpImg;
}
/// 
/// \brief <pre>
/// Load the BMP image file. Not available in CE.
/// </pre>
/// \param   a_hWnd
/// \param   a_pstrFileName	- Image File Full Path
/// \param   cx				- Imgae Width
/// \param   cy				- Image Height
/// \return  HBITMAP	- Load Bitmap Handle
/// 
HBITMAP CImgHelper::LoadBMPFile(HWND a_hWnd, TCHAR* a_pstrFileName, int cx, int cy)
{
	HINSTANCE hInstance = (HINSTANCE)GetWindowLong( a_hWnd, GWL_HINSTANCE );
	HANDLE hBitmap = ::LoadImage( hInstance, a_pstrFileName, IMAGE_BITMAP, cx, cy, LR_LOADMAP3DCOLORS | LR_LOADFROMFILE );
	return hBitmap ? (HBITMAP)hBitmap : NULL;
}
/// 
/// \brief
/// 
/// \param   a_nCx
/// \param   a_nCy
/// \param   a_hSrcDC
/// \param   a_strImgFile
/// \return  BOOL 
/// 
BOOL CImgHelper::SaveDCToPNG(int a_nCx, int a_nCy, HDC a_hSrcDC, const TCHAR* a_strImgFile)
{
	CImage capImage;
	if( !capImage.Create(a_nCx, a_nCy, 32) ) return FALSE;
	HDC hDC = capImage.GetDC();
	BitBlt(hDC, 0, 0, a_nCx, a_nCy, a_hSrcDC, 0, 0, SRCCOPY);
	BOOL bRet = capImage.Save(a_strImgFile, Gdiplus::ImageFormatPNG) == E_FAIL ? FALSE : TRUE;
	capImage.ReleaseDC();
	return bRet;
}
#endif
#define ALLOC_UNIT	3072
/// 
/// \brief <pre>
/// Find the transparent region according to the mask color.
/// </pre>
/// \param   hBmp				- Srouce Bitmap
/// \param   a_clrTransparent	- Transparet Color
/// \param   nOutLineWidth		- RGB Transparent OutLine Width
/// \return  HRGN	- Transparent Region
/// 
HRGN  CImgHelper::Transparent(HBITMAP hBmp,COLORREF a_clrTransparent, int nOutLineWidth) 
{
	HRGN hRgn = NULL;
	if (hBmp) {
		HDC hMemDC = CreateCompatibleDC(NULL);
		if (hMemDC) {
			BITMAP bm;
			GetObject(hBmp, sizeof(bm), &bm);			
			BITMAPINFOHEADER RGB32BITSBITMAPINFO = {	
					sizeof(BITMAPINFOHEADER),	// biSize 
					bm.bmWidth,					// biWidth; 
					bm.bmHeight,				// biHeight; 
					1,							// biPlanes; 
					32,							// biBitCount 
					BI_RGB,						// biCompression; 
					0,							// biSizeImage; 
					0,							// biXPelsPerMeter; 
					0,							// biYPelsPerMeter; 
					0,							// biClrUsed; 
					0							// biClrImportant; 
			};
			VOID * pbits32; 
			HBITMAP hbm32 = CreateDIBSection(hMemDC, (BITMAPINFO *)&RGB32BITSBITMAPINFO, DIB_RGB_COLORS, &pbits32, NULL, 0);
			if (hbm32) {
				HBITMAP holdBmp = (HBITMAP)SelectObject(hMemDC, hbm32);			
				HDC hDC = CreateCompatibleDC(hMemDC);
				if (hDC) {					
					BITMAP bm32;
					GetObject(hbm32, sizeof(bm32), &bm32);
				
					while (bm32.bmWidthBytes % 4) bm32.bmWidthBytes++;					

					HBITMAP holdBmp2 = (HBITMAP)SelectObject(hDC, hBmp);
					BitBlt(hMemDC, 0, 0, bm.bmWidth, bm.bmHeight, hDC, 0, 0, SRCCOPY);
					
					DWORD maxRects = ALLOC_UNIT;
					HANDLE hData = GlobalAlloc(GMEM_MOVEABLE, sizeof(RGNDATAHEADER) + (sizeof(RECT) * maxRects));
					RGNDATA *pData = (RGNDATA *)GlobalLock(hData);
					pData->rdh.dwSize = sizeof(RGNDATAHEADER);
					pData->rdh.iType = RDH_RECTANGLES;
					pData->rdh.nCount = pData->rdh.nRgnSize = 0;
					SetRect(&pData->rdh.rcBound, MAXLONG, MAXLONG, 0, 0);
					BYTE lr = GetRValue(a_clrTransparent);
					BYTE lg = GetGValue(a_clrTransparent);
					BYTE lb = GetBValue(a_clrTransparent);
					BYTE *p32 = (BYTE *)bm32.bmBits + (bm32.bmHeight - 1) * bm32.bmWidthBytes;
					for (int y = 0; y < bm.bmHeight; y++) {
						for (int x = 0; x < bm.bmWidth; x++) {
							int x0 = x;
							LONG *p = (LONG *)p32 + x;
							while (x < bm.bmWidth) {
								if(	GetRValue(*p) == lr && GetGValue(*p) == lg && GetBValue(*p) == lb )
									break;
								++p; ++x;
							}
							if (x > x0) {
								if (pData->rdh.nCount >= maxRects) {
									GlobalUnlock(hData);
									maxRects += ALLOC_UNIT;
									//GlobalFree(hData); hData = NULL;
									hData = GlobalReAlloc(hData, sizeof(RGNDATAHEADER) + (sizeof(RECT) * maxRects), GMEM_MOVEABLE);
									pData = (RGNDATA *)GlobalLock(hData);
								}
								RECT *pr = (RECT *)&pData->Buffer;
                                int index = pData->rdh.nCount;
#ifdef _WIN32_WCE								
								int ecx = index;
								ecx <<= 4;
								int eax = x0;
								eax -= nOutLineWidth;
								memcpy((void*)(pData->Buffer+ecx), &eax, 4);

								eax = y;
								eax -= nOutLineWidth;
								memcpy((void*)(pData->Buffer+ecx+4), &eax, sizeof(eax));

								eax = x;
								eax += nOutLineWidth;
								memcpy((void*)(pData->Buffer+ecx+8), &eax, sizeof(eax));

								eax ^= eax;
								eax += nOutLineWidth;
								ecx = y;
								int edx = ecx+eax+1;
								eax = index;
								eax <<= 4;
								memcpy((void*)(pData->Buffer+eax+12), &edx, sizeof(edx));
#else
								__asm {
									mov ecx, index
									shl ecx, 4
									mov eax, x0
									sub eax, nOutLineWidth
									mov edx, pr
									mov [edx+ecx],eax
									mov eax, y
									sub eax, nOutLineWidth
									mov edx, pr
									mov [edx+ecx+4],eax
									mov eax, x
									add eax, nOutLineWidth
									mov edx, pr
									mov [edx+ecx+8],eax
									xor eax, eax
									add eax, nOutLineWidth
									mov ecx, y
									lea edx, [ecx+eax+1]
									mov eax, index
									shl eax, 4
									mov ecx, pr
									mov [ecx+eax+12], edx
							}
#endif
								if (x0 < pData->rdh.rcBound.left)	 pData->rdh.rcBound.left = x0;
								if (y < pData->rdh.rcBound.top)  	 pData->rdh.rcBound.top = y;
								if (x > pData->rdh.rcBound.right)	 pData->rdh.rcBound.right = x;
								if (y+1 > pData->rdh.rcBound.bottom) pData->rdh.rcBound.bottom = y+1;
								pData->rdh.nCount++;

								if (pData->rdh.nCount == 2000) {
									HRGN h = ExtCreateRegion(NULL, sizeof(RGNDATAHEADER) + (sizeof(RECT) * maxRects), pData);
									if (hRgn) {
										CombineRgn(hRgn, hRgn, h, RGN_OR);
										DeleteObject(h); h = NULL;
									} else hRgn = h;
									pData->rdh.nCount = 0;
									SetRect(&pData->rdh.rcBound, MAXLONG, MAXLONG, 0, 0);
								}
							}
						}
						p32 -= bm32.bmWidthBytes;
					}

					HRGN h = ExtCreateRegion(NULL, sizeof(RGNDATAHEADER) + (sizeof(RECT) * maxRects), pData);
					if (hRgn) {
						CombineRgn(hRgn, hRgn, h, RGN_OR);
						DeleteObject(h); h = NULL;
					} else hRgn = h;

					GlobalFree(hData); hData = NULL;
					SelectObject(hDC, holdBmp2);
					DeleteObject(holdBmp2); holdBmp2 = NULL;
					DeleteDC(hDC);
				}
				DeleteObject(SelectObject(hMemDC, holdBmp));
				DeleteDC(hMemDC);
				DeleteObject(hbm32);
				DeleteObject(holdBmp); holdBmp = NULL;
			} else OutputDebugString(_T("ImgHelper >> Fail to DBI Section"));
		} else OutputDebugString(_T("ImgHelper >> Fail to MemDC Transparent"));
	}
	return hRgn;
}

/// 
/// \brief <pre>
/// Transparent BitBlt
/// </pre>
/// \param   hdcDest	- Dest DC
/// \param   nXDest		- Dest x
/// \param   nYDest		- Dest y
/// \param   nWidth		- Dest Width
/// \param   nHeight	- Dest Height
/// \param   hBitmap	- Source Bitmap
/// \param   nXSrc		- Source x
/// \param   nYSrc		- Source y
/// \param   a_clrTransparent	- Transparent Color
/// \param   hPal		- Palatte may NULL
/// 
void CImgHelper::MyTransparentBlt(HDC hdcDest, int nXDest, int nYDest, int nWidth, int nHeight, HBITMAP hBitmap, int nXSrc, int nYSrc, COLORREF a_clrTransparent, HPALETTE hPal)
{
  CDC dc, memDC, maskDC, tempDC;
  dc.Attach( hdcDest );
  maskDC.CreateCompatibleDC(&dc);
  CBitmap maskBitmap;

  // These store return of SelectObject() calls
  CBitmap* pOldMemBmp = NULL;
  CBitmap* pOldMaskBmp = NULL;
  HBITMAP hOldTempBmp = NULL;

  memDC.CreateCompatibleDC (&dc);
  tempDC.CreateCompatibleDC (&dc);
  CBitmap bmpImage;
  bmpImage.CreateCompatibleBitmap (&dc, nWidth, nHeight);
  pOldMemBmp = memDC.SelectObject (&bmpImage);

  // Select and realize the palette
  if (dc.GetDeviceCaps (RASTERCAPS) & RC_PALETTE && hPal) {
     ::SelectPalette( dc, hPal, FALSE );
     dc.RealizePalette();
     ::SelectPalette( memDC, hPal, FALSE );
  }

  hOldTempBmp = (HBITMAP) ::SelectObject (tempDC.m_hDC, hBitmap);
  memDC.BitBlt (0, 0, nWidth, nHeight, &tempDC, nXSrc, nYSrc, SRCCOPY);

  // Create monochrome bitmap for the mask
  maskBitmap.CreateBitmap (nWidth, nHeight, 1, 1, NULL);
  pOldMaskBmp = maskDC.SelectObject (&maskBitmap);
  memDC.SetBkColor (a_clrTransparent);

  // Create the mask from the memory DC
  maskDC.BitBlt (0, 0, nWidth, nHeight, &memDC, 0, 0, SRCCOPY);

  // Set the background in memDC to black. Using SRCPAINT with black
  // and any other color results in the other color, thus making
  // black the transparent color
  memDC.SetBkColor (RGB (0,0,0));
  memDC.SetTextColor (RGB (255,255,255));
  memDC.BitBlt (0, 0, nWidth, nHeight, &maskDC, 0, 0, SRCAND);

  // Set the foreground to black. See comment above.
  dc.SetBkColor (RGB (255,255,255));
  dc.SetTextColor (RGB (0,0,0));
  dc.BitBlt (nXDest, nYDest, nWidth, nHeight, &maskDC, 0, 0, SRCAND);

  // Combine the foreground with the background
  dc.BitBlt (nXDest, nYDest, nWidth, nHeight, &memDC, 0, 0, SRCPAINT);

  if (hOldTempBmp)
     ::SelectObject (tempDC.m_hDC, hOldTempBmp);
  if (pOldMaskBmp)
     maskDC.SelectObject (pOldMaskBmp);
  if (pOldMemBmp)
     memDC.SelectObject (pOldMemBmp);
  dc.Detach();
}


	}	// end of namespace UI
}	// end of namespace MFC