
#include "StdAfx.h"
#include "UIComonControls.h"
/////////////////////////////////////////////////////////////////////////////////////
//
//
namespace UILIB_LIB
{
CLabelUI::CLabelUI() : m_uTextStyle(DT_VCENTER), m_dwTextColor(0), 
    m_dwDisabledTextColor(0), m_iFont(-1), m_bShowHtml(false)
{
    ::ZeroMemory(&m_rcTextPadding, sizeof(m_rcTextPadding));
}

LPCTSTR CLabelUI::GetClass() const
{
    return _T("LabelUI");
}

LPVOID CLabelUI::GetInterface(LPCTSTR pstrName)
{
    if( _tcscmp(pstrName, _T("Label")) == 0 ) return static_cast<CLabelUI*>(this);
    return CControlUI::GetInterface(pstrName);
}

UINT CLabelUI::GetTextStyle()
{
	return m_uTextStyle;
}

void CLabelUI::SetTextStyle(UINT uStyle)
{
    m_uTextStyle = uStyle;
    Invalidate();
}

void CLabelUI::SetTextColor(DWORD dwTextColor)
{
    m_dwTextColor = dwTextColor;
}

void CLabelUI::SetDisabledTextColor(DWORD dwTextColor)
{
    m_dwDisabledTextColor = dwTextColor;
}

void CLabelUI::SetFont(int index)
{
    m_iFont = index;
}

RECT CLabelUI::GetTextPadding() const
{
    return m_rcTextPadding;
}

void CLabelUI::SetTextPadding(RECT rc)
{
    m_rcTextPadding = rc;
    Invalidate();
}

bool CLabelUI::IsShowHtml()
{
    return m_bShowHtml;
}

void CLabelUI::SetShowHtml(bool bShowHtml)
{
    if( m_bShowHtml == bShowHtml ) return;

    m_bShowHtml = bShowHtml;
    Invalidate();
}

SIZE CLabelUI::EstimateSize(SIZE szAvailable)
{
    if( m_cxyFixed.cy == 0 ) return CSize(m_cxyFixed.cx, m_pManager->GetDefaultFontInfo().tmHeight + 4);
    return CControlUI::EstimateSize(szAvailable);
}

void CLabelUI::Event(TEventUI& event)
{
    if( event.Type == UIEVENT_SETFOCUS ) 
    {
        m_bFocused = true;
        return;
    }
    if( event.Type == UIEVENT_KILLFOCUS ) 
    {
        m_bFocused = false;
        return;
    }
    if( event.Type == UIEVENT_MOUSEENTER )
    {
        return;
    }
    if( event.Type == UIEVENT_MOUSELEAVE )
    {
        return;
    }
    CControlUI::Event(event);
}

void CLabelUI::SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue)
{
    if( _tcscmp(pstrName, _T("align")) == 0 ) {
        if( _tcsstr(pstrValue, _T("left")) != NULL ) {
            m_uTextStyle &= ~(DT_CENTER | DT_RIGHT);
            m_uTextStyle |= DT_LEFT;
        }
        if( _tcsstr(pstrValue, _T("center")) != NULL ) {
            m_uTextStyle &= ~(DT_LEFT | DT_RIGHT);
            m_uTextStyle |= DT_CENTER;
        }
        if( _tcsstr(pstrValue, _T("right")) != NULL ) {
            m_uTextStyle &= ~(DT_LEFT | DT_CENTER);
            m_uTextStyle |= DT_RIGHT;
        }
		if( _tcsstr(pstrValue, _T("top")) != NULL ) {
			m_uTextStyle &= ~(DT_BOTTOM | DT_VCENTER);
			m_uTextStyle |= DT_TOP;
		}
		if( _tcsstr(pstrValue, _T("bottom")) != NULL ) {
			m_uTextStyle &= ~(DT_TOP | DT_VCENTER);
			m_uTextStyle |= DT_BOTTOM;
		}
    }
    else if( _tcscmp(pstrName, _T("font")) == 0 ) SetFont(_ttoi(pstrValue));
    else if( _tcscmp(pstrName, _T("textcolor")) == 0 ) {
        if( *pstrValue == _T('#')) pstrValue = ::CharNext(pstrValue);
        LPTSTR pstr = NULL;
        DWORD clrColor = _tcstoul(pstrValue, &pstr, 16);
        SetTextColor(clrColor);
    }
    else if( _tcscmp(pstrName, _T("disabledtextcolor")) == 0 ) {
        if( *pstrValue == _T('#')) pstrValue = ::CharNext(pstrValue);
        LPTSTR pstr = NULL;
        DWORD clrColor = _tcstoul(pstrValue, &pstr, 16);
        SetDisabledTextColor(clrColor);
    }
    else if( _tcscmp(pstrName, _T("textpadding")) == 0 ) {
        RECT rcTextPadding = { 0 };
        LPTSTR pstr = NULL;
        rcTextPadding.left = _tcstol(pstrValue, &pstr, 10);  ASSERT(pstr);    
        rcTextPadding.top = _tcstol(pstr + 1, &pstr, 10);    ASSERT(pstr);    
        rcTextPadding.right = _tcstol(pstr + 1, &pstr, 10);  ASSERT(pstr);    
        rcTextPadding.bottom = _tcstol(pstr + 1, &pstr, 10); ASSERT(pstr);    
        SetTextPadding(rcTextPadding);
    }
    else if( _tcscmp(pstrName, _T("showhtml")) == 0 ) SetShowHtml(_tcscmp(pstrValue, _T("true")) == 0);
    else CControlUI::SetAttribute(pstrName, pstrValue);
}

void CLabelUI::PaintText(HDC hDC)
{
    if( m_dwTextColor == 0 ) m_dwTextColor = m_pManager->GetDefaultFontColor();
    if( m_dwDisabledTextColor == 0 ) m_dwDisabledTextColor = m_pManager->GetDefaultDisabledColor();

    if( m_sText.IsEmpty() ) return;
    int nLinks = 0;
    RECT rc = m_rcItem;
    rc.left += m_rcTextPadding.left;
    rc.right -= m_rcTextPadding.right;
    rc.top += m_rcTextPadding.top;
    rc.bottom -= m_rcTextPadding.bottom;
    if( IsEnabled() ) {
        if( m_bShowHtml )
            CRenderEngine::DrawHtmlText(hDC, m_pManager, rc, m_sText, m_dwTextColor, \
            NULL, NULL, nLinks, DT_SINGLELINE | m_uTextStyle);
        else
            CRenderEngine::DrawText(hDC, m_pManager, rc, m_sText, m_dwTextColor, \
            m_iFont, DT_SINGLELINE | m_uTextStyle);
    }
    else {
        if( m_bShowHtml )
            CRenderEngine::DrawHtmlText(hDC, m_pManager, rc, m_sText, m_dwDisabledTextColor, \
            NULL, NULL, nLinks, DT_SINGLELINE | m_uTextStyle);
        else
            CRenderEngine::DrawText(hDC, m_pManager, rc, m_sText, m_dwDisabledTextColor, \
            m_iFont, DT_SINGLELINE | m_uTextStyle);
    }
}

/////////////////////////////////////////////////////////////////////////////////////
//
//

CButtonUI::CButtonUI() : m_uButtonState(0), m_dwHotTextColor(0), m_dwPushedTextColor(0), m_dwFocusedTextColor(0)
{
    m_uTextStyle = DT_SINGLELINE | DT_VCENTER | DT_CENTER;
}

LPCTSTR CButtonUI::GetClass() const
{
    return _T("ButtonUI");
}

LPVOID CButtonUI::GetInterface(LPCTSTR pstrName)
{
    if( _tcscmp(pstrName, _T("Button")) == 0 ) return static_cast<CButtonUI*>(this);
    return CLabelUI::GetInterface(pstrName);
}

UINT CButtonUI::GetControlFlags() const
{
    return UIFLAG_TABSTOP | (IsEnabled() ? UIFLAG_SETCURSOR : 0);
}

void CButtonUI::Event(TEventUI& event)
{
    if( !IsMouseEnabled() && event.Type > UIEVENT__MOUSEBEGIN && event.Type < UIEVENT__MOUSEEND ) {
        if( m_pParent != NULL ) m_pParent->Event(event);
        else CLabelUI::Event(event);
        return;
    }

    if( event.Type == UIEVENT_SETFOCUS ) 
    {
        Invalidate();
    }
    if( event.Type == UIEVENT_KILLFOCUS ) 
    {
        Invalidate();
    }
    if( event.Type == UIEVENT_BUTTONDOWN || event.Type == UIEVENT_DBLCLICK )
    {
        if( ::PtInRect(&m_rcItem, event.ptMouse) && IsEnabled() ) {
            m_uButtonState |= UISTATE_PUSHED | UISTATE_CAPTURED;
            Invalidate();
        }
        return;
    }
    if( event.Type == UIEVENT_MOUSEMOVE )
    {
        if( (m_uButtonState & UISTATE_CAPTURED) != 0 ) {
            if( ::PtInRect(&m_rcItem, event.ptMouse) ) m_uButtonState |= UISTATE_PUSHED;
            else m_uButtonState &= ~UISTATE_PUSHED;
            Invalidate();
        }
        return;
    }
    if( event.Type == UIEVENT_BUTTONUP )
    {
        if( (m_uButtonState & UISTATE_CAPTURED) != 0 ) {
            if( ::PtInRect(&m_rcItem, event.ptMouse) ) Activate();
            m_uButtonState &= ~(UISTATE_PUSHED | UISTATE_CAPTURED);
            Invalidate();
        }
        return;
    }
    if( event.Type == UIEVENT_MOUSEENTER )
    {
        if( IsEnabled() ) {
            m_uButtonState |= UISTATE_HOT;
            Invalidate();
        }
        return;
    }
    if( event.Type == UIEVENT_MOUSELEAVE )
    {
        if( IsEnabled() ) {
            m_uButtonState &= ~UISTATE_HOT;
            Invalidate();
        }
        return;
    }
    if( event.Type == UIEVENT_SETCURSOR ) {
        ::SetCursor(::LoadCursor(NULL, MAKEINTRESOURCE(IDC_ARROW)));
        return;
    }
    CLabelUI::Event(event);
}

bool CButtonUI::Activate()
{
    if( !CControlUI::Activate() ) return false;
    if( m_pManager != NULL ) m_pManager->SendNotify(this, MST_CLICK);
    return true;
}

void CButtonUI::SetEnabled(bool bEnable)
{
    CControlUI::SetEnabled(bEnable);
    if( !IsEnabled() ) {
        m_uButtonState = 0;
    }
}

void CButtonUI::SetHotTextColor(DWORD dwColor)
{
	m_dwHotTextColor = dwColor;
}

DWORD CButtonUI::GetHotTextColor() const
{
	return m_dwHotTextColor;
}

void CButtonUI::SetPushedTextColor(DWORD dwColor)
{
	m_dwPushedTextColor = dwColor;
}

DWORD CButtonUI::GetPushedTextColor() const
{
	return m_dwPushedTextColor;
}

void CButtonUI::SetFocusedTextColor(DWORD dwColor)
{
	m_dwFocusedTextColor = dwColor;
}

DWORD CButtonUI::GetFocusedTextColor() const
{
	return m_dwFocusedTextColor;
}


LPCTSTR CButtonUI::GetNormalImage()
{
    return m_sNormalImage;
}

void CButtonUI::SetNormalImage(LPCTSTR pStrImage)
{
    m_sNormalImage = pStrImage;
    Invalidate();
}

LPCTSTR CButtonUI::GetHotImage()
{
    return m_sHotImage;
}

void CButtonUI::SetHotImage(LPCTSTR pStrImage)
{
    m_sHotImage = pStrImage;
    Invalidate();
}

LPCTSTR CButtonUI::GetPushedImage()
{
    return m_sPushedImage;
}

void CButtonUI::SetPushedImage(LPCTSTR pStrImage)
{
    m_sPushedImage = pStrImage;
    Invalidate();
}

LPCTSTR CButtonUI::GetFocusedImage()
{
    return m_sFocusedImage;
}

void CButtonUI::SetFocusedImage(LPCTSTR pStrImage)
{
    m_sFocusedImage = pStrImage;
    Invalidate();
}

LPCTSTR CButtonUI::GetDisabledImage()
{
    return m_sDisabledImage;
}

void CButtonUI::SetDisabledImage(LPCTSTR pStrImage)
{
    m_sDisabledImage = pStrImage;
    Invalidate();
}

SIZE CButtonUI::EstimateSize(SIZE szAvailable)
{
    if( m_cxyFixed.cy == 0 ) return CSize(m_cxyFixed.cx, m_pManager->GetDefaultFontInfo().tmHeight + 8);
    return CControlUI::EstimateSize(szAvailable);
}

