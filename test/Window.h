// Window.h
#pragma once
#include <Windows.h>
#include <string>
#include <optional>
class Window
{

public: 
    Window(HINSTANCE hInstance, const std::string& windowClassName, const std::string& title, int width, int height, DWORD windowStyle);
    ~Window() noexcept;

    void ProcessMessages() noexcept;
    bool ShouldClose() const noexcept { return m_ShouldClose; }

    HDC GetDeviceContext() const noexcept  { return GetDC(m_hWnd); }
    int GetWidth() const noexcept  { return m_Width; }
	int GetHeight() const noexcept  { return mHeight; }

	void Close() noexcept { m_ShouldClose = true; }

	bool isCreated() const noexcept { return m_IsCreated; }
    explicit operator bool() const noexcept { return m_IsCreated; }
    static LRESULT CALLBACK StaticWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;

	HWND GetHWND() const noexcept { return m_hWnd; }
private:
    LRESULT HandleMessage(HWND hWnd,UINT msg, WPARAM wParam, LPARAM lParam) noexcept;

	static bool DoesWindowClassExistsAndHavePrivateDCAndUsesCorrectWndProc(const std::string& className, HINSTANCE hInstance ) noexcept;
private:


    HINSTANCE m_hInstance;
    HWND m_hWnd;
    std::string windowTitle;
    int m_Width, mHeight;
	float m_AspectRatio;
    bool m_ShouldClose;
    bool m_IsCreated;
};
