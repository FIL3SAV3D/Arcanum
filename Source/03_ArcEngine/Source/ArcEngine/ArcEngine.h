#pragma once

#include "ArcEngine/Platform/Window.h"
#include "ArcEngine/Graphics/Graphics.h"
#include "ArcEngine/Graphics/UIGraphics.h"

#include "ArcEngine/Asset/Types/Model.h"
#include "Graphics/CameraData.h"

class Engine
{
public:
    Engine() = default;
    ~Engine() = default;

    void Create();
    void Destroy();

    void UpdateCameraData(const glm::vec4& _Position, const glm::mat4& _View, const glm::mat4& _Projection);

    void RenderMesh(std::shared_ptr<Model> _Model, const glm::mat4x4& _ObjectToWorld);

    void RenderStart();
    void RenderEnd();

    void RenderUIStart();
    void RenderUIEnd();

    const glm::uvec2 GetWindowSize();
    void ResizeSwapchain(const glm::uvec2& _Size);
    void SwapFrame();

private:
    ArcEngine::Window m_Window;
    ArcEngine::Graphics m_Graphics;
    ArcEngine::UIGraphics m_UIGraphics;
};