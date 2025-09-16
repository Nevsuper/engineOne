#include "pch.h"
#include "Shader.h"
#include<fstream>
#include<sstream>
#include<iostream>
#include <glm/gtc/type_ptr.hpp> // for value_ptr
#include "glUtils.h"

Shader::Shader(ShaderType type) noexcept
	: m_ID(0), m_Type(type)
{
	m_ID = glCreateShader(shaderTypeToGLenum(m_Type));
}

Shader::Shader(ShaderType type, std::string& data, ShaderLoadOption loadOption)
	: Shader(type)
{
	if (loadOption == ShaderLoadOption::File) {
		loadFromFile(data);
	}
	else if (loadOption == ShaderLoadOption::String) {
		loadFromString(data);
	}
}

Shader::~Shader() noexcept
{
	SafeDeleteGLShader(m_ID);
}

Shader::Shader(Shader&& other) noexcept
	:m_Type(other.m_Type), m_ID(other.m_ID)
{
	other.m_ID = 0;
}

Shader& Shader::operator=(Shader&& other) noexcept
{
	if (this != &other)
	{
		SafeDeleteGLShader(m_ID);

		m_Type = other.m_Type;
		m_ID = other.m_ID;

		other.m_ID = 0;
	}
	return *this;
}

bool Shader::loadFromFile(const std::string& filePath)
{
	std::ifstream file(filePath);
	if (!file.is_open()) {
		std::cerr << "Failed to open shader file: " << filePath << std::endl;
		return false;
	}

	std::stringstream buffer;
	buffer << file.rdbuf();
	file.close();
	auto src = buffer.str();
	return loadFromString(src);

}

bool Shader::loadFromString(const std::string& shaderSrc)
{
	const char* sources[] = { shaderSrc.c_str() };
	glShaderSource(m_ID, 1, sources, nullptr);
	glCompileShader(m_ID);
	return checkCompileStatus();
}



bool Shader::checkCompileStatus()
{
	int success;
	char infoLog[1024];

	glGetShaderiv(m_ID, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(m_ID, 1024, NULL, infoLog);
		std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << shaderTypeToString(m_Type) << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
		return false;
	}
	return true;
}

ShaderProgram::ShaderProgram() noexcept
	: m_ID(0)
{
	m_ID = glCreateProgram();
}

ShaderProgram::~ShaderProgram() noexcept
{
	SafeDeleteGLProgram(m_ID);
}

ShaderProgram::ShaderProgram(ShaderProgram&& other) noexcept
	: m_ID(other.m_ID)
{
	other.m_ID = 0;
}

ShaderProgram::ShaderProgram(const std::string& vertexPath, const std::string& fragmentPath)
	:
	m_ID(0)
{
	Shader vertexShader(ShaderType::VERTEX);
	if (!vertexShader.loadFromFile(vertexPath)) {
		std::cerr << "Failed to load vertex shader from file: " << vertexPath << std::endl;
		return;
	}
	Shader fragmentShader(ShaderType::FRAGMENT);
	if (!fragmentShader.loadFromFile(fragmentPath)) {
		std::cerr << "Failed to load fragment shader from file: " << fragmentPath << std::endl;
		return;
	}
	m_ID = glCreateProgram();
	attachShader(vertexShader);
	attachShader(fragmentShader);
	if (!link()) {
		std::cerr << "Failed to link shader program." << std::endl;
	}
}

ShaderProgram::ShaderProgram(Shader& vertexShader, Shader& fragmentShader)
{
	m_ID = glCreateProgram();
	attachShader(vertexShader);
	attachShader(fragmentShader);
	if (!link()) {
		std::cerr << "Failed to link shader program." << std::endl;
	}
}

ShaderProgram& ShaderProgram::operator=(ShaderProgram&& other) noexcept
{
	if (this != &other)
	{
		SafeDeleteGLProgram(m_ID);

		m_ID = other.m_ID;
		other.m_ID = 0;
	}
	return *this;
}


void ShaderProgram::attachShader(const Shader& shader)
{
	glAttachShader(m_ID, shader.getID());
}

bool ShaderProgram::link()
{
	glLinkProgram(m_ID);
	return checkLinkStatus();
}

void ShaderProgram::Bind() const
{
	glUseProgram(m_ID);
}

void ShaderProgram::Unbind() const
{
	glUseProgram(0);
}

bool ShaderProgram::checkLinkStatus()
{
	int success;
	char infoLog[1024];
	glGetProgramiv(m_ID, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(m_ID, 1024, NULL, infoLog);
		std::cout << "ERROR::PROGRAM_LINKING_ERROR\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
		return false;
	}
	return true;
}

void ShaderProgram::SetUniform1i(const std::string& uniformName, int v) noexcept
{
	glProgramUniform1i(m_ID, GetUniformLocation(uniformName), v);
}

void ShaderProgram::SetUniform1f(const std::string& uniformName, float v) noexcept
{
	glProgramUniform1f(m_ID,GetUniformLocation(uniformName), v);
}

void ShaderProgram::SetUniform2f(const std::string& uniformName, float v0, float v1) noexcept
{
	glProgramUniform2f(m_ID, GetUniformLocation(uniformName), v0, v1);
}

void ShaderProgram::SetUniform3f(const std::string& uniformName, float v0, float v1, float v2) noexcept
{
	glProgramUniform3f(m_ID, GetUniformLocation(uniformName), v0, v1, v2);
}

void ShaderProgram::SetUniform4f(const std::string& uniformName, float v0, float v1, float v2, float v3) noexcept
{
	glProgramUniform4f(m_ID, GetUniformLocation(uniformName), v0, v1, v2, v3);
}



void ShaderProgram::SetUniformVec2(const std::string& uniformName, const glm::vec2& vec) noexcept
{
	glProgramUniform2fv(m_ID,GetUniformLocation(uniformName), 1, glm::value_ptr(vec));
}

void ShaderProgram::SetUniformVec3(const std::string& uniformName, const glm::vec3& vec) noexcept
{
	glProgramUniform3fv(m_ID,GetUniformLocation(uniformName), 1, glm::value_ptr(vec));
}

void ShaderProgram::SetUniformVec4(const std::string& uniformName, const glm::vec4& vec) noexcept
{
	glProgramUniform4fv(m_ID,GetUniformLocation(uniformName), 1, glm::value_ptr(vec));
}

void ShaderProgram::SetUniformMat2(const std::string& uniformName, const glm::mat2& mat) noexcept
{
	glProgramUniformMatrix2fv(m_ID,GetUniformLocation(uniformName), 1, GL_FALSE, glm::value_ptr(mat));
}

void ShaderProgram::SetUniformMat3(const std::string& uniformName, const glm::mat3& mat) noexcept
{
	glProgramUniformMatrix3fv(m_ID,GetUniformLocation(uniformName), 1, GL_FALSE, glm::value_ptr(mat));
}

void ShaderProgram::SetUniformMat4(const std::string& uniformName, const glm::mat4& mat) noexcept
{
	glProgramUniformMatrix4fv(m_ID,GetUniformLocation(uniformName), 1, GL_FALSE, glm::value_ptr(mat));
}

unsigned int ShaderProgram::GetUniformLocation(const std::string& uniformName) noexcept
{
	if (m_UniformCache.find(uniformName) != m_UniformCache.end())
	{
		return m_UniformCache[uniformName];
	}
	auto loc = glGetUniformLocation(m_ID, uniformName.c_str());
	if (loc == -1)
	{
		std::cerr << "uniform" << uniformName << "  does not exist " << std::endl;
		return -1;
	}
	m_UniformCache[uniformName] = loc;
	return loc;
}
