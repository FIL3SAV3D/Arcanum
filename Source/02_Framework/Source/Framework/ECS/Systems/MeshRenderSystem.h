#pragma once

#include <memory>
#include "Framework/ECS/Interfaces/ISystem.h"

#include "ArcEngine/Graphics/OpenGL-4.6/Camera.h"
#include <ArcEngine/Platform/InputHandler.h>
#include <ArcEngine/Renderer/MeshShaderRenderer.h>

class MeshRenderSystem : public ISystem
{
public:
	MeshRenderSystem(std::shared_ptr<InputHandler> _InputHandler, std::shared_ptr<ArcEngine::Window> _Window);

	void OnCreate() override;
	void OnUpdate(const float& _DeltaTime) override;
	void OnRender() override;

	void OnResize(const glm::uvec2& _Size) override;

private:
	std::shared_ptr<MeshShaderRenderer> m_Renderer;
	std::shared_ptr<Camera> cam;

	std::shared_ptr<ArcEngine::Window> window;
};