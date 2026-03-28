#pragma once

#include <Framework/Modes/IMode.h>

class EditorMode : public IMode
{
public:
    void Create (std::shared_ptr<Coordinator> _Coordinator, std::shared_ptr<SceneManager> _SceneManager) override;
    void Destroy(std::shared_ptr<Coordinator> _Coordinator, std::shared_ptr<SceneManager> _SceneManager) override;

public:
    void RegisterSystems  (std::shared_ptr<Coordinator> _Coordinator) override;
    void UnregisterSystems(std::shared_ptr<Coordinator> _Coordinator) override;
};