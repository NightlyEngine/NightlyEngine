#pragma once

#include "Core/EngineAPI.h"
#include "Core/Math.h"

namespace NL
{
	enum class ShaderType
	{
		VERTEX,
		FRAGMENT
	};

	struct NL_API ShaderProgram
	{
		ShaderProgram() = default;
		ShaderProgram(const std::string& vertexPath, const std::string& fragmentPath);

		~ShaderProgram() = default;

		void Use() const;
		void Delete() const;

		void SetUniformInt(const char* name, int value) const;
		void SetUniformFloat(const char* name, float value) const;
		void SetUniformVec3(const char* name, const Vec3& value) const;
		void SetUniformVec4(const char* name, const Vec3& value) const;
		void SetUniformMat4(const char* name, const Mat4& value) const;

		explicit operator uint32_t() const
		{
			return m_Program;
		}

	private:
		static uint32_t Compile(const ShaderType& type, const std::string& path);

		void Link() const;

		static int GetOpenGLShaderType(const ShaderType& type);

		uint32_t m_Program { };
		bool m_IsInitialized = false;
	};
}
