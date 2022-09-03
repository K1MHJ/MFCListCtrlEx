
#include "pch.h"
#include "ListCtrlEx.h"
#include <ATLComTime.h>
#include "ImgHelper.h"

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
		CListCtrlEx::CListCtrlEx()
			: m_bDrag(FALSE)
			, m_bUseKeyboard(FALSE)
			, m_bGrid(FALSE)
			, m_bSort(FALSE)
			, m_bShowHeader(TRUE)
			, m_bDrawFocusRect(TRUE)
			, m_bBorder(FALSE)
			, m_bOwnerData(FALSE)
			, m_nItemHeight(DEFAULT_LIST_HEIGHT)
			, m_cxBitmap(0)
			, m_cyBitmap(0)
			, m_nSelectItem(-1)
			, m_nSortedCol(-1)
			, m_pDragImage(NULL)
			, m_nDragIndex(-1)
			, m_nDropIndex(-1)
			, m_nTopGap(0)
			, m_nUnderLineStyle(PS_SOLID)
			, m_bUseDragDrop(TRUE)
			, m_bDragging(FALSE)
			, m_bUnderLine(FALSE)
			, m_bUseMulti(FALSE)
			, m_hDropItem(NULL)
			, m_pDropWnd(NULL)
			, m_BkImgAlign(BK_RIGHT_BOTTOM)
			, m_pHandler(NULL)
			, m_DefaultHandler(NULL)
		{
			m_clrUnderLine = RGB(255, 255, 255);
			m_clrHilightBG = ::GetSysColor(COLOR_HIGHLIGHT);//(208,218,243);
			m_clrDefaultBG = RGB(255, 255, 255);//::GetSysColor(CTLCOLOR_LISTBOX);
			m_clrHilightTxt = ::GetSysColor(COLOR_HIGHLIGHTTEXT);
			m_clrDefaultTxt = ::GetSysColor(COLOR_WINDOWTEXT);
			m_clrLoseFocusBG = GetSysColor(CTLCOLOR_STATIC);
			m_clrLoseFocusTxt = GetSysColor(COLOR_GRADIENTINACTIVECAPTION);
			m_clrDefaultTxtColor = RGB(0, 0, 0);
			m_strDefaultMsg = _T("No items found.");
		}
		/// 
		/// \brief <pre>
		/// Destructor
		/// </pre>
		/// 
		CListCtrlEx::~CListCtrlEx()
		{
			if (m_pDragImage != NULL) {
				m_pDragImage->DeleteImageList();
				delete m_pDragImage;
				m_pDragImage = NULL;
			}
			if (m_bmpBkImg.GetSafeHandle()) m_bmpBkImg.DeleteObject();
			if (m_bmpDefault.GetSafeHandle()) m_bmpDefault.DeleteObject();
			if (m_bmpFocus.GetSafeHandle()) m_bmpFocus.DeleteObject();
			if (m_fntList.GetSafeHandle()) m_fntList.DeleteObject();
			if (m_fntHeader.GetSafeHandle()) m_fntHeader.DeleteObject();
		}
		/// 
		/// \brief <pre>
		/// The message handler, WM_MEASUREITEM defines the outer part of the message to receive the message.
		/// </pre>
		/// 
		BEGIN_MESSAGE_MAP(CListCtrlEx, CListCtrl)
			//{{AFX_MSG_MAP(CListCtrlEx)
			ON_WM_CREATE()
			ON_WM_ERASEBKGND()
			ON_WM_QUERYNEWPALETTE()
			ON_WM_PALETTECHANGED()
			ON_NOTIFY_REFLECT(LVN_COLUMNCLICK, OnColumnclick)
			ON_NOTIFY(HDN_ITEMCLICKA, 0, OnHeaderClicked)
			ON_NOTIFY(HDN_ITEMCLICKW, 0, OnHeaderClicked)
			ON_WM_LBUTTONDOWN()
			ON_WM_LBUTTONUP()
			ON_NOTIFY_REFLECT(NM_CLICK, OnClick)
			ON_NOTIFY_REFLECT(NM_RCLICK, OnRclick)
			ON_NOTIFY_REFLECT(LVN_BEGINDRAG, OnBegindrag)
			ON_WM_MOUSEMOVE()
			ON_NOTIFY_REFLECT(NM_DBLCLK, OnDblclk)
			ON_NOTIFY_REFLECT(LVN_ITEMCHANGED, OnItemchanged)
			//}}AFX_MSG_MAP
			ON_WM_MEASUREITEM_REFLECT()
			ON_WM_PAINT()
			ON_NOTIFY_REFLECT(LVN_DELETEITEM, &CListCtrlEx::OnLvnDeleteitem)
			ON_NOTIFY_REFLECT(LVN_INSERTITEM, &CListCtrlEx::OnLvnInsertitem)
			ON_NOTIFY_REFLECT(LVN_DELETEALLITEMS, &CListCtrlEx::OnLvnDeleteallitems)
			ON_NOTIFY_REFLECT(LVN_GETDISPINFO, &CListCtrlEx::OnLvnGetdispinfo)
			ON_WM_SIZE()
		END_MESSAGE_MAP()

		/// 
		/// \brief <pre>
		/// List Ctrl, Set OwnerDraw Style
		/// </pre>
		/// \param   cs
		/// \return  BOOL 
		/// 
		BOOL CListCtrlEx::PreCreateWindow(CREATESTRUCT& cs)
		{
			cs.style = 0x0000;
			cs.style = WS_CHILD | WS_VISIBLE | LVS_REPORT | LVS_OWNERDRAWFIXED | LVS_SINGLESEL | LVS_SHOWSELALWAYS;
			if (m_bBorder) cs.style |= WS_BORDER;
			if (!m_bShowHeader) cs.style |= LVS_NOCOLUMNHEADER;
			if (m_bOwnerData) cs.style |= LVS_OWNERDATA;
			return CListCtrl::PreCreateWindow(cs);
		}
		/// 
		/// \brief <pre>
		/// Create Instance.
		/// </pre>
		/// \param   lpCreateStruct
		/// \return  int 
		/// 
		int CListCtrlEx::OnCreate(LPCREATESTRUCT lpCreateStruct)
		{
			if (CListCtrl::OnCreate(lpCreateStruct) == -1)
				return -1;

			if (m_bShowHeader) {
				HWND hwnd = GetHeaderCtrl()->m_hWnd;
				m_HeaderCtrl.SubclassWindow(hwnd);
			}
			SetExtendedStyle(LVS_EX_FULLROWSELECT);
			m_DefaultHandler.SetListCtrl(this);
			return 0;
		}
		/// 
		/// \brief
		/// Create Control. 
		/// \param   pWnd	- Parent Window
		/// \param   x		- x
		/// \param   y		- y
		/// \param   cx		- cx
		/// \param   cy		- cy
		/// \param   bShowHeader	- Show Header Control ? 
		/// \param   a_bOwnerData - LVS_OWNERDATA적용여부
		/// \param   nID	- Control ID
		/// \return  BOOL	- if Create Success return true else false
		/// 
		/// 
		BOOL CListCtrlEx::Create(CWnd* pWnd, int x, int y, int cx, int cy, BOOL bShowHeader, BOOL a_bBorder, BOOL a_bOwnerData, UINT nID)
		{
			m_bShowHeader = bShowHeader;
			m_bBorder = a_bBorder;
			m_bOwnerData = a_bOwnerData;
			return CListCtrl::Create(WS_CHILD | WS_VISIBLE, CRect(x, y, x + cx, y + cy), pWnd, nID);
		}
		/// 
		/// \brief <pre>
		/// Add a header column. LVCFMT_CENTER
		/// </pre>
		/// \param   nWidth		- Column Width
		/// \param   strTitle	- Column Title
		/// \param   nAlign		- Column Alignment
		/// 
		void CListCtrlEx::AddColumnText(int nWidth, LPTSTR strTitle, UINT nAlign)
		{
			LVCOLUMN	listColumn;
			int			nIdx = GetHeaderCtrl()->GetItemCount();
			listColumn.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
			listColumn.fmt = nAlign;
			listColumn.iSubItem = nIdx;
			listColumn.pszText = strTitle;
			listColumn.cx = nWidth;
			InsertColumn(nIdx, &listColumn);

			LV_COLUMN lvc;
			lvc.mask = LVCF_FMT;
			lvc.fmt = nAlign;
			SetColumn(nIdx, &lvc);
		}
		/// 
		/// \brief
		/// Modify the text in the header column.
		/// \param   a_nIdx
		/// \param   strTitle
		/// 
		void CListCtrlEx::UpdateColumnText(int a_nIdx, LPTSTR strTitle)
		{
			LV_COLUMN lvc;
			lvc.mask = LVCF_TEXT;
			lvc.pszText = strTitle;
			SetColumn(a_nIdx, &lvc);
		}
		/// 
		/// \brief <pre>
		/// Real Draw List Control.
		/// 1. Obtain each area.
		/// 2. paint the background.
		/// 3. Draw a status icon.
		/// 4. Print the first column.
		/// 5. Print the remaining columns.
		/// </pre>
		/// \param   lpDrawItemStruct
		/// 
		void CListCtrlEx::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
		{
			CDC* pDC = CDC::FromHandle(lpDrawItemStruct->hDC);
			CFont* pOldFont = pDC->SelectObject(&m_fntList);
			CRect	rcItem(lpDrawItemStruct->rcItem);
			int		nItem = lpDrawItemStruct->itemID;
			int		nSavedDC = pDC->SaveDC();

			LV_ITEM lvi;
			lvi.mask = LVIF_IMAGE | LVIF_STATE;
			lvi.iItem = nItem;
			lvi.iSubItem = 0;
			lvi.stateMask = 0xFFFF;		// get all state flags
			GetItem(&lvi);

			BOOL	bHighlight = ((lvi.state & LVIS_DROPHILITED) || ((lvi.state & LVIS_SELECTED) && ((GetFocus() == this) || (GetStyle() & LVS_SHOWSELALWAYS))));
			CRect	rcWnd;
			GetClientRect(&rcWnd);

			CRect rcBounds, rcLabel, rcIcon, rcHighlight;
			GetItemRect(nItem, rcBounds, LVIR_BOUNDS);	// All Item Area
			GetItemRect(nItem, rcLabel, LVIR_LABEL);	// Lable Area
			GetItemRect(nItem, rcIcon, LVIR_ICON);		// Icon Area
			rcHighlight = rcBounds;						// Hilight Area
			rcHighlight.left = 0;
			rcHighlight.right = rcWnd.right;
			bool bFocus = (lvi.state & LVIS_FOCUSED && (GetFocus() == this)) ? true : false;
			bool bDrawDefault = false;

			if (m_pHandler) m_pHandler->DrawItemBG(pDC, nItem, bHighlight, bFocus, rcBounds, bDrawDefault);
			if (!m_pHandler || bDrawDefault) m_DefaultHandler.DrawItemBG(pDC, nItem, bHighlight, bFocus, rcBounds, bDrawDefault);
			// ------------------------------------------------------------------------
			CRect	rcCol(rcBounds);
			// ------------------------------------------------------------------------
			bDrawDefault = false;
			if (m_pHandler) m_pHandler->DrawStateIconImage(pDC, nItem, bHighlight, bFocus, rcCol, bDrawDefault);
			if (!m_pHandler || bDrawDefault) m_DefaultHandler.DrawStateIconImage(pDC, nItem, bHighlight, bFocus, rcCol, bDrawDefault);
			bDrawDefault = false;
			if (m_pHandler) m_pHandler->DrawSmallIconImage(pDC, nItem, bHighlight, bFocus, rcIcon, bDrawDefault);
			if (!m_pHandler || bDrawDefault) m_DefaultHandler.DrawSmallIconImage(pDC, nItem, bHighlight, bFocus, rcIcon, bDrawDefault);
			bDrawDefault = false;
			if (m_pHandler) m_pHandler->DrawSubItem(pDC, nItem, 0, bHighlight, bFocus, rcLabel, bDrawDefault);
			if (!m_pHandler || bDrawDefault) m_DefaultHandler.DrawSubItem(pDC, nItem, 0, bHighlight, bFocus, rcLabel, bDrawDefault);
			rcBounds.right = rcHighlight.right > rcBounds.right ? rcHighlight.right : rcBounds.right;
			// ------------------------------------------------------------------------
			int nWidth = 0;
			rcCol.right = rcCol.left + GetColumnWidth(0);
			LV_COLUMN	lvc;
			lvc.mask = LVCF_FMT | LVCF_WIDTH;
			for (int nColumn = 1; GetColumn(nColumn, &lvc); nColumn++) {
				nWidth += GetColumnWidth(nColumn);
				rcCol.left = rcCol.right;
				rcCol.right += lvc.cx;
				bDrawDefault = false;
				if (m_pHandler) m_pHandler->DrawSubItem(pDC, nItem, nColumn, bHighlight, bFocus, rcCol, bDrawDefault);
				if (!m_pHandler || bDrawDefault) m_DefaultHandler.DrawSubItem(pDC, nItem, nColumn, bHighlight, bFocus, rcCol, bDrawDefault);
			}
			nWidth += GetColumnWidth(0);
			rcHighlight.right = nWidth;
			// ------------------------------------------------------------------------
			DrawOption(pDC, bFocus, rcBounds, rcHighlight);
			// ------------------------------------------------------------------------
			pDC->SelectObject(pOldFont);
			pDC->RestoreDC(nSavedDC);
		}
		/// 
		/// \brief <pre>
		/// Draw the remaining options. Grid, Underline, FocusRect
		/// </pre>
		/// \param   a_pDC
		/// \param   a_rcBounds
		/// \param   a_rcHighlight
		/// 
		void CListCtrlEx::DrawOption(CDC* a_pDC, BOOL a_bFocus, const CRect& a_rcBounds, const CRect& a_rcHighlight)
		{
			if (m_bGrid) {
				CPen penLine(PS_SOLID, 0, m_clrGridLine);
				CPen* pOldPen2 = a_pDC->SelectObject(&penLine);
				a_pDC->MoveTo(a_rcHighlight.left, a_rcHighlight.bottom - 1);
				a_pDC->LineTo(a_rcHighlight.right, a_rcHighlight.bottom - 1);
				LV_COLUMN lvc;
				lvc.mask = LVCF_FMT | LVCF_WIDTH;
				CRect rcGrid(a_rcBounds);
				rcGrid.right = rcGrid.left;
				for (int i = 0; GetColumn(i, &lvc); i++) {
					rcGrid.left = i == 0 ? rcGrid.left : rcGrid.right;
					rcGrid.right += lvc.cx;
					a_pDC->MoveTo(rcGrid.right - 1, rcGrid.top);
					a_pDC->LineTo(rcGrid.right - 1, rcGrid.bottom);
				}
				a_pDC->SelectObject(pOldPen2);
			}
			else if (m_bUnderLine) {
				CPen penBtnShadow(m_nUnderLineStyle, 0, m_clrUnderLine);
				CPen* pOldPen2 = a_pDC->SelectObject(&penBtnShadow);
				a_pDC->MoveTo(a_rcHighlight.left, a_rcHighlight.bottom - 1);
				a_pDC->LineTo(a_rcHighlight.right, a_rcHighlight.bottom - 1);
				a_pDC->SelectObject(pOldPen2);
			}
			if (a_bFocus && m_bDrawFocusRect)
				a_pDC->DrawFocusRect(a_rcHighlight);
		}
		/// 
		/// \brief <pre>
		/// Add an item to List Ctrl
		/// </pre>
		/// \param   pszText - Insert 0 Index Item Text
		/// \return  int - Add Index Number
		/// 
		int CListCtrlEx::AddItem(TCHAR* pszText, int a_nPos)
		{
			LV_ITEM		lvitem;
			lvitem.iItem = a_nPos == -1 ? GetItemCount() : a_nPos;
			lvitem.mask = LVIF_TEXT | LVIF_STATE;
			lvitem.iSubItem = 0;
			lvitem.stateMask = LVIS_STATEIMAGEMASK;
			lvitem.state = INDEXTOSTATEIMAGEMASK(1);
			lvitem.pszText = pszText;
			InsertItem(&lvitem);
			return lvitem.iItem;
		}
		/// 
		/// \brief <pre>
		/// Changes the nPos'th Text at the Index position of the list control item.
		/// </pre>
		/// \param   nIdx	- ListControl Row Index
		/// \param   nPos	- Column Position
		/// \param   pszText- Text
		/// 
		void CListCtrlEx::SetItemText(int nIdx, int nPos, const TCHAR* pszText)
		{
			CListCtrl::SetItemText(nIdx, nPos, pszText);
		}
		/// 
		/// \brief <pre>
		/// Sets the height of the list control. It can be set 1 time when created and can not be changed dynamically.
		/// </pre>
		/// \param   lpMeasureItemStruct
		/// 
		void CListCtrlEx::MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct)
		{
			lpMeasureItemStruct->itemHeight = m_nItemHeight;
		}
		/// 
		/// \brief <pre>
		/// Sets the background image of the list control.
		/// </pre>
		/// \param   a_pstrFileName
		/// \param   a_nFileType
		/// \param   a_nBkAlign
		/// \return  BOOL 
		/// 
		BOOL CListCtrlEx::SetBkImage(TCHAR* a_pstrFileName, IMGFILETYPE a_nFileType, BKIMGALIGN a_nBkAlign)
		{
			if (m_bmpBkImg.GetSafeHandle())
				m_bmpBkImg.DeleteObject();

			m_BkImgAlign = a_nBkAlign;
			switch (a_nFileType)
			{
			case PNG:
				break;
			case OTHER:
			{
#ifndef UNDER_CE
				HBITMAP hBkImg = CImgHelper::LoadImageFile(a_pstrFileName);
#else
				HBITMAP hBkImg = CImgHelper::LoadBMPFile(a_pstrFileName);
#endif
				if (!hBkImg) break;
				m_bmpBkImg.Attach(hBkImg);
				BITMAP hBMP;
				m_bmpBkImg.GetBitmap(&hBMP);
				m_cxBitmap = hBMP.bmWidth;
				m_cyBitmap = hBMP.bmHeight;
			}
			break;
			}
			return TRUE;
		}
		/// 
		/// \brief <pre>
		/// if exist background image, Draw Background Image
		/// </pre>
		/// \param   pDC
		/// \return  BOOL 
		/// 
		BOOL CListCtrlEx::OnEraseBkgnd(CDC* pDC)
		{
			if (m_bmpBkImg.GetSafeHandle()) {
				DrawBkImg(pDC);
				return TRUE;
			}
			return CListCtrl::OnEraseBkgnd(pDC);
		}
		/// 
		/// \brief <pre>
		/// Draw Background image
		/// </pre>
		/// \param   a_pDC
		/// 
		void CListCtrlEx::DrawBkImg(CDC* a_pDC)
		{
			CDC	hMemDC;
			hMemDC.CreateCompatibleDC(a_pDC);
			CBitmap* pOldBmp = (CBitmap*)hMemDC.SelectObject(m_bmpBkImg);
			switch (m_BkImgAlign)
			{
			case BK_TILT:
			case BK_CENTER:
			case BK_LEFT_BOTTOM:
			case BK_LEFT_TOP:
			case BK_RIGHT_TOP:
			case BK_RIGHT_BOTTOM:
			{
				CRect rcWnd;
				GetWindowRect(&rcWnd);
				a_pDC->BitBlt(rcWnd.Width() - m_cxBitmap, rcWnd.Height() - m_cyBitmap,
					m_cxBitmap, m_cyBitmap, &hMemDC, 0, 0, SRCCOPY);
			}
			}
		}
		/// 
		/// \brief <pre>
		/// When the column is resized, it will Invalidate () the area only.
		/// </pre>
		/// \param   wParam
		/// \param   lParam
		/// \param   pResult
		/// \return  BOOL 
		/// 
		BOOL CListCtrlEx::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult)
		{
			HD_NOTIFY* pHDN = (HD_NOTIFY*)lParam;
			if (pHDN->hdr.code == HDN_ITEMCHANGINGW || pHDN->hdr.code == HDN_ITEMCHANGINGA) {
				if (m_bmpBkImg.GetSafeHandle()) {
					CRect rcClient;
					GetClientRect(&rcClient);
					DWORD dwPos = GetMessagePos();
					CPoint pt(LOWORD(dwPos), HIWORD(dwPos));
					ScreenToClient(&pt);
					rcClient.left = pt.x;
					InvalidateRect(&rcClient);
				}
			}
			return CListCtrl::OnNotify(wParam, lParam, pResult);
		}
		/// 
		/// \brief <pre>
		/// Input Focus를 받을때 윈도우에게 보내는 메시지.
		/// 이 메시지는 Top Level Window로 보내지므로 부모 윈도에서 이 메시지를 처리해주어야한다.
		/// </pre>
		/// \return  BOOL 
		/// 
		BOOL CListCtrlEx::OnQueryNewPalette()
		{
			CClientDC dc(this);

			if (dc.GetDeviceCaps(RASTERCAPS) & RC_PALETTE && m_pal.m_hObject != NULL) {
				dc.SelectPalette(&m_pal, FALSE);
				BOOL result = dc.RealizePalette();
				if (result) Invalidate();
				return result;
			}
			return CListCtrl::OnQueryNewPalette();
		}
		/// 
		/// \brief <pre>
		/// List Ctrl 자신이 변경할수 있게 해준다.
		/// </pre>
		/// \param   pFocusWnd
		/// 
		void CListCtrlEx::OnPaletteChanged(CWnd* pFocusWnd)
		{
			CListCtrl::OnPaletteChanged(pFocusWnd);
			if (pFocusWnd == this) return;
			OnQueryNewPalette();
		}
		/// 
		/// \brief <pre>
		/// The event that occurs when you click a column
		/// </pre>
		/// \param   pNMHDR
		/// \param   pResult
		/// 
		void CListCtrlEx::OnColumnclick(NMHDR* pNMHDR, LRESULT* pResult)
		{
			NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
			*pResult = 0;
		}
		/// 
		/// \brief <pre>
		/// 마우스 왼쪽 버튼에 대한 처리
		/// </pre>
		/// \param   nFlags
		/// \param   point
		/// 
		void CListCtrlEx::OnLButtonDown(UINT nFlags, CPoint point)
		{
			LVHITTESTINFO hitInfo;
			hitInfo.pt = point;
			SubItemHitTest(&hitInfo);
			if (m_pHandler) m_pHandler->OnLButtonDown(point, hitInfo.iItem, hitInfo.iSubItem);
			CListCtrl::OnLButtonDown(nFlags, point);
		}
		/// 
		/// \brief <pre>
		/// Handling when left mouse button is left
		/// </pre>
		/// \param   nFlags
		/// \param   point
		/// 
		void CListCtrlEx::OnLButtonUp(UINT nFlags, CPoint point)
		{
			if (m_bDragging) {
				ReleaseCapture();
				m_bDragging = FALSE;
			}
			LVHITTESTINFO hitInfo;
			hitInfo.pt = point;
			SubItemHitTest(&hitInfo);
			if (m_pHandler) m_pHandler->OnLButtonUp(point, hitInfo.iItem, hitInfo.iSubItem);
			CListCtrl::OnLButtonUp(nFlags, point);
		}
		/// 
		/// \brief <pre>
		/// When an item is clicked ... Event
		/// </pre>
		/// \param   pNMHDR
		/// \param   pResult
		/// 
		void CListCtrlEx::OnClick(NMHDR* pNMHDR, LRESULT* pResult)
		{
			NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
			m_nSelectItem = pNMListView->iItem;
			if (m_pHandler) m_pHandler->OnItemClick(pNMListView->iItem == -1 ? -1 : pNMListView->iItem,
				pNMListView->iItem == -1 ? m_nSelectItem : pNMListView->iSubItem);
			*pResult = 0;
		}
		/// 
		/// \brief <pre>
		/// 내용을 넣으세요.
		/// </pre>
		/// \param   pNMHDR
		/// \param   pResult
		/// 
		void CListCtrlEx::OnItemchanged(NMHDR* pNMHDR, LRESULT* pResult)
		{
			NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
			int nSelectItem = GetSelectedListIdx();
			if (m_nSelectItem != nSelectItem) {
				m_nSelectItem = nSelectItem;
				if (m_pHandler) m_pHandler->OnItemChange(m_nSelectItem, pNMListView->iSubItem);
			}
			*pResult = 0;
		}
		/// 
		/// \brief <pre>
		/// delete
		/// </pre>
		/// \param   *pNMHDR
		/// \param   *pResult
		/// 
		void CListCtrlEx::OnLvnDeleteitem(NMHDR* pNMHDR, LRESULT* pResult)
		{
			LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
			if (m_nSelectItem != -1) {
				POSITION pos = GetFirstSelectedItemPosition();
				if (pos == NULL)
					m_nSelectItem = -1;
				else {
					int nSelectItem = GetNextSelectedItem(pos);
					m_nSelectItem = nSelectItem;
				}
			}
			*pResult = 0;
		}
		/// 
		/// \brief <pre>
		/// Add
		/// </pre>
		/// \param   *pNMHDR
		/// \param   *pResult
		/// 
		void CListCtrlEx::OnLvnInsertitem(NMHDR* pNMHDR, LRESULT* pResult)
		{
			LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
			if (m_nSelectItem != -1) {
				POSITION pos = GetFirstSelectedItemPosition();
				if (pos == NULL)
					m_nSelectItem = -1;
				else {
					int nSelectItem = GetNextSelectedItem(pos);
					m_nSelectItem = nSelectItem;
				}
			}
			*pResult = 0;
		}
		/// 
		/// \brief <pre>
		/// Handling Double click
		/// </pre>
		/// \param   pNMHDR
		/// \param   pResult
		/// 
		void CListCtrlEx::OnDblclk(NMHDR* pNMHDR, LRESULT* pResult)
		{
			int nSelectItem = GetSelectedListIdx();
			if (nSelectItem == -1) return;
			if (nSelectItem != m_nSelectItem)
				m_nSelectItem = nSelectItem;
			NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
			if (m_pHandler) m_pHandler->OnItemDBClick(m_nSelectItem, pNMListView->iSubItem);
			*pResult = 0;
		}
		/// 
		/// \brief <pre>
		/// Delete all item event.
		/// </pre>
		/// \param   *pNMHDR
		/// \param   *pResult
		/// 
		void CListCtrlEx::OnLvnDeleteallitems(NMHDR* pNMHDR, LRESULT* pResult)
		{
			LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
			m_nSelectItem = -1;
			*pResult = 0;
		}
		/// 
		/// \brief <pre>
		/// Handling of mouse right button in an item
		/// </pre>
		/// \param   pNMHDR
		/// \param   pResult
		/// 
		void CListCtrlEx::OnRclick(NMHDR* pNMHDR, LRESULT* pResult)
		{
			NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
			int nSelectItem = GetSelectedListIdx();
			//if( nSelectItem == -1 ) return;
			if (nSelectItem != m_nSelectItem)
				m_nSelectItem = nSelectItem;
			if (m_pHandler) m_pHandler->OnRbuttonClick(m_nSelectItem, pNMListView->iSubItem);
			*pResult = 0;
		}
		/// 
		/// \brief <pre>
		/// Drag Start Event
		/// </pre>
		/// \param   pNMHDR
		/// \param   pResult
		/// 
		void CListCtrlEx::OnBegindrag(NMHDR* pNMHDR, LRESULT* pResult)
		{
			if (!m_bUseDragDrop) return;
			NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
			m_nDragIndex = ((NM_LISTVIEW*)pNMHDR)->iItem;
			m_bDragging = TRUE;
			SetCapture();
			/*CPoint	pt(8, 8);
			CRect	rcItem;
			m_pDragImage = CreateDragImageEx(m_nDragIndex);
			GetItemRect(m_nDragIndex, &rcItem, LVIR_BOUNDS);

			if( GetImageList(LVSIL_SMALL) == NULL ) {
				pt.x = ((NM_LISTVIEW *)pNMHDR)->ptAction.x + GetScrollPos(SB_HORZ) - 6;
				pt.y = ((NM_LISTVIEW *)pNMHDR)->ptAction.y - rcItem.top + ( (m_nDragIndex + 1)*(rcItem.Height()/2));
			} else {
				pt.x = ((NM_LISTVIEW *)pNMHDR)->ptAction.x + GetScrollPos(SB_HORZ);
				pt.y = ((NM_LISTVIEW *)pNMHDR)->ptAction.y - rcItem.top;
			}

			if(m_pDragImage) {
				m_pDragImage->BeginDrag( 0, pt);
				m_pDragImage->DragEnter( GetDesktopWindow(), ((NM_LISTVIEW *)pNMHDR)->ptAction);
				m_bDragging = TRUE;
				m_hDropItem = NULL;
				m_nDropIndex = -1;
				SetCapture ();
			}*/
			* pResult = 0;
		}
		/// 
		/// \brief <pre>
		/// Handles drop items.
		/// </pre>
		/// \return  BOOL 
		/// 
		BOOL CListCtrlEx::DropItems()
		{
			if (m_nDropIndex == -1) return FALSE;

			TCHAR		szLabel[256] = { 0, };
			LV_ITEM		lvi;

			memset(&lvi, 0, sizeof(LV_ITEM));
			SetItemState(m_nDropIndex, 0, LVIS_DROPHILITED);
			lvi.iItem = m_nDragIndex;
			lvi.mask = LVIF_IMAGE | LVIF_TEXT;
			lvi.pszText = szLabel;
			lvi.cchTextMax = 255;

			GetItem(&lvi);
			lvi.iItem = m_nDropIndex == -1 ? GetItemCount() : m_nDropIndex;
			InsertItem(&lvi);

			SetItemState(lvi.iItem, LVIS_SELECTED, LVIS_SELECTED);
			if (m_nDragIndex < m_nDropIndex) DeleteItem(m_nDragIndex);
			else							  DeleteItem(m_nDragIndex + 1);
			return TRUE;
		}
		/// 
		/// \brief <pre>
		/// Handle dragging when moving the mouse
		/// </pre>
		/// \param   nFlags
		/// \param   point
		/// 
		void CListCtrlEx::OnMouseMove(UINT nFlags, CPoint point)
		{
			UINT		flags;
			if (m_bDragging) {
				CPoint	pt(point);
				int		nIndex;
				ClientToScreen(&pt);
				m_pDragImage->DragMove(pt);
				m_pDropWnd = WindowFromPoint(pt);
				m_pDropWnd->ScreenToClient(&pt);
				if (m_pDropWnd != this && m_pDropWnd->IsKindOf(RUNTIME_CLASS(CTreeCtrl))) {
					UINT uFlags;
					m_hDropItem = ((CTreeCtrl*)m_pDropWnd)->HitTest(pt, &uFlags);
				}
				else if ((nIndex = HitTest(point, &flags)) != -1) {
					m_pDragImage->DragShowNolock(FALSE);
					m_nDropIndex = nIndex;
					m_pDragImage->DragShowNolock(TRUE);
				}
			}
			CListCtrl::OnMouseMove(nFlags, point);
		}
		/// 
		/// \brief <pre>
		/// If there is no image, it creates a text image list. Based on the first label.
		/// </pre>
		/// \param   nItem
		/// \return  CImageList* 
		/// 
		CImageList* CListCtrlEx::CreateDragImageEx(int nItem)
		{
			CPoint		pt(8, 8);
			CRect		rcItem,
				rcImage,
				rcDraw;
			CDC 		memDC;
			CBitmap		bitmap;
			COLORREF	crMask = RGB(0, 255, 0);

			CClientDC	dc(this);

			CImageList* pImageSmall = GetImageList(LVSIL_SMALL);
			GetItemRect(nItem, &rcItem, LVIR_LABEL);
			rcItem.top = rcItem.left = 0;
			rcDraw = rcItem;

			if (pImageSmall != NULL) {
				IMAGEINFO	imageInfo;
				pImageSmall->GetImageInfo(0, &imageInfo);
				rcImage = imageInfo.rcImage;
				rcDraw.right = rcItem.Width();
				rcDraw.bottom = rcImage.Height();
				crMask = pImageSmall->GetBkColor();
			}
			if (!memDC.CreateCompatibleDC(&dc) || !bitmap.CreateCompatibleBitmap(&dc, rcDraw.Width(), rcDraw.Height())) return NULL;

			CBitmap* pOldMemDCBitmap = memDC.SelectObject(&bitmap);
			CFont* pOldFont = memDC.SelectObject(GetFont());
			memDC.FillSolidRect(&rcDraw, crMask);
			memDC.SetTextColor(RGB(0, 0, 0));

			if (pImageSmall != NULL)
				pImageSmall->Draw(&memDC, 0, CPoint(0, 0), ILD_NORMAL);

			if (pImageSmall != NULL) {
				rcDraw.left = rcImage.Width() + 5;
				memDC.DrawText(GetItemText(nItem, 0), rcDraw, DT_SINGLELINE | DT_LEFT | DT_VCENTER);
			}
			else {
				memDC.DrawText(GetItemText(nItem, 0), rcItem, DT_SINGLELINE | DT_LEFT | DT_VCENTER);
			}
			memDC.SelectObject(pOldFont);
			memDC.SelectObject(pOldMemDCBitmap);

			CImageList* pImageList = new CImageList;
			pImageList->Create(rcDraw.Width(), rcDraw.Height(), ILC_COLOR | ILC_MASK, 0, 1);
			pImageList->Add(&bitmap, crMask);
			return pImageList;
		}
		/// 
		/// \brief <pre>
		/// Sets whether or not to use UnderLine.
		/// </pre>
		/// \param   bUse	- use or not
		/// \param   nUnderLineStyle	- UnderLine PEN STYLE (PS_SOLID, PS_DOT)
		/// \param   clrUnderLine	- Color
		/// 
		void CListCtrlEx::SetUnderLine(BOOL bUse, UINT nUnderLineStyle, COLORREF clrUnderLine)
		{
			m_bUnderLine = bUse;
			m_clrUnderLine = clrUnderLine;
			m_nUnderLineStyle = nUnderLineStyle;
			if (bUse) m_bGrid = FALSE;
		}
		/// 
		/// \brief <pre>
		/// Loads the image to use as the list background. Focus / Use six images to be used as the base image.
		/// </pre>
		/// \param   nIDFLeft	- Default BG Left
		/// \param   nIDFMiddle	- Default BG Middle
		/// \param   nIDFRight	- Default BG Right
		/// \param   nIDDLeft	- Focus BG Left
		/// \param   nIDDMiddle	- Focus BG Middle
		/// \param   nIDDRight	- Focus BG Right
		/// 
		void CListCtrlEx::LoadListImg(UINT nIDFLeft, UINT nIDFMiddle, UINT nIDFRight, UINT nIDDLeft, UINT nIDDMiddle, UINT nIDDRight)
		{
			CBitmap	bmpTmp;
			CRect	rcClient;
			BITMAP	bm;
			CDC		memDC,
				bufDC;

			if (m_bmpDefault.GetSafeHandle())	m_bmpDefault.DeleteObject();
			if (m_bmpFocus.GetSafeHandle())	m_bmpFocus.DeleteObject();

			GetClientRect(&rcClient);
			int		nWidth = rcClient.Width();
			CDC* pDC = GetDC();

			memDC.CreateCompatibleDC(pDC);
			bufDC.CreateCompatibleDC(pDC);

			m_bmpDefault.CreateCompatibleBitmap(pDC, nWidth, m_nItemHeight);
			m_bmpFocus.CreateCompatibleBitmap(pDC, nWidth, m_nItemHeight);

			bmpTmp.LoadBitmap(MAKEINTRESOURCE(nIDDMiddle));
			bmpTmp.GetBitmap(&bm);

			CBitmap* pOldbmp = memDC.SelectObject(&bmpTmp);
			CBitmap* pOldbmp2 = bufDC.SelectObject(&m_bmpDefault);

			// DEFAULT 이미지를 생성한다. ---------------------------------------------
			bufDC.StretchBlt(0, 0, nWidth, m_nItemHeight, &memDC, 0, 0, bm.bmWidth, bm.bmHeight, SRCCOPY);
			bmpTmp.DeleteObject(); memDC.SelectObject(pOldbmp);

			// IDB_DEFAULTL이미지를 로드한다.
			bmpTmp.LoadBitmap(MAKEINTRESOURCE(nIDDLeft));
			bmpTmp.GetBitmap(&bm);

			pOldbmp = memDC.SelectObject(&bmpTmp);
			bufDC.StretchBlt(0, 0, bm.bmWidth, bm.bmHeight, &memDC, 0, 0, bm.bmWidth, bm.bmHeight, SRCCOPY);

			bmpTmp.DeleteObject(); memDC.SelectObject(pOldbmp);

			// IDB_DEFAULTR이미지를 로드한다.
			bmpTmp.LoadBitmap(MAKEINTRESOURCE(nIDDRight));
			bmpTmp.GetBitmap(&bm);

			pOldbmp = memDC.SelectObject(&bmpTmp);
			bufDC.StretchBlt(nWidth - bm.bmWidth, 0, bm.bmWidth, bm.bmHeight, &memDC, 0, 0, bm.bmWidth, bm.bmHeight, SRCCOPY);
			//-------------------------------------------------------------------------
			bmpTmp.DeleteObject(); memDC.SelectObject(pOldbmp); bufDC.SelectObject(pOldbmp2);

			bmpTmp.LoadBitmap(MAKEINTRESOURCE(nIDFMiddle));
			bmpTmp.GetBitmap(&bm);

			pOldbmp = memDC.SelectObject(&bmpTmp);
			pOldbmp2 = bufDC.SelectObject(&m_bmpFocus);
			// DEFAULT 이미지를 생성한다. ---------------------------------------------
			bufDC.StretchBlt(0, 0, nWidth, m_nItemHeight, &memDC, 0, 0, bm.bmWidth, bm.bmHeight, SRCCOPY);
			bmpTmp.DeleteObject(); memDC.SelectObject(pOldbmp);

			// IDB_FOCUSL 이미지를 로드한다.
			bmpTmp.LoadBitmap(MAKEINTRESOURCE(nIDFLeft));
			bmpTmp.GetBitmap(&bm);

			pOldbmp = memDC.SelectObject(&bmpTmp);
			bufDC.StretchBlt(0, 0, bm.bmWidth, bm.bmHeight, &memDC, 0, 0, bm.bmWidth, bm.bmHeight, SRCCOPY);
			bmpTmp.DeleteObject(); memDC.SelectObject(pOldbmp);

			// IDB_FOCUSR 이미지를 로드한다.
			bmpTmp.LoadBitmap(MAKEINTRESOURCE(nIDFRight));
			bmpTmp.GetBitmap(&bm);
			pOldbmp = memDC.SelectObject(&bmpTmp);
			bufDC.StretchBlt(nWidth - bm.bmWidth, 0, bm.bmWidth, bm.bmHeight, &memDC, 0, 0, bm.bmWidth, bm.bmHeight, SRCCOPY);
			//-------------------------------------------------------------------------

			bmpTmp.DeleteObject();
			bufDC.SelectObject(pOldbmp2);
			memDC.SelectObject(pOldbmp);
			ReleaseDC(pDC);
		}
		/// 
		/// \brief <pre>
		/// Draw a base image. The base image should be created via LoadListImage.
		/// </pre>
		/// \param   pDC	- Dest DC
		/// \param   rcRgn	- Draw Image Region
		/// 
		void CListCtrlEx::DrawDefaultImg(CDC* pDC, CRect rcRgn)
		{
			if (!m_bmpDefault.GetSafeHandle()) return;
			CDC		memDC;
			BITMAP	bm;
			memDC.CreateCompatibleDC(pDC);
			CBitmap* pOldBmp = memDC.SelectObject(&m_bmpDefault);
			m_bmpDefault.GetBitmap(&bm);
			pDC->StretchBlt(rcRgn.left, rcRgn.top, rcRgn.Width(), rcRgn.Height(), &memDC, 0, 0, bm.bmWidth, bm.bmHeight, SRCCOPY);
			memDC.SelectObject(pOldBmp);
		}
		/// 
		/// \brief <pre>
		/// Draws a focus image.
		/// </pre>
		/// \param   pDC	- Dest DC
		/// \param   rcRgn	- Draw Image Region
		/// 
		void CListCtrlEx::DrawFocusImg(CDC* pDC, CRect rcRgn)
		{
			if (!m_bmpFocus.GetSafeHandle()) return;
			CDC		memDC;
			BITMAP	bm;
			memDC.CreateCompatibleDC(pDC);
			CBitmap* pOldBmp = memDC.SelectObject(&m_bmpFocus);
			m_bmpDefault.GetBitmap(&bm);
			pDC->StretchBlt(rcRgn.left, rcRgn.top, rcRgn.Width(), rcRgn.Height(), &memDC, 0, 0, bm.bmWidth, bm.bmHeight, SRCCOPY);
			memDC.SelectObject(pOldBmp);
		}
		/// 
		/// \brief <pre>
		/// Disabling the keyboard disregards the processing of the four directional keys.
		/// </pre>
		/// \param   pMsg
		/// \return  BOOL 
		/// 
		BOOL CListCtrlEx::PreTranslateMessage(MSG* pMsg)
		{
			if (pMsg->message == WM_KEYDOWN) {
				if (m_bUseKeyboard && (pMsg->wParam == 37 || pMsg->wParam == 38 || pMsg->wParam == 39 || pMsg->wParam == 40)) {
					pMsg->message = NULL;
				}
			}
			return CListCtrl::PreTranslateMessage(pMsg);
		}
		/// 
		/// \brief <pre>
		/// Delete all headers.
		/// </pre>
		/// 
		void CListCtrlEx::RemoveAllHeader()
		{
			int nColumnCount = GetHeaderCtrl()->GetItemCount();
			for (int i = 0; i < nColumnCount; ++i) DeleteColumn(0);
		}
		/// 
		/// \brief <pre>
		/// Whether multiple lines are used
		/// </pre>
		/// \param   bMulti		- 사용 여부
		/// \param   nTopGap	- TA_UPDATECP를 사용하면 세로 정렬이 안되므로 이를 보완할 TOP좌표 보정값
		/// 
		void CListCtrlEx::SetMultiLine(BOOL bMulti, int nTopGap)
		{
			m_bUseMulti = bMulti;
			m_nTopGap = nTopGap;
		}
		/// 
		/// \brief <pre>
		/// Hide List ScrollBar Control. if under CE Not support Windows.
		/// </pre>
		/// 
		void CListCtrlEx::HideScrollBar()
		{
			RECT ierect;
			int cxvs, cyvs;
			GetClientRect(&ierect);
			cxvs = GetSystemMetrics(SM_CXVSCROLL);
			cyvs = GetSystemMetrics(SM_CYVSCROLL);
			SetWindowPos(NULL, ierect.left, ierect.top, ierect.right + cxvs, ierect.bottom + cyvs, SWP_NOMOVE | SWP_NOZORDER);
			ierect.right -= ierect.left;
			//ierect.bottom -= cyvs;
			ierect.top = 0;
			ierect.left = 0;
			HRGN iehrgn = NULL;
			iehrgn = CreateRectRgn(ierect.left, ierect.top + 2, ierect.right - 2, ierect.bottom + 5);
			SetWindowRgn(iehrgn, TRUE);
		}
		/// 
		/// \brief <pre>
		/// If there is no list control item, print the default output message.
		/// </pre>
		/// 
		void CListCtrlEx::OnPaint()
		{
			if (GetItemCount() != 0) {
				Default();
				return;
			}
			CPaintDC dc(this);
			if ((m_pHandler && !m_pHandler->DrawEmptyMsg(&dc)) || !m_pHandler) {
				CRect Rect;
				GetClientRect(Rect);
				UINT nFormat = DT_CENTER | DT_WORDBREAK | DT_NOPREFIX | DT_NOCLIP | DT_VCENTER | DT_SINGLELINE;
				CFont* m_pOldFont = dc.SelectObject(&m_fntList);
				dc.SetTextColor(m_clrDefaultTxtColor);
				dc.SetBkMode(TRANSPARENT);
				dc.DrawText(m_strDefaultMsg, Rect, nFormat);
				dc.SelectObject(m_pOldFont);
			}
		}
		/// 
		/// \brief <pre>
		/// Handle onSize
		/// </pre>
		/// \param   nType
		/// \param   cx
		/// \param   cy
		/// 
		void CListCtrlEx::OnSize(UINT nType, int cx, int cy)
		{
			CListCtrl::OnSize(nType, cx, cy);
			if (GetItemCount() == 0) Invalidate();
		}
		/// 
		/// \brief <pre>
		/// Obtain the index number of the selected list.
		/// </pre>
		///	\return	int	- List selection index number
		/// 
		int	CListCtrlEx::GetSelectedListIdx()
		{
			POSITION pos = GetFirstSelectedItemPosition();
			return pos == NULL ? -1 : GetNextSelectedItem(pos);
		}
		/// 
		/// \brief <pre>
		/// Select an item at the Index position.
		/// </pre>
		/// \param   nIdx
		/// 
		void CListCtrlEx::SelectList(int nIdx)
		{
			SetItemState(nIdx, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);
			EnsureVisible(nIdx, FALSE);
		}
		/// 
		/// \brief <pre>
		/// The selection of the Index position is reversed.
		/// </pre>
		/// \param   nIdx
		/// 
		void CListCtrlEx::UnSelect(int nIdx)
		{
			SetItemState(nIdx, 0, LVIS_SELECTED | LVIS_FOCUSED);
			m_nSelectItem = -1;
		}
		/// 
		/// \brief <pre>
		/// Set the bit in the header.
		/// </pre>
		/// \param   nCol	- column index number
		/// \param   bitmap	- bitmap handle
		/// 
		void CListCtrlEx::SetHeaderBitmap(int nCol, HBITMAP bitmap)
		{
			if (bitmap == NULL) return;
			HDITEM    curItem;
			curItem.mask = HDI_FORMAT;
			CHeaderCtrl* pHeader = GetHeaderCtrl();
			pHeader->GetItem(nCol, &curItem);
			curItem.mask = HDI_BITMAP | HDI_FORMAT;
			curItem.fmt |= HDF_BITMAP;
			curItem.hbm = bitmap;

			pHeader->SetItem(nCol, &curItem);
			m_nSortedCol = nCol;
		}
		/// 
		/// \brief <pre>
		/// Unset the bit set in the header.
		/// </pre>
		/// \param   nCol
		/// 
		void CListCtrlEx::DeleteHeaderBitmap(int nCol)
		{
			if (nCol < 0) return;
			HD_ITEM hditem;
			CHeaderCtrl* pHeader = (CHeaderCtrl*)GetDlgItem(0);

			hditem.mask = HDI_FORMAT;
			pHeader->GetItem(nCol, &hditem);

			hditem.mask = HDI_FORMAT;
			hditem.fmt &= ~HDF_BITMAP;
			pHeader->SetItem(nCol, &hditem);
		}
		/// 
		/// \brief <pre>
		/// Header click processing.
		/// </pre>
		/// \param   pNMHDR
		/// \param   pResult
		/// 
		void CListCtrlEx::OnHeaderClicked(NMHDR* pNMHDR, LRESULT* pResult)
		{
			//if( !m_bSort ) return;
			HD_NOTIFY* phdn = (HD_NOTIFY*)pNMHDR;
			if (phdn->iButton == 0 && GetItemCount() != 0) {
				if (m_pHandler) m_pHandler->OnColumnClick(phdn->iItem, m_nSortedCol);
				m_nSortedCol = phdn->iItem;
			}
		}
		/// 
		/// \brief <pre>
		/// Set the font.
		/// </pre>
		/// \param   szFont	- font name
		/// \param   nFontSize	- size
		/// \param   a_bBold - is bold
		/// 
		void CListCtrlEx::SetFont(TCHAR* szFont, int nFontSize, BOOL a_bBold, BOOL a_bClearFont)
		{
			if (m_fntList.GetSafeHandle()) m_fntList.DeleteObject();
			LOGFONT lf;
			memset(&lf, 0, sizeof(LOGFONT));
			_tcscpy_s(lf.lfFaceName, szFont);
			lf.lfWeight = a_bBold ? FW_BOLD : FW_NORMAL;
			lf.lfHeight = nFontSize;
			lf.lfCharSet = HANGEUL_CHARSET;
			lf.lfPitchAndFamily = FF_SWISS | DEFAULT_PITCH;
			if (a_bClearFont) lf.lfQuality = CLEARTYPE_QUALITY;
			m_fntList.CreateFontIndirect(&lf);
		}
		/// 
		/// \brief <pre>
		/// Sets whether to draw grid lines. You can not use underscores when setting.
		/// </pre>
		/// \param   bGrid
		/// \param   clrGridLine
		/// 
		void CListCtrlEx::SetGrid(BOOL bGrid, COLORREF clrGridLine)
		{
			m_bGrid = bGrid;
			m_clrGridLine = clrGridLine;
			if (m_bGrid) m_bUnderLine = FALSE;
		}
		/// 
		/// \brief <pre>
		/// Start sorting.
		/// </pre>
		/// \param   bAsc
		/// \param   nDataType
		/// 
		void CListCtrlEx::Sort(BOOL bAsc, int nDataType)
		{
			if (!m_bSort || GetItemCount() == 0) return;

			CSortClass csc(this, m_nSortedCol);
			CSortClass::EDataType dataType;
			switch (nDataType)
			{
			case 0:
				dataType = CSortClass::dtSTRING;
				break;
			case 1:
				dataType = CSortClass::dtINT;
				break;
			}
			csc.Sort(bAsc ? true : false, dataType);
		}
		/// 
		/// \brief <pre>
		/// Each column returns a character alignment status.
		/// </pre>
		/// \param   nCol
		/// \return  UINT - DT_CENTER, DT_LEFT, DT_RIGHT
		/// 
		UINT CListCtrlEx::GetColumnFMT(int nCol)
		{
			LV_COLUMN	lvc;
			lvc.mask = LVCF_FMT;
			GetColumn(nCol, &lvc);
			UINT nTxtAlign = DT_LEFT;
			switch (lvc.fmt & LVCFMT_JUSTIFYMASK)
			{
			case LVCFMT_RIGHT:
				nTxtAlign = DT_RIGHT;
				break;
			case LVCFMT_CENTER:
				nTxtAlign = DT_CENTER;
				break;
			}
			return nTxtAlign;
		}
		/// 
		/// \brief <pre>
		/// Adjust the height of the header control. The list control's header control
		/// There is no way to set the font.
		/// </pre>
		/// \param   a_nHeight
		/// 
		void CListCtrlEx::SetHeaderHeight(UINT a_nHeight)
		{
			LOGFONT lf;
			memset(&lf, 0, sizeof(LOGFONT));
			_tcscpy_s(lf.lfFaceName, _T("굴림"));
			lf.lfWeight = FW_NORMAL;
			lf.lfHeight = a_nHeight;
			lf.lfCharSet = HANGEUL_CHARSET;
			lf.lfPitchAndFamily = FF_SWISS | DEFAULT_PITCH;
			m_fntHeader.CreateFontIndirect(&lf);
			CListCtrl::SetFont(&m_fntHeader);
		}
		/// 
		/// \brief
		/// Called when LVN_OWNERDATA.
		/// NMLVDISPINFO *pDispInfo = reinterpret_cast<NMLVDISPINFO*>(pNMHDR);
		/// LV_ITEM* pItem= &(pDispInfo)->item;
		/// \param   *pNMHDR
		/// \param   *pResult
		/// 
		void CListCtrlEx::OnLvnGetdispinfo(NMHDR* pNMHDR, LRESULT* pResult)
		{
			if (m_pHandler) m_pHandler->OnLvnGetdispinfo(pNMHDR, pResult);
			*pResult = 0;
		}

		//--------- CSorting Class Function ----------------------------------------------------------------------------

		CSortClass::CSortClass(CListCtrl* _pWnd, const int _iCol)
		{
			pWnd = _pWnd;

			ASSERT(pWnd);
			int max = pWnd->GetItemCount();
			DWORD dw;
			CString txt;

			// replace Item data with pointer to CSortItem structure
			for (int t = 0; t < max; t++)
			{
				dw = pWnd->GetItemData(t); // save current data to restore it later
				txt = pWnd->GetItemText(t, _iCol);
				pWnd->SetItemData(t, (DWORD) new CSortItem(dw, txt));
			}
		}

		CSortClass::~CSortClass()
		{
			ASSERT(pWnd);
			int max = pWnd->GetItemCount();
			CSortItem* pItem;
			for (int t = 0; t < max; t++)
			{
				pItem = (CSortItem*)pWnd->GetItemData(t);
				ASSERT(pItem);
				pWnd->SetItemData(t, pItem->dw);
				delete pItem;
			}
		}

		void CSortClass::Sort(bool _bAsc, EDataType _dtype)
		{
			long lParamSort = _dtype;

			// if lParamSort positive - ascending sort order, negative - descending
			if (!_bAsc)
				lParamSort *= -1;

			pWnd->SortItems(Compare, lParamSort);
		}

		int CALLBACK CSortClass::Compare(LPARAM lParam1, LPARAM lParam2, LPARAM
			lParamSort)
		{
			CSortItem* item1 = (CSortItem*)lParam1;
			CSortItem* item2 = (CSortItem*)lParam2;
			ASSERT(item1 && item2);

			// restore data type and sort order from lParamSort
			// if lParamSort positive - ascending sort order, negative - descending
			short   sOrder = lParamSort < 0 ? -1 : 1;
			EDataType dType = (EDataType)(lParamSort * sOrder); // get rid of sign

			// declare typed buffers
			COleDateTime t1, t2;
#ifndef UNDER_CE
			switch (dType)
			{
			case  dtINT:
				return (_tstoi(item1->txt) - _tstoi(item2->txt)) * sOrder;
			case  dtDEC:
				return (_tstof(item1->txt) < _tstof(item2->txt) ? -1 : 1) * sOrder;
			case  dtDATETIME:
				if (t1.ParseDateTime(item1->txt) && t2.ParseDateTime(item2->txt))
					return (t1 < t2 ? -1 : 1) * sOrder;
				else
					return 0;
			case  dtSTRING:
				return item1->txt.CompareNoCase(item2->txt) * sOrder;

			default:
				ASSERT("Error: attempt to sort a column without type.");
				return 0;
			}
#endif
			return 0;
		}


		CSortClass::CSortItem::CSortItem(const DWORD _dw, const CString& _txt)
		{
			dw = _dw;
			txt = _txt;
		}


	}	// end of namespace UI
}	// end of namespace MFC