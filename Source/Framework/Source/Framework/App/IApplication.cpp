#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "IApplication.h"
#include <format>
#include "Framework/Modes/ModeMain.h"

//#include "Layers/ImGUILayer.h"
#include <backends/imgui_impl_opengl3.h>
#include <backends/imgui_impl_glfw.h>

#include "Framework/ECS/Systems/UIRenderSystem.h"
#include <Framework/ECS/Systems/MeshRenderSystem.h>
#include <Framework/ECS/Components/RenderComponent.h>
#include <Framework/ECS/Components/TransformComponent.h>

#include <random>
#include <ArcEngine/OpenGL/Model.h>

#include "Jolt/Jolt.h"
#include "Jolt/Math/Vec3.h"
#include "Jolt/Math/Quat.h"
#include <Framework/ECS/Systems/PhysicsSystem.h>
#include <Framework/ECS/Components/RigidBodyComponet.h>

#include "ArcEngine/Renderer/DeferredRenderer.h"

#include "glm/gtc/quaternion.hpp"

IApplication::IApplication(const ApplicationSpecification& _Spec) :
	modeManger{ std::make_unique<ModeManager>() },
	window{ std::make_shared<Window>(glm::uvec2(_Spec.windowSize.x, _Spec.windowSize.y), _Spec.name.c_str()) },
	inputHandler{ std::make_shared<InputHandler>() },
	clock{ std::make_unique<Clock>() },
	coordinator{ std::make_shared<Coordinator>() },
	assetManager{ std::make_shared<ArcEngine::AssetManager>() }
{
	glfwSetWindowUserPointer(window->GetNativeWindow(), this);
	glfwSetFramebufferSizeCallback(window->GetNativeWindow(), FrameBufferSizeCallback);

	glfwSetCursorPosCallback(window->GetNativeWindow(), InputHandler::CursorCallBackImpl);
	glfwSetScrollCallback(window->GetNativeWindow(), InputHandler::ScrollCallBackImpl);
	glfwSetKeyCallback(window->GetNativeWindow(), InputHandler::KeyCallBackImpl);


	glfwSetWindowUserPointer(window->GetNativeWindow(), this);

	Initialize();
}

IApplication::~IApplication()
{
}

void IApplication::SetMode()
{
}

void IApplication::Run()
{
	OnInput();

	clock->Update();
	const float deltaTime = clock->GetDeltaTime();

	OnUpdate(deltaTime);

	OnLateUpdate(deltaTime);

	// Set camera bindings

	OnRender();

	//OnRenderUI();

	OnApplicationPause();

	OnCheckForDisabled();
}

#pragma region  IApplicationStart

// Start
void IApplication::OnCreate()
{
	coordinator->RegisterComponent<RenderComponent>();
	coordinator->RegisterComponent<TransformComponent>();
	coordinator->RegisterComponent<RigidBodyComponent>();

	//UI = std::static_pointer_cast<UIRenderSystem>(coordinator->RegisterSystem<UIRenderSystem, 10>(window));

	coordinator->RegisterSystem<MeshRenderSystem, 10>(inputHandler, window);
	//coordinator->RegisterSystem<ECSPhysicsSystem, 20>();

	Signature signature;
	//signature.set(coordinator->GetComponentType<TransformComponent>());
	//signature.set(coordinator->GetComponentType<RigidBodyComponent>());
	//coordinator->SetSystemSignature<ECSPhysicsSystem>(signature);

	signature.reset();

	signature.set(coordinator->GetComponentType<TransformComponent>());
	signature.set(coordinator->GetComponentType<RenderComponent>());
	coordinator->SetSystemSignature<MeshRenderSystem>(signature);

	coordinator->m_SystemManager->RecalculateUpdateOrder();

	coordinator->OnCreate();

	auto m_cubeModel = assetManager->LoadAsset(std::string("D:\\PersonalProjects\\Arcanum\\Data\\Models\\GLB_Models\\Rat.glb").c_str());

	std::vector<Entity> entities(100);

	std::default_random_engine generator;
	std::uniform_real_distribution<float> randPosition(-100.0f, 100.0f);
	std::uniform_real_distribution<float> randRotation(0.0f, 3.0f);
	std::uniform_real_distribution<float> randScale(3.0f, 5.0f);
	std::uniform_real_distribution<float> randGravity(-10.0f, -1.0f);

	float scale = randScale(generator);

	for (auto& entity : entities)
	{
		entity = coordinator->CreateEntity();

		glm::mat4 transform = glm::mat4{ 1.0f };
		transform = glm::translate(transform, glm::vec3(randPosition(generator), randPosition(generator), randPosition(generator)));
		auto rotQuat = glm::quat(glm::vec3(randRotation(generator), randRotation(generator), randRotation(generator)));
		transform = transform * glm::mat4_cast(rotQuat);
		transform = glm::scale(transform, glm::vec3(scale));

		coordinator->AddComponent(
			entity,
			TransformComponent{
				.transform = transform
			});

		coordinator->AddComponent(
			entity,
			RigidBodyComponent{}
		);

		RenderComponent renderComponent{};

		renderComponent.model = *std::static_pointer_cast<Model>(m_cubeModel);

		coordinator->AddComponent(
			entity,
			renderComponent);
	}

	auto cameraEntity = coordinator->CreateEntity();
	coordinator->AddComponent(cameraEntity, TransformComponent{});
	
	glfwSetInputMode(window->GetNativeWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	coordinator->OnStart();
}

void IApplication::OnEnable()
{
}

void IApplication::OnStart()
{
}
// Start

#pragma endregion

#pragma region IApplicationRun

// Update
void IApplication::OnInput()
{
	inputHandler->ProcessInput(window->GetNativeWindow());

	coordinator->OnInput(window);
}

void IApplication::OnUpdate(const float& _DeltaTime)
{
	coordinator->OnUpdate(_DeltaTime);
}

void IApplication::OnLateUpdate(const float& _DeltaTime)
{
	coordinator->OnLateUpdate(_DeltaTime);
}

void IApplication::OnRender()
{
	coordinator->OnRender();
}

void IApplication::OnRenderUI()
{
	//coordinator->OnRenderUI();
	//ImGui::Render();
	//int display_w, display_h;
	//glfwGetFramebufferSize(window->GetNativeWindow(), &display_w, &display_h);
	//glViewport(0, 0, display_w, display_h);
	////glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
	//glClear(GL_COLOR_BUFFER_BIT);
	//ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	//// Imgui end render

	//if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	//{
	//	GLFWwindow* backup_current_context = glfwGetCurrentContext();
	//	ImGui::UpdatePlatformWindows();
	//	ImGui::RenderPlatformWindowsDefault();
	//	glfwMakeContextCurrent(backup_current_context);
	//}

}

void IApplication::OnApplicationPause()
{
}

void IApplication::OnCheckForDisabled()
{
}
// Update

#pragma endregion

#pragma region IApplicationEnd

// Quit
void IApplication::OnQuit()
{
}

void IApplication::OnDisable()
{
}

void IApplication::OnDestroy()
{

}
// Quit

#pragma endregion

bool IApplication::IsQuitting()
{
	return glfwWindowShouldClose(window->GetNativeWindow());
}

void IApplication::FrameBufferSizeCallback(GLFWwindow* _window, int _width, int _height)
{
	IApplication* ptr = static_cast<IApplication*>(glfwGetWindowUserPointer(_window));
	glm::uvec2 newSize{_width, _height};

	ptr->window->SetScreenSize(newSize);
	ptr->coordinator->OnResize(newSize);
}