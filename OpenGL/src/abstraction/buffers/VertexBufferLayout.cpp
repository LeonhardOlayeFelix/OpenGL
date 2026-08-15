#include "VertexBufferLayout.h"

VertexBufferLayout::VertexBufferLayout(std::initializer_list<int> nums)
{
	for (float n : nums) {
		Push<float>(n);
	}
}
