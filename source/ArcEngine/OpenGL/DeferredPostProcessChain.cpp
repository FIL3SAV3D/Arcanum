#include "DeferredPostProcessChain.h"

#include <glad/glad.h>

#include "GBuffer.h"

#include "IDeferredPostProcess.h"
#include "PostProcessEffectGaussianBlur.h"
#include "PostProcessEffectBloom.h"

#include <functional>


DeferredPostProcessChain::DeferredPostProcessChain()
{
    m_currentFrameBufferObject = 0;
    quadVAO = 0;
    quadVBO = 0;
}

DeferredPostProcessChain::~DeferredPostProcessChain()
{
    Destroy();
}

bool DeferredPostProcessChain::Create(std::shared_ptr<GBuffer> _GBuffer)
{
    m_gBuffer = _GBuffer;

    for (unsigned int i = 0; i < LIVE_FRAME_BUFFERS; i++)
    {
        std::shared_ptr<FBO> fboObject = std::make_shared<FBO>();

        // Create the FBO
        glGenFramebuffers(1, &fboObject->fbo);
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, fboObject->fbo);

        glGenTextures(OUTPUT_FBO_CUSTOM_TEXTURE_SIZE, fboObject->textures);
        for (unsigned int i = 0; i < OUTPUT_FBO_CUSTOM_TEXTURE_SIZE; i++) {
            glBindTexture(GL_TEXTURE_2D, fboObject->textures[i]);

            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, static_cast<int>(_GBuffer->bufferSize.x), static_cast<int>(_GBuffer->bufferSize.y), 0, GL_RGB, GL_FLOAT, NULL);

            glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

            glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
            glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, fboObject->textures[i], 0);
        }

        glGenTextures(1, &fboObject->depthTexture);
        glBindTexture(GL_TEXTURE_2D, fboObject->depthTexture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, static_cast<int>(_GBuffer->bufferSize.x), static_cast<int>(_GBuffer->bufferSize.y), 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, fboObject->depthTexture, 0);

        GLenum DrawBuffers[OUTPUT_FBO_CUSTOM_TEXTURE_SIZE];
        for (unsigned int i = 0; i < OUTPUT_FBO_CUSTOM_TEXTURE_SIZE; i++)
        {
            DrawBuffers[i] = GL_COLOR_ATTACHMENT0 + i;
        }

        glDrawBuffers(OUTPUT_FBO_CUSTOM_TEXTURE_SIZE, DrawBuffers);

        m_frameBufferObjects[i] = fboObject;
    }

    float quadVertices[] = { // vertex attributes for a quad that fills the entire screen in Normalized Device Coordinates.
        // positions   // texCoords
        -1.0f,  1.0f,  0.0f, 1.0f,
        -1.0f, -1.0f,  0.0f, 0.0f,
         1.0f, -1.0f,  1.0f, 0.0f,

        -1.0f,  1.0f,  0.0f, 1.0f,
         1.0f, -1.0f,  1.0f, 0.0f,
         1.0f,  1.0f,  1.0f, 1.0f
    };

    glGenVertexArrays(1, &quadVAO);
    glGenBuffers(1, &quadVBO);
    glBindVertexArray(quadVAO);
    glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));

    std::shared_ptr<PostProcessEffectGaussianBlur> blur = std::make_shared<PostProcessEffectGaussianBlur>();
    std::shared_ptr<PostProcessEffectBloom> bloom = std::make_shared<PostProcessEffectBloom>();

    m_postProcessChain.push_back(bloom);

    return true;
}

void DeferredPostProcessChain::Destroy()
{
    m_gBuffer = nullptr;

    for (unsigned int i = 0; i < LIVE_FRAME_BUFFERS; i++)
    {
        glDeleteFramebuffers(1, &m_frameBufferObjects[i]->fbo);
        glDeleteTextures(OUTPUT_FBO_CUSTOM_TEXTURE_SIZE, m_frameBufferObjects[i]->textures);
        glDeleteTextures(1, &m_frameBufferObjects[i]->depthTexture);
    }
}

void DeferredPostProcessChain::ApplyPostProcessChain()
{
    for (unsigned int i = 0; i < m_postProcessChain.size(); i++)
    {
        m_postProcessChain[i]->ApplyPostProcess(quadVAO, [this]() { this->PingPongFrameBufferObjects(); });
    }
}

void DeferredPostProcessChain::FinalBlitToScreen()
{
    glBindFramebuffer(GL_READ_FRAMEBUFFER, m_frameBufferObjects[m_currentFrameBufferObject]->fbo);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_frameBufferObjects[m_currentFrameBufferObject]->textures[0]);

    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
    glBlitFramebuffer(
        0, 0, static_cast<int>(m_gBuffer->bufferSize.x), static_cast<int>(m_gBuffer->bufferSize.y),
        0, 0, static_cast<int>(m_gBuffer->bufferSize.x), static_cast<int>(m_gBuffer->bufferSize.y), GL_COLOR_BUFFER_BIT, GL_NEAREST);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void DeferredPostProcessChain::PingPongFrameBufferObjects()
{
    unsigned int readFBO = (m_currentFrameBufferObject % LIVE_FRAME_BUFFERS);
    unsigned int drawFBO = ((m_currentFrameBufferObject + 1) % LIVE_FRAME_BUFFERS);

    m_currentFrameBufferObject = drawFBO;

    glBindFramebuffer(GL_READ_FRAMEBUFFER, m_frameBufferObjects[readFBO]->fbo);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_frameBufferObjects[drawFBO]->fbo);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_gBuffer->m_gBuffer_textures[GBuffer::GBUFFER_TEXTURE_TYPE_COMBINED]);

    for (unsigned int i = 0; i < OUTPUT_FBO_CUSTOM_TEXTURE_SIZE; i++) {
        glActiveTexture(GL_TEXTURE1 + i);
        glBindTexture(GL_TEXTURE_2D, m_frameBufferObjects[readFBO]->textures[i]);
    }
}
