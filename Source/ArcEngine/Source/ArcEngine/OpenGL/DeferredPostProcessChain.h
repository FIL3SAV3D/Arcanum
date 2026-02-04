#pragma once

#include <memory>
#include <vector>

#include <glm/ext/vector_float2.hpp>

class GBuffer;
class IDeferredPostProcess;

#define LIVE_FRAME_BUFFERS 2
#define OUTPUT_FBO_CUSTOM_TEXTURE_SIZE 1

class DeferredPostProcessChain
{
private:
	struct FBO
	{
		unsigned int fbo = 0;
		unsigned int textures[OUTPUT_FBO_CUSTOM_TEXTURE_SIZE] = { 0 };
		unsigned int depthTexture = 0;
	};

public:
	DeferredPostProcessChain();
	~DeferredPostProcessChain();

	bool Create(std::shared_ptr<GBuffer> _GBuffer);
	void Destroy();

	void ApplyPostProcessChain();
	void FinalBlitToScreen();

private:
	void PingPongFrameBufferObjects();

private:
	unsigned int m_currentFrameBufferObject;
	std::shared_ptr<FBO> m_frameBufferObjects[LIVE_FRAME_BUFFERS];

	unsigned int quadVAO;
	unsigned int quadVBO;

	std::shared_ptr<GBuffer> m_gBuffer;

	std::vector<std::shared_ptr<IDeferredPostProcess>> m_postProcessChain;
};