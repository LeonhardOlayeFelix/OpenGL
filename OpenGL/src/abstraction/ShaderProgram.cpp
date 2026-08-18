#include <GL/glew.h>
#include <fstream>
#include <sstream>
#include <iostream>
#include "ShaderProgram.h"
#include "ErrorHandling.h"


ShaderProgram::ShaderProgram(const std::string& filepath) : m_FilePath(filepath), m_RendererID(0) {

    ShaderProgramSource source = ParseShaderProgram(filepath);

    m_RendererID = CreateShaderProgram(source);
}

ShaderProgram::~ShaderProgram()
{
    if (m_RendererID != 0)
        GLCall(glDeleteProgram(m_RendererID));
}

ShaderProgramSource ShaderProgram::ParseShaderProgram(const std::string& filepath) {
    std::ifstream stream(filepath);

    enum class ShaderType {
        NONE = -1, VERTEX = 0, FRAGMENT = 1, GEOMETRY = 2
    };

    ShaderType type = ShaderType::NONE;

    std::string line;
    std::stringstream ss[3];

    while (getline(stream, line)) {
        if (line.find("#shader") != std::string::npos) {

            if (line.find("vertex") != std::string::npos)
                type = ShaderType::VERTEX;
            else if (line.find("fragment") != std::string::npos)
                type = ShaderType::FRAGMENT;
            else if (line.find("geometry") != std::string::npos)
                type = ShaderType::GEOMETRY;
        }
        else {
            ss[(int)type] << line << "\n";
        }
    }
    return { ss[0].str(), ss[1].str(), ss[2].str()};
    
}

unsigned int ShaderProgram::CreateShaderProgram(const ShaderProgramSource& source) {

    unsigned int program = glCreateProgram();

    unsigned int vs = AttachShaderToProgram(program, source.VertexSource, GL_VERTEX_SHADER);
    unsigned int fs = AttachShaderToProgram(program, source.FragmentSource, GL_FRAGMENT_SHADER);

    unsigned int gs = 0;
    if (!source.GeometrySource.empty()) {
        gs = AttachShaderToProgram(program, source.GeometrySource, GL_GEOMETRY_SHADER);
    }

    glLinkProgram(program);
    glValidateProgram(program);

    glDeleteShader(vs);
    glDeleteShader(fs);

    if (gs != 0) {
        glDeleteShader(gs);
    }

    return program;
}

unsigned int ShaderProgram::AttachShaderToProgram(unsigned int program, const std::string& shaderSource, GLenum type)
{
    unsigned int compiledShader = CompileShader(type, shaderSource);
    glAttachShader(program, compiledShader);
    return compiledShader;
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
ShaderProgram::ShaderProgram(ShaderProgram&& other) noexcept
    : m_FilePath(std::move(other.m_FilePath)),
    m_RendererID(other.m_RendererID),
    m_UniformLocationCache(std::move(other.m_UniformLocationCache))
{
    other.m_RendererID = 0;
}
ShaderProgram& ShaderProgram::operator=(ShaderProgram&& other) noexcept
{
    if (this != &other)
    {
        glDeleteProgram(m_RendererID);

        m_FilePath = std::move(other.m_FilePath);
        m_RendererID = other.m_RendererID;
        m_UniformLocationCache = std::move(other.m_UniformLocationCache);

        other.m_RendererID = 0;
    }
    return *this;
}
void ShaderProgram::SetUniform4f(const std::string& name, const glm::vec4& vec)
{
    GLCall(glProgramUniform4f(m_RendererID, GetUniformLocation(name), vec.x, vec.y, vec.z, vec.w));
}
void ShaderProgram::SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3)
{
    GLCall(glProgramUniform4f(m_RendererID, GetUniformLocation(name), v0, v1, v2, v3));
}

void ShaderProgram::SetUniform1i(const std::string& name, int value)
{
    GLCall(glProgramUniform1i(m_RendererID, GetUniformLocation(name), value));
}

void ShaderProgram::SetUniform1f(const std::string& name, float value)
{
    GLCall(glProgramUniform1f(m_RendererID, GetUniformLocation(name), value));
}

void ShaderProgram::SetUniformMat4f(const std::string& name, const glm::mat4& matrix)
{
    GLCall(glProgramUniformMatrix4fv(m_RendererID, GetUniformLocation(name), 1, GL_FALSE, &matrix[0][0]));
}
void ShaderProgram::SetUniformMat3f(const std::string& name, const glm::mat3& matrix)
{
    GLCall(glProgramUniformMatrix3fv(m_RendererID, GetUniformLocation(name), 1, GL_FALSE, &matrix[0][0]));
}
void ShaderProgram::SetUniform1fv(const std::string& name, int count, const float* values)
{
    GLCall(glProgramUniform1fv(m_RendererID, GetUniformLocation(name), count, values))
}

