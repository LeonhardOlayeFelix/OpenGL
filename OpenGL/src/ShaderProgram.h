#pragma once
#include <string>

struct ShaderProgramSource {
	std::string VertexSource;
	std::string FragmentSource;
};

class ShaderProgram
{
private:
	std::string m_FilePath;
	unsigned int m_RendererID;
public:
	ShaderProgram(const std::string& filename);
	~ShaderProgram();

	void SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3);

	void Bind() const;
	void Unbind() const;
private:
	ShaderProgramSource ParseShaderProgram(const std::string& filepath);
	unsigned int CreateShaderProgram(const std::string& vertexShader, const std::string& fragmentShader);
	unsigned int CompileShader(unsigned int type, const std::string& source);
	int GetUniformLocation(const std::string& name);
};

