#include "GBuffer.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stdio.h>

#define ARRAY_LENGTH(x) (sizeof(x) / sizeof(x[0]))

GBuffer::GBuffer()
{
}

GBuffer::~GBuffer()
{
    Destroy();
}

bool GBuffer::Create(const glm::vec2& _size)
{
    output_FBOs.resize(OUTPUT_FBO_SIZE);
    for (unsigned int i = 0; i < OUTPUT_FBO_SIZE; i++)
    {
        FBO fboObject;
        // Create the FBO
        glGenFramebuffers(1, &fboObject.fbo);
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, fboObject.fbo);

        glGenTextures(OUTPUT_FBO_CUSTOM_TEXTURE_SIZE, fboObject.texture);
        for (unsigned int i = 0; i < OUTPUT_FBO_CUSTOM_TEXTURE_SIZE; i++) {
            glBindTexture(GL_TEXTURE_2D, fboObject.texture[i]);

            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, static_cast<int>(_size.x), static_cast<int>(_size.y), 0, GL_RGB, GL_FLOAT, NULL);

            glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

            glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
            glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, fboObject.texture[i], 0);
        }

        glGenTextures(1, &fboObject.depthTexture);
        glBindTexture(GL_TEXTURE_2D, fboObject.depthTexture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, static_cast<int>(_size.x), static_cast<int>(_size.y), 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, fboObject.depthTexture, 0);

        GLenum DrawBuffers[OUTPUT_FBO_CUSTOM_TEXTURE_SIZE];
        for (unsigned int i = 0; i < OUTPUT_FBO_CUSTOM_TEXTURE_SIZE; i++)
        {
            DrawBuffers[i] = GL_COLOR_ATTACHMENT0 + i;
        }

        glDrawBuffers(OUTPUT_FBO_CUSTOM_TEXTURE_SIZE, DrawBuffers);

        output_FBOs[i] = fboObject;
    }

    // Create the FBO
    glGenFramebuffers(1, &m_gBuffer_fbo);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_gBuffer_fbo);

    // Create the gbuffer textures
    glGenTextures(ARRAY_LENGTH(m_gBuffer_textures), m_gBuffer_textures);
    glGenTextures(1, &(m_gBuffer_depthTexture));

    for (unsigned int i = 0; i < ARRAY_LENGTH(m_gBuffer_textures); i++) {
        glBindTexture(GL_TEXTURE_2D, m_gBuffer_textures[i]);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, static_cast<int>(_size.x), static_cast<int>(_size.y), 0, GL_RGB, GL_FLOAT, NULL);

        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, m_gBuffer_textures[i], 0);
    }

    // depth
    glBindTexture(GL_TEXTURE_2D, m_gBuffer_depthTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, static_cast<int>(_size.x), static_cast<int>(_size.y), 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_gBuffer_depthTexture, 0);

    GLenum DrawBuffers[ARRAY_LENGTH(m_gBuffer_textures)];
    for (unsigned int i = 0; i < ARRAY_LENGTH(m_gBuffer_textures); i++)
    {
        DrawBuffers[i] = GL_COLOR_ATTACHMENT0 + i;
    }

    glDrawBuffers(ARRAY_LENGTH(m_gBuffer_textures), DrawBuffers);

    GLenum Status = glCheckFramebufferStatus(GL_FRAMEBUFFER);

    if (Status != GL_FRAMEBUFFER_COMPLETE) {
        printf("FB error, status: 0x%x\n", Status);
        return false;
    }

    // restore default FBO
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);

    bufferSize = _size;

    output_FBO_Current = 0;

    return true;
}


void GBuffer::Destroy()
{
    if (m_gBuffer_fbo != 0) {
        glDeleteFramebuffers(1, &m_gBuffer_fbo);
    }

    if (m_gBuffer_textures[0] != 0) {
        glDeleteTextures(GBUFFER_NUM_TEXTURES, m_gBuffer_textures);
    }

    if (m_gBuffer_depthTexture != 0) {
        glDeleteTextures(1, &m_gBuffer_depthTexture);
    }

    int size = output_FBOs.size();
    for (unsigned int i = 0; i < size; i++)
    {
        glDeleteFramebuffers(1, &output_FBOs[i].fbo);
        for (unsigned int x = 0; x < OUTPUT_FBO_CUSTOM_TEXTURE_SIZE; x++)
        {
            glDeleteTextures(1, &output_FBOs[i].texture[x]);
        }
        glDeleteTextures(1, &output_FBOs[i].depthTexture);
    }
    output_FBOs.clear();
}

void GBuffer::BindForWriting()
{
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_gBuffer_fbo);
}


void GBuffer::BindForReading()
{
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);

    for (unsigned int i = 0; i < ARRAY_LENGTH(m_gBuffer_textures); i++) {
        glActiveTexture(GL_TEXTURE0 + i);
        glBindTexture(GL_TEXTURE_2D, m_gBuffer_textures[GBUFFER_TEXTURE_TYPE_POSITION + i]);
    }
}

void GBuffer::SetReadBuffer(GBUFFER_TEXTURE_TYPE TextureType)
{
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
    glBindFramebuffer(GL_READ_FRAMEBUFFER, m_gBuffer_fbo);

    glReadBuffer(GL_COLOR_ATTACHMENT0 + TextureType);
}

void GBuffer::SetActiveTexture(GBUFFER_TEXTURE_TYPE TextureType)
{
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_gBuffer_textures[TextureType]);
}

void GBuffer::CycleOutputFBO()
{
    unsigned int readFBO = (output_FBO_Current % OUTPUT_FBO_SIZE);
    unsigned int drawFBO = ((output_FBO_Current + 1) % OUTPUT_FBO_SIZE);

    glBindFramebuffer(GL_READ_FRAMEBUFFER, output_FBOs[readFBO].fbo);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, output_FBOs[drawFBO].fbo);

    for (unsigned int i = 0; i < OUTPUT_FBO_CUSTOM_TEXTURE_SIZE; i++) {
        glActiveTexture(GL_TEXTURE0 + i);
        glBindTexture(GL_TEXTURE_2D, output_FBOs[readFBO].texture[i]);
    }

    output_FBO_Current = drawFBO;
}