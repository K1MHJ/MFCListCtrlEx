#ifndef __LIST_HANDLER_EX__
#define __LIST_HANDLER_EX__

namespace MFC {
	namespace UI {

class CListCtrlEx;
/// \brief <pre>
/// The IListHandlerEx control handles changes to the UI and handles notifications as a CListCtrlEx control
/// To change the UI, register it through CListCtrlEx :: SetHandler ().
/// Once you register the handler, you can control the UI by each step.
///
/// If the Handler is called, it will be called when the Item in the List is drawn
/// It will be called at each step below.
///
/// 1. DrawItem BG: List Row Called when drawing a background (one whole row)
/// 2. DrawStateIconImage: Called when drawing a State Image List on a List Control.
/// 3. DrawSmallIconImage: Called when drawing a Small Imget List on a List Control.
/// 4. DrawSubItem: This is called when you have finished the above steps 1, 2, and 3 and then draw each column.
/// 
/// * The Onxxxxx method is the part for event handling in the list.
/// * If you set a_bDrawDefault = true, CDefaultListHandler will do the processing.
/// 
/// * DrawEmptyMsg () is the content to be dispatched if there is no item in the list, 
/// and if it returns false, the default message set in CListCtrlEx is output.
///
/// * CDefaultListHandler Provides a default handler, and CDefaultListHandler is not inherited.
/// Only the CListCtrlEx control is accessible.
///
/// ---------------------------------------------------------------------------
/// Date          Author      Comment
/// ---------------------------------------------------------------------------
/// 2009-11-20    YangManWoo  Originally created
/// 
/// </pre>
class IListHandlerEx
{
public:
	virtual ~IListHandlerEx() {}

	virtual bool	DrawEmptyMsg(CDC* a_pDC) { return false; }
	virtual void	DrawItemBG(CDC* a_pDC, int a_nItemIdx, BOOL a_bHighlight, BOOL a_bFocus, const CRect& a_rcBounds, bool& a_bDrawDefault) = 0;
	virtual void	DrawStateIconImage(CDC* a_pDC, int a_nItemIdx, BOOL a_bHighlight, BOOL a_bFocus, const CRect& a_rcCol, bool& a_bDrawDefault) = 0;
	virtual void	DrawSmallIconImage(CDC* a_pDC, int a_nItemIdx, BOOL a_bHighlight, BOOL a_bFocus, const CRect& a_rcIcon, bool& a_bDrawDefault) = 0;
	virtual void	DrawSubItem(CDC* a_pDC, int a_nItemIdx, int a_nSubItemIdx, BOOL a_bHighlight, BOOL a_bFocus, const CRect& a_rcLabel, bool& a_bDrawDefault) = 0;	

	virtual void	OnLButtonDown(CPoint a_ptPoint, int a_nItemIdx, int a_nSubItemIdx) {}
	virtual void	OnLButtonUp(CPoint a_ptPoint, int a_nItemIdx, int a_nSubItemIdx) {}
	virtual void	OnRbuttonClick(int a_nItem, int a_nSubItem) {}
	virtual void	OnColumnClick(int a_nIdx, int a_nOldIdx) {}
	virtual void	OnItemClick(int a_nItem, int a_nSubItem) {}
	virtual void	OnItemChange(int a_nItem, int a_nSubItem) {}
	virtual void	OnItemDBClick(int a_nItem, int a_nSubItem) {}
	virtual void	OnLvnGetdispinfo(NMHDR *pNMHDR, LRESULT *pResult) {}
protected:
	virtual void	SetListCtrl(CListCtrlEx* a_pList) { m_pList = a_pList; }
	IListHandlerEx(CListCtrlEx* a_pList) : m_pList(a_pList) {}
	IListHandlerEx(const IListHandlerEx& rhs) : m_pList(rhs.m_pList) {}
	CListCtrlEx*	m_pList;
};


/// \brief <pre>
/// Default Custom List Handler. only use default draw..
/// 
/// ---------------------------------------------------------------------------
/// Date          Author      Comment
/// ---------------------------------------------------------------------------
/// 2009-11-20    YangManWoo  Originally created
/// 
/// </pre>
class CDefaultListHandler : public IListHandlerEx
{
public:	
	~CDefaultListHandler();

	void	DrawItemBG(CDC* a_pDC, int a_nItemIdx, BOOL a_bHighlight, BOOL a_bFocus, const CRect& a_rcBounds, bool& a_bDrawDefault);
	void	DrawStateIconImage(CDC* a_pDC, int a_nItemIdx, BOOL a_bHighlight, BOOL a_bFocus, const CRect& a_rcCol, bool& a_bDrawDefault);
	void	DrawSmallIconImage(CDC* a_pDC, int a_nItemIdx, BOOL a_bHighlight, BOOL a_bFocus, const CRect& a_rcIcon, bool& a_bDrawDefault);
	void	DrawSubItem(CDC* a_pDC, int a_nItemIdx, int a_nSubItemIdx, BOOL a_bHighlight, BOOL a_bFocus, const CRect& a_rcLabel, bool& a_bDrawDefault);

	friend	class CListCtrlEx;
private:
	CDefaultListHandler(CListCtrlEx* a_pList);
	CDefaultListHandler(const CDefaultListHandler&rhs) : IListHandlerEx(rhs) {}
};

	}	// end of namespace UI
}	// end of namespace MFC

#endif // __LIST_HANDLER_EX__