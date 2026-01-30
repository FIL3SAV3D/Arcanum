#include "MeshRenderSystem.h"

#include "ECS/Coordinator.h"
#include "ECS/Components/RenderComponent.h"
#include "ECS/Components/TransformComponent.h"


MeshRenderSystem::MeshRenderSystem(std::shared_ptr<InputHandler> _InputHandler)
{
	cam = std::make_shared<Camera>();
	_InputHandler->AddListener(cam);
}

void MeshRenderSystem::OnCreate()
{
}

void MeshRenderSystem::OnUpdate(const float& _DeltaTime)
{
	cam->Update(_DeltaTime);
}

void MeshRenderSystem::OnRender(const RenderParams& _RenderParams)
{
	//_Renderer->BeginRender(*cam);

	for (auto const& entity : mEntities)
	{
		auto& renderComponent = coordinator->GetComponent<RenderComponent>(entity);
		auto& transformComponent = coordinator->GetComponent<TransformComponent>(entity);

		//JPH::Mat44& transform = transformComponent.transform;


		//_Renderer->RenderModel(renderComponent.model);
	}

	//_Renderer->EndRender(Window);
}
