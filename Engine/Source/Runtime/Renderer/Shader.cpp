#include "Shader.h"

#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>
#include <filesystem>
#include <fstream>

#include "Core/Core.h"
#include "Core/Log.h"

// FIXME: Probably obsolete, since Initialize() has been removed
// Asserts if the shader has not been initialized yet.
#define CHECK_STATE() NL_ASSERT(m_IsInitialized, "Forgot to call Initialize() on ShaderProgram.", ENGINE)

namespace NL
{
	ShaderProgram::ShaderProgram(const std::string& vertexPath, const std::string& fragmentPath)
			: m_Program(glCreateProgram())
	{
		auto vert = Compile(ShaderType::VERTEX, vertexPath);
		auto frag = Compile(ShaderType::FRAGMENT, fragmentPath);

		m_IsInitialized = true;

		glAttachShader(m_Program, (GLuint) vert);
		glAttachShader(m_Program, (GLuint) frag);

		glDeleteShader(vert);
		glDeleteShader(frag);

		Link();
	}

	void ShaderProgram::Use() const
	{
		CHECK_STATE();
		glUseProgram(m_Program);
	}

	void ShaderProgram::Delete() const
	{
		CHECK_STATE();
		glDeleteShader(m_Program);
	}

	void ShaderProgram::SetUniformInt(const char* name, int value) const
	{
		CHECK_STATE();

		int location = glGetUniformLocation(m_Program, name);
		glUniform1i(location, value);
	}

	void ShaderProgram::SetUniformFloat(const char* name, float value) const
	{
		CHECK_STATE();

		int location = glGetUniformLocation(m_Program, name);
		glUniform1f(location, value);
	}

	void ShaderProgram::SetUniformVec3(const char* name, const Vec3& value) const
	{
		CHECK_STATE();

		int location = glGetUniformLocation(m_Program, name);
		glUniform3fv(location, 1, glm::value_ptr(value));
	}

	void ShaderProgram::SetUniformVec4(const char* name, const Vec3& value) const
	{
		CHECK_STATE();

		int location = glGetUniformLocation(m_Program, name);
		glUniform4fv(location, 1, glm::value_ptr(value));
	}

	void ShaderProgram::SetUniformMat4(const char* name, const Mat4& value) const
	{
		CHECK_STATE();

		int location = glGetUniformLocation(m_Program, name);
		glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(value));
	}

	uint32_t ShaderProgram::Compile(const ShaderType& type, const std::string& path)
	{
		std::ifstream file(std::string(path).c_str());

		// Check if file exists
		if (!std::filesystem::exists(path))
		{
			NL_CORE_FATAL("Shader file was not found: " << path, ENGINE);
		}

		// Read file
		std::string shaderStr((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
		const char* shaderSource = shaderStr.c_str();

		auto m_Shader = glCreateShader(GetOpenGLShaderType(type));
		glShaderSource(m_Shader, 1, &shaderSource, nullptr);
		glCompileShader(m_Shader);

		int success;
		char infoLog[1024];
		glGetShaderiv(m_Shader, GL_COMPILE_STATUS, &success);

		if (success)
		{
			NL_CORE_INFO("Successfully compiled shader: " << path, ENGINE);
		}
		else
		{
			glGetShaderInfoLog(m_Shader, 1024, nullptr, infoLog);
			NL_CORE_ERROR("Failed to compile shader: " << path, ENGINE);
			NL_CORE_ERROR(infoLog, ENGINE);
		}

		return m_Shader;
	}

	void ShaderProgram::Link() const
	{
		CHECK_STATE();
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

	int ShaderProgram::GetOpenGLShaderType(const ShaderType& type)
	{
		switch (type)
		{
			case ShaderType::VERTEX: return GL_VERTEX_SHADER;
			case ShaderType::FRAGMENT: return GL_FRAGMENT_SHADER;
		}
	}
}