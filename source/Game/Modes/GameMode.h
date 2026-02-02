#pragma once

#include "Modes/IMode.h"

class GameMode : public IMode
{
public:
    GameMode();
    ~GameMode();

public:
    void OnResize(const uint32_t _width, const uint32_t _height) override;

    void OnCreate() override;
    void OnDestroy() override;
    void Update(const float& _delta_time) override;
    void OnEnter() override;
    void OnExit() override;
};