#pragma once
#include "VertexArray.h"
#include "ShaderProgram.h"

class Renderer {
public:
	Renderer();
	void Draw(const VertexArray& vao, const ShaderProgram& shader) const;
	void Clear() const;
};
