#include "pch.h"

#include "Application.h"
#include <future>
#include "Input/Input.h"
#include "Input/keyCodes.h"

#include<iostream>
#include<glad/gl.h>
#include<stb_image.h>
#include<tiny_obj_loader.h>
#include<glm/gtc/matrix_transform.hpp>


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

	size_t dataSize = 0;
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

	std::cout << "Loaded texture: " << filename << " (" << createInfo.width << "x" << createInfo.height << ", " << channels << " channels)\n";
	return true;
}

Application::Application(HINSTANCE hInstance, const std::string& appName) noexcept
	: m_hInstance(hInstance),
	m_ApplicationName(appName), m_OpenGLLoader(hInstance)
{
}



bool Application::Init() noexcept
{
	if (!RegisterWindowClass(m_hInstance))
	{
		std::cerr << "Failed to register window class\n";
		return false;
	}


	if (!m_OpenGLLoader.isLoaded())
	{
		std::cerr << "Failed to load OpenGL functions\n";
		return false;
	}

	const int windowWidth = 1200;
	const int windowHeight = 900;

	m_Window = std::make_unique<Window>(m_hInstance, s_WindowClassName, m_ApplicationName, windowWidth, windowHeight, WS_OVERLAPPEDWINDOW);

	if (!m_Window->isCreated())
	{
		std::cerr << "Failed to create window\n";
		return false;
	}

	m_RenderContext = std::make_unique<RenderContext>(*m_Window, 4, 5, true);

	if (m_RenderContext->IsNull())
	{
		std::cerr << "Failed to create OpenGL context\n";
		return false;
	}

	float aspect = m_Window->GetAspectRatio();
	m_Camera.UpdateProjection(glm::radians(90.0f), aspect, 0.1f, 500.0f);

	if (!InitGraphics())
	{
		std::cerr << "Failed to initialize graphics\n";
		return false;
	};
	if (!InitResources())
	{
		std::cerr << "Failed to initialize resources\n";
		return false;
	}

	return true;

}
void Application::Run() noexcept
{
	while (!m_Window->ShouldClose())
	{
		m_Window->ProcessMessages();
		float dt = static_cast<float>(m_Timer.elapsedAndReset());
		ProcessInput(dt);
		Update(dt);
		Render();
	}
}
bool Application::InitGraphics() noexcept
{
	std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << std::endl;
	std::cout << "OpenGL Vendor: " << glGetString(GL_VENDOR) << std::endl;
	std::cout << "OpenGL Renderer: " << glGetString(GL_RENDERER) << std::endl;

	glViewport(0, 0, m_Window->GetWidth(), m_Window->GetHeight());
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	// Enable blending for alpha transparency
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDebugMessageCallback(Application::OpenGLDebugCallback, nullptr);
	// Control which messages are reported (optional)
	glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);

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
					std::cerr << "TinyObjReader: " << reader.Error();
				}
				return false;
			}
			if (!reader.Warning().empty()) {
				std::cout << "TinyObjReader: " << reader.Warning();
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
						// No texcoord available  set default
					/*	std::cout << "No texture coordinate available for vertex " << index.vertex_index << "\n";*/
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
			std::cout << "Number of triangles: " << m_Mesh.indices.size() / 3 << "\n";
		};


	Timer loadTimer;
	
	auto modelFuture = std::async(std::launch::deferred, loadModel);
	//load both textures

	Texture2DDataCreateInfo diffuseCreateInfo, alphaCreateInfo;
	auto texDataFuturw = std::async(std::launch::deferred, LoadTextureFromFile, "assets/textures/8k_texture.png",std::ref(diffuseCreateInfo));
	auto texAlphaDataFuture = std::async(std::launch::deferred, LoadTextureFromFile, "assets/textures/8k_texture_alpha.png", std::ref(alphaCreateInfo));
	
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

	// --- Later, wait for all tasks to finish ---
	bool isModelLoaded = modelFuture.get();
	bool isTexLoaded = texDataFuturw.get();
	bool isAlphaTexLoaded = texAlphaDataFuture.get();

	if (!isModelLoaded)
	{
		std::cout << "Model Load Failed";
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
		std::cout << "Texture Load Failed" << std::endl;
		return false;
	}

	auto loadTime = loadTimer.elapsed();

	std::cout << " Took " << loadTime << "s" << "to load resources" << std::endl;
	//village model 
	m_VAO = std::make_unique<VertexArray>();
	m_VAO->Bind();
	m_VBO = std::make_unique<VertexBuffer>(m_Mesh.vertices.data(), m_Mesh.vertices.size() * sizeof(Vertex));
	m_VBO->Bind();
	m_VAO->addAttribute(0, 3, GLType::Float, false, sizeof(Vertex), (void*)0);
	m_VAO->addAttribute(1, 3, GLType::Float, false, sizeof(Vertex), reinterpret_cast<const void*>(3 * sizeof(float)));
	m_VAO->addAttribute(2, 2, GLType::Float, false, sizeof(Vertex), reinterpret_cast<const void*>(6 * sizeof(float)));
	m_EBO = std::make_unique<IndexBuffer>(m_Mesh.indices.data(), m_Mesh.indices.size() * sizeof(m_Mesh.indices[0]));
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
	m_EBOLit = std::make_unique<IndexBuffer>(cubeIndices.data(), cubeIndices.size() * sizeof(cubeIndices[0]));
	m_EBOLit->Bind();

	return true;
}
void Application::ProcessInput(float deltaTime) noexcept
{


	float cameraSpeed = 10.5f; // units per second
	float cameraAnglularSpeed = 30.0f; // degrees per second

	if (isKeyPressed(keyCode::key_w))
		m_Camera.MoveForward(deltaTime * cameraSpeed);
	if (isKeyPressed(keyCode::key_a))
		m_Camera.MoveRight(-deltaTime * cameraSpeed);
	if (isKeyPressed(keyCode::key_s))
		m_Camera.MoveForward(-deltaTime * cameraSpeed);
	if (isKeyPressed(keyCode::key_d))
		m_Camera.MoveRight(deltaTime * cameraSpeed);

	if (isKeyPressed(keyCode::key_space))
		m_Camera.MoveUp(deltaTime * cameraSpeed);
	if (isKeyPressed(keyCode::key_c))
		m_Camera.MoveUp(-deltaTime * cameraSpeed);

	if (isKeyPressed(keyCode::key_arrow_left))
		m_Camera.Yaw(deltaTime * cameraAnglularSpeed);
	if (isKeyPressed(keyCode::key_arrow_right))
		m_Camera.Yaw(-deltaTime * cameraAnglularSpeed);
	if (isKeyPressed(keyCode::key_arrow_up))
		m_Camera.Pitch(deltaTime * cameraAnglularSpeed);
	if (isKeyPressed(keyCode::key_arrow_down))
		m_Camera.Pitch(-deltaTime * cameraAnglularSpeed);

	if (isKeyPressed(keyCode::key_q))
		m_Camera.Roll(-deltaTime * cameraAnglularSpeed);
	if (isKeyPressed(keyCode::key_e))
		m_Camera.Roll(deltaTime * cameraAnglularSpeed);


	if (isKeyPressed(keyCode::key_r))
		m_Camera.RestPosAndOrient();

	if (isKeyPressed(keyCode::key_esc))
		m_Window->Close();

	auto& pos = m_Camera.GetPos();

	//std::cout << "Camera Position is" << "{" << pos.x << ","<<pos.y << "," << pos.z  << "}" << std::endl;
}

