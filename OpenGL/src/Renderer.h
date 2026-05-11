#pragma once
#include "VertexArray.h"
#include "ShaderProgram.h"

class Renderer {
public:
	void Draw(const VertexArray& vao, const ShaderProgram& shader) const;
	void Clear() const;
};
