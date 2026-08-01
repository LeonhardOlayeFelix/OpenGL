#pragma once
#include <string>
#include <unordered_map>
#include "vendor/glm/glm.hpp"

struct ShaderProgramSource {
	std::string VertexSource;
	std::string FragmentSource;
};

class ShaderProgram
{
private:
	std::string m_FilePath;
	unsigned int m_RendererID;
	std::unordered_map<std::string, int> m_UniformLocationCache;
public:
	ShaderProgram(const std::string& filename);
	~ShaderProgram();

	void SetUniform4f(const std::string& name, const glm::vec4& vec);
	void SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3);
	void SetUniform3f(const std::string& name, const glm::vec3& vec);
	void SetUniform2f(const std::string& name, const glm::vec2& vec);
	void SetUniform3f(const std::string& name, float v0, float v1, float v2);
	void SetUniform2f(const std::string& name, float v0, float v1);
	void SetUniform1i(const std::string& name, int value);
	void SetUniform1f(const std::string& name, float value);
	void SetUniformMat4f(const std::string& name, const glm::mat4& matrix);
	void SetUniformMat3f(const std::string& name, const glm::mat3& matrix);
	void SetUniform1fv(const std::string& name, int count, const float* values);
	void SetUniformBlockBinding(const std::string& name, int bindingPoint);
	void Bind() const;
	void Unbind() const;
	inline int GetID() { return m_RendererID; }
private:
	ShaderProgramSource ParseShaderProgram(const std::string& filepath);
	unsigned int CreateShaderProgram(const std::string& vertexShader, const std::string& fragmentShader);
	unsigned int CompileShader(unsigned int type, const std::string& source);
	int GetUniformLocation(const std::string& name);
	int GetUniformBlockIndex(const std::string& name);
};

