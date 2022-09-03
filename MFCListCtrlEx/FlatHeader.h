#pragma once
/// Flat Header Control.
/// 
/// ---------------------------------------------------------------------------
/// Date			Author      Comment
/// ---------------------------------------------------------------------------
/// 2022-09-03		HJ-KIM	
///
namespace MFC {
	namespace UI {

class FlatHeader : public CHeaderCtrl
{
public:
	FlatHeader();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(FlatHeader)
	//}}AFX_VIRTUAL
public:
	virtual ~FlatHeader();

protected:
	//{{AFX_MSG(FlatHeader)
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnPaint();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()

public:
	void	SetSortImage(int nCol, BOOL bAsc);  
	int		GetSortImage(int a_nCol);
	void	SetSortColor(COLORREF a_clrSort) { m_clrSort = a_clrSort; }
	void	SetFont(TCHAR* a_pstrFontName, UINT a_nSize);

private:
	void	DrawCtrl(CDC* pDC);	
	INT		DrawText(CDC* pDC, CRect rect, LPHDITEM lphdi);	
	void	DrawItem(int nItem, int nItemState);
	INT		DrawImage(CDC* pDC, CRect rect, LPHDITEM lphdi, BOOL bRight);
	INT		DrawBitmap(CDC* pDC, CRect rect, LPHDITEM lphdi, CBitmap* pBitmap, BITMAP* pBitmapInfo, BOOL bRight);

private:
	enum			{ HEADER_TXT_MAX = 80 };
	int				m_nSortCol;
	BOOL			m_bSortAsc;
	CFont			m_fntHeader;
	COLORREF		m_cr3DHighLight,
					m_cr3DShadow,
					m_cr3DFace,
					m_crText,
					m_clrSort;
	INT				m_iHotIndex; // Column value index value to be dragged.. 	
	INT				m_iHotOrder;
	INT				m_iSpacing;	
	INT				m_iSortColumn;
	HDHITTESTINFO	m_hdhtiHotItem;
	SIZE			m_sizeImage;
};

	} // end of namespace UI
} // end of namespace MFC