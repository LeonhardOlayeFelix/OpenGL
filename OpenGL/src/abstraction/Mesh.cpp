#include "Mesh.h"
#include <iostream>

Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<MeshTexture> textures)
{
    this->Vertices = vertices;
    this->Indices = indices;
    this->Textures = textures;

    setupMesh();
}

void Mesh::Draw(ShaderProgram& shader)
{
    unsigned int diffuseNr = 1;
    unsigned int specularNr = 1;


    for (unsigned int i = 0; i < Textures.size(); i++) {
        glActiveTexture(GL_TEXTURE0 + i);

        std::string number;
        std::string name = Textures[i].type;

        if (name == "texture_diffuse")
            number = std::to_string(diffuseNr++);
        else if (name == "texture_specular")
            number = std::to_string(specularNr++);

        std::string uniformName = "u_Material." + name + number;
        shader.SetUniform1i(uniformName.c_str(), i);
        glBindTexture(GL_TEXTURE_2D, Textures[i].id);
    }
    glActiveTexture(GL_TEXTURE0);

    m_Vao->Bind();
    glDrawElements(GL_TRIANGLES, Indices.size(), GL_UNSIGNED_INT, 0);
    m_Vao->Unbind();
}

void Mesh::DrawInstanced(ShaderProgram& shader, unsigned int count)
{
    SetTextures(shader);

    m_Vao->Bind();
    glDrawElementsInstanced(GL_TRIANGLES, Indices.size(), GL_UNSIGNED_INT, 0, count);
    m_Vao->Unbind();
}

void Mesh::setupMesh()
{
    m_Vao.emplace();
    m_Vao->Bind();

    m_Vbo.emplace(Vertices.data(), Vertices.size() * sizeof(Vertex));
    m_Vbo->Bind();

    VertexBufferLayout vbl;
    vbl.Push<float>(3);
    vbl.Push<float>(3);
    vbl.Push<float>(2);

    m_Vao->RecordVBOLayout(*m_Vbo, vbl);

    m_Ibo.emplace(Indices.data(), Indices.size());
    m_Vao->RecordIndexBuffer(m_Ibo.value());

    m_Vao->Unbind();

}

/// <summary>
/// Sets the texture slots needed for this mesh to be drawn with the correct texture.
/// </summary>
/// <param name="shader"></param>
void Mesh::SetTextures(ShaderProgram& shader)
{
    unsigned int diffuseNr = 1;
    unsigned int specularNr = 1;

    for (unsigned int i = 0; i < Textures.size(); i++) {
        glActiveTexture(GL_TEXTURE0 + i);

        std::string number;
        std::string name = Textures[i].type;

        if (name == "texture_diffuse")
            number = std::to_string(diffuseNr++);
        else if (name == "texture_specular")
            number = std::to_string(specularNr++);

        std::string uniformName = "u_Material." + name + number;
        shader.SetUniform1i(uniformName.c_str(), i);
        glBindTexture(GL_TEXTURE_2D, Textures[i].id);
    }
    glActiveTexture(GL_TEXTURE0);
}
