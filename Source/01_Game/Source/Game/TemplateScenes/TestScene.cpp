#include "TestScene.h"

#include "Framework/ECS/Coordinator.h"
#include <Framework/ECS/Components.h>

#include <vector>
#include <random>
#include <glm/glm.hpp>

#include <glm/gtc/quaternion.hpp>

void TestScene::Create(std::shared_ptr<Coordinator> _Coordinator)
{
	//auto cubeModel = assetManager.LoadAsset("Cube.fbx");

	std::vector<Entity> entities(1000);

	std::default_random_engine generator;
	std::uniform_real_distribution<float> randPosition(-50.0f, 50.0f);
	std::uniform_real_distribution<float> randRotation(0.0f, 3.0f);
	std::uniform_real_distribution<float> randScale(0.5f, 1.0f);

	float scale = randScale(generator);

	for (auto& entity : entities)
	{
		entity = _Coordinator->CreateEntity();

		glm::mat4 transform = glm::mat4{ 1.0f };
		transform = glm::translate(transform, glm::vec3(randPosition(generator), randPosition(generator), randPosition(generator)));
		auto rotQuat = glm::quat(glm::vec3(randRotation(generator), randRotation(generator), randRotation(generator)));
		transform = transform * glm::mat4_cast(rotQuat);
		transform = glm::scale(transform, glm::vec3(scale));

		_Coordinator->AddComponent(
			entity,
			TransformComponent{
				.transform = transform
			});

		_Coordinator->AddComponent(
			entity,
			ModelComponent{
			});

		//_Coordinator.AddComponent(
		//	entity,
		//	RigidBodyComponent{});
	}

	Entity cameraEntity = _Coordinator->CreateEntity();
	_Coordinator->AddComponent(
		cameraEntity,
		CameraComponent{}
	);
	_Coordinator->AddComponent(
		cameraEntity,
		InputComponent{}
	);
	_Coordinator->AddComponent(
		cameraEntity,
		TransformComponent{}
	);
}

void TestScene::Destroy(std::shared_ptr<Coordinator> _coordinator)
{
}