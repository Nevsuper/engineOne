#include "Application.h"
#include<random>
#include<numeric>
#include<algorithm>
#include <future>
#include <engineOne/Core/Input/Input.h>
#include <engineOne/Core/Input/KeyCodes.h>
#include <engineOne/Core/logger/Logger.h>

#include<iostream>
#include<glad/gl.h>
#include<stb_image.h>
#include<tiny_obj_loader.h>
#include<glm/gtc/matrix_transform.hpp>
#include <engineOne/Core/Engine.h>
#include "PerlinNoise.h"

Application::Application(const std::string& appName) noexcept
	:
	ApplicationBase(appName)
{
}



bool Application::Init() noexcept
{

	m_Camera.UpdateProjection(glm::radians(90.0f), m_Aspect, 0.1f, 500.0f);
	if (!InitResources())
	{
		std::cerr << "Failed to initialize resources\n";
		return false;
	}

	return true;
}


bool Application::InitResources() noexcept
{


	Timer loadTimer;


	//load both textures


	m_ShaderProgramLit = std::make_unique<ShaderProgram>("assets/shaders/vertexLit.glsl", "assets/shaders/fragmentLit.glsl");

	if (!m_ShaderProgramLit->checkLinkStatus())
	{
		return false;
	}
	m_ShaderProgramTerrain = std::make_unique<ShaderProgram>("assets/shaders/vertex.glsl", "assets/shaders/fragment.glsl");

	if (!m_ShaderProgramTerrain->checkLinkStatus())
	{
		return false;
	}


	GenerateTerrain();



	double loadTime = loadTimer.elapsed();
	LOG_DEBUG(" Took {} s to load resources", loadTime);
	//village model 

	// lit cube

	std::vector<glm::vec3> cubeVertices = {
		// Front face
		{-0.5f, -0.5f,  0.5f}, // 0
		{ 0.5f, -0.5f,  0.5f}, // 1
		{ 0.5f,  0.5f,  0.5f}, // 2
		{-0.5f,  0.5f,  0.5f}, // 3

		// Back face
		{-0.5f, -0.5f, -0.5f}, // 4
		{ 0.5f, -0.5f, -0.5f}, // 5
		{ 0.5f,  0.5f, -0.5f}, // 6
		{-0.5f,  0.5f, -0.5f}  // 7
	};

	std::vector<unsigned int> cubeIndices = {
		// Front face
		0, 1, 2,
		2, 3, 0,

		// Right face
		1, 5, 6,
		6, 2, 1,

		// Back face
		5, 4, 7,
		7, 6, 5,

		// Left face
		4, 0, 3,
		3, 7, 4,

		// Top face
		3, 2, 6,
		6, 7, 3,

		// Bottom face
		4, 5, 1,
		1, 0, 4
	};
	m_VAOLit = std::make_unique<VertexArray>();
	m_VAOLit->Bind();
	m_VBOLit = std::make_unique<VertexBuffer>(cubeVertices.data(), cubeVertices.size() * sizeof(glm::vec3));
	m_VBOLit->Bind();
	m_VAOLit->addAttribute(0, 3, GLType::Float, false, sizeof(glm::vec3), (void*)0);
	m_EBOLit = std::make_unique<IndexBuffer>(cubeIndices.data(), cubeIndices.size() * sizeof(cubeIndices[0]), GLType::UnsignedInt);
	m_EBOLit->Bind();

	m_TerrainVAO = std::make_unique<VertexArray>();
	m_TerrainVAO->Bind();
	m_TerrainVBO = std::make_unique<VertexBuffer>(m_TerrainMesh.vertices.data(), m_TerrainMesh.vertices.size() * sizeof(m_TerrainMesh.vertices[0]));
	m_TerrainVBO->Bind();
	m_TerrainEBO = std::make_unique<IndexBuffer>(m_TerrainMesh.indices.data(), m_TerrainMesh.indices.size() * sizeof(m_TerrainMesh.indices[0]), GLType::UnsignedInt);
	m_TerrainEBO->Bind();
	m_TerrainVAO->addAttribute(0, 3, GLType::Float, false, sizeof(Vertex), reinterpret_cast<const void*>(0));
	m_TerrainVAO->addAttribute(1, 3, GLType::Float, false, sizeof(Vertex), reinterpret_cast<const void*>(sizeof(glm::vec3)));

	return true;
}
void Application::ProcessInput(float deltaTime) noexcept
{


	float cameraSpeed1 = 100.5f; // units per second
	float cameraAnglularSpeed = 30.0f; // degrees per second
	float cameraSpeed2 = 10000.0f;
	float cameraSpeed = cameraSpeed1;

	if (m_pInput->IsKeyDown(KeyCode::Leftshift))
	{
		cameraSpeed = cameraSpeed2;
	}
	if (m_pInput->IsKeyDown(KeyCode::W))
		m_Camera.MoveForward(deltaTime * cameraSpeed);
	if (m_pInput->IsKeyDown(KeyCode::A))
		m_Camera.MoveRight(-deltaTime * cameraSpeed);
	if (m_pInput->IsKeyDown(KeyCode::S))
		m_Camera.MoveForward(-deltaTime * cameraSpeed);
	if (m_pInput->IsKeyDown(KeyCode::D))
		m_Camera.MoveRight(deltaTime * cameraSpeed);

	if (m_pInput->IsKeyDown(KeyCode::Space))
		m_Camera.MoveUp(deltaTime * cameraSpeed);
	if (m_pInput->IsKeyDown(KeyCode::C))
		m_Camera.MoveUp(-deltaTime * cameraSpeed);

	if (m_pInput->IsKeyDown(KeyCode::Arrowleft))
		m_Camera.Yaw(deltaTime * cameraAnglularSpeed);
	if (m_pInput->IsKeyDown(KeyCode::Arrowright))
		m_Camera.Yaw(-deltaTime * cameraAnglularSpeed);
	if (m_pInput->IsKeyDown(KeyCode::Arrowup))
		m_Camera.Pitch(deltaTime * cameraAnglularSpeed);
	if (m_pInput->IsKeyDown(KeyCode::Arrowdown))
		m_Camera.Pitch(-deltaTime * cameraAnglularSpeed);

	if (m_pInput->IsKeyPressed(KeyCode::R))
		m_Camera.RestPosAndOrient();


	if (m_pInput->IsKeyPressed(KeyCode::Ctrl))
	{
		m_IsWireMode = !m_IsWireMode;
	}

	if (m_pInput->IsKeyPressed(KeyCode::Rightshift)) 
	{
		m_IsTex = !m_IsTex;
	}

	auto& pos = m_Camera.GetPos();

}



