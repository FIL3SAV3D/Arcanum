#pragma once

#include <memory>

class Coordinator;
class Engine;
union SDL_Event;

struct StartState
{
    std::shared_ptr<Coordinator> coordinator;
};

struct InputState
{
    std::shared_ptr<Coordinator> coordinator;
    SDL_Event* event;
};

struct GameState
{
    std::shared_ptr<Coordinator> coordinator;
    std::shared_ptr<Engine> engine;
};

struct EndState
{
    std::shared_ptr<Coordinator> coordinator;
};

struct RenderState
{
    std::shared_ptr<Coordinator> coordinator;
    std::shared_ptr<Engine> engine;
};