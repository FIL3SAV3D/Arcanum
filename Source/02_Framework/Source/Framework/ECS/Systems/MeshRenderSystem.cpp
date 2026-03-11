#include "MeshRenderSystem.h"

#include "Framework/ECS/Coordinator.h"
#include "ArcEngine/Renderer/DeferredRenderer.h"
#include "ArcEngine/Renderer/MeshShaderRenderer.h"

#include "Framework/ECS/Components/RenderComponent.h"
#include "Framework/ECS/Components/TransformComponent.h"


MeshRenderSystem::MeshRenderSystem(std::shared_ptr<InputHandler> _InputHandler, std::shared_ptr<ArcEngine::Window> _Window)
{
	cam = std::make_shared<Camera>();
	_InputHandler->AddListener(cam);

	window = _Window;

	m_Renderer = std::make_shared<MeshShaderRenderer>();
	m_Renderer->Create(window);
}

void MeshRenderSystem::OnCreate()
{
	
}

void MeshRenderSystem::OnUpdate(const float& _DeltaTime)
{
	cam->Update(_DeltaTime);
}

void MeshRenderSystem::OnRender()
{
    RenderParams rParam
    {
        .camera = cam.get(),
        .shader = nullptr
    };

    m_Renderer->BeginRender(rParam);

    for (auto const& entity : mEntities)
    {
        auto& renderComponent = coordinator->GetComponent<RenderComponent>(entity);
        auto& transformComponent = coordinator->GetComponent<TransformComponent>(entity);

        glm::mat4& transform = transformComponent.transform;


        m_Renderer->RenderClusterMesh(rParam, renderComponent.model, 0, transform);
    }


    m_Renderer->EndRender(rParam);
}

void MeshRenderSystem::OnResize(const glm::uvec2& _Size)
{
	m_Renderer->Resize(window);
}
