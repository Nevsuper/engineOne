#include "Application.h"
#include "pch.h"

#include "Application.h"
#include <future>
#include <engineOne/Core/Input/Input.h>
#include <engineOne/Core/Input/KeyCodes.h>

#include<iostream>
#include<glad/gl.h>
#include<stb_image.h>
#include<tiny_obj_loader.h>
#include<glm/gtc/matrix_transform.hpp>
#include <engineOne/Core/Engine.h>


bool LoadTextureFromFile(const std::string& filename, Texture2DDataCreateInfo& createInfo)
{
	int  channels;
	createInfo.type = pixelDataType::UNSIGNEDBYTE;
	stbi_set_flip_vertically_on_load(true); // flip vertically to match OpenGL
	unsigned char* data = stbi_load(filename.c_str(), &createInfo
		.width, &createInfo.height, &channels, 0);

	if (!data)
	{
		//clenbaup
		std::cerr << "Failed to load texture:" << filename << std::endl;
		std::cerr << "Reason: " << stbi_failure_reason() << std::endl;
		//return nullptr;
		return false;
	}

	switch (channels)
	{
	case 1:
	{
		createInfo.internalFormat = TextureInternalFormat::R8;
		break;
	};        // grayscale
	case 2:
	{
		createInfo.internalFormat = TextureInternalFormat::RG8;
		break;
	}// grayscale + alpha or similar
	case 3:
	{
		createInfo.internalFormat = TextureInternalFormat::RGB8;
		break;
	}// RGB
	case 4:
	{
		createInfo.internalFormat = TextureInternalFormat::RGBA8;
		break;       // RGBA
	}
	default:
		std::cerr << "Unsupported number of channels: " << channels << "\n";
		stbi_image_free(data);
		return false;
	}


	//move data to textureData
	  // Copy pixel data to textureData vector

	createInfo.data = data;
	createInfo.mipLevels = 1;

	LOG_INFO("Texture ({}) loaded and has {} channels", filename, channels);
	return true;
}

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

	auto loadModel = [&]() ->bool
		{
			std::string inputfile = "assets/models/Big Village.obj";
			tinyobj::ObjReaderConfig reader_config;
			reader_config.triangulate = true; // Triangulate faces;

			tinyobj::ObjReader reader;



			if (!reader.ParseFromFile(inputfile, reader_config)) {
				if (!reader.Error().empty()) {
					LOG_ERROR( "TinyObjReader: {} " , reader.Error() ) ;
				}
				return false;
			}
			if (!reader.Warning().empty()) {
				LOG_WARN("TinyObjReader: {} ", reader.Warning());
			}

			auto& attrib = reader.GetAttrib();
			auto& shapes = reader.GetShapes();
			auto& materials = reader.GetMaterials();

			m_Mesh.vertices.clear();
			m_Mesh.indices.clear();

			for (const auto& shape : shapes)
			{
				for (const auto& index : shape.mesh.indices)
				{
					Vertex vertex{};
					vertex.position = glm::vec3
					(
						attrib.vertices[3 * index.vertex_index + 0],
						attrib.vertices[3 * index.vertex_index + 1],
						attrib.vertices[3 * index.vertex_index + 2]
					);
					if (index.texcoord_index >= 0)
					{
						vertex.texCoords.x = attrib.texcoords[2 * index.texcoord_index + 0];
						vertex.texCoords.y = attrib.texcoords[2 * index.texcoord_index + 1];
					}
					else
					{
						vertex.texCoords.x = 0.0f;
						vertex.texCoords.y = 0.0f;
					}

					if (index.normal_index >= 0)
					{

						vertex.normal = glm::vec3
						(
							attrib.normals[3 * index.normal_index + 0],
							attrib.normals[3 * index.normal_index + 1],
							attrib.normals[3 * index.normal_index + 2]
						);
					}
					else
					{
						vertex.normal = glm::vec3(0.0f, 1.0f, 0.0f);
					}

					m_Mesh.vertices.push_back(vertex);
					m_Mesh.indices.push_back(static_cast<uint32_t>(m_Mesh.indices.size()));
				}
			}

			//print number of triangles
			LOG_INFO( "Number of triangles: {}" , m_Mesh.indices.size() / 3 );
			return true;
		};


	Timer loadTimer;

	auto modelFuture = std::async(std::launch::async, loadModel);
	//load both textures

	Texture2DDataCreateInfo diffuseCreateInfo, alphaCreateInfo;
	auto texDataFuture = std::async(std::launch::async, LoadTextureFromFile, "assets/textures/8k_texture.png", std::ref(diffuseCreateInfo));
	auto texAlphaDataFuture = std::async(std::launch::async, LoadTextureFromFile, "assets/textures/8k_texture_alpha.png", std::ref(alphaCreateInfo));

	//load Shader
	m_ShaderProgram = std::make_unique<ShaderProgram>("assets/shaders/vertex.glsl", "assets/shaders/fragment.glsl");


	if (!m_ShaderProgram->checkLinkStatus())
	{
		return false;
	}
	m_ShaderProgramLit = std::make_unique<ShaderProgram>("assets/shaders/vertexLit.glsl", "assets/shaders/fragmentLit.glsl");

	if (!m_ShaderProgramLit->checkLinkStatus())
	{
		return false;
	}
	m_ShaderProgramNoTex  = std::make_unique<ShaderProgram>("assets/shaders/vertex.glsl", "assets/shaders/fragmentNoTex.glsl");

	if (!m_ShaderProgramNoTex->checkLinkStatus())
	{
		return false;
	}

	// --- Later, wait for all tasks to finish ---
	bool isModelLoaded = modelFuture.get();
	bool isTexLoaded = texDataFuture.get();
	bool isAlphaTexLoaded = texAlphaDataFuture.get();

	if (!isModelLoaded)
	{
		LOG_ERROR( "Model Load Failed" ) ;
		return false;
	}

	if (isTexLoaded && isAlphaTexLoaded)
	{
		// Now safe to create your Texture and Model objects
		m_Texture = std::make_unique<Texture2D>(diffuseCreateInfo);
		m_AlphaTexture = std::make_unique<Texture2D>(alphaCreateInfo);

		stbi_image_free(diffuseCreateInfo.data);
		stbi_image_free(alphaCreateInfo.data);
	}

	else
	{
		LOG_ERROR( "Texture Load Failed" );
		return false;
	}

	auto loadTime = loadTimer.elapsed();

	LOG_DEBUG(" Took {}s to load resources", loadTime);
	//village model 
	m_VAO = std::make_unique<VertexArray>();
	m_VAO->Bind();
	m_VBO = std::make_unique<VertexBuffer>(m_Mesh.vertices.data(), m_Mesh.vertices.size() * sizeof(Vertex));
	m_VBO->Bind();
	m_VAO->addAttribute(0, 3, GLType::Float, false, sizeof(Vertex), (void*)0);
	m_VAO->addAttribute(1, 3, GLType::Float, false, sizeof(Vertex), reinterpret_cast<const void*>(3 * sizeof(float)));
	m_VAO->addAttribute(2, 2, GLType::Float, false, sizeof(Vertex), reinterpret_cast<const void*>(6 * sizeof(float)));
	m_EBO = std::make_unique<IndexBuffer>(m_Mesh.indices.data(), m_Mesh.indices.size() * sizeof(m_Mesh.indices[0]),GLType::UnsignedInt);
	m_EBO->Bind();
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
	m_EBOLit = std::make_unique<IndexBuffer>(cubeIndices.data(), cubeIndices.size() * sizeof(cubeIndices[0]),GLType::UnsignedInt);
	m_EBOLit->Bind();

	return true;
}
void Application::ProcessInput(float deltaTime) noexcept
{


	float cameraSpeed = 30.5f; // units per second
	float cameraAnglularSpeed = glm::radians(30.0f); // degrees per second

	float mouseSensitivity = 1.0f;

	int dx = m_pInput->GetMouseMoveDeltaX();
	int dy = m_pInput->GetMouseMoveDeltaY();

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

	

	m_Camera.Yaw(deltaTime * (static_cast<float>(-dx) * mouseSensitivity) * cameraAnglularSpeed);
	m_Camera.Pitch(deltaTime * (static_cast<float>(-dy) * mouseSensitivity) * cameraAnglularSpeed);

	if (m_pInput->IsKeyPressed(KeyCode::R))
		m_Camera.RestPosAndOrient();


	if (m_pInput->IsKeyPressed(KeyCode::Ctrl))
	{
		m_IsWireMode = !m_IsWireMode;
	}

	if (m_pInput->IsKeyPressed(KeyCode::Shift))
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
	if (m_IsTex)
	{
		m_ShaderProgram->SetUniformMat4("u_Projection", m_Camera.GetProjectionMatrix());
		m_ShaderProgram->SetUniformMat4("u_View", m_Camera.GetViewMatrix());
		m_ShaderProgram->SetUniformMat4("u_Model", model);
		m_ShaderProgram->SetUniform1i("u_Texture", 0);
		m_ShaderProgram->SetUniform1i("u_AlphaTexture", 1);
		m_ShaderProgram->SetUniform3f("u_lightPos", -2.0f, 120.0f, -20.0f);
		m_ShaderProgram->SetUniformVec3("u_viewPos", m_Camera.GetPos());
		m_ShaderProgram->Bind();
		m_Texture->Bind(0);
		m_AlphaTexture->Bind(1);
	}
	else
	{
		m_ShaderProgramNoTex->SetUniformMat4("u_Projection", m_Camera.GetProjectionMatrix());
		m_ShaderProgramNoTex->SetUniformMat4("u_View", m_Camera.GetViewMatrix());
		m_ShaderProgramNoTex->SetUniformMat4("u_Model", model);
		m_ShaderProgramNoTex->SetUniform3f("u_lightPos", -2.0f, 120.0f, -20.0f);
		m_ShaderProgramNoTex->SetUniformVec3("u_viewPos", m_Camera.GetPos());

		m_ShaderProgramNoTex->Bind();
	}
	m_VAO->Bind();
	
	if (m_IsWireMode)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}
	else
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
	glDrawElements(GL_TRIANGLES, m_EBO->getCount(), GLTypeToGLenum(GLType::UnsignedInt), reinterpret_cast<const void*>(0));

	model = glm::translate(model, glm::vec3(-2.0f, 120.0f, -20.0f));

	m_ShaderProgramLit->SetUniformMat4("u_Projection", m_Camera.GetProjectionMatrix());
	m_ShaderProgramLit->SetUniformMat4("u_View", m_Camera.GetViewMatrix());
	m_ShaderProgramLit->SetUniformMat4("u_Model", model);
	m_ShaderProgramLit->SetUniform4f("u_Color", 1.0f, 0.8f, 0.0f, 1.0f);



	m_ShaderProgramLit->Bind();
	m_VAOLit->Bind();
	glDrawElements(GL_TRIANGLES, m_EBOLit->getCount(), GLTypeToGLenum(GLType::UnsignedInt), reinterpret_cast<const void*>(0));
}




