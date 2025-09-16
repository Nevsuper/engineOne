#pragma once
#include"Window.h"
#include <memory>
#include "Rendering/LowLevel/Buffer.h"
#include "Rendering/LowLevel/Shader.h"
#include "Rendering/LowLevel/VertexArray.h"
#include "Rendering/LowLevel/Texture.h"
#include "Rendering/LowLevel/RenderContext.h"
#include "Camera.h"
#include "Timer.h"
#include "GLLoader.h"

struct Vertex
{
	glm::vec3 position; // 3D position
	glm::vec3 normal;
	glm::vec2 texCoords;
};

struct Mesh
{
	std::vector<Vertex> vertices;
	std::vector<uint32_t> indices;
};

/**
 * @brief  class for runnning engine Application
 */
class Application
{
public:
	/**
	 * @brief constructer  for creating Application object
	 * @param hInstance HINSTANCE from WinMain
	 * @param appName  Name of the application which will be used to set window name
	 */
	Application(HINSTANCE hInstance, const std::string& appName) noexcept;

	Application(const Application&) = delete;
	Application& operator=(const Application&) = delete;

	virtual ~Application() noexcept = default;

	/**
	 * @brief Initialize the App class.
	 * @return returns true if initalizetion succeeds and false otherwise
	 */
	bool Init() noexcept;
	/**
	 * @brief Run the application
	 */
	void Run() noexcept;
private:
	/**
	 * @brief function to Initialize graphics which is called by the Init function
	 * @return returns true if initalizetion succeeds and false otherwise
	 */
	bool InitGraphics() noexcept;
	/**
	 * @brief function ti Initialize Resources like textures and models which is called by the Init function
	 * @return returns true if initalizetion succeeds and false otherwise
	 */
	bool InitResources() noexcept;
	
	/**
	 * @brief function to handle inputs and is called by the Run function
	 * @param deltaTime 
	 */
	void ProcessInput(float deltaTime) noexcept;

	/**
	 * @brief function to updates per frame and is called by the Run function
	 * @param deltaTime 
	 */
	void Update(float deltaTime) noexcept;
	/**
	 * @brief function to Render and is called by the Run function
	 */
	void Render() noexcept;

private:
	/**
	 * @brief static function which registers the window class (win32)
	 * @param hInstance 
	 * @return returns true upon succeeding and false otherwise
	 */
	static bool RegisterWindowClass(HINSTANCE hInstance) noexcept;

	/**
	 * @brief callback function which is called by opengl when enountering openglerrors
	 * @param source 
	 * @param type 
	 * @param id 
	 * @param severity 
	 * @param length 
	 * @param message 
	 * @param userParam 
	 * @return 
	 */
	static void APIENTRY OpenGLDebugCallback(
		GLenum source, GLenum type, GLuint id, GLenum severity,
		GLsizei length, const GLchar* message, const void* userParam);
private:
	inline static constexpr const char* s_WindowClassName = "MyEngineWindowClass";
private:
	//in the order of initialization and reverse order of destruction
	HINSTANCE m_hInstance;
	std::string m_ApplicationName;
	CameraFPS m_Camera;
	Timer m_Timer;



	GLLoader m_OpenGLLoader;
	std::unique_ptr<Window> m_Window;
	std::unique_ptr<RenderContext> m_RenderContext;
	std::unique_ptr<ShaderProgram> m_ShaderProgram;
	std::unique_ptr<ShaderProgram> m_ShaderProgramLit;

	Mesh m_Mesh;

	std::unique_ptr<VertexArray> m_VAO;
	std::unique_ptr<VertexBuffer> m_VBO;
	std::unique_ptr<IndexBuffer> m_EBO;

	std::unique_ptr<VertexArray> m_VAOLit;
	std::unique_ptr<VertexBuffer> m_VBOLit;
	std::unique_ptr<IndexBuffer> m_EBOLit;

	std::unique_ptr<Texture2D> m_Texture,m_AlphaTexture;
};

