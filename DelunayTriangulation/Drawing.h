#ifndef __DRAWING_H__
#define __DRAWING_H__

#include <windows.h>    // Win32API Header File
#include <cstring>
#include <cstdio>

using namespace std;
#define Red  RGB(255, 0, 0)
#define Lime RGB(206, 255, 0)
#define Blue RGB(0, 0, 255)
#define White RGB(255, 255, 255)
#define Black RGB(0, 0, 0)
static HWND    hConWnd;
int     BCX_Line(HWND, int, int, int, int, int = 0, HDC = 0);
int     BCX_Circle(HWND, int, int, int, int = 0, int = 0, HDC = 0);
int     BCX_Rect(HWND, int, int, int, int = 0, int = 0, HDC = 0);
HWND    GetConsoleWndHandle(void);


int BCX_Line(HWND Wnd, int x1, int y1, int x2, int y2, int Pen, HDC DrawHDC)
{
    int a, b = 0;
    HPEN hOPen;
    // penstyle, width, color
    HPEN hNPen = CreatePen(PS_SOLID, 2, Pen);
    if (!DrawHDC) DrawHDC = GetDC(Wnd), b = 1;
    hOPen = (HPEN)SelectObject(DrawHDC, hNPen);
    // starting point of line
    MoveToEx(DrawHDC, x1, y1, NULL);
    // ending point of line
    a = LineTo(DrawHDC, x2, y2);
    DeleteObject(SelectObject(DrawHDC, hOPen));
    if (b) ReleaseDC(Wnd, DrawHDC);
    return a;
}
int BCX_Rect(HWND Wnd, int x1, int y1, int x2, int y2, int Pen, HDC DrawHDC)
{
    int a, b = 0;
    HPEN hOPen;
    // penstyle, width, color
    HPEN hNPen = CreatePen(PS_SOLID, 2, Pen);
    if (!DrawHDC) DrawHDC = GetDC(Wnd), b = 1;
    hOPen = (HPEN)SelectObject(DrawHDC, hNPen);
    // starting point of line
    MoveToEx(DrawHDC, x1, y1, NULL);
    // ending point of line

    HBRUSH hNewBrush = CreateSolidBrush(Pen);
    HBRUSH hOldBrush = (HBRUSH)SelectObject(DrawHDC, hNewBrush);

    a = Rectangle(DrawHDC, x1, y1, _In_ x2, y2);
    DeleteObject(SelectObject(DrawHDC, hOPen));
    DeleteObject(SelectObject(DrawHDC, hOldBrush));
    if (b) ReleaseDC(Wnd, DrawHDC);
    return a;
}
// converts circle(centerX,centerY,radius,pen) to WinApi function
// ellipse inside box with upper left and lower right coordinates
int BCX_Circle(HWND Wnd, int X, int Y, int R, int Pen, int Fill, HDC DrawHDC)
{
    int a, b = 0;
    if (!DrawHDC) DrawHDC = GetDC(Wnd), b = 1;
    // penstyle, width, color
    HPEN   hNPen = CreatePen(PS_SOLID, 2, Pen);
    HPEN   hOPen = (HPEN)SelectObject(DrawHDC, hNPen);
    HBRUSH hOldBrush;
    HBRUSH hNewBrush;
    // if true will fill circle with pencolor
    if (Fill)
    {
        hNewBrush = CreateSolidBrush(Pen);
        hOldBrush = (HBRUSH)SelectObject(DrawHDC, hNewBrush);
    }
    else
    {
        hNewBrush = (HBRUSH)GetStockObject(NULL_BRUSH);
        hOldBrush = (HBRUSH)SelectObject(DrawHDC, hNewBrush);
    }
    a = Ellipse(DrawHDC, X - R, Y + R, X + R, Y - R);
    DeleteObject(SelectObject(DrawHDC, hOPen));
    DeleteObject(SelectObject(DrawHDC, hOldBrush));
    if (b) ReleaseDC(Wnd, DrawHDC);
    return a;
}
// the hoop ...
HWND GetConsoleWndHandle(void)
{
    HWND hConWnd;
    OSVERSIONINFO os;
    char szTempTitle[64], szClassName[128], szOriginalTitle[1024];
    os.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
    GetVersionEx(&os);
    // may not work on WIN9x
    if (os.dwPlatformId == VER_PLATFORM_WIN32s) return 0;
    GetConsoleTitle((LPWSTR)szOriginalTitle, sizeof(szOriginalTitle));
    sprintf(szTempTitle, "%u - %u", GetTickCount(), GetCurrentProcessId());
    SetConsoleTitle((LPWSTR)szTempTitle);
    Sleep(40);
    // handle for NT
    hConWnd = FindWindow(NULL, (LPWSTR)szTempTitle);
    SetConsoleTitle((LPWSTR)szOriginalTitle);
    // may not work on WIN9x
    if (os.dwPlatformId == VER_PLATFORM_WIN32_WINDOWS)
    {
        hConWnd = GetWindow(hConWnd, GW_CHILD);
        if (hConWnd == NULL)  return 0;
        GetClassName(hConWnd, (LPWSTR)szClassName, sizeof (szClassName));
        while (strcmp(szClassName, "ttyGrab") != 0)
        {
            hConWnd = GetNextWindow(hConWnd, GW_HWNDNEXT);
            if (hConWnd == NULL)  return 0;
            GetClassName(hConWnd, (LPWSTR)szClassName, sizeof(szClassName));
        }
    }
    RECT r;
    GetWindowRect(hConWnd, &r); //stores the console's current dimensions
    MoveWindow(hConWnd, r.left, r.top, 800, 800, TRUE);
    return hConWnd;
}

#endif // __DRAWING_H__