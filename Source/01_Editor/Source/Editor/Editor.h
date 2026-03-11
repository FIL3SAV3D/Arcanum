#pragma once

#include <Game/Game.h>
#include <Framework/Framework.h>

#include <memory>

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
    std::unique_ptr<Game> m_Game;
    std::shared_ptr<Framework> m_Framework;
};