#include "TestScene.h"
#include <Framework/ECS/Data/ECSTypeInfo.h>
#include <vector>
#include <random>

void TestScene::Create()
{
	//coordinator->RegisterComponent<TransformComponent>();
	//coordinator->RegisterComponent<RenderComponent>();

	//////UI = std::static_pointer_cast<UIRenderSystem>(coordinator->RegisterSystem<UIRenderSystem, 10>(window));

	//////coordinator->RegisterSystem<ECSPhysicsSystem, 20>();

	///*signature.set(coordinator->GetComponentType<TransformComponent>());
	//signature.set(coordinator->GetComponentType<RigidBodyComponent>());
	//coordinator->SetSystemSignature<ECSPhysicsSystem>(signature);*/

	////signature.reset();

	//coordinator->RegisterSystem<MeshRenderSystem, 10>(inputHandler, window);

	//Signature signature;
	//signature.set(coordinator->GetComponentType<TransformComponent>());
	//signature.set(coordinator->GetComponentType<RenderComponent>());
	//coordinator->SetSystemSignature<MeshRenderSystem>(signature);

	//coordinator->m_SystemManager->RecalculateUpdateOrder();

	//coordinator->OnCreate();

	//auto cubeModel = assetManager.LoadAsset("Cube.fbx");


	std::vector<Entity> entities(100);

	std::default_random_engine generator;
	std::uniform_real_distribution<float> randPosition(-1.0f, 1.0f);
	std::uniform_real_distribution<float> randRotation(0.0f, 3.0f);
	std::uniform_real_distribution<float> randScale(1.0f, 1.0f);
	std::uniform_real_distribution<float> randGravity(-10.0f, -1.0f);

	float scale = randScale(generator);

	for (auto& entity : entities)
	{
		//entity = coordinator->CreateEntity();

		//glm::mat4 transform = glm::mat4{ 1.0f };
		//transform = glm::translate(transform, glm::vec3(randPosition(generator), randPosition(generator), randPosition(generator)));
		//auto rotQuat = glm::quat(glm::vec3(randRotation(generator), randRotation(generator), randRotation(generator)));
		//transform = transform * glm::mat4_cast(rotQuat);
		//transform = glm::scale(transform, glm::vec3(scale));

		//coordinator->AddComponent(
		//	entity,
		//	TransformComponent{
		//		.transform = transform
		//	});

		//RenderComponent renderComponent{};

		//renderComponent.model = *std::static_pointer_cast<ClusterModel>(cubeModel);

		//coordinator->AddComponent(
		//	entity,
		//	renderComponent);
	}

	//auto cameraEntity = coordinator->CreateEntity();
	//coordinator->AddComponent(cameraEntity, TransformComponent{});
}

void TestScene::Destroy()
{
}