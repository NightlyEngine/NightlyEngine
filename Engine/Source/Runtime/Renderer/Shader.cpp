#include "Shader.h"

#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>
#include <filesystem>
#include <fstream>

#include "Core/Core.h"
#include "Core/Log.h"

namespace Nightly
{
	void Shader::Compile(uint32_t type, std::string_view path)
	{
		std::ifstream file(std::string(path).c_str());

		// Check if file exists
		NL_ASSERT(std::filesystem::exists(path), NL_TEXT("Shader file was not found: " << path), ENGINE);

		// Read file
		std::string shaderStr((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
		const char* shaderSource = shaderStr.c_str();

		m_Shader = glCreateShader(type);
		glShaderSource(m_Shader, 1, &shaderSource, nullptr);
		glCompileShader(m_Shader);

		int success;
		char infoLog[1024];
		glGetShaderiv(m_Shader, GL_COMPILE_STATUS, &success);

		if (!success)
		{
			glGetShaderInfoLog(m_Shader, 1024, nullptr, infoLog);
			NL_CORE_ERROR("Failed to compile shader: " << path, ENGINE);
			NL_CORE_ERROR(infoLog, ENGINE);
			return;
		}

		NL_CORE_INFO("Successfully compiled shader: " << path, ENGINE);
	}

	void Shader::Delete() const
	{
		glDeleteShader(m_Shader);
	}

	ShaderProgram::ShaderProgram()
	{
		m_Program = glCreateProgram();
	}

	void ShaderProgram::Attach(const Shader* shader) const
	{
		glAttachShader(m_Program, (GLuint) *shader);
	}

	void ShaderProgram::Link() const
	{
		glLinkProgram(m_Program);

		int success;
		char infoLog[1024];

		glGetProgramiv(m_Program, GL_LINK_STATUS, &success);
		if (!success)
		{
			glGetProgramInfoLog(m_Program, 1024, nullptr, infoLog);
			NL_CORE_ERROR("Failed to link shader program!", ENGINE);
			NL_CORE_ERROR(infoLog, ENGINE);
		}
	}

	void ShaderProgram::Use() const
	{
		glUseProgram(m_Program);
	}

	void ShaderProgram::Delete() const
	{
		glDeleteShader(m_Program);
	}

	void ShaderProgram::SetUniform3fv(const char* name, const Vec3& value) const
	{
		int location = glGetUniformLocation(m_Program, name);
		glUniform3fv(location, 1, glm::value_ptr(value));
	}

	void ShaderProgram::SetUniform4fv(const char* name, const Vec3& value) const
	{
		int location = glGetUniformLocation(m_Program, name);
		glUniform4fv(location, 1, glm::value_ptr(value));
	}

	void ShaderProgram::SetUniformMatrix4fv(const char* name, const Mat4& value) const
	{
		int location = glGetUniformLocation(m_Program, name);
		glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(value));
	}
}