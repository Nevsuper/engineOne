#include "PerlinNoise.h"

 PerlinNoise::PerlinNoise(uint32_t seed) noexcept
{
	std::iota(permutation.begin(), permutation.begin() + 256, 0u);

	std::mt19937 engine(seed); // Mersenne Twister RNG

	for (int i = 255u; i >= 0; --i)
	{
		std::uniform_int_distribution<uint32_t> dist(0, i);
		int j = dist(engine);
		std::swap(permutation[i], permutation[j]);
	}
	std::copy(permutation.begin(), permutation.begin() + 256, permutation.begin() + 256);
}

 double PerlinNoise::noise(double x, double y) const noexcept
{
	uint32_t X = static_cast<uint32_t>(std::floor(x)) & 255u;
	uint32_t Y = static_cast<uint32_t>(std::floor(y)) & 255u;

	double xOffset = x - std::floor(x);
	double yOffset = y - std::floor(y);

	double u = fade(xOffset);
	double v = fade(yOffset);

	auto index1 = permutation[X];
	auto index2 = permutation[X + 1];


	// Hash coordinates of corners
	int topLeft = permutation[index1 + Y];
	int bottomLeft = permutation[index1 + Y + 1];
	int topRight = permutation[index2 + Y];
	int bottomRight = permutation[index2 + Y + 1];

	// Gradient dot products & linear interpolation
	double res = lerp(v,
		lerp(u, grad(topLeft, xOffset, yOffset), grad(topRight, xOffset - 1, yOffset)),
		lerp(u, grad(bottomLeft, xOffset, yOffset - 1), grad(bottomRight, xOffset - 1, yOffset - 1))
	);

	return res;

}

 double PerlinNoise::FBMNoise(double x, double y,  int numOctaves, double freq , double persistance, double multiplier ) const noexcept
 {
	 double val = 0.0;
	 double maxAmplitude = 0.0;
	 double amplitude = 1.0;
	 for (int i = 0; i < numOctaves; i++)
	 {
		 val += noise(x * freq, y * freq) * amplitude;
		 maxAmplitude += amplitude;

		 amplitude *= persistance;
		 freq *= multiplier;
	 }
	 return val / maxAmplitude;
 }

 double PerlinNoise::fade(double t) {
	return t * t * t * (t * (t * 6 - 15) + 10);
}

 double PerlinNoise::lerp(double t, double a, double b) {
	return a + t * (b - a);
}

 double PerlinNoise::grad(int hash, double x, double y) noexcept {
	const auto& g = gradients[hash & 7];
	return g.first * x + g.second * y;
}
