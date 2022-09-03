// FlatHeader.cpp: 구현 파일
//

#include "pch.h"
#include "FlatHeader.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace MFC {
	namespace UI {

/// 
/// \brief <pre>
/// Constructor
/// </pre>
/// 
FlatHeader::FlatHeader() : m_cr3DHighLight(::GetSysColor(COLOR_3DHIGHLIGHT))
	,m_cr3DShadow(::GetSysColor(COLOR_3DSHADOW))
	,m_cr3DFace(::GetSysColor(COLOR_3DFACE))
	,m_crText(::GetSysColor(COLOR_BTNTEXT))
	,m_clrSort(RGB(0,0,0))
	,m_iSpacing(2)
	,m_iHotIndex(-1)	
	,m_iSortColumn(-1)	
	,m_nSortCol(-1)
{	
	m_sizeImage.cx = 10;
	m_sizeImage.cy = 16;
	SetFont(_T("����"), 12);
}
/// 
/// \brief <pre>
/// Destructor
/// </pre>
/// 
FlatHeader::~FlatHeader()
{
	if( m_fntHeader.GetSafeHandle() ) m_fntHeader.DeleteObject();
}

BEGIN_MESSAGE_MAP(FlatHeader, CHeaderCtrl)
	//{{AFX_MSG_MAP(FlatHeader)
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/// 
/// \brief <pre>
/// Drawing the header control.
/// </pre>
/// \param   pDC
/// 
void FlatHeader::DrawCtrl(CDC* pDC)
{
	CRect rectClip;
	if (pDC->GetClipBox(&rectClip) == ERROR) return;
	CRect rectClient, rectItem;
	INT iItem;
	GetClientRect(&rectClient);

	pDC->FillSolidRect(rectClip, m_cr3DFace);

	INT iItems = GetItemCount();
	ASSERT(iItems >= 0);

	CPen penHighLight(PS_SOLID, 1, m_cr3DHighLight);
	CPen penShadow(PS_SOLID, 1, m_cr3DShadow);
	CPen* pPen = pDC->GetCurrentPen();

	CFont* pFont = pDC->SelectObject(&m_fntHeader);

	pDC->SetBkColor(m_cr3DFace);
	pDC->SetTextColor(m_crText);

	INT iWidth = 0;

	for(INT i=0; i<iItems; ++i) {
		iItem = OrderToIndex(i);
		TCHAR szText[HEADER_TXT_MAX] = {0,};
		HDITEM hditem;
		hditem.mask = HDI_WIDTH | HDI_FORMAT | HDI_TEXT | HDI_IMAGE | HDI_BITMAP;
		hditem.pszText = szText;
		hditem.cchTextMax = sizeof(szText);
		VERIFY(GetItem(iItem, &hditem));
		VERIFY(GetItemRect(iItem, rectItem));
		if (rectItem.right >= rectClip.left || rectItem.left <= rectClip.right) {
			rectItem.DeflateRect(m_iSpacing, 0);			
			DrawText( pDC, rectItem, &hditem);
			rectItem.InflateRect(m_iSpacing, 0);
			
			if(0 & MK_LBUTTON && m_iHotIndex == iItem && m_hdhtiHotItem.flags&HHT_ONHEADER)
				pDC->InvertRect(rectItem);

			if(i < iItems-1) {
				pDC->SelectObject(&penShadow);
				pDC->MoveTo(rectItem.right-1, rectItem.top+2);
				pDC->LineTo(rectItem.right-1, rectItem.bottom-2);

				pDC->SelectObject(&penHighLight);
				pDC->MoveTo(rectItem.right, rectItem.top+2);
				pDC->LineTo(rectItem.right, rectItem.bottom-2);
			}
		}
		
		// Sorts the image.
		if( m_nSortCol != -1 ) {
			if( m_nSortCol == iItem) {// Draw only those columns.
				COLORREF clrOld = pDC->SetTextColor(m_clrSort);
				if( m_bSortAsc ) {
					pDC->DrawText(_T("  ��"), -1, rectItem, DT_LEFT | DT_END_ELLIPSIS | DT_SINGLELINE | DT_VCENTER);
				} else {
					pDC->DrawText(_T("  ��"), -1, rectItem, DT_LEFT | DT_END_ELLIPSIS | DT_SINGLELINE | DT_VCENTER);
				}
				pDC->SetTextColor(clrOld);
			}
		}
		iWidth += hditem.cxy;
	}
	if(iWidth > 0) {
		rectClient.right = rectClient.left + iWidth;
		pDC->Draw3dRect(rectClient, m_cr3DHighLight, m_cr3DShadow);
	}
	pDC->SelectObject(pFont);
	pDC->SelectObject(pPen);
}
/// 
/// \brief <pre>
/// Draw an item. For bitmap images. Only draw bitmaps.
/// </pre>
/// \param   pDC
/// \param   rect
/// \param   lphdi
/// \return  INT 
/// 
INT FlatHeader::DrawText(CDC* pDC, CRect rect, LPHDITEM lphdi)
{
	CString str;
	INT iWidth = 0;
	CBitmap* pBitmap = NULL;
	BITMAP BitmapInfo;
	CString strMsg;
	pDC->DrawText(rect.Width() < 3 ? _T("") : lphdi->pszText, -1, rect, DT_CENTER | DT_SINGLELINE | DT_VCENTER);
	if(lphdi->fmt&HDF_BITMAP) {
		ASSERT(lphdi->mask&HDI_BITMAP);
		ASSERT(lphdi->hbm);
		pBitmap = CBitmap::FromHandle(lphdi->hbm);
		if(pBitmap)
			VERIFY(pBitmap->GetObject(sizeof(BITMAP), &BitmapInfo));

		//pDC->DrawText(lphdi->pszText, -1, rect, DT_LEFT | DT_END_ELLIPSIS | DT_SINGLELINE | DT_VCENTER);
		if(lstrcmp(lphdi->pszText, _T("")) == 0) {
			rect.right = rect.left + 10;
			DrawBitmap(pDC, rect, lphdi, pBitmap, &BitmapInfo, TRUE);
		} else {
			rect.left += 0;
			rect.right -= (iWidth=DrawBitmap(pDC, rect, lphdi, pBitmap, &BitmapInfo, TRUE)) ? iWidth+m_iSpacing:0;
		}
		return 0;
	}
	return 0;
}
/// 
/// \brief <pre>
/// Sets the font of the header control.
/// </pre>
/// \param   a_pstrFontName
/// \param   a_nSize
/// 
void FlatHeader::SetFont(TCHAR* a_pstrFontName, UINT a_nSize)
{
	if( m_fntHeader.GetSafeHandle() ) m_fntHeader.DeleteObject();
	LOGFONT lf;	
	memset (&lf, 0, sizeof (LOGFONT));	
	_tcscpy_s (lf.lfFaceName, a_pstrFontName);
	lf.lfWeight = FW_NORMAL;
	lf.lfHeight = a_nSize;
	lf.lfCharSet  =  HANGEUL_CHARSET;
	lf.lfPitchAndFamily = FF_SWISS  | DEFAULT_PITCH;
	m_fntHeader.CreateFontIndirect (&lf);
}
/// 
/// \brief <pre>
/// 
/// </pre>
/// \param   nItem
/// \param   nItemState
/// 
void FlatHeader::DrawItem(int nItem, int nItemState)
{
	if(nItem < 0) return;
	CDC* pDC = GetDC();
	CRect rect;
	GetItemRect(nItem, rect);
	CFont* pOldFont = (CFont*)pDC->SelectObject(m_fntHeader);

	HD_ITEM item;
	TCHAR	text[HEADER_TXT_MAX] = {0,};
	item.pszText = text;
	item.mask = HDI_TEXT | HDI_WIDTH;
	item.cchTextMax = HEADER_TXT_MAX;

	GetItem(nItem, &item);
	COLORREF cfForeColor = RGB(255, 255, 255);
	COLORREF crBackColor = RGB(0, 0, 0);

	rect.left += 5;
	rect.bottom += 5;
	pDC->DrawText(item.pszText, strlen((const char*)item.pszText), &rect, DT_SINGLELINE | DT_END_ELLIPSIS | DT_VCENTER | DT_LEFT);
	pDC->SelectObject(pOldFont);
	ReleaseDC(pDC);	
}
/// 
/// \brief <pre>
/// 
/// </pre>
/// \param   pDC
/// \return  BOOL 
/// 
BOOL FlatHeader::OnEraseBkgnd(CDC* pDC) 
{
	return TRUE;
}
/// 
/// \brief <pre>
/// Draw Header Control.
/// </pre>
/// 
void FlatHeader::OnPaint() 
{
	CPaintDC dc(this);	
	DrawCtrl(&dc);
}
/// 
/// \brief <pre>
/// Draw Image..
/// </pre>
/// \param   pDC
/// \param   rect
/// \param   lphdi
/// \param   bRight
/// \return  INT 
/// 
INT FlatHeader::DrawImage(CDC* pDC, CRect rect, LPHDITEM lphdi, BOOL bRight)
{
	CImageList* pImageList = GetImageList();
	INT iWidth = 0;
	if(lphdi->mask&HDI_IMAGE && lphdi->fmt&HDF_IMAGE) {
		ASSERT(pImageList);
		ASSERT(lphdi->iImage>=0 && lphdi->iImage<pImageList->GetImageCount());
		if(rect.Width()>0) {
			POINT point;
			point.y = rect.CenterPoint().y - (m_sizeImage.cy>>1);
			point.x = bRight ? rect.right - m_sizeImage.cx : point.x = rect.left;
			SIZE size;
			size.cx = rect.Width()<m_sizeImage.cx ? rect.Width():m_sizeImage.cx;
			size.cy = m_sizeImage.cy;
			iWidth = m_sizeImage.cx;
		}
	}
	return iWidth;
}
/// 
/// \brief <pre>
/// Draw Bitmap
/// </pre>
/// \param   pDC
/// \param   rect
/// \param   lphdi
/// \param   pBitmap
/// \param   pBitmapInfo
/// \param   bRight
/// \return  INT 
/// 
INT FlatHeader::DrawBitmap(CDC* pDC, CRect rect, LPHDITEM lphdi, CBitmap* pBitmap, BITMAP* pBitmapInfo, BOOL bRight)
{
	INT iWidth = 0;
#ifndef UNDER_CE
	if(pBitmap) {
		iWidth = pBitmapInfo->bmWidth;
		if(iWidth<=rect.Width() && rect.Width() > 0) {
			POINT point;
			point.y = rect.CenterPoint().y - (pBitmapInfo->bmHeight>>1);
			point.x = bRight ? rect.right - iWidth : point.x = rect.left;
			CDC dc;
			if(dc.CreateCompatibleDC(pDC) == TRUE) {
				VERIFY(dc.SelectObject(pBitmap));
				iWidth = pDC->TransparentBlt(point.x, point.y, pBitmapInfo->bmWidth, pBitmapInfo->bmHeight, 
											&dc, 0, 0, pBitmapInfo->bmWidth, pBitmapInfo->bmHeight, RGB(255, 255, 255)) ? iWidth:0;
			} else iWidth = 0;
		} else iWidth = 0;
	}
#endif
	return iWidth;
}
/// 
/// \brief <pre>
/// ������ ��������.
/// </pre>
/// \param   nCol
/// \param   
/// 
void FlatHeader::SetSortImage( int nCol, BOOL bAsc )
{
	m_nSortCol = nCol;
	m_bSortAsc = bAsc;
	Invalidate();
}
/// 
/// \brief
/// Returns the sorting type of the column.
/// \param   a_nCol
/// \return  int - -1:not set, 1:ASC, 0:DESC
/// 
int FlatHeader::GetSortImage(int a_nCol)
{
	return m_nSortCol == a_nCol ? m_bSortAsc : -1;
}


	} // end of namespace UI
} // end of namespace MFC
