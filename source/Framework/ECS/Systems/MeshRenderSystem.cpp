#include "MeshRenderSystem.h"

#include "ECS/Coordinator.h"
#include "ECS/Components/RenderComponent.h"
#include "ECS/Components/TransformComponent.h"

MeshRenderSystem::MeshRenderSystem(std::shared_ptr<IRenderer> _Renderer):
	renderer{_Renderer}
{
}

void MeshRenderSystem::OnRender(std::shared_ptr<IRenderer> _Renderer)
{
	for (auto const& entity : mEntities)
	{
		auto& renderComponent = coordinator->GetComponent<RenderComponent>(entity);
		auto& transformComponent = coordinator->GetComponent<TransformComponent>(entity);

		renderComponent.model->Draw(renderComponent.shader.get());
	}
}
