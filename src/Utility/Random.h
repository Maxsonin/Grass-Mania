#ifndef RANDOM
#define RANDOM

#include <random>
#include <limits>
#include <algorithm>

class Random
{
public:
	Random() = delete;

	static int GenerateInt(int min, int max);
	static float GenerateFloat(float min, float max);
	static double GenerateDouble(double min, double max);

private:
	static std::random_device randomDevice;
	static std::mt19937_64 generator;
};

#endif