void CButtonUI::SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue)
{
    if( _tcscmp(pstrName, _T("normalimage")) == 0 ) SetNormalImage(pstrValue);
    else if( _tcscmp(pstrName, _T("hotimage")) == 0 ) SetHotImage(pstrValue);
    else if( _tcscmp(pstrName, _T("pushedimage")) == 0 ) SetPushedImage(pstrValue);
    else if( _tcscmp(pstrName, _T("focusedimage")) == 0 ) SetFocusedImage(pstrValue);
    else if( _tcscmp(pstrName, _T("disabledimage")) == 0 ) SetDisabledImage(pstrValue);
	else if( _tcscmp(pstrName, _T("hottextcolor")) == 0 ) {
		if( *pstrValue == _T('#')) pstrValue = ::CharNext(pstrValue);
		LPTSTR pstr = NULL;
		DWORD clrColor = _tcstoul(pstrValue, &pstr, 16);
		SetHotTextColor(clrColor);
	}
	else if( _tcscmp(pstrName, _T("pushedtextcolor")) == 0 ) {
		if( *pstrValue == _T('#')) pstrValue = ::CharNext(pstrValue);
		LPTSTR pstr = NULL;
		DWORD clrColor = _tcstoul(pstrValue, &pstr, 16);
		SetPushedTextColor(clrColor);
	}
	else if( _tcscmp(pstrName, _T("focusedtextcolor")) == 0 ) {
		if( *pstrValue == _T('#')) pstrValue = ::CharNext(pstrValue);
		LPTSTR pstr = NULL;
		DWORD clrColor = _tcstoul(pstrValue, &pstr, 16);
		SetFocusedTextColor(clrColor);
	}
    else CLabelUI::SetAttribute(pstrName, pstrValue);
}

void CButtonUI::PaintText(HDC hDC)
{
	if( IsFocused() ) m_uButtonState |= UISTATE_FOCUSED;
	else m_uButtonState &= ~ UISTATE_FOCUSED;
	if( !IsEnabled() ) m_uButtonState |= UISTATE_DISABLED;
	else m_uButtonState &= ~ UISTATE_DISABLED;

	if( m_dwTextColor == 0 ) m_dwTextColor = m_pManager->GetDefaultFontColor();
	if( m_dwDisabledTextColor == 0 ) m_dwDisabledTextColor = m_pManager->GetDefaultDisabledColor();

	if( m_sText.IsEmpty() ) return;
	int nLinks = 0;
	RECT rc = m_rcItem;
	rc.left += m_rcTextPadding.left;
	rc.right -= m_rcTextPadding.right;
	rc.top += m_rcTextPadding.top;
	rc.bottom -= m_rcTextPadding.bottom;

	DWORD clrColor = IsEnabled()?m_dwTextColor:m_dwDisabledTextColor;

	if( ((m_uButtonState & UISTATE_PUSHED) != 0) && (GetPushedTextColor() != 0) )
		clrColor = GetPushedTextColor();
	else if( ((m_uButtonState & UISTATE_HOT) != 0) && (GetHotTextColor() != 0) )
		clrColor = GetHotTextColor();
	else if( ((m_uButtonState & UISTATE_FOCUSED) != 0) && (GetFocusedTextColor() != 0) )
		clrColor = GetFocusedTextColor();

	if( m_bShowHtml )
		CRenderEngine::DrawHtmlText(hDC, m_pManager, rc, m_sText, clrColor, \
		NULL, NULL, nLinks, m_uTextStyle);
	else
		CRenderEngine::DrawText(hDC, m_pManager, rc, m_sText, clrColor, \
		m_iFont, m_uTextStyle);
}


void CButtonUI::PaintStatusImage(HDC hDC)
{
    if( IsFocused() ) m_uButtonState |= UISTATE_FOCUSED;
    else m_uButtonState &= ~ UISTATE_FOCUSED;
    if( !IsEnabled() ) m_uButtonState |= UISTATE_DISABLED;
    else m_uButtonState &= ~ UISTATE_DISABLED;

    if( (m_uButtonState & UISTATE_DISABLED) != 0 ) {
        if( !m_sDisabledImage.IsEmpty() ) {
            if( !DrawImage(hDC, (LPCTSTR)m_sDisabledImage) ) m_sDisabledImage.Empty();
            else return;
        }
    }
    else if( (m_uButtonState & UISTATE_PUSHED) != 0 ) {
        if( !m_sPushedImage.IsEmpty() ) {
            if( !DrawImage(hDC, (LPCTSTR)m_sPushedImage) ) m_sPushedImage.Empty();
            else return;
        }
    }
    else if( (m_uButtonState & UISTATE_HOT) != 0 ) {
        if( !m_sHotImage.IsEmpty() ) {
            if( !DrawImage(hDC, (LPCTSTR)m_sHotImage) ) m_sHotImage.Empty();
            else return;
        }
    }
    else if( (m_uButtonState & UISTATE_FOCUSED) != 0 ) {
        if( !m_sFocusedImage.IsEmpty() ) {
            if( !DrawImage(hDC, (LPCTSTR)m_sFocusedImage) ) m_sFocusedImage.Empty();
            else return;
        }
    }

	if( !m_sNormalImage.IsEmpty() ) {
		if( !DrawImage(hDC, (LPCTSTR)m_sNormalImage) ) m_sNormalImage.Empty();
	}

    //DWORD dwBorderColor = 0xFF4EA0D1;
    //int nBorderSize = 1;
    //if( (m_uButtonState & UISTATE_HOT) != 0 ) {
    //    dwBorderColor = 0xFF85E4FF;
    //    nBorderSize = 2;
    //}
    //CRenderEngine::DrawRect(hDC, m_rcItem, nBorderSize, dwBorderColor);
}


/////////////////////////////////////////////////////////////////////////////////////
//
//

COptionUI::COptionUI() : m_bGroup(false), m_bSelected(false), m_dwSelectedTextColor(0),m_dwSelectedBKColor(0)
{
}

LPCTSTR COptionUI::GetClass() const
{
    return _T("OptionUI");
}

LPVOID COptionUI::GetInterface(LPCTSTR pstrName)
{
    if( _tcscmp(pstrName, _T("Option")) == 0 ) return static_cast<COptionUI*>(this);
    return CButtonUI::GetInterface(pstrName);
}

bool COptionUI::IsGroup() const
{
    return m_bGroup;
}

void COptionUI::SetGroup(bool bGroup)
{
    m_bGroup = bGroup;
//     if(m_bGroup && m_bSelected) {
//         m_bSelected = false;
//         Invalidate();
//     }
}

bool COptionUI::IsSelected() const
{
    return m_bSelected;
}

void COptionUI::Selected(bool bSelected)
{
    if( m_bSelected == bSelected ) return;
    m_bSelected = bSelected;
    if( m_bSelected ) m_uButtonState |= UISTATE_SELECTED;
    else m_uButtonState &= ~UISTATE_SELECTED;
 
    if(m_bGroup) {
        if(m_bSelected)  {
            if( GetParent() ) {
                IContainerUI* pParent = (IContainerUI*)(GetParent()->GetInterface(_T("IContainer")));
                if(pParent) {
                    for( int it = 0; it < pParent->GetCount(); it++ ) {
                        COptionUI* pControl = static_cast<COptionUI*>(pParent->GetItemAt(it)->GetInterface(_T("Option")));
                        if( pControl && pControl != this && pControl->IsGroup() && pControl->IsVisible() && pControl->IsEnabled() ) {
                            pControl->Selected(false);
                        }
                    }
                }
            }

            if( m_pManager != NULL ) m_pManager->SendNotify(this, MST_SELECTCHANGED);
        }
    }
    else {
        if( m_pManager != NULL ) m_pManager->SendNotify(this, MST_SELECTCHANGED);
    }

    Invalidate();
}

bool COptionUI::Activate()
{
    if( !CControlUI::Activate() ) return false;
    if( m_bGroup ) Selected(true);
    else Selected(!m_bSelected);

    return true;
}

LPCTSTR COptionUI::GetSelectedImage()
{
    return m_sSelectedImage;
}

void COptionUI::SetSelectedImage(LPCTSTR pStrImage)
{
    m_sSelectedImage = pStrImage;
    Invalidate();
}

LPCTSTR COptionUI::GetForeImage()
{
	return m_sForeImage;
}

void COptionUI::SetForeImage(LPCTSTR pStrImage)
{
	m_sForeImage=pStrImage;
	Invalidate();
}

void COptionUI::SetSelectedTextColor(DWORD dwTextColor)
{
	m_dwSelectedTextColor = dwTextColor;
}

SIZE COptionUI::EstimateSize(SIZE szAvailable)
{
    if( m_cxyFixed.cy == 0 ) return CSize(m_cxyFixed.cx, m_pManager->GetDefaultFontInfo().tmHeight + 8);
    return CControlUI::EstimateSize(szAvailable);
}

void COptionUI::SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue)
{
    if( _tcscmp(pstrName, _T("group")) == 0 ) SetGroup(_tcscmp(pstrValue, _T("true")) == 0);
    else if( _tcscmp(pstrName, _T("selected")) == 0 ) Selected(_tcscmp(pstrValue, _T("true")) == 0);
    else if( _tcscmp(pstrName, _T("selectedimage")) == 0 ) SetSelectedImage(pstrValue);
	else if( _tcscmp(pstrName, _T("foreimage")) == 0 ) SetForeImage(pstrValue);
	else if( _tcscmp(pstrName, _T("selectedtextcolor")) == 0 ) {
		if( *pstrValue == _T('#')) pstrValue = ::CharNext(pstrValue);
		LPTSTR pstr = NULL;
		DWORD clrColor = _tcstoul(pstrValue, &pstr, 16);
		SetSelectedTextColor(clrColor);
	}
	else if( _tcscmp(pstrName, _T("selectedbkcolor")) == 0 ) {
		if( *pstrValue == _T('#')) pstrValue = ::CharNext(pstrValue);
		LPTSTR pstr = NULL;
		DWORD clrColor = _tcstoul(pstrValue, &pstr, 16);
		SetSelectBkColor(clrColor);
	}
    else CButtonUI::SetAttribute(pstrName, pstrValue);
}

void COptionUI::PaintStatusImage(HDC hDC)
{
//     m_uButtonState &= ~UISTATE_PUSHED;

    if( (m_uButtonState & UISTATE_SELECTED) != 0 ) {
		m_uButtonState &= ~UISTATE_HOT;
        if( !m_sSelectedImage.IsEmpty() ) {
            if( !DrawImage(hDC, (LPCTSTR)m_sSelectedImage) ) m_sSelectedImage.Empty();
            else goto Label_ForeImage;
        }
    }

    CButtonUI::PaintStatusImage(hDC);

Label_ForeImage:
	if( !m_sForeImage.IsEmpty() ) {
		if( !DrawImage(hDC, (LPCTSTR)m_sForeImage) ) m_sForeImage.Empty();
	}
}

void COptionUI::PaintText(HDC hDC)
{
	if( (m_uButtonState & UISTATE_SELECTED) != 0 )
	{
		DWORD oldTextColor = m_dwTextColor;
		if( m_dwSelectedTextColor != 0 ) m_dwTextColor = m_dwSelectedTextColor;

		if( m_dwTextColor == 0 ) m_dwTextColor = m_pManager->GetDefaultFontColor();
		if( m_dwDisabledTextColor == 0 ) m_dwDisabledTextColor = m_pManager->GetDefaultDisabledColor();

		if( m_sText.IsEmpty() ) return;
		int nLinks = 0;
		RECT rc = m_rcItem;
		rc.left += m_rcTextPadding.left;
		rc.right -= m_rcTextPadding.right;
		rc.top += m_rcTextPadding.top;
		rc.bottom -= m_rcTextPadding.bottom;

		if( m_bShowHtml )
			CRenderEngine::DrawHtmlText(hDC, m_pManager, rc, m_sText, IsEnabled()?m_dwTextColor:m_dwDisabledTextColor, \
			NULL, NULL, nLinks, m_uTextStyle);
		else
			CRenderEngine::DrawText(hDC, m_pManager, rc, m_sText, IsEnabled()?m_dwTextColor:m_dwDisabledTextColor, \
			m_iFont, m_uTextStyle);

		m_dwTextColor = oldTextColor;
	}
	else
		CButtonUI::PaintText(hDC);
}

void COptionUI::SetSelectBkColor(DWORD dwColor)
{
	m_dwSelectedBKColor = dwColor;
}

