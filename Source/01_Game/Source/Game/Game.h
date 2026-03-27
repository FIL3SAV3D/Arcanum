#pragma once

#include <memory>

class Framework;

class Game
{
public:
    Game()  = default;
    ~Game() = default;

    void Create();
    void Destroy();

    void OnEnter();

    void Run();

    void OnExit();

    bool IsQuitting();

protected:
    void SetFramework(const std::shared_ptr<Framework> _Framework);

private:
    bool m_EditorPiloted = false;
    std::shared_ptr<Framework> m_Framework;

private:
    friend class Editor;
};