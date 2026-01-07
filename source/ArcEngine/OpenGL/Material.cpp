#include "Material.h"

#include "Texture.h"
#include "Shader.h"

Material::Material()
{
	shader = nullptr;
}

Material::~Material()
{
}

void Material::AppendTexture(const TextureType* _type)
{
#if ARC_DEBUG
	if (_type == nullptr)
	{

	}
#endif
}

void Material::BindShader(const Shader* _shader)
{
#if ARC_DEBUG
	if (_shader == nullptr)
	{

	}
#endif
}
