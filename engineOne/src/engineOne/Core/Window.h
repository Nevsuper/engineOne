// Window.h
#pragma once
#include <WindowsLean.h>
#include <string>
#include "Input/Input.h"

class Window
{
public: 
    Window(Input& input,HINSTANCE hInstance, const std::string& windowClassName, const std::string& title, int width, int height, DWORD windowStyle = WS_OVERLAPPEDWINDOW | WS_VISIBLE);
    ~Window() noexcept;

    void ProcessMessages() noexcept;
    bool ShouldClose() const noexcept { return m_ShouldClose; }

    HDC GetDeviceContext() const noexcept  { return GetDC(m_hWnd); }
    int GetWidth() const noexcept  { return m_Width; }
	int GetHeight() const noexcept  { return mHeight; }
	float GetAspectRatio() const noexcept { return m_AspectRatio; }

	void Close() noexcept { m_ShouldClose = true; }

	bool isCreated() const noexcept { return m_IsCreated; }
    explicit operator bool() const noexcept { return m_IsCreated; }
    static LRESULT CALLBACK StaticWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
private:
    LRESULT HandleMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;

	static bool DoesWindowClassExistsAndHavePrivateDCAndUsesCorrectWndProc(const std::string& className, HINSTANCE hInstance ) noexcept;
private:

    Input& m_Input;
    HINSTANCE m_hInstance;
    HWND m_hWnd;
    std::string windowTitle;
    int m_Width, mHeight;
	float m_AspectRatio;
    bool m_ShouldClose;
    bool m_IsCreated;
};