void COptionUI::PaintBkColor(HDC hDC)
{
	if((m_dwSelectedBKColor != 0) && m_bSelected == true)
		CRenderEngine::DrawColor(hDC, m_rcItem, m_dwSelectedBKColor);
	else if( m_dwBackColor != 0 ) {
		if( m_dwBackColor2 != 0 ) CRenderEngine::DrawGradient(hDC, m_rcItem, m_dwBackColor, m_dwBackColor2, true, 16);
		else if( m_dwBackColor >= 0xFF000000 ) CRenderEngine::DrawColor(hDC, m_rcPaint, m_dwBackColor);
		else CRenderEngine::DrawColor(hDC, m_rcItem, m_dwBackColor);
	}
}

/////////////////////////////////////////////////////////////////////////////////////
//
//

CTextUI::CTextUI() : m_nLinks(0), m_nHoverLink(-1)
{
    m_uTextStyle = DT_WORDBREAK;
    m_rcTextPadding.left = 2;
    m_rcTextPadding.right = 2;
    ::ZeroMemory(m_rcLinks, sizeof(m_rcLinks));
}

CTextUI::~CTextUI()
{
}

LPCTSTR CTextUI::GetClass() const
{
    return _T("TextUI");
}

LPVOID CTextUI::GetInterface(LPCTSTR pstrName)
{
    if( _tcscmp(pstrName, _T("Text")) == 0 ) return static_cast<CTextUI*>(this);
    return CLabelUI::GetInterface(pstrName);
}

UINT CTextUI::GetControlFlags() const
{
    if( IsEnabled() && m_nLinks > 0 ) return UIFLAG_SETCURSOR;
    else return 0;
}

CStdString* CTextUI::GetLinkContent(int iIndex)
{
    if( iIndex >= 0 && iIndex < m_nLinks ) return &m_sLinks[iIndex];
    return NULL;
}

void CTextUI::Event(TEventUI& event)
{
    if( !IsMouseEnabled() && event.Type > UIEVENT__MOUSEBEGIN && event.Type < UIEVENT__MOUSEEND ) {
        if( m_pParent != NULL ) m_pParent->Event(event);
        else CLabelUI::Event(event);
        return;
    }

    if( event.Type == UIEVENT_SETCURSOR ) {
        for( int i = 0; i < m_nLinks; i++ ) {
            if( ::PtInRect(&m_rcLinks[i], event.ptMouse) ) {
                ::SetCursor(::LoadCursor(NULL, MAKEINTRESOURCE(IDC_HAND)));
                return;
            }
        }
    }
    if( event.Type == UIEVENT_BUTTONDOWN || event.Type == UIEVENT_DBLCLICK && IsEnabled() ) {
        for( int i = 0; i < m_nLinks; i++ ) {
            if( ::PtInRect(&m_rcLinks[i], event.ptMouse) ) {
                Invalidate();
                return;
            }
        }
    }
    if( event.Type == UIEVENT_BUTTONUP && IsEnabled() ) {
        for( int i = 0; i < m_nLinks; i++ ) {
            if( ::PtInRect(&m_rcLinks[i], event.ptMouse) ) {
                m_pManager->SendNotify(this, MST_LINK, i);
                return;
            }
        }
    }
    // When you move over a link
    if( m_nLinks > 0 && event.Type == UIEVENT_MOUSEMOVE && IsEnabled() ) {
        int nHoverLink = -1;
        for( int i = 0; i < m_nLinks; i++ ) {
            if( ::PtInRect(&m_rcLinks[i], event.ptMouse) ) {
                nHoverLink = i;
                break;
            }
        }

        if(m_nHoverLink != nHoverLink) {
            m_nHoverLink = nHoverLink;
            Invalidate();
            return;
        }      
    }
    if( event.Type == UIEVENT_MOUSELEAVE ) {
        if( m_nLinks > 0 && IsEnabled() ) {
            if(m_nHoverLink != -1) {
                m_nHoverLink = -1;
                Invalidate();
                return;
            }
        }
    }

    CLabelUI::Event(event);
}

SIZE CTextUI::EstimateSize(SIZE szAvailable)
{
    RECT rcText = { 0, 0, MAX(szAvailable.cx, m_cxyFixed.cx), 9999 };
    rcText.left += m_rcTextPadding.left;
    rcText.right -= m_rcTextPadding.right;
    if( m_bShowHtml ) {   
        int nLinks = 0;
        CRenderEngine::DrawHtmlText(m_pManager->GetPaintDC(), m_pManager, rcText, m_sText, m_dwTextColor, NULL, NULL, nLinks, DT_CALCRECT | m_uTextStyle);
    }
    else {
        CRenderEngine::DrawText(m_pManager->GetPaintDC(), m_pManager, rcText, m_sText, m_dwTextColor, m_iFont, DT_CALCRECT | m_uTextStyle);
    }
    SIZE cXY = {rcText.right - rcText.left + m_rcTextPadding.left + m_rcTextPadding.right,
        rcText.bottom - rcText.top + m_rcTextPadding.top + m_rcTextPadding.bottom};

    if( m_cxyFixed.cy != 0 ) cXY.cy = m_cxyFixed.cy;
    return cXY;
}

void CTextUI::PaintText(HDC hDC)
{
    if( m_sText.IsEmpty() ) {
        m_nLinks = 0;
        return;
    }

    if( m_dwTextColor == 0 ) m_dwTextColor = m_pManager->GetDefaultFontColor();
    if( m_dwDisabledTextColor == 0 ) m_dwDisabledTextColor = m_pManager->GetDefaultDisabledColor();

    if( m_sText.IsEmpty() ) return;

    m_nLinks = lengthof(m_rcLinks);
    RECT rc = m_rcItem;
    rc.left += m_rcTextPadding.left;
    rc.right -= m_rcTextPadding.right;
    rc.top += m_rcTextPadding.top;
    rc.bottom -= m_rcTextPadding.bottom;
    if( IsEnabled() ) {
        if( m_bShowHtml )
            CRenderEngine::DrawHtmlText(hDC, m_pManager, rc, m_sText, m_dwTextColor, \
            m_rcLinks, m_sLinks, m_nLinks, m_uTextStyle);
        else
            CRenderEngine::DrawText(hDC, m_pManager, rc, m_sText, m_dwTextColor, \
            m_iFont, m_uTextStyle);
    }
    else {
        if( m_bShowHtml )
            CRenderEngine::DrawHtmlText(hDC, m_pManager, rc, m_sText, m_dwDisabledTextColor, \
            m_rcLinks, m_sLinks, m_nLinks, m_uTextStyle);
        else
            CRenderEngine::DrawText(hDC, m_pManager, rc, m_sText, m_dwDisabledTextColor, \
            m_iFont, m_uTextStyle);
    }
}


/////////////////////////////////////////////////////////////////////////////////////
//
//

CProgressUI::CProgressUI() : m_bHorizontal(true), m_nMin(0), m_nMax(100), m_nValue(0), m_nValue2(0)
{
    m_uTextStyle = DT_SINGLELINE | DT_CENTER;
    SetFixedHeight(12);
}

LPCTSTR CProgressUI::GetClass() const
{
    return _T("ProgressUI");
}

LPVOID CProgressUI::GetInterface(LPCTSTR pstrName)
{
    if( _tcscmp(pstrName, _T("Progress")) == 0 ) return static_cast<CProgressUI*>(this);
    return CLabelUI::GetInterface(pstrName);
}

bool CProgressUI::IsHorizontal()
{
    return m_bHorizontal;
}

void CProgressUI::SetHorizontal(bool bHorizontal)
{
    if( m_bHorizontal == bHorizontal ) return;

    m_bHorizontal = bHorizontal;
    Invalidate();
}

int CProgressUI::GetMinValue() const
{
    return m_nMin;
}

void CProgressUI::SetMinValue(int nMin)
{
    m_nMin = nMin;
    Invalidate();
}

int CProgressUI::GetMaxValue() const
{
    return m_nMax;
}

void CProgressUI::SetMaxValue(int nMax)
{
    m_nMax = nMax;
    Invalidate();
}

int CProgressUI::GetValue() const
{
    return m_nValue;
}

int CProgressUI::GetValue2() const
{
    return m_nValue2;
}

void CProgressUI::SetValue(int nValue)
{
    m_nValue = nValue;
    Invalidate();
}

void CProgressUI::SetValue2(int nValue)
{
    m_nValue2 = nValue;
    Invalidate();
}

LPCTSTR CProgressUI::GetFgImage() const
{
    return m_sFgImage;
}

void CProgressUI::SetFgImage(LPCTSTR pStrImage)
{
    if( m_sFgImage == pStrImage ) return;

    m_sFgImage = pStrImage;
    Invalidate();
}

LPCTSTR CProgressUI::GetFgImage2() const
{
	return m_sFgImage2;
}

void CProgressUI::SetFgImage2(LPCTSTR pStrImage)
{
	m_sFgImage2 = pStrImage;
	Invalidate();
}


void CProgressUI::SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue)
{
    if( _tcscmp(pstrName, _T("fgimage")) == 0 ) SetFgImage(pstrValue);
	else if( _tcscmp(pstrName, _T("fgimage2")) == 0 ) SetFgImage2(pstrValue);
    else if( _tcscmp(pstrName, _T("hor")) == 0 ) SetHorizontal(_tcscmp(pstrValue, _T("true")) == 0);
    else if( _tcscmp(pstrName, _T("min")) == 0 ) SetMinValue(_ttoi(pstrValue));
    else if( _tcscmp(pstrName, _T("max")) == 0 ) SetMaxValue(_ttoi(pstrValue));
    else if( _tcscmp(pstrName, _T("value")) == 0 ) SetValue(_ttoi(pstrValue));
    else CLabelUI::SetAttribute(pstrName, pstrValue);
}

void CProgressUI::PaintStatusImage(HDC hDC)
{
    if( m_nMax <= m_nMin ) m_nMax = m_nMin + 1;
    if( m_nValue > m_nMax ) m_nValue = m_nMax;
    if( m_nValue < m_nMin ) m_nValue = m_nMin;
	if( m_nValue2 > m_nMax ) m_nValue2 = m_nMax;
	if( m_nValue2 < m_nMin) m_nValue2 = m_nMin;

	RECT rc = {0},rc2 = {0};
    if( m_bHorizontal ) {
        rc.right = (m_nValue - m_nMin) * (float)(m_rcItem.right - m_rcItem.left) / (m_nMax - m_nMin);
        rc.bottom = m_rcItem.bottom - m_rcItem.top;

        rc2.right = (m_nValue2 - m_nMin) * (float)(m_rcItem.right - m_rcItem.left) / (m_nMax - m_nMin);
        rc2.bottom = m_rcItem.bottom - m_rcItem.top;
    }
	else {
		rc.top = (float)(m_rcItem.bottom - m_rcItem.top) * (m_nMax - m_nValue) / (m_nMax - m_nMin);
		rc.right = m_rcItem.right - m_rcItem.left;
		rc.bottom = m_rcItem.bottom - m_rcItem.top;

		rc2.top = (float)(m_rcItem.bottom - m_rcItem.top) * (m_nMax - m_nValue2) / (m_nMax - m_nMin);
		rc2.right = m_rcItem.right - m_rcItem.left;
		rc2.bottom = m_rcItem.bottom - m_rcItem.top;
	}

	if(!m_sFgImage2.IsEmpty() ) {
		m_sFgImage2Modify.Empty();
		m_sFgImage2Modify.SmallFormat(_T("dest='%d,%d,%d,%d'"), rc2.left, rc2.top, rc2.right, rc2.bottom);

		if(!DrawImage(hDC,(LPCTSTR)m_sFgImage2, (LPCTSTR)m_sFgImage2Modify) ) m_sFgImage2.Empty();
	}

	if( !m_sFgImage.IsEmpty() ) {
		m_sFgImageModify.Empty();
		m_sFgImageModify.SmallFormat(_T("dest='%d,%d,%d,%d'"), rc.left, rc.top, rc.right, rc.bottom);

		if( !DrawImage(hDC, (LPCTSTR)m_sFgImage, (LPCTSTR)m_sFgImageModify) ) m_sFgImage.Empty();
		else return;
	}

	DWORD dwBorderColor = 0xFF4EA0D1;
    int nBorderSize = 1;
    CRenderEngine::DrawRect(hDC, m_rcItem, nBorderSize, dwBorderColor);
}


/////////////////////////////////////////////////////////////////////////////////////
//
//

CSliderUI::CSliderUI() : m_uButtonState(0)
{
    m_uTextStyle = DT_SINGLELINE | DT_CENTER;
    m_szThumb.cx = m_szThumb.cy = 10;
}

LPCTSTR CSliderUI::GetClass() const
{
    return _T("SliderUI");
}

UINT CSliderUI::GetControlFlags() const
{
    if( IsEnabled() ) return UIFLAG_SETCURSOR;
    else return 0;
}

