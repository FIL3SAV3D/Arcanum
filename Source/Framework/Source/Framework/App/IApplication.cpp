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
#include <ArcEngine/Graphics/OpenGL-4.6/Model.h>

#include "Jolt/Jolt.h"
#include "Jolt/Math/Vec3.h"
#include "Jolt/Math/Quat.h"
#include <Framework/ECS/Systems/PhysicsSystem.h>
#include <Framework/ECS/Components/RigidBodyComponet.h>

#include "ArcEngine/Renderer/DeferredRenderer.h"

#include "glm/gtc/quaternion.hpp"

#include "SDL3/SDL.h"
#include <backends/imgui_impl_vulkan.h>
#include <backends/imgui_impl_sdl3.h>

#include <fmt/core.h>

#include <iostream>
#include <fstream>

#include "ArcEngine/Util/Filepath.h"

#include <filesystem>

#include "ArcEngine/Asset/Serializer.h"

IApplication::IApplication(const ApplicationSpecification& _Spec)
{
	ApplicationSpecification specs;

	std::filesystem::path path{ Filepath::FindDataFolder() };
	//if (!Serializer::Deserialize(path, "\\ApplicationConfig.Scroll", specs))
	//{
	specs = _Spec;
	//}

	ArcEngine::WindowSpecification windowSpecification;
	windowSpecification.graphicsAPI = ArcEngine::EngineGraphicsAPI::OPENGL;
	windowSpecification.windowName = specs.name;
	windowSpecification.windowSize = glm::uvec2(specs.windowSizeX, specs.windowSizeY);

	window = std::make_shared<ArcEngine::Window>();
	window->Create(windowSpecification);

	graphics = std::make_shared<ArcEngine::Graphics>();
	graphics->Create(ArcEngine::Graphics::OPENGL, window);

	assetManager.Create(graphics, true);

	coordinator = std::make_shared<Coordinator>();

	inputHandler = std::make_shared<InputHandler>();

	clock = std::make_unique<Clock>();
}

IApplication::~IApplication()
{
	
}

void IApplication::SetMode()
{
}

void IApplication::Run()
{
	inputHandler->ProcessInput();
	//SDL_Event e;

	//while (SDL_PollEvent(&e) != 0) {
	//	// close the window when user alt-f4s or clicks the X button
	//	if (e.type == SDL_EVENT_QUIT)
	//		bQuit = true;

	//	if (e.type == SDL_EVENT_WINDOW_RESIZED)
	//	{
	//		int x{}, y{};
	//		SDL_GetWindowSize(window->GetNativeWindow(), &x, &y);
	//		graphics->Resize(glm::uvec2(x, y));
	//	}

	//	const bool* key_states = SDL_GetKeyboardState(nullptr);

	//	if (key_states[SDL_SCANCODE_ESCAPE])
	//	{
	//		bQuit = true;
	//	}

	//	if (e.window.type == SDL_EVENT_WINDOW_MINIMIZED) {
	//		stop_rendering = true;
	//	}
	//	if (e.window.type == SDL_EVENT_WINDOW_RESTORED) {
	//		stop_rendering = false;
	//	}

	//	//ImGui_ImplSDL3_ProcessEvent(&e);
	//}
	//OnInput();

	clock->Update();
	const float deltaTime = clock->GetDeltaTime();

	OnUpdate(deltaTime);

	//OnLateUpdate(deltaTime);

	//// Set camera bindings

	OnRender();

	////OnRenderUI();

	//OnApplicationPause();

	//OnCheckForDisabled();

	// imgui new frame
	//ImGui_ImplVulkan_NewFrame();
	//ImGui_ImplSDL3_NewFrame();
	//ImGui::NewFrame();

	////some imgui UI to test
	//ImGui::ShowDemoWindow();

	////make imgui calculate internal draw structures
	//ImGui::Render();

	//graphics.RenderMesh();

	SDL_GL_SwapWindow(window->GetNativeWindow());
}

#pragma region  IApplicationStart

// Start
void IApplication::OnCreate()
{
	coordinator->RegisterComponent<TransformComponent>();
	coordinator->RegisterComponent<RenderComponent>();

	////UI = std::static_pointer_cast<UIRenderSystem>(coordinator->RegisterSystem<UIRenderSystem, 10>(window));

	////coordinator->RegisterSystem<ECSPhysicsSystem, 20>();

	/*signature.set(coordinator->GetComponentType<TransformComponent>());
	signature.set(coordinator->GetComponentType<RigidBodyComponent>());
	coordinator->SetSystemSignature<ECSPhysicsSystem>(signature);*/

	//signature.reset();

	coordinator->RegisterSystem<MeshRenderSystem, 10>(inputHandler, window);

	Signature signature;
	signature.set(coordinator->GetComponentType<TransformComponent>());
	signature.set(coordinator->GetComponentType<RenderComponent>());
	coordinator->SetSystemSignature<MeshRenderSystem>(signature);

	coordinator->m_SystemManager->RecalculateUpdateOrder();

	coordinator->OnCreate();

	auto cubeModel = assetManager.LoadAsset("Cube.fbx");


	std::vector<Entity> entities(1);

	std::default_random_engine generator;
	std::uniform_real_distribution<float> randPosition(-1.0f, 1.0f);
	std::uniform_real_distribution<float> randRotation(0.0f, 3.0f);
	std::uniform_real_distribution<float> randScale(1.0f, 1.0f);
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

		RenderComponent renderComponent{};

		renderComponent.model = *std::static_pointer_cast<ClusterModel>(cubeModel);

		coordinator->AddComponent(
			entity,
			renderComponent);
	}

	auto cameraEntity = coordinator->CreateEntity();
	coordinator->AddComponent(cameraEntity, TransformComponent{});

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
	/*inputHandler->ProcessInput(window->GetNativeWindow());

	coordinator->OnInput(window);*/
}

void IApplication::OnUpdate(const float& _DeltaTime)
{
	coordinator->OnUpdate(_DeltaTime);
}

void IApplication::OnLateUpdate(const float& _DeltaTime)
{
	/*coordinator->OnLateUpdate(_DeltaTime);*/
}

void IApplication::OnRender()
{
	coordinator->OnRender();
}

void IApplication::OnRenderUI()
{

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
	auto [data, out] = zpp::bits::data_out();

	graphics->Destroy();

	ApplicationSpecification specs;

	SDL_GetWindowSize(window->GetNativeWindow(), &specs.windowSizeX, &specs.windowSizeY);

	auto result = out(specs);

	if (failure(result)) {
		// `result` is implicitly convertible to `std::errc`.
		// handle the error or return/throw exception.
		fmt::println("Error");
	}
	else
	{
		std::ofstream outfile("AppTest.Scroll", std::ios::out | std::ios::binary);
		outfile.write(reinterpret_cast<char*>(data.data()), static_cast<long>(data.size()));
	}

	window->Destroy();
	window.reset();
}
// Quit

#pragma endregion

bool IApplication::IsQuitting()
{
	return true;
	/*return glfwWindowShouldClose(window->GetNativeWindow());*/
}

void IApplication::FrameBufferSizeCallback(GLFWwindow* _window, int _width, int _height)
{
	/*IApplication* ptr = static_cast<IApplication*>(glfwGetWindowUserPointer(_window));
	glm::uvec2 newSize{_width, _height};

	ptr->window->SetScreenSize(newSize);
	ptr->coordinator->OnResize(newSize);*/
}

auto Serialize(const ApplicationSpecification& person) -> zpp::bits::members<3>
{
	return zpp::bits::members<3>();
}
