#include "Math.h"

#include <random>

namespace Nightly
{
	int Math::Random(int min, int max)
	{
		std::random_device device;
		std::uniform_int_distribution distribution(min, max);

		return distribution(device);
	}

	float Math::Random(float min, float max)
	{
		std::random_device device;
		std::uniform_real_distribution distribution(min, max);

		return distribution(device);
	}

	double Math::Random(double min, double max)
	{
		std::random_device device;
		std::uniform_real_distribution distribution(min, max);

		return distribution(device);
	}
}