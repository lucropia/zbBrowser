#pragma once

#include "include/cef_client.h"
#include "include/wrapper/cef_helpers.h"

#include <windows.h>

class zbCefHandler : public CefClient,
                     public CefContextMenuHandler,
                     public CefDisplayHandler,
                     public CefDownloadHandler,
                     public CefDragHandler,
                     public CefKeyboardHandler,
                     public CefLifeSpanHandler,
                     public CefLoadHandler,
                     public CefRequestHandler {
public:
    zbCefHandler();
    virtual ~zbCefHandler();
    CefRefPtr<CefBrowser> GetBrowser();

#pragma region CefClient
    // since we are letting the base implementations handle all of the heavy lifting,
    // these functions just return the this pointer
    virtual CefRefPtr<CefContextMenuHandler> GetContextMenuHandler() OVERRIDE;
    virtual CefRefPtr<CefDisplayHandler> GetDisplayHandler() OVERRIDE;
    virtual CefRefPtr<CefDownloadHandler> GetDownloadHandler() OVERRIDE;
    virtual CefRefPtr<CefDragHandler> GetDragHandler() OVERRIDE;
    virtual CefRefPtr<CefKeyboardHandler> GetKeyboardHandler() OVERRIDE;
    virtual CefRefPtr<CefLifeSpanHandler> GetLifeSpanHandler() OVERRIDE;
    virtual CefRefPtr<CefLoadHandler> GetLoadHandler() OVERRIDE;
    virtual CefRefPtr<CefRequestHandler> GetRequestHandler() OVERRIDE;
#pragma endregion // CefClient

#pragma region CefDownloadHandler
    // This function is virtual and must be implemented;
    // We do nothing in it, so downloading files won't work as the callback function isn't invoked
    virtual void OnBeforeDownload(CefRefPtr<CefBrowser> browser,
                                  CefRefPtr<CefDownloadItem> download_item,
                                  const CefString& suggested_name,
                                  CefRefPtr<CefBeforeDownloadCallback> callback) OVERRIDE;
#pragma endregion // CefDownloadHandler

#pragma region CefLifeSpanHandler
    // cache a reference to the browser
    virtual void OnAfterCreated(CefRefPtr<CefBrowser> browser) OVERRIDE;
    // release the browser reference
    virtual void OnBeforeClose(CefRefPtr<CefBrowser> browser) OVERRIDE;
#pragma endregion // CefLifeSpanHandler

#if 0
#pragma region CefRequestHandler
    // CefRequestHandler methods:
    virtual bool GetAuthCredentials(CefRefPtr<CefBrowser> browser,
                                    const CefString& origin_url,
                                    bool isProxy,
                                    const CefString& host,
                                    int port,
                                    const CefString& realm,
                                    const CefString& scheme,
                                    CefRefPtr<CefAuthCallback> callback) OVERRIDE;
#pragma endregion // CefRequestHandler
#endif // 0

// Platform-specific implementation.
void PlatformTitleChange(CefRefPtr<CefBrowser> browser,
                         const CefString& title);
protected:
    // the browser reference
    CefRefPtr<CefBrowser> browser_;

    // Include the default reference counting implementation.
    IMPLEMENT_REFCOUNTING(zbCefHandler);
};
