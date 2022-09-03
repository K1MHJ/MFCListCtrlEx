
// ChildView.cpp: CChildView 클래스의 구현
//

#include "pch.h"
#include "framework.h"
#include "MFCListCtrlEx.h"
#include "ChildView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CChildView

CChildView::CChildView()
	: IListHandlerEx(&m_MainList)
{
}

CChildView::~CChildView()
{
}


BEGIN_MESSAGE_MAP(CChildView, CWnd)
	ON_WM_PAINT()
	ON_WM_CREATE()
	ON_WM_SIZE()
END_MESSAGE_MAP()



// CChildView 메시지 처리기

BOOL CChildView::PreCreateWindow(CREATESTRUCT& cs) 
{
	if (!CWnd::PreCreateWindow(cs))
		return FALSE;

	cs.dwExStyle |= WS_EX_CLIENTEDGE;
	cs.style &= ~WS_BORDER;
	cs.lpszClass = AfxRegisterWndClass(CS_HREDRAW|CS_VREDRAW|CS_DBLCLKS, 
		::LoadCursor(nullptr, IDC_ARROW), reinterpret_cast<HBRUSH>(COLOR_WINDOW+1), nullptr);

	return TRUE;
}

void CChildView::OnPaint() 
{
	CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.
	
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	
	// 그리기 메시지에 대해서는 CWnd::OnPaint()를 호출하지 마십시오.
}

void CChildView::InitMainUI()
{
	enum { LIST_HEIGHT = 40, LIST_HEADER_HEIGHT = 30, ID_MAINLIST = 10002 };

	CRect rcWnd;
	GetClientRect(&rcWnd);
	int nX = 10, nY = 40;
	m_MainList.SetListHeight(LIST_HEIGHT);
	// make virtual list
	m_MainList.Create(this, nX, nY, rcWnd.Width() - nX - 10, rcWnd.Height() - nY - 12, TRUE, TRUE, TRUE, ID_MAINLIST);
	m_MainList.SetHeaderHeight(LIST_HEADER_HEIGHT);
	m_MainList.SetFont(_T("굴림"), 12);
	m_MainList.AddColumnText(40, _T("Seq"));
	m_MainList.AddColumnText(100, _T("Name"), LVCFMT_LEFT);
	m_MainList.AddColumnText(200, _T("Title"), LVCFMT_LEFT);
	m_MainList.AddColumnText(100, _T("Author"), LVCFMT_LEFT);
	m_MainList.AddColumnText(30, _T("DEL"), LVCFMT_LEFT);
	m_MainList.SetDefaultMsg(_T("No Data."));
	//m_MainList.SetGrid(TRUE, RGB(165,165,165));
	m_MainList.SetUnderLine(TRUE, RGB(165, 165, 165));
	// set handler.
	m_MainList.SetHandler(this);
	//////////////////////////////////////////////////////////////////////////
	//m_ImgList.Create(18, 18, ILC_COLOR32 | ILC_MASK, 1, 1);
	//CBitmap bmIcon;
	//bmIcon.Attach(LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDB_CLOSE)));
	//m_ImgList.Add(&bmIcon, RGB(255, 255, 255));

}


//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

