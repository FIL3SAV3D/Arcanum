#pragma once

#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

#include <glm/vec2.hpp>

class FrameBuffer
{
public:
	unsigned int ID{ 0 };

public:
	FrameBuffer(const glm::vec2& _size);
	~FrameBuffer();

	void Bind();
	void Unbind();

	void Resize(const glm::vec2& _size);

	glm::vec2 size{};

	unsigned int rbo;
	unsigned int textureColorbuffer;

private:
	void CreateFrameBuffer(const glm::vec2& _size);
	void DeleteFrameBuffer();
};

#endif // !SHADER_H