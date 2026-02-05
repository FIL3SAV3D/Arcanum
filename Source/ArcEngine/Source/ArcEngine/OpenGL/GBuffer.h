#pragma once

#include <vector>

#include "glm/vec2.hpp"

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

public:
    GBuffer();
    ~GBuffer();

    bool Create(const glm::uvec2& _Size);
    void Destroy();

    void BindForWriting();
    void BindForReading();

    void SetReadBuffer(GBUFFER_TEXTURE_TYPE TextureType);
    void SetActiveTexture(GBUFFER_TEXTURE_TYPE TextureType);

    const glm::uvec2& GetBufferSize() const { return bufferSize; }
    const unsigned int& GetFBO() const { return m_gBuffer_fbo; }
private:
    glm::uvec2 bufferSize;

    unsigned int m_gBuffer_fbo;
    unsigned int m_gBuffer_textures[GBUFFER_NUM_TEXTURES] = {};
    unsigned int m_gBuffer_depthTexture;

};