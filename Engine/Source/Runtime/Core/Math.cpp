#include "Math.h"

#include <random>

namespace Nightly
{
	static std::random_device m_Device;

	int Math::Random(int min, int max)
	{
		std::uniform_int_distribution distribution(min, max);
		return distribution(m_Device);
	}

	float Math::Random(float min, float max)
	{
		std::uniform_real_distribution distribution(min, max);
		return distribution(m_Device);
	}

	double Math::Random(double min, double max)
	{
		std::uniform_real_distribution distribution(min, max);
		return distribution(m_Device);
	}
}