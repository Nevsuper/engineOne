#pragma once
#include<engineOne/Core/Rendering/LowLevel/Shader.h>
#include<engineOne/Core/Rendering/LowLevel/Texture.h>
#include<engineOne/Core/Rendering/Mesh.h>

#include<vector>


class AssetManager 
{
public:
	AssetManager() = default;
	~AssetManager() = default;

	AssetManager(const AssetManager&) = delete;
	AssetManager(AssetManager&&) = delete;

	AssetManager& operator=(AssetManager&&) = delete;
	AssetManager& operator=(const AssetManager&) = delete;

	std::unique_ptr<ShaderProgram> LoadShader(const std::string& vertexPath, const std::string& fragmentPath);
	bool LoadTexture(const std::string& path, Texture2DDataCreateInfo& createInfo);
	std::unique_ptr<Mesh> LoadMesh(const std::string& path);

private:	
	//TODO :Hold resource handles (need to make handle system before that)
};

