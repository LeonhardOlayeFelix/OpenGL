#pragma once
#include <string>
#include "textures/Texture.h"

/*
struct Material
{
	Texture DiffuseMap;
	Texture SpecularMap;
	Texture NormalMap;
	Texture EmissiveMap;
	float Shininess;
	bool Blinn;
}
*/



class MapBindingConfig {
public:
	int DiffuseSlot{-1};
	int SpecularSlot{-1};
	int NormalSlot{-1};
	int EmissiveSlot{-1};
public:
	MapBindingConfig(int diffuseSlot = -1, int specularSlot = -1, int normalSlot = -1, int emissiveSlot = -1) : DiffuseSlot(diffuseSlot), SpecularSlot(specularSlot), NormalSlot(normalSlot) {};
};

class Material
{
public:
	Texture DiffuseMap{};
	Texture SpecularMap{};
	Texture NormalMap{};
	Texture EmissiveMap{};
	float Shininess{ 256 };
	bool Blinn{ true };

private:
	MapBindingConfig m_BindingConfig;

private:
	void BindMapToSlot(const Texture& Map, int slot) const;

public:
	Material() = default;
	Material(Texture diffuseMap, Texture specularMap, Texture normalMap) : DiffuseMap(std::move(diffuseMap)), SpecularMap(std::move(specularMap)), NormalMap(std::move(normalMap)) {};
	Material(const std::string& diffuseMap, const std::string& specularMap, const std::string& normalMap) : Material(Texture(diffuseMap), Texture(specularMap), Texture(normalMap)) {};
	void SetBindingConfig(MapBindingConfig config);
	void BindMaps() const;
};

