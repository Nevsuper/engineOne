#pragma once
#include "engineOne/Core/Window.h"
#include "engineOne/utils/utils.h"

#include<glad/gl.h>
#include<glad/wgl.h>


struct RenderContextCreateInfo
{
	Window& window;
	int majorVersion = 4;
	int minorVersion = 5;
	bool debugContext = true;
	int colorBits = 32;
	int depthBits = 24;
	int stencilBits = 8;
	int samples = 0; // Number of samples for multisampling (0 = no multisampling)
	RenderContextCreateInfo(Window& win,
		int maj = 4,
		int min = 5,
		bool debug = true,
		int color = 32,
		int depth = 24,
		int stencil = 8,
		int sampleCount = 0)
		: window(win),
		majorVersion(maj),
		minorVersion(min),
		debugContext(debug),
		colorBits(color),
		depthBits(depth),
		stencilBits(stencil),
		samples(sampleCount)
	{
	}
};


class RenderContext
{
public:
	RenderContext(const RenderContextCreateInfo& createInfo);
	RenderContext(Window& window, int majorVersion = 4, int minorVersion = 5, bool debug = true);
	~RenderContext() noexcept;

	RenderContext(const RenderContext&) = delete;
	RenderContext& operator=(const RenderContext&) = delete;

	bool MakeCurrent(Window& window) noexcept;

	void clearColor(float r, float g, float b, float a) noexcept
	{
		glClearColor(r, g, b, a);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	}

	void Present() const noexcept
	{
		SwapBuffers(m_hDC);
	}

	void SetSwapInterval(int interval) const noexcept
	{
		wglSwapIntervalEXT(interval);
	}

	bool IsNull() const noexcept { return m_hGLRC == nullptr; }
	operator bool() const noexcept { return m_hGLRC != nullptr; }
private:
	HDC m_hDC;
	HGLRC m_hGLRC;
};

