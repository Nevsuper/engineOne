#pragma once
#include <algorithm>
#include <numeric>
#include<array>
#include<random>

class PerlinNoise
{
public:
	PerlinNoise(uint32_t seed = 2025) noexcept;

	double noise(double x, double y) const noexcept;
	double FBMNoise(double x, double y, int numOctaves = 8, double freq = 0.5f, double persistance = 0.5f, double multiplier = 2.0) const noexcept;
private:
	inline static constexpr std::array<std::pair<double, double>, 8> gradients{ {
	{ 1, 0}, {-1, 0}, { 0, 1}, { 0,-1},
	{ 0.707, 0.707}, {-0.707, 0.707},
	{ 0.707,-0.707}, {-0.707,-0.707}
} };

	static double fade(double t);

	static double lerp(double t, double a, double b);
	static double grad(int hash, double x, double y) noexcept;
	std::array<int, 512> permutation;
};

