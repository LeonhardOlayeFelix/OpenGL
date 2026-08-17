#include "Primitives.h"

std::array<float, 288> Primitives::CubePNT()
{
	return {
			-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f,
			 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 1.0, 0.0f,
			 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 1.0, 1.0,
			 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 1.0, 1.0,
			-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 0.0f, 1.0,
			-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f,

			-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,  0.0f, 0.0f,
			 0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,  1.0, 0.0f,
			 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,  1.0, 1.0,
			 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,  1.0, 1.0,
			-0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,  0.0f, 1.0,
			-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,  0.0f, 0.0f,

			-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f, 1.0, 0.0f,
			-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f, 1.0, 1.0,
			-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0,
			-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0,
			-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f, 0.0f, 0.0f,
			-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f, 1.0, 0.0f,

			 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f, 1.0, 0.0f,
			 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f, 1.0, 1.0,
			 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0,
			 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0,
			 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f, 0.0f, 0.0f,
			 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f, 1.0, 0.0f,

			-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0,
			 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f, 1.0, 1.0,
			 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f, 1.0, 0.0f,
			 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f, 1.0, 0.0f,
			-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f, 0.0f, 0.0f,
			-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0,

			-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0,
			 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f, 1.0, 1.0,
			 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f, 1.0, 0.0f,
			 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f, 1.0, 0.0f,
			-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f, 0.0f, 0.0f,
			-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0 };
}

std::array<float, 30> Primitives::NDCQuad()
{
	return {
		-1.0f,  1.0f, 0.0f,     0.0f, 1.0f,
		-1.0f, -1.0f, 0.0f,     0.0f, 0.0f,
		 1.0f, -1.0f, 0.0f,     1.0f, 0.0f,

		-1.0f,  1.0f, 0.0f,     0.0f, 1.0f,
		 1.0f, -1.0f, 0.0f,     1.0f, 0.0f,
		 1.0f,  1.0f, 0.0f,     1.0f, 1.0f
	};
}

std::vector<float> Primitives::SpherePNT(unsigned int sectorCount, unsigned int stackCount)
{
	std::vector<float> vertices;

	for (unsigned int i = 0; i <= stackCount; ++i)
	{
		float stackAngle = PI / 2 - i * (PI / stackCount); // +PI/2 (top) to -PI/2 (bottom)
		float xy = cosf(stackAngle);
		float z = sinf(stackAngle);

		for (unsigned int j = 0; j <= sectorCount; ++j)
		{
			float sectorAngle = j * (2 * PI / sectorCount);

			float x = xy * cosf(sectorAngle);
			float y = xy * sinf(sectorAngle);

			float u = (float)j / sectorCount;
			float v = (float)i / stackCount;

			vertices.insert(vertices.end(), {
				x, y, z,   // position
				x, y, z,   // normal (== position, unit sphere)
				u, v       // texcoord
				});
		}
	}

	return vertices;
}

std::vector<unsigned int> Primitives::SphereIndicies(unsigned int sectorCount, unsigned int stackCount)
{
	std::vector<unsigned int> indices;

	for (unsigned int i = 0; i < stackCount; ++i)
	{
		unsigned int k1 = i * (sectorCount + 1);       // start of current stack
		unsigned int k2 = k1 + sectorCount + 1;        // start of next stack

		for (unsigned int j = 0; j < sectorCount; ++j, ++k1, ++k2)
		{
			// Skip degenerate triangles at the poles
			if (i != 0)
			{
				indices.insert(indices.end(), { k1, k2, k1 + 1 });
			}
			if (i != (stackCount - 1))
			{
				indices.insert(indices.end(), { k1 + 1, k2, k2 + 1 });
			}
		}
	}

	return indices;
}
