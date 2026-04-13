
#ifndef __ZB_BASE_WINDOWS_H
#define __ZB_BASE_WINDOWS_H

#include <windows.h>

template <class DERIVED_TYPE> class zbBaseWindow {
public:
    int _x=0, _y=0, _w=0, _h=0;
    int _xx=0, _yy=0, _ww=0, _hh=0;
    DWORD _style=0;

    static LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
        DERIVED_TYPE *pThis = NULL;

        if (uMsg == WM_NCCREATE) {
            CREATESTRUCT* pCreate = (CREATESTRUCT*)lParam;
            pThis = (DERIVED_TYPE*)pCreate->lpCreateParams;
            SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)pThis);

            pThis->m_hwnd = hwnd;
        }
        else {
            pThis = (DERIVED_TYPE*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
        }

        if (pThis) {
            return pThis->HandleMessage(uMsg, wParam, lParam);
        }
        else {
            return DefWindowProc(hwnd, uMsg, wParam, lParam);
        }
    }

    zbBaseWindow() : m_hwnd(NULL) { }

    BOOL Create(WNDCLASSEX *zbwc,
                PCWSTR lpWindowName,
                DWORD dwStyle,
                DWORD dwExStyle = 0,
                int x = CW_USEDEFAULT,
                int y = CW_USEDEFAULT,
                int nWidth = CW_USEDEFAULT,
                int nHeight = CW_USEDEFAULT,
                HWND hWndParent = 0,
                HMENU hMenu = 0) {
        _x=x, _y=y, _w=nWidth, _h=nHeight;
        _xx=x, _yy=y, _ww=nWidth, _hh=nHeight;
        _style = dwStyle;

        if(hWndParent == 0)
            hWndParent = getParentHandle();
        else
            setParentHandle(hWndParent);

        if(zbwc == nullptr) {
            WNDCLASS wc = {0};
            wc.lpfnWndProc   = DERIVED_TYPE::WindowProc;
            wc.hInstance     = GetModuleHandle(NULL);
            wc.lpszClassName = ClassName();
            wc.hCursor       = LoadCursor(NULL, IDC_ARROW);
            RegisterClass(&wc);
        }
        else
            RegisterClassEx(zbwc);

        m_hwnd = CreateWindowEx(dwExStyle, ClassName(), lpWindowName, dwStyle,
                                _x, _y, _w, _h,
                                hWndParent, hMenu, GetModuleHandle(NULL), this
                                );

        afterCreate();
        return (m_hwnd ? TRUE : FALSE);
    }

    BOOL Create(PCWSTR lpWindowName,
                DWORD dwStyle,
                DWORD dwExStyle = 0,
                int x = CW_USEDEFAULT,
                int y = CW_USEDEFAULT,
                int nWidth = CW_USEDEFAULT,
                int nHeight = CW_USEDEFAULT,
                HWND hWndParent = 0,
                HMENU hMenu = 0,
                WNDCLASSEX *zbwc=nullptr) {
        return Create(zbwc, lpWindowName, dwStyle, dwExStyle, x, y, nWidth, nHeight, hWndParent, hMenu);
    }

    virtual void afterCreate() = 0;

    void updateLayoutForDpi(bool isMoveWindow=false) {
        int iDpi = zbGetDpiForWindow(Window());
        if(iDpi <= 0)
            return;
        int dpiScaledX = MulDiv(_xx, iDpi, 96);
        int dpiScaledY = MulDiv(_yy, iDpi, 96);
        int dpiScaledWidth = MulDiv(_ww, iDpi, 96);
        int dpiScaledHeight = MulDiv(_hh, iDpi, 96);
        if(isMoveWindow)
            SetWindowPos(Window(), Window(), dpiScaledX, dpiScaledY, dpiScaledWidth, dpiScaledHeight, SWP_NOZORDER | SWP_NOACTIVATE);
        else{
            _x = dpiScaledX;
            _y = dpiScaledY;
            _w = dpiScaledWidth;
            _h = dpiScaledHeight;
        }
    }

    void setParentHandle(HWND hwnd) { this->__hWndParent = hwnd; };
    HWND getParentHandle() const { return this->__hWndParent; };
    HWND Window() const { return m_hwnd; }

protected:
    HWND m_hwnd;
    HWND __hWndParent = 0;

    PCWSTR ClassName() { return L"ZB_BASE_WINDOW"; };
    virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam) = 0;

public:
    virtual ~zbBaseWindow() { UnregisterClass(ClassName(), GetModuleHandle(NULL)); };
};

#endif /* __ZB_BASE_WINDOWS_H */
