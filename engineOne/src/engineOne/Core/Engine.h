#pragma once

#include "Window.h"
#include "ApplicationBase.h"
#include "Rendering/LowLevel/RenderContext.h"
#include "GLLoader.h"
#include "Timer.h"
#include<memory>
class ApplicationBase;

class Engine
{
public:
	Engine(HINSTANCE hInstance, ApplicationBase* app) noexcept;
	bool Run() noexcept;
	~Engine() noexcept;
private:
	bool Init() noexcept;
private:
	inline static constexpr const char* s_WindowClassName = "MyEngineWindowClass";
	static bool RegisterWindowClass(HINSTANCE hInstance) noexcept;

	static void APIENTRY OpenGLDebugCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam);

private:
	HINSTANCE m_hInstance;
	Timer m_Timer;
	bool m_IsFailed;
	Input m_Input;

	GLLoader m_OpenGLLoader;
	std::unique_ptr<Window> m_pWindow;
	std::unique_ptr<RenderContext> m_pRenderContext;

	ApplicationBase* m_pApp;
};

