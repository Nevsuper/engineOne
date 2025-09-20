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
#include "Input/Input.h"
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


class Application
{
public:
	Input* m_pInput;
	float m_Aspect;
	Application(const std::string& appName) noexcept;

	Application(const Application&) = delete;
	Application& operator=(const Application&) = delete;

	virtual ~Application() noexcept = default;


	bool Init() noexcept;

	void Run() noexcept;
	void Update(float deltaTime) noexcept;
	void OnRender() noexcept;
private:

	bool InitResources() noexcept;
	void ProcessInput(float deltaTime) noexcept;
public:
	const std::string& getName() const noexcept {return m_ApplicationName;}
private:
	//in the order of initialization and reverse order of destruction
	std::string m_ApplicationName;
	CameraFPS m_Camera;
	Timer m_Timer;
	bool m_IsWireMode = false;
	bool m_IsTex = true;

	std::unique_ptr<ShaderProgram> m_ShaderProgram;
	std::unique_ptr<ShaderProgram> m_ShaderProgramNoTex;
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

