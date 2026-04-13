
#ifndef _ZB_WIDGET_H
#define _ZB_WIDGET_H

#include <string>

#include <windows.h>

typedef int (* pFun)(HWND);

int zbGetDpiForWindow(HWND hwnd);
bool zbSetProcessDPIAware();

/**
 * class ZBWidget
 */
class ZBWidget {
public:
    int _x, _y, _w, _h;
    int _xx, _yy, _ww, _hh;
    void show(void);
    void hide(void);
    void setStyle(DWORD style);
    void addStyle(DWORD style);
    HWND getHandle(void) const { return _hSelf; };
    HWND getParentHandle(void) const { return _hParent; };
    HMENU getWidgetID(void) const { return _wgtid; };
    void setWidgetID(HMENU wid);
    void updateLayoutForDpi(bool isMoveWindow);
    void setContour(int xx, int yy, int ww, int hh);
    void setFont(const wchar_t *fontface, const int fontsize=0);
    virtual void beforeCreate(void) { };
    virtual void afterCreate(void) { };
protected:
    DWORD _style = 0L;
    HWND _hParent = NULL;
    HWND _hSelf = NULL;
    bool _isShowed = false;
    HMENU _wgtid = 0L;
    LOGFONT _lgfont = {0};

    void _init(void);
    void _redraw(void);
    void _create(const wchar_t *classname, const wchar_t *text=NULL);
public:
    ZBWidget(HWND hParent);
    virtual ~ZBWidget();
};

#endif /* _ZB_WIDGET_H */
