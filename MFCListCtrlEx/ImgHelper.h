
#if !defined(__MFC_UI_LIB_IMG_HELPER__)
#define __MFC_UI_LIB_IMG_HELPER__

//#define SUPPORT_IMAGING

namespace MFC {
	namespace UI {

class CImgHelper  
{
public:
#ifdef _WIN32_WCE	
	static HBITMAP	LoadImageFile(HWND a_hWnd, TCHAR* a_pstrFileName);
	static HBITMAP	LoadBMPFile(TCHAR* a_pstrFileName);
#else
	static HBITMAP	LoadImageFile(TCHAR* a_pstrFileName);
	static HBITMAP	LoadBMPFile(HWND a_hWnd, TCHAR* a_pstrFileName, int cx, int cy);
	static HBITMAP	LoadImageRes(HWND a_hWnd, UINT a_nResID, TCHAR* a_pstrType);
	static HBITMAP	LoadPNGFile(HWND a_hWnd, TCHAR* a_pstrFileName);
	static BOOL		SaveDCToPNG(int a_nCx, int a_nCy, HDC a_hSrcDC, const TCHAR* a_strImgFile);
#endif	
	static HRGN		Transparent(HBITMAP hBmp, COLORREF a_clrTransparent, int nOutLineWidth);
	static void		MyTransparentBlt(HDC hdcDest, int nXDest, int nYDest, int nWidth, int nHeight, HBITMAP hBitmap, int nXSrc, int nYSrc, COLORREF a_clrTransparent, HPALETTE hPal);
};

	}	// end of namespace UI
}	// end of namespace MFC

#endif // __MFC_UI_LIB_IMG_HELPER__
