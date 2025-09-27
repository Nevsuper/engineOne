#pragma once
#include <memory>
#include <engineOne/Core/Rendering/LowLevel/Buffer.h>
#include <engineOne/Core/Rendering/LowLevel/Shader.h>
#include <engineOne/Core/Rendering/LowLevel/VertexArray.h>
#include <engineOne/Core/Rendering/LowLevel/Texture.h>
#include <engineOne/Core/Rendering/LowLevel/RenderContext.h>
#include <engineOne/Core/Camera.h>
#include <engineOne/Core/Timer.h>

#include <engineOne/Core/Input/Input.h>
#include <engineOne/Core/ApplicationBase.h>
#include <engineOne/Core/Rendering/Mesh.h>


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

	void GenerateTerrain() noexcept;

	void CalculateNormals(std::vector<Vertex>&, const std::vector<uint32_t>& indices) noexcept;

private:
	//in the order of initialization and reverse order of destruction
	Camera m_Camera;
	Timer m_Timer;
	bool m_IsWireMode = false;
	bool m_IsTex = true;

	std::unique_ptr<ShaderProgram> m_ShaderProgramLit;
	std::unique_ptr<ShaderProgram> m_ShaderProgramTerrain;

	Mesh m_TerrainMesh;

	std::unique_ptr<VertexArray> m_VAOLit;
	std::unique_ptr<VertexBuffer> m_VBOLit;
	std::unique_ptr<IndexBuffer> m_EBOLit;

	std::unique_ptr<VertexArray> m_TerrainVAO;
	std::unique_ptr<VertexBuffer> m_TerrainVBO;
	std::unique_ptr<IndexBuffer> m_TerrainEBO;

};

