#include<Windows.h>
#include<iostream>
#include<random>
#include<array>
#include<glad/gl.h>
#include<glad/wgl.h>
#define GLM_ENABLE_EXPERIMENTAL
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<stb_image.h>
#define TINYOBJLOADER_USE_MAPBOX_EARCUT
#include<tiny_obj_loader.h>
#include"Shader.h"
#include "Camera.h"
#include"VertexArray.h"
#include "Buffer.h"
#include "Texture.h"

#include "keyCodes.h"
#include "Window.h"
#include "GLLoader.h"
#include "Timer.h"
#define RETURN_FAIL_IF_FAILED(cond,msg) \
	if(!(cond)) \
{\
	MessageBeep(MB_ICONERROR); \
    MessageBox(NULL, msg, "Error", MB_OK | MB_ICONERROR);\
	return -1;\
}

#define CLEANUP_AND_RETURN_FAIL_IF_FAILED(cond,msg,cleanup) \
	if(!(cond)) \
{\
	MessageBeep(MB_ICONERROR); \
	MessageBox(NULL, msg, "Error", MB_OK | MB_ICONERROR);\
	cleanup;\
	return -1;\
}


void APIENTRY OpenGLDebugCallback(
	GLenum source, GLenum type, GLuint id, GLenum severity,
	GLsizei length, const GLchar * message, const void* userParam) {
	// Filter out non-critical messages (optional)
	if (type == GL_DEBUG_TYPE_OTHER || severity == GL_DEBUG_SEVERITY_LOW)
		return;

	// Output debug message
	fprintf(stderr, "OpenGL Debug Message:\n"
		"  Source: 0x%X\n  Type: 0x%X\n  ID: %d\n  Severity: 0x%X\n"
		"  Message: %s\n\n",
		source, type, id, severity, message);
}

bool isKeyPressed(int vKey)
{
	return (GetAsyncKeyState(vKey) & (short)0x8000) != 0;
}

