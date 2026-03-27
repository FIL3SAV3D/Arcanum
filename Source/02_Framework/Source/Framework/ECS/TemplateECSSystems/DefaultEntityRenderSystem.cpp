#include "DefaultEntityRenderSystem.h"

#include "Framework/ECS/Managers/ComponentManager.h"
#include "Framework/ECS/Components/TransformComponent.h"
#include "Framework/ECS/Components/ModelComponent.h"

#include "Framework/ECS/Coordinator.h"
#include <ArcEngine/ArcEngine.h>

#include <SDL3/SDL_events.h>

void DefaultEntityRenderSystem::GetSignature(SignatureParameters& _Parameters)
{
    Signature&        signature        = _Parameters.signature;
    ComponentManager& componentManager = _Parameters.componentManager;

    signature.set(componentManager.GetComponentType<TransformComponent>());
    signature.set(componentManager.GetComponentType<ModelComponent    >());
}

void DefaultEntityRenderSystem::OnRender(RenderState& _State)
{
    for (auto const& entity : mEntities)
    {
        TransformComponent& transform = _State.coordinator->GetComponent<TransformComponent>(entity);
        ModelComponent&     model     = _State.coordinator->GetComponent<ModelComponent    >(entity);

        _State.engine->RenderMesh(model.model, transform.transform);
    }
}