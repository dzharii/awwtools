#include <windows.h>
#include "aww-banner.hpp"
#include "aww-common.hpp"

namespace aww::banner
{
  void NotificationDialogBox::show()
  {
    // Create a window
    const HWND hwnd = CreateWindowEx(
        WS_EX_CLIENTEDGE,
        "STATIC",
        title.c_str(),
        WS_POPUP | WS_VISIBLE | WS_BORDER,
        CW_USEDEFAULT, CW_USEDEFAULT,
        300, 200,
        NULL,
        NULL,
        GetModuleHandle(NULL),
        NULL);

    // Set the background color of the title
    const COLORREF titleBg = RGB(
        titleBackgroundColor.getRed(),
        titleBackgroundColor.getGreen(),
        titleBackgroundColor.getBlue());

    const HBRUSH brush = CreateSolidBrush(titleBg);
    SendMessage(hwnd, WM_CTLCOLORSTATIC, (WPARAM)GetDC(hwnd), (LPARAM)brush);

    // Set the foreground color of the title
    const HFONT font = CreateFont(
        20, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_OUTLINE_PRECIS,
        CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY, VARIABLE_PITCH, TEXT("Arial"));

    SendMessage(hwnd, WM_SETFONT, (WPARAM)font, TRUE);
    SendMessage(hwnd, WM_SETTEXT, 0, (LPARAM)title.c_str());

    // Set the text alignment of the message ???
    // SendMessage(hwnd, WM_SETTEXTALIGN, (WPARAM)TA_CENTER | TA_TOP | TA_NOUPDATECP | TA_RTLREADING | TA_CENTER | TA_BOTTOM | TA_NOUPDATECP | TA_CENTER | TA_BASELINE | TA_NOUPDATECP | TA_LEFT | TA_UPDATECP | TA_RIGHT | TA_NOUPDATECP | TA_TOP | TA_UPDATECP | TA_BOTTOM | TA_NOUPDATECP | TA_BASELINE | TA_UPDATECP | TA_LEFT | TA_RTLREADING | TA_UPDATECP | TA_CENTER | TA_UPDATECP | TA_RIGHT | TA_UPDATECP,
    //             (LPARAM)NULL);

    // Set the text of the message
    const HWND hwndMessage = CreateWindowEx(
        WS_EX_CLIENTEDGE,
        "STATIC",
        message.c_str(),
        WS_CHILD | WS_VISIBLE,
        10,
        50,
        280,
        100,
        hwnd,
        NULL,
        GetModuleHandle(NULL),
        NULL);
    SendMessage(hwndMessage, WM_SETFONT, (WPARAM)font, TRUE);

    // Show the window
    ShowWindow(hwndMessage, SW_SHOW);
    ShowWindow(hwnd, SW_SHOW);

    // Wait for delayInSeconds seconds before closing the window
    Sleep(delayInSeconds * 1000);

    // Fade out and close the window
    for (unsigned char i = 255; i >= 0; i -= 5)
    {
      SetLayeredWindowAttributes(hwndMessage, RGB(255, 255, 255), i * 255 / 100, LWA_ALPHA);
      SetLayeredWindowAttributes(hwndMessage, RGB(255, 255, 255), i * 255 / 100, LWA_COLORKEY);
      Sleep(10);
    }

    DestroyWindow(hwnd);
  }
}