bool RegisterWindowClass(HINSTANCE hInstance, const char* className, WNDPROC wndProc)
{
	WNDCLASS wc{};
	wc.hInstance = hInstance;
	wc.lpszClassName = className;
	wc.lpfnWndProc = wndProc;
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

int main()
{

	const char* windowClassName = "kkppOOK";
	HINSTANCE hInstance = GetModuleHandle(nullptr);


	RETURN_FAIL_IF_FAILED(RegisterWindowClass(hInstance, windowClassName, Window::StaticWndProc), "Failed to register window class");

	bool isLoaded = loadModernOpenGLFunctions();

	RETURN_FAIL_IF_FAILED(isLoaded == true, "Failed to load modern OpenGL functions");

	const int windowWidth = 1200;
	const int windowHeight = 900;

	Window window(hInstance, windowClassName, "OpenGL Window", windowWidth, windowHeight, WS_OVERLAPPEDWINDOW);
	RETURN_FAIL_IF_FAILED(window.isCreated() == true, "Failed to create window");


	HDC hDC = window.GetDeviceContext();

	int pixelFormatAttribs[] =
	{
		WGL_DRAW_TO_WINDOW_ARB, GL_TRUE,
		WGL_SUPPORT_OPENGL_ARB, GL_TRUE,
		WGL_DOUBLE_BUFFER_ARB, GL_TRUE,
		WGL_PIXEL_TYPE_ARB, WGL_TYPE_RGBA_ARB,
		WGL_COLOR_BITS_ARB, 32,
		WGL_DEPTH_BITS_ARB, 24,
		WGL_STENCIL_BITS_ARB, 8,
		0 // End of attributes list
	};
	int pixelFormat;
	UINT numFormats;
	int status = wglChoosePixelFormatARB(hDC, pixelFormatAttribs, NULL, 1, &pixelFormat, &numFormats);
	RETURN_FAIL_IF_FAILED(status == TRUE && numFormats > 0, "Failed to choose pixel format");

	PIXELFORMATDESCRIPTOR pfd;

	RETURN_FAIL_IF_FAILED(DescribePixelFormat(hDC, pixelFormat, sizeof(pfd), &pfd) != 0, "Failed to describe pixel format");

	RETURN_FAIL_IF_FAILED(SetPixelFormat(hDC, pixelFormat, &pfd) == TRUE, "Failed to set pixel format");

	int contextAttribs[] =
	{
		WGL_CONTEXT_MAJOR_VERSION_ARB, 4,
		WGL_CONTEXT_MINOR_VERSION_ARB, 5,
		WGL_CONTEXT_MINOR_VERSION_ARB, 5,
		WGL_CONTEXT_FLAGS_ARB, WGL_CONTEXT_DEBUG_BIT_ARB, // Request debug context
		WGL_CONTEXT_PROFILE_MASK_ARB, WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
		0 // End of attributes list
	};


	HGLRC hGLRC = wglCreateContextAttribsARB(hDC, 0, contextAttribs);

	CLEANUP_AND_RETURN_FAIL_IF_FAILED(hGLRC != nullptr, "Failed to create OpenGL context", gladLoaderUnloadGL());

	auto cleanupGL = [&]()
		{
			wglMakeCurrent(NULL, NULL);
			wglDeleteContext(hGLRC);
			unloadModernOpenGLFunctions();
		};

	

	// Test message
	glDebugMessageInsert(GL_DEBUG_SOURCE_APPLICATION, GL_DEBUG_TYPE_MARKER,
		1, GL_DEBUG_SEVERITY_NOTIFICATION, -1, "Hello OpenGL Debug!");
	CLEANUP_AND_RETURN_FAIL_IF_FAILED(wglMakeCurrent(hDC, hGLRC) == TRUE, "Failed to make OpenGL context current", cleanupGL());
	//print opengl version, venndor name,and rendere
	std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << std::endl;
	std::cout << "OpenGL Vendor: " << glGetString(GL_VENDOR) << std::endl;
	std::cout << "OpenGL Renderer: " << glGetString(GL_RENDERER) << std::endl;
	glViewport(0, 0, windowWidth, windowHeight);

	// Enable debug output
	glEnable(GL_DEBUG_OUTPUT);
	glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS); // Optional: Ensure callbacks are synchronous
	glDebugMessageCallback(OpenGLDebugCallback, nullptr);

	// Control which messages are reported (optional)
	glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
#pragma region triangle geometry

	struct Vertex
	{
		glm::vec3 position; // 3D position
		float u, v;     // Texture coordinates
	};

	//constexpr auto vertices = std::array{
	//	// Front face (z = 0.5)
	//	Vertex{ -0.5f, -0.5f,  0.5f,   0.0f, 0.0f },
	//	Vertex{  0.5f, -0.5f,  0.5f,   1.0f, 0.0f },
	//	Vertex{  0.5f,  0.5f,  0.5f,   1.0f, 1.0f },
	//	Vertex{ -0.5f,  0.5f,  0.5f,   0.0f, 1.0f },

	//	// Back face (z = -0.5)
	//	Vertex{  0.5f, -0.5f, -0.5f,   0.0f, 0.0f },
	//	Vertex{ -0.5f, -0.5f, -0.5f,   1.0f, 0.0f },
	//	Vertex{ -0.5f,  0.5f, -0.5f,   1.0f, 1.0f },
	//	Vertex{  0.5f,  0.5f, -0.5f,   0.0f, 1.0f },

	//	// Left face (x = -0.5)
	//	Vertex{ -0.5f, -0.5f, -0.5f,   0.0f, 0.0f },
	//	Vertex{ -0.5f, -0.5f,  0.5f,   1.0f, 0.0f },
	//	Vertex{ -0.5f,  0.5f,  0.5f,   1.0f, 1.0f },
	//	Vertex{ -0.5f,  0.5f, -0.5f,   0.0f, 1.0f },

	//	// Right face (x = 0.5)
	//	Vertex{  0.5f, -0.5f,  0.5f,   0.0f, 0.0f },
	//	Vertex{  0.5f, -0.5f, -0.5f,   1.0f, 0.0f },
	//	Vertex{  0.5f,  0.5f, -0.5f,   1.0f, 1.0f },
	//	Vertex{  0.5f,  0.5f,  0.5f,   0.0f, 1.0f },

	//	// Top face (y = 0.5)
	//	Vertex{ -0.5f,  0.5f,  0.5f,   0.0f, 0.0f },
	//	Vertex{  0.5f,  0.5f,  0.5f,   1.0f, 0.0f },
	//	Vertex{  0.5f,  0.5f, -0.5f,   1.0f, 1.0f },
	//	Vertex{ -0.5f,  0.5f, -0.5f,   0.0f, 1.0f },

	//	// Bottom face (y = -0.5)
	//	Vertex{ -0.5f, -0.5f, -0.5f,   0.0f, 0.0f },
	//	Vertex{  0.5f, -0.5f, -0.5f,   1.0f, 0.0f },
	//	Vertex{  0.5f, -0.5f,  0.5f,   1.0f, 1.0f },
	//	Vertex{ -0.5f, -0.5f,  0.5f,   0.0f, 1.0f },
	//};

	//// Indices for the cube (12 triangles)
	//constexpr auto indices = std::array{
	//	// Front face
	//	0u, 1u, 2u,
	//	2u, 3u, 0u,

	//	// Back face
	//	4u, 5u, 6u,
	//	6u, 7u, 4u,

	//	// Left face
	//	8u, 9u,10u,
	//   10u,11u, 8u,

	//   // Right face
	//  12u,13u,14u,
	//  14u,15u,12u,

	//  // Top face
	// 16u,17u,18u,
	// 18u,19u,16u,

	// // Bottom face
	//20u,21u,22u,
	//22u,23u,20u
	//};

	std::string inputfile = "assets/models/viking_room.obj";
	tinyobj::ObjReaderConfig reader_config;
	reader_config.triangulate = true; // Triangulate faces;

	tinyobj::ObjReader reader;



	if (!reader.ParseFromFile(inputfile, reader_config)) {
		if (!reader.Error().empty()) {
			std::cerr << "TinyObjReader: " << reader.Error();
		}
		cleanupGL();
		exit(1);
	}
	if (!reader.Warning().empty()) {
		std::cout << "TinyObjReader: " << reader.Warning();
	}
	
	auto& attrib = reader.GetAttrib();
	auto& shapes = reader.GetShapes();
	auto& materials = reader.GetMaterials();

	std::vector<Vertex> vertices;
	std::vector<uint32_t> indices;

	for(const auto &shape : shapes)
	{
		for (const auto& index : shape.mesh.indices) 
		{
			Vertex vertex{};
			vertex.position = glm::vec3(
				attrib.vertices[3 * index.vertex_index + 0],
				attrib.vertices[3 * index.vertex_index + 1],
				attrib.vertices[3 * index.vertex_index + 2]
			);
			if (index.texcoord_index >= 0)
			{
				vertex.u = attrib.texcoords[2 * index.texcoord_index + 0];
				vertex.v = attrib.texcoords[2 * index.texcoord_index + 1];
			}
			else
			{
				// No texcoord available  set default
			/*	std::cout << "No texture coordinate available for vertex " << index.vertex_index << "\n";*/
				vertex.u = 0.0f;
				vertex.v = 0.0f;
			}

			vertices.push_back(vertex);
			indices.push_back(static_cast<uint32_t>(indices.size()));
		}
	}


	float aspect = static_cast<float>(windowWidth) / static_cast<float>(windowHeight);
	CameraFPS camera(glm::radians(60.0f), aspect, 0.1f, 100.0f);

	float cameraSpeed = 1.5f; // units per second
	float cameraAnglularSpeed = 30.0f; // degrees per second

	auto handleInput = [&](float dt)
		{


			if (isKeyPressed(keyCode::key_w))
				camera.MoveForward(dt * cameraSpeed);
			if (isKeyPressed(keyCode::key_a))
				camera.MoveRight(-dt * cameraSpeed);
			if (isKeyPressed(keyCode::key_s))
				camera.MoveForward(-dt * cameraSpeed);
			if (isKeyPressed(keyCode::key_d))
				camera.MoveRight(dt * cameraSpeed);

			if (isKeyPressed(keyCode::key_space))
				camera.MoveUp(dt * cameraSpeed);
			if (isKeyPressed(keyCode::key_c))
				camera.MoveUp(-dt * cameraSpeed);

			if (isKeyPressed(keyCode::key_arrow_left))
				camera.Yaw(dt * cameraAnglularSpeed);
			if (isKeyPressed(keyCode::key_arrow_right))
				camera.Yaw(-dt * cameraAnglularSpeed);
			if (isKeyPressed(keyCode::key_arrow_up))
				camera.Pitch(dt * cameraAnglularSpeed);
			if (isKeyPressed(keyCode::key_arrow_down))
				camera.Pitch(-dt * cameraAnglularSpeed);

			if(isKeyPressed(keyCode::key_q))
				camera.Roll(-dt * cameraAnglularSpeed);
			if (isKeyPressed(keyCode::key_e))
				camera.Roll(dt * cameraAnglularSpeed);
	

			if (isKeyPressed(keyCode::key_r))
				camera.RestPosAndOrient();

			if (isKeyPressed(keyCode::key_esc))
				window.Close();

			/*std::cout << "Camera pos" << camera.GetViewMatrix()[3].x << ", "
				<< camera.GetViewMatrix()[3].y << ", "
				<< camera.GetViewMatrix()[3].z << "\n";*/

		};

	auto model = glm::mat4(1.0f);
	//make z axis up
	model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 0.0f, 1.0f));

	{

		int width, height, channels;
		stbi_set_flip_vertically_on_load(true); // flip vertically to match OpenGL
		unsigned char* data = stbi_load("assets/textures/viking_room.png", &width, &height, &channels, 0);

		if (!data)
		{
			//clenbaup
			std::cerr << "Failed to load texture: assets/textures/viking_room.png" << std::endl;
			std::cerr << "Reason: " << stbi_failure_reason() << std::endl;
			cleanupGL();
			return -1;
		}

		TextureInternalFormat format;
		switch (channels)
		{
		case 1: format = TextureInternalFormat::R8; break;        // grayscale
		case 2: format = TextureInternalFormat::RG8; break;         // grayscale + alpha or similar
		case 3: format = TextureInternalFormat::RGB8; break;        // RGB
		case 4: format = TextureInternalFormat::RGBA8; break;       // RGBA
		default:
			std::cerr << "Unsupported number of channels: " << channels << "\n";
			stbi_image_free(data);
			cleanupGL();
			return -1;
		}



		Texture2D texture(width, height, data, pixelDataType::UNSIGNEDBYTE, format);
		stbi_image_free(data);
		VertexArray VAO;
		VAO.Bind();
		VertexBuffer VBO(vertices.data(), vertices.size() * sizeof(Vertex));
		VBO.Bind();
		VAO.addAttribute(0, 3, GLType::Float, false, sizeof(Vertex), (void*)0);
		VAO.addAttribute(1, 2, GLType::Float, false, sizeof(Vertex), reinterpret_cast<const void*>(3 * sizeof(float)));
		IndexBuffer EBO(indices.data(), indices.size() * sizeof(indices[0]));
		EBO.Bind();
		VAO.Unbind();
		VBO.Unbind();
		EBO.Unbind();

#pragma endregion

		ShaderProgram Shader("assets/shaders/vertex.glsl", "assets/shaders/fragment.glsl");
		Shader.Bind();
		wglSwapIntervalEXT(0);
	

		float r = 1.0f, g = 1.0f, b = 1.0f;
		std::random_device rd;
		std::mt19937 generator(rd());
		std::uniform_real_distribution<float> dist(0.0f, 1.0f);
		Shader.SetUniformMat4("u_Projection", camera.GetProjectionMatrix());
		//Shader.SetUniformMat4("u_Projection", pj);
		Timer timer, fpsTimer;
		int frameCount = 0;
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);
		glDepthFunc(GL_LESS);
		glEnable(GL_DEPTH_TEST);
		while (!window.ShouldClose())
		{
			window.ProcessMessages();

			double deltaTime = timer.elapsedAndReset();
			frameCount++;
			auto fpsTime = fpsTimer.elapsed();
			if (fpsTime >= 1.0) {
				double fps = frameCount / fpsTime;   // average FPS
				std::cout << "FPS: " << fps << "\r";
				std::cout.flush();
				frameCount = 0;
				fpsTimer.reset();
				r = dist(generator);
				g = dist(generator);
				b = dist(generator);
			}

			handleInput(static_cast<float>(deltaTime));
			//rotate cube
			/*Shader.SetUniform4f("u_Color", r, g, b, 1.0f);*/

			//Rendering code goes here
			glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			Shader.Bind();
			VAO.Bind();
			texture.Bind();
			Shader.SetUniformMat4("u_View", camera.GetViewMatrix());
			Shader.SetUniformMat4("u_Model", model);
			glDrawElements(GL_TRIANGLES, indices.size(), GLTypeToGLenum(GLType::UnsignedInt), reinterpret_cast<const void*>(0));
			SwapBuffers(hDC);
		}

	}
	cleanupGL();

	std::cout << "Underlying type of ShaderType is: " << typeid(std::underlying_type_t<ShaderType>).name() << std::endl;
	return 0;
}


