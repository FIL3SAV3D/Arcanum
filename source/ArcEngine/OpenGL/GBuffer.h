#pragma once

#include <glm/ext/vector_float2.hpp>
#include <vector>

constexpr int OUTPUT_FBO_SIZE = 2;
constexpr int OUTPUT_FBO_CUSTOM_TEXTURE_SIZE = 2;

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

    struct FBO
    {
        unsigned int fbo          = 0;
        unsigned int texture[OUTPUT_FBO_CUSTOM_TEXTURE_SIZE];
        unsigned int depthTexture = 0;
    };

    GBuffer();
    ~GBuffer();

    bool Create(const glm::vec2& _size);
    void Destroy();

    void BindForWriting();
    void BindForReading();

    void SetReadBuffer(GBUFFER_TEXTURE_TYPE TextureType);
    void SetActiveTexture(GBUFFER_TEXTURE_TYPE TextureType);

    void CycleOutputFBO();
    void GetActiveOutputFBO();

    glm::vec2 bufferSize;

    std::vector<FBO> output_FBOs;
    unsigned int output_FBO_Current;
    unsigned int internal_FBO_Current;

    unsigned int m_gBuffer_fbo;
    unsigned int m_gBuffer_textures[GBUFFER_NUM_TEXTURES] = {};
    unsigned int m_gBuffer_depthTexture;
private:

};