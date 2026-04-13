#pragma once

#include <windows.h>

#include "zbWidget.h"

#include "zbCefApp.h"
#include "zbCefHandler.h"

#define ZB_BROWSER_CLASS TEXT("ZB_Cef_Browser")

class zbBrowser: public ZBWidget {
public:
    CefRefPtr<zbCefHandler> zb_cef_handler;

    void setTitle(std::wstring &title);
    void setUrl(std::wstring &url);
    void onSize(HWND hHostWin, WPARAM wParam, LPARAM lParam);
protected:
    void beforeCreate(void) override;
    void afterCreate(void) override;
private:
    std::wstring _title;
    HINSTANCE _hInstance;

    void _init(void);
    void _create(void);

    std::wstring _url;
    CefRefPtr<zbCefApp> zb_cef_app;
    CefMainArgs *main_args;
    CefSettings settings;
public:
    zbBrowser(HWND hParent);
    zbBrowser(HWND hParent, std::wstring &url);
    virtual ~zbBrowser();
};
