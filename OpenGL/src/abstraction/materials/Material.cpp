#include "Material.h"

void Material::BindMapToSlot(const Texture& Map, int slot) const
{
	if (slot != -1) Map.Bind(slot);
}

void Material::SetBindingConfig(MapBindingConfig config)
{
	m_BindingConfig = config;
}

void Material::BindMaps() const
{
	BindMapToSlot(DiffuseMap, m_BindingConfig.DiffuseSlot);
	BindMapToSlot(SpecularMap, m_BindingConfig.SpecularSlot);
	BindMapToSlot(NormalMap, m_BindingConfig.NormalSlot);
	BindMapToSlot(EmissiveMap, m_BindingConfig.EmissiveSlot);
}
