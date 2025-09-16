#include "pch.h"
#include "RenderContext.h"
#include <glad/wgl.h>
#include<iostream>
RenderContext::RenderContext(const RenderContextCreateInfo& createInfo)
	:
	m_hDC(nullptr), m_hGLRC(nullptr)
{
	m_hDC = createInfo.window.GetDeviceContext();
	if (!m_hDC)
	{
		return;
	}

	int debugContext = createInfo.debugContext ? WGL_CONTEXT_DEBUG_BIT_ARB : 0;



	int pixelFormatAttribs[] =
	{
		WGL_DRAW_TO_WINDOW_ARB, GL_TRUE,
		WGL_SUPPORT_OPENGL_ARB, GL_TRUE,
		WGL_DOUBLE_BUFFER_ARB, GL_TRUE,
		WGL_PIXEL_TYPE_ARB, WGL_TYPE_RGBA_ARB,
		WGL_COLOR_BITS_ARB, createInfo.colorBits,
		WGL_DEPTH_BITS_ARB, createInfo.depthBits,
		WGL_STENCIL_BITS_ARB, createInfo.stencilBits,
		WGL_ACCELERATION_ARB, WGL_FULL_ACCELERATION_ARB,
		0 // End of attributes list
	};

	int pixelFormat;
	UINT numFormats;
	int status = wglChoosePixelFormatARB(m_hDC, pixelFormatAttribs, NULL, 1, &pixelFormat, &numFormats);
	if (status == FALSE || numFormats <= 0)
	{
		std::cerr << "Failed to choose pixel format for OpenGL context\n";
		return;
	}

	PIXELFORMATDESCRIPTOR pfd;
	if (DescribePixelFormat(m_hDC, pixelFormat, sizeof(pfd), &pfd) == 0)
	{
		std::cerr << "Failed to describe pixel format for OpenGL context\n";
		return;
	}
	if (SetPixelFormat(m_hDC, pixelFormat, &pfd) == FALSE)
	{
		std::cerr << "Failed to set pixel format for OpenGL context\n";
		return;
	}
	int contextAttribs[] =
	{
		WGL_CONTEXT_MAJOR_VERSION_ARB, createInfo.majorVersion,
		WGL_CONTEXT_MINOR_VERSION_ARB, createInfo.minorVersion,
		WGL_CONTEXT_FLAGS_ARB, debugContext, // Request debug context
		WGL_CONTEXT_PROFILE_MASK_ARB,WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
	
		0 // End of attributes list
	};
	m_hGLRC = wglCreateContextAttribsARB(m_hDC, 0, contextAttribs);

	if (!m_hGLRC)
	{
		std::cerr << "wglCreateContextAttribsARB failed\n";
		return;
	}
	if (wglMakeCurrent(m_hDC, m_hGLRC) == FALSE)
	{
		std::cerr << "Failed to make OpenGL context current\n";
		wglDeleteContext(m_hGLRC);
		m_hGLRC = nullptr;
		return;
	}

	std::cout << "OpenGL context created successfully\n";
}

RenderContext::RenderContext(Window& window, int majorVersion, int minorVersion, bool debug)
	:
	RenderContext(RenderContextCreateInfo{ window, majorVersion, minorVersion, debug, 32, 24, 8, 0 })
{
}

RenderContext::~RenderContext() noexcept
{
	if (m_hGLRC)
	{
		wglMakeCurrent(nullptr, nullptr);
		wglDeleteContext(m_hGLRC);
		m_hGLRC = nullptr;
	}
}

bool RenderContext::MakeCurrent(Window& window) noexcept
{
	m_hDC = window.GetDeviceContext();
	BOOL ret = wglMakeCurrent(m_hDC, m_hGLRC);
	return ret == TRUE;
}