LPVOID CSliderUI::GetInterface(LPCTSTR pstrName)
{
    if( _tcscmp(pstrName, _T("Slider")) == 0 ) return static_cast<CSliderUI*>(this);
    return CProgressUI::GetInterface(pstrName);
}

void CSliderUI::SetEnabled(bool bEnable)
{
    CControlUI::SetEnabled(bEnable);
    if( !IsEnabled() ) {
        m_uButtonState = 0;
    }
}

void CSliderUI::SetThumbSize(SIZE szXY)
{
    m_szThumb = szXY;
}

RECT CSliderUI::GetThumbRect() const
{
    if( m_bHorizontal ) {
        int left = m_rcItem.left + (m_rcItem.right - m_rcItem.left - m_szThumb.cx) * (float)(m_nValue - m_nMin) / (m_nMax - m_nMin);
        int top = (m_rcItem.bottom + m_rcItem.top - m_szThumb.cy) / (float)2;
        return CRect(left, top, left + m_szThumb.cx, top + m_szThumb.cy); 
    }
    else {
        int left = (m_rcItem.right + m_rcItem.left - m_szThumb.cx) / (float)2;
        int top = m_rcItem.bottom - m_szThumb.cy - (m_rcItem.bottom - m_rcItem.top - m_szThumb.cy) * (float)(m_nValue - m_nMin) / (m_nMax - m_nMin);
        return CRect(left, top, left + m_szThumb.cx, top + m_szThumb.cy); 
    }
}

LPCTSTR CSliderUI::GetThumbImage() const
{
    return m_sThumbImage;
}

void CSliderUI::SetThumbImage(LPCTSTR pStrImage)
{
    m_sThumbImage = pStrImage;
    Invalidate();
}

LPCTSTR CSliderUI::GetThumbHotImage() const
{
    return m_sThumbHotImage;
}

void CSliderUI::SetThumbHotImage(LPCTSTR pStrImage)
{
    m_sThumbHotImage = pStrImage;
    Invalidate();
}

LPCTSTR CSliderUI::GetThumbPushedImage() const
{
    return m_sThumbPushedImage;
}

void CSliderUI::SetThumbPushedImage(LPCTSTR pStrImage)
{
    m_sThumbPushedImage = pStrImage;
    Invalidate();
}

void CSliderUI::Event(TEventUI& event)
{
    if( !IsMouseEnabled() && event.Type > UIEVENT__MOUSEBEGIN && event.Type < UIEVENT__MOUSEEND ) {
        if( m_pParent != NULL ) m_pParent->Event(event);
        else CProgressUI::Event(event);
        return;
    }

    if( event.Type == UIEVENT_BUTTONDOWN || event.Type == UIEVENT_DBLCLICK )
	{
		if( IsEnabled() ) {
			// RECT rcThumb = GetThumbRect();
			//if( ::PtInRect(&rcThumb, event.ptMouse) ) {
			m_uButtonState |= UISTATE_CAPTURED;
			if( m_bHorizontal ){
				if( event.ptMouse.x >= m_rcItem.right - m_szThumb.cx / 2 ) {
					m_nValue = m_nMax;
				}
				else if( event.ptMouse.x <= m_rcItem.left + m_szThumb.cx / 2 ) {
					m_nValue = m_nMin;
				}
				else {
					m_nValue = m_nMin + (m_nMax - m_nMin) * (float)(event.ptMouse.x - m_rcItem.left - m_szThumb.cx / 2 ) / (m_rcItem.right - m_rcItem.left - m_szThumb.cx);
				}
			}
			else {
				if( event.ptMouse.y >= m_rcItem.bottom - m_szThumb.cy / 2 ) {
					m_nValue = m_nMin;
				}
				else if( event.ptMouse.y <= m_rcItem.top + m_szThumb.cy / 2  ) {
					m_nValue = m_nMax;
				}
				else {
					m_nValue = m_nMin + (m_nMax - m_nMin) * (float)(m_rcItem.bottom - event.ptMouse.y - m_szThumb.cy / 2 ) / (m_rcItem.bottom - m_rcItem.top - m_szThumb.cy);
				}
			}

			Invalidate();
			//}
		}
        return;
    }
    if( event.Type == UIEVENT_BUTTONUP )
    {
        if( (m_uButtonState & UISTATE_CAPTURED) != 0 ) {
            m_uButtonState &= ~UISTATE_CAPTURED;
            m_pManager->SendNotify(this, MST_VALUECHANGED);
            Invalidate();
        }
        return;
    }
    if( event.Type == UIEVENT_MOUSEMOVE )
    {
        if( (m_uButtonState & UISTATE_CAPTURED) != 0 ) {
            if( m_bHorizontal ){
                if( event.ptMouse.x >= m_rcItem.right - m_szThumb.cx / 2 ) {
                    m_nValue = m_nMax;
					m_pManager->SendNotify(this, MST_VALUECHANGING);
                }
                else if( event.ptMouse.x <= m_rcItem.left + m_szThumb.cx / 2 ) {
                    m_nValue = m_nMin;
					m_pManager->SendNotify(this, MST_VALUECHANGING);
                }
                else {
                    m_nValue = m_nMin + (m_nMax - m_nMin) * (float)(event.ptMouse.x - m_rcItem.left - m_szThumb.cx / 2 ) / (m_rcItem.right - m_rcItem.left - m_szThumb.cx);
					m_pManager->SendNotify(this, MST_VALUECHANGING);
				}
            }
            else {
                if( event.ptMouse.y >= m_rcItem.bottom - m_szThumb.cy / 2 ) {
                    m_nValue = m_nMin;
					m_pManager->SendNotify(this, MST_VALUECHANGING);
                }
                else if( event.ptMouse.y <= m_rcItem.top + m_szThumb.cy / 2  ) {
                    m_nValue = m_nMax;
					m_pManager->SendNotify(this, MST_VALUECHANGING);
                }
                else {
                    m_nValue = m_nMin + (m_nMax - m_nMin) * (float)(m_rcItem.bottom - event.ptMouse.y - m_szThumb.cy / 2 ) / (m_rcItem.bottom - m_rcItem.top - m_szThumb.cy);
					m_pManager->SendNotify(this, MST_VALUECHANGING);
                }
            }

            Invalidate();
        }
		else
		{
			int value;
            if( m_bHorizontal ){
                if( event.ptMouse.x >= m_rcItem.right - m_szThumb.cx / 2 ) {
                    value = m_nMax;
					m_pManager->SendNotify(this, MST_MOVING,0,value);
                }
                else if( event.ptMouse.x <= m_rcItem.left + m_szThumb.cx / 2 ) {
                    value = m_nMin;
					m_pManager->SendNotify(this, MST_MOVING,0,value);
                }
                else {
                    value = m_nMin + (m_nMax - m_nMin) * (float)(event.ptMouse.x - m_rcItem.left - m_szThumb.cx / 2 ) / (m_rcItem.right - m_rcItem.left - m_szThumb.cx);
					m_pManager->SendNotify(this, MST_MOVING,0,value);
				}
            }
            else {
                if( event.ptMouse.y >= m_rcItem.bottom - m_szThumb.cy / 2 ) {
                    value = m_nMin;
					m_pManager->SendNotify(this, MST_MOVING,0,value);
                }
                else if( event.ptMouse.y <= m_rcItem.top + m_szThumb.cy / 2  ) {
                    value = m_nMax;
					m_pManager->SendNotify(this, MST_MOVING,0,value);
                }
                else {
                    value = m_nMin + (m_nMax - m_nMin) * (float)(m_rcItem.bottom - event.ptMouse.y - m_szThumb.cy / 2 ) / (m_rcItem.bottom - m_rcItem.top - m_szThumb.cy);
					m_pManager->SendNotify(this, MST_MOVING,0,value);
                }
            }
		}
        return;
    }
    if( event.Type == UIEVENT_SETCURSOR )
    {
        if( IsEnabled()) {
            ::SetCursor(::LoadCursor(NULL, MAKEINTRESOURCE(IDC_HAND)));
            return;
        }
    }
    if( event.Type == UIEVENT_MOUSEENTER )
    {
        if( IsEnabled() ) {
            m_uButtonState |= UISTATE_HOT;
            Invalidate();
        }
        return;
    }
    if( event.Type == UIEVENT_MOUSELEAVE )
    {
        if( IsEnabled() ) {
            m_uButtonState &= ~UISTATE_HOT;
            Invalidate();
        }
        return;
    }
    CControlUI::Event(event);
}


void CSliderUI::SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue)
{
    if( _tcscmp(pstrName, _T("thumbimage")) == 0 ) SetThumbImage(pstrValue);
    else if( _tcscmp(pstrName, _T("thumbhotimage")) == 0 ) SetThumbHotImage(pstrValue);
    else if( _tcscmp(pstrName, _T("thumbpushedimage")) == 0 ) SetThumbPushedImage(pstrValue);
    else if( _tcscmp(pstrName, _T("thumbsize")) == 0 ) {
        SIZE szXY = {0};
        LPTSTR pstr = NULL;
        szXY.cx = _tcstol(pstrValue, &pstr, 10);  ASSERT(pstr);    
        szXY.cy = _tcstol(pstr + 1, &pstr, 10);    ASSERT(pstr); 
        SetThumbSize(szXY);
    }
    else CProgressUI::SetAttribute(pstrName, pstrValue);
}

void CSliderUI::PaintStatusImage(HDC hDC)
{
    CProgressUI::PaintStatusImage(hDC);

    RECT rcThumb = GetThumbRect();
    rcThumb.left -= m_rcItem.left;
    rcThumb.top -= m_rcItem.top;
    rcThumb.right -= m_rcItem.left;
    rcThumb.bottom -= m_rcItem.top;
    if( (m_uButtonState & UISTATE_CAPTURED) != 0 ) {
        if( !m_sThumbPushedImage.IsEmpty() ) {
            m_sImageModify.Empty();
            m_sImageModify.SmallFormat(_T("dest='%d,%d,%d,%d'"), rcThumb.left, rcThumb.top, rcThumb.right, rcThumb.bottom);
            if( !DrawImage(hDC, (LPCTSTR)m_sThumbPushedImage, (LPCTSTR)m_sImageModify) ) m_sThumbPushedImage.Empty();
            else return;
        }
    }
    else if( (m_uButtonState & UISTATE_HOT) != 0 ) {
        if( !m_sThumbHotImage.IsEmpty() ) {
            m_sImageModify.Empty();
            m_sImageModify.SmallFormat(_T("dest='%d,%d,%d,%d'"), rcThumb.left, rcThumb.top, rcThumb.right, rcThumb.bottom);
            if( !DrawImage(hDC, (LPCTSTR)m_sThumbHotImage, (LPCTSTR)m_sImageModify) ) m_sThumbHotImage.Empty();
            else return;
        }
    }

    if( !m_sThumbImage.IsEmpty() ) {
        m_sImageModify.Empty();
        m_sImageModify.SmallFormat(_T("dest='%d,%d,%d,%d'"), rcThumb.left, rcThumb.top, rcThumb.right, rcThumb.bottom);
        if( !DrawImage(hDC, (LPCTSTR)m_sThumbImage, (LPCTSTR)m_sImageModify) ) m_sThumbImage.Empty();
        else return;
    }
}

/////////////////////////////////////////////////////////////////////////////////////
//
//


CEditWnd::CEditWnd() : m_pOwner(NULL)
{
}

void CEditWnd::Init(CEditUI* pOwner)
{
    CRect rcPos = pOwner->GetPos();
    RECT rcInset = pOwner->GetTextPadding();
    rcPos.left += rcInset.left;
    rcPos.top += rcInset.top;
    rcPos.right -= rcInset.right;
    rcPos.bottom -= rcInset.bottom;
    UINT uStyle = WS_CHILD | ES_AUTOHSCROLL;
    if( pOwner->IsPasswordMode() ) uStyle |= ES_PASSWORD;
    Create(pOwner->GetManager()->GetPaintWindow(), NULL, uStyle, 0, rcPos);
    SetWindowFont(m_hWnd, pOwner->GetManager()->GetDefaultFont(), TRUE);
    Edit_LimitText(m_hWnd, pOwner->GetMaxChar());
    if( pOwner->IsPasswordMode() ) Edit_SetPasswordChar(m_hWnd, pOwner->GetPasswordChar());
    Edit_SetText(m_hWnd, pOwner->GetText());
    Edit_SetModify(m_hWnd, FALSE);
    SendMessage(EM_SETMARGINS, EC_LEFTMARGIN | EC_RIGHTMARGIN, MAKELPARAM(0, 0));
    Edit_Enable(m_hWnd, pOwner->IsEnabled() == true);
	Edit_SetReadOnly(m_hWnd, pOwner->IsReadOnly() == true);
	LONG dwStyle = GetWindowLong( m_hWnd, GWL_STYLE);
	if(pOwner->GetNumOnly())
		::SetWindowLong(m_hWnd, GWL_STYLE, dwStyle|ES_NUMBER);
	else
		::SetWindowLong(m_hWnd, GWL_STYLE, dwStyle&~ES_NUMBER);
    ::ShowWindow(m_hWnd, SW_SHOWNOACTIVATE);
    ::SetFocus(m_hWnd);
    m_pOwner = pOwner;
}

