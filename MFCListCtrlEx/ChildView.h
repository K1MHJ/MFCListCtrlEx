
// ChildView.h: CChildView 클래스의 인터페이스
//
#pragma once
#include "ListCtrlEx.h"
#include <vector>
// CChildView 창

class CChildView : public CWnd, public MFC::UI::IListHandlerEx
{
// 생성입니다.
public:
	CChildView();


	typedef struct _member_info {
		CString	m_strDate,
			m_strName,
			m_strTitle,
			m_strAuthor;
	} MEMBER_INFO;
private:
	MFC::UI::CListCtrlEx		m_MainList;
	std::vector<MEMBER_INFO>	m_vList;
	CImageList					m_ImgList;

// 특성입니다.
public:

// 작업입니다.
public:

// 재정의입니다.
protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
private:
	/// IListHandler Interface
	void		OnRbuttonClick(int a_nItem, int a_nSubItem);
	void		OnItemClick(int a_nItem, int a_nSubItem);
	void		DrawItemBG(CDC* a_pDC, int a_nItemIdx, BOOL a_bHighlight, BOOL a_bFocus, const CRect& a_rcBounds, bool& a_bDrawDefault);
	void		DrawStateIconImage(CDC* a_pDC, int a_nItemIdx, BOOL a_bHighlight, BOOL a_bFocus, const CRect& a_rcCol, bool& a_bDrawDefault);
	void		DrawSmallIconImage(CDC* a_pDC, int a_nItemIdx, BOOL a_bHighlight, BOOL a_bFocus, const CRect& a_rcIcon, bool& a_bDrawDefault);
	void		DrawSubItem(CDC* a_pDC, int a_nItemIdx, int a_nSubItemIdx, BOOL a_bHighlight, BOOL a_bFocus, const CRect& a_rcLabel, bool& a_bDrawDefault);
	void		OnLvnGetdispinfo(NMHDR* pNMHDR, LRESULT* pResult);
	void		OnItemDBClick(int a_nItem, int a_nSubItem);

// 구현입니다.
public:
	virtual ~CChildView();
	void InitMainUI();
	// 생성된 메시지 맵 함수
protected:
	afx_msg void OnPaint();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
};

