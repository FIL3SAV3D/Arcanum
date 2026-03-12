#include "TemplateMode01.h"

#include "Framework/ECS/Coordinator.h"
#include "Framework/ECS/Systems/DefaultEntityRenderSystem.h"
#include "Framework/ECS/Systems/PhysicsSystem.h"

#include <random>

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <Framework/ECS/Components/TransformComponent.h>
#include <Framework/ECS/Components/ModelComponent.h>
#include <Framework/ECS/Components/RigidBodyComponet.h>

void TemplateMode01::Create(Coordinator& _Coordinator)
{
	//auto cubeModel = assetManager.LoadAsset("Cube.fbx");

	std::vector<Entity> entities(1000);

	std::default_random_engine generator;
	std::uniform_real_distribution<float> randPosition(-10.0f, 10.0f);
	std::uniform_real_distribution<float> randRotation(0.0f, 3.0f);
	std::uniform_real_distribution<float> randScale(0.1f, 0.1f);
	std::uniform_real_distribution<float> randGravity(-10.0f, -1.0f);

	float scale = randScale(generator);

	for (auto& entity : entities)
	{
		entity = _Coordinator.CreateEntity();

		glm::mat4 transform = glm::mat4{ 1.0f };
		transform = glm::translate(transform, glm::vec3(randPosition(generator), randPosition(generator), randPosition(generator)));
		auto rotQuat = glm::quat(glm::vec3(randRotation(generator), randRotation(generator), randRotation(generator)));
		transform = transform * glm::mat4_cast(rotQuat);
		transform = glm::scale(transform, glm::vec3(scale));

		_Coordinator.AddComponent(
			entity,
			TransformComponent{
				.transform = transform
			});

		_Coordinator.AddComponent(
			entity,
			ModelComponent{});

		_Coordinator.AddComponent(
			entity,
			RigidBodyComponent{});
	}
}

void TemplateMode01::Destroy(Coordinator& _Coordinator)
{
}

void TemplateMode01::RegisterSystems(Coordinator& _Coordinator)
{
	_Coordinator.RegisterSystem<ECSPhysicsSystem, UpdatePriorities::UPDATE - 1>();
	_Coordinator.RegisterSystem<DefaultEntityRenderSystem, UpdatePriorities::UPDATE + 1>();
}

void TemplateMode01::UnregisterSystems(const Coordinator& _Coordinator)
{
}