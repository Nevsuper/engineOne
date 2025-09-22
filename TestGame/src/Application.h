#pragma once
#include <memory>
#include <engineOne/Core/Rendering/LowLevel/Buffer.h>
#include <engineOne/Core/Rendering/LowLevel/Shader.h>
#include <engineOne/Core/Rendering/LowLevel/VertexArray.h>
#include <engineOne/Core/Rendering/LowLevel/Texture.h>
#include <engineOne/Core/Rendering/LowLevel/RenderContext.h>
#include <engineOne/Core/Camera.h>
#include <engineOne/Core/Timer.h>
#include <engineOne/Core/GLLoader.h>
#include <engineOne/Core/Input/Input.h>
#include <engineOne/Core/ApplicationBase.h>
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


class Application : public ApplicationBase
{
public:
	Application(const std::string& appName) noexcept;

	Application(const Application&) = delete;
	Application& operator=(const Application&) = delete;

	virtual ~Application() noexcept = default;


	virtual bool Init() noexcept override;

	virtual void Update(float deltaTime) noexcept override;
	virtual void OnRender() noexcept override;
private:

	bool InitResources() noexcept;
	void ProcessInput(float deltaTime) noexcept;

private:
	//in the order of initialization and reverse order of destruction
	Camera m_Camera;
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

