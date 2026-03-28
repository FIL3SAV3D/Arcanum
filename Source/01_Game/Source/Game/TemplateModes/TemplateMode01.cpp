#include "TemplateMode01.h"

#include "Framework/ECS/Coordinator.h"
#include "Framework/ECS/TemplateECSSystems/DefaultEntityRenderSystem.h"
#include "Framework/ECS/TemplateECSSystems/PhysicsSystem.h"

#include <random>

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

#include <Framework/Managers/SceneManager.h>

#include <Game/TemplateScenes/TestScene.h>
#include <Framework/ECS/TemplateECSSystems/CameraSystem.h>


void TemplateMode01::Create(std::shared_ptr<Coordinator> _Coordinator, std::shared_ptr <SceneManager> _SceneManager)
{
	_SceneManager->RegisterScene<TestScene>("Test", _Coordinator);
	_SceneManager->SwitchScene("Test", _Coordinator);
}

void TemplateMode01::Destroy(std::shared_ptr<Coordinator> _Coordinator, std::shared_ptr <SceneManager> _SceneManager)
{
}

void TemplateMode01::RegisterSystems(std::shared_ptr<Coordinator> _Coordinator)
{
	_Coordinator->RegisterSystem<CameraSystem, UpdatePriorities::LATE_UPDATE>();
	_Coordinator->RegisterSystem<DefaultEntityRenderSystem, UpdatePriorities::UPDATE + 1>();
}

void TemplateMode01::UnregisterSystems(std::shared_ptr<Coordinator> _Coordinator)
{
	_Coordinator->re
}