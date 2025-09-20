#pragma once
#include<string>
#include<unordered_map>

#include<glad/gl.h>
#include<glm/glm.hpp>
#include "engineOne/utils/utils.h"

enum class ShaderType : GLenum
{
	VERTEX = GL_VERTEX_SHADER,
	FRAGMENT = GL_FRAGMENT_SHADER,
	GEOMETRY = GL_GEOMETRY_SHADER,
	TESS_CONTROL = GL_TESS_CONTROL_SHADER,
	TESS_EVALUATION = GL_TESS_EVALUATION_SHADER,
	COMPUTE = GL_COMPUTE_SHADER
};

inline std::string shaderTypeToString(ShaderType type)
{
	switch (type)
	{
	case ShaderType::VERTEX: return "VERTEX";
	case ShaderType::FRAGMENT: return "FRAGMENT";
	case ShaderType::GEOMETRY: return "GEOMETRY";
	case ShaderType::TESS_CONTROL: return "TESS_CONTROL";
	case ShaderType::TESS_EVALUATION: return "TESS_EVALUATION";
	case ShaderType::COMPUTE: return "COMPUTE";
	default: return "UNKNOWN_SHADER_TYPE";
	}
}

constexpr GLenum shaderTypeToGLenum(ShaderType type)
{
	return enumValue(type);
}

enum class ShaderLoadOption
{
	String,
	File
};
class Shader
{
public:
	Shader(ShaderType type) noexcept;
	Shader(ShaderType type, std::string& data, ShaderLoadOption loadOption);
	Shader(Shader&&) noexcept;

	~Shader() noexcept;

	// Prevent copying
	Shader(const Shader&) = delete;
	Shader& operator=(const Shader&) = delete;


	Shader& operator=(Shader&&) noexcept;

	bool loadFromFile(const std::string& filePath);
	bool loadFromString(const std::string& shaderSrc);

	 ShaderType getType() const { return m_Type; }
	 unsigned int getID() const { return m_ID; }
private:
	bool checkCompileStatus();
private:
	unsigned int m_ID;
	ShaderType m_Type;
};


class ShaderProgram
{
public:
	ShaderProgram() noexcept;
	ShaderProgram(ShaderProgram&&)noexcept;
	ShaderProgram(const std::string& vertexPath, const std::string& fragmentPath);
	ShaderProgram(Shader& vertexShader, Shader& fragmentShader);

	~ShaderProgram() noexcept;

	// Prevent copying
	ShaderProgram(const ShaderProgram&) = delete;
	ShaderProgram& operator=(const ShaderProgram&) = delete;


	ShaderProgram& operator=(ShaderProgram&&)noexcept;

	void attachShader(const Shader& shader);
	bool link();
	void Bind() const;
	void Unbind() const;

	 unsigned int getID() const { return m_ID; }
	 bool isValid() const { return m_ID != 0; }


	bool checkLinkStatus();

	void SetUniform1i(const std::string& uniformName, int v) noexcept;


	void SetUniform1f(const std::string& uniformName, float v) noexcept;
	void SetUniform2f(const std::string& uniformName, float v0,float v1) noexcept;
	void SetUniform3f(const std::string& uniformName, float v0,float v1, float v2) noexcept;
	void SetUniform4f(const std::string& uniformName, float v0,float v1, float v2,float v3) noexcept;

	void SetUniformVec2(const std::string& uniformName, const glm::vec2& vec) noexcept;
	void SetUniformVec3(const std::string& uniformName, const glm::vec3& vec) noexcept;
	void SetUniformVec4(const std::string& uniformName, const glm::vec4& vec) noexcept;
	// specify array size
	void SetUniformMat2(const std::string& uniformName, const glm::mat2& mat) noexcept;
	void SetUniformMat3(const std::string& uniformName, const glm::mat3& mat) noexcept;
	void SetUniformMat4(const std::string& uniformName, const glm::mat4& mat) noexcept;
private:
	unsigned int GetUniformLocation(const std::string& uniformName) noexcept;
private:
	unsigned int m_ID;
	std::unordered_map<std::string, int> m_UniformCache;
};


