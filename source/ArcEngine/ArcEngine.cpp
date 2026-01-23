#include "ArcEngine.h"

#include "OpenGL/Window.h"

#include "ECS/Coordinator.h"
#include "ECS/Systems/MeshRenderSystem.h"
//#include "ECS/Systems/UIRenderSystem.h"

#include "ECS/Components/TransformComponent.h"
//#include "ECS/Components/ModelComponent.h"
//#include "ECS/Components/PhysicsComponent.h"
//#include "ECS/Components/PointLightComponent.h"
//#include "ECS/Components/CameraComponent.h"

#include <OpenGL/InputHandler.h>
#include <imgui.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>

ArcEngine::ArcEngine()
{
    // Created in order of importance
    window = std::make_shared<Window>(screenWidth, screenHeight, windowName.c_str());
    inputHandler = std::make_shared<InputHandler>();

    coordinator = std::make_shared<Coordinator>();

    coordinator->RegisterSystem<MeshRenderSystem, 100>(window, MeshRenderSystem::DEFFERED);
    //coordinator->RegisterSystem<UIRenderSystem, 101>(window);

    //coordinator->mSystemManager->RecalculateUpdateOrder();

    //coordinator->RegisterComponent<ModelComponent>();
    coordinator->RegisterComponent<TransformComponent>();
    //coordinator->RegisterComponent<PhysicsComponent>();
    //coordinator->RegisterComponent<PointLightComponent>();
    //coordinator->RegisterComponent<CameraComponent>();*/
}

ArcEngine::~ArcEngine()	
{
    coordinator.reset();
}

void ArcEngine::Run()
{
    //coordinator->mSystemManager->OnCreate();

    while (!glfwWindowShouldClose(window->GetNativeWindow()))
    {
        inputHandler = std::make_shared<InputHandler>();

        //coordinator->mSystemManager->OnUpdate();

        // Check and call events and swap buffers
        glfwSwapBuffers(window->GetNativeWindow());
        glfwPollEvents();
    }

    //coordinator->mSystemManager->OnDestroy();

    glfwTerminate();
}