#pragma once
#include "Common.h"
#include <optional>

struct Vertex {
    glm::vec3 Position;
    glm::vec3 Normal;
    glm::vec2 TexCoords;
};

struct MeshTexture {
    unsigned int id;
    std::string type;
    std::string path;
};

class Mesh
{
public:
    std::vector<Vertex>       Vertices;
    std::vector<unsigned int> Indices;
    std::vector<MeshTexture>  Textures;

    Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<MeshTexture> textures);
    Mesh(const Mesh&) = delete;
    Mesh& operator=(const Mesh&) = delete;
    Mesh(Mesh&&) = default;
    Mesh& operator=(Mesh&&) = default;

    void Draw(ShaderProgram& shader);
    void DrawInstanced(ShaderProgram& shader, unsigned int count);
    void SetTextures(ShaderProgram& shader);

private:
    std::optional<VertexArray> m_Vao;
    std::optional<VertexBuffer> m_Vbo;
    std::optional<IndexBuffer> m_Ibo;

    void setupMesh();
public:
    inline VertexArray& GetVAO() { return m_Vao.value(); }
};

