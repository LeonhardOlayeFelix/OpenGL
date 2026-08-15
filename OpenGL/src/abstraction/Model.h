#pragma once
#include <vector>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "Mesh.h"
#include "vendor/stb_image/stb_image.h"

class Model
{
public:
	Model() = default;
	Model(const char* path) 
	{
		loadModel(path);
	}
	void Draw(ShaderProgram& shader);
	void DrawInstanced(ShaderProgram& shader, unsigned int count);

private:
	void loadModel(std::string path);
	void processNode(aiNode* node, const aiScene* scene);
	Mesh processMesh(aiMesh* mesh, const aiScene* scene);
	std::vector<MeshTexture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);
	unsigned int TextureFromFile(const char* path, const std::string& directory);

public:
	inline std::vector<Mesh>& GetMeshes() { return m_Meshes; }

private:
	std::vector<Mesh> m_Meshes{};
	std::string m_Directory{};
	std::vector<MeshTexture> m_TexturesLoaded{};
};

