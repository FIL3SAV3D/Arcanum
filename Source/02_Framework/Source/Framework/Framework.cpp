#include "Framework.h"
#include <SDL3/SDL.h>
#include <backends/imgui_impl_sdl3.h>

void Framework::Create()
{
    m_ArcEngine.Create();

    m_AssetManager.Create();

    m_ECSCoordinator.Create();
}

void Framework::Destroy()
{
    m_ECSCoordinator.Destroy();

    m_AssetManager.Destroy();

    m_ArcEngine.Destroy();
}

void Framework::Start()
{
    State state
    {
        .coordinator = m_ECSCoordinator,
        .engine = m_ArcEngine
    };

    m_ECSCoordinator.OnCreate(state);
    m_ECSCoordinator.OnStart(state);
}

void Framework::Run()
{
    SDL_Event e;

    while (SDL_PollEvent(&e) != 0) {
        if (e.type == SDL_EVENT_QUIT)
            bQuit = true;

        if (e.type == SDL_EVENT_WINDOW_RESIZED)
        {
            //int x{}, y{};
            //SDL_GetWindowSize(SDL_GL_GetCurrentWindow(), &x, &y);
            //m_ArcEngine->Resize(glm::uvec2(x, y));
        }

        const bool* key_states = SDL_GetKeyboardState(nullptr);

        if (key_states[SDL_SCANCODE_ESCAPE])
        {
            bQuit = true;
        }

        if (e.window.type == SDL_EVENT_WINDOW_MINIMIZED) {
            stop_rendering = true;
        }
        if (e.window.type == SDL_EVENT_WINDOW_RESTORED) {
            stop_rendering = false;
        }

        ImGui_ImplSDL3_ProcessEvent(&e);
    }

    State state
    {
        .coordinator = m_ECSCoordinator,
        .engine      = m_ArcEngine
    };

    //OnInput();

    clock.Update();
    const float deltaTime = clock.GetDeltaTime();

    OnUpdate(state, deltaTime);

    //OnLateUpdate(deltaTime);

    //// Set camera bindings

    OnRender(state);

    //OnApplicationPause();

    //OnCheckForDisabled();
}

#pragma region  FrameworkStart

// Start
void Framework::OnCreate()
{
}

void Framework::OnEnable()
{
}

void Framework::OnStart()
{
}
// Start

#pragma endregion

#pragma region FrameworkRun

// Update
void Framework::OnInput()
{
    /*inputHandler->ProcessInput(window->GetNativeWindow());

    coordinator->OnInput(window);*/
}

void Framework::OnUpdate(State& _State, const float& _DeltaTime)
{
    m_ECSCoordinator.OnUpdate(_State, _DeltaTime);
}

void Framework::OnLateUpdate(const float& _DeltaTime)
{
    /*coordinator->OnLateUpdate(_DeltaTime);*/
}

void Framework::OnRender(State& _State)
{
    m_ArcEngine.RenderStart();

    m_ECSCoordinator.OnRender(_State);

    { // UI Rendering
        m_ArcEngine.RenderUIStart();

        //m_ECSCoordinator.OnRenderUI();

        m_ArcEngine.RenderUIEnd();
    } // UI Rendering


    m_ArcEngine.RenderEnd();
}

void Framework::OnApplicationPause()
{
}

void Framework::OnCheckForDisabled()
{
}
// Update

#pragma endregion

#pragma region FrameworkEnd

// Quit
void Framework::OnQuit()
{
}

void Framework::OnDisable()
{
}

void Framework::OnDestroy()
{
    //auto [data, out] = zpp::bits::data_out();

    //graphics->Destroy();

    //ApplicationSpecification specs;

    //SDL_GetWindowSize(window->GetNativeWindow(), &specs.windowSizeX, &specs.windowSizeY);

    //auto result = out(specs);

    //if (failure(result)) {
    //	// `result` is implicitly convertible to `std::errc`.
    //	// handle the error or return/throw exception.
    //	fmt::println("Error");
    //}
    //else
    //{
    //	std::ofstream outfile("AppTest.Scroll", std::ios::out | std::ios::binary);
    //	outfile.write(reinterpret_cast<char*>(data.data()), static_cast<long>(data.size()));
    //}

    //window->Destroy();
    //window.reset();
}
// Quit

#pragma endregion

bool Framework::IsQuitting()
{
    return bQuit;
    /*return glfwWindowShouldClose(window->GetNativeWindow());*/
}

auto Serialize(const ApplicationSpecification& person) -> zpp::bits::members<3>
{
    return zpp::bits::members<3>();
}