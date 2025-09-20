#include "pch.h"
#include "Engine.h"

#include "Application.h"
#include <engineOne/utils/utils.h>


Engine::Engine(HINSTANCE hInstance, Application* app) noexcept
	:
m_OpenGLLoader(hInstance), m_pApp(app), m_hInstance(hInstance)
{

}

Engine::~Engine()
{
	SafeDeletePtr(m_pApp);
}


bool Engine::Run() noexcept
{
	if (!Init())
	{
		return false;
	};
	std::cout <<"Engine Running" << std::endl;
	m_pRenderContext->SetSwapInterval(1);
	while ( !m_pWindow->ShouldClose())
	{
		m_Input.Flush();
		m_pWindow->ProcessMessages();

		if (m_Input.IsKeyPressed(KeyCode::Esc))
		{
			m_pWindow->Close();
		}

		float dt = static_cast<float>(m_Timer.elapsedAndReset());
		m_pApp->Update(dt);
		m_pRenderContext->clearColor(0.0f, 0.0f, 0.0f, 1.0f);
		m_pApp->OnRender();
		m_pRenderContext->Present();
	}
	return true;
}

bool Engine::Init() noexcept
{
	if (!RegisterWindowClass(m_hInstance)) return false;

	m_pWindow = std::make_unique<Window>(m_Input, m_hInstance, s_WindowClassName, m_pApp->getName().c_str(),900,800);

	if (!m_pWindow->isCreated()) return false;

	m_pRenderContext = std::make_unique<RenderContext>(*m_pWindow);


	if (m_pRenderContext->IsNull()) return false;

	std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << std::endl;
	std::cout << "OpenGL Vendor: " << glGetString(GL_VENDOR) << std::endl;
	std::cout << "OpenGL Renderer: " << glGetString(GL_RENDERER) << std::endl;

	glViewport(0, 0, m_pWindow->GetWidth(), m_pWindow->GetHeight());
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	// Enable blending for alpha transparency
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDebugMessageCallback(Engine::OpenGLDebugCallback, nullptr);
	// Control which messages are reported (optional)
	glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);

	m_pApp->m_Aspect = m_pWindow->GetAspectRatio();
	m_pApp->m_pInput = &m_Input;
	if (!m_pApp->Init()) return false;

	return true;

}

bool Engine::RegisterWindowClass(HINSTANCE hInstance) noexcept
{
	WNDCLASS wc{};
	wc.hInstance = hInstance;
	wc.lpszClassName = s_WindowClassName;
	wc.lpfnWndProc = Window::StaticWndProc;
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.style = CS_OWNDC;
	if (!RegisterClass(&wc))
	{
		return false;
	}
	return true;
}

inline void APIENTRY Engine::OpenGLDebugCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam) {
	// Filter out non-critical messages (optional)
	if (type == GL_DEBUG_TYPE_OTHER || severity == GL_DEBUG_SEVERITY_LOW)
		return;

	// Output debug message
	fprintf(stderr, "OpenGL Debug Message:\n"
		"  Source: 0x%X\n  Type: 0x%X\n  ID: %d\n  Severity: 0x%X\n"
		"  Message: %s\n\n",
		source, type, id, severity, message);
}
