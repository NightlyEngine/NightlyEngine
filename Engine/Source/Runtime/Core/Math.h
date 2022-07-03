#pragma once

#include "EngineAPI.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/quaternion.hpp>

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
	};
}
