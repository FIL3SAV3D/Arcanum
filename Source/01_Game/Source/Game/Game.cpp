#include "Game.h"

#include "TemplateModes/TemplateMode01.h"

void Game::Create()
{
    if (!m_EditorPiloted)
    {
        m_Framework = std::make_shared<Framework>();
        m_Framework->Create();
    }
}

void Game::OnEnter()
{
    m_Framework->RegisterMode<TemplateMode01>("TemplateMode01");
    m_Framework->SwitchMode("TemplateMode01");

    m_Framework->Start();
}

void Game::Run()
{
    m_Framework->Run();
}

void Game::OnExit()
{

}

void Game::Destroy()
{
    if (!m_EditorPiloted)
    {
        m_Framework->Destroy();
        m_Framework.reset();
    }
}

void Game::SetFramework(const std::shared_ptr<Framework> _Framework)
{
    m_EditorPiloted = true;
    m_Framework = _Framework;
}