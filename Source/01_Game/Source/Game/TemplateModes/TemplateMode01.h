#pragma once

#include <Framework/Modes/IMode.h>
#include <Framework/Managers/SceneManager.h>

class TemplateMode01 : public IMode
{
public:
    void Create (Coordinator& _Coordinator)  override;
    void Destroy(Coordinator& _Coordinator) override;

public:
    void RegisterSystems  (Coordinator& _Coordinator) override;
    void UnregisterSystems(const Coordinator& _Coordinator) override;

private:
    SceneManager m_SceneManager;
};