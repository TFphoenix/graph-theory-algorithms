#pragma once
#include <random>

class Random
{
public:
	Random();
	int generate(int minim, int maxim);
	float generate(float minim, float maxim);

private:
	std::random_device randomDevice;
	std::mt19937 m_MTwister;
};

