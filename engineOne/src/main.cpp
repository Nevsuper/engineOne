#include<Windows.h>
#include<iostream>

#include<glad/gl.h>
#include<glad/wgl.h>

#include"Shader.h"
#include"VertexArray.h"
#include "Buffer.h"
#define RETURN_FAIL_IF_FAILED(cond,msg) \
	if(!(cond)) \
{\
	MessageBeep(MB_ICONERROR); \
    MessageBox(NULL, msg, "Error", MB_OK | MB_ICONERROR);\
	return -1;\
}

#define CLEANUP_AND_RETURN_FAIL_IF_FAILED(cond,msg,cleanup) \
	if(!(cond)) \
{\
	MessageBeep(MB_ICONERROR); \
	MessageBox(NULL, msg, "Error", MB_OK | MB_ICONERROR);\
	cleanup;\
	return -1;\
}

bool hasAVX2() {
	int cpuInfo[4];
	__cpuid(cpuInfo, 0);                // highest supported function
	if (cpuInfo[0] < 7) return false;   // no extended features

	__cpuidex(cpuInfo, 7, 0);           // call leaf 7
	return (cpuInfo[1] & (1 << 5)) != 0; // EBX bit 5 = AVX2
}

bool check_sse41() {
	int cpuInfo[4];

	__cpuid(cpuInfo, 1);
	return (cpuInfo[2] & (1 << 19)) != 0; // ECX bit 19 = SSE
}

LRESULT WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

