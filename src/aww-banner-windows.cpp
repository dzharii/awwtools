#include <windows.h>
#include "aww-banner.hpp"
#include "aww-common.hpp"

#include <iostream>
#include <stdexcept>

namespace aww::banner
{

  // Defines where to place the notification window
  enum class NotificationPosition
  {
    TopLeft,
    TopRight,
    BottomLeft,
    BottomRight
  };

  // ValueObject. Holds X and Y coordinates
  // invariants:
  // X and Y must be greater than 0
  // add accessors
  // validation in constructor
  // validate X and Y separately
  class DesktopCoordinates
  {
  public:
    DesktopCoordinates(int x, int y) : m_x(x), m_y(y)
    {
      if (x < 0)
      {
        throw std::invalid_argument("X must be greater than 0");
      }
      if (y < 0)
      {
        throw std::invalid_argument("Y must be greater than 0");
      }
    }
    int getX() const
    {
      return m_x;
    }
    int getY() const
    {
      return m_y;
    }

  private:
    int m_x;
    int m_y;
  };

  DesktopCoordinates getDesktopCoordinates(NotificationPosition position, int windowWidth, int windowHeight)
  {
    int x = 0;
    int y = 0;

    RECT workArea = {0};
    SystemParametersInfo(SPI_GETWORKAREA, 0, &workArea, 0);

    switch (position)
    {
    case NotificationPosition::TopLeft:
    {
      x = workArea.left;
      y = workArea.top;
    }
    break;
    case NotificationPosition::TopRight:
    {
      x = workArea.right - windowWidth;
      y = workArea.top;
    }
    break;
    case NotificationPosition::BottomLeft:
    {
      x = workArea.left;
      y = workArea.bottom - windowHeight;
    }
    break;
    case NotificationPosition::BottomRight:
    {
      x = workArea.right - windowWidth;
      y = workArea.bottom - windowHeight;
    }
    break;
    }

    return DesktopCoordinates(x, y);
  }

  // Oh, passing state is not that simple
  // Managing Application State - Win32 apps
  // https://learn.microsoft.com/en-us/windows/win32/learnwin32/managing-application-state-?redirectedfrom=MSDN
  //

  static LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
  {
    static std::string titleText = "Hello World";
    static std::string messageText = "Hello World";
    static unsigned char transparency = 255;
    static int skipCycles = 20;
    switch (msg)
    {
    case WM_CREATE:
    {
      CREATESTRUCT *create_struct = reinterpret_cast<CREATESTRUCT *>(lParam);
      NotificationDialogBox *my_class = reinterpret_cast<NotificationDialogBox *>(create_struct->lpCreateParams);
      SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(my_class));
      std::cout << "Title: " << my_class->getTitle() << "\n";
    }
    break;
    case WM_PAINT:
    {
      NotificationDialogBox *my_class = reinterpret_cast<NotificationDialogBox *>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
      titleText = my_class->getTitle();
      messageText = my_class->getMessage();

      std::cout << "WM_PAINT Title: " << my_class->getTitle() << "\n";
      std::cout << "WM_PAINT Message: " << my_class->getMessage() << "\n";

      PAINTSTRUCT ps;
      HDC hdc = BeginPaint(hwnd, &ps);
      TextOut(hdc, 0, 0, titleText.c_str(), static_cast<int>(titleText.length()));
      TextOut(hdc, 0, 16, messageText.c_str(), static_cast<int>(messageText.length()));
      EndPaint(hwnd, &ps);

      SetLayeredWindowAttributes(
          hwnd,
          RGB(255, 255, 255),
          transparency,
          LWA_ALPHA);
    }
    break;
    case WM_TIMER:
    {
      if (skipCycles > 0)
      {
        skipCycles--;
        break;
      }
      else
      {
        transparency -= 5;
        if (transparency <= 0)
        {
          KillTimer(hwnd, NULL);
          DestroyWindow(hwnd);
        }
      }

      InvalidateRect(hwnd, NULL, TRUE);
    }
    break;
    case WM_DESTROY:
    {
      PostQuitMessage(0);
    }
    break;
    default:
      return DefWindowProc(hwnd, msg, wParam, lParam);
    }
    return 0;
  }

  void NotificationDialogBox::show()
  {
    HINSTANCE hInstance = GetModuleHandle(NULL);

    WNDCLASS wc = {0};
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInstance;
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wc.lpszClassName = "myWindowClass";
    RegisterClass(&wc);

    // Create a window
    const HWND hwnd = CreateWindowEx(
        WS_EX_CLIENTEDGE | WS_EX_LAYERED,
        "myWindowClass",
        title.c_str(),
        WS_POPUP | WS_VISIBLE | WS_BORDER,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        300,
        200,
        NULL,
        NULL,
        hInstance,
        this);

    // Set the background color of the title
    const COLORREF titleBg = RGB(
        titleBackgroundColor.get_red(),
        titleBackgroundColor.get_green(),
        titleBackgroundColor.get_blue());

    const HBRUSH brush = CreateSolidBrush(titleBg);
    SendMessage(hwnd, WM_CTLCOLORSTATIC, (WPARAM)GetDC(hwnd), (LPARAM)brush);

    // Set the foreground color of the title
    const HFONT font = CreateFont(
        20, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_OUTLINE_PRECIS,
        CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY, VARIABLE_PITCH, TEXT("Arial"));

    SendMessage(hwnd, WM_SETFONT, (WPARAM)font, TRUE);
    SendMessage(hwnd, WM_SETTEXT, 0, (LPARAM)title.c_str());

    ShowWindow(hwnd, SW_SHOW);

    SetTimer(hwnd, NULL, 100, NULL);

    MSG msg = {0};
    while (GetMessage(&msg, NULL, 0, 0))
    {
      TranslateMessage(&msg);
      DispatchMessage(&msg);
    }

    DestroyWindow(hwnd);
  }
}
