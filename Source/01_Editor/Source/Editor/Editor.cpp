#include "Editor.h"

#include <Framework/Framework.h>
#include <Game/Game.h>

#include "Modes/EditorMode.h"

void Editor::Create()
{
    m_Framework = std::make_shared<Framework>();
    m_Framework->Create();

    m_Framework->RegisterMode<EditorMode>("Editor");

    m_Game = std::make_unique<Game>();
    m_Game->SetFramework(m_Framework);
}

void Editor::Run()
{
    if (isPlaying)
    {
        m_Game->Run();
    }
    m_Framework->Run();
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