void Application::Update(float deltaTime) noexcept
{
	ProcessInput(deltaTime);

}
void Application::OnRender() noexcept
{

	glm::mat4 model = glm::mat4(1.0f);
	
	if (m_IsWireMode)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}
	else
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}

	m_ShaderProgramTerrain->SetUniformMat4("u_Projection", m_Camera.GetProjectionMatrix());
	m_ShaderProgramTerrain->SetUniformMat4("u_View", m_Camera.GetViewMatrix());
	m_ShaderProgramTerrain->SetUniformMat4("u_Model", model);
	m_ShaderProgramTerrain->SetUniform4f("u_Color", 0.3f, 0.3f, 1.0f, 1.0f);
	m_ShaderProgramTerrain->Bind();
	m_ShaderProgramTerrain->SetUniform3f("u_lightPos", 0.0f, 400.0f, 0.0f);
	m_ShaderProgramTerrain->SetUniformVec3("u_viewPos", m_Camera.GetPos());
	m_TerrainVAO->Bind();
	glDrawElements(GL_TRIANGLES, m_TerrainEBO->getCount(), GLTypeToGLenum(GLType::UnsignedInt), reinterpret_cast<const void*>(0));


	model = glm::translate(model, glm::vec3(0.0f, 300.0f, 0.0f));

	m_ShaderProgramLit->SetUniformMat4("u_Projection", m_Camera.GetProjectionMatrix());
	m_ShaderProgramLit->SetUniformMat4("u_View", m_Camera.GetViewMatrix());
	m_ShaderProgramLit->SetUniformMat4("u_Model", model);
	m_ShaderProgramLit->SetUniform4f("u_Color", 1.0f, 0.0f, 0.0f, 1.0f);



	m_ShaderProgramLit->Bind();
	m_VAOLit->Bind();
	glDrawElements(GL_TRIANGLES, m_EBOLit->getCount(), GLTypeToGLenum(GLType::UnsignedInt), reinterpret_cast<const void*>(0));
}



void Application::GenerateTerrain() noexcept
{
	const int width = 2000;
	const int height = 2000;

	PerlinNoise noise;

	std::random_device rd;
	std::mt19937 engine(rd());

	std::normal_distribution<float> dist(10.0f,105.0f);

	std::vector<float> htMap;

	htMap.resize(width * height);

	for (auto& x : htMap)
	{
		x = dist(engine);
	}

	auto minEl = *std::min_element(htMap.begin(), htMap.end());

	for (auto& x : htMap)
	{
		x += minEl;
	}
	m_TerrainMesh.vertices.clear();
	m_TerrainMesh.indices.clear();
	for (int z = 0; z < height; z++)
	{
		for (int x = 0; x < width; x++)
		{
			Vertex vertex;
			int index = z * width + x;
			vertex.position.x = (x - width / 2.0f);
			vertex.position.z = (z - height / 2.0f);

			vertex.position.y = noise.FBMNoise(vertex.position.x, vertex.position.z,32,0.01,0.4,2.1)*100.0f + 100.0f;

			m_TerrainMesh.vertices.push_back(vertex);

		}

	}

	for (int z = 0; z < height -1; z++)
	{
		for (int x = 0; x < width -1; x++)
		{

			int topLeft = z * width + x;
			int topRight = topLeft + 1;
			int bottomLeft = (z + 1) * width + x;
			int bottomRight = bottomLeft + 1;

			m_TerrainMesh.indices.emplace_back(topLeft);
			m_TerrainMesh.indices.emplace_back(bottomLeft);
			m_TerrainMesh.indices.emplace_back(topRight);

			m_TerrainMesh.indices.emplace_back(topRight);
			m_TerrainMesh.indices.emplace_back(bottomLeft);
			m_TerrainMesh.indices.emplace_back(bottomRight);
		}
	}

	CalculateNormals(m_TerrainMesh.vertices,m_TerrainMesh.indices);
}

void Application::CalculateNormals(std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices) noexcept
{
	for (auto& vertex: vertices)
	{
		vertex.normal = glm::vec3(1.0f);
	}

	for (int i = 0; i < indices.size(); i+=3)
	{
		Vertex& v0 = vertices[indices[i]];
		Vertex& v1 = vertices[indices[i+1]];
		Vertex& v2 = vertices[indices[i+2]];


		glm::vec3 e1 = v1.position - v0.position;
		glm::vec3 e2 = v2.position - v0.position;

		glm::vec3 normal = glm::normalize(glm::cross(e1, e2));

		v0.normal = normal;
		v1.normal = normal;
		v2.normal = normal;
	}
}

