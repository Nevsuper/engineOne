#include "Window.h"
#include <iostream>

int main()
{
    auto hInstance = GetModuleHandle(nullptr);
    const char* windowClassName = "MyWindowClass";

    // Check if class is already registered
    WNDCLASS existingWc;
    if (!GetClassInfo(hInstance, windowClassName, &existingWc))
    {
        // Register the class only if it doesn't exist
        WNDCLASS wc{};
        wc.hInstance = hInstance;
        wc.lpszClassName = windowClassName;
        wc.lpfnWndProc = Window::StaticWndProc;
        wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
        wc.hCursor = LoadCursor(NULL, IDC_ARROW);
        wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
        wc.style = CS_OWNDC | CS_HREDRAW | CS_VREDRAW;

        std::cout << "Starting application..." << std::endl;
        if (!RegisterClass(&wc))
        {
            std::cerr << "Failed to register window class." << std::endl;
            return -1;
        }
    }

    // Create window with a style that definitely includes a border
    DWORD style = WS_OVERLAPPEDWINDOW | WS_VISIBLE;
    Window window(hInstance, windowClassName, "Test Window", 800, 600, style);

    if (!window.isCreated())
    {
        std::cerr << "Window creation failed." << std::endl;
        return -1;
    }

    std::cout << "Window created successfully." << std::endl;

    MSG msg;
    while (!window.ShouldClose())
    {
        window.ProcessMessages();
        // Add a small delay to prevent high CPU usage
        Sleep(10);
    }

    return 0;
}