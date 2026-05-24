#include <GL/glew.h>
#include <fstream>
#include <sstream>
#include <iostream>
#include "ShaderProgram.h"
#include "ErrorHandling.h"


ShaderProgram::ShaderProgram(const std::string& filepath) : m_FilePath(filepath), m_RendererID(0) {

    ShaderProgramSource source = ParseShaderProgram(filepath);

    m_RendererID = CreateShaderProgram(source.VertexSource, source.FragmentSource);
}

ShaderProgram::~ShaderProgram()
{
    GLCall(glDeleteProgram(m_RendererID));
}

ShaderProgramSource ShaderProgram::ParseShaderProgram(const std::string& filepath) {
    std::ifstream stream(filepath);

    enum class ShaderType {
        NONE = -1, VERTEX = 0, FRAGMENT = 1,
    };

    ShaderType type = ShaderType::NONE;

    std::string line;
    std::stringstream ss[2];

    while (getline(stream, line)) {
        if (line.find("#shader") != std::string::npos) {

            if (line.find("vertex") != std::string::npos)
                type = ShaderType::VERTEX;
            else if (line.find("fragment") != std::string::npos)
                type = ShaderType::FRAGMENT;
        }
        else {
            ss[(int)type] << line << "\n";
        }
    }

    return { ss[0].str(), ss[1].str() };
}

unsigned int ShaderProgram::CreateShaderProgram(const std::string& vertexShader, const std::string& fragmentShader) {

    unsigned int program = glCreateProgram();
    unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
    unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);
    glValidateProgram(program);

    glDeleteShader(vs);
    glDeleteShader(fs);

    return program;
}

unsigned int ShaderProgram::CompileShader(unsigned int type, const std::string& source) {
    unsigned int shader = glCreateShader(type);
    const char* shaderSource = source.c_str();

    glShaderSource(shader, 1, &shaderSource, nullptr);
    glCompileShader(shader);

    int result;


    glGetShaderiv(shader, GL_COMPILE_STATUS, &result);

    if (result == GL_FALSE) {
        int length;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);
        char* message = (char*)alloca(length * sizeof(char));

        glGetShaderInfoLog(shader, length, &length, message);

        std::cout << "Failed to compile " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << " shader!" << std::endl;
        std::cout << message << std::endl;

        glDeleteShader(shader);
        return 0;
    }


    return shader;
}

void ShaderProgram::Bind() const
{
    GLCall(glUseProgram(m_RendererID));
}

void ShaderProgram::Unbind() const
{
    GLCall(glUseProgram(0));
}
void ShaderProgram::SetUniform4f(const std::string& name, const glm::vec4& vec)
{
    GLCall(glUniform4f(GetUniformLocation(name), vec.x, vec.y, vec.z, vec.w));
}
void ShaderProgram::SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3)
{
    GLCall(glUniform4f(GetUniformLocation(name), v0, v1, v2, v3));
}

void ShaderProgram::SetUniform1i(const std::string& name, int value)
{
    GLCall(glUniform1i(GetUniformLocation(name), value));
}

void ShaderProgram::SetUniform1f(const std::string& name, float value)
{
    GLCall(glUniform1f(GetUniformLocation(name), value));
}

void ShaderProgram::SetUniformMat4f(const std::string& name, const glm::mat4& matrix)
{
    GLCall(glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, &matrix[0][0]));
}
void ShaderProgram::SetUniform3f(const std::string& name, const glm::vec3& vec) 
{
    GLCall(glUniform3f(GetUniformLocation(name), vec.x, vec.y, vec.z));
}
void ShaderProgram::SetUniform3f(const std::string& name, float v0, float v1, float v2) 
{
    GLCall(glUniform3f(GetUniformLocation(name), v0, v1, v2));
}

int ShaderProgram::GetUniformLocation(const std::string& name)
{
    if (m_UniformLocationCache.find(name) != m_UniformLocationCache.end())
        return m_UniformLocationCache[name];

    int location = glGetUniformLocation(m_RendererID, name.c_str());
    if (location == -1)
        std::cout << "Warning: Uniform '" << name << "' doesn't exist!" << std::endl;

    m_UniformLocationCache[name] = location;
    return location;
}

