#pragma once
#include <array>
#include <vector>

class Primitives
{

public:
	Primitives() = delete;

	static std::array<float, 288> CubePNT();
	static std::array<float, 30> NDCQuad();
	static std::vector<float> SpherePNT(unsigned int sectorCount = 36, unsigned int stackCount = 18);
	static std::vector<unsigned int> SphereIndicies(unsigned int sectorCount = 36, unsigned int stackCount = 18);
private:
	static constexpr float PI = 3.14159265358979323846f;

};