LPCTSTR CEditWnd::GetWindowClassName() const
{
    return _T("EditWnd");
}

LPCTSTR CEditWnd::GetSuperClassName() const
{
    return WC_EDIT;
}

void CEditWnd::OnFinalMessage(HWND /*hWnd*/)
{
    // Clear reference and die
    m_pOwner->m_pWindow = NULL;
    delete this;
}

LRESULT CEditWnd::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    LRESULT lRes = 0;
    BOOL bHandled = TRUE;
    if( uMsg == WM_KILLFOCUS ) lRes = OnKillFocus(uMsg, wParam, lParam, bHandled);
    else if( uMsg == OCM_COMMAND && GET_WM_COMMAND_CMD(wParam, lParam) == EN_CHANGE ) lRes = OnEditChanged(uMsg, wParam, lParam, bHandled);
	else if( uMsg == WM_KEYDOWN && TCHAR(wParam) == VK_RETURN )	m_pOwner->GetManager()->SendNotify(m_pOwner, MST_RETURN);
    else bHandled = FALSE;
    if( !bHandled ) return CWindowWnd::HandleMessage(uMsg, wParam, lParam);
    return lRes;
}

LRESULT CEditWnd::OnKillFocus(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
    LRESULT lRes = ::DefWindowProc(m_hWnd, uMsg, wParam, lParam);
    PostMessage(WM_CLOSE);
    return lRes;
}

LRESULT CEditWnd::OnEditChanged(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
    if( m_pOwner == NULL ) return 0;
    // Copy text back
    int cchLen = ::GetWindowTextLength(m_hWnd) + 1;
    LPTSTR pstr = static_cast<LPTSTR>(_alloca(cchLen * sizeof(TCHAR)));
    ASSERT(pstr);
    if( pstr == NULL ) return 0;
    ::GetWindowText(m_hWnd, pstr, cchLen);
    m_pOwner->SetText(pstr);
    return 0;
}


/////////////////////////////////////////////////////////////////////////////////////
//
//

CEditUI::CEditUI() : m_pWindow(NULL), m_uMaxChar(255), m_bReadOnly(false), 
m_bPasswordMode(false), m_cPasswordChar(_T('*')), m_uButtonState(0)
, m_bNumOnly(FALSE)
{
    SetTextPadding(CRect(4, 4, 4, 4));
    SetBkColor(0xFFFFFFFF);
}

LPCTSTR CEditUI::GetClass() const
{
    return _T("EditUI");
}

LPVOID CEditUI::GetInterface(LPCTSTR pstrName)
{
    if( _tcscmp(pstrName, _T("Edit")) == 0 ) return static_cast<CEditUI*>(this);
    return CLabelUI::GetInterface(pstrName);
}

UINT CEditUI::GetControlFlags() const
{
    if( !IsEnabled() ) return CControlUI::GetControlFlags();

    return UIFLAG_SETCURSOR | UIFLAG_TABSTOP;
}

void CEditUI::Event(TEventUI& event)
{
    if( !IsMouseEnabled() && event.Type > UIEVENT__MOUSEBEGIN && event.Type < UIEVENT__MOUSEEND ) {
        if( m_pParent != NULL ) m_pParent->Event(event);
        else CLabelUI::Event(event);
        return;
    }

    if( event.Type == UIEVENT_SETCURSOR && IsEnabled() )
    {
        ::SetCursor(::LoadCursor(NULL, MAKEINTRESOURCE(IDC_IBEAM)));
        return;
    }
    if( event.Type == UIEVENT_WINDOWSIZE )
    {
        if( m_pWindow != NULL ) m_pManager->SetFocus(NULL);
    }
    if( event.Type == UIEVENT_SCROLLWHEEL )
    {
        if( m_pWindow != NULL ) m_pManager->SetFocus(NULL);
    }
    if( event.Type == UIEVENT_SETFOCUS && IsEnabled() ) 
    {
        if( m_pWindow ) return;
        m_pWindow = new CEditWnd();
        ASSERT(m_pWindow);
        m_pWindow->Init(this);
        Invalidate();
    }
    if( event.Type == UIEVENT_KILLFOCUS && IsEnabled() ) 
    {
        Invalidate();
    }
    if( event.Type == UIEVENT_BUTTONDOWN || event.Type == UIEVENT_DBLCLICK ) 
    {
        if( IsEnabled() ) {
            if( IsFocused() && m_pWindow == NULL )
            {
                m_pWindow = new CEditWnd();
                ASSERT(m_pWindow);
                m_pWindow->Init(this);
            }
            else if( m_pWindow != NULL )
            {
                POINT pt = event.ptMouse;
                pt.x -= m_rcItem.left + m_rcTextPadding.left;
                pt.y -= m_rcItem.top + m_rcTextPadding.top;
                ::SendMessage(*m_pWindow, WM_LBUTTONDOWN, event.wParam, MAKELPARAM(pt.x, pt.y));
            }
        }
        return;
    }
    if( event.Type == UIEVENT_MOUSEMOVE ) 
    {
        return;
    }
    if( event.Type == UIEVENT_BUTTONUP ) 
    {
        return;
    }
    if( event.Type == UIEVENT_MOUSEENTER )
    {
        if( IsEnabled() ) {
            m_uButtonState |= UISTATE_HOT;
            Invalidate();
        }
        return;
    }
    if( event.Type == UIEVENT_MOUSELEAVE )
    {
        if( IsEnabled() ) {
            m_uButtonState &= ~UISTATE_HOT;
            Invalidate();
        }
        return;
    }
    CLabelUI::Event(event);
}

void CEditUI::SetEnabled(bool bEnable)
{
    CControlUI::SetEnabled(bEnable);
    if( !IsEnabled() ) {
        m_uButtonState = 0;
    }
}

void CEditUI::SetText(LPCTSTR pstrText)
{
    m_sText = pstrText;
    if( m_pManager != NULL ) m_pManager->SendNotify(this, MST_TEXTCHANGED);
    Invalidate();
}

void CEditUI::SetMaxChar(UINT uMax)
{
    m_uMaxChar = uMax;
    if( m_pWindow != NULL ) Edit_LimitText(*m_pWindow, m_uMaxChar);
}

UINT CEditUI::GetMaxChar()
{
    return m_uMaxChar;
}

void CEditUI::SetReadOnly(bool bReadOnly)
{
    if( m_bReadOnly == bReadOnly ) return;

    m_bReadOnly = bReadOnly;
    if( m_pWindow != NULL ) Edit_SetReadOnly(*m_pWindow, m_bReadOnly);
    Invalidate();
}

bool CEditUI::IsReadOnly() const
{
    return m_bReadOnly;
}

void CEditUI::SetPasswordMode(bool bPasswordMode)
{
    if( m_bPasswordMode == bPasswordMode ) return;
    m_bPasswordMode = bPasswordMode;
    Invalidate();
}

bool CEditUI::IsPasswordMode() const
{
    return m_bPasswordMode;
}

void CEditUI::SetPasswordChar(TCHAR cPasswordChar)
{
    if( m_cPasswordChar == cPasswordChar ) return;
    m_cPasswordChar = cPasswordChar;
    if( m_pWindow != NULL ) Edit_SetPasswordChar(*m_pWindow, m_cPasswordChar);
    Invalidate();
}

TCHAR CEditUI::GetPasswordChar() const
{
    return m_cPasswordChar;
}

LPCTSTR CEditUI::GetNormalImage()
{
    return m_sNormalImage;
}

void CEditUI::SetNormalImage(LPCTSTR pStrImage)
{
    m_sNormalImage = pStrImage;
    Invalidate();
}

LPCTSTR CEditUI::GetHotImage()
{
    return m_sHotImage;
}

void CEditUI::SetHotImage(LPCTSTR pStrImage)
{
    m_sHotImage = pStrImage;
    Invalidate();
}

LPCTSTR CEditUI::GetFocusedImage()
{
    return m_sFocusedImage;
}

void CEditUI::SetFocusedImage(LPCTSTR pStrImage)
{
    m_sFocusedImage = pStrImage;
    Invalidate();
}

LPCTSTR CEditUI::GetDisabledImage()
{
    return m_sDisabledImage;
}

void CEditUI::SetDisabledImage(LPCTSTR pStrImage)
{
    m_sDisabledImage = pStrImage;
    Invalidate();
}

void CEditUI::SetVisible(bool bVisible)
{
    CControlUI::SetVisible(bVisible);
    if( !IsVisible() && m_pWindow != NULL ) m_pManager->SetFocus(NULL);
}

void CEditUI::SetInternVisible(bool bVisible)
{
    if( !IsVisible() && m_pWindow != NULL ) m_pManager->SetFocus(NULL);
}

SIZE CEditUI::EstimateSize(SIZE szAvailable)
{
    if( m_cxyFixed.cy == 0 ) return CSize(m_cxyFixed.cx, m_pManager->GetDefaultFontInfo().tmHeight + 6);
    return CControlUI::EstimateSize(szAvailable);
}

void CEditUI::SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue)
{
    if( _tcscmp(pstrName, _T("readonly")) == 0 ) SetReadOnly(_tcscmp(pstrValue, _T("true")) == 0);
	else if(_tcscmp(pstrName,_T("numonly"))==0 ) SetNumOnly(_tcscmp(pstrValue,_T("true")) == 0);
    else if( _tcscmp(pstrName, _T("password")) == 0 ) SetPasswordMode(_tcscmp(pstrValue, _T("true")) == 0);
    else if( _tcscmp(pstrName, _T("normalimage")) == 0 ) SetNormalImage(pstrValue);
    else if( _tcscmp(pstrName, _T("hotimage")) == 0 ) SetHotImage(pstrValue);
    else if( _tcscmp(pstrName, _T("focusedimage")) == 0 ) SetFocusedImage(pstrValue);
    else if( _tcscmp(pstrName, _T("disabledimage")) == 0 ) SetDisabledImage(pstrValue);
    else CLabelUI::SetAttribute(pstrName, pstrValue);
}

void CEditUI::PaintStatusImage(HDC hDC)
{
    if( IsFocused() ) m_uButtonState |= UISTATE_FOCUSED;
    else m_uButtonState &= ~ UISTATE_FOCUSED;
    if( !IsEnabled() ) m_uButtonState |= UISTATE_DISABLED;
    else m_uButtonState &= ~ UISTATE_DISABLED;

    if( (m_uButtonState & UISTATE_DISABLED) != 0 ) {
        if( !m_sDisabledImage.IsEmpty() ) {
            if( !DrawImage(hDC, (LPCTSTR)m_sDisabledImage) ) m_sDisabledImage.Empty();
            else return;
        }
    }
    else if( (m_uButtonState & UISTATE_FOCUSED) != 0 ) {
        if( !m_sFocusedImage.IsEmpty() ) {
            if( !DrawImage(hDC, (LPCTSTR)m_sFocusedImage) ) m_sFocusedImage.Empty();
            else return;
        }
    }
    else if( (m_uButtonState & UISTATE_HOT) != 0 ) {
        if( !m_sHotImage.IsEmpty() ) {
            if( !DrawImage(hDC, (LPCTSTR)m_sHotImage) ) m_sHotImage.Empty();
            else return;
        }
    }

    if( !m_sNormalImage.IsEmpty() ) {
        if( !DrawImage(hDC, (LPCTSTR)m_sNormalImage) ) m_sNormalImage.Empty();
        else return;
    }

    DWORD dwBorderColor = 0xFF4EA0D1;
    int nBorderSize = 1;
    if( (m_uButtonState & UISTATE_HOT) != 0 || (m_uButtonState & UISTATE_FOCUSED) != 0) {
        dwBorderColor = 0xFF85E4FF;
        nBorderSize = 2;
    }
    CRenderEngine::DrawRect(hDC, m_rcItem, nBorderSize, dwBorderColor);
}

