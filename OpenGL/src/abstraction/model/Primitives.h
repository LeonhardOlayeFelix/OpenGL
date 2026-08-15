#pragma once
#include <array>

class Primitives
{

public:
	Primitives() = delete;

	static std::array<float, 288> CubePNT();
	static std::array<float, 24> NDCQuad();

};