int main()
{
	if(!hasAVX2())
	{
		MessageBox(NULL, "CPU does not support AVX2. The application will exit.", "Error", MB_OK | MB_ICONERROR);
		return -1;
	}
	if(!check_sse41())
	{
		MessageBox(NULL, "CPU does not support SSE4.1. The application will exit.", "Error", MB_OK | MB_ICONERROR);
		return -1;
	}
	const char* windowClassName = "Sample Window Class";

	HINSTANCE hInstance = GetModuleHandle(nullptr);

	WNDCLASS wc{};
	wc.lpfnWndProc = WndProc;
	wc.hInstance = hInstance;
	wc.lpszClassName = windowClassName;
	wc.lpfnWndProc = WndProc;
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.style = CS_OWNDC;

	RETURN_FAIL_IF_FAILED(RegisterClass(&wc), "Failed to register window class");

	HWND hDummyWindow = CreateWindow(
		windowClassName,
		"Fake Window",
		WS_OVERLAPPEDWINDOW,
		0, 0,
		0, 0,
		nullptr,
		nullptr,
		hInstance,
		nullptr
	);

	RETURN_FAIL_IF_FAILED(hDummyWindow != nullptr, "Failed to create dummy window");

	HDC hDummyDC = GetDC(hDummyWindow);
	RETURN_FAIL_IF_FAILED(hDummyDC != nullptr, "Failed to get device context");

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
	RETURN_FAIL_IF_FAILED(dummyPixelFormat != 0, "Failed to choose pixel format");
	RETURN_FAIL_IF_FAILED(SetPixelFormat(hDummyDC, dummyPixelFormat, &dummyPFD) == TRUE, "Failed to set pixel format");


	HGLRC hDummyRC = wglCreateContext(hDummyDC);
	RETURN_FAIL_IF_FAILED(hDummyRC != nullptr, "Failed to create dummy OpenGL context");
	RETURN_FAIL_IF_FAILED(wglMakeCurrent(hDummyDC, hDummyRC) == TRUE, "Failed to make dummy OpenGL context current");


	auto dummyContextCleanup = [&]()
		{
			wglMakeCurrent(NULL, NULL);
			wglDeleteContext(hDummyRC);
			//we using cs_owndc so no need to delete dc
			DestroyWindow(hDummyWindow);
		};


	CLEANUP_AND_RETURN_FAIL_IF_FAILED(gladLoaderLoadWGL(hDummyDC) != 0, "Failed to load WGL", dummyContextCleanup());

	//load gl now
	CLEANUP_AND_RETURN_FAIL_IF_FAILED(gladLoaderLoadGL() != 0, "Failed to load OpenGL", dummyContextCleanup());

	dummyContextCleanup();

	DWORD windowStyle = WS_OVERLAPPEDWINDOW | WS_VISIBLE;
	const int windowWidth = 800;
	const int windowHeight = 600;
	RECT windowRect = { 0, 0, windowWidth, windowHeight };
	AdjustWindowRect(&windowRect, windowStyle, FALSE);

	HWND hWnd = CreateWindow(
		windowClassName,
		"Sample Window",
		windowStyle,
		CW_USEDEFAULT, CW_USEDEFAULT,
		windowRect.right - windowRect.left,
		windowRect.bottom - windowRect.top,
		nullptr,
		nullptr,
		hInstance,
		nullptr
	);

	RETURN_FAIL_IF_FAILED(hWnd != nullptr, "Failed to create window");

	ShowWindow(hWnd, SW_SHOW);
	UpdateWindow(hWnd);

	HDC hDC = GetDC(hWnd);

	int pixelFormatAttribs[] =
	{
		WGL_DRAW_TO_WINDOW_ARB, GL_TRUE,
		WGL_SUPPORT_OPENGL_ARB, GL_TRUE,
		WGL_DOUBLE_BUFFER_ARB, GL_TRUE,
		WGL_PIXEL_TYPE_ARB, WGL_TYPE_RGBA_ARB,
		WGL_COLOR_BITS_ARB, 32,
		WGL_DEPTH_BITS_ARB, 24,
		WGL_STENCIL_BITS_ARB, 8,
		0 // End of attributes list
	};
	int pixelFormat;
	UINT numFormats;
	int status = wglChoosePixelFormatARB(hDC, pixelFormatAttribs, NULL, 1, &pixelFormat, &numFormats);
	RETURN_FAIL_IF_FAILED(status == TRUE && numFormats > 0, "Failed to choose pixel format");

	PIXELFORMATDESCRIPTOR pfd;

	RETURN_FAIL_IF_FAILED(DescribePixelFormat(hDC, pixelFormat, sizeof(pfd), &pfd) != 0, "Failed to describe pixel format");

	RETURN_FAIL_IF_FAILED(SetPixelFormat(hDC, pixelFormat, &pfd) == TRUE, "Failed to set pixel format");

	int contextAttribs[] =
	{
		WGL_CONTEXT_MAJOR_VERSION_ARB, 4,
		WGL_CONTEXT_MINOR_VERSION_ARB, 6,
		WGL_CONTEXT_PROFILE_MASK_ARB, WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
		0 // End of attributes list
	};


	HGLRC hGLRC = wglCreateContextAttribsARB(hDC, 0, contextAttribs);

	CLEANUP_AND_RETURN_FAIL_IF_FAILED(hGLRC != nullptr, "Failed to create OpenGL context", gladLoaderUnloadGL());

	auto cleanupGL = [&]()
		{
			wglMakeCurrent(NULL, NULL);
			wglDeleteContext(hGLRC);
			gladLoaderUnloadGL();
			DestroyWindow(hWnd);
		};

	CLEANUP_AND_RETURN_FAIL_IF_FAILED(wglMakeCurrent(hDC, hGLRC) == TRUE, "Failed to make OpenGL context current", cleanupGL());
	//print opengl version, venndor name,and rendere
	std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << std::endl;
	std::cout << "OpenGL Vendor: " << glGetString(GL_VENDOR) << std::endl;
	std::cout << "OpenGL Renderer: " << glGetString(GL_RENDERER) << std::endl;
	glViewport(0, 0, windowWidth, windowHeight);
	bool running = true;



	MSG msg{};

	LARGE_INTEGER	frequency, start, end;
	QueryPerformanceFrequency(&frequency);
	double timeAccumulator = 0.0;
	int frameCount = 0;

#pragma region triangle geometry

	float vertices[] = {
		//positions
		 0.0f, 0.5f,
		-0.5f,-0.5f,
		0.5f,-0.5f
	};

	unsigned int indices[] = {
		0,1,2
	};

	{

		VertexArray VAO;
		VAO.Bind();
		VertexBuffer VBO(vertices, sizeof(vertices));
		VAO.addAttribute(0, 2, GLType::Float, false, 2 * sizeof(float), (void*)0);
		IndexBuffer EBO(indices, sizeof(indices));
		EBO.Bind();
		VAO.Unbind();
		VBO.Unbind();
		EBO.Unbind();

#pragma endregion

		ShaderProgram Shader("assets/shaders/vertex.glsl", "assets/shaders/fragment.glsl");
		Shader.Bind();
		wglSwapIntervalEXT(0);
		QueryPerformanceCounter(&start);
		while (running)
		{
			while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
			{
				if (msg.message == WM_QUIT)
				{
					running = false;
				}
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
			QueryPerformanceCounter(&end);
			double deltaTime = static_cast<double>(end.QuadPart - start.QuadPart) / frequency.QuadPart;
			start = end;
			timeAccumulator += deltaTime;
			frameCount++;
			if (timeAccumulator >= 1.0) {
				double fps = frameCount / timeAccumulator;   // average FPS
				std::cout << "FPS: " << fps << "\r";
				std::cout.flush();
				frameCount = 0;
				timeAccumulator = 0.0;
			}
			//Rendering code goes here
			glClearColor(0.1f, 0.2f, 0.3f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			Shader.Bind();
			VAO.Bind();
			glDrawElements(GL_TRIANGLES, 3, GLTypeToGLenum(GLType::UnsignedInt), reinterpret_cast<const void*>(0));
			SwapBuffers(hDC);
		}

	}
	cleanupGL();

	std::cout << "Underlying type of ShaderType is: " << typeid(std::underlying_type_t<ShaderType>).name() << std::endl;
	return 0;
}



LRESULT WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{

	//quit upon wm_close
	switch (msg)
	{
	case WM_CLOSE:
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(hwnd, msg, wParam, lParam);
}
