#include "EditorMode.h"

#include <Framework/ECS/Coordinator.h>
#include <Framework/Managers/SceneManager.h>
#include <Framework/ECS/TemplateECSSystems/UIRenderSystem.h>

void EditorMode::Create(std::shared_ptr<Coordinator> _Coordinator, std::shared_ptr<SceneManager> _SceneManager)
{
}

void EditorMode::Destroy(std::shared_ptr<Coordinator> _Coordinator, std::shared_ptr<SceneManager> _SceneManager)
{
}

void EditorMode::RegisterSystems(std::shared_ptr<Coordinator> _Coordinator)
{
    _Coordinator->RegisterSystem<UIRenderSystem, UpdatePriorities::UPDATE>();
}

void EditorMode::UnregisterSystems(std::shared_ptr<Coordinator> _Coordinator)
{
}
