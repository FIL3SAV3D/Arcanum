#include "Editor.h"

void Editor::Create()
{
    m_Framework = std::make_shared<Framework>();
    m_Framework->Create();

    m_Game = std::make_unique<Game>();
    m_Game->SetFramework(m_Framework);
    m_Game->Create();
}

void Editor::Run()
{

}

void Editor::Destroy()
{
    m_Game->Destroy();
    m_Game.reset();

    m_Framework->Destroy();
    m_Framework.reset();
}