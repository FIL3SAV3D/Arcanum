#pragma once

#include "ArcEngine/Asset/Types/Model.h"
#include "ArcEngine/Asset/Types/Mesh.h"
#include "ArcEngine/OpenGL/Camera.h"
#include "ArcEngine/OpenGL/Shader.h"

#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>

#include <memory>

#include "ArcEngine/OpenGL/Window.h"

struct RenderParams
{
    Camera* camera;
    Shader* shader;
};

class IRenderer
{
public:
    virtual void Create (std::shared_ptr<Window> _Window) = 0;
    virtual void Destroy() = 0;

    // Start Pass Functions
    virtual void BeginRender(const RenderParams& _Params) const = 0;

    // Main Pass Functions
    virtual void RenderMesh(const RenderParams& _RParams, const Mesh& _Mesh, const int& _SubMeshIndex, const glm::mat4& objectToWorld) = 0;
    virtual void RenderMeshInstanced(const RenderParams& _RParams, const Mesh& _Mesh, const int& _SubMeshIndex, std::vector<glm::mat4> _InstanceData, const int& _InstanceCount) = 0;

    // End Pass Functions
    virtual void EndRender(const RenderParams& _RParams) const = 0;

    // Helper Functions
    virtual void Blit() const = 0;

    // Callback Functions
    virtual void Resize(std::shared_ptr<Window> _Window) const = 0;
};