void CEditUI::PaintText(HDC hDC)
{
    if( m_dwTextColor == 0 ) m_dwTextColor = m_pManager->GetDefaultFontColor();
    if( m_dwDisabledTextColor == 0 ) m_dwDisabledTextColor = m_pManager->GetDefaultDisabledColor();

    if( m_sText.IsEmpty() ) return;

    CStdString sText = m_sText;
    if( m_bPasswordMode ) {
        sText.Empty();
        LPCTSTR p = m_sText.GetData();
        while( *p != _T('\0') ) {
            sText += m_cPasswordChar;
            p = ::CharNext(p);
        }
    }

    RECT rc = m_rcItem;
    rc.left += m_rcTextPadding.left;
    rc.right -= m_rcTextPadding.right;
    rc.top += m_rcTextPadding.top;
    rc.bottom -= m_rcTextPadding.bottom;
    if( IsEnabled() ) {
        CRenderEngine::DrawText(hDC, m_pManager, rc, sText, m_dwTextColor, \
            m_iFont, DT_SINGLELINE | m_uTextStyle);
    }
    else {
        CRenderEngine::DrawText(hDC, m_pManager, rc, sText, m_dwDisabledTextColor, \
            m_iFont, DT_SINGLELINE | m_uTextStyle);
    }
}

/////////////////////////////////////////////////////////////////////////////////////
//
//

CScrollbarUI::CScrollbarUI() : m_bHorizontal(false), m_nRange(100), m_nScrollPos(0), m_nLineSize(8), 
m_pOwner(NULL), m_nLastScrollPos(0), m_nLastScrollOffset(0), m_nScrollRepeatDelay(0), m_uButton1State(0), \
m_uButton2State(0), m_uThumbState(0)
{
    m_cxyFixed.cx = DEFAULT_SCROLLBAR_SIZE;
    ptLastMouse.x = ptLastMouse.y = 0;
    ::ZeroMemory(&m_rcThumb, sizeof(m_rcThumb));
    ::ZeroMemory(&m_rcButton1, sizeof(m_rcButton1));
    ::ZeroMemory(&m_rcButton2, sizeof(m_rcButton2));
}

LPCTSTR CScrollbarUI::GetClass() const
{
    return _T("ScrollbarUI");
}

LPVOID CScrollbarUI::GetInterface(LPCTSTR pstrName)
{
    if( _tcscmp(pstrName, _T("Scrollbar")) == 0 ) return static_cast<CScrollbarUI*>(this);
    return CControlUI::GetInterface(pstrName);
}

CContainerUI* CScrollbarUI::GetOwner() const
{
    return m_pOwner;
}

void CScrollbarUI::SetOwner(CContainerUI* pOwner)
{
    m_pOwner = pOwner;
}

void CScrollbarUI::SetEnabled(bool bEnable)
{
    CControlUI::SetEnabled(bEnable);
    if( !IsEnabled() ) {
        m_uButton1State = 0;
        m_uButton2State = 0;
        m_uThumbState = 0;
    }
}

bool CScrollbarUI::IsHorizontal()
{
    return m_bHorizontal;
}

void CScrollbarUI::SetHorizontal(bool bHorizontal)
{
    if( m_bHorizontal == bHorizontal ) return;

    m_bHorizontal = bHorizontal;
    if( m_bHorizontal ) {
        if( m_cxyFixed.cy == 0 ) {
            m_cxyFixed.cx = 0;
            m_cxyFixed.cy = DEFAULT_SCROLLBAR_SIZE;
        }
    }
    else {
        if( m_cxyFixed.cx == 0 ) {
            m_cxyFixed.cx = DEFAULT_SCROLLBAR_SIZE;
            m_cxyFixed.cy = 0;
        }
    }

    if( m_pOwner != NULL ) m_pOwner->NeedUpdate(); else NeedParentUpdate();
}

int CScrollbarUI::GetScrollRange() const
{
    return m_nRange;
}

void CScrollbarUI::SetScrollRange(int nRange)
{
    if( m_nRange == nRange ) return;
    
    m_nRange = nRange;
    if( m_nRange < 0 ) m_nRange = 0;
    if( m_nScrollPos > m_nRange ) m_nScrollPos = m_nRange;
    SetPos(m_rcItem);
}

int CScrollbarUI::GetScrollPos() const
{
    return m_nScrollPos;
}

void CScrollbarUI::SetScrollPos(int nPos)
{
    if( m_nScrollPos == nPos ) return;

    m_nScrollPos = nPos;
    if( m_nScrollPos < 0 ) m_nScrollPos = 0;
    if( m_nScrollPos > m_nRange ) m_nScrollPos = m_nRange;
    SetPos(m_rcItem);
}

int CScrollbarUI::GetLineSize() const
{
    return m_nLineSize;
}

void CScrollbarUI::SetLineSize(int nSize)
{
    m_nLineSize = nSize;
}

LPCTSTR CScrollbarUI::GetButton1NormalImage()
{
    return m_sButton1NormalImage;
}

void CScrollbarUI::SetButton1NormalImage(LPCTSTR pStrImage)
{
    m_sButton1NormalImage = pStrImage;
    Invalidate();
}

LPCTSTR CScrollbarUI::GetButton1HotImage()
{
    return m_sButton1HotImage;
}

void CScrollbarUI::SetButton1HotImage(LPCTSTR pStrImage)
{
    m_sButton1HotImage = pStrImage;
    Invalidate();
}

LPCTSTR CScrollbarUI::GetButton1PushedImage()
{
    return m_sButton1PushedImage;
}

void CScrollbarUI::SetButton1PushedImage(LPCTSTR pStrImage)
{
    m_sButton1PushedImage = pStrImage;
    Invalidate();
}

LPCTSTR CScrollbarUI::GetButton1DisabledImage()
{
    return m_sButton1DisabledImage;
}

void CScrollbarUI::SetButton1DisabledImage(LPCTSTR pStrImage)
{
    m_sButton1DisabledImage = pStrImage;
    Invalidate();
}

LPCTSTR CScrollbarUI::GetButton2NormalImage()
{
    return m_sButton2NormalImage;
}

void CScrollbarUI::SetButton2NormalImage(LPCTSTR pStrImage)
{
    m_sButton2NormalImage = pStrImage;
    Invalidate();
}

LPCTSTR CScrollbarUI::GetButton2HotImage()
{
    return m_sButton2HotImage;
}

void CScrollbarUI::SetButton2HotImage(LPCTSTR pStrImage)
{
    m_sButton2HotImage = pStrImage;
    Invalidate();
}

LPCTSTR CScrollbarUI::GetButton2PushedImage()
{
    return m_sButton2PushedImage;
}

void CScrollbarUI::SetButton2PushedImage(LPCTSTR pStrImage)
{
    m_sButton2PushedImage = pStrImage;
    Invalidate();
}

LPCTSTR CScrollbarUI::GetButton2DisabledImage()
{
    return m_sButton2DisabledImage;
}

void CScrollbarUI::SetButton2DisabledImage(LPCTSTR pStrImage)
{
    m_sButton2DisabledImage = pStrImage;
    Invalidate();
}

LPCTSTR CScrollbarUI::GetThumbNormalImage()
{
    return m_sThumbNormalImage;
}

void CScrollbarUI::SetThumbNormalImage(LPCTSTR pStrImage)
{
    m_sThumbNormalImage = pStrImage;
    Invalidate();
}

LPCTSTR CScrollbarUI::GetThumbHotImage()
{
    return m_sThumbHotImage;
}

void CScrollbarUI::SetThumbHotImage(LPCTSTR pStrImage)
{
    m_sThumbHotImage = pStrImage;
    Invalidate();
}

LPCTSTR CScrollbarUI::GetThumbPushedImage()
{
    return m_sThumbPushedImage;
}

void CScrollbarUI::SetThumbPushedImage(LPCTSTR pStrImage)
{
    m_sThumbPushedImage = pStrImage;
    Invalidate();
}

LPCTSTR CScrollbarUI::GetThumbDisabledImage()
{
    return m_sThumbDisabledImage;
}

void CScrollbarUI::SetThumbDisabledImage(LPCTSTR pStrImage)
{
    m_sThumbDisabledImage = pStrImage;
    Invalidate();
}

LPCTSTR CScrollbarUI::GetRailNormalImage()
{
    return m_sRailNormalImage;
}

void CScrollbarUI::SetRailNormalImage(LPCTSTR pStrImage)
{
    m_sRailNormalImage = pStrImage;
    Invalidate();
}

LPCTSTR CScrollbarUI::GetRailHotImage()
{
    return m_sRailHotImage;
}

void CScrollbarUI::SetRailHotImage(LPCTSTR pStrImage)
{
    m_sRailHotImage = pStrImage;
    Invalidate();
}

LPCTSTR CScrollbarUI::GetRailPushedImage()
{
    return m_sRailPushedImage;
}

void CScrollbarUI::SetRailPushedImage(LPCTSTR pStrImage)
{
    m_sRailPushedImage = pStrImage;
    Invalidate();
}

LPCTSTR CScrollbarUI::GetRailDisabledImage()
{
    return m_sRailDisabledImage;
}

void CScrollbarUI::SetRailDisabledImage(LPCTSTR pStrImage)
{
    m_sRailDisabledImage = pStrImage;
    Invalidate();
}

LPCTSTR CScrollbarUI::GetBkNormalImage()
{
    return m_sBkNormalImage;
}

void CScrollbarUI::SetBkNormalImage(LPCTSTR pStrImage)
{
    m_sBkNormalImage = pStrImage;
    Invalidate();
}

LPCTSTR CScrollbarUI::GetBkHotImage()
{
    return m_sBkHotImage;
}

void CScrollbarUI::SetBkHotImage(LPCTSTR pStrImage)
{
    m_sBkHotImage = pStrImage;
    Invalidate();
}

LPCTSTR CScrollbarUI::GetBkPushedImage()
{
    return m_sBkPushedImage;
}

void CScrollbarUI::SetBkPushedImage(LPCTSTR pStrImage)
{
    m_sBkPushedImage = pStrImage;
    Invalidate();
}

LPCTSTR CScrollbarUI::GetBkDisabledImage()
{
    return m_sBkDisabledImage;
}

void CScrollbarUI::SetBkDisabledImage(LPCTSTR pStrImage)
{
    m_sBkDisabledImage = pStrImage;
    Invalidate();
}

void CScrollbarUI::SetPos(RECT rc)
{
    CControlUI::SetPos(rc);
    rc = m_rcItem;

    if( m_bHorizontal ) {
        int cx = rc.right - rc.left - 2 * m_cxyFixed.cy;
        if( cx > m_cxyFixed.cy ) {
            m_rcButton1.left = rc.left;
            m_rcButton1.top = rc.top;
            m_rcButton1.right = rc.left + m_cxyFixed.cy;
            m_rcButton1.bottom = rc.top + m_cxyFixed.cy;

            m_rcButton2.left = rc.right - m_cxyFixed.cy;
            m_rcButton2.top = rc.top;
            m_rcButton2.right = rc.right;
            m_rcButton2.bottom = rc.top + m_cxyFixed.cy;

            m_rcThumb.top = rc.top;
            m_rcThumb.bottom = rc.top + m_cxyFixed.cy;
            if( m_nRange > 0 ) {
                int cxThumb = cx * (rc.right - rc.left) / (m_nRange + rc.right - rc.left);
                if( cxThumb < m_cxyFixed.cy ) cxThumb = m_cxyFixed.cy;

                m_rcThumb.left = m_nScrollPos * (cx - cxThumb) / m_nRange + m_rcButton1.right;
                m_rcThumb.right = m_rcThumb.left + cxThumb;
                if( m_rcThumb.right > m_rcButton2.left ) {
                    m_rcThumb.left = m_rcButton2.left - cxThumb;
                    m_rcThumb.right = m_rcButton2.left;
                }
            }
            else {
                m_rcThumb.left = m_rcButton1.right;
                m_rcThumb.right = m_rcButton2.left;
            }
        }
        else {
            int cxButton = (rc.right - rc.left) / 2;
            if( cxButton > m_cxyFixed.cy ) cxButton = m_cxyFixed.cy;
            m_rcButton1.left = rc.left;
            m_rcButton1.top = rc.top;
            m_rcButton1.right = rc.left + cxButton;
            m_rcButton1.bottom = rc.top + m_cxyFixed.cy;

            m_rcButton2.left = rc.right - cxButton;
            m_rcButton2.top = rc.top;
            m_rcButton2.right = rc.right;
            m_rcButton2.bottom = rc.top + m_cxyFixed.cy;

            ::ZeroMemory(&m_rcThumb, sizeof(m_rcThumb));
        }
    }
    else {
        int cy = rc.bottom - rc.top - 2 * m_cxyFixed.cx;
        if( cy > m_cxyFixed.cx ) {
            m_rcButton1.left = rc.left;
            m_rcButton1.top = rc.top;
            m_rcButton1.right = rc.left + m_cxyFixed.cx;
            m_rcButton1.bottom = rc.top + m_cxyFixed.cx;

            m_rcButton2.left = rc.left;
            m_rcButton2.top = rc.bottom - m_cxyFixed.cx;
            m_rcButton2.right = rc.left + m_cxyFixed.cx;
            m_rcButton2.bottom = rc.bottom;

            m_rcThumb.left = rc.left;
            m_rcThumb.right = rc.left + m_cxyFixed.cx;
            if( m_nRange > 0 ) {
                int cyThumb = cy * (rc.bottom - rc.top) / (m_nRange + rc.bottom - rc.top);
                if( cyThumb < m_cxyFixed.cx ) cyThumb = m_cxyFixed.cx;

                m_rcThumb.top = m_nScrollPos * (cy - cyThumb) / m_nRange + m_rcButton1.bottom;
                m_rcThumb.bottom = m_rcThumb.top + cyThumb;
                if( m_rcThumb.bottom > m_rcButton2.top ) {
                    m_rcThumb.top = m_rcButton2.top - cyThumb;
                    m_rcThumb.bottom = m_rcButton2.top;
                }
            }
            else {
                m_rcThumb.top = m_rcButton1.bottom;
                m_rcThumb.bottom = m_rcButton2.top;
            }
        }
        else {
            int cyButton = (rc.bottom - rc.top) / 2;
            if( cyButton > m_cxyFixed.cx ) cyButton = m_cxyFixed.cx;
            m_rcButton1.left = rc.left;
            m_rcButton1.top = rc.top;
            m_rcButton1.right = rc.left + m_cxyFixed.cx;
            m_rcButton1.bottom = rc.top + cyButton;
            
            m_rcButton2.left = rc.left;
            m_rcButton2.top = rc.bottom - cyButton;
            m_rcButton2.right = rc.left + m_cxyFixed.cx;
            m_rcButton2.bottom = rc.bottom;
            
            ::ZeroMemory(&m_rcThumb, sizeof(m_rcThumb));
        }
    }
}

