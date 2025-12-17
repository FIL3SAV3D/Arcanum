#pragma once

#include <glm/ext/vector_float2.hpp>
#include <vector>

constexpr int OUTPUT_FBO_SIZE = 2;


class GBuffer
{
public:

    enum GBUFFER_TEXTURE_TYPE {
        GBUFFER_TEXTURE_TYPE_POSITION,
        GBUFFER_TEXTURE_TYPE_NORMAL,
        GBUFFER_TEXTURE_TYPE_ALBEDO,
        GBUFFER_TEXTURE_TYPE_ORM,
        GBUFFER_TEXTURE_TYPE_DEPTH,
        GBUFFER_TEXTURE_TYPE_COMBINED,
        GBUFFER_NUM_TEXTURES
    };

    GBuffer();
    ~GBuffer();

    bool Create(const glm::vec2& _size);
    void Destroy();

    void BindForWriting();
    void BindForReading();

    void SetReadBuffer(GBUFFER_TEXTURE_TYPE TextureType);
    void SetActiveTexture(GBUFFER_TEXTURE_TYPE TextureType);

    glm::vec2 bufferSize;

    unsigned int m_gBuffer_fbo;
    unsigned int m_gBuffer_textures[GBUFFER_NUM_TEXTURES] = {};
    unsigned int m_gBuffer_depthTexture;
private:

};