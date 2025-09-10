#include "Window.h"
#include<iostream>
#include<stdexcept>

Window::Window(HINSTANCE hInstance, const std::string& windowClassName, const std::string& title, int width, int height, DWORD windowStyle)
	:
	m_hInstance(hInstance),
	windowTitle(title),
	m_Width(width),
	mHeight(height),
	m_ShouldClose(false),
	m_hWnd(nullptr),
	m_AspectRatio(static_cast<float>(width) / static_cast<float>(height)),
	m_IsCreated(true)

{
	if (!DoesWindowClassExistsAndHavePrivateDCAndUsesCorrectWndProc(windowClassName, hInstance))
	{
		m_IsCreated = false;
		return;
	}

	if (width <= 0 || height <= 0)
	{
		m_IsCreated = false;
		return;
	}

	RECT windowRect = { 0, 0, width, height };
	AdjustWindowRect(&windowRect, windowStyle, FALSE);

	m_hWnd = CreateWindow(
		windowClassName.c_str(),
		title.c_str(),
		windowStyle,
		CW_USEDEFAULT, CW_USEDEFAULT,
		windowRect.right - windowRect.left,
		windowRect.bottom - windowRect.top,
		nullptr,
		nullptr,
		hInstance,
		this // Pass 'this' pointer for use in StaticWndProc
	);

	if (!m_hWnd)
	{
		m_IsCreated = false;
		return;
	}
	ShowWindow(m_hWnd, SW_NORMAL);
	UpdateWindow(m_hWnd);
}

Window::~Window() noexcept
{
	if (m_hWnd)
	{
		DestroyWindow(m_hWnd);
	}
}

void Window::ProcessMessages() noexcept
{
	MSG msg;
	while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
	{
		if (msg.message == WM_QUIT)
		{
			m_ShouldClose = true;
		}
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
}


bool Window::DoesWindowClassExistsAndHavePrivateDCAndUsesCorrectWndProc(const std::string& className, HINSTANCE hInstance) noexcept
{
	WNDCLASSEX wc{};
	if (!GetClassInfoExA(hInstance, className.c_str(), &wc))
	{
		return false;
	}

	// Ensure it has CS_OWNDC style
	if ((wc.style & CS_OWNDC) == 0)
	{
		return false; // CS_OWNDC not set
	}

	// Ensure the window procedure is our StaticWndProc
	if (wc.lpfnWndProc != StaticWndProc)
		return false;

	return true; // Class exists and has CS_OWNDC
}

LRESULT Window::StaticWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
{
	if (msg == WM_NCCREATE)
	{
		CREATESTRUCT* pCreate = reinterpret_cast<CREATESTRUCT*>(lParam);
		Window* pWindow = reinterpret_cast<Window*>(pCreate->lpCreateParams);
		SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pWindow));

		// Let Windows initialize the non-client area fully
		return DefWindowProc(hwnd, msg, wParam, lParam);
	}

	// After WM_NCCREATE, retrieve 'this'
	Window* pWindow = reinterpret_cast<Window*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
	if (pWindow)
	{
		return pWindow->HandleMsg(hwnd,msg, wParam, lParam);
	}

	// Fallback: before WM_NCCREATE, or if pointer missing
	return DefWindowProc(hwnd, msg, wParam, lParam);
}

LRESULT Window::HandleMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
{
	switch (msg)
	{
	case WM_SIZE:
	{
		m_Width = LOWORD(lParam);
		mHeight = HIWORD(lParam);
		m_AspectRatio = static_cast<float>(m_Width) / static_cast<float>(mHeight);
		return 0;
	}
	case WM_CLOSE:
		PostQuitMessage(0);
		return 0;
	default:
		return DefWindowProc(hWnd, msg, wParam, lParam);
	}
	return 0l;
}

