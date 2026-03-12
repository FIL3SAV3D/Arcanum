#include "DefaultEntityRenderSystem.h"

#include "Framework/ECS/Managers/ComponentManager.h"
#include "Framework/ECS/Components/TransformComponent.h"
#include "Framework/ECS/Components/ModelComponent.h"

#include "Framework/ECS/Coordinator.h"

#include <ArcEngine/ArcEngine.h>

void DefaultEntityRenderSystem::GetSignature(SignatureParameters& _Parameters)
{
    Signature&              signature             = _Parameters.signature;
    ComponentManager& componentManager = _Parameters.componentManager;

    signature.set(componentManager.GetComponentType<TransformComponent>());
    signature.set(componentManager.GetComponentType<ModelComponent>());
}

void DefaultEntityRenderSystem::OnRender(State& _State)
{
    Coordinator&          coordinator = _State.coordinator;
    ArcEngine::ArcEngine& engine      = _State.engine;

    for (auto const& entity : mEntities)
    {
        TransformComponent& transform = coordinator.GetComponent<TransformComponent>(entity);
        ModelComponent&     model     = coordinator.GetComponent<ModelComponent    >(entity);

        engine.RenderMesh(model.model, transform.transform);
    }
}