/// 
/// \brief <pre>
/// Draw Item BG. 
/// </pre>
/// \param   a_pDC
/// \param   a_nItemIdx
/// \param   a_bHighlight
/// \param   a_bFocus
/// \param   a_rcBounds
/// \param   a_bDrawDefault - true is draw CDefaultHandler
/// 
void CChildView::DrawItemBG(CDC* a_pDC, int a_nItemIdx, BOOL a_bHighlight, BOOL a_bFocus, const CRect& a_rcBounds, bool& a_bDrawDefault)
{
	a_bDrawDefault = true;
	if (a_bHighlight) {
		a_pDC->FillRect(a_rcBounds, &CBrush(a_bFocus ? m_pList->GetHilightBGColor() : RGB(192, 192, 192)));
		a_bDrawDefault = false;
	}
	if (a_bHighlight) return;
}
/// 
/// \brief <pre>
/// Draw State Icon Image.
/// </pre>
/// \param   a_pDC
/// \param   a_nItemIdx
/// \param   a_bHighlight
/// \param   a_bFocus
/// \param   a_rcCol
/// \param   a_bDrawDefault
/// 
void CChildView::DrawStateIconImage(CDC* a_pDC, int a_nItemIdx, BOOL a_bHighlight, BOOL a_bFocus, const CRect& a_rcCol, bool& a_bDrawDefault)
{
	a_bDrawDefault = true;
}
/// 
/// \brief <pre>
/// Draw Small Icon Image
/// </pre>
/// \param   a_pDC
/// \param   a_nItemIdx
/// \param   a_bHighlight
/// \param   a_bFocus
/// \param   a_rcIcon
/// \param   a_bDrawDefault
/// 
void CChildView::DrawSmallIconImage(CDC* a_pDC, int a_nItemIdx, BOOL a_bHighlight, BOOL a_bFocus, const CRect& a_rcIcon, bool& a_bDrawDefault)
{
	a_bDrawDefault = true;
}
/// 
/// \brief <pre>
/// Draw Sub Item. 
/// </pre>
/// \param   a_pDC
/// \param   a_nItemIdx
/// \param   a_nSubItemIdx
/// \param   a_bHighlight
/// \param   a_bFocus
/// \param   a_rcLabel
/// \param   a_bDrawDefault
/// 
void CChildView::DrawSubItem(CDC* a_pDC, int a_nItemIdx, int a_nSubItemIdx, BOOL a_bHighlight, BOOL a_bFocus, const CRect& a_rcLabel, bool& a_bDrawDefault)
{
	a_bDrawDefault = false;
	CString sLabel = m_pList->GetItemText(a_nItemIdx, a_nSubItemIdx);
	//////////////////////////////////////////////////////////////////////////
	if (a_bHighlight) a_pDC->SetTextColor(RGB(255, 255, 255));
	//////////////////////////////////////////////////////////////////////////
	a_pDC->SetBkMode(TRANSPARENT);
	if (a_nSubItemIdx == 4) {
		CPoint ptDraw(a_rcLabel.left + (a_rcLabel.Width() / 2 - 18 / 2), a_rcLabel.top + (a_rcLabel.Height() / 2 - 18 / 2));
		m_ImgList.Draw(a_pDC, 0, ptDraw, ILD_NORMAL);
	}
	else {
		UINT nJustify = m_pList->GetColumnFMT(a_nSubItemIdx);
		CRect	rcLabel(a_rcLabel);
		rcLabel.left += 4;
		a_pDC->DrawText(sLabel, -1, rcLabel, nJustify | DT_SINGLELINE | DT_END_ELLIPSIS | DT_VCENTER);
	}
}
/// 
/// \brief
/// if listcontrol have OWNER_DATA, set text.
/// \param   *pNMHDR
/// \param   *pResult
/// 
void CChildView::OnLvnGetdispinfo(NMHDR* pNMHDR, LRESULT* pResult)
{
	NMLVDISPINFO* pDispInfo = reinterpret_cast<NMLVDISPINFO*>(pNMHDR);
	LV_ITEM* pItem = &(pDispInfo)->item;
	switch (pItem->iSubItem)
	{
	case 0:
		lstrcpyn(pItem->pszText, m_vList[pItem->iItem].m_strDate, pItem->cchTextMax);
		break;
	case 1:
		lstrcpyn(pItem->pszText, m_vList[pItem->iItem].m_strName, pItem->cchTextMax);
		break;
	case 2:
		lstrcpyn(pItem->pszText, m_vList[pItem->iItem].m_strTitle, pItem->cchTextMax);
		break;
	case 3:
		lstrcpyn(pItem->pszText, m_vList[pItem->iItem].m_strAuthor, pItem->cchTextMax);
		break;
	}
}
/// 
/// \brief
/// 내용을 넣으세요.
/// \param   a_nItem
/// \param   a_nSubItem
/// 
void CChildView::OnRbuttonClick(int a_nItem, int a_nSubItem)
{
}
/// 
/// \brief
/// Item click event.
/// \param   a_nItem
/// \param   a_nSubItem
/// 
void CChildView::OnItemClick(int a_nItem, int a_nSubItem)
{
	if (a_nItem == -1) return;
	if (a_nSubItem == 4) {
		if (::MessageBox(m_hWnd, _T("Are you want delete?"), _T("Info"), MB_YESNO | MB_ICONINFORMATION) == IDNO) return;
		// do it delete.
	}
}
/// 
/// \brief
/// item db click event
/// \param   a_nItem
/// \param   a_nSubItem
/// 
void CChildView::OnItemDBClick(int a_nItem, int a_nSubItem)
{
	if (a_nItem == -1) return;
}



int CChildView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (__super::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  여기에 특수화된 작성 코드를 추가합니다.
	InitMainUI();

	return 0;
}


void CChildView::OnSize(UINT nType, int cx, int cy)
{
	__super::OnSize(nType, cx, cy);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	CRect rc;
	GetWindowRect(&rc);
	ScreenToClient(&rc);
	m_MainList.MoveWindow(rc);
}
