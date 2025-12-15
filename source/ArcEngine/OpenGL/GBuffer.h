#pragma once

class GBuffer
{
public :
	enum GBUFFER_TEXTURE_TYPE
	{
		GBUFFER_TEXTURE_TYPE_POSITION,
		GBUFFER_TEXTURE_TYPE_DIFFUSE,
		GBUFFER_TEXTURE_TYPE_NORMAL,
		GBUFFER_NUM_TEXTURES
	};

	GBuffer();

	~GBuffer();

	bool Init(const unsigned int& screen_width, const unsigned int& screen_height);

	void BindForWriting();

	void BindForReading();

	void SetReadBuffer(GBUFFER_TEXTURE_TYPE TextureType);

private:

	unsigned int m_fbo;
	unsigned int m_textures[GBUFFER_NUM_TEXTURES];
	unsigned int m_depthTexture;
};