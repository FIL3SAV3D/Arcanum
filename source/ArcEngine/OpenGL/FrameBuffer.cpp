#include "FrameBuffer.h"

#include <glad/glad.h>

#include <iostream>

FrameBuffer::FrameBuffer(const glm::vec2& _size)
{
	CreateFrameBuffer(_size);
}

FrameBuffer::~FrameBuffer()
{
	DeleteFrameBuffer();
}

void FrameBuffer::Bind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, ID);
}

void FrameBuffer::Unbind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void FrameBuffer::Resize(const glm::vec2& _size)
{
	DeleteFrameBuffer();
	CreateFrameBuffer(_size);
}

void FrameBuffer::CreateFrameBuffer(const glm::vec2& _size)
{
	size = _size;

	// Create framebuffer
	glGenFramebuffers(1, &ID);
	glBindFramebuffer(GL_FRAMEBUFFER, ID);

	//// generate texture
	//glGenTextures(1, &textureColorbuffer);
	//glBindTexture(GL_TEXTURE_2D, textureColorbuffer);
	//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, _size.x, _size.y, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//glBindTexture(GL_TEXTURE_2D, 0);

	//// attach it to currently bound framebuffer object
	//glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureColorbuffer, 0);

	// MSAA FrameTexture
	glGenTextures(1, &textureColorbuffer);
	glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, textureColorbuffer);
	glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, 4, GL_RGB, _size.x, _size.y, GL_TRUE);
	glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE, textureColorbuffer, 0);

	// Genereate render buffer
	glGenRenderbuffers(1, &rbo);
	glBindRenderbuffer(GL_RENDERBUFFER, rbo);
	/*glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, _size.x, _size.y);*/
	glRenderbufferStorageMultisample(GL_RENDERBUFFER, 4, GL_DEPTH24_STENCIL8, _size.x, _size.y);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);

	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);
	
	

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void FrameBuffer::DeleteFrameBuffer()
{
	glDeleteFramebuffers(1, &ID);
	glDeleteRenderbuffers(1, &rbo);
	glDeleteTextures(1, &textureColorbuffer);
}
