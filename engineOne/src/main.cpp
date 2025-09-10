#include<Windows.h>
#include<iostream>
#include<random>
#include<array>
#include<glad/gl.h>
#include<glad/wgl.h>
#define GLM_ENABLE_EXPERIMENTAL
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>

#include"Shader.h"
#include "Camera.h"
#include"VertexArray.h"
#include "Buffer.h"

#include "keyCodes.h"
#include "Window.h"
#include "GLLoader.h"
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



bool isKeyPressed(int vKey)
{
	return (GetAsyncKeyState(vKey) & (short)0x8000) != 0;
}





int main()
{
	
	const char* windowClassName = "kkppOOK";
	const char* fakeWindowClassName = "tttkkppOOK";

	HINSTANCE hInstance = GetModuleHandle(nullptr);

	WNDCLASS wc{};
	wc.hInstance = hInstance;
	wc.lpszClassName = windowClassName;
	wc.lpfnWndProc = Window::StaticWndProc;
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.style = CS_OWNDC | CS_HREDRAW | CS_VREDRAW;

	RETURN_FAIL_IF_FAILED(RegisterClass(&wc), "Failed to register window class");

	bool isLoaded = loadModernOpenGLFunctions();

	RETURN_FAIL_IF_FAILED(isLoaded == true, "Failed to load modern OpenGL functions");

	const int windowWidth = 800;
	const int windowHeight = 600;
	
	Window window(hInstance, windowClassName, "OpenGL Window", windowWidth, windowHeight,WS_OVERLAPPEDWINDOW );
	RETURN_FAIL_IF_FAILED(window.isCreated() == true, "Failed to create window");


	HDC hDC = window.GetDeviceContext();

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
			unloadModernOpenGLFunctions();
		};

	CLEANUP_AND_RETURN_FAIL_IF_FAILED(wglMakeCurrent(hDC, hGLRC) == TRUE, "Failed to make OpenGL context current", cleanupGL());
	//print opengl version, venndor name,and rendere
	std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << std::endl;
	std::cout << "OpenGL Vendor: " << glGetString(GL_VENDOR) << std::endl;
	std::cout << "OpenGL Renderer: " << glGetString(GL_RENDERER) << std::endl;
	glViewport(0, 0, windowWidth, windowHeight);


#pragma region triangle geometry

	// 8 unique vertices for a cube
	std::array vertices{
		-1.0f, -1.0f,  1.0f,  // 0: front-bottom-left
		 1.0f, -1.0f,  1.0f,  // 1: front-bottom-right
		 1.0f,  1.0f,  1.0f,  // 2: front-top-right
		-1.0f,  1.0f,  1.0f,  // 3: front-top-left
		-1.0f, -1.0f, -1.0f,  // 4: back-bottom-left
		 1.0f, -1.0f, -1.0f,  // 5: back-bottom-right
		 1.0f,  1.0f, -1.0f,  // 6: back-top-right
		-1.0f,  1.0f, -1.0f   // 7: back-top-left
	};

	// Indices for the cube (12 triangles)
	std::array indices
	{
		// Front face
		0u, 1u, 2u,
		2u, 3u, 0u,

		// Back face
		5u, 4u, 7u,
		7u, 6u, 5u,

		// Left face
		4u, 0u, 3u,
		3u, 7u, 4u,

		// Right face
		1u, 5u, 6u,
		6u, 2u, 1u,

		// Top face
		3u, 2u, 6u,
		6u, 7u, 3u,

		// Bottom face
		4u, 5u, 1u,
		1u, 0u, 4u
	};

	float aspect = static_cast<float>(windowWidth) / static_cast<float>(windowHeight);
	CameraFPS camera(90.0f, aspect, 0.1f, 100.0f);

	float cameraSpeed = 10.5f; // units per second
	float cameraAnglularSpeed = 15.0f; // degrees per second

	auto handleInput = [&](float dt)
		{
			if (isKeyPressed(keyCode::key_w))
				camera.MoveForward(dt * cameraSpeed);
			if (isKeyPressed(keyCode::key_a))
				camera.MoveRight(-dt * cameraSpeed);
			if (isKeyPressed(keyCode::key_s))
				camera.MoveForward(-dt * cameraSpeed);
			if (isKeyPressed(keyCode::key_d))
				camera.MoveRight(dt * cameraSpeed);

			if (isKeyPressed(keyCode::key_space))
				camera.MoveUp(dt * cameraSpeed);
			if (isKeyPressed(keyCode::key_c))
				camera.MoveUp(-dt * cameraSpeed);

			if (isKeyPressed(keyCode::key_arrow_left))
				camera.Yaw(-dt * cameraAnglularSpeed);
			if (isKeyPressed(keyCode::key_arrow_right))
				camera.Yaw(dt * cameraAnglularSpeed);
			if (isKeyPressed(keyCode::key_arrow_up))
				camera.Pitch(-dt * cameraAnglularSpeed);
			if (isKeyPressed(keyCode::key_arrow_down))
				camera.Pitch(dt * cameraAnglularSpeed);

			if (isKeyPressed(keyCode::key_r))
				camera.RestPosAndOrient();

			if(isKeyPressed(keyCode::key_esc))
				window.Close();

		};

	auto model = glm::mat4(1.0f);

	{

		VertexArray VAO;
		VAO.Bind();
		VertexBuffer VBO(vertices.data(), sizeof(vertices));
		VAO.addAttribute(0, 3, GLType::Float, false, 3 * sizeof(float), (void*)0);
		IndexBuffer EBO(indices.data(), sizeof(indices));
		EBO.Bind();
		VAO.Unbind();
		VBO.Unbind();
		EBO.Unbind();

#pragma endregion

		ShaderProgram Shader("assets/shaders/vertex.glsl", "assets/shaders/fragment.glsl");
		Shader.Bind();
		wglSwapIntervalEXT(0);

		LARGE_INTEGER	frequency, start, end;
		QueryPerformanceFrequency(&frequency);
		double timeAccumulator = 0.0;
		int frameCount = 0;
		QueryPerformanceCounter(&start);
		float r = 1.0f, g = 1.0f, b = 1.0f;
		std::random_device rd;
		std::mt19937 generator(rd());
		std::uniform_real_distribution<float> dist(0.0f, 1.0f);
		Shader.SetUniformMat4("u_Projection", camera.GetProjectionMatrix());
		while (!window.ShouldClose())
		{
			window.ProcessMessages();
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
				r = dist(generator);
				g = dist(generator);
				b = dist(generator);
			}

			handleInput(static_cast<float>(deltaTime));
			//rotate cube
			Shader.SetUniform4f("u_Color", r, g, b, 1.0f);

			//Rendering code goes here
			glClearColor(0.1f, 0.2f, 0.3f, 1.0f);
			glDepthFunc(GL_LESS);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			Shader.Bind();
			VAO.Bind();
			Shader.SetUniformMat4("u_View", camera.GetViewMatrix());
			Shader.SetUniformMat4("u_Model", model);
			glDrawElements(GL_TRIANGLES, std::size(indices), GLTypeToGLenum(GLType::UnsignedInt), reinterpret_cast<const void*>(0));
			SwapBuffers(hDC);
		}

	}
	cleanupGL();

	std::cout << "Underlying type of ShaderType is: " << typeid(std::underlying_type_t<ShaderType>).name() << std::endl;
	return 0;
}


