#pragma once

#include "EngineAPI.h"
#include "Core.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/quaternion.hpp>

// Unwraps a Vec2 into its x and y components.
#define NL_UNWRAP_XY(vec2) vec2.x, vec2.y

// Unwraps a Vec3 into its x y, and z components.
#define NL_UNWRAP_XYZ(vec3) vec3.x, vec3.y, vec3.z

// Unwraps a Vec4 into its x, y, z and w components.
#define NL_UNWRAP_XYZW(vec4) vec4.x, vec4.y, vec4.z, vec4.w

namespace NL
{
	using Vec2 = glm::vec2;
	using Vec3 = glm::vec3;
	using Vec4 = glm::vec4;
	using Mat4 = glm::mat4;

	class NL_API Math
	{
	public:
		Math() = default;
		~Math() = default;

		// Returns a randomly generated integer between
		// min (inclusive) and max (inclusive).
		static int Random(int min, int max);

		// Returns a randomly generated float between
		// min (inclusive) and max (inclusive).
		static float Random(float min, float max);

		// Returns a randomly generated double between
		// min (inclusive) and max (inclusive).
		static double Random(double min, double max);

		// Returns whether an int is between
		// min (inclusive) and max (inclusive).
		static bool IsInRange(int i, int min, int max)
		{
			return i >= min && i <= max;
		}

		// Returns whether a float is between
		// min (inclusive) and max (inclusive).
		static bool IsInRange(float f, float min, float max)
		{
			return f >= min && f <= max;
		}
	};

	class Bounds2D
	{
	public:
		Bounds2D() : m_Bounds(0.0f)
		{
		}

		Bounds2D(float left, float right, float bottom, float top)
				: m_Bounds(left, right, bottom, top)
		{
		}

		NL_NODISCARD inline float GetLeft() const
		{
			return m_Bounds.x;
		}

		NL_NODISCARD inline float GetRight() const
		{
			return m_Bounds.y;
		}

		NL_NODISCARD inline float GetBottom() const
		{
			return m_Bounds.z;
		}

		NL_NODISCARD inline float GetTop() const
		{
			return m_Bounds.w;
		}

		NL_NODISCARD inline Vec4 GetBounds() const
		{
			return m_Bounds;
		}

	private:
		Vec4 m_Bounds;
	};

	// A color represents the RGBA channels from 0 to 1.
	class Color
	{
	public:
		explicit Color(float r = 0.0f, float g = 0.0f, float b = 0.0f, float a = 0.0f)
				: m_Color(r, g, b, a)
		{
		}

		static Color Black()
		{
			return Color(0, 0, 0, 1);
		}

		static Color White()
		{
			return Color(1, 1, 1, 1);
		}

		static Color Red()
		{
			return Color(1, 0, 0, 1);
		}

		static Color Green()
		{
			return Color(0, 1, 0, 1);
		}

		static Color Blue()
		{
			return Color(0, 0, 1, 1);
		}

		static Color Cyan()
		{
			return Color(0, 1, 1, 1);
		}

		static Color Yellow()
		{
			return Color(1, 1, 0, 1);
		}

		static Color Magenta()
		{
			return Color(1, 0, 1, 1);
		}

		NL_NODISCARD const float& GetRed() const
		{
			return m_Color.x;
		}

		NL_NODISCARD const float& GetGreen() const
		{
			return m_Color.y;
		}

		NL_NODISCARD const float& GetBlue() const
		{
			return m_Color.z;
		}

		NL_NODISCARD const float& GetAlpha() const
		{
			return m_Color.w;
		}

		NL_NODISCARD Vec3 GetRGB() const
		{
			return { GetRed(), GetGreen(), GetBlue() };
		}

		void SetRGB(float r, float g, float b)
		{
			if (Math::IsInRange(r, 0.0f, 1.0f))
			{
				m_Color.x = r;
			}

			if (Math::IsInRange(g, 0.0f, 1.0f))
			{
				m_Color.y = g;
			}

			if (Math::IsInRange(b, 0.0f, 1.0f))
			{
				m_Color.z = b;
			}
		}

		NL_NODISCARD const Vec4& GetRGBA() const
		{
			return m_Color;
		}

		void SetRGBA(float r, float g, float b, float a)
		{
			SetRGB(r, g, b);

			if (Math::IsInRange(a, 0.0f, 1.0f))
			{
				m_Color.w = a;
			}
		}

		NL_NODISCARD Vec4 GetRGBA255() const
		{
			return m_Color * Vec4(255.0f);
		}

		NL_NODISCARD std::string ToString() const
		{
			std::stringstream stream;
			stream << "R: " << GetRed() << " G: " << GetGreen() << " B: " << GetBlue() << " A: " << GetAlpha();
			return stream.str();
		}

		explicit operator Vec4() const
		{
			return m_Color;
		}

	private:
		Vec4 m_Color;
	};
}
