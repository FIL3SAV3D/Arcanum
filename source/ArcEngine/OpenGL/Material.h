#pragma once

#include <vector>


enum TextureType : char
{
	TT_ALBEDO,
	TT_NORMAL,
	TT_ORM,
};

class Texture;
class Shader;

class Material
{
public:
	Material ();
	~Material();

	void AppendTexture(const TextureType* _type);
	void BindShader(const Shader* _shader);

private:
	Shader* shader;
	std::vector<Texture> textures;
};