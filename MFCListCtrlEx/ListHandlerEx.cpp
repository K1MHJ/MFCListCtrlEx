#include "pch.h"
#include "ListHandlerEx.h"
#include "ListCtrlEx.h"

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
CDefaultListHandler::CDefaultListHandler(CListCtrlEx* a_pList) : IListHandlerEx(a_pList)
{
}
/// 
/// \brief <pre>
/// Destructor
/// </pre>
/// 
CDefaultListHandler::~CDefaultListHandler()
{
}
/// 
/// \brief <pre>
/// Print the raw background. If the background image is output, the default background is not output.
/// </pre>
/// \param   a_pDC
/// \param   a_nItemIdx
/// \param   a_bHighlight
/// \param   a_bFocus
/// \param   a_rcBounds
/// 
void CDefaultListHandler::DrawItemBG(CDC* a_pDC, int a_nItemIdx, BOOL a_bHighlight, BOOL a_bFocus, const CRect& a_rcBounds, bool& a_bDrawDefault) 
{
	if( a_bHighlight ) {
		if( a_bFocus ) a_pDC->SetBkColor(m_pList->GetHilightBGColor());
		a_pDC->FillRect(a_rcBounds, &CBrush(a_bFocus ? m_pList->GetHilightBGColor() : m_pList->GetLoseFocusBGColor()));
	} else if( !m_pList->GetBkImg()->GetSafeHandle() ) {
		a_pDC->FillRect(a_rcBounds, &CBrush(m_pList->GetDefaultBGColor()));
	}
}
/// 
/// \brief <pre>
/// 
/// </pre>
/// \param   a_pDC
/// \param   a_nItemIdx
/// \param   a_bHighlight
/// \param   a_bFocus
/// \param   a_rcCol
/// 
void CDefaultListHandler::DrawStateIconImage(CDC* a_pDC, int a_nItemIdx, BOOL a_bHighlight, BOOL a_bFocus, const CRect& a_rcCol, bool& a_bDrawDefault)
{
	CImageList*	pImgList = NULL;
	LV_ITEM lvi;	
	lvi.mask = LVIF_IMAGE | LVIF_STATE;
	lvi.iItem = a_nItemIdx;
	lvi.iSubItem = 0;
	lvi.stateMask = 0xFFFF;		// get all state flags
	m_pList->GetItem(&lvi);
	if ( lvi.state & LVIS_STATEIMAGEMASK ) {
		int nImage = ((lvi.state & LVIS_STATEIMAGEMASK)>>12) - 1;
		pImgList = m_pList->GetImageList(LVSIL_STATE);
		if (pImgList) {
			int nTop = a_rcCol.top + a_rcCol.Height()/2 - 6;
			pImgList->Draw(a_pDC, nImage,	CPoint(a_rcCol.left, nTop), ILD_TRANSPARENT);
		}
	}	
}
/// 
/// \brief <pre>
/// Draw normal and overlay icon, the part displayed by default in the first column when applied to the image list
/// </pre>
/// \param   a_pDC
/// \param   a_nItemIdx
/// \param   a_bHighlight
/// \param   a_bFocus
/// \param   a_pStatImgList
/// \param   a_rcIcon
/// 
void CDefaultListHandler::DrawSmallIconImage(CDC* a_pDC, int a_nItemIdx, BOOL a_bHighlight, BOOL a_bFocus, const CRect& a_rcIcon, bool& a_bDrawDefault)
{
	CImageList* pImgList = m_pList->GetImageList(LVSIL_SMALL);
	if( pImgList ) {
		LV_ITEM lvi;	
		lvi.mask = LVIF_IMAGE | LVIF_STATE;
		lvi.iItem = a_nItemIdx;
		lvi.iSubItem = 0;
		lvi.stateMask = 0xFFFF;		// get all state flags
		m_pList->GetItem(&lvi);
		UINT nOvlImageMask=lvi.state & LVIS_OVERLAYMASK;
		pImgList->Draw(a_pDC, lvi.iImage, CPoint(a_rcIcon.left, a_rcIcon.top), (a_bHighlight ? ILD_BLEND50 : 0) | ILD_TRANSPARENT | nOvlImageMask );
	}
}
/// 
/// \brief <pre>
/// Draw Sub Item
/// </pre>
/// \param   a_pDC
/// \param   a_nItemIdx
/// \param   a_nSubItemIdx
/// \param   a_bHighlight
/// \param   a_bFocus
/// \param   a_rcLabel
/// 
void CDefaultListHandler::DrawSubItem(CDC* a_pDC, int a_nItemIdx, int a_nSubItemIdx, BOOL a_bHighlight, BOOL a_bFocus, const CRect& a_rcLabel, bool& a_bDrawDefault)
{
	if( a_bHighlight ) {
		a_pDC->SetTextColor(a_bFocus ? m_pList->GetHilightTxtColor() : m_pList->GetLoseFocusTxtColor());
	} else a_pDC->SetTextColor(m_pList->GetDefaultTxtColor());

	CString sLabel = m_pList->GetItemText(a_nItemIdx, a_nSubItemIdx);
	a_pDC->SetBkMode(TRANSPARENT);
	UINT nJustify = m_pList->GetColumnFMT(a_nSubItemIdx);
	if( m_pList->GetMultiLine() ) {
		CString strTxt, strTmp;
		strTmp = sLabel;
		int nPos = strTmp.Find(_T("\r\n"));
		if( nPos != -1 ) {
			strTxt = strTmp.Left(nPos);
			strTmp.Delete(0, nPos+2);
			CRect rcMulti = a_rcLabel;
			rcMulti.bottom = a_rcLabel.top + a_rcLabel.Height()/2;
			a_pDC->DrawText(strTxt, -1, rcMulti, nJustify | DT_SINGLELINE | DT_END_ELLIPSIS | DT_VCENTER);
			rcMulti = a_rcLabel;
			rcMulti.top = a_rcLabel.bottom - a_rcLabel.Height()/2;
			a_pDC->DrawText(strTmp, -1, rcMulti, nJustify | DT_SINGLELINE | DT_END_ELLIPSIS | DT_VCENTER);
		} else a_pDC->DrawText(sLabel, -1, CRect(a_rcLabel), nJustify | DT_SINGLELINE | DT_END_ELLIPSIS | DT_VCENTER);
	} else { 
		CRect	rcLabel(a_rcLabel);
		rcLabel.left += 4;
		rcLabel.right -= 4;
		a_pDC->DrawText(sLabel, -1, rcLabel, nJustify | DT_SINGLELINE | DT_END_ELLIPSIS | DT_VCENTER);
	}	
}


	}	// end of namespace UI
}	// end of namespace MFC