void Application::Update(float deltaTime) noexcept
{
	

}
void Application::Render() noexcept
{
	glm::mat4 model = glm::mat4(1.0f);
	m_ShaderProgram->SetUniformMat4("u_Projection", m_Camera.GetProjectionMatrix());
	m_ShaderProgram->SetUniformMat4("u_View", m_Camera.GetViewMatrix());
	m_ShaderProgram->SetUniformMat4("u_Model", model);
	m_ShaderProgram->SetUniform1i("u_Texture", 0);
	m_ShaderProgram->SetUniform1i("u_AlphaTexture", 1);
	m_ShaderProgram->SetUniform3f("u_lightPos", -2.0f, 120.0f, -20.0f);
	m_ShaderProgram->SetUniformVec3("u_viewPos", m_Camera.GetPos());

	model = glm::translate(model, glm::vec3(-2.0f, 120.0f, -20.0f));

	m_ShaderProgramLit->SetUniformMat4("u_Projection", m_Camera.GetProjectionMatrix());
	m_ShaderProgramLit->SetUniformMat4("u_View", m_Camera.GetViewMatrix());
	m_ShaderProgramLit->SetUniformMat4("u_Model", model);
	m_ShaderProgramLit->SetUniform4f("u_Color", 1.0f, 0.8f, 0.0f, 1.0f);


	m_RenderContext->clearColor(0.1f, 0.1f, 0.1f, 1.0f);
	m_ShaderProgram->Bind();
	m_VAO->Bind();
	m_Texture->Bind(0);
	m_AlphaTexture->Bind(1);

	glDrawElements(GL_TRIANGLES, m_EBO->getCount(), GLTypeToGLenum(GLType::UnsignedInt), reinterpret_cast<const void*>(0));

	m_ShaderProgramLit->Bind();
	m_VAOLit->Bind();
	glDrawElements(GL_TRIANGLES, m_EBOLit->getCount(), GLTypeToGLenum(GLType::UnsignedInt), reinterpret_cast<const void*>(0));

	m_RenderContext->Present();
}


bool Application::RegisterWindowClass(HINSTANCE hInstance) noexcept
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

void APIENTRY Application::OpenGLDebugCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam) {
	// Filter out non-critical messages (optional)
	if (type == GL_DEBUG_TYPE_OTHER || severity == GL_DEBUG_SEVERITY_LOW)
		return;

	// Output debug message
	fprintf(stderr, "OpenGL Debug Message:\n"
		"  Source: 0x%X\n  Type: 0x%X\n  ID: %d\n  Severity: 0x%X\n"
		"  Message: %s\n\n",
		source, type, id, severity, message);
}