void CScrollbarUI::Event(TEventUI& event)
{
    if( !IsMouseEnabled() && event.Type > UIEVENT__MOUSEBEGIN && event.Type < UIEVENT__MOUSEEND ) {
        if( m_pOwner != NULL ) m_pOwner->Event(event);
        else CControlUI::Event(event);
        return;
    }

    if( event.Type == UIEVENT_SETFOCUS ) 
    {
        return;
    }
    if( event.Type == UIEVENT_KILLFOCUS ) 
    {
        return;
    }
    if( event.Type == UIEVENT_BUTTONDOWN || event.Type == UIEVENT_DBLCLICK )
    {
        if( !IsEnabled() ) return;

        m_nLastScrollOffset = 0;
        m_nScrollRepeatDelay = 0;
        m_pManager->SetTimer(this, DEFAULT_TIMERID, 50U);

        if( ::PtInRect(&m_rcButton1, event.ptMouse) ) {
            m_uButton1State |= UISTATE_PUSHED;
            if( !m_bHorizontal ) {
                if( m_pOwner != NULL ) m_pOwner->LineUp(); 
                else SetScrollPos(m_nScrollPos - m_nLineSize);
            }
            else {
                if( m_pOwner != NULL ) m_pOwner->LineLeft(); 
                else SetScrollPos(m_nScrollPos - m_nLineSize);
            }
        }
        else if( ::PtInRect(&m_rcButton2, event.ptMouse) ) {
            m_uButton2State |= UISTATE_PUSHED;
            if( !m_bHorizontal ) {
                if( m_pOwner != NULL ) m_pOwner->LineDown(); 
                else SetScrollPos(m_nScrollPos + m_nLineSize);
            }
            else {
                if( m_pOwner != NULL ) m_pOwner->LineRight(); 
                else SetScrollPos(m_nScrollPos + m_nLineSize);
            }
        }
        else if( ::PtInRect(&m_rcThumb, event.ptMouse) ) {
            m_uThumbState |= UISTATE_CAPTURED | UISTATE_PUSHED;
            ptLastMouse = event.ptMouse;
            m_nLastScrollPos = m_nScrollPos;
        }
        else {
            if( !m_bHorizontal ) {
                if( event.ptMouse.y < m_rcThumb.top ) {
                    if( m_pOwner != NULL ) m_pOwner->PageUp(); 
                    else SetScrollPos(m_nScrollPos + m_rcItem.top - m_rcItem.bottom);
                }
                else if ( event.ptMouse.y > m_rcThumb.bottom ){
                    if( m_pOwner != NULL ) m_pOwner->PageDown(); 
                    else SetScrollPos(m_nScrollPos - m_rcItem.top + m_rcItem.bottom);                    
                }
            }
            else {
                if( event.ptMouse.x < m_rcThumb.left ) {
                    if( m_pOwner != NULL ) m_pOwner->PageLeft(); 
                    else SetScrollPos(m_nScrollPos + m_rcItem.left - m_rcItem.right);
                }
                else if ( event.ptMouse.x > m_rcThumb.right ){
                    if( m_pOwner != NULL ) m_pOwner->PageRight(); 
                    else SetScrollPos(m_nScrollPos - m_rcItem.left + m_rcItem.right);                    
                }
            }
        }
        return;
    }
    if( event.Type == UIEVENT_BUTTONUP )
    {
        m_nScrollRepeatDelay = 0;
        m_nLastScrollOffset = 0;
        m_pManager->KillTimer(this, DEFAULT_TIMERID);

        if( (m_uThumbState & UISTATE_CAPTURED) != 0 ) {
            m_uThumbState &= ~( UISTATE_CAPTURED | UISTATE_PUSHED );
            Invalidate();
        }
        else if( (m_uButton1State & UISTATE_PUSHED) != 0 ) {
            m_uButton1State &= ~UISTATE_PUSHED;
            Invalidate();
        }
        else if( (m_uButton2State & UISTATE_PUSHED) != 0 ) {
            m_uButton2State &= ~UISTATE_PUSHED;
            Invalidate();
        }
        return;
    }
    if( event.Type == UIEVENT_MOUSEMOVE )
    {
        if( (m_uThumbState & UISTATE_CAPTURED) != 0 ) {
            if( !m_bHorizontal ) {
                 m_nLastScrollOffset = (event.ptMouse.y - ptLastMouse.y) * m_nRange / \
                    (m_rcItem.bottom - m_rcItem.top - m_rcThumb.bottom + m_rcThumb.top - 2 * m_cxyFixed.cx);
            }
            else {
                m_nLastScrollOffset = (event.ptMouse.x - ptLastMouse.x) * m_nRange / \
                    (m_rcItem.right - m_rcItem.left - m_rcThumb.right + m_rcThumb.left - 2 * m_cxyFixed.cy);
            }
        }
        else {
            if( (m_uThumbState & UISTATE_HOT) != 0 ) {
                if( !::PtInRect(&m_rcThumb, event.ptMouse) ) {
                    m_uThumbState &= ~UISTATE_HOT;
                    Invalidate();
                }
            }
            else {
                if( !IsEnabled() ) return;
                if( ::PtInRect(&m_rcThumb, event.ptMouse) ) {
                    m_uThumbState |= UISTATE_HOT;
                    Invalidate();
                }
            }
        }
        return;
    }
    if( event.Type == UIEVENT_TIMER && event.wParam == DEFAULT_TIMERID )
    {
        ++m_nScrollRepeatDelay;
        if( (m_uThumbState & UISTATE_CAPTURED) != 0 ) {
            if( !m_bHorizontal ) {
                if( m_pOwner != NULL ) m_pOwner->SetScrollPos(CSize(m_pOwner->GetScrollPos().cx, \
                    m_nLastScrollPos + m_nLastScrollOffset)); 
                else SetScrollPos(m_nLastScrollPos + m_nLastScrollOffset);
            }
            else {
                if( m_pOwner != NULL ) m_pOwner->SetScrollPos(CSize(m_nLastScrollPos + m_nLastScrollOffset, \
                    m_pOwner->GetScrollPos().cy)); 
                else SetScrollPos(m_nLastScrollPos + m_nLastScrollOffset);
            }
            Invalidate();
        }
        else if( (m_uButton1State & UISTATE_PUSHED) != 0 ) {
            if( m_nScrollRepeatDelay <= 5 ) return;
            if( !m_bHorizontal ) {
                if( m_pOwner != NULL ) m_pOwner->LineUp(); 
                else SetScrollPos(m_nScrollPos - m_nLineSize);
            }
            else {
                if( m_pOwner != NULL ) m_pOwner->LineLeft(); 
                else SetScrollPos(m_nScrollPos - m_nLineSize);
            }
        }
        else if( (m_uButton2State & UISTATE_PUSHED) != 0 ) {
            if( m_nScrollRepeatDelay <= 5 ) return;
            if( !m_bHorizontal ) {
                if( m_pOwner != NULL ) m_pOwner->LineDown(); 
                else SetScrollPos(m_nScrollPos + m_nLineSize);
            }
            else {
                if( m_pOwner != NULL ) m_pOwner->LineRight(); 
                else SetScrollPos(m_nScrollPos + m_nLineSize);
            }
        }
        else {
            if( m_nScrollRepeatDelay <= 5 ) return;
            POINT pt = { 0 };
            ::GetCursorPos(&pt);
            ::ScreenToClient(m_pManager->GetPaintWindow(), &pt);
            if( !m_bHorizontal ) {
                if( pt.y < m_rcThumb.top ) {
                    if( m_pOwner != NULL ) m_pOwner->PageUp(); 
                    else SetScrollPos(m_nScrollPos + m_rcItem.top - m_rcItem.bottom);
                }
                else if ( pt.y > m_rcThumb.bottom ){
                    if( m_pOwner != NULL ) m_pOwner->PageDown(); 
                    else SetScrollPos(m_nScrollPos - m_rcItem.top + m_rcItem.bottom);                    
                }
            }
            else {
                if( pt.x < m_rcThumb.left ) {
                    if( m_pOwner != NULL ) m_pOwner->PageLeft(); 
                    else SetScrollPos(m_nScrollPos + m_rcItem.left - m_rcItem.right);
                }
                else if ( pt.x > m_rcThumb.right ){
                    if( m_pOwner != NULL ) m_pOwner->PageRight(); 
                    else SetScrollPos(m_nScrollPos - m_rcItem.left + m_rcItem.right);                    
                }
            }
        }
        return;
    }
    if( event.Type == UIEVENT_MOUSEENTER )
    {
        if( IsEnabled() ) {
            m_uButton1State |= UISTATE_HOT;
            m_uButton2State |= UISTATE_HOT;
            if( ::PtInRect(&m_rcThumb, event.ptMouse) ) m_uThumbState |= UISTATE_HOT;
            Invalidate();
        }
        return;
    }
    if( event.Type == UIEVENT_MOUSELEAVE )
    {
        if( IsEnabled() ) {
            m_uButton1State &= ~UISTATE_HOT;
            m_uButton2State &= ~UISTATE_HOT;
            m_uThumbState &= ~UISTATE_HOT;
            Invalidate();
        }
        return;
    }

    if( m_pOwner != NULL ) m_pOwner->Event(event); else CControlUI::Event(event);
}

void CScrollbarUI::SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue)
{
    if( _tcscmp(pstrName, _T("button1normalimage")) == 0 ) SetButton1NormalImage(pstrValue);
    else if( _tcscmp(pstrName, _T("button1hotimage")) == 0 ) SetButton1HotImage(pstrValue);
    else if( _tcscmp(pstrName, _T("button1pushedimage")) == 0 ) SetButton1PushedImage(pstrValue);
    else if( _tcscmp(pstrName, _T("button1disabledimage")) == 0 ) SetButton1DisabledImage(pstrValue);
    else if( _tcscmp(pstrName, _T("button2normalimage")) == 0 ) SetButton2NormalImage(pstrValue);
    else if( _tcscmp(pstrName, _T("button2hotimage")) == 0 ) SetButton2HotImage(pstrValue);
    else if( _tcscmp(pstrName, _T("button2pushedimage")) == 0 ) SetButton2PushedImage(pstrValue);
    else if( _tcscmp(pstrName, _T("button2disabledimage")) == 0 ) SetButton2DisabledImage(pstrValue);
    else if( _tcscmp(pstrName, _T("thumbnormalimage")) == 0 ) SetThumbNormalImage(pstrValue);
    else if( _tcscmp(pstrName, _T("thumbhotimage")) == 0 ) SetThumbHotImage(pstrValue);
    else if( _tcscmp(pstrName, _T("thumbpushedimage")) == 0 ) SetThumbPushedImage(pstrValue);
    else if( _tcscmp(pstrName, _T("thumbdisabledimage")) == 0 ) SetThumbDisabledImage(pstrValue);
    else if( _tcscmp(pstrName, _T("railnormalimage")) == 0 ) SetRailNormalImage(pstrValue);
    else if( _tcscmp(pstrName, _T("railhotimage")) == 0 ) SetRailHotImage(pstrValue);
    else if( _tcscmp(pstrName, _T("railpushedimage")) == 0 ) SetRailPushedImage(pstrValue);
    else if( _tcscmp(pstrName, _T("raildisabledimage")) == 0 ) SetRailDisabledImage(pstrValue);
    else if( _tcscmp(pstrName, _T("bknormalimage")) == 0 ) SetBkNormalImage(pstrValue);
    else if( _tcscmp(pstrName, _T("bkhotimage")) == 0 ) SetBkHotImage(pstrValue);
    else if( _tcscmp(pstrName, _T("bkpushedimage")) == 0 ) SetBkPushedImage(pstrValue);
    else if( _tcscmp(pstrName, _T("bkdisabledimage")) == 0 ) SetBkDisabledImage(pstrValue);
    else if( _tcscmp(pstrName, _T("hor")) == 0 ) SetHorizontal(_tcscmp(pstrValue, _T("true")) == 0);
    else if( _tcscmp(pstrName, _T("linesize")) == 0 ) SetLineSize(_ttoi(pstrValue));
    else if( _tcscmp(pstrName, _T("range")) == 0 ) SetScrollRange(_ttoi(pstrValue));
    else if( _tcscmp(pstrName, _T("value")) == 0 ) SetScrollPos(_ttoi(pstrValue));
    else CControlUI::SetAttribute(pstrName, pstrValue);
}

