#pragma once

#include "ArcEngine/Asset/Types/Model.h"
#include "ArcEngine/Asset/Types/Mesh.h"
#include "ArcEngine/Rendering/OpenGL-4.6/Camera.h"
#include "ArcEngine/Rendering/OpenGL-4.6/Shader.h"

#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>

#include <memory>

#include "ArcEngine/Platform/Window.h"

struct RenderParams
{
    Camera* camera;
    Shader* shader;
};

class IRenderer
{
public:
    virtual void Create (std::shared_ptr<ArcEngine::Window> _Window) = 0;
    virtual void Destroy() = 0;

    virtual void BeginRender(const RenderParams& _RParams) const = 0;
    virtual void EndRender  (const RenderParams& _RParams) const = 0;

    virtual void RenderMesh         (const RenderParams& _RParams, const Mesh& _Mesh, const int& _SubMeshIndex, const glm::mat4& objectToWorld) = 0;
    virtual void RenderMeshInstanced(const RenderParams& _RParams, const Mesh& _Mesh, const int& _SubMeshIndex, std::vector<glm::mat4> _InstanceData, const int& _InstanceCount) = 0;

    virtual void Blit() const = 0;
    virtual void Resize(std::shared_ptr<ArcEngine::Window> _Window) = 0;
};