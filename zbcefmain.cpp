
#include <windows.h>

#include "zbCefApp.h"
#include "zbCefHandler.h"

#include "zbBaseWindow.h"
#include "zbWidget.h"
#include "zbBrowser.h"


class zbMainWin: public zbBaseWindow<zbMainWin> {
public:
    LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam) override;
    BOOL Create(PCWSTR lpWindowName, HWND hWndParent, int nWidth, int nHeight, WNDCLASSEX *wc=NULL);
protected:
    zbBrowser *zbb;

    void afterCreate() override {};
public:
    zbMainWin(HWND hParent): zbBaseWindow() { setParentHandle(hParent); };
    virtual ~zbMainWin() {};
};

BOOL zbMainWin::Create(PCWSTR lpWindowName, HWND hWndParent, int nWidth, int nHeight, WNDCLASSEX *wc) {
    // Create and show window in center of the primary screen.
    bool re = zbBaseWindow::Create(wc, lpWindowName, WS_CAPTION | WS_SYSMENU | WS_EX_APPWINDOW, 0, 0, 0, nWidth, nHeight, hWndParent, 0);

    // show window
    ShowWindow(Window(), SW_SHOW);
    UpdateWindow(Window());
    return re;
}

LRESULT zbMainWin::HandleMessage(UINT message, WPARAM w_param, LPARAM l_param)
{
    LRESULT result (0);
    switch (message) {
    case WM_CREATE: {
        std::wstring url = L"https://www.google.com";
        zbb = new zbBrowser(Window(), url);
        }
        break;
    case WM_SIZE:
        zbb->onSize(Window(), w_param, l_param);
        break;
    case WM_CLOSE:
        PostQuitMessage(0);
        break;
    case WM_DESTROY:
        delete zbb;
        break;
    default:
        result = DefWindowProc(Window(), message, w_param, l_param);
        break;
    }

    return result;
}


// Program entry point function.
int APIENTRY wWinMain (HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow)
{
    UNREFERENCED_PARAMETER (hPrevInstance);
    UNREFERENCED_PARAMETER (lpCmdLine);

    zbSetProcessDPIAware(); // by system

    // create main window.
    zbMainWin *mwin = new zbMainWin(NULL);
    mwin->Create(L"cef wrap", NULL, 800, 600);

    MSG msg;
    while (GetMessage (&msg, NULL, 0, 0)) {
        TranslateMessage (&msg);
        DispatchMessage (&msg);
    }

    int result = static_cast<int>(msg.wParam);

    // disabled due to https://code.google.com/p/chromiumembedded/issues/detail?id=755
    // CefShutdown ();

    return result;
}
