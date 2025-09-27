#include "pch.h"
#include "AssetManager.h"
#include <engineOne/utils/utils.h>

#include<tiny_obj_loader.h>
#include<stb_image.h>

#include<memory>


std::unique_ptr<ShaderProgram> AssetManager::LoadShader(const std::string& vertexPath, const std::string& fragmentPath)
{
	std::string vertexSrc, fragmentSrc;
	if(!LoadStringFromFile(vertexPath, vertexSrc))
		return nullptr;
	if (!LoadStringFromFile(fragmentPath, fragmentSrc))
		return nullptr;

	Shader vertexShader(ShaderType::VERTEX,vertexSrc), fragmentShader(ShaderType::FRAGMENT,fragmentSrc);

	if (!vertexShader.checkCompileStatus()) return nullptr;
	if (!fragmentShader.checkCompileStatus()) return nullptr;

	std::unique_ptr<ShaderProgram> shaderProgramPtr = std::make_unique<ShaderProgram>();

	if (!shaderProgramPtr->isValid()) return nullptr;

	shaderProgramPtr->attachShader(vertexShader);
	shaderProgramPtr->attachShader(fragmentShader);

	if (!shaderProgramPtr->link()) return nullptr;

	return shaderProgramPtr;
}

bool AssetManager::LoadTexture(const std::string& path, Texture2DDataCreateInfo& createInfo)
{
	int  channels;
	createInfo.type = pixelDataType::UNSIGNEDBYTE;
	stbi_set_flip_vertically_on_load(true); // flip vertically to match OpenGL
	unsigned char* data = stbi_load(path.c_str(), &createInfo
		.width, &createInfo.height, &channels, 0);

	if (!data)
	{
		//clenbaup
		LOG_WARN( "Failed to load texture: {} -> Reason: {}", path , stbi_failure_reason());
		//return fasle;
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
		LOG_ERROR("Unsupported number of channels: {}" ,channels);
		stbi_image_free(data);
		return false;
	}

	createInfo.data = data;
	createInfo.mipLevels = 1;

	LOG_INFO("Texture ({}) loaded and has {} channels", path, channels);

	return true;

}

std::unique_ptr<Mesh> AssetManager::LoadMesh(const std::string& path)
{
	tinyobj::ObjReaderConfig reader_config;
	reader_config.triangulate = true; // Triangulate faces;

	tinyobj::ObjReader reader;



	if (!reader.ParseFromFile(path, reader_config)) {
		if (!reader.Error().empty()) {
			LOG_ERROR("TinyObjReader: {} ", reader.Error());
		}
		return nullptr;
	}
	if (!reader.Warning().empty()) {
		LOG_WARN("TinyObjReader: {} ", reader.Warning());
	}

	auto& attrib = reader.GetAttrib();
	auto& shapes = reader.GetShapes();
	auto& materials = reader.GetMaterials();

	std::unique_ptr<Mesh> meshPtr = std::make_unique<Mesh>();

	meshPtr->vertices.clear();
	meshPtr->indices.clear();

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

			meshPtr->vertices.push_back(vertex);
			meshPtr->indices.push_back(static_cast<uint32_t>(meshPtr->indices.size()));
		}
	}

	return meshPtr;

}
