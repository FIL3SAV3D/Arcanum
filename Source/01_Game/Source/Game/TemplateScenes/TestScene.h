#pragma once

#include <Framework/Scenes/IScene.h>

class TestScene : public IScene
{
public:
    void Create (std::shared_ptr<Coordinator> _Coordinator) override;
    void Destroy(std::shared_ptr<Coordinator> _Coordinator) override;
};