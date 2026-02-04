#pragma once

class IScene
{
public:
    IScene() {};
    virtual ~IScene() {};

    virtual void OnStart() = 0;

    virtual void OnUpdate(const float& _DeltaTime) = 0;
    virtual void OnLateUpdate(const float& _DeltaTime) = 0;

    virtual void OnQuit() = 0;
};