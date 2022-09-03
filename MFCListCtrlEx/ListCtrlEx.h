#pragma once

#include "FlatHeader.h"
#include "ListHandlerEx.h"

// CListCtrlEx
namespace MFC {
	namespace UI {

class CListCtrlEx : public CListCtrl
{
public:
	CListCtrlEx();
	virtual ~CListCtrlEx();

	enum IMGFILETYPE { PNG = 0, OTHER };
	enum BKIMGALIGN	{ BK_TILT = 0, BK_CENTER, BK_RIGHT_BOTTOM, BK_LEFT_BOTTOM, BK_LEFT_TOP, BK_RIGHT_TOP };

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CListCtrlEx)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CListCtrlEx)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg BOOL OnQueryNewPalette();
	afx_msg void OnPaletteChanged(CWnd* pFocusWnd);
	afx_msg void OnColumnclick(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnClick(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnRclick(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnBegindrag(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnDblclk(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnItemchanged(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnHeaderClicked(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnLvnDeleteitem(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnLvnInsertitem(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnPaint();
	afx_msg void OnLvnDeleteallitems(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnSize(UINT nType, int cx, int cy);	
	//}}AFX_MSG
	afx_msg void MeasureItem ( LPMEASUREITEMSTRUCT lpMeasureItemStruct );
	afx_msg void OnLvnGetdispinfo(NMHDR *pNMHDR, LRESULT *pResult);
	DECLARE_MESSAGE_MAP()
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);

public:	
	BOOL		Create(CWnd* pWnd, int x, int y, int cx, int cy, BOOL bShowHeader = TRUE, BOOL a_bBorder = FALSE, BOOL a_bOwnerData = FALSE, UINT nID = ID_MAIN_LIST);
	BOOL		SetBkImage(TCHAR* a_pstrFileName, IMGFILETYPE a_nFileType, BKIMGALIGN a_nBkAlign);
	BOOL		OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
	void		LoadListImg(UINT nIDFLeft, UINT nIDFMiddle, UINT nIDFRight, UINT nIDDLeft, UINT nIDDMiddle, UINT nIDDRight);
	void		DrawDefaultImg(CDC* pDC, CRect rcRgn);
	void		DrawFocusImg(CDC* pDC, CRect rcRgn);

	// Drag & Drop
	BOOL		DropItems();
	CImageList* CreateDragImageEx(int nItem);
	
	// Helper Function
	int			AddItem(TCHAR* pszText, int a_nPos = -1);
	void		AddColumnText(int nWidth, LPTSTR strTitle, UINT nAlign = LVCFMT_CENTER);
	void		UpdateColumnText(int a_nIdx, LPTSTR strTitle);
	void		SelectList(int nIdx);
	void		UnSelect(int nIdx);
	UINT		GetColumnFMT(int nCol);
	int			GetSelectedListIdx();
	void		HideScrollBar();
	void		RemoveAllHeader();
	void		DeleteHeaderBitmap(int nCol);
	void		Sort(BOOL bAsc, int nDataType);
	void		SetHeaderHeight(UINT a_nHeight);

	// Getter/Setter Function
	void		SetItemText(int nIdx, int nPos, const TCHAR* pszText);	
	void		SetGrid(BOOL bGrid, COLORREF clrGridLine);
	BOOL		GetGrid() { return m_bGrid; }
	COLORREF	GetGridColor() { return m_clrGridLine; }
	void		SetFont(TCHAR* szFont, int nFontSize, BOOL a_bBold = FALSE, BOOL a_bClearFont = TRUE);
	void		SetMultiLine(BOOL bMulti, int nTopGap);
	BOOL		GetMultiLine() { return m_bUseMulti; }
	int			GetMultiLineTopGap() { return m_nTopGap; }	
	void		SetHeaderBitmap(int nCol, HBITMAP bitmap);
	void		SetUnderLine(BOOL bUse, UINT nUnderLineStyle = PS_DOT, COLORREF clrUnderLine = RGB(192,192,192));
	BOOL		GetUnderLine() { return m_bUnderLine; }
	UINT		GetUnderLineStyle() { return m_nUnderLineStyle; }
	COLORREF	GetUnderLineColor() { return m_clrUnderLine; }
	void		SetHilightBGColor(COLORREF clrHilightBG)	{ m_clrHilightBG = clrHilightBG; }
	COLORREF	GetHilightBGColor() { return m_clrHilightBG; }
	void		SetDefaultBGColor(COLORREF clrDefault)		{ m_clrDefaultBG = clrDefault; }
	COLORREF	GetDefaultBGColor() { return m_clrDefaultBG; }
	void		SetHilightTxtColor(COLORREF clrHilight)		{ m_clrHilightTxt = clrHilight; }
	COLORREF	GetHilightTxtColor() { return m_clrHilightTxt; }
	void		SetDefaultTxtColor(COLORREF clrDefault)		{ m_clrDefaultTxt = clrDefault; }
	COLORREF	GetDefaultTxtColor() { return m_clrDefaultTxt; }
	void		SetLoseFocusBGColor(COLORREF a_clrBG) { m_clrLoseFocusBG = a_clrBG; }
	COLORREF	GetLoseFocusBGColor() { return m_clrLoseFocusBG; }
	void		SetLoseFocusTxtColor(COLORREF a_clrTxt) { m_clrLoseFocusTxt = a_clrTxt; }
	COLORREF	GetLoseFocusTxtColor() { return m_clrLoseFocusTxt; }
	void		SetUseKeyboard(BOOL bKeyboard)		{ m_bUseKeyboard = bKeyboard; }
	BOOL		GetUseKeyboard() { return m_bUseKeyboard; }
	void		SetListHeight(int nHeight)	{ m_nItemHeight = nHeight; }
	int			GetListHeight()				{ return m_nItemHeight; }	
	void		SetDefaultMsg(TCHAR* szMsg) { m_strDefaultMsg = szMsg; }
	CString		GetDefaultMsg() { return m_strDefaultMsg; }	
	void		SetDefaulMsgTxtColor(COLORREF a_clrColor) { m_clrDefaultTxtColor = a_clrColor; }
	void		SetDrawFocusRect(BOOL bDraw){ m_bDrawFocusRect = bDraw; }
	BOOL		GetDrawFocusRect() { return m_bDrawFocusRect; }
	void		SetHandler(IListHandlerEx* a_pHandler) { m_pHandler = a_pHandler; }
 
	void		UseSort(BOOL bSort) { m_bSort = bSort; }
	BOOL		IsUseSort() { return m_bSort; }
	void		ShowHeader(BOOL bShow)		{ m_bShowHeader = bShow; }
	BOOL		IsShowHeader() { return m_bShowHeader; }
	CBitmap*	GetBkImg() { return &m_bmpBkImg; }
	void		SetSortColumn(int a_nSortCol) { m_nSortedCol = a_nSortCol; }
	FlatHeader*	GetHeader() { return &m_HeaderCtrl; }
private:
	void		DrawOption(CDC* a_pDC, BOOL a_bFocus, const CRect& a_rcBounds, const CRect& a_rcHighlight);
	void		DrawBkImg(CDC* a_pDC);

private:	
	enum EHighlight {HIGHLIGHT_NORMAL, HIGHLIGHT_ALLCOLUMNS, HIGHLIGHT_ROW};
	enum { DEFAULT_LIST_HEIGHT = 24, ID_MAIN_LIST = 1000 };

	BOOL				m_bDrag,
						m_bUseKeyboard,
						m_bGrid,
						m_bSort,
						m_bShowHeader,
						m_bDrawFocusRect,
						m_bBorder,
						m_bOwnerData;
	int					m_nItemHeight,
						m_cxBitmap, 
						m_cyBitmap,
						m_nSelectItem,
						m_nSortedCol;

	CPalette			m_pal;
	FlatHeader			m_HeaderCtrl;
	CString				m_strDefaultMsg;

	// Drag & Drop
	CImageList*			m_pDragImage;
	int					m_nDragIndex,
						m_nDropIndex,
						m_nTopGap,
						m_nUnderLineStyle;

	BOOL				m_bUseDragDrop,
						m_bDragging,
						m_bUnderLine,
						m_bUseMulti;

	HTREEITEM			m_hDropItem;
	CWnd*				m_pDropWnd;
	COLORREF			m_clrUnderLine,
						m_clrHilightBG,
						m_clrDefaultBG,
						m_clrHilightTxt,
						m_clrDefaultTxt,
						m_clrLoseFocusBG,
						m_clrLoseFocusTxt,
						m_clrGridLine,
						m_clrDefaultTxtColor;

	// Bitmap List operation
	CBitmap				m_bmpDefault,
						m_bmpFocus,
						m_bmpBkImg;
	BKIMGALIGN			m_BkImgAlign;

	CFont				m_fntList,
						m_fntHeader;
	IListHandlerEx*		m_pHandler;
	CDefaultListHandler	m_DefaultHandler;
};

/// \brief <pre>
/// List sort module. From codeguru... 
/// 
/// ---------------------------------------------------------------------------
/// Date          Author      Comment
/// ---------------------------------------------------------------------------
/// 2009-11-20    YangManWoo  Applyed Coding Guide
/// 
/// </pre>
class CSortClass
{
public:
	enum EDataType {dtNULL, dtINT, dtSTRING, dtDATETIME, dtDEC};
	
	CSortClass(CListCtrl * _pWnd, const int _iCol);
	virtual ~CSortClass();
	void Sort(bool bAsc, EDataType _dtype);
	
protected:
	CListCtrl * pWnd;
	
	static int CALLBACK Compare(LPARAM lParam1, LPARAM lParam2, LPARAM 
		lParamSort);
	
	struct CSortItem
	{
		CSortItem(const DWORD _dw, const CString &_txt);
		DWORD dw; 
		CString txt;
	};
};

	}	// end of namespace UI
}	// end of namespace MFC