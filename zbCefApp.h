#pragma once

#include "include/cef_app.h"

// CefApp does all of the work, but it is an abstract base class that needs reference counting implemented;
// thus, we create a dummy class that inherits off of CefApp but does nothing
class zbCefApp : public CefApp,
                 public CefBrowserProcessHandler {
public:
    zbCefApp() { }

    virtual ~zbCefApp() { }

    virtual CefRefPtr<CefBrowserProcessHandler> GetBrowserProcessHandler() OVERRIDE { return this; }

    virtual void OnBeforeCommandLineProcessing(const CefString& process_type, CefRefPtr<CefCommandLine> command_line) OVERRIDE {
        // Bypass certificate errors.
        command_line->AppendSwitch("ignore-certificate-errors");

        // Bypass google id sync sharing.
        command_line->AppendSwitch("disable-sync");
        command_line->AppendSwitchWithValue("allow-browser-signin", "false");
    };

private:
    IMPLEMENT_REFCOUNTING(zbCefApp);
};
