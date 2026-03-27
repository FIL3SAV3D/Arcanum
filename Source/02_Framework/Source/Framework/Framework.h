#pragma once

#include "States/States.h"
#include <memory>
#include <string>
#include "Managers/ModeManager.h"

class Engine;
class SceneManager;
class AssetManager;
class Coordinator;
class Clock;

struct ApplicationSpecification
{
    std::string name{};
    int windowSizeX{};
    int windowSizeY{};
    bool fullscreen = false;
};

struct GLFWwindow;

class Framework
{
public:
    Framework()  = default;
    ~Framework() = default;

    void Create();
    void Destroy();

    template<typename T>
    void RegisterMode(const std::string& _Name)
    {
        modeManger->RegisterMode<T>(_Name, coordinator);
    }

    void SwitchMode(const std::string& _Name)
    {
        modeManger->SwitchMode(_Name, coordinator, sceneManager);
    }

    void Start();

    void Run();

    bool IsQuitting();

private:
    void HandleInput();
    void HandleUpdate();
    void HandleRender();

public:
    bool bQuit = false;
    bool stop_rendering = false;

private:
    StartState  startState  = {};
    InputState  inputState  = {};
    GameState   gameState   = {};
    RenderState renderState = {};
    EndState    endState    = {};

private:
    std::shared_ptr<ModeManager> modeManger;
    std::shared_ptr<SceneManager> sceneManager;
    std::shared_ptr<AssetManager> assetManager;

    std::shared_ptr<Coordinator> coordinator;

    std::shared_ptr<Clock> clock;

    std::shared_ptr<Engine> arcEngine;
};