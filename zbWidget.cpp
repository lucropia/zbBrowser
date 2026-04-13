
#include "zbWidget.h"

#include <cwchar>

int zbGetDpiForWindow(HWND hwnd) {
    HINSTANCE hdll = LoadLibrary(L"user32.dll");
    if(hdll == NULL)
        return -2; // load dll wrong

    int re = 0;
    pFun pGetDpiForWindow = (pFun)GetProcAddress(hdll, "GetDpiForWindow");
    if(pGetDpiForWindow == NULL)
        re = -3; // no function
    else
        re = pGetDpiForWindow(hwnd);

    FreeLibrary(hdll);
    return re;
}

bool zbSetProcessDPIAware() {
    HINSTANCE hdll = LoadLibrary(L"user32.dll");
    if(hdll == NULL)
        return false; // load dll wrong

    bool re = false;
    pFun pSetProcessDPIAware = (pFun)GetProcAddress(hdll, "SetProcessDPIAware");
    if(pSetProcessDPIAware != NULL)
        re = pSetProcessDPIAware(NULL);

    FreeLibrary(hdll);
    return re;
}


/**
 * clss ZBWidget
 */
ZBWidget::ZBWidget(HWND hParent) {
    this->_init();
    _hParent = hParent;
}

ZBWidget::~ZBWidget() {
}

void ZBWidget::_init() {
    _x = _y = _w = _h = 0;
    _hParent = _hSelf = NULL;
    _isShowed = false;
    _wgtid = 0;
}

void ZBWidget::_redraw(void) {
    RECT rect;
    GetClientRect(_hSelf, &rect);
    InvalidateRect(_hSelf, &rect, TRUE);
    MapWindowPoints(_hSelf, _hParent, (POINT *) &rect, 2);
    RedrawWindow(_hParent, &rect, NULL, RDW_ERASE | RDW_INVALIDATE);
}

void ZBWidget::setContour(int xx, int yy, int ww, int hh) {
    _x = xx, _y = yy, _w = ww, _h = hh;
    if(_hSelf != NULL)
        //SetWindowPos(_hSelf, NULL, _x, _y, _w, _h, _style);
        this->updateLayoutForDpi(true);
}

void ZBWidget::show() {
    if(_hSelf == NULL)
        return;
    if(! _isShowed) {
        ShowWindow(_hSelf, SW_SHOW);
        UpdateWindow(_hSelf);
        _isShowed = true;
    }
}

void ZBWidget::hide() {
    if(_hSelf == NULL)
        return;
    if(_isShowed) {
        ShowWindow(_hSelf, SW_HIDE);
        _isShowed = false;
    }
}

void ZBWidget::addStyle(DWORD style) {
    _style |= style;
}

void ZBWidget::setStyle(DWORD style) {
    _style = style;
}

void ZBWidget::setWidgetID(HMENU wid) {
    _wgtid = wid;
}

void ZBWidget::_create(const wchar_t *classname, const wchar_t *text) {
    this->beforeCreate();
    _hSelf = CreateWindowEx(0, classname, text,
                            _style,
                            _x, _y, _w, _h,
                            _hParent, (HMENU)_wgtid,
                            0, NULL);
    this->afterCreate();
    _xx=_x, _yy=_y, _ww=_w, _hh=_h;
    int btmx = _x + _w, btmy = _y + _h;

    RECT rt;
    GetClientRect(_hParent, &rt);
    if(((rt.right-rt.left) < btmx) || ((rt.bottom-rt.top) < btmy))
        SetWindowPos(_hParent, NULL, rt.left, rt.top, rt.right-rt.left, rt.bottom-rt.top, _style);

    updateLayoutForDpi(true);
    this->show();
}

void ZBWidget::setFont(const wchar_t *fontface, const int fontsize) {
    GetObject(GetStockObject(DEFAULT_GUI_FONT), sizeof(LOGFONT), &_lgfont);

    if(fontsize != 0) {
        HDC hdc = GetDC(_hSelf);
        _lgfont.lfHeight = -MulDiv(fontsize, GetDeviceCaps(hdc, LOGPIXELSY), 72);
        ReleaseDC(_hSelf, hdc);
    }

    wcscpy(_lgfont.lfFaceName, fontface);
    HFONT hFont = CreateFontIndirect(&_lgfont);

    SendMessage(_hSelf, WM_SETFONT, (WPARAM)hFont, (LPARAM)MAKELONG(true, 0));
}

void ZBWidget::updateLayoutForDpi(bool isMoveWindow=false) {
    int iDpi = zbGetDpiForWindow(_hSelf);
    if(iDpi <= 0)
        return;
    int dpiScaledX = MulDiv(_xx, iDpi, 96);
    int dpiScaledY = MulDiv(_yy, iDpi, 96);
    int dpiScaledWidth = MulDiv(_ww, iDpi, 96);
    int dpiScaledHeight = MulDiv(_hh, iDpi, 96);
    if(isMoveWindow)
        SetWindowPos(_hSelf, _hSelf, dpiScaledX, dpiScaledY, dpiScaledWidth, dpiScaledHeight, SWP_NOZORDER | SWP_NOACTIVATE);
    else{
        _x = dpiScaledX;
        _y = dpiScaledY;
        _w = dpiScaledWidth;
        _h = dpiScaledHeight;
    }
}
