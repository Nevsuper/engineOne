#include "pch.h"
#include "GLLoader.h"
#include<glad/gl.h>
#include<glad/wgl.h>

bool loadModernOpenGLFunctions(HINSTANCE hInstance)
{

	const char* windowClassName = "TempGLLoaderWindowClass";

	WNDCLASS wc{};
	wc.lpfnWndProc = DefWindowProc;
	wc.hInstance = hInstance;
	wc.lpszClassName = windowClassName;
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.style = CS_OWNDC;
	
	if(!RegisterClass(&wc))
	{
		MessageBox(NULL, "Failed to register window class for OpenGL function loading", "Error", MB_OK | MB_ICONERROR);
		return false;
	}

	HWND hDummyWnd = CreateWindowEx(
		0,
		windowClassName,
		"Dummy OpenGL Loader Window",
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
		NULL,
		NULL,
		hInstance,
		NULL
	);

	if (!hDummyWnd)
	{
		MessageBox(NULL, "Failed to create dummy window for OpenGL function loading", "Error", MB_OK | MB_ICONERROR);
		UnregisterClass(windowClassName, hInstance);
		return false;
	}

	HDC hDummyDC = GetDC(hDummyWnd);
	if (!hDummyDC)
	{
		MessageBox(NULL, "Failed to get device context for dummy window", "Error", MB_OK | MB_ICONERROR);
		DestroyWindow(hDummyWnd);
		UnregisterClass(windowClassName, hInstance);
		return false;
	}

	//create opwngl context
	PIXELFORMATDESCRIPTOR dummyPFD{};
	dummyPFD.nSize = sizeof(PIXELFORMATDESCRIPTOR);
	dummyPFD.nVersion = 1;
	dummyPFD.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
	dummyPFD.iPixelType = PFD_TYPE_RGBA;
	dummyPFD.cColorBits = 32;
	dummyPFD.cDepthBits = 24;
	dummyPFD.iLayerType = PFD_MAIN_PLANE;
	int dummyPixelFormat = ChoosePixelFormat(hDummyDC, &dummyPFD);

	if(dummyPixelFormat == 0)
	{
		MessageBox(NULL, "Failed to choose pixel format for dummy window", "Error", MB_OK | MB_ICONERROR);
		DestroyWindow(hDummyWnd);
		UnregisterClass(windowClassName, hInstance);
		return false;
	}
	if(!SetPixelFormat(hDummyDC, dummyPixelFormat, &dummyPFD))
	{
		MessageBox(NULL, "Failed to set pixel format for dummy window", "Error", MB_OK | MB_ICONERROR);
		DestroyWindow(hDummyWnd);
		UnregisterClass(windowClassName, hInstance);
		return false;
	}

	HGLRC hDummyGLRC = wglCreateContext(hDummyDC);
	if (!hDummyGLRC)
	{
		MessageBox(NULL, "Failed to create OpenGL context for dummy window", "Error", MB_OK | MB_ICONERROR);
		DestroyWindow(hDummyWnd);
		UnregisterClass(windowClassName, hInstance);
		return false;
	}
	if (!wglMakeCurrent(hDummyDC, hDummyGLRC))
	{
		MessageBox(NULL, "Failed to make OpenGL context current for dummy window", "Error", MB_OK | MB_ICONERROR);
		wglDeleteContext(hDummyGLRC);
		DestroyWindow(hDummyWnd);
		UnregisterClass(windowClassName, hInstance);
		return false;
	}

	if (!gladLoaderLoadWGL(hDummyDC))
	{
		MessageBox(NULL, "Failed to load WGL functions", "Error", MB_OK | MB_ICONERROR);
		wglMakeCurrent(NULL, NULL);
		wglDeleteContext(hDummyGLRC);
		DestroyWindow(hDummyWnd);
		UnregisterClass(windowClassName, hInstance);
		return false;
	}

	if (!gladLoaderLoadGL())
	{
		MessageBox(NULL, "Failed to load OpenGL functions", "Error", MB_OK | MB_ICONERROR);
		wglMakeCurrent(NULL, NULL);
		wglDeleteContext(hDummyGLRC);
		DestroyWindow(hDummyWnd);
		UnregisterClass(windowClassName, hInstance);
		return false;
	}
	//cleanup
	wglMakeCurrent(NULL, NULL);
	wglDeleteContext(hDummyGLRC);
	DestroyWindow(hDummyWnd);
	UnregisterClass(windowClassName, hInstance);
	return true;

}

void unloadModernOpenGLFunctions()
{
	gladLoaderUnloadGL();
}
