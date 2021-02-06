#include "Random.h"

Random::Random()
{
	m_MTwister.seed(randomDevice());
}

int Random::generate(int minim, int maxim)
{
	const std::uniform_int_distribution<int> dist(minim, maxim);
	return dist(m_MTwister);
}

float Random::generate(float minim, float maxim)
{
	const std::uniform_real_distribution<float> dist(minim, maxim);
	return dist(m_MTwister);
}