void ShaderProgram::SetUniform1PointLight(const std::string& name, const PointLight& pointLight)
{
    SetUniform3f(name + ".Ambient", pointLight.Ambient);
    SetUniform3f(name + ".Diffuse", pointLight.Diffuse);
    SetUniform3f(name + ".Specular", pointLight.Specular);
    SetUniform1f(name + ".Near", pointLight.Near);
    SetUniform1f(name + ".Far", pointLight.Far);
    SetUniform3f(name + ".Position", pointLight.Position);
    SetUniform1f(name + ".Kc", pointLight.Kc);
    SetUniform1f(name + ".Kl", pointLight.Kl);
    SetUniform1f(name + ".Kq", pointLight.Kq);
}

void ShaderProgram::SetUniform1DirectionalLight(const std::string& name, const DirectionalLight& directionalLight)
{
    SetUniform3f(name + ".Ambient", directionalLight.Ambient);
    SetUniform3f(name + ".Diffuse", directionalLight.Diffuse);
    SetUniform3f(name + ".Specular", directionalLight.Specular);
    SetUniform1f(name + ".Near", directionalLight.Near);
    SetUniform1f(name + ".Far", directionalLight.Far);
    SetUniform3f(name + ".Direction", directionalLight.Direction);
}

void ShaderProgram::SetUniform1SpotLight(const std::string& name, const SpotLight& spotLight)
{
    SetUniform3f(name + ".Ambient", spotLight.Ambient);
    SetUniform3f(name + ".Diffuse", spotLight.Diffuse);
    SetUniform3f(name + ".Specular", spotLight.Specular);
    SetUniform1f(name + ".Near", spotLight.Near);
    SetUniform1f(name + ".Far", spotLight.Far);
    SetUniform3f(name + ".Position", spotLight.Position);
    SetUniform3f(name + ".Direction", spotLight.Direction);
    SetUniform1f(name + ".Kc", spotLight.Kc);
    SetUniform1f(name + ".Kl", spotLight.Kl);
    SetUniform1f(name + ".Kq", spotLight.Kq);
    SetUniform1f(name + ".CutOff", glm::cos(glm::radians(spotLight.CutOff)));
    SetUniform1f(name + ".OuterCutOff", glm::cos(glm::radians(spotLight.OuterCutOff)));
}

void ShaderProgram::SetUniform1Material(const std::string& name, const Material& material)
{
    SetUniform1i(name + ".DiffuseMap", material.DiffuseMap.GetLastBoundSlot());
    SetUniform1i(name + ".SpecularMap", material.SpecularMap.GetLastBoundSlot());
    SetUniform1i(name + ".NormalMap", material.NormalMap.GetLastBoundSlot());
    SetUniform1i(name + ".EmissiveMap", material.EmissiveMap.GetLastBoundSlot());
    SetUniform1f(name + ".Shininess", material.Shininess);
    SetUniform1i(name + ".Blinn", material.Blinn);
}

void ShaderProgram::SetUniformBlockBinding(const std::string& name, int bindingPoint)
{
    GLCall(glUniformBlockBinding(m_RendererID, GetUniformBlockIndex(name), bindingPoint))
}

void ShaderProgram::SetUniform3f(const std::string& name, const glm::vec3& vec) 
{
    GLCall(glProgramUniform3f(m_RendererID, GetUniformLocation(name), vec.x, vec.y, vec.z));
}
void ShaderProgram::SetUniform2f(const std::string& name, const glm::vec2& vec) 
{
    GLCall(glProgramUniform2f(m_RendererID, GetUniformLocation(name), vec.x, vec.y));
}
void ShaderProgram::SetUniform3f(const std::string& name, float v0, float v1, float v2) 
{
    GLCall(glProgramUniform3f(m_RendererID, GetUniformLocation(name), v0, v1, v2));
}
void ShaderProgram::SetUniform2f(const std::string& name, float v0, float v1) 
{
    GLCall(glProgramUniform2f(m_RendererID, GetUniformLocation(name), v0, v1));
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

int ShaderProgram::GetUniformBlockIndex(const std::string& name)
{
    int index = glGetUniformBlockIndex(m_RendererID, name.c_str());
    if (index == -1)
        std::cout << "Warning: Uniform block '" << name << "' doesn't exist!" << std::endl;

    return index;
}

