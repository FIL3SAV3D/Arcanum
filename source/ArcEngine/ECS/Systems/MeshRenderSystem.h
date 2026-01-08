#pragma once

#include <memory>

#include "ECS/Interfaces/ISystem.h"
#include <OpenGL/Camera.h>

#include <OpenGL/Window.h>

class IRenderer;

class MeshRenderSystem : public ISystem
{
public:
    enum RENDER_TYPE : char
    {
        FORWARD,
        DEFFERED,
        FORWARDPLUS,
        DEFFEREDPLUS,
    };


public:
    MeshRenderSystem(std::shared_ptr<Window> _window, const RENDER_TYPE& _type);
    ~MeshRenderSystem();

    void OnCreate() override;
    void OnUpdate(const float& _deltaTime) override;
    void OnDestroy() override;

private:
    std::shared_ptr<IRenderer> renderer;

    std::shared_ptr<Window> window;

    std::shared_ptr<Camera> camera;
};