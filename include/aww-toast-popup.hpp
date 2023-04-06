#pragma once
#ifndef AWW_TOAST_POPUP_HPP
#define AWW_TOAST_POPUP_HPP

Dmytro! TODO

#include <Windows.h>
#include <string>
#include <stdexcept>

// ValueObject
// Implement Validation
// Use C++17
class RgbaColor {
public:
    RgbaColor(int red, int green, int blue, int alpha) {
        if (red < 0 || red > 255) {
            throw std::invalid_argument("red must be between 0 and 255");
        }

        if (green < 0 || green > 255) {
            throw std::invalid_argument("green must be between 0 and 255");
        }

        if (blue < 0 || blue > 255) {
            throw std::invalid_argument("blue must be between 0 and 255");
        }

        if (alpha < 0 || alpha > 255) {
            throw std::invalid_argument("alpha must be between 0 and 255");
        }

        this->red = red;
        this->green = green;
        this->blue = blue;
        this->alpha = alpha;
    }

    int getRed() {
        return red;
    }

    int getGreen() {
        return green;
    }

    int getBlue() {
        return blue;
    }

    int getAlpha() {
        return alpha;
    }

    COLORREF toCOLORREF() {
        return RGB(red, green, blue);
    }

private:
    int red;
    int green;
    int blue;
    int alpha;

}

class NotificationDialogBox {
public:
    NotificationDialogBox(
      const std::string& title,
      const std::string& message,
      COLORREF titleBackgroundColor, COLORREF titleForegroundColor, int delayInSeconds) {
        this->title = title;
        this->message = message;
        this->titleBackgroundColor = titleBackgroundColor;
        this->titleForegroundColor = titleForegroundColor;
        this->delayInSeconds = delayInSeconds;
    }

    void show() {
        // Create a window
        HWND hwnd = CreateWindowEx(
            WS_EX_CLIENTEDGE,
            L"STATIC",
            title,
            WS_POPUP | WS_VISIBLE | WS_BORDER,
            CW_USEDEFAULT, CW_USEDEFAULT,
            300, 200,
            NULL,
            NULL,
            GetModuleHandle(NULL),
            NULL);

        // Set the background color of the title
        HBRUSH brush = CreateSolidBrush(titleBackgroundColor);
        SendMessage(hwnd, WM_CTLCOLORSTATIC, (WPARAM)GetDC(hwnd), (LPARAM)brush);

        // Set the foreground color of the title
        HFONT font = CreateFont(20, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_OUTLINE_PRECIS,
            CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY, VARIABLE_PITCH, TEXT("Arial"));
        SendMessage(hwnd, WM_SETFONT, (WPARAM)font, TRUE);
        SendMessage(hwnd, WM_SETTEXT, 0, (LPARAM)title);
        SendMessage(hwnd, WM_SETTEXTALIGN, (WPARAM)TA_CENTER | TA_TOP | TA_NOUPDATECP | TA_RTLREADING | TA_CENTER | TA_BOTTOM | TA_NOUPDATECP | TA_CENTER | TA_BASELINE | TA_NOUPDATECP | TA_LEFT | TA_UPDATECP | TA_RIGHT | TA_NOUPDATECP | TA_TOP | TA_UPDATECP | TA_BOTTOM | TA_NOUPDATECP | TA_BASELINE | TA_UPDATECP | TA_LEFT | TA_RTLREADING | TA_UPDATECP | TA_CENTER | TA_UPDATECP | TA_RIGHT | TA_UPDATECP,
            (LPARAM)NULL);

        // Set the text of the message
        HWND hwndMessage = CreateWindowEx(
            WS_EX_CLIENTEDGE,
            L"STATIC",
            message,
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
        for (int i = 255; i >= 0; i -= 5) {
            SetLayeredWindowAttributes(hwndMessage, RGB(255, 255, 255), i * 255 / 100, LWA_ALPHA);
            SetLayeredWindowAttributes(hwndMessage, RGB(255, 255, 255), i * 255 / 100, LWA_COLORKEY);
            Sleep(10);
        }

        DestroyWindow(hwnd);
    }

private:
    const wchar_t* title;
    const wchar_t* message;
    COLORREF titleBackgroundColor;
    COLORREF titleForegroundColor;
    int delayInSeconds;
};



#endif // AWW_TOAST_POPUP_HPP
