#include "Editor.h"

#include <Framework/Framework.h>
#include <Game/Game.h>

void Editor::Create()
{
    m_Framework = std::make_shared<Framework>();
    m_Framework->Create();

    m_Game = std::make_unique<Game>();
    m_Game->SetFramework(m_Framework);
    m_Game->Create();
    m_Game->OnEnter();
}

void Editor::Run()
{
    m_Game->Run();
}

bool Editor::IsQuitting()
{
    return m_Game->IsQuitting();
}

void Editor::Destroy()
{
    m_Game->OnExit();

    m_Game->Destroy();
    m_Game.reset();

    m_Framework->Destroy();
    m_Framework.reset();
}