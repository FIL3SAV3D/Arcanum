#pragma once

#include "Framework/Scenes/IScene.h"

class TestScene : public IScene
{
public:
    void Create () override;
    void Destroy() override;
};