void CScrollbarUI::DoPaint(HDC hDC, const RECT& rcPaint)
{
    if( !::IntersectRect(&m_rcPaint, &rcPaint, &m_rcItem) ) return;
    PaintBk(hDC);
    PaintButton1(hDC);
    PaintButton2(hDC);
    PaintThumb(hDC);
    PaintRail(hDC);
}

void CScrollbarUI::PaintBk(HDC hDC)
{
    if( !IsEnabled() ) m_uThumbState |= UISTATE_DISABLED;
    else m_uThumbState &= ~ UISTATE_DISABLED;

    if( (m_uThumbState & UISTATE_DISABLED) != 0 ) {
        if( !m_sBkDisabledImage.IsEmpty() ) {
            if( !DrawImage(hDC, (LPCTSTR)m_sBkDisabledImage) ) m_sBkDisabledImage.Empty();
            else return;
        }
    }
    else if( (m_uThumbState & UISTATE_PUSHED) != 0 ) {
        if( !m_sBkPushedImage.IsEmpty() ) {
            if( !DrawImage(hDC, (LPCTSTR)m_sBkPushedImage) ) m_sBkPushedImage.Empty();
            else return;
        }
    }
    else if( (m_uThumbState & UISTATE_HOT) != 0 ) {
        if( !m_sBkHotImage.IsEmpty() ) {
            if( !DrawImage(hDC, (LPCTSTR)m_sBkHotImage) ) m_sBkHotImage.Empty();
            else return;
        }
    }

    if( !m_sBkNormalImage.IsEmpty() ) {
        if( !DrawImage(hDC, (LPCTSTR)m_sBkNormalImage) ) m_sBkNormalImage.Empty();
        else return;
    }
}

void CScrollbarUI::PaintButton1(HDC hDC)
{
    if( !IsEnabled() ) m_uButton1State |= UISTATE_DISABLED;
    else m_uButton1State &= ~ UISTATE_DISABLED;

    m_sImageModify.Empty();
    m_sImageModify.SmallFormat(_T("dest='%d,%d,%d,%d'"), m_rcButton1.left - m_rcItem.left, \
        m_rcButton1.top - m_rcItem.top, m_rcButton1.right - m_rcItem.left, m_rcButton1.bottom - m_rcItem.top);

    if( (m_uButton1State & UISTATE_DISABLED) != 0 ) {
        if( !m_sButton1DisabledImage.IsEmpty() ) {
            if( !DrawImage(hDC, (LPCTSTR)m_sButton1DisabledImage, (LPCTSTR)m_sImageModify) ) m_sButton1DisabledImage.Empty();
            else return;
        }
    }
    else if( (m_uButton1State & UISTATE_PUSHED) != 0 ) {
        if( !m_sButton1PushedImage.IsEmpty() ) {
            if( !DrawImage(hDC, (LPCTSTR)m_sButton1PushedImage, (LPCTSTR)m_sImageModify) ) m_sButton1PushedImage.Empty();
            else return;
        }
    }
    else if( (m_uButton1State & UISTATE_HOT) != 0 ) {
        if( !m_sButton1HotImage.IsEmpty() ) {
            if( !DrawImage(hDC, (LPCTSTR)m_sButton1HotImage, (LPCTSTR)m_sImageModify) ) m_sButton1HotImage.Empty();
            else return;
        }
    }

    if( !m_sButton1NormalImage.IsEmpty() ) {
        if( !DrawImage(hDC, (LPCTSTR)m_sButton1NormalImage, (LPCTSTR)m_sImageModify) ) m_sButton1NormalImage.Empty();
        else return;
    }

    DWORD dwBorderColor = 0xFF85E4FF;
    int nBorderSize = 2;
    CRenderEngine::DrawRect(hDC, m_rcButton1, nBorderSize, dwBorderColor);
}

void CScrollbarUI::PaintButton2(HDC hDC)
{
    if( !IsEnabled() ) m_uButton2State |= UISTATE_DISABLED;
    else m_uButton2State &= ~ UISTATE_DISABLED;

    m_sImageModify.Empty();
    m_sImageModify.SmallFormat(_T("dest='%d,%d,%d,%d'"), m_rcButton2.left - m_rcItem.left, \
        m_rcButton2.top - m_rcItem.top, m_rcButton2.right - m_rcItem.left, m_rcButton2.bottom - m_rcItem.top);

    if( (m_uButton2State & UISTATE_DISABLED) != 0 ) {
        if( !m_sButton2DisabledImage.IsEmpty() ) {
            if( !DrawImage(hDC, (LPCTSTR)m_sButton2DisabledImage, (LPCTSTR)m_sImageModify) ) m_sButton2DisabledImage.Empty();
            else return;
        }
    }
    else if( (m_uButton2State & UISTATE_PUSHED) != 0 ) {
        if( !m_sButton2PushedImage.IsEmpty() ) {
            if( !DrawImage(hDC, (LPCTSTR)m_sButton2PushedImage, (LPCTSTR)m_sImageModify) ) m_sButton2PushedImage.Empty();
            else return;
        }
    }
    else if( (m_uButton2State & UISTATE_HOT) != 0 ) {
        if( !m_sButton2HotImage.IsEmpty() ) {
            if( !DrawImage(hDC, (LPCTSTR)m_sButton2HotImage, (LPCTSTR)m_sImageModify) ) m_sButton2HotImage.Empty();
            else return;
        }
    }

    if( !m_sButton2NormalImage.IsEmpty() ) {
        if( !DrawImage(hDC, (LPCTSTR)m_sButton2NormalImage, (LPCTSTR)m_sImageModify) ) m_sButton2NormalImage.Empty();
        else return;
    }

    DWORD dwBorderColor = 0xFF85E4FF;
    int nBorderSize = 2;
    CRenderEngine::DrawRect(hDC, m_rcButton2, nBorderSize, dwBorderColor);
}

void CScrollbarUI::PaintThumb(HDC hDC)
{
    if( m_rcThumb.left == 0 && m_rcThumb.top == 0 && m_rcThumb.right == 0 && m_rcThumb.bottom == 0 ) return;
    if( !IsEnabled() ) m_uThumbState |= UISTATE_DISABLED;
    else m_uThumbState &= ~ UISTATE_DISABLED;

    m_sImageModify.Empty();
    m_sImageModify.SmallFormat(_T("dest='%d,%d,%d,%d'"), m_rcThumb.left - m_rcItem.left, \
        m_rcThumb.top - m_rcItem.top, m_rcThumb.right - m_rcItem.left, m_rcThumb.bottom - m_rcItem.top);

    if( (m_uThumbState & UISTATE_DISABLED) != 0 ) {
        if( !m_sThumbDisabledImage.IsEmpty() ) {
            if( !DrawImage(hDC, (LPCTSTR)m_sThumbDisabledImage, (LPCTSTR)m_sImageModify) ) m_sThumbDisabledImage.Empty();
            else return;
        }
    }
    else if( (m_uThumbState & UISTATE_PUSHED) != 0 ) {
        if( !m_sThumbPushedImage.IsEmpty() ) {
            if( !DrawImage(hDC, (LPCTSTR)m_sThumbPushedImage, (LPCTSTR)m_sImageModify) ) m_sThumbPushedImage.Empty();
            else return;
        }
    }
    else if( (m_uThumbState & UISTATE_HOT) != 0 ) {
        if( !m_sThumbHotImage.IsEmpty() ) {
            if( !DrawImage(hDC, (LPCTSTR)m_sThumbHotImage, (LPCTSTR)m_sImageModify) ) m_sThumbHotImage.Empty();
            else return;
        }
    }

    if( !m_sThumbNormalImage.IsEmpty() ) {
        if( !DrawImage(hDC, (LPCTSTR)m_sThumbNormalImage, (LPCTSTR)m_sImageModify) ) m_sThumbNormalImage.Empty();
        else return;
    }

    DWORD dwBorderColor = 0xFF85E4FF;
    int nBorderSize = 2;
    CRenderEngine::DrawRect(hDC, m_rcThumb, nBorderSize, dwBorderColor);
}

void CScrollbarUI::PaintRail(HDC hDC)
{
    if( m_rcThumb.left == 0 && m_rcThumb.top == 0 && m_rcThumb.right == 0 && m_rcThumb.bottom == 0 ) return;
    if( !IsEnabled() ) m_uThumbState |= UISTATE_DISABLED;
    else m_uThumbState &= ~ UISTATE_DISABLED;

    m_sImageModify.Empty();
    if( !m_bHorizontal ) {
        m_sImageModify.SmallFormat(_T("dest='%d,%d,%d,%d'"), m_rcThumb.left - m_rcItem.left, \
            (m_rcThumb.top + m_rcThumb.bottom) / 2 - m_rcItem.top - m_cxyFixed.cx / 2, \
            m_rcThumb.right - m_rcItem.left, \
            (m_rcThumb.top + m_rcThumb.bottom) / 2 - m_rcItem.top + m_cxyFixed.cx - m_cxyFixed.cx / 2);
    }
    else {
        m_sImageModify.SmallFormat(_T("dest='%d,%d,%d,%d'"), \
            (m_rcThumb.left + m_rcThumb.right) / 2 - m_rcItem.left - m_cxyFixed.cy / 2, \
            m_rcThumb.top - m_rcItem.top, \
            (m_rcThumb.left + m_rcThumb.right) / 2 - m_rcItem.left + m_cxyFixed.cy - m_cxyFixed.cy / 2, \
            m_rcThumb.bottom - m_rcItem.top);
    }

    if( (m_uThumbState & UISTATE_DISABLED) != 0 ) {
        if( !m_sRailDisabledImage.IsEmpty() ) {
            if( !DrawImage(hDC, (LPCTSTR)m_sRailDisabledImage, (LPCTSTR)m_sImageModify) ) m_sRailDisabledImage.Empty();
            else return;
        }
    }
    else if( (m_uThumbState & UISTATE_PUSHED) != 0 ) {
        if( !m_sRailPushedImage.IsEmpty() ) {
            if( !DrawImage(hDC, (LPCTSTR)m_sRailPushedImage, (LPCTSTR)m_sImageModify) ) m_sRailPushedImage.Empty();
            else return;
        }
    }
    else if( (m_uThumbState & UISTATE_HOT) != 0 ) {
        if( !m_sRailHotImage.IsEmpty() ) {
            if( !DrawImage(hDC, (LPCTSTR)m_sRailHotImage, (LPCTSTR)m_sImageModify) ) m_sRailHotImage.Empty();
            else return;
        }
    }

    if( !m_sRailNormalImage.IsEmpty() ) {
        if( !DrawImage(hDC, (LPCTSTR)m_sRailNormalImage, (LPCTSTR)m_sImageModify) ) m_sRailNormalImage.Empty();
        else return;
    }
}


void CEditUI::SetNumOnly(BOOL bNumOnly)
{
	if( bNumOnly == m_bNumOnly ) return;
	m_bNumOnly = bNumOnly;
	if( m_pWindow != NULL )
	{
		LONG dwStyle = GetWindowLong( *m_pWindow, GWL_STYLE);
		::SetWindowLong(*m_pWindow, GWL_STYLE, dwStyle|ES_NUMBER);
	}
	Invalidate();
}

BOOL CEditUI::GetNumOnly(void)
{
	return m_bNumOnly;
}

}
