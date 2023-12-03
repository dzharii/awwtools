## with C++ 17 best practices in mind, how do I pass object (application data) to WndProc?

To pass an object (application data) to WndProc in C++17, you can pass the original object to CreateWindow/Ex() directly and then have the WndProc call SetWindowLong() while the window is being created. That way, GWL_USERDATA is ready for subsequent messages, and you don't need to use a custom INIT message¹.

I hope this helps! Let me know if you have any other questions.

Source: Conversation with Bing, 4/8/2023(1) c++ - How to use WndProc as a class function - Stack Overflow. https://stackoverflow.com/questions/21369256/how-to-use-wndproc-as-a-class-function Accessed 4/8/2023.
(2) Passing and Returning Objects in C++ - GeeksforGeeks. https://www.geeksforgeeks.org/passing-and-returning-objects-in-c/ Accessed 4/8/2023.
(3) c# - How to handle WndProc messages in WPF? - Stack Overflow. https://stackoverflow.com/questions/624367/how-to-handle-wndproc-messages-in-wpf Accessed 4/8/2023.
(4) c++ - Use object method as WinApi WndProc callback - Stack Overflow. https://stackoverflow.com/questions/14661865/use-object-method-as-winapi-wndproc-callback Accessed 4/8/2023.

## Example
Here's an example in C++17:

```cpp
#include <Windows.h>

class MyClass {
public:
    MyClass() = default;
    ~MyClass() = default;

    void SetData(int data) { data_ = data; }
    int GetData() const { return data_; }

private:
    int data_{};
};

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
        case WM_CREATE: {
            CREATESTRUCT* create_struct = reinterpret_cast<CREATESTRUCT*>(lParam);
            MyClass* my_class = reinterpret_cast<MyClass*>(create_struct->lpCreateParams);
            SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(my_class));
            break;
        }
        case WM_DESTROY:
            PostQuitMessage(0);
            break;
        default:
            return DefWindowProc(hwnd, msg, wParam, lParam);
    }
    return 0;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    const char* class_name = "MyClass";
    WNDCLASSEX wcex{};
    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.hInstance = hInstance;
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.lpszClassName = class_name;
    RegisterClassEx(&wcex);

    MyClass my_class{};
    my_class.SetData(42);

    HWND hwnd = CreateWindowEx(
        0,
        class_name,
        "My Window",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        nullptr,
        nullptr,
        hInstance,
        &my_class);

    if (!hwnd) {
        return -1;
    }

    ShowWindow(hwnd, nCmdShow);

    MSG msg{};
    while (GetMessage(&msg, nullptr, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return static_cast<int>(msg.wParam);
}
```

In this example code snippet, the `MyClass` object is passed as the `lpParam` parameter to the `CreateWindowEx()` function. The `lpParam` parameter is then passed to the `WndProc()` function via the `CREATESTRUCT` structure. The `SetWindowLongPtr()` function is called with the `GWL_USERDATA` flag to store the pointer to the `MyClass` object in the window's user data.

I hope this helps! Let me know if you have any other questions.
