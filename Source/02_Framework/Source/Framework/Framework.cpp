#include "Framework.h"
#include <SDL3/SDL.h>
#include <backends/imgui_impl_sdl3.h>

#include <ArcEngine/ArcEngine.h>
#include <ArcEngine/Util/Clock.h>

#include "Framework/Managers/AssetManager.h"
#include "Framework/Managers/SceneManager.h"
#include "Framework/ECS/Coordinator.h"

void Framework::Create()
{
    arcEngine       = std::make_shared<Engine>();
    assetManager    = std::make_shared<AssetManager>();
    coordinator = std::make_shared<Coordinator>();
    modeManger      = std::make_shared<ModeManager>();
    sceneManager    = std::make_shared<SceneManager>();
    clock           = std::make_shared<Clock>();

    arcEngine->Create();
    assetManager->Create();
    coordinator->Create();

    startState = StartState{
        .coordinator = coordinator,
    };
}

void Framework::Destroy()
{
    coordinator->Destroy();
    coordinator.reset();

    assetManager->Destroy();
    assetManager.reset();

    arcEngine->Destroy();
    arcEngine.reset();
}

void Framework::Start()
{
    coordinator->OnCreate(startState);
    coordinator->OnStart(startState);

    clock->Update();

    inputState = InputState{
        .coordinator = coordinator,
        .event = {}
    };

    gameState = GameState{
        .coordinator = coordinator,
        .engine = arcEngine
    };

    renderState = RenderState{
        .coordinator = coordinator,
        .engine = arcEngine
    };
}

void Framework::Run()
{
    HandleInput();

    HandleUpdate();

    HandleRender();
}

void Framework::HandleInput()
{
    SDL_Event event;
    while (SDL_PollEvent(&event) != 0) {
        if (event.type == SDL_EVENT_QUIT)
            bQuit = true;

        if (event.type == SDL_EVENT_WINDOW_RESIZED)
        {
            int x{}, y{};
            SDL_GetWindowSize(SDL_GL_GetCurrentWindow(), &x, &y);
            arcEngine->ResizeSwapchain(glm::uvec2(x, y));
        }

        const bool* key_states = SDL_GetKeyboardState(nullptr);

        if (key_states[SDL_SCANCODE_ESCAPE])
        {
            bQuit = true;
        }

        if (event.window.type == SDL_EVENT_WINDOW_MINIMIZED) {
            stop_rendering = true;
        }
        if (event.window.type == SDL_EVENT_WINDOW_RESTORED) {
            stop_rendering = false;
        }

        inputState.event = &event;

        coordinator->OnInput(inputState);

        ImGui_ImplSDL3_ProcessEvent(&event);
    }
}

void Framework::HandleUpdate()
{
    clock->Update();
    const float deltaTime = static_cast<float>(clock->GetDeltaTime());

    { // Update
        coordinator->OnUpdate(gameState, deltaTime);
    } // Update

    { // LateUpdate
        coordinator->OnLateUpdate(gameState, deltaTime);
    } // LateUpdate
}

void Framework::HandleRender()
{
    arcEngine->RenderStart();

    coordinator->OnRenderStart(renderState);

    coordinator->OnRender(renderState);

    { // UI Rendering
        arcEngine->RenderUIStart();

        //coordinator.OnRenderUI(renderState);

        arcEngine->RenderUIEnd();
    } // UI Rendering

    coordinator->OnRenderEnd(renderState);


    arcEngine->RenderEnd();
}

bool Framework::IsQuitting()
{
    return bQuit;
}