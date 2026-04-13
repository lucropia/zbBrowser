
#include "zbBrowser.h"

#include <string>

#include "zbCefApp.h"
#include "zbCefHandler.h"

/**
 * class zbBrowser
 */
zbBrowser::zbBrowser(HWND hParent): ZBWidget(hParent) {
    _init();
    _hParent = hParent;
    _create();
}

zbBrowser::zbBrowser(HWND hParent, std::wstring &url): ZBWidget(hParent) {
    _init();
    _hParent = hParent;
    _url = url;
    _create();
}

zbBrowser::~zbBrowser() {
    delete main_args;
}

void zbBrowser::_init() {
    ZBWidget::_init();
    _title = L"ZB Browser";
    _url = L"https://google.com";
    _hInstance = GetModuleHandle(NULL);
}

void zbBrowser::beforeCreate() {
    // set High DPI support.
    CefEnableHighDPISupport(); // by cef itself.

    main_args = new CefMainArgs(GetModuleHandle(NULL));
    zb_cef_app = new zbCefApp();
    if (CefExecuteProcess(*main_args, zb_cef_app.get(), nullptr) == -1) {
        settings.multi_threaded_message_loop = true;
        CefInitialize(*main_args, settings, zb_cef_app.get (), nullptr);
    }
}

void zbBrowser::_create() {
    beforeCreate();

    // set rect of browser.
    RECT rect = {0};
    if(_x == 0 && _y == 0 && _w == 0 && _h == 0)
        GetClientRect(_hParent, &rect);
    else
        SetRect(&rect, _x, _y, _x+_w, _y+_h);

    zb_cef_handler = new zbCefHandler();

    // set as child of parent.
    CefWindowInfo info;
    info.SetAsChild(_hParent, rect);

    // create browser.
    CefBrowserSettings browserSettings;
    CefBrowserHost::CreateBrowser(info, zb_cef_handler.get(), CefString(_url), browserSettings, NULL, NULL);
    afterCreate();
}

void zbBrowser::afterCreate() {
}

void zbBrowser::setTitle(std::wstring &title) {
    _title = title;
}

void zbBrowser::setUrl(std::wstring &url) {
    _url = url;
    CefRefPtr<CefBrowser> browser = zb_cef_handler->GetBrowser();
    browser->StopLoad();
    browser->GetMainFrame()->LoadURL(url);
}

void zbBrowser::onSize(HWND hHostWin, WPARAM wParam, LPARAM lParam) {
    // from the cefclient example, do not allow the window to be resized to 0x0 or the layout will break;
    // also be aware that if the size gets too small, GPU acceleration disables
    if ((wParam != SIZE_MINIMIZED) && (zb_cef_handler.get()) && (zb_cef_handler->GetBrowser())) {
        CefWindowHandle hwnd(zb_cef_handler->GetBrowser()->GetHost()->GetWindowHandle());
        if (hwnd) {
            RECT rect = {0};
            GetClientRect(hHostWin, &rect);
            HDWP hdwp = BeginDeferWindowPos(1);
            hdwp = DeferWindowPos(hdwp, hwnd, NULL,
                                  rect.left, rect.top,
                                  rect.right - rect.left, rect.bottom - rect.top,
                                  SWP_NOZORDER);
            EndDeferWindowPos(hdwp);
        }
    }
}
