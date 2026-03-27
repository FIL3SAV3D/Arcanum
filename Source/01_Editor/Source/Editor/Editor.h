#pragma once

#include <memory>

class Game;
class Framework;

class Editor
{
public:
    Editor()  = default;
    ~Editor() = default;

    void Create();
    void Destroy();

    void Run();

    bool IsQuitting();

private:
    std::shared_ptr<Game> m_Game = nullptr;
    std::shared_ptr<Framework> m_Framework;
};