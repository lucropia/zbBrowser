
#include "zbCefHandler.h"
#include <string>

zbCefHandler::zbCefHandler() { }
zbCefHandler::~zbCefHandler() { }

CefRefPtr<CefBrowser> zbCefHandler::GetBrowser() { return browser_; }
CefRefPtr<CefContextMenuHandler> zbCefHandler::GetContextMenuHandler() { return this; }
CefRefPtr<CefDisplayHandler> zbCefHandler::GetDisplayHandler() { return this; }
CefRefPtr<CefDownloadHandler> zbCefHandler::GetDownloadHandler() { return this; }
CefRefPtr<CefDragHandler> zbCefHandler::GetDragHandler() { return this; }
CefRefPtr<CefKeyboardHandler> zbCefHandler::GetKeyboardHandler() { return this; }
CefRefPtr<CefLifeSpanHandler> zbCefHandler::GetLifeSpanHandler() { return this; }
CefRefPtr<CefLoadHandler> zbCefHandler::GetLoadHandler() { return this; }
CefRefPtr<CefRequestHandler> zbCefHandler::GetRequestHandler() { return this; }

void zbCefHandler::OnBeforeDownload(CefRefPtr<CefBrowser> browser,
                                         CefRefPtr<CefDownloadItem> download_item,
                                         const CefString& suggested_name,
                                         CefRefPtr<CefBeforeDownloadCallback> callback) {
   UNREFERENCED_PARAMETER(browser);
   UNREFERENCED_PARAMETER(download_item);
   callback->Continue(suggested_name, true);
}

void zbCefHandler::OnAfterCreated(CefRefPtr<CefBrowser> browser) {
    CEF_REQUIRE_UI_THREAD();
    this->browser_ = browser;
    CefLifeSpanHandler::OnAfterCreated(browser);
}

void zbCefHandler::OnBeforeClose(CefRefPtr<CefBrowser> browser) {
    CEF_REQUIRE_UI_THREAD();

    browser = NULL;
    //AppQuitMessageLoop(); // --> PostQuitMessage(0);

    CefLifeSpanHandler::OnBeforeClose(browser_);
}

#if 0
bool zbCefHandler::GetAuthCredentials(CefRefPtr<CefBrowser> browser,
                                       const CefString& origin_url,
                                       bool isProxy,
                                       const CefString& host,
                                       int port,
                                       const CefString& realm,
                                       const CefString& scheme,
                                       CefRefPtr<CefAuthCallback> callback) {
    std::string user = "user";
    std::string pwd = "pwd";
    callback->Continue(user,c_str(), pwd.c_str());
    return true;
}
#endif // 0

// Platform-specific implementation.
void zbCefHandler::PlatformTitleChange(CefRefPtr<CefBrowser> browser,
                                            const CefString& title) {
    CefWindowHandle hwnd = browser->GetHost()->GetWindowHandle();
    if (hwnd) {
        std::wstring ws = title;
        SetWindowText(hwnd, (LPTSTR)ws.c_str());
    }
}

