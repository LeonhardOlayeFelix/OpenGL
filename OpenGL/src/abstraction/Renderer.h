#pragma once
#include "VertexArray.h"
#include "ShaderProgram.h"

class Renderer {
public:
	Renderer();
	void DrawElements(const VertexArray& vao, const ShaderProgram& shader) const;
	void DrawArray(const VertexArray& vao, const ShaderProgram& shader) const;
	void Clear